//
//  eventmanager.hpp
//  Game Engine
//
//  Created by Indi Kernick on 11/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef eventmanager_hpp
#define eventmanager_hpp

#include <functional>
#include "event.hpp"
#include <map>
#include <list>
#include <queue>
#include "../Task Manager/task.hpp"
#include "../debugmacros.hpp"

#ifdef DEBUG
#include "../Utils/logger.hpp"
#endif

class EventManager : public Task {
public:
  using Listener = std::function<void (Event::Ptr)>;
  
  EventManager();
  
  ///Adds a listener to be called when an event is fired
  void addListener(Event::Type, const Listener &);
  ///Returns true if listener was actually removed
  bool remListener(Event::Type, const Listener &);
  ///Calls the listeners immediately
  void triggerNow(Event::Ptr);
  ///Calls the listeners on the next frame
  void triggerLater(Event::Ptr);
  
private:
  using EventQueue = std::queue<Event::Ptr>;
  using ListenerList = std::list<Listener>;
  using ListenerMap = std::map<Event::Type, ListenerList>;
  
  EventQueue queues[2];
  ListenerMap listeners;
  uint8_t activeQueue = 0;
  #ifdef DEBUG
  Logger logger;
  #endif
  
  void update(DeltaType) override;
  
  static bool compare(const Listener &, const Listener &);
};

#endif