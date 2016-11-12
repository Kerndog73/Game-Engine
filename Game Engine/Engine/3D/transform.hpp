//
//  transform3d.hpp
//  game engine
//
//  Created by Indi Kernick on 7/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef transform3d_hpp
#define transform3d_hpp

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
  Transform() = default;
  
  glm::mat4 getMat() const;
  
  glm::vec3 pos;
  glm::vec3 scale {1,1,1};
  glm::quat rot;
};

#endif