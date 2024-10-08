// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/cppwinrt/concepts.hpp>
#include <FredEmmott/weak_refs/extensions.hpp>

namespace FredEmmott::weak_refs_extensions {

using namespace FredEmmott::cppwinrt::cppwinrt_concepts;

template <class T>
concept winrt_raw_pointer_with_weak_ref = winrt_raw_pointer<T>
  && !winrt_implements<std::remove_pointer_t<T>, winrt::no_weak_ref>;

/** Make a `winrt_weak_ref` from a WinRT `this`
 *
 * Unless they implement `winrt::no_weak_ref`, WinRT implementation classes have
 * a feature similar to `std::enable_shared_from_this`:
 *
 * https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/weak-references
 */
template <winrt_raw_pointer_with_weak_ref T>
struct make_weak_ref_fn<T> {
  [[nodiscard]]
  static constexpr auto make(auto&& value) {
    return value->get_weak();
  }
};

}// namespace FredEmmott::weak_refs_extensions