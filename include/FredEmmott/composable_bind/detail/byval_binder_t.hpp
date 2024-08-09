// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../bindable_t.hpp"

namespace FredEmmott::composable_bind_detail {

template <class bind_fn, class... Binds>
struct byval_binder_t : public composable_bind::bindable_t {
  static_assert((std::same_as<Binds, std::decay_t<Binds>> && ...));
  byval_binder_t() = delete;

  template <class... TInitBinds>
    requires(sizeof...(Binds) == sizeof...(TInitBinds))
  constexpr byval_binder_t(TInitBinds&&... args)
    : mBound(std::forward<TInitBinds>(args)...) {
  }

  template <class TFn>
  [[nodiscard]]
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

}// namespace FredEmmott::composable_bind_detail