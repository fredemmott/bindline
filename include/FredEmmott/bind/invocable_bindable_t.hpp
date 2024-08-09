// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <atomic>
#include <concepts>

#include "bindable_t.hpp"

namespace FredEmmott::bind {

template <template <class...> class TNext, class TFirst, class... TRest>
struct invocable_bindable_t : bindable_t {
  template <class... TInit>
  constexpr invocable_bindable_t(TInit&&... args)
    : mArgs(std::forward_as_tuple(std::forward<TInit>(args)...)) {
  }

  constexpr auto bind_to(const auto& fn) const {
    const auto next = std::apply(
      []<class... NextArgs>(NextArgs&&... nextArgs) {
        return TNext<TFirst, TRest...> {std::forward<NextArgs>(nextArgs)...};
      },
      mArgs);

    return next.bind_to(fn);
  }

  template <class... Unbound>
  constexpr auto operator()(Unbound&&... unbound) const {
    const auto next = std::apply(
      []<class... NextArgs>(auto, NextArgs&&... nextArgs) {
        // Ignored first arg is the function we're invoking
        return TNext<TRest...> {std::forward<NextArgs>(nextArgs)...};
      },
      mArgs);
    return std::invoke(
      next.bind_to(get<0>(mArgs)), std::forward<Unbound>(unbound)...);
  }

 private:
  std::tuple<TFirst, TRest...> mArgs;
};

template <template <class...> class T, class... TArgs>
auto make_bindable(TArgs&&... args) {
  return T<std::decay_t<TArgs>...>(std::forward<TArgs>(args)...);
}

template <template <class...> class T, class TFirst, class... TRest>
  requires std::constructible_from<
             T<std::decay_t<TFirst>, std::decay_t<TRest>...>,
             TFirst,
             TRest...>
  && std::constructible_from<T<std::decay_t<TRest>...>, TRest...>
auto make_bindable(TFirst&& first, TRest&&... rest) {
  return invocable_bindable_t<T, std::decay_t<TFirst>, std::decay_t<TRest>...> {
    std::forward<TFirst>(first),
    std::forward<TRest>(rest)...,
  };
}

}// namespace FredEmmott::bind