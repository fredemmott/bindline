// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../cppwinrt/concepts.hpp"
#include "../cppwinrt.hpp"

namespace FredEmmott::weak_refs_extensions {

using namespace FredEmmott::cppwinrt::cppwinrt_concepts;

template <class T>
concept winrt_raw_pointer_with_weak_ref = winrt_raw_pointer<T>
  && !winrt_implements<std::remove_pointer_t<T>, winrt::no_weak_ref>;

template <winrt_raw_pointer_with_weak_ref T>
struct make_weak_ref_fn<T> {
  static constexpr auto make(auto&& value) {
    return value->get_weak();
  }
};

}// namespace FredEmmott::weak_refs_extensions