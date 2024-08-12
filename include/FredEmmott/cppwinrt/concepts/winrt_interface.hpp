// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "winrt_implementation.hpp"

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {

template <class T>
concept winrt_interface = winrt_type<T> && !winrt_implementation<T>;

}// namespace FredEmmott::cppwinrt::inline cppwinrt_concepts