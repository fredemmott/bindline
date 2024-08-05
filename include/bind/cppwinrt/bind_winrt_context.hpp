// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../cppwinrt/bind_context.hpp"
#include "../bind_front.hpp"

namespace FredEmmott::bind_detail {

struct bind_winrt_context_t {};

template <class Context, class Fn, class... Args>
constexpr auto adl_bind_front(
  bind_winrt_context_t,
  Fn&& fn,
  Context&& context,
  Args&&... args) {
  using namespace FredEmmott::bind;
  // Fully-qualify so that ADL doesn't lead us to `std::bind_front()` if `fn` is
  // an `std::function<>`
  const auto next = ::FredEmmott::bind::bind_front(
    std::forward<Fn>(fn), std::forward<Args>(args)...);

  return FredEmmott::cppwinrt::bind_context(
    std::forward<Context>(context), next);
}

}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind::inline cppwinrt_bind_tags {
// Marker for ADL
constexpr bind_detail::bind_winrt_context_t bind_winrt_context;
}// namespace FredEmmott::bind::inline cppwinrt_bind_tags