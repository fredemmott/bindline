// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#if __has_include(<FredEmmott.bind.config-tweaks.hpp>)
#include <FredEmmott.bind.config-tweaks.hpp>
#endif

#ifndef FREDEMMOTT_BIND_ENABLE_WEAK_REFS
#define FREDEMMOTT_BIND_ENABLE_WEAK_REFS true
#endif

#ifndef FREDEMMOTT_BIND_ENABLE_BIND_BACK
#include <version>
#ifdef __cpp_lib_bind_back
#define FREDEMMOTT_BIND_ENABLE_BIND_BACK true
#else
#define FREDEMMOTT_BIND_ENABLE_BIND_BACK false
#endif
#endif