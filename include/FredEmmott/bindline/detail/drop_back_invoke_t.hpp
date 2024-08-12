// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "drop_back_invocable.hpp"

namespace FredEmmott::bindline_detail {

template <class TDropCount, class TDropTraits, class TFn, class... TArgs>
struct drop_back_invoke_counted_t {
  static_assert(std::same_as<
                TDropCount,
                std::integral_constant<size_t, TDropCount::value>>);
  static constexpr decltype(auto) invoke(TFn&& fn, TArgs&&... args)
    requires drop_back_invocable_counted<TDropCount, TDropTraits, TFn, TArgs...>
  {
    if constexpr (std::invocable<TFn, TArgs...>) {
      return std::invoke(std::forward<TFn>(fn), std::forward<TArgs>(args)...);
    } else if constexpr (sizeof...(TArgs) >= 1) {
      using next_t = drop_last_t::template next_t<
        drop_back_invoke_counted_t,
        std::integral_constant<size_t, TDropCount::value + 1>,
        TDropTraits,
        TFn,
        TArgs...>;

      auto next_args = drop_last_t::make_tuple(
        std::forward<TFn>(fn), std::forward<TArgs>(args)...);
      // no `- 1` as `args` excludes TFn, `next-args` includes it
      static_assert(std::tuple_size_v<decltype(next_args)> == sizeof...(TArgs));

      return std::apply(&next_t::invoke, std::move(next_args));
    }
    // `static_assert(false)` should be fine since C++17 and as a defect report
    // on C++11 (CWG2518); unfortunately for now we're testing on G++ 11.4
    // (Ubuntu 22.04), which does not permit it.
#if defined(_MSC_VER) && !defined(__clang__)
    // Giving this priority over `std::unreachable()` because MSVC will raise an
    // 'unreachable code' warning for `std::unreachable`
    __assume(false);
#elif defined(__cpp_lib_unreachable)
    std::unreachable();
#else
    __builtin_unreachable();
#endif
  }
};

template <class TDropTraits, class TFn, class... TArgs>
using drop_back_invoke_entrypoint_t = drop_back_invoke_counted_t<
  std::integral_constant<std::size_t, 0>,
  TDropTraits,
  TFn,
  TArgs...>;

}// namespace FredEmmott::bindline_detail