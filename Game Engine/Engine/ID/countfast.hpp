//
//  countfast.hpp
//  Game Engine
//
//  Created by Indi Kernick on 11/08/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_id_countfast_hpp
#define engine_id_countfast_hpp

#include "interface.hpp"

namespace ID {
  ///Ridiculously fast ID generator
  class CountFast : public Interface {
  public:
    CountFast() = default;
    
    inline Type create() override {
      return ++next;
    }
    inline void remove(Type) override {
      assert(0);
    }
    inline void removeAll() override {
      assert(0);
    }
    inline bool has(Type id) override {
      return id <= next;
    }
  private:
    Type next = MAX_VAL;
  };
}

#endif
