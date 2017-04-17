//
//  impl.hpp
//  Game Engine
//
//  Created by Indi Kernick on 24/1/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_game_view_human_impl_hpp
#define engine_game_view_human_impl_hpp

#include "base.hpp"
#include <Ogre/Ogre.h>

namespace Game {
  class HumanViewImpl final : public HumanView {
  public:
    HumanViewImpl() = default;
    ~HumanViewImpl() = default;
    
    void init() override;
    void update(uint64_t) override;
    void quit() override;
  
  private:
    Ogre::RenderWindow *window;
    Ogre::SceneManager *scene;
    Ogre::Camera *camera;
    Ogre::Viewport *viewport;
    Ogre::SceneNode *ogrehead;
  };
}

#endif
