// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#ifndef FREDEMMOTT_BIND_ENABLE_CPPWINRT
#define FREDEMMOTT_BIND_ENABLE_CPPWINRT (__has_include(<winrt/base.h>))
#endif

#ifndef FREDEMMOTT_BIND_ENABLE_WEAK_REFS
#define FREDEMMOTT_BIND_ENABLE_WEAK_REFS true
#endif