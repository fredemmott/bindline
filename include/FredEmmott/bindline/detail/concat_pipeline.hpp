// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/config.hpp>
#include <FredEmmott/bindline/detail/traced_bindline.hpp>

namespace FredEmmott::bindline_detail {

template <class TLeft, class TBindable, class TBound>
constexpr auto wrap_traced(TBound&& bound) {
  if constexpr (std::derived_from<TLeft, traced_bindline_marker_t>) {
    using next_t = typename TLeft::template next_t<TBound, TBindable>;
    return next_t {.fn = std::forward<TBound>(bound)};
  } else {
    return traced_bindline<std::decay_t<TBound>, TBindable> {
      .fn = std::forward<TBound>(bound),
    };
  }
}

template <class T>
constexpr decltype(auto) unwrap_traced(T&& lhs) {
  if constexpr (std::derived_from<T, traced_bindline_marker_t>) {
    return std::move(lhs.fn);
  } else {
    return std::forward<T>(lhs);
  }
}

template <class TLeft, class TRight>
constexpr auto concat_pipeline(TLeft&& lhs, TRight&& rhs) {
  auto bound = rhs.bind_to(unwrap_traced(std::forward<TLeft>(lhs)));
  return wrap_traced<TLeft, TRight>(std::move(bound));
}

}// namespace FredEmmott::bindline_detail