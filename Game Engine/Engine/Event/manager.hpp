//
//  manager.hpp
//  Game Engine
//
//  Created by Indi Kernick on 11/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_event_manager_hpp
#define engine_event_manager_hpp

#include <functional>
#include "event.hpp"
#include <unordered_map>
#include <list>
#include <queue>
#include "../Utils/logger.hpp"

namespace Game {
  class EventManager {
  public:
    using Ptr = std::shared_ptr<EventManager>;
    using Listener = std::function<void (Event::Ptr)>;
    
    EventManager() = default;
    
    void update();
    
    ///Adds a listener to be called when an event is fired
    void addListener(Event::Type, const Listener &);
    ///Returns true if listener was actually removed
    bool remListener(Event::Type, const Listener &);
    ///Calls the listeners immediately
    void triggerNow(Event::Ptr);
    ///Calls the listeners on the next frame
    void trigger(Event::Ptr);
    
  private:
    using EventQueue = std::queue<Event::Ptr>;
    using ListenerList = std::list<Listener>;
    using ListenerMap = std::unordered_map<Event::Type, ListenerList>;
    
    EventQueue queues[2];
    ListenerMap listeners;
    uint8_t activeQueue = 0;
    
    static bool compare(const Listener &, const Listener &);
  };
}

#endif
