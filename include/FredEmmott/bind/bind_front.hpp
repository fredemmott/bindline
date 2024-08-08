// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bind_component_t.hpp"
#include "detail/binder_t.hpp"

namespace FredEmmott::bind_detail {

struct bind_front_fn {
  template <class... Args>
  static constexpr auto bind(Args&&... args) {
    return std::bind_front(std::forward<Args>(args)...);
  }
};
}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {

template <class... Args>
auto bind_front(Args&&... args) {
  using namespace bind_detail;

  return binder_t<bind_front_fn, std::decay_t<Args>...> {
    std::forward<Args>(args)...};
}

}// namespace FredEmmott::bind