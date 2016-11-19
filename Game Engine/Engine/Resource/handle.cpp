//
//  handle.cpp
//  game engine
//
//  Created by Indi Kernick on 6/11/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "handle.hpp"

Resource::Handle::Handle(Cache *cache, std::string path, Memory::Buffer buffer)
  : buffer(buffer), cache(cache), path(path) {}

Resource::Handle::~Handle() {
  cache->freed(buffer.size());
}
