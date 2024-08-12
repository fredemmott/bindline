// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "winrt_type.hpp"

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {

/// A WinRT implementation type, as opposed to a `winrt_interface
template <class T>
concept winrt_implementation
  = winrt_type<T> && requires { typename T::implements_type; };

}// namespace FredEmmott::cppwinrt::inline cppwinrt_concepts