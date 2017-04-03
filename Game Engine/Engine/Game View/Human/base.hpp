//
//  base.hpp
//  Game Engine
//
//  Created by Indi Kernick on 10/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_game_view_human_base_hpp
#define engine_game_view_human_base_hpp

#include "../base.hpp"
#include <list>
#include "../../Anim/process manager.hpp"
#include "../../3D/Scene/root.hpp"
#include "root.hpp"

#ifdef USE_OPENGL
#include "../../3D/OpenGL/renderer.hpp"
#include "opengl renderer.hpp"
#include "../../3D/OpenGL/program manager.hpp"
#else
#error No rendering implementation was specified
#endif

namespace Game {
  class HumanView : public View {
  public:
    using Ptr = std::shared_ptr<HumanView>;

    HumanView();
    virtual ~HumanView() = default;
    
    virtual void init() override;
    virtual void update(uint64_t) override;
    virtual void render() override;
    virtual void quit() override;
    Type getType() override final;
    
    Scene::Root::Ptr getScene() const;
    UI::Root::Ptr getUI() const;
  protected:
    ProcessManager processManager;
    Scene::Root::Ptr scene;
    UI::Root::Ptr ui;
    Scene::Renderer::Ptr sceneRenderer;
    UI::Renderer::Ptr uiRenderer;
    Graphics3D::ProgramManager::Ptr progMan;
  };
}

#endif
