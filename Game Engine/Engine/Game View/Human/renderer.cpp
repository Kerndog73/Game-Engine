//
//  renderer.cpp
//  Game Engine
//
//  Created by Indi Kernick on 19/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "renderer.hpp"

const size_t UI::Renderer::ESTIMATE_NUM_ELEMENTS = 64;
//MAX_HEIGHT should be a power of two
const Ogre::Real UI::Renderer::MAX_HEIGHT = 1024;
//MAX_TEXTURES should be a power of two
const Ogre::Real UI::Renderer::MAX_TEXTURES = 16;

UI::Renderer::Renderer(
  std::weak_ptr<Platform::Window> window,
  Ogre::Viewport *viewport,
  Ogre::SceneManager *sceneManager
) : window(window), viewport(viewport) {
  assert(viewport);
  assert(sceneManager);
  
  node = sceneManager->getRootSceneNode()->createChildSceneNode();
  manualObject = sceneManager->createManualObject();
  node->attachObject(manualObject);
  manualObject->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_OVERLAY);
  
  manualObject->estimateVertexCount(4 * ESTIMATE_NUM_ELEMENTS);
  manualObject->estimateIndexCount(6 * ESTIMATE_NUM_ELEMENTS);
  manualObject->setDynamic(true);
  manualObject->setUseIdentityProjection(true);
  manualObject->setUseIdentityView(true);
  manualObject->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
  manualObject->setKeepDeclarationOrder(true);
  
  Ogre::Root::getSingleton().addFrameListener(this);
}

UI::Renderer::~Renderer() {
  Ogre::Root::getSingleton().removeFrameListener(this);
}

void UI::Renderer::setRoot(
  Element::Ptr newRoot,
  const std::string &matName,
  const std::string &atlasName
) {
  assert(newRoot);
  root = newRoot;
  defaultMaterial = matName;
  atlas = getAtlas(atlasName);
  assert(atlas->getType() == Res::TextureAtlas::Type::IMAGE);
  manualObject->clear();
  addSection(defaultMaterial);
}

void UI::Renderer::unSetRoot() {
  root = nullptr;
}

UI::Renderer::Quad::Quad(
  Bounds bounds,
  TexCoords texCoords,
  UI::Color color,
  UI::Height height,
  const UI::Trans2D &newTransform
) : texCoords(texCoords),
    color(color.r, color.g, color.b, color.a),
    depth(1.0f - static_cast<Ogre::Real>(height) / MAX_HEIGHT) {
  const Trans2D posSize = {
    bounds.s.x, 0.0f,       bounds.p.x,
    0.0f,       bounds.s.y, bounds.p.y,
    0.0f,       0.0f,       1.0f
  };
  const Trans2D space = {
    2.0f,  0.0f, -1.0f,
    0.0f, -2.0f,  1.0f,
    0.0f,  0.0f,  1.0f
  };
  transform = posSize * newTransform * space;
}

bool UI::Renderer::frameStarted(const Ogre::FrameEvent &) {
  PROFILE(UI::Renderer::frameStarted);

  if (root == nullptr || atlas.get() == nullptr) {
    return true;
  }
  
  AABBStack aabbStack(getWindowAspectRatio());
  HeightStack heightStack;
  Groups groups;
  groups.push_back({{}, defaultMaterial, false});
  {
    //invoking the profiler in a recursive function is not a good idea!
    PROFILE(UI::Renderer::fillGroups);
    fillGroups(aabbStack, root, heightStack, groups);
  }
  sortGroups(groups);
  fillManualObject(groups);
  
  return true;
}

UI::Coord UI::Renderer::getWindowAspectRatio() const {
  return static_cast<Coord>(viewport->getActualWidth()) /
         viewport->getActualHeight();
}

UI::PointPx UI::Renderer::getWindowSize() const {
  return {viewport->getActualWidth(), viewport->getActualHeight()};
}

void UI::Renderer::fillGroups(
  UI::AABBStack &aabbStack,
  const UI::Element::Ptr element,
  UI::HeightStack &heightStack,
  Groups &groups
) {
  aabbStack.push(element->getBounds());
  heightStack.push(element->getHeight());
  
  if (element->isVisible()) {
    if (const Paragraph::Ptr paragraph = std::dynamic_pointer_cast<Paragraph>(element)) {
      if (paragraph->getFont().size() && paragraph->getText().size()) {
        groups.push_back({{}, paragraph->getFont(), true});
        renderParagraph(
          paragraph,
          aabbStack.top(),
          heightStack.top(),
          groups.back().quads
        );
        groups.push_back({{}, defaultMaterial, false});
      }
    } else {
      const Textures &textures = element->getTextures();
      assert(textures.size() < MAX_TEXTURES);
      if (textures.size() == 0) {
        assert(atlas->hasWhitepixel());
        groups.back().quads.emplace_back(
          aabbStack.top(),
          atlas->getWhitepixel(),
          element->getColor(),
          heightStack.top(),
          Trans2D()
        );
      }
      for (size_t t = 0; t != textures.size(); t++) {
        assert(textures[t].path.size());
        groups.back().quads.emplace_back(
          aabbStack.top(),
          atlas->getSprite(textures[t].path),
          element->getColor(),
          heightStack.top() + t / MAX_TEXTURES,
          textures[t].transform
        );
      }
    }
  }
  
  element->forEachChild([this, &aabbStack, &heightStack, &groups] (const Element::Ptr child) {
    fillGroups(aabbStack, child, heightStack, groups);
  });
  
  heightStack.pop();
  aabbStack.pop();
}

///Crop the given quad so that it fits in within the bounds.
///Returns false if the quad is completly outside of the bounds
bool UI::Renderer::cropQuadBounds(
  const BoundsPx bounds,
  const PointPx texSize,
  BoundsPx &quadBounds,
  TexCoords &texCoords
) {
  if (bounds.encloses(quadBounds)) {
    return true;
  }
  if (!bounds.interceptsWith(quadBounds)) {
    return false;
  }
  
  TexCoordsPx texCoordsPx = toPixels(texCoords, texSize);
  TexCoordsPx quadPoints = static_cast<TexCoordsPx>(quadBounds);
  
  const CoordPx left   = std::max(0, bounds.left()     - quadPoints.left);
  const CoordPx top    = std::max(0, bounds.top()      - quadPoints.top );
  const CoordPx right  = std::max(0, quadPoints.right  - bounds.right() );
  const CoordPx bottom = std::max(0, quadPoints.bottom - bounds.bottom());
  
  quadPoints.left    += left;
  texCoordsPx.left   += left;
  quadPoints.top     += top;
  texCoordsPx.top    += top;
  quadPoints.right   -= right;
  texCoordsPx.right  -= right;
  quadPoints.bottom  -= bottom;
  texCoordsPx.bottom -= bottom;
  
  quadBounds = static_cast<BoundsPx>(quadPoints);
  texCoords = fromPixels(texCoordsPx, texSize);
  
  return true;
}

Res::TextureAtlas::Ptr UI::Renderer::getAtlas(const std::string &name) {
  Res::TextureAtlas::Ptr loadedAtlas = Res::TextureAtlasManager::getSingleton().load(
    name + ".atlas",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
  ).dynamicCast<Res::TextureAtlas>();
  assert(loadedAtlas.get() != nullptr);
  return loadedAtlas;
}

const Res::TextureAtlas::FontFace &UI::Renderer::getFontFace(const std::string &font, FontSize size) {
  return getFontFace(getAtlas(font), size);
}

const Res::TextureAtlas::FontFace &UI::Renderer::getFontFace(const Res::TextureAtlas::Ptr fontAtlas, FontSize size) {
  assert(fontAtlas->getType() == Res::TextureAtlas::Type::FONT);
  return fontAtlas->getFontFace(size);
}

void UI::Renderer::renderText(
  const Res::TextureAtlas::FontFace &face,
  const PointPx texSize,
  const TextInfo &textInfo,
  const BoundsPx bounds,
  Quads &quads
) {
  PROFILE(UI::Renderer::renderText);

  const Res::TextureAtlas::FontMetrics fontMetrics = face.getFontMetrics();
  const PointPx windowSize = getWindowSize();
  PointPx origin = textInfo.pos;
  origin.y += fontMetrics.maxY;
  
  for (auto c = textInfo.text.cbegin(); c != textInfo.text.cend(); c++) {
    if (*c == '\n') {
      origin.x = bounds.left();
      origin.y += fontMetrics.lineHeight;
      continue;
    }
    const Res::TextureAtlas::Glyph glyph = face.getGlyph(*c);
    if (Res::TextureAtlas::isZero(glyph)) {
      continue;
    }
    BoundsPx glyphBounds = {
      {
        origin.x + glyph.metrics.bearing.x,
        origin.y - glyph.metrics.bearing.y
      },
      glyph.metrics.size
    };
    TexCoords texCoords = glyph.glyph;
    if (cropQuadBounds(bounds, texSize, glyphBounds, texCoords)) {
      quads.emplace_back(
        fromPixels(glyphBounds, windowSize),
        texCoords,
        textInfo.color,
        textInfo.height,
        Trans2D()
      );
    }
    origin.x += glyph.metrics.advance;
    if (c + 1 != textInfo.text.cend()) {
      origin.x += face.getKerning(*c, *(c+1));
    }
  }
}

class TextPos {
public:
  TextPos(const UI::Paragraph::Align alignment,
          const UI::BoundsPx bounds,
          const UI::CoordPx lineHeight)
    : lineHeight(lineHeight),
      bounds(bounds),
      alignment(alignment) {}
  
  void beginWord() {
    wordWidth = 0;
  }
  
  void endWord() {
    lastWidth = currentWidth;
  }
  
  void beginLine() {
    currentWidth = 0;
    lastWidth = 0;
    lineNumber++;
  }
  
  void beginLineWithWord() {
    currentWidth = wordWidth;
    lineNumber++;
  }
  
  void advance(const UI::CoordPx offset) {
    currentWidth += offset;
    wordWidth += offset;
  }
  
  bool needToWrap() const {
    return currentWidth > bounds.s.x;
  }
  
  ///Get the position of this line
  UI::PointPx linePos() const {
    return {
      calcAlign(currentWidth) + bounds.p.x,
      lineNumber * lineHeight + bounds.p.y
    };
  }
  
  ///Get the position of the words preceding this one
  UI::PointPx prevWordsPos() const {
    return {
      calcAlign(lastWidth)    + bounds.p.x,
      lineNumber * lineHeight + bounds.p.y
    };
  }
  
private:
  //the sum of the advance of the characters preceding this word
  UI::CoordPx lastWidth = 0;
  //the sum of the advance of the characters preceding this character
  UI::CoordPx currentWidth = 0;
  //the sum of the advance of the current word
  UI::CoordPx wordWidth = 0;
  UI::CoordPx lineNumber = 0;
  const UI::CoordPx lineHeight;
  const UI::BoundsPx bounds;
  const UI::Paragraph::Align alignment;
  
  UI::CoordPx calcAlign(const UI::CoordPx innerWidth) const {
    switch (alignment) {
      case UI::Paragraph::Align::LEFT:
        return 0;
      case UI::Paragraph::Align::CENTER:
        return (bounds.s.x - innerWidth) / 2;
      case UI::Paragraph::Align::RIGHT:
        return bounds.s.x - innerWidth;
    }
  }
};

class Line {
public:
  using View = std::experimental::string_view;
  using ViewIter = View::const_iterator;

  explicit Line(const ViewIter begin)
    : lineBegin(begin), wordBegin(begin) {}
  
  void beginWord(const ViewIter begin) {
    wordBegin = begin;
  }
  
  ///End the previous line and begin a new line
  View endAndBeginLine(const ViewIter end) {
    const View view = {lineBegin, static_cast<size_t>(end - lineBegin)};
    lineBegin = end;
    return view;
  }
  
  ///End the previous line and begin a new line with a word
  View endAndBeginLineWithWord() {
    const View view = {lineBegin, static_cast<size_t>(wordBegin - lineBegin)};
    lineBegin = wordBegin;
    return view;
  }
  
  bool lineIsOneWord() const {
    return lineBegin == wordBegin;
  }
  
private:
  ViewIter lineBegin;
  ViewIter wordBegin;
};

void UI::Renderer::renderParagraph(
  const Paragraph::Ptr paragraph,
  const Bounds bounds,
  const Height height,
  Quads &quads
) {
  PROFILE(UI::Renderer::renderParagraph);
  
  #define RENDER_LINE(textRange, position) \
    textInfo.text = textRange; \
    textInfo.pos = position; \
    renderText(face, texSize, textInfo, boundsPx, quads)
  
  const Res::TextureAtlas::Ptr fontAtlas = getAtlas(paragraph->getFont());
  const Res::TextureAtlas::FontFace &face = getFontFace(fontAtlas, paragraph->getFontSize());
  const PointPx texSize = fontAtlas->getTextureSize();
  const BoundsPx boundsPx = toPixels(bounds, getWindowSize());
  const bool wrap = paragraph->getWrap();
  
  //we could bypass all of this if the paragraph is left-aligned and no-wrap
  //but is there any point?
  
  TextInfo textInfo;
  textInfo.color = paragraph->getColor();
  textInfo.height = height;
  
  TextPos textPos(
    paragraph->getAlign(),
    boundsPx,
    face.getFontMetrics().lineHeight
  );
  std::experimental::string_view text = paragraph->getText();
  
  Line line(text.cbegin());
  
  for (auto c = text.cbegin(); c != text.cend() + 1; c++) {
    const char prevChar = c == text.cbegin()   ? 0 : *(c - 1);
    const char thisChar = c == text.cend()     ? 0 : *c;
    const char nextChar = c + 1 >= text.cend() ? 0 : *(c + 1);
    
    if (thisChar == '\n') {
      RENDER_LINE(line.endAndBeginLine(c + 1), textPos.linePos());
      textPos.beginLine();
    }
    if (std::isalpha(prevChar) && !std::isalpha(thisChar)) {
      if (wrap && textPos.needToWrap()) {
        if (line.lineIsOneWord()) {
          RENDER_LINE(line.endAndBeginLine(c + 1), textPos.linePos());
          textPos.beginLine();
        } else {
          RENDER_LINE(line.endAndBeginLineWithWord(), textPos.prevWordsPos());
          textPos.beginLineWithWord();
        }
      }
      textPos.endWord();
    }
    
    if (!std::isalpha(prevChar) && std::isalpha(thisChar)) {
      line.beginWord(c);
      textPos.beginWord();
    }
    
    textPos.advance(
      face.getGlyph(thisChar).metrics.advance +
      face.getKerning(thisChar, nextChar)
    );
  }
  
  RENDER_LINE(line.endAndBeginLine(text.cend()), textPos.linePos());
  
  #undef RENDER_LINE
}

//Sort the quads from deepest to heighest
void UI::Renderer::sortQuads(Quads &quads) {
  PROFILE(UI::Renderer::sortQuads);
  
  std::sort(
    quads.begin(),
    quads.end(),
    [] (const Quad &a, const Quad &b) {
      return a.depth > b.depth;
    }
  );
}

UI::Renderer::GroupPtrsPair UI::Renderer::partionGroups(Groups &groups) {
  PROFILE(UI::Renderer::partionGroups);

  GroupPtrsPair pair;
  
  for (auto g = groups.begin(); g != groups.end(); g++) {
    if (g->quads.size()) {
      if (g->sameDepth) {
        pair.text.push_back(&(*g));
      } else {
        pair.quad.push_back(&(*g));
      }
    }
  }
  
  return pair;
}

void UI::Renderer::sortGroupPair(GroupPtrsPair &pair) {
  PROFILE(UI::Renderer::sortGroupPair);

  for (auto g = pair.quad.begin(); g != pair.quad.end(); g++) {
    sortQuads((*g)->quads);
  }
  
  std::sort(pair.text.begin(), pair.text.end(), [](Group *a, Group *b) {
    return a->quads[0].depth > b->quads[0].depth;
  });
}

UI::Renderer::QuadIters UI::Renderer::getDeepestQuadIters(const GroupPtrs &quadGroups) {
  PROFILE(UI::Renderer::getDeepestQuadIters);

  QuadIters deepestQuads;
  for (auto g = quadGroups.cbegin(); g != quadGroups.cend(); g++) {
    deepestQuads.push_back((*g)->quads.cbegin());
  }
  return deepestQuads;
}

//Search the groups for the deepest quad
UI::Renderer::QuadIterRef UI::Renderer::getDeepestQuad(
  std::vector<QuadIter> &deepestQuads,
  const GroupPtrs &quadGroups
) {
  PROFILE(UI::Renderer::getDeepestQuad);

  /*
  deepestQuads stores an iterator for each group. The iterator points to the
  next deepest quad. This function compares the depth of all the quads pointed
  to by the iterators and returns a reference to the iterator of the deepest one.
  
  This is kind of like merge sort with a variable number of input arrays
  */

  QuadIterRef deepestQuad;
  for (size_t g = 0; g != quadGroups.size(); g++) {
    if (deepestQuads[g] == quadGroups[g]->quads.cend()) {
      continue;
    } else if (!deepestQuad.valid || deepestQuads[g]->depth >= deepestQuad.iter->depth) {
      deepestQuad.iter = deepestQuads[g];
      deepestQuad.groupIndex = g;
      deepestQuad.valid = true;
    }
  }
  return deepestQuad;
}

void UI::Renderer::sortGroups(Groups &groups) {
  PROFILE(UI::Renderer::sortGroups);

  if (groups.size() == 1) {
    if (!groups[0].sameDepth) {
      sortQuads(groups[0].quads);
    }
    return;
  } else if (groups.size() == 0) {
    return;
  }

  GroupPtrsPair pair = partionGroups(groups);
  sortGroupPair(pair);
  
  //the deepest text group
  GroupPtrs::const_iterator textGroup = pair.text.cbegin();
  //the deepest quads in the quad groups
  QuadIters deepestQuads = getDeepestQuadIters(pair.quad);
  Groups out;
  
  while (true) {
    QuadIterRef deepestQuad = getDeepestQuad(deepestQuads, pair.quad);
    
    //if there are no more quads then push the text if there is any and return
    if (!deepestQuad.valid) {
      while (textGroup != pair.text.cend()) {
        out.emplace_back(std::move(*(*textGroup)));
        textGroup++;
      }
      groups = std::move(out);
      return;
    } else {
      //if the deepestQuad is deeper than the text then push
      if (textGroup == pair.text.cend() || deepestQuad.iter->depth >= (*textGroup)->quads[0].depth) {
        if (out.size() == 0 || out.back().sameDepth) {
          out.push_back({{*deepestQuad.iter}, defaultMaterial, false});
        } else {
          out.back().quads.push_back(*deepestQuad.iter);
        }
        //we only increment the iterator if the quad is used
        deepestQuads[deepestQuad.groupIndex]++;
      //otherwise push the text
      } else {
        out.emplace_back(std::move(*(*textGroup)));
        textGroup++;
      }
    }
  }
}

void UI::Renderer::writeQuad(
  const UI::Renderer::Quad &quad,
  Ogre::uint32 i
) {
  PROFILE(UI::Renderer::writeQuad);

  static constexpr Trans2D::col_type TOP_LEFT     = {0.0f, 0.0f, 1.0f};
  static constexpr Trans2D::col_type BOTTOM_LEFT  = {0.0f, 1.0f, 1.0f};
  static constexpr Trans2D::col_type BOTTOM_RIGHT = {1.0f, 1.0f, 1.0f};
  static constexpr Trans2D::col_type TOP_RIGHT    = {1.0f, 0.0f, 1.0f};

  //top left
  const Point topLeft = TOP_LEFT * quad.transform;
  manualObject->position(topLeft.x, topLeft.y, quad.depth);
  manualObject->textureCoord(quad.texCoords.left, quad.texCoords.top);
  manualObject->colour(quad.color);
  //bottom left
  const Point bottomLeft = BOTTOM_LEFT * quad.transform;
  manualObject->position(bottomLeft.x, bottomLeft.y, quad.depth);
  manualObject->textureCoord(quad.texCoords.left, quad.texCoords.bottom);
  manualObject->colour(quad.color);
  //bottom right
  const Point bottomRight = BOTTOM_RIGHT * quad.transform;
  manualObject->position(bottomRight.x, bottomRight.y, quad.depth);
  manualObject->textureCoord(quad.texCoords.right, quad.texCoords.bottom);
  manualObject->colour(quad.color);
  //top right
  const Point topRight = TOP_RIGHT * quad.transform;
  manualObject->position(topRight.x, topRight.y, quad.depth);
  manualObject->textureCoord(quad.texCoords.right, quad.texCoords.top);
  manualObject->colour(quad.color);
  
  manualObject->quad(i, i + 1, i + 2, i + 3);
}

void UI::Renderer::addSection(const Ogre::String &material) {
  PROFILE(UI::Renderer::addSection);

  manualObject->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
  manualObject->position(1.0f, 1.0f, 0.0f);
  manualObject->textureCoord(1.0f, 0.0f);
  manualObject->colour(1.0f, 1.0f, 1.0f, 1.0f);
 
  manualObject->position(-1.0f, 1.0f, 0.0f);
  manualObject->textureCoord(0.0f, 0.0f);
  manualObject->colour(1.0f, 1.0f, 1.0f, 1.0f);
  
  manualObject->position(-1.0f, -1.0f, 0.0f);
  manualObject->textureCoord(0.0f, 1.0f);
  manualObject->colour(1.0f, 1.0f, 1.0f, 1.0f);
  
  manualObject->triangle(0, 1, 2);
  manualObject->end();
}

void UI::Renderer::fillManualObject(const Groups &groups) {
  PROFILE(UI::Renderer::fillManualObject);

  if (groups.size() == 0) {
    return;
  }
  
  const size_t extraSection = groups[0].sameDepth;
  size_t numSections = manualObject->getNumSections();
  
  //if the materials mismatch, clear
  if (numSections > 1) {
    const Ogre::uint32 numSections32 = static_cast<Ogre::uint32>(numSections);
    for (Ogre::uint32 s = 1; s < numSections32; s += 2) {
      if (manualObject->getSection(s)->getMaterialName() != groups[s - extraSection].material) {
        manualObject->clear();
        numSections = 0;
        break;
      }
    }
  }
  
  //make sure there are enough sections
  while (numSections + extraSection < groups.size()) {
    if (numSections % 2) {
      addSection(groups[numSections - extraSection].material);
    } else {
      addSection(defaultMaterial);
    }
    numSections++;
  }
  
  size_t nextSection = 0;
  
  if (extraSection) {
    manualObject->beginUpdate(nextSection++);
    manualObject->end();
  }
  
  for (auto g = groups.cbegin(); g != groups.cend(); g++, nextSection++) {
    manualObject->beginUpdate(nextSection);
    Ogre::uint32 i = 0;
    for (auto q = g->quads.cbegin(); q != g->quads.cend(); q++, i += 4) {
      writeQuad(*q, i);
    }
    manualObject->end();
  }
}
