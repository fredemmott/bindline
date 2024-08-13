// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#if __has_include(<FredEmmott.bindline.config-tweaks.hpp>)
#include <FredEmmott.bindline.config-tweaks.hpp>
#endif

#ifndef FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES
#define FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES true
#endif

#ifdef __GNUC__
#define FREDEMMOTT_BINDLINE_GCC_VERSION \
  ((__GNUC__ * 10000) + (__GNUC_MINOR__ * 100) + __GNUC_PATCHLEVEL__)
#endif

// Permitted all the way back to C++11 with defect report (CWG2518), but not
// available on G++ 11 (Ubuntu 22.04)
//
// See https://godbolt.org/z/7xjd6Prv6
#ifndef FREDEMMOTT_BINDLINE_CAN_STATIC_ASSERT_FALSE
#ifndef __GNUC__
#define FREDEMMOTT_BINDLINE_CAN_STATIC_ASSERT_FALSE true
#else
#define FREDEMMOTT_BINDLINE_CAN_STATIC_ASSERT_FALSE \
  (FREDEMMOTT_BINDLINE_GCC_VERSION >= 130100)
#endif
#endif

//-----
#ifndef FREDEMMOTT_BINDLINE_ENABLE_WEAK_REFS
#define FREDEMMOTT_BINDLINE_ENABLE_WEAK_REFS \
  FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES
#endif

#ifndef FREDEMMOTT_BINDLINE_HAVE_STD_FORMAT
#if __has_include(<format>)
#define FREDEMMOTT_BINDLINE_HAVE_STD_FORMAT \
  FREDEMMOTT_BINDLINE_IMPLICIT_OPTIONAL_FEATURES
#else
#define FREDEMMOTT_BINDLINE_HAVE_STD_FORMAT false
#endif
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