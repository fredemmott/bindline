// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../cppwinrt/concepts.hpp"

namespace FredEmmott::weak_refs_extensions {

using namespace FredEmmott::cppwinrt::cppwinrt_concepts;

template <winrt_strong_ref T>
struct make_weak_ref_fn<T> {
  template <class TValue>
  static constexpr auto make(TValue&& value) {
    return winrt::make_weak(std::forward<TValue>(value));
  }
};

template <winrt_weak_ref T>
struct lock_weak_ref_fn<T> {
  static constexpr auto lock(auto&& value) {
    return value.get();
  }
};

}// namespace FredEmmott::weak_refs_extensions
