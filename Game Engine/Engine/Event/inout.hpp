//
//  eventinout.hpp
//  game engine
//
//  Created by Indi Kernick on 2/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef eventinout_hpp
#define eventinout_hpp

#include <functional>
#include <vector>
#include <map>
#include <ID/rand.hpp>

namespace EventSystem {
  template<typename EVENT, typename IN_DATA, typename OUT_DATA>
  class InOut {
  private:
    struct Callback {
      std::function<OUT_DATA (IN_DATA)> func;
      bool removed = false;
    };
  public:
    class ListenerID {
    friend class InOut;
    public:
      ListenerID() = default;
    private:
      ::ID::Type sysID;
      typename std::vector<Callback>::iterator callback;
    };
    
    InOut() : id(ID.create()) {}
    
    ListenerID addListener(EVENT event, std::function<OUT_DATA (IN_DATA)> callback) {
      Callbacks& callbacks = getCallbacks(event);
      callbacks.push_back({callback});
      ListenerID id;
      id.sysID = this->id;
      id.callback = --callbacks.end();
      return id;
    }
    
    void removeListener(ListenerID id) {
      if (id.sysID == this->id) {
        id.callback->removed = true;
      }
    }
  protected:
    std::vector<OUT_DATA> dispatch(EVENT event, IN_DATA in) {
      std::vector<OUT_DATA> out;
      auto iter = listeners.find(event);
      if (iter != listeners.end()) {
        for (auto i = iter->begin(); i != iter->end(); ++i) {
          if (!i->removed) {
            out.push_back(i->func(in));
          }
        }
      }
      return out;
    }
  private:
    typedef std::vector<Callback> Callbacks;
    typedef std::map<EVENT, Callbacks> Listeners;
    Listeners listeners;
    
    Callbacks& getCallbacks(EVENT event) {
      auto iter = listeners.find(event);
      if (iter != listeners.end()) {
        return iter->second;
      } else {
        listeners[event] = {};
        return listeners.find(event)->second;
      }
    }
    
    ::ID::Type id;
    static ::ID::Rand ID;
  };
}

template<typename EVENT, typename IN_DATA, typename OUT_DATA>
::ID::Rand EventSystem::InOut<EVENT, IN_DATA, OUT_DATA>::ID;

#endif