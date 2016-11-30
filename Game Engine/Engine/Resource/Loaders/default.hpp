//
//  default.hpp
//  Game Engine
//
//  Created by Indi Kernick on 19/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_resource_loaders_default_hpp
#define engine_resource_loaders_default_hpp

#include "loader.hpp"

namespace Resource {
  namespace Loaders {
    class Default : public Loader {
    public:
      bool canLoad(const std::string &fileExt) override;
      size_t getSize(const Memory::Buffer file) override;
      bool useRaw() override;
      Desc::Ptr process(const Memory::Buffer file, Memory::Buffer resource) override;
    };
  }
}

#endif
