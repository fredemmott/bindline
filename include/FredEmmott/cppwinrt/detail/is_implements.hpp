
// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <winrt/base.h>

#include "../concepts/winrt_implementation.hpp"
#include "../concepts/winrt_interface.hpp"

namespace FredEmmott::cppwinrt_detail {

template <class T, class Find, class...>
struct is_implements_t;

template <class T, class Find, class Derived, class First, class... Rest>
struct is_implements_t<T, Find, winrt::implements<Derived, First, Rest...>> {
  static consteval bool test() {
    if constexpr (std::same_as<Find, First>) {
      return true;
    }

    if constexpr (sizeof...(Rest) == 0) {
      return false;
    } else {
      return is_implements_t<
        T,
        Find,
        winrt::implements<typename Derived::implements_type, Rest...>>::test();
    }
  }
};

template <FredEmmott::cppwinrt::winrt_implementation T, class Find>
struct is_implements_t<T, Find> {
  static consteval bool test() {
    return is_implements_t<T, Find, typename T::implements_type>::test();
  }
};

template <FredEmmott::cppwinrt::winrt_interface T, class Find>
struct is_implements_t<T, Find> {
  static consteval bool test() {
    return std::convertible_to<T, Find>;
  }
};

template <class T, class Find>
constexpr bool is_implements_v = is_implements_t<T, Find>::test();
}// namespace FredEmmott::cppwinrt_detail