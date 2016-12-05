//
//  mouse.hpp
//  Game Engine
//
//  Created by Indi Kernick on 5/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef mouse_hpp
#define mouse_hpp

#include <cstdint>

namespace Input {
  namespace MButton {
    using Type = uint8_t;
    enum : Type {
      UNKNOWN = 0,
      LEFT,
      MIDDLE,
      RIGHT,
      NUM_OF_BUTTONS
    };
  }
}

#endif
