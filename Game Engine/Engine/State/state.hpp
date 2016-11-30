//
//  state.hpp
//  Game Engine
//
//  Created by Indi Kernick on 18/08/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_state_state_hpp
#define engine_state_state_hpp

namespace FSM {
  class Machine;

  class State {
  friend class Machine;
  
  public:
    State() = default;
    virtual ~State() = default;
  private:
    ///Update the state. Return a new State object to change State
    ///or return nullptr
    virtual State* update(Machine*) = 0;
    virtual void enter() = 0;
    virtual void leave() = 0;
  };
}

#endif
