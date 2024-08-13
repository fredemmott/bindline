// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/extension_api/bindable_t.hpp>

#include "detail/byval_binder_t.hpp"

namespace FredEmmott::bindline_detail {

template <std::derived_from<::FredEmmott::bindline_extension_api::bindable_t> T>
struct bind_suppress_validation_t
  : ::FredEmmott::bindline_extension_api::bindable_t {
  static_assert(std::same_as<std::decay_t<T>, T>);
  bind_suppress_validation_t() = delete;
  template <std::convertible_to<T> TInit>
  constexpr bind_suppress_validation_t(TInit&& next)
    : mWrapped(std::forward<TInit>(next)) {
  }
  T mWrapped;

  template <class TFn>
  [[nodiscard]]
  constexpr auto bind_to(TFn&& fn) const {
    return mWrapped.bind_to(std::forward<TFn>(fn));
  }
};

}// namespace FredEmmott::bindline_detail

namespace FredEmmott::bindline {

template <std::derived_from<::FredEmmott::bindline_extension_api::bindable_t> T>
[[nodiscard]]
constexpr auto bind_suppress_validation(T&& wrapped) {
  return ::FredEmmott::bindline_detail::bind_suppress_validation_t<
    std::decay_t<T>> {std::forward<T>(wrapped)};
}

}// namespace FredEmmott::bindline