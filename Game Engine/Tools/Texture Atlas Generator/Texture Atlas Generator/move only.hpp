//
//  move only.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 8/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef move_only_hpp
#define move_only_hpp

//structs become non-agregate when they inherit so agregate initialization
//doesn't work.
//This isn't true in c++17 but my compiler doesn't support this feature yet
//so I have to write a macro :(

#define MOVE_ONLY(type) \
  type() = default; \
  type(const type &) = delete; \
  type(type &&) = default; \
  ~type() = default; \
  type &operator=(const type &) = delete; \
  type &operator=(type &&) = delete

#endif