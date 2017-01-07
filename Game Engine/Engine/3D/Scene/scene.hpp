//
//  scene.hpp
//  Game Engine
//
//  Created by Indi Kernick on 18/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_3d_scene_scene_hpp
#define engine_3d_scene_scene_hpp

#include "../matstack.hpp"
#include <list>
#include <map>
#include <memory>
#include "../../Game Logic/actor.hpp"
#include "program 3d.hpp"
#include "program factory.hpp"

namespace Graphics3D {
  class RootNode;
  class CameraNode;
  class LightManager;
  class SceneNode;

  class Scene {
  public:
    using Ptr = std::shared_ptr<Scene>;
  
    Scene();
    virtual ~Scene() = default;
    
    void restore();
    void update(uint64_t delta);
    void render();
  
    std::shared_ptr<SceneNode> getNode(Game::Actor::ID) const;
    void addChild(Game::Actor::ID, std::shared_ptr<SceneNode>);
    void addRootChild(std::shared_ptr<SceneNode>);
    void remChild(Game::Actor::ID);
    
    void setCamera(std::shared_ptr<CameraNode>);
    std::shared_ptr<CameraNode> getCamera() const;
    
    LightManager &getLightManager();
    std::shared_ptr<RootNode> getRoot() const;
    
    Program3D::Ptr getProgram3d() const;
  protected:
    MatStack matStack;
    
    std::shared_ptr<RootNode> root;
    std::shared_ptr<CameraNode> camera;
    std::shared_ptr<LightManager> lightManager;
    Program3D::Ptr program3d;
    
    std::map<Game::Actor::ID, std::shared_ptr<SceneNode>> actorMap;
  };
}

#endif
