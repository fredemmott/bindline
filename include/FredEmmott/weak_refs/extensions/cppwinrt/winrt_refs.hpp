// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/cppwinrt/concepts.hpp>
#include <FredEmmott/weak_refs/extensions.hpp>

namespace FredEmmott::weak_refs_extensions {

using namespace FredEmmott::cppwinrt::cppwinrt_concepts;

template <class T>
concept winrt_strong_ref_with_weak_ref = winrt_strong_ref<T>
  && !winrt_implements<std::remove_pointer_t<T>, winrt::no_weak_ref>;

template <winrt_strong_ref_with_weak_ref T>
struct make_weak_ref_fn<T> {
  template <class TValue>
  [[nodiscard]]
  static constexpr auto make(TValue&& value) {
    return winrt::make_weak(std::forward<TValue>(value));
  }
};

template <winrt_weak_ref T>
struct lock_weak_ref_fn<T> {
  [[nodiscard]]
  static constexpr auto lock(auto&& value) {
    return value.get();
  }
};

}// namespace FredEmmott::weak_refs_extensions
