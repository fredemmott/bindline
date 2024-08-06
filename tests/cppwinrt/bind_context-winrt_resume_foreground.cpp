// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

// Must be *before* cppwinrt.hpp
#include <winrt/Windows.System.h>

#define FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND true

#include <FredEmmott/cppwinrt.hpp>

#include <catch2/catch_test_macros.hpp>

static_assert(
  !FREDEMMOTT_CPPWINRT_ENABLE_WIL,
  "WIL support is covered in a separate test");

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

#include "common/test_does_not_invoke_if_destroyed.hpp"
#include "common/test_dq_always_suspends.hpp"
#include "common/test_switch_to_dispatcherqueue.hpp"

TEST_CASE("switch to DispatcherQueue thread") {
  test_switch_to_dispatcherqueue();
}

TEST_CASE("suspends even if already in correct thread") {
  test_dq_always_suspends();
}

TEST_CASE("doesn't invoke callback if destructed while waiting") {
  test_does_not_invoke_if_destroyed();
}