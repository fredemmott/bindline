// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#if __has_include(<FredEmmott.bindline.config-tweaks.hpp>)
#include <FredEmmott.bindline.config-tweaks.hpp>
#endif

#ifndef FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES
#define FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES true
#endif

//-----
#ifndef FREDEMMOTT_BINDLINE_ENABLE_WEAK_REFS
#define FREDEMMOTT_BINDLINE_ENABLE_WEAK_REFS \
  FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES
#endif

//-----
#ifndef FREDEMMOTT_BINDLINE_ENABLE_CPPWINRT
#if __has_include(<winrt/base.h>)
#define FREDEMMOTT_BINDLINE_ENABLE_CPPWINRT \
  FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES
#else
#define FREDEMMOTT_BINDLINE_ENABLE_CPPWINRT false
#endif
#endif

//-----
#ifndef FREDEMMOTT_BINDLINE_ENABLE_BIND_BACK

#if FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES

#include <version>
#ifdef __cpp_lib_bind_back
#define FREDEMMOTT_BINDLINE_ENABLE_BIND_BACK true
#else
#define FREDEMMOTT_BINDLINE_ENABLE_BIND_BACK false
#endif

#else// FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES

#define FREDEMMOTT_BINDLINE_ENABLE_BIND_BACK false

#endif// FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES

#endif// ifndef