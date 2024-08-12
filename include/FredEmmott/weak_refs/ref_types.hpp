// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "extensions.hpp"

namespace FredEmmott::weak_refs::inline weak_refs_ref_types {

/** The corresponding weak ref type for a given strong ref type
 *
 * For example, `weak_ref_t<std::shared_ptr<T>>` is `std::weak_ptr<T>`
 */
template <class TStrong>
using weak_ref_t
  = decltype(weak_refs_extensions::make_weak_ref_fn<
             std::decay_t<TStrong>>::make(std::declval<TStrong>()));

/** The corresponding strong ref type for a given weakref type
 *
 * For example, `strong_ref_t<std::weak_ptr<T>>` is `std::shared_ptr<T>`
 */
template <class TWeak>
using strong_ref_t
  = decltype(weak_refs_extensions::lock_weak_ref_fn<std::decay_t<TWeak>>::lock(
    std::declval<TWeak>()));

}// namespace FredEmmott::weak_refs::inline weak_refs_ref_types