// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "detail/static_const.hpp"

#include <functional>

namespace FredEmmott::bind_detail {

template <class... TArgs>
concept has_adl_bind_front
  = requires(TArgs... args) { adl_bind_front(std::forward<TArgs>(args)...); };

struct bind_front_fn {
  template <class TFn>
  constexpr auto operator()(TFn&& fn) const noexcept {
    return std::forward<TFn>(fn);
  }

  template <class TFn, class TFirst, class... TRest>
    requires has_adl_bind_front<TFirst, TFn, TRest...>
  constexpr auto operator()(TFn&& fn, TFirst&& first, TRest&&... rest)
    const noexcept {
    return adl_bind_front(
      std::forward<TFirst>(first),
      std::forward<TFn>(fn),
      std::forward<TRest>(rest)...);
  }

  template <class... TArgs>
    requires(sizeof...(TArgs) >= 2 && !has_adl_bind_front<TArgs...>)
  constexpr auto operator()(TArgs&&... args) const noexcept {
    return std::bind_front(std::forward<TArgs>(args)...);
  }
};

}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind::inline bind_core {

namespace {
constexpr auto const& bind_front
  = bind_detail::static_const<bind_detail::bind_front_fn>;
}

}// namespace FredEmmott::bind