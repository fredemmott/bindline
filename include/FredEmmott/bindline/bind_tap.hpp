// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindable_t.hpp"

namespace FredEmmott::bindline_detail {

template <class TTap>
struct bind_tap_t : public ::FredEmmott::bindline::bindable_t {
  static_assert(std::same_as<TTap, std::decay_t<TTap>>);

  bind_tap_t() = delete;
  template <class TInit>
  constexpr bind_tap_t(TInit&& tap) : mTap(std::forward<TTap>(tap)) {
  }

  template <class TFn>
  [[nodiscard]]
  constexpr auto bind_to(TFn&& f) const {
    return std::bind_front(
      []<class TInvokeFn, class TInvokeTap, class... TInvokeArgs>(
        TInvokeFn&& fn, TInvokeTap&& tap, TInvokeArgs&&... args)
        requires std::invocable<TInvokeFn, TInvokeArgs...>
                   && std::invocable<TInvokeTap, TInvokeArgs...>
      {
        std::invoke(
          std::forward<TInvokeTap>(tap),
          static_cast<const TInvokeArgs&>(args)...);
        return std::invoke(
          std::forward<TInvokeFn>(fn), std::forward<TInvokeArgs>(args)...);
      },
      std::forward<TFn>(f),
      mTap);
  }

 private:
  TTap mTap;
};
template <class T>
bind_tap_t(T) -> bind_tap_t<std::decay_t<T>>;

}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {
/** Invoke an additional function before executing the original.
 *
 * The tap function does not affect execution of the original, or return value.
 *
 * This is primarily useful for debugging or logging.
 *
 * Usage: `wrapped_function | bind_tap(tap_function)`
 */
template <class T>
[[nodiscard]]
constexpr auto bind_tap(T&& tap) {
  auto x = std::bind_front(std::forward<T>(tap));
  return bindline_detail::bind_tap_t(std::forward<T>(tap));
}

/** Invoke an additional function before executing the original.
 *
 * The tap function does not affect execution of the original, or return value.
 *
 * This is primarily useful for debugging or logging.
 *
 * Usage: `bind_tap(wrapped_function, tap_function)`
 */
template <class TFn, class TTap>
[[nodiscard]]
constexpr auto bind_tap(TFn&& fn, TTap&& tap) {
  return bind_tap(std::forward<TTap>(tap)).bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bindline