//
//  sdl opengl.hpp
//  Game Engine
//
//  Created by Indi Kernick on 16/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_application_renderers_sdl_opengl_hpp
#define engine_application_renderers_sdl_opengl_hpp

#include "opengl.hpp"

#ifdef _SDL_opengl_h

#include "../renderer.hpp"
#include "../../Utils/logger.hpp"

namespace Renderers {
  class SDLOpenGL : public Renderer {
  public:
    SDLOpenGL() = default;
    
    void init(Window *window, const Desc &) override;
    void quit() override;
    
    void preRender() override;
    void postRender() override;
  private:
    SDL_GLContext context;
    SDL_Window *window;
    Color4F clearColor;
  };
}

#include "../Windows/sdl opengl.hpp"

#endif

#endif
