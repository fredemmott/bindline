// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#ifndef FREDEMMOTT_BIND_ENABLE_CPPWINRT
#if __has_include(<winrt/base.h>)
#define FREDEMMOTT_BIND_ENABLE_CPPWINRT true
#else
#define FREDEMMOTT_BIND_ENABLE_CPPWINRT false
#endif
#endif

#ifndef FREDEMMOTT_BIND_ENABLE_WEAK_REFS
#define FREDEMMOTT_BIND_ENABLE_WEAK_REFS true
#endif