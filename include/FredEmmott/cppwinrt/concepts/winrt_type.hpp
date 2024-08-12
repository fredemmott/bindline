// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/base.h>

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {

/** A WinRT interface or implementation.
 *
 * Use `winrt_implementation` or `winrt_interface` for a more specific check.
 */
template <class T>
concept winrt_type = std::
  convertible_to<std::decay_t<T>, winrt::Windows::Foundation::IInspectable>;

}// namespace FredEmmott::cppwinrt::inline cppwinrt_concepts