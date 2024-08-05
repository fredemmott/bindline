// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bind_maybe_refs_front.hpp"

#include <tuple>
#include <utility>

namespace FredEmmott::weak_refs {

template <class F, convertible_to_weak_ref... Binds>
auto bind_refs_front(F&& f, Binds&&... binds) {
  const auto ret = bind_maybe_refs_detail::front_binder<
    bind_maybe_refs_detail::NotARefBehavior::Error,
    std::decay_t<F>,
    std::decay_t<Binds>...>(std::forward<F>(f), std::forward<Binds>(binds)...);
  static_assert(decltype(ret)::refs_required_v);
  return ret;
}

}// namespace FredEmmott::weak_refs