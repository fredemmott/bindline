// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "extension_api/drop_back_bindable_t.hpp"
namespace FredEmmott::composable_bind_detail {
template <size_t N>
struct drop_n_back_traits {
  template <size_t DropCount, class T>
  static constexpr bool can_drop_v = DropCount < N;
  static constexpr size_t minimum_dropped_v = N;
};
}// namespace FredEmmott::composable_bind_detail

namespace FredEmmott::composable_bind {

template <size_t N>
[[nodiscard]]
constexpr auto drop_n_back() {
  return ::FredEmmott::composable_bind_extension_api::drop_back_bindable_t<
    ::FredEmmott::composable_bind_detail::drop_n_back_traits<N>> {};
}

template <size_t N, class TFn>
[[nodiscard]]
constexpr auto drop_n_back(TFn&& fn) {
  return drop_n_back<N>().bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::composable_bind