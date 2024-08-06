// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <winrt/Windows.System.h>

#include <FredEmmott/cppwinrt.hpp>

#include <catch2/catch_test_macros.hpp>

static_assert(FREDEMMOTT_CPPWINRT_ENABLE_WIL);

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

#include "common/always_suspends.hpp"
#include "common/test_does_not_invoke_if_destroyed.hpp"
#include "common/test_switch_to_dispatcherqueue.hpp"

TEST_CASE("switch to DispatcherQueue thread") {
  test_switch_to_dispatcherqueue();
}

// This tests that we're using `wil::resume_foreground()`, not
// `winrt::resume_foreground()`
TEST_CASE("suspends even if already in the correct thread") {
  test_dq_always_suspends();
}

TEST_CASE("doesn't invoke callback if destructed while waiting") {
  test_does_not_invoke_if_destroyed();
}