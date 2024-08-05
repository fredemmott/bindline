// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../cppwinrt/concepts.hpp"
#include "base.hpp"

#include <winrt/Windows.Foundation.h>

#include <memory>

namespace FredEmmott::weak_refs_extensions {

using namespace FredEmmott::cppwinrt::cppwinrt_concepts;

template <cppwinrt_strong_ref T>
struct make_weak_ref_fn<T> {
  template <class TValue>
  static constexpr auto make(TValue&& value) {
    return winrt::make_weak(std::forward<TValue>(value));
  }
};

template <cppwinrt_weak_ref T>
struct lock_weak_ref_fn<T> {
  static constexpr auto lock(auto&& value) {
    return value.get();
  }
};

}// namespace FredEmmott::weak_refs_extensions

namespace FredEmmott::weak_refs {
static_assert(convertible_to_weak_ref<winrt::Windows::Foundation::IStringable>);
static_assert(strong_ref<winrt::Windows::Foundation::IStringable>);
static_assert(!weak_ref<winrt::Windows::Foundation::IStringable>);

static_assert(!strong_ref<decltype(winrt::make_weak(
                winrt::Windows::Foundation::IStringable {nullptr}))>);
static_assert(!strong_ref<winrt::Windows::Foundation::IStringable*>);
static_assert(weak_ref<decltype(winrt::make_weak(
                winrt::Windows::Foundation::IStringable {nullptr}))>);
}// namespace FredEmmott::weak_refs