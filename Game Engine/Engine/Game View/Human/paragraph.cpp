//
//  paragraph.cpp
//  Game Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "paragraph.hpp"

UI::Paragraph::Paragraph(const std::string &id)
  : Element(std::make_unique<DefaultNodeImpl<Element>>(id)) {}

void UI::Paragraph::setFont(const std::string &newFont) {
  assert(newFont.size());
  font = newFont;
}

void UI::Paragraph::setText(const std::string &newText) {
  text = newText;
}

void UI::Paragraph::setFontSize(FontSize newFontSize) {
  fontSize = newFontSize;
}

void UI::Paragraph::setAlign(Align newAlign) {
  align = newAlign;
}

void UI::Paragraph::setWrap(bool newWrap) {
  wrap = newWrap;
}

const std::string &UI::Paragraph::getFont() const {
  return font;
}

const std::string &UI::Paragraph::getText() const {
  return text;
}

UI::FontSize UI::Paragraph::getFontSize() const {
  return fontSize;
}

UI::Paragraph::Align UI::Paragraph::getAlign() const {
  return align;
}

bool UI::Paragraph::getWrap() const {
  return wrap;
}
