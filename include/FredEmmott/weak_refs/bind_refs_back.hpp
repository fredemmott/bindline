// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <tuple>
#include <utility>

#include "detail/bind_back_traits.hpp"
#include "detail/binder_t.hpp"

namespace FredEmmott::weak_refs {

template <class F, convertible_to_weak_ref... Binds>
  requires(sizeof...(Binds) >= 1)
auto bind_refs_back(F&& f, Binds&&... binds) {
  using namespace weak_refs_detail;
  return binder_t<
    bind_back_traits,
    std::decay_t<F>,
    weak_ref_t<std::decay_t<Binds>>...>(
    std::forward<F>(f), std::forward<Binds>(binds)...);
}

}// namespace FredEmmott::weak_refs