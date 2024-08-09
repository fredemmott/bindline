// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../bindable_t.hpp"

namespace FredEmmott::bind_detail {

template <class bind_fn, class... Binds>
struct argument_binder_t : public bind::bindable_t {
  argument_binder_t() = delete;

  template <class... TInitBinds>
    requires(sizeof...(Binds) == sizeof...(TInitBinds))
  argument_binder_t(TInitBinds&&... args)
    : mBound(std::forward<TInitBinds>(args)...) {
  }

  template <class TFn>
  constexpr auto bind_to(TFn&& fn) const {
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