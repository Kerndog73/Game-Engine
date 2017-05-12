//
//  write atlas.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

#include <fstream>
#include <iostream>
#include "../Utils/profiler.hpp"
#include "../Utils/write atlas.hpp"

std::string getImageName(const std::string &path) {
  const size_t lastSlash = path.find_last_of('/');
  return path.substr(lastSlash + 1, path.find_last_of('.') - lastSlash - 1);
}

void writeImages(
  YAML::Emitter &emitter,
  const std::vector<Image> &images,
  const std::vector<RectPx> &rects,
  bool hasWhitepixel
) {
  assert(images.size() == rects.size());

  emitter << YAML::BeginMap;
  size_t size = images.size();
  if (hasWhitepixel) {
    size--;
  }
  for (size_t i = 0; i != size; i++) {
    emitter << YAML::Key << getImageName(images[i].path) << YAML::Value <<
      YAML::Flow << YAML::BeginSeq <<
        rects[i].p.x <<
        rects[i].p.y <<
        (rects[i].p.x + rects[i].s.x - 1) <<
        (rects[i].p.y + rects[i].s.y - 1) <<
      YAML::EndSeq;
  }
  emitter << YAML::EndMap;
}

void writeAtlas(
  const std::string &output,
  const std::vector<Image> &images,
  const std::vector<RectPx> &rects,
  SizePx size,
  bool hasWhitepixel
) {
  PROFILE(writeAtlas(Image));

  std::cout << "Writing atlas to file \"" << output << "\"\n";
  
  std::ofstream file(output);
  YAML::Emitter emitter(file);
  
  checkEmitter(emitter);
  
  emitter << YAML::BeginDoc << YAML::BeginMap <<
    YAML::Key << "type" << YAML::Value << "image" <<
    YAML::Key << "size" << YAML::Value << YAML::Flow << YAML::BeginSeq <<
      size << size <<
    YAML::EndSeq;
  
  if (hasWhitepixel) {
    const PosPx2 pos = {
      rects.back().p.x + (rects.back().s.x - 1) / 2,
      rects.back().p.y + (rects.back().s.y - 1) / 2
    };
    emitter <<
      YAML::Key << "whitepixel" << YAML::Value << YAML::Flow << YAML::BeginSeq <<
        pos.x << pos.y <<
      YAML::EndSeq;
  }
  
  emitter <<
    YAML::Key << "images" << YAML::Value;
  
  writeImages(emitter, images, rects, hasWhitepixel);
  
  emitter << YAML::EndMap << YAML::EndDoc;
  
  checkEmitter(emitter);
}
