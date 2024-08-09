// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindable_t.hpp"
#include "detail/argument_binder_t.hpp"

namespace FredEmmott::bind_detail {

struct bind_back_fn {
  template <class... Args>
  [[nodiscard]]
  static constexpr auto bind(Args&&... args) {
    return std::bind_back(std::forward<Args>(args)...);
  }
};

template <class... Args>
struct back_binder_t : argument_binder_t<bind_back_fn, std::decay_t<Args>...> {
  using argument_binder_t<bind_back_fn, std::decay_t<Args>...>::
    argument_binder_t;
};
}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {

template <class... Args>
[[nodiscard]]
constexpr auto bind_back(Args&&... args) {
  return bindable_t::make<::FredEmmott::bind_detail::back_binder_t>(
    std::forward<Args>(args)...);
}

}// namespace FredEmmott::bind