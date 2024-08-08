// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bind_component_t.hpp"
#include "detail/binder_t.hpp"

namespace FredEmmott::bind_detail {

struct bind_back_fn {
  template <class... Args>
  static constexpr auto bind(Args&&... args) {
    return std::bind_back(std::forward<Args>(args)...);
  }
};
}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {

template <class... Args>
auto bind_back(Args&&... args) {
  using namespace bind_detail;

  return binder_t<bind_back_fn, std::decay_t<Args>...> {
    std::forward<Args>(args)...};
}

}// namespace FredEmmott::bind