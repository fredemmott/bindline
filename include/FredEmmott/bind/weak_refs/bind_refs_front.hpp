// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../weak_refs/bind_refs_front.hpp"
#include "../bindable_t.hpp"
#include "../detail/binder_t.hpp"

namespace FredEmmott::bind_detail {

struct bind_refs_front_fn {
  template <class... Args>
  static constexpr auto bind(Args&&... args) {
    return ::FredEmmott::weak_refs::bind_refs_front(
      std::forward<Args>(args)...);
  }
};
template <class... Args>
struct bind_refs_front_t : binder_t<bind_refs_front_fn, std::decay_t<Args>...> {
  using binder_t<bind_refs_front_fn, std::decay_t<Args>...>::binder_t;
};

}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {

template <class... TArgs>
[[nodiscard]]
constexpr auto bind_refs_front(TArgs&&... args) {
  using ::FredEmmott::bind_detail::bind_refs_front_t;
  namespace wr = ::FredEmmott::weak_refs;

  // We need to convert to weak refs here so that the
  // `invocable_or_bindable_t` doesn't store a strong reference
  return bindable_t::make_projected<bind_refs_front_t>(
    []<wr::convertible_to_weak_ref T>(T&& v) constexpr {
      return wr::make_weak_ref(std::forward<T>(v));
    },
    std::forward<TArgs>(args)...);
}

}// namespace FredEmmott::bind