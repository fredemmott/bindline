// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "factory_concepts.hpp"
#include "invocable_bindable_t.hpp"

namespace FredEmmott::bindline_detail {

template <template <class...> class T, class TProjection, class... TArgs>
struct bindable_factory_t {
  static_assert((std::same_as<TArgs, std::decay_t<TArgs>> && ...));

  template <class TFirst, class... TRest>
    requires is_bindable<T, TProjection, TFirst, TRest...>
    && (!is_invocable<T, TProjection, TFirst, TRest...>)
  [[nodiscard]]
  static constexpr auto make(
    TProjection projection,
    TFirst&& first,
    TRest&&... rest) {
    return T<projected_t<TProjection, TArgs>...> {
      std::invoke(projection, std::forward<TFirst>(first)),
      std::invoke(projection, std::forward<TRest>(rest))...,
    };
  }

  template <class TFirst, class... TRest>
    requires is_bindable<T, TProjection, TFirst, TRest...>
    && is_invocable<T, TProjection, TFirst, TRest...>
  [[nodiscard]]
  static constexpr auto
  make(TProjection projection, TFirst&& first, TRest&&... rest) {
    return invocable_bindable_t<T, projected_t<TProjection, TArgs>...> {
      std::invoke(projection, std::forward<TFirst>(first)),
      std::invoke(projection, std::forward<TRest>(rest)...),
    };
  }

  template <class TFirst, class... TRest>
    requires is_invocable<T, TProjection, TFirst, TRest...>
    && (!is_bindable<T, TProjection, TFirst, TRest...>)
  [[nodiscard]]
  static constexpr auto make(
    TProjection projection,
    TFirst&& first,
    TRest&&... rest) {
    T<projected_t<TProjection, TRest>...> bindable {
      std::invoke(projection, std::forward<TRest>(rest))...,
    };
    return bindable.bind_to(std::forward<TFirst>(first));
  }
};
}// namespace FredEmmott::bindline_detail