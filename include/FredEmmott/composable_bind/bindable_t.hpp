// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <concepts>
#include <functional>
#include <tuple>

namespace FredEmmott::composable_bind_detail {
template <template <class...> class T, class TProjection, class... TArgs>
struct bindable_factory_t;
}

namespace FredEmmott::composable_bind {

struct bindable_t {
  [[nodiscard]]
  constexpr auto bind_to(auto&& fn) const
    = delete;

  template <template <class...> class T, class TProjection, class... TArgs>
  [[nodiscard]]
  static constexpr auto make_projected(
    TProjection&& projection,
    TArgs&&... args) {
    return ::FredEmmott::composable_bind_detail::
      bindable_factory_t<T, std::decay_t<TProjection>, std::decay_t<TArgs>...>::
        make(
          std::forward<TProjection>(projection), std::forward<TArgs>(args)...);
  }

  template <template <class...> class T, class... TArgs>
  [[nodiscard]]
  static constexpr auto make(TArgs&&... args) {
    return make_projected<T, std::identity, TArgs...>(
      {}, std::forward<TArgs>(args)...);
  }
};

template <class TFunction, std::derived_from<bindable_t> TBindable>
[[nodiscard]]
constexpr auto operator|(TFunction&& lhs, TBindable&& rhs) {
  return rhs.bind_to(std::forward<TFunction>(lhs));
}

}// namespace FredEmmott::composable_bind

#include "detail/bindable_factory_t.hpp"