// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../cppwinrt/concepts.hpp"

namespace FredEmmott::bind_detail::cppwinrt {
using namespace ::FredEmmott::cppwinrt::cppwinrt_concepts;

template <class... Args>
struct bind_trace_t {};

struct discard_winrt_event_args_t {};

template <class TNext, class BindTrace>
struct discard_winrt_event_args_fn {
  TNext mNext;

  template <cppwinrt_type TSender, cppwinrt_type TEventArgs, class... Extras>
    requires std::invocable<TNext, Extras...>
  constexpr auto
  operator()(Extras&&... extras, const TSender&, const TEventArgs&) const {
    return std::invoke(mNext, std::forward<Extras>(extras)...);
  }
};

template <class Fn, class... Args>
constexpr auto
adl_bind_front(discard_winrt_event_args_t, Fn&& fn, Args&&... args) {
  // Fully-qualify so that ADL doesn't lead us to `std::bind_front()` if `fn` is
  // an `std::function<>`
  const auto next = ::FredEmmott::bind::bind_front(
    std::forward<Fn>(fn), std::forward<Args>(args)...);

  return discard_winrt_event_args_fn<
    std::decay_t<decltype(next)>,
    bind_trace_t<Args...>> {next};
}
}// namespace FredEmmott::bind_detail::cppwinrt

namespace FredEmmott::bind::inline cppwinrt_bind_tags {
constexpr bind_detail::cppwinrt::discard_winrt_event_args_t
  discard_winrt_event_args {};

}