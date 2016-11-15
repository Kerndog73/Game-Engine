//
//  obj ascii.hpp
//  game engine
//
//  Created by Indi Kernick on 3/11/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef obj_ascii_hpp
#define obj_ascii_hpp

#include <fstream>
#include <vector>
#include <string>
#include <map>

class ObjASCII {
public:
  struct Vertex {
    float x;
    float y;
    float z;
    float w = 1.0;
  };
  struct TextureVertex {
    float u;
    float v = 0.0;
    float w = 0.0;
  };
  struct VertexNormal {
    float i;
    float j;
    float k;
  };
  struct Face {
    std::vector<uint32_t> verts;
    std::vector<uint32_t> textVerts;
    std::vector<uint32_t> normals;
    std::string material;
  };
  struct Group {
    std::vector<Face> faces;
  };
  
  void write(std::ofstream &);
  static ObjASCII read(std::ifstream &);
private:
  std::vector<Vertex> verts;
  std::vector<TextureVertex> textVerts;
  std::vector<VertexNormal> vertNorms;
  std::map<std::string, Group> groups;
  
  
};

#endif