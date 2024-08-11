// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <cstddef>
#include <utility>

#include "drop_last_t.hpp"

namespace FredEmmott::composable_bind_detail {

template <class TDropCount, class TDropTraits, class TFn, class... TArgs>
struct drop_back_invocable_counted_t {
  static_assert(
    std::convertible_to<TDropCount, size_t>,
    "TDropCount should be an std::integral_constant<size_t>");

  static consteval bool value() {
    if constexpr (
      std::invocable<TFn, TArgs...>
      && TDropCount::value >= TDropTraits::minimum_dropped_v) {
      return true;
    } else if constexpr (sizeof...(TArgs) == 0) {
      return false;
    } else if constexpr (
      !TDropTraits::template can_drop_v<
        TDropCount::value,
        std::tuple_element_t<sizeof...(TArgs) - 1, std::tuple<TArgs...>>>) {
      return false;
    } else {
      using next_t = drop_last_t::template next_t<
        drop_back_invocable_counted_t,
        std::integral_constant<size_t, TDropCount::value + 1>,
        TDropTraits,
        TFn,
        TArgs...>;
      return next_t::value();
    }
#ifdef __cpp_lib_unreachable
    std::unreachable();
#elif defined(_MSC_VER) && !defined(__clang__)
    __assume(false);
#else
    __builtin_unreachable();
#endif
  }
};

template <class TDropTraits, class TFn, class... TArgs>
using drop_back_invocable_entrypoint_t = drop_back_invocable_counted_t<
  std::integral_constant<size_t, 0>,
  TDropTraits,
  TFn,
  TArgs...>;

template <class TDropCount, class TDropTraits, class TFn, class... TArgs>
concept drop_back_invocable_counted
  = drop_back_invocable_counted_t<TDropCount, TDropTraits, TFn, TArgs...>::
    value();

template <class TDropTraits, class TFn, class... TArgs>
concept drop_back_invocable_entrypoint
  = drop_back_invocable_entrypoint_t<TDropTraits, TFn, TArgs...>::value();

}// namespace FredEmmott::composable_bind_detail