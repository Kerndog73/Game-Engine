//
//  texture atlas.cpp
//  Game Engine
//
//  Created by Indi Kernick on 21/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "texture atlas.hpp"

Res::InvalidTextureAtlas::InvalidTextureAtlas(const std::string &error)
  : InvalidResource("Texture Atlas", error) {}

namespace YAML {
  template <>
  struct convert<UI::TexCoords> {
    static glm::ivec2 textureSize;
    
    static bool decode(const Node &node, UI::TexCoords &sprite) {
      if (!node.IsSequence() || node.size() != 4) {
        return false;
      }
      
      const UI::TexCoordsPx spritePx = {
        node[0].as<int>(),//left
        node[1].as<int>(),//top
        node[2].as<int>(),//right
        node[3].as<int>() //bottom
      };
      sprite = UI::fromPixels(spritePx, textureSize);
      
      return UI::TexCoords::NORM_0_1.encloses(sprite);
    }
  };
  
  UI::PointPx convert<UI::TexCoords>::textureSize = {0, 0};
  
  template <typename T>
  struct convert<glm::tvec2<T>> {
    static bool decode(const Node &node, glm::tvec2<T> &vector) {
      if (!node.IsSequence() || node.size() != 2) {
        return false;
      }
      
      vector[0] = node[0].as<T>();
      vector[1] = node[1].as<T>();
      
      return true;
    }
  };
  
  #define SET_PROPERTY(prop) \
    if (const Node &prop = node[#prop]) { \
      obj.prop = prop.as<decltype(obj.prop)>(); \
    } else { \
      return false; \
    }
  
  #define SET_NAMED_PROPERTY(prop, propName) \
    if (const Node &prop = node[#propName]) { \
      obj.prop = prop.as<decltype(obj.prop)>(); \
    } else { \
      return false; \
    }
  
  template <>
  struct convert<Res::TextureAtlas::GlyphMetrics> {
    static bool decode(const Node &node, Res::TextureAtlas::GlyphMetrics &obj) {
      if (!node.IsMap()) {
        return false;
      }
      
      SET_PROPERTY(bearing);
      SET_PROPERTY(size);
      SET_PROPERTY(advance);
      
      return true;
    }
  };
  
  template <>
  struct convert<Res::TextureAtlas::FontMetrics> {
    static bool decode(const Node &node, Res::TextureAtlas::FontMetrics &obj) {
      if (!node.IsMap()) {
        return false;
      }
      
      SET_NAMED_PROPERTY(lineHeight, line height);
      SET_NAMED_PROPERTY(minY, min y);
      SET_NAMED_PROPERTY(maxY, max y);
      
      return true;
    }
  };
  
  #undef SET_PROPERTY
  #undef SET_NAMED_PROPERTY
}

#define CHECK_NODE(varName, node) \
  const YAML::Node &varName = node; \
  if (!varName) { \
    throw InvalidTextureAtlas("Part of the texture atlas is missing"); \
  }

void Res::TextureAtlasSerializer::importAtlas(Ogre::DataStreamPtr &stream, TextureAtlas *atlas) {
  assert(stream->isReadable());
  assert(atlas);
  
  try {
    const YAML::Node doc = YAML::Load(stream->getAsString());
    
    CHECK_NODE(texSize, doc["size"]);
    atlas->textureSize = texSize.as<decltype(atlas->textureSize)>();
    if (atlas->textureSize.x <= 0 || atlas->textureSize.y <= 0) {
      throw InvalidTextureAtlas("Atlas has negative size");
    }
    YAML::convert<UI::TexCoords>::textureSize = atlas->textureSize;

    CHECK_NODE(type, doc["type"]);
    const std::string strType = type.as<std::string>();
    if (strType == "image") {
      atlas->type = TextureAtlas::Type::IMAGE;
      importImageAtlas(doc, atlas);
    } else if (strType == "font") {
      atlas->type = TextureAtlas::Type::FONT;
      importFontAtlas(doc, atlas);
    } else {
      throw InvalidTextureAtlas("head.type is invalid");
    }
    
  } catch (YAML::Exception &e) {
    throw InvalidTextureAtlas(e.what());
  }
}

void Res::TextureAtlasSerializer::importImageAtlas(const YAML::Node &doc, Res::TextureAtlas *atlas) {
  CHECK_NODE(images, doc["images"]);
  atlas->sprites = images.as<decltype(atlas->sprites)>();
  if (const YAML::Node whitepixel = doc["whitepixel"]) {
    const UI::Point posPx = whitepixel.as<UI::PointPx>();
    const UI::TexCoordsPx coordsPx(posPx, posPx);
    atlas->whitepixel = UI::fromPixels(coordsPx, atlas->textureSize);
  } else {
    atlas->whitepixel = {-1.0f, -1.0f, -1.0f, -1.0f};
  }
}

void Res::TextureAtlasSerializer::importFontAtlas(const YAML::Node &doc, Res::TextureAtlas *atlas) {
  CHECK_NODE(faces, doc["faces"]);
  for (auto f = faces.begin(); f != faces.end(); f++) {
    const YAML::Node &docFace = *f;
    CHECK_NODE(points, docFace["points"]);
    atlas->faces.insert({points.as<decltype(atlas->faces)::key_type>(), {}});
    Res::TextureAtlas::FontFace &face = atlas->faces.rbegin()->second;
    
    CHECK_NODE(range, docFace["range"]);
    face.begin = static_cast<decltype(face.begin)>(range[0].as<int64_t>());
    face.end = static_cast<decltype(face.end)>(range[1].as<int64_t>());
    CHECK_NODE(fontMetrics, docFace["font metrics"]);
    face.fontMetrics = fontMetrics.as<decltype(face.fontMetrics)>();
    CHECK_NODE(metrics, docFace["glyph metrics"]);
    face.metrics = metrics.as<decltype(face.metrics)>();
    CHECK_NODE(glyphs, docFace["glyphs"]);
    face.glyphs = glyphs.as<decltype(face.glyphs)>();
    CHECK_NODE(hasKerning, docFace["has kerning"]);
    if (hasKerning.as<bool>()) {
      CHECK_NODE(kerning, docFace["kerning"]);
      face.kerning = kerning.as<decltype(face.kerning)>();
    }
  }
}

#undef CHECK_NODE
