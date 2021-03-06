//
//  base.cpp
//  Game Engine
//
//  Created by Indi Kernick on 10/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "base.hpp"

void Game::View::onAttach() {
  init();
}

void Game::View::onDetach() {
  quit();
}

void Game::View::attach(View::ID newID, Actor::ID newActorID) {
  assert(!attached);
  attached = true;
  id = newID;
  actor = newActorID;
  onAttach();
}

void Game::View::detach() {
  assert(attached);
  attached = false;
  onDetach();
}

Game::View::ID Game::View::getID() const {
  return id;
}

Game::Actor::ID Game::View::getActor() const {
  return actor;
}

bool Game::View::isAttached() const {
  return attached;
}
