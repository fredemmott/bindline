// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "concepts.hpp"
#include "detail/static_const.hpp"

namespace FredEmmott::weak_refs_detail {
struct make_weak_ref_fn {
  template <weak_refs::weak_ref T>
  [[nodiscard]]
  constexpr auto operator()(T&& weak) const {
    return std::forward<T>(weak);
  }

  template <weak_refs::convertible_to_weak_ref T>
    requires(!weak_refs::weak_ref<T>)
  [[nodiscard]]
  constexpr auto operator()(T&& strong) const {
    return weak_refs_extensions::make_weak_ref_fn<std::decay_t<T>>::make(
      std::forward<T>(strong));
  }
};
}// namespace FredEmmott::weak_refs_detail

namespace FredEmmott::weak_refs {

/** Convert a pointer or `strong_ref` to a `weak_ref`.
 *
 * `convertible_to_weak_ref` includes:
 * - `strong_ref`s
 * - raw pointers of types that extend `std::shared_ptr<>`
 * - raw pointers to WinRT implementation types that do not implement
 *   `winrt::no_weak_ref`
 *
 * This is implemented using the pattern described in
 * "Suggested Design For Customization Points" (https://wg21.link/N4381)
 *
 * For this case, it's particular important that functors don't participate
 * in ADL, to avoid recursive compile-time tests for wether or not
 * the function is usable.
 *
 * For example, if I make `weak_refs::make_weak_ref()` available in namespace
 * `Foo`, it shouldn't be used in ADL for `make_ref(Foo::Bar{})`.
 */
namespace {
constexpr auto const& make_weak_ref
  = weak_refs_detail::static_const<weak_refs_detail::make_weak_ref_fn>;
}// namespace

}// namespace FredEmmott::weak_refs