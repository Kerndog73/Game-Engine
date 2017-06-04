//
//  actor.cpp
//  Game Engine
//
//  Created by Indi Kernick on 20/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "actor.hpp"

const Game::Actor::ID Game::Actor::NULL_ID = 0;

Game::MissingComponent::MissingComponent(const char *what)
  : std::runtime_error(what) {}

Game::DuplicateComponent::DuplicateComponent()
  : std::runtime_error("Cannot add more than Component of the same type to an Actor") {}

Game::BadParentPtr::BadParentPtr(const char *what)
  : std::runtime_error(what) {}

Game::Actor::Actor(const ID id)
  : id(id) {}

Game::Actor::ID Game::Actor::getID() const {
  return id;
}

void Game::Actor::update(const uint64_t delta) {
  flushMessages();
  for (auto c = components.cbegin(); c != components.cend(); ++c) {
    (*c)->update(delta);
  }
}

void Game::Actor::flushMessages() {
  try {
    MessageManager<Component::ID>::flushMessages();
  } catch (MissingMessenger &e) {
    throw MissingComponent("Tried to send a message to a component that doesn't exist");
  }
}

Game::Messenger<Game::Component::ID> *Game::Actor::getMessenger(const Component::ID id) const {
  if (id < components.size()) {
    return components[id].get();
  } else {
    return nullptr;
  }
}

Game::Component::ID Game::Actor::getNumMessengers() const {
  return components.size();
}
