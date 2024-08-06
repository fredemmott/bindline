// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#ifndef FREDEMMOTT_CPPWINRT_ENABLE_WIL
#if __has_include(<wil/cppwinrt_helpers.h>)
#define FREDEMMOTT_CPPWINRT_ENABLE_WIL true
#else
#define FREDEMMOTT_CPPWINRT_ENABLE_WIL false
#endif
#endif

#ifndef FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND
#define FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND \
  (!FREDEMMOTT_CPPWINRT_ENABLE_WIL)
#endif