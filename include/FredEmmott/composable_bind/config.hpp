// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#if __has_include(<FredEmmott.composable_bind.config-tweaks.hpp>)
#include <FredEmmott.composable_bind.config-tweaks.hpp>
#endif

#ifndef FREDEMMOTT_COMPOSABLE_BINDENABLE_WEAK_REFS
#define FREDEMMOTT_COMPOSABLE_BINDENABLE_WEAK_REFS true
#endif

#ifndef FREDEMMOTT_COMPOSABLE_BINDENABLE_BIND_BACK
#include <version>
#ifdef __cpp_lib_bind_back
#define FREDEMMOTT_COMPOSABLE_BINDENABLE_BIND_BACK true
#else
#define FREDEMMOTT_COMPOSABLE_BINDENABLE_BIND_BACK false
#endif
#endif

#ifndef FREDEMMOTT_COMPOSABLE_BINDENABLE_CPPWINRT
#if __has_include(<winrt/base.h>)
#define FREDEMMOTT_COMPOSABLE_BINDENABLE_CPPWINRT true
#else
#define FREDEMMOTT_COMPOSABLE_BINDENABLE_CPPWINRT false
#endif
#endif
