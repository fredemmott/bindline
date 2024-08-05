// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "concepts.hpp"
#include "detail/static_const.hpp"

#include <utility>

/**
 * This is implemented using the pattern described in
 * "Suggested Design For Customization Points" (https://wg21.link/N4381)
 *
 * For this case, it's particular important that functors don't participate
 * in ADL, to avoid recursive compile-time tests for wether or not
 * the function is usable.
 *
 * For example, if I make `weak_refs::lock_weak_ref()` available in namespace
 * `Foo`, it shouldn't be used in ADL for `weak_ref(Foo::Bar {})`.
 */

namespace FredEmmott::weak_refs_detail {
struct lock_weak_ref_fn {
  template <weak_refs::weak_ref T>
  constexpr auto operator()(T&& weak) const {
    return weak_refs_extensions::lock_weak_ref_fn<std::decay_t<T>>::lock(
      std::forward<T>(weak));
  }
};

}// namespace FredEmmott::weak_refs_detail

namespace FredEmmott::weak_refs {

namespace {
constexpr auto const& lock_weak_ref
  = weak_refs_detail::static_const<weak_refs_detail::lock_weak_ref_fn>;
}// namespace

}// namespace FredEmmott::weak_refs