//
//  byteptr.hpp
//  Game Engine
//
//  Created by Indi Kernick on 2/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_byteptr_hpp
#define engine_memory_byteptr_hpp

#include <cstdint>
#include <cstddef>
#include <new>
#include <type_traits>

using Byte = uint8_t;
using Word = uint16_t;
using Long = uint32_t;
using Quad = uint64_t;

namespace Memory {
  ///Casts the pointer to an integer which can hold the value of the pointer
  ///safely (uintptr_t)
  inline uintptr_t toNum(void *bytePtr) {
    return reinterpret_cast<uintptr_t>(bytePtr);
  }
  ///Casts the pointer to a Byte pointer
  inline Byte *toByte(void *ptr) {
    return reinterpret_cast<Byte *>(ptr);
  }
  ///Increases the address of the pointer so that it is a multiple
  ///of align
  inline void *alignTo(void *ptr, size_t align) {
    uintptr_t outOfAlign = toNum(ptr) % align;
    // x % x == 0 so without the check we would waste memory when the ptr
    // is aligned
    return outOfAlign ? toByte(ptr) + (align - outOfAlign) : ptr;
  }
  ///Increases the address of the pointer so that it is suitable for the
  ///given type
  template <typename T>
  inline void *alignFor(void *ptr) {
    return alignTo(ptr, alignof(T));
  }
  ///Increases the address of the pointer so that it is a multiple
  ///of align
  inline Byte *alignTo(Byte *ptr, size_t align) {
    uintptr_t outOfAlign = toNum(ptr) % align;
    return outOfAlign ? ptr + (align - outOfAlign) : ptr;
  }
  ///Increases the address of the pointer so that it is suitable for the
  ///given type
  template <typename T>
  inline Byte *alignFor(Byte *ptr) {
    return alignTo(ptr, alignof(T));
  }
  ///Dereferences the Byte pointer as a type
  template <typename T>
  T& deref(Byte *bytePtr) {
    return *reinterpret_cast<T *>(bytePtr);
  }
  ///Assigns a value to the memory pointed to by the pointer
  template <typename T>
  inline void assign(Byte *ptr, T &&value) {
    new (ptr) typename std::remove_reference<T>::type (std::forward<T>(value));
  }
  ///Assigns a value to the memory pointed to by the pointer
  template <typename T>
  inline void assign(Byte *ptr, const T &value) {
    new (ptr) typename std::remove_reference<T>::type (std::forward<T>(value));
  }
}

#endif
