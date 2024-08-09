// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindable_t.hpp"

namespace FredEmmott::bind_detail {

template <class TTap>
struct bind_tap_t : public ::FredEmmott::bind::bindable_t {
  static_assert(std::same_as<TTap, std::decay_t<TTap>>);

  bind_tap_t() = delete;
  template <class TInit>
  constexpr bind_tap_t(TInit&& tap) : mTap(std::forward<TTap>(tap)) {
  }

  template <class TFn>
  [[nodiscard]]
  constexpr auto bind_to(TFn&& f) const {
    return std::bind_front(
      []<class... Args>(Args&&... args) {
        return bind_tap_t<TTap>::invoke(std::forward<Args>(args)...);
      },
      std::forward<TFn>(f),
      mTap);
  }

 private:
  TTap mTap;
  template <class TFn, class TInvokeTap, class... TUnbound>
  constexpr static auto
  invoke(TFn&& fn, TInvokeTap&& tap, TUnbound&&... unbound) {
    static_assert(
      std::invocable<TFn, TUnbound...>,
      "The function is not invocable with the provided arguments");
    static_assert(
      std::invocable<TInvokeTap, TUnbound...>,
      "The tap is not invocable with the provided arguments");

    std::invoke(tap, static_cast<const TUnbound&>(unbound)...);
    return std::invoke(
      std::forward<TFn>(fn), std::forward<TUnbound>(unbound)...);
  }
};
template <class T>
bind_tap_t(T) -> bind_tap_t<std::decay_t<T>>;

}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {
template <class T>
[[nodiscard]]
constexpr auto bind_tap(T&& tap) {
  auto x = std::bind_front(std::forward<T>(tap));
  return bind_detail::bind_tap_t(std::forward<T>(tap));
}

template <class TFn, class TTap>
[[nodiscard]]
constexpr auto bind_tap(TFn&& fn, TTap&& tap) {
  return bind_tap(std::forward<TTap>(tap)).bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bind