// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <atomic>
#include <concepts>

#include "bindable_t.hpp"

namespace FredEmmott::bind {

template <template <class...> class TNext, class TFirst, class... TRest>
struct invocable_bindable_t : bindable_t {
  static_assert(
    std::same_as<TFirst, std::decay_t<TFirst>>
    && (std::same_as<TRest, std::decay_t<TRest>> && ...));

  using bindable_t = TNext<TFirst, TRest...>;
  using invocable_t = TNext<TRest...>;

  template <class... TInit>
  constexpr invocable_bindable_t(TInit&&... args)
    : mArgs(std::forward_as_tuple(std::forward<TInit>(args)...)) {
  }

  template <class TFn>
    requires requires(bindable_t bindable, TFn fn) { bindable.bind_to(fn); }
  constexpr auto bind_to(const TFn& fn) const {
    const auto next = std::apply(
      []<class... NextArgs>(NextArgs&&... nextArgs) {
        return bindable_t {std::forward<NextArgs>(nextArgs)...};
      },
      mArgs);

    return next.bind_to(fn);
  }

  template <class... Unbound>
    requires std::invocable<
      decltype(std::declval<invocable_t>().bind_to(std::declval<TFirst>())),
      Unbound...>
  constexpr auto operator()(Unbound&&... unbound) const {
    const auto next = std::apply(
      []<class... NextArgs>(auto, NextArgs&&... nextArgs) {
        // Ignored first arg is the function we're invoking
        return invocable_t {std::forward<NextArgs>(nextArgs)...};
      },
      mArgs);
    return std::invoke(
      next.bind_to(get<0>(mArgs)), std::forward<Unbound>(unbound)...);
  }

 private:
  std::tuple<TFirst, TRest...> mArgs;
};

template <template <class...> class T, class... TArgs>
[[nodiscard]]
constexpr auto make_bindable(TArgs&&... args) {
  return T<std::decay_t<TArgs>...>(std::forward<TArgs>(args)...);
}

template <template <class...> class T, class TFirst, class... TRest>
  requires std::constructible_from<
             T<std::decay_t<TFirst>, std::decay_t<TRest>...>,
             TFirst,
             TRest...>
  && std::constructible_from<T<std::decay_t<TRest>...>, TRest...>
[[nodiscard]]
constexpr auto make_bindable(TFirst&& first, TRest&&... rest) {
  return invocable_bindable_t<T, std::decay_t<TFirst>, std::decay_t<TRest>...> {
    std::forward<TFirst>(first),
    std::forward<TRest>(rest)...,
  };
}

}// namespace FredEmmott::bind