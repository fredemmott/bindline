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

#include "common/test_dq_always_suspends.hpp"
#include "common/test_dq_forwards_arguments.hpp"
#include "common/test_invoked_even_if_destroyed.hpp"
#include "common/test_switch_to_dispatcherqueue.hpp"

TEST_CASE("switch to DispatcherQueue thread") {
  STATIC_CHECK(
    decltype(bind_context([]() {}, DispatcherQueue {nullptr}))::use_coro_v);
  STATIC_CHECK_FALSE(decltype(bind_context(
    []() {}, DispatcherQueue {nullptr}))::use_tryenqueue_v);
  test_switch_to_dispatcherqueue();
}

// This tests that we're using `wil::resume_foreground()`, not
// `winrt::resume_foreground()`
TEST_CASE("suspends even if already in the correct thread") {
  test_dq_always_suspends();
}

TEST_CASE("invokes callback, even if functor destroyed while waiting") {
  test_invoked_even_if_destroyed();
}