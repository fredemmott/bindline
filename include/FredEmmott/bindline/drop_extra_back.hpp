// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "extension_api/drop_back_bindable_t.hpp"

namespace FredEmmott::bindline_detail {

/// Drop any number of arguments of any type
struct drop_any_arg_traits {
  template <size_t DropCount, class T>
  static constexpr bool can_drop_v = true;

  static constexpr size_t minimum_dropped_v = 0;
};

}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

/** Drop any number of unusable trailing arguments.
 *
 * Usage: `(callable | drop_extra_back())(unused, unused)`
 *
 * The minimum number of extra arguments will be dropped.
 */
[[nodiscard]]
constexpr auto drop_extra_back() {
  return ::FredEmmott::bindline_extension_api::drop_back_bindable_t<
    ::FredEmmott::bindline_detail::drop_any_arg_traits> {};
}

/** Drop any number of unusable trailing arguments.
 *
 * Usage: `drop_extra_back(callable)(unused, unused)`
 *
 * The minimum number of extra arguments will be dropped.
 */
template <class TFn>
[[nodiscard]]
constexpr auto drop_extra_back(TFn&& fn) {
  return drop_extra_back().bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bindline