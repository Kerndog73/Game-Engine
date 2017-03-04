//
//  enum combine.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_enum_combine_hpp
#define engine_utils_enum_combine_hpp

#include "../Math/type least bytes.hpp"

template <typename ...ARGS>
constexpr size_t size() {
  return (0 + ... + sizeof(ARGS));
}

template <typename ...ARGS>
using Ret = typename Math::UnsignedTypeLeastBytes<size<ARGS...>()>::type;

template <typename FIRST>
constexpr std::make_unsigned_t<FIRST> combine(FIRST first) {
  return static_cast<std::make_unsigned_t<FIRST>>(first);
}

template <typename FIRST, typename... REST>
constexpr Ret<FIRST, REST...> combine(FIRST first, REST... rest) {
  return (static_cast<Ret<FIRST, REST...>>(first) << (size<REST...>() * 8)) | combine(rest...);
}

#endif
