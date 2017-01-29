//
//  mesh.cpp
//  Game Engine
//
//  Created by Indi Kernick on 13/1/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "mesh.hpp"

Scene::Mesh::Mesh(Game::Actor::ID actor,
                  const glm::mat4 &toWorld,
                  const Res::ID &mesh)
  : Node(actor, toWorld), mesh(mesh) {
  assert(mesh);
}

const Res::ID &Scene::Mesh::getMesh() const {
  return mesh;
}
