//
//  color.cpp
//  Game Engine
//
//  Created by Indi Kernick on 16/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "color.hpp"

IColor::IColor(int r, int g, int b, int a)
  : r(r), g(g), b(b), a(a) {}

IColor::IColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  : r(r), g(g), b(b), a(a) {}

IColor::IColor(float r, float g, float b, float a)
  : r(Math::clamp(std::round(r * 255), 0.0f, 255.0f)),
    g(Math::clamp(std::round(g * 255), 0.0f, 255.0f)),
    b(Math::clamp(std::round(b * 255), 0.0f, 255.0f)),
    a(Math::clamp(std::round(a * 255), 0.0f, 255.0f)) {}

IColor::IColor(const FColor &color)
  : IColor(color.r, color.g, color.b, color.a) {}

IColor::IColor(uint32_t rgba)
  : rgba(rgba) {}

IColor::IColor(uint16_t rgba)
  //each component is 4 bits
  //so i copy the 4 bit component into
  //the upper half of the 8 bit component
  : r((rgba & 0xF000) >> 8),
    g((rgba & 0x0F00) >> 4),
    b((rgba & 0x00F0)),
    a(rgba << 4) {}

IColor::IColor(uint8_t rgba)
  : r(rgba & 0b11000000),
    g((rgba & 0b00110000) << 2),
    b((rgba & 0b00001100) << 4),
    a((rgba & 0b00000011) << 6) {}

bool IColor::operator==(IColor other) const {
  return rgba == other.rgba;
}

bool IColor::operator!=(IColor other) const {
  return rgba != other.rgba;
}

FColor IColor::rgbaFloat() const {
  return FColor(*this);
}

uint16_t IColor::rgba4() const {
  return ((r & 0xF0) << 8) |
         ((g & 0xF0) << 4) |
         (b & 0xF0) |
         (a >> 4);
}

uint8_t IColor::rgba2() const {
  return (r & 0b11000000) |
         ((g & 0b11000000) >> 2) |
         ((b & 0b11000000) >> 4) |
         ((a & 0b11000000) >> 8);
}

FColor::FColor(float r, float g, float b, float a)
  : r(r), g(g), b(b), a(a) {}

FColor::FColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}

FColor::FColor(int r, int g, int b, int a)
  : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}

FColor::FColor(IColor color)
  : r(color.r / 255.0f),
    g(color.g / 255.0f),
    b(color.b / 255.0f),
    a(color.a / 255.0f) {}

bool FColor::operator==(const FColor &other) const {
  return r == other.r &&
         g == other.g &&
         b == other.b &&
         a == other.a;
}

bool FColor::operator!=(const FColor &other) const {
  return r != other.r ||
         g != other.g ||
         b != other.b ||
         a != other.a;
}

IColor FColor::rgbaInt() const {
  return IColor(*this);
}

void FColor::clamp() {
  r = Math::clamp(r, 0.0f, 1.0f);
  g = Math::clamp(g, 0.0f, 1.0f);
  b = Math::clamp(b, 0.0f, 1.0f);
  a = Math::clamp(a, 0.0f, 1.0f);
}
