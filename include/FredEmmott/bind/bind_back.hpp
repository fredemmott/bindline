// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindable_t.hpp"
#include "detail/binder_t.hpp"

namespace FredEmmott::bind_detail {

struct bind_back_fn {
  template <class... Args>
  static constexpr auto bind(Args&&... args) {
    return std::bind_back(std::forward<Args>(args)...);
  }
};

template <class... Args>
struct back_binder_t : binder_t<bind_back_fn, std::decay_t<Args>...> {
  using binder_t<bind_back_fn, std::decay_t<Args>...>::binder_t;
};
}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {

template <class... Args>
[[nodiscard]]
auto bind_back(Args&&... args) {
  using namespace bind_detail;

  return make_bindable<back_binder_t>(std::forward<Args>(args)...);
}

}// namespace FredEmmott::bind