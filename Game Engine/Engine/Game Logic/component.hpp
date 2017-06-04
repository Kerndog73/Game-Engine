//
//  component.hpp
//  Game Engine
//
//  Created by Indi Kernick on 10/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_game_logic_component_hpp
#define engine_game_logic_component_hpp

#include <memory>
#include <tinyxml2.h>
#include "message.hpp"
#include "../ID/type.hpp"

namespace Game {
  class Actor;

  //Component inherits Messenger<Component::ID>

  class Component : public Messenger<uint8_t> {
  friend class ActorFactory;
  public:
    using Ptr = std::shared_ptr<Component>;
    using ID = uint8_t;

    Component() = default;
    virtual ~Component() = default;
    
    virtual void init(const tinyxml2::XMLElement *) = 0;
    
    virtual void update(uint64_t) = 0;
    
  protected:
    Actor *actor = nullptr;
    
    using Messenger::broadcastMessage;
    using Messenger::sendMessage;
    
  private:
    MessageManager<ID> *getManager() const override;
  };
  
  template <typename ComponentClass>
  using GetComponentID = ID::TypeCounter<Component::ID, ComponentClass, Component>;
}

#endif
