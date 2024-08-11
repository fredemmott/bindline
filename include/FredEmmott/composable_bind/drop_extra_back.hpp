// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "detail/drop_any_arg_traits.hpp"
#include "extension_api/drop_back_bindable_t.hpp"

namespace FredEmmott::composable_bind {

template <class TFn, class... TArgs>
concept drop_extra_back_invocable
  = ::FredEmmott::composable_bind_extension_api::drop_back_invocable<
    ::FredEmmott::composable_bind_detail::drop_any_arg_traits,
    TFn,
    TArgs...>;

[[nodiscard]]
constexpr auto drop_extra_back() {
  return ::FredEmmott::composable_bind_extension_api::drop_back_bindable_t<
    ::FredEmmott::composable_bind_detail::drop_any_arg_traits> {};
}

template <class TFn>
[[nodiscard]]
constexpr auto drop_extra_back(TFn&& fn) {
  return drop_extra_back().bind_to(std::forward<TFn>(fn));
}

}// namespace FredEmmott::composable_bind