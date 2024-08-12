// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/bindable_t.hpp>
#include <FredEmmott/bindline/detail/byval_binder_t.hpp>
#include <FredEmmott/weak_refs/bind_refs_front.hpp>

namespace FredEmmott::bindline_detail {

struct bind_refs_front_fn {
  template <class... Args>
  [[nodiscard]]
  static constexpr auto bind(Args&&... args) {
    return ::FredEmmott::weak_refs::bind_refs_front(
      std::forward<Args>(args)...);
  }
};

template <class... Args>
struct bind_refs_front_t
  : byval_binder_t<bind_refs_front_fn, std::decay_t<Args>...> {
  using byval_binder_t<bind_refs_front_fn, std::decay_t<Args>...>::
    byval_binder_t;
};

}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

/** Bind weak references as leading arguments, but invoke with strong
 * references.
 *
 * The callable will not be invoked if the strong references are invalid/stale.
 *
 * See `::FredEmmott::weak_refs::bind_refs_back()`
 *
 * Usage:
 * - `callable | bind_refs_back(ref, ...)`
 * - `bind_refs_back(f, ref, ...)`
 */
template <class... TArgs>
[[nodiscard]]
constexpr auto bind_refs_front(TArgs&&... args) {
  using ::FredEmmott::bindline_detail::bind_refs_front_t;
  namespace wr = ::FredEmmott::weak_refs;

  // We need to convert to weak refs here so that the
  // `invocable_or_bindable_t` doesn't store a strong reference
  return bindable_t::make_projected<bind_refs_front_t>(
    []<wr::convertible_to_weak_ref T>(T&& v) constexpr {
      return wr::make_weak_ref(std::forward<T>(v));
    },
    std::forward<TArgs>(args)...);
}

}// namespace FredEmmott::bindline