//
//  impl.cpp
//  Game Engine
//
//  Created by Indi Kernick on 5/1/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "impl.hpp"

Game::AppImpl::AppImpl()
  : interval(16'666) {}

void Game::AppImpl::init() {
  using namespace Math::Literals;

  Library::Ptr lib = std::make_shared<Libraries::SDL>();

  Window::Desc window;
  window.title = "Game Engine";
  window.size = {1280, 720};
  window.resizable = false;
  window.fullscreen = false;
  
  Renderer::Desc renderer;
  renderer.clearColor = {1.0, 0.0, 0.0, 1.0};
  
  initWindow(lib, window, renderer);
  
  eventManager = std::make_shared<EventManager>();
  //init logic
  cache = std::make_shared<Resource::Cache>(50_mb);
  scene = std::make_shared<Graphics3D::Scene>();
}

void Game::AppImpl::update(uint64_t delta) {
  input->update();
  scene->update(delta);
  
  renderer->preRender();
  scene->render();
  renderer->postRender();
  
  interval.wait();
}

void Game::AppImpl::quit() {
  quitWindow();
}

std::string Game::AppImpl::getCompany() {
  return "Indi Kernick";
}

std::string Game::AppImpl::getAppName() {
  return "Game Engine";
}
