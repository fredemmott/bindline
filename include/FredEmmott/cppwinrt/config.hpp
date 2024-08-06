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
/**
 * Disabling by default as:
 * - it's deprecated in favor of `wil::resume_foreground`
 * - it has visibly-different behavior: if called from the correct thread, it
 *   returns immediately, whereas `wil::resume_foreground()` will always suspend
 */
#define FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND false
#endif