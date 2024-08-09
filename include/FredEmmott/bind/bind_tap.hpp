// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindable_t.hpp"

namespace FredEmmott::bind_detail {

template <class TTap>
struct bind_tap_t : public ::FredEmmott::bind::bindable_t {
  bind_tap_t() = delete;
  constexpr bind_tap_t(TTap tap) : mTap(std::forward<TTap>(tap)) {
  }

  [[nodiscard]]
  constexpr auto bind_to(auto&& f) const {
    return [f, tap = mTap]<class... Args>(Args&&... args) {
      tap(std::add_const_t<std::add_rvalue_reference_t<std::decay_t<Args>>>(
        args)...);
      return f(std::forward<Args>(args)...);
    };
  }

 private:
  std::decay_t<TTap> mTap;
};

}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {
template <class... Args>
[[nodiscard]]
constexpr auto bind_tap(Args&&... args) {
  return bindable_t::make<::FredEmmott::bind_detail::bind_tap_t>(
    std::forward<Args>(args)...);
}

}// namespace FredEmmott::bind