// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bind_component_t.hpp"

namespace FredEmmott::bind {

template <class TTap>
struct bind_tap : public bind_component_t {
  bind_tap() = delete;
  bind_tap(TTap tap) : mTap(std::forward<TTap>(tap)) {
  }

  auto bind_to(auto&& f) {
    return [f, tap = mTap]<class... Args>(Args&&... args) {
      tap(std::add_const_t<std::add_rvalue_reference_t<std::decay_t<Args>>>(
        args)...);
      return f(std::forward<Args>(args)...);
    };
  }

 private:
  std::decay_t<TTap> mTap;
};

}// namespace FredEmmott::bind