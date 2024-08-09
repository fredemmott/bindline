// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../cppwinrt/bind_context.hpp"

namespace FredEmmott::bind_detail {

template <class T>
struct winrt_context_binder_t : ::FredEmmott::bind::bindable_t {
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
      mContext, std::forward<TFn>(fn));
  }

 private:
  T mContext;
};

}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind {
template <::FredEmmott::cppwinrt_detail::switchable_context T>
[[nodiscard]]
constexpr auto bind_winrt_context(T&& context) {
  return ::FredEmmott::bind_detail::winrt_context_binder_t<std::decay_t<T>>(
    std::forward<T>(context));
}

template <class TFn, ::FredEmmott::cppwinrt_detail::switchable_context T>
[[nodiscard]]
constexpr auto bind_winrt_context(TFn&& fn, T&& context) {
  auto binder = bind_winrt_context(std::forward<T>(context));
  return binder.bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::bind