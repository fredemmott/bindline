// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "drop_back_invocable.hpp"

namespace FredEmmott::composable_bind_detail {

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
      using next_t = drop_last_t::next_t<
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
    } else {
      static_assert(false, "Should be statically unreachable due to concept");
    }
  }
};

template <class TDropTraits, class TFn, class... TArgs>
using drop_back_invoke_entrypoint_t = drop_back_invoke_counted_t<
  std::integral_constant<std::size_t, 0>,
  TDropTraits,
  TFn,
  TArgs...>;

}// namespace FredEmmott::composable_bind_detail