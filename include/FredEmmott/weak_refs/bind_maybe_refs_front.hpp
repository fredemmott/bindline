// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include "detail/front_binder.hpp"

namespace FredEmmott::weak_refs {

template <class F, class First, class... Rest>
auto bind_maybe_refs_front(F&& f, First&& first, Rest&&... rest) {
  static_assert(
    convertible_to_weak_ref<std::decay_t<First>>
      || !std::is_member_function_pointer_v<std::decay_t<F>>,
    "For safety, refusing to bind object pointer as the first argument to a "
    "member function pointer, as the object pointer can not be converted to "
    "a "
    "weak ref");
  const auto ret = weak_refs_detail::front_binder<
    weak_refs_detail::NotARefBehavior::Passthrough,
    std::decay_t<F>,
    std::decay_t<First>,
    std::decay_t<Rest>...>(
    std::forward<F>(f),
    std::forward<First>(first),
    std::forward<Rest>(rest)...);
  static_assert(!decltype(ret)::refs_required_v);
  return ret;
}

}// namespace FredEmmott::weak_refs