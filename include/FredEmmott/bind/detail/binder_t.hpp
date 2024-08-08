// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../bindable_t.hpp"

namespace FredEmmott::bind_detail {

template <class bind_fn, class... Binds>
  requires(sizeof...(Binds) >= 1)
struct binder_t : public bind::bindable_t {
  binder_t() = delete;

  binder_t(const Binds&... args) : mBound(static_cast<Binds>(args)...) {
  }

  template <class TFn>
  constexpr auto bind_to(TFn&& fn) {
    return std::apply(
      [&](const Binds&... args) {
        return bind_fn::bind(std::forward<TFn>(fn), args...);
      },
      mBound);
  }

 private:
  std::tuple<Binds...> mBound;
};

}// namespace FredEmmott::bind_detail