// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindable_t.hpp"
#include "detail/binder_t.hpp"

namespace FredEmmott::bind_detail {

struct bind_front_fn {
  template <class... Args>
  static constexpr auto bind(Args&&... args) {
    return std::bind_front(std::forward<Args>(args)...);
  }
};

template <class... Args>
struct front_binder_t : binder_t<bind_front_fn, std::decay_t<Args>...> {
  using binder_t<bind_front_fn, std::decay_t<Args>...>::binder_t;
};

}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {

template <class... Args>
[[nodiscard]]
auto bind_front(Args&&... args) {
  return bindable_t::make<::FredEmmott::bind_detail::front_binder_t>(
    std::forward<Args>(args)...);
}

}// namespace FredEmmott::bind