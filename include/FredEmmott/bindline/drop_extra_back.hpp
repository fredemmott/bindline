// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "extension_api/drop_back_bindable_t.hpp"

namespace FredEmmott::bindline_detail {

struct drop_any_arg_traits {
  template <size_t DropCount, class T>
  static constexpr bool can_drop_v = true;

  static constexpr size_t minimum_dropped_v = 0;
};

}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

[[nodiscard]]
constexpr auto drop_extra_back() {
  return ::FredEmmott::bindline_extension_api::drop_back_bindable_t<
    ::FredEmmott::bindline_detail::drop_any_arg_traits> {};
}

template <class TFn>
[[nodiscard]]
constexpr auto drop_extra_back(TFn&& fn) {
  return drop_extra_back().bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bindline