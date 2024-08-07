// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <tuple>
#include <utility>

#include "detail/front_binder.hpp"

namespace FredEmmott::weak_refs {

template <class F, convertible_to_weak_ref... Binds>
auto bind_refs_front(F&& f, Binds&&... binds) {
  const auto ret = weak_refs_detail::front_binder<
    weak_refs_detail::NotARefBehavior::Error,
    std::decay_t<F>,
    std::decay_t<Binds>...>(std::forward<F>(f), std::forward<Binds>(binds)...);
  static_assert(decltype(ret)::refs_required_v);
  return ret;
}

}// namespace FredEmmott::weak_refs