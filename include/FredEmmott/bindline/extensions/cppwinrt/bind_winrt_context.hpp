// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/cppwinrt/bind_context.hpp>

namespace FredEmmott::bindline_detail {

template <class T>
struct winrt_context_binder_t
  : ::FredEmmott::bindline_extension_api::bindable_t {
  static_assert(std::same_as<T, std::decay_t<T>>);

  winrt_context_binder_t() = delete;

  template <std::convertible_to<T> TInit>
  constexpr winrt_context_binder_t(TInit&& v)
    : mContext(std::forward<TInit>(v)) {
  }

  template <class TFn>
  [[nodiscard]]
  constexpr auto bind_to(TFn&& fn) const {
    return ::FredEmmott::cppwinrt::bind_context(
      std::forward<TFn>(fn), mContext);
  }

 private:
  T mContext;
};

}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

/** Bind execution to a given `winrt::apartment_context` or `DispatcherQueue`.
 *
 * Usage: `(f | bind_context(dispatcherQueue))`
 */
template <::FredEmmott::cppwinrt_detail::switchable_context T>
[[nodiscard]]
constexpr auto bind_winrt_context(T&& context) {
  return ::FredEmmott::bindline_detail::winrt_context_binder_t<std::decay_t<T>>(
    std::forward<T>(context));
}

/** Bind execuction to a given `winrt::apartment_context` or `DispatcherQueue`.
 *
 * Usage: `bind_context(f, dispatcherQueue)`
 */
template <class TFn, ::FredEmmott::cppwinrt_detail::switchable_context T>
[[nodiscard]]
constexpr auto bind_winrt_context(TFn&& fn, T&& context) {
  auto binder = bind_winrt_context(std::forward<T>(context));
  return binder.bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bindline