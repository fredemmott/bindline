// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#ifndef FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT
#define FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT (__has_include(<winrt/base.h>))
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