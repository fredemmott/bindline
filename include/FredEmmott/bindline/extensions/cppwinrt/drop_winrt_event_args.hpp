// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../../cppwinrt/concepts.hpp"
#include "../../extension_api/drop_back_bindable_t.hpp"

namespace FredEmmott::bindline_detail {

struct drop_winrt_event_args_trait {
  template <size_t DropCount, class T>
  static constexpr bool can_drop_v
    = DropCount < 2 && ::FredEmmott::cppwinrt::winrt_type<T>;
  static constexpr size_t minimum_dropped_v = 2;
};
}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

/** Drop exactly 2 trailing `winrt_type` arguments.
 *
 * These are usually the event sender and event arguments.
 *
 * Usage: `f | drop_winrt_event_args()`
 */
[[nodiscard]]
constexpr auto drop_winrt_event_args() {
  return ::FredEmmott::bindline_extension_api::drop_back_bindable_t<
    ::FredEmmott::bindline_detail::drop_winrt_event_args_trait> {};
}

/** Drop exactly 2 trailing `winrt_type` arguments.
 *
 * These are usually the event sender and event arguments.
 *
 * Usage: `drop_winrt_event_args(fn)`
 */
template <class TFn>
[[nodiscard]]
constexpr auto drop_winrt_event_args(TFn&& fn) {
  return drop_winrt_event_args().bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bindline