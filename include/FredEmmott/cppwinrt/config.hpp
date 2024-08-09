// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#if __has_include(<FredEmmott.cppwinrt.config-tweaks.hpp>)
#include <FredEmmott.cppwinrt.config-tweaks.hpp>
#endif

#ifndef FREDEMMOTT_CPPWINRT_ENABLE_WIL
#if __has_include(<wil/cppwinrt_helpers.h>)
#define FREDEMMOTT_CPPWINRT_ENABLE_WIL true
#else
#define FREDEMMOTT_CPPWINRT_ENABLE_WIL false
#endif
#endif