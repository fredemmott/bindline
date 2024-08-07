// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <concepts>

#include "ref_types.hpp"

namespace FredEmmott::weak_refs::inline weak_refs_concepts {

/** An object without keep-alive semantics, but can be 'locked' to produce one
 * with keep-alive semantics.
 *
 * e.g. `std::weak_ptr`
 */
template <class T>
concept weak_ref = requires(T v) {
  {
    weak_refs_extensions::lock_weak_ref_fn<std::decay_t<T>>::lock(v)
  } -> std::copyable;
};

/** Either a weak_ref, or an object that can be converted to a `weak_ref`.
 *
 * For example, an object implementing `get_weak()` via
 * `std::enable_shared_from_this`
 */
template <class T>
concept convertible_to_weak_ref
  = weak_ref<T> || weak_ref<weak_ref_t<std::decay_t<T>>>;

/** A pointer-like object with 'keep-alive' semantics.
 *
 * e.g. `std::shared_ptr`
 *
 * It is possible for these to hold an invalid reference, e.g. `nullptr`
 */
template <class T>
concept strong_ref = convertible_to_weak_ref<T>
  && std::same_as<std::decay_t<T>, strong_ref_t<weak_ref_t<T>>>;

}// namespace FredEmmott::weak_refs::inline weak_refs_concepts