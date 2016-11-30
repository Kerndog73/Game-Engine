//
//  interface.hpp
//  Game Engine
//
//  Created by Indi Kernick on 11/08/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_id_interface_hpp
#define engine_id_interface_hpp

#include <stdint.h>
#include <stdexcept>
#include "../Math/format.hpp"
#include <string>

namespace ID {
  typedef uint64_t Type;
  const Type MAX_VAL = -1;
  
  std::string display(Type id);

  class Interface {
  public:
    Interface() = default;
    ///Generate a "unique" ID
    virtual Type create() = 0;
    ///Remove the ID from the list of used IDs so it's
    ///available next time create is called
    virtual void remove(Type id) = 0;
    ///Make all IDs available
    virtual void removeAll() = 0;
    ///Checks if the ID is being used
    virtual bool has(Type id) = 0;
  };
};

#endif
