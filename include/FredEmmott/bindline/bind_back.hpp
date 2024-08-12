// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/bindable_t.hpp>

#include "detail/byval_binder_t.hpp"

namespace FredEmmott::bindline_detail {

struct bind_back_fn {
  template <class... Args>
  [[nodiscard]]
  static constexpr auto bind(Args&&... args) {
    return std::bind_back(std::forward<Args>(args)...);
  }
};

template <class... Args>
struct back_binder_t : byval_binder_t<bind_back_fn, std::decay_t<Args>...> {
  using byval_binder_t<bind_back_fn, std::decay_t<Args>...>::byval_binder_t;
};
}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

/** Bind trailing arguments.
 *
 * This is a wrapper around C++23's `std::bind_back()`
 *
 * Usage:
 * - `bind_back(callable, args...)`
 * - `callable | bind_back(args...)`
 */
template <class... Args>
[[nodiscard]]
constexpr auto bind_back(Args&&... args) {
  return bindable_t::make<::FredEmmott::bindline_detail::back_binder_t>(
    std::forward<Args>(args)...);
}

}// namespace FredEmmott::bindline