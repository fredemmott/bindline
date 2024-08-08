// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "weak_refs/config.hpp"

/*
 * Helpers for writing templates that deal with both
 * std::weak_ptr and winrt::weak_ref.
 *
 * # Concepts
 *
 * See `weak_refs/concepts.hpp` for details:
 *
 * - `strong_ref<T>`: e.g. `std::shared_ptr<T>`
 * - `weak_ref<T>`: e.g. `std::weak_ptr<T>`
 * - `convertible_to_weak_ref<T>`: either a `weak_ref<T>`, or something else
 *   that can be converted to one, e.g. a `strong_ref<T>`, or `this` when
 *   extending `std::enable_shared_from_this<T>`
 *
 * # Types
 *
 * - strong_ref_t<TWeak>: the corresponding strong type
 * - weak_ref_t<TStrong>: the corresponding weak type
 *
 * # Functions
 *
 * - `make_weak_ref()`: creates a `weak_ref`
 * - `lock_weak_ref()`: upgrades a `weak_ref` to a `strong_ref`
 * - `bind_maybe_refs_front()`: like `std::bind_front()`, but automatically
 *    converts `convertible_to_weak_ref()` parameters to weak, then back to
 *    strong
 * - `bind_front(maybe_refs, ...)`: tag-based ADL alias for
 *   `bind_maybe_refs_front()`
 */

#include "weak_refs/bind_refs_back.hpp"
#include "weak_refs/bind_refs_front.hpp"
#include "weak_refs/concepts.hpp"

#if FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT
#include "weak_refs/cppwinrt.hpp"
#endif

#if FREDEMMOTT_WEAK_REFS_ENABLE_STD_SHARED_PTR
#include "weak_refs/std_shared_ptr.hpp"
#endif

#if FREDEMMOTT_WEAK_REFS_ENABLE_WEAK_FROM_THIS
#include "weak_refs/weak_from_this.hpp"
#endif