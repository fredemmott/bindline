// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindable_t.hpp"
#include "detail/byval_binder_t.hpp"

namespace FredEmmott::bindline_detail {

struct bind_front_fn {
  template <class... Args>
  [[nodiscard]]
  static constexpr auto bind(Args&&... args) {
    return std::bind_front(std::forward<Args>(args)...);
  }
};

template <class... Args>
struct front_binder_t : byval_binder_t<bind_front_fn, std::decay_t<Args>...> {
  using byval_binder_t<bind_front_fn, std::decay_t<Args>...>::byval_binder_t;
};

}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

/** Bind leading arguments.
 *
 * This is a wrapper around C++20's `std::bind_front()`
 *
 * Usage:
 * - `bind_front(callable, args...)`
 * - `callable | bind_front(args...)`
 */
template <class... Args>
[[nodiscard]]
constexpr auto bind_front(Args&&... args) {
  return bindable_t::make<::FredEmmott::bindline_detail::front_binder_t>(
    std::forward<Args>(args)...);
}

}// namespace FredEmmott::bindline