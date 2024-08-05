// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "extensions.hpp"

namespace FredEmmott::weak_refs::inline weak_refs_ref_types {

/// e.g. `std::weak_ptr<T>`
template <class TStrong>
using weak_ref_t
  = decltype(weak_refs_extensions::make_weak_ref_fn<
             std::decay_t<TStrong>>::make(std::declval<TStrong>()));
/// e.g. `std::strong_ptr<T>`
template <class TWeak>
using strong_ref_t
  = decltype(weak_refs_extensions::lock_weak_ref_fn<std::decay_t<TWeak>>::lock(
    std::declval<TWeak>()));

}// namespace FredEmmott::weak_refs::inline weak_refs_ref_types