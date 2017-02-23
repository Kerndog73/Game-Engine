//
//  shader path.hpp
//  Game Engine
//
//  Created by Indi Kernick on 19/2/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_3d_opengl_shader_path_hpp
#define engine_3d_opengl_shader_path_hpp

#include "constants.hpp"
#include "../../Resource/id.hpp"
#include <array>

namespace Graphics3D {
  struct Shaders {
    Res::ID vert = nullptr;
    Res::ID frag = nullptr;
  };
  
  Shaders getShaders(ProgType);
}

#endif
