// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <tuple>
#include <utility>

#include "detail/front_binder.hpp"

namespace FredEmmott::weak_refs {

template <class F, convertible_to_weak_ref... Binds>
  requires(sizeof...(Binds) >= 1)
auto bind_refs_front(F&& f, Binds&&... binds) {
  return weak_refs_detail::
    front_binder<std::decay_t<F>, weak_ref_t<std::decay_t<Binds>>...>(
      std::forward<F>(f), std::forward<Binds>(binds)...);
}

}// namespace FredEmmott::weak_refs