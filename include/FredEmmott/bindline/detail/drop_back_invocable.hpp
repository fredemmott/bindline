// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <cstddef>
#include <utility>

#include "meta/parameter_pack_slice.hpp"

namespace FredEmmott::bindline_detail {

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
      using next_t = instantiate_from_parameter_pack_slice_t<
        0,
        sizeof...(TArgs) + 2,// + TDropCount + TDropTraits + TFn + TArgs - 1
        drop_back_invocable_counted_t,
        std::integral_constant<size_t, TDropCount::value + 1>,
        TDropTraits,
        TFn,
        TArgs...>;
      return next_t::value();
    }
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

}// namespace FredEmmott::bindline_detail