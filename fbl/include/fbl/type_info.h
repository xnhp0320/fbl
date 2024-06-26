// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FBL_TYPE_INFO_H_
#define FBL_TYPE_INFO_H_

#include <stddef.h>

#include <utility>

//
// Compile-time type info utility without RTTI.
//

// Check for supported versions of Clang or GCC.
#if defined(__clang__)
#if (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__) < 30600
#error "Unsupported clang version!"
#endif
#elif defined(__GNUC__)
#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) < 50400
#error "Unsupported gcc version!"
#endif
#else
#error "Unrecognized compiler!"
#endif

namespace fbl {

// TypeInfo provides a compile-time string name for the given type T without
// requiring RTTI to be enabled. The type string is derived from
// __PRETTY_FUNCTION__ using a constexpr expression that works with both GCC and
// Clang.
//
// TypeInfo may be used in a wide variety of contexts. Here is one example of
// logging the types used to invoke a template function:
//
//  template <typename T, typename U, typename V>
//  void Function(const T& t, const U& u, const V& v) {
//      log("Function<%s, %s, %s>(...) invoked!",  TypeInfo<T>::Name(),
//          TypeInfo<U>::Name(), TypeInfo<V>::Name());
//      // Function body...
//  }
//
namespace internal {

template <typename T>
class TypeInfo {
 public:
  constexpr TypeInfo() : TypeInfo(std::make_index_sequence<Size>{}) {}
  constexpr const char* Name() const { return name_; }

 private:
  struct Info {
    const char* const kName;
    const size_t kOffset;
    const size_t kSize;
  };

  static constexpr Info Get() {
    const char* type_name = __PRETTY_FUNCTION__;
    const size_t size = sizeof(__PRETTY_FUNCTION__);
    size_t start = Find(type_name, '=', 0, Forward) + 2;
    size_t end = Find(type_name, ']', size - 1, Reverse) + 1;
    return {type_name, start, end - start};
  }

  enum Direction { Forward, Reverse };
  static constexpr size_t Find(const char* subject, char value, size_t index, Direction direction) {
    while (subject[index] != value)
      index += direction == Forward ? 1 : -1;
    return index;
  }

  static constexpr size_t Size = Get().kSize;
  static constexpr size_t Offset = Get().kOffset;
  static constexpr const char* BaseName = Get().kName;

  template <size_t... Is>
  constexpr TypeInfo(std::index_sequence<Is...>)
      : name_{(Is < Size - 1 ? BaseName[Offset + Is] : '\0')...} {}

  const char name_[Size];
};

}  // namespace internal

template <typename T>
class TypeInfo {
 public:
  // Returns the string name for type T.
  static constexpr const char* Name() { return storage_.Name(); }

 private:
  static constexpr internal::TypeInfo<T> storage_{};
};

}  // namespace fbl

#endif  // FBL_TYPE_INFO_H_
