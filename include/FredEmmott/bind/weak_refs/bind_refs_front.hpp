// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../weak_refs/bind_refs_front.hpp"
#include "../bindable_t.hpp"
#include "../detail/binder_t.hpp"
#include "../invocable_bindable_t.hpp"

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

template <class First, ::FredEmmott::weak_refs::convertible_to_weak_ref... Rest>
  requires ::FredEmmott::weak_refs::convertible_to_weak_ref<First>
  || requires(First first, Rest... rest) {
       ::FredEmmott::weak_refs::bind_refs_front(first, rest...);
     }
[[nodiscard]]
auto bind_refs_front(First&& first, Rest&&... rest) {
  using namespace ::FredEmmott::bind_detail;
  using namespace ::FredEmmott::weak_refs;

  if constexpr (!convertible_to_weak_ref<First>) {
    return ::FredEmmott::weak_refs::bind_refs_front(
      std::forward<First>(first), std::forward<Rest>(rest)...);
  } else {
    // We need to convert to weak refs here so that the
    // `invocable_or_bindable_t` doesn't store a strong reference
    return make_bindable<bind_refs_front_t>(
      make_weak_ref(std::forward<First>(first)),
      make_weak_ref(std::forward<Rest>(rest))...);
  }
}

}// namespace FredEmmott::bind