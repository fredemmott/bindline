// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/weak_refs.hpp>

#include <tuple>
#include <utility>

#include "detail/bind_back_traits.hpp"
#include "detail/binder_t.hpp"

namespace FredEmmott::weak_refs {

/** Like `std::bind_back()`, but stores weak references and invokes with strong
 * references.
 *
 * If the resulting strong references are invalid, `f` will not be invoked.
 *
 * The returned callable returns `void`, as the function might not be invoked.
 */
template <class F, convertible_to_weak_ref... Binds>
  requires(sizeof...(Binds) >= 1)
[[nodiscard]]
constexpr auto bind_refs_back(F&& f, Binds&&... binds) {
  using namespace weak_refs_detail;
  return binder_t<
    bind_back_traits,
    std::decay_t<F>,
    weak_ref_t<std::decay_t<Binds>>...>(
    std::forward<F>(f), std::forward<Binds>(binds)...);
}

}// namespace FredEmmott::weak_refs