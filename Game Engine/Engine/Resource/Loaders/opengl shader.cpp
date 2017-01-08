//
//  opengl shader.cpp
//  Game Engine
//
//  Created by Indi Kernick on 8/1/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "opengl shader.hpp"

using namespace Resource;

bool Loaders::ShaderOpenGL::canLoad(const std::string &fileExt) {
  vertShader = fileExt == "vert";
  return vertShader || fileExt == "frag";
}

size_t Loaders::ShaderOpenGL::getSize(const Memory::Buffer) {
  return 0;
}

bool Loaders::ShaderOpenGL::useRaw() {
  return false;
}

Desc::Ptr Loaders::ShaderOpenGL::process(const Memory::Buffer file, Memory::Buffer) {
  GLenum type = GL_FRAGMENT_SHADER;
  if (vertShader) {
    type = GL_VERTEX_SHADER;
  }
  const char *typeName = type == GL_FRAGMENT_SHADER ? "fragment" : "vertex";
  
  GLuint id = glCreateShader(type);
  Descs::ShaderOpenGL::Ptr shader =
    std::make_shared<Descs::ShaderOpenGL>(id, type);
  
  if (id == 0 || !glIsShader(id)) {
    Log::write(Log::RENDERING, Log::ERROR,
      "Failed to create shader object: %s", gluErrorString(glGetError()));
  }
  
  const GLchar *string = reinterpret_cast<const GLchar *>(file.begin());
  const GLint length = static_cast<GLint>(file.size());
  glShaderSource(id, 1, &string, &length);
  glCompileShader(id);
  
  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status == GL_TRUE) {
    Log::write(Log::RENDERING, Log::INFO,
      "Successfully compiled %s shader", typeName);
  } else {
    Log::write(Log::RENDERING, Log::ERROR,
      "Failed to compile %s shader", typeName);
  }
  
  GLint logLength;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength) {
    char *log = new char[logLength];
    glGetShaderInfoLog(id, logLength, nullptr, log);
    Log::write(Log::RENDERING, Log::INFO,
      "%s shader info log:\n%s", typeName, log);
    delete[] log;
  } else {
    Log::write(Log::RENDERING, Log::INFO,
      "%s shader didn't produce an info log", typeName);
  }
  
  return shader;
}
