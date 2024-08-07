// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#if __has_include(<FredEmmott.weak_refs.tweaks.hpp>)
#include <FredEmmott.weak_refs.tweaks.hpp>
#endif

#ifndef FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT
#if __has_include(<winrt/base.h>)
#define FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT true
#else
#define FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT false
#endif
#endif

#ifndef FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT_GET_WEAK
#define FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT_GET_WEAK \
  FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT
#endif

#ifndef FREDEMMOTT_WEAK_REFS_ENABLE_STD_SHARED_PTR
#define FREDEMMOTT_WEAK_REFS_ENABLE_STD_SHARED_PTR true
#endif
#ifndef FREDEMMOTT_WEAK_REFS_ENABLE_WEAK_FROM_THIS
#define FREDEMMOTT_WEAK_REFS_ENABLE_WEAK_FROM_THIS \
  FREDEMMOTT_WEAK_REFS_ENABLE_STD_SHARED_PTR
#endif