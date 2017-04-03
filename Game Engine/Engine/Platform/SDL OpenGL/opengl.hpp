//
//  opengl.hpp
//  Game Engine
//
//  Created by Indi Kernick on 2/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_sdl_opengl_opengl_hpp
#define engine_platform_sdl_opengl_opengl_hpp

#ifdef USE_SDL_OPENGL

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef NDEBUG

#define CHECK_OPENGL_ERROR()

#else

#define CHECK_OPENGL_ERROR() \
for (GLenum error; (error = glGetError()) != GL_NO_ERROR;) {\
  printf("OpenGL error: %s\n", gluErrorString(error));\
  assert(false);\
}\

#endif //NDEBUG

#endif //USE_SDL_OPENGL

#endif
