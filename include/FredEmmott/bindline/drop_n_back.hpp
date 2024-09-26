// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "detail/drop_back_bindable_t.hpp"
namespace FredEmmott::bindline_detail {

#if defined(_MSVC_LANG) && !defined(__clang__)
// C4296: expression is always false
//
// As of 2024-08-12, MSVC 2022 checks this per instantiation - so, it raises
// this warning for `can_drop_v` if you use drop_n_args<2>() and then invoke
// with two extra arguments
#pragma warning(push)
#pragma warning(disable : 4296)
#endif
/// Drop exactly <N> arguments
template <size_t N>
struct drop_n_traits {
  template <size_t DropCount, class T>
  static constexpr bool can_drop_v = DropCount < N;
  static constexpr size_t minimum_dropped_v = N;
};
#if defined(_MSVC_LANG) && !defined(__clang__)
#pragma warning(pop)
#endif
}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

/** Drop exactly <N> trailing arguments.
 *
 * This is roughly the opposite of `std::bind_back()`.
 *
 * Usage: `f | drop_n_back<2>()`
 */
template <size_t N>
[[nodiscard]]
constexpr auto drop_n_back() {
  return ::FredEmmott::bindline_detail::drop_back_bindable_t<
    ::FredEmmott::bindline_detail::drop_n_traits<N>> {};
}

/** Drop exactly <N> trailing arguments.
 *
 * This is roughly the opposite of `std::bind_back()`.
 *
 * Usage: `drop_n_back<2>(f)`
 */
template <size_t N, class TFn>
[[nodiscard]]
constexpr auto drop_n_back(TFn&& fn) {
  return drop_n_back<N>().bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bindline