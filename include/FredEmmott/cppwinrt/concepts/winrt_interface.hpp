// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "winrt_implementation.hpp"

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {

/** A WinRT interface type, as opposed to an implementation.
 *
 * This *can not* be used to test if a WinRT implementation type implements a
 * particular interface; use `winrt_implements` for that.
 */
template <class T>
concept winrt_interface = winrt_type<T> && !winrt_implementation<T>;

}// namespace FredEmmott::cppwinrt::inline cppwinrt_concepts