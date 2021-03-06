//
//  height stack.hpp
//  Game Engine
//
//  Created by Indi Kernick on 5/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_game_view_human_height_stack_hpp
#define engine_game_view_human_height_stack_hpp

#include "types.hpp"
#include "../../Utils/op stack.hpp"

namespace UI {
  class HeightStack final : public Utils::OpStack<Height> {
  public:
    HeightStack();
    ~HeightStack() = default;
    
  private:
    Height operation(const Height &, const Height &) override;
  };
}

#endif
