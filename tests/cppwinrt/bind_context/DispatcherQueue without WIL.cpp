// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

// Must be *before* cppwinrt.hpp
#include <winrt/Windows.System.h>

#include <FredEmmott/cppwinrt.hpp>

#include <catch2/catch_test_macros.hpp>

static_assert(
  !FREDEMMOTT_CPPWINRT_ENABLE_WIL,
  "WIL support is covered in a separate test");

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

#include "common/test_dq_always_suspends.hpp"
#include "common/test_dq_forwards_arguments.hpp"
#include "common/test_invoked_even_if_destroyed.hpp"
#include "common/test_switch_to_dispatcherqueue.hpp"

TEST_CASE("switch to DispatcherQueue thread") {
  STATIC_CHECK_FALSE(
    decltype(bind_context([]() {}, DispatcherQueue {nullptr}))::use_coro_v);
  STATIC_CHECK(decltype(bind_context(
    []() {}, DispatcherQueue {nullptr}))::use_tryenqueue_v);

  test_switch_to_dispatcherqueue();
}

TEST_CASE("suspends even if already in correct thread") {
  test_dq_always_suspends();
}

TEST_CASE("invokes callback, even if functor destroyed while waiting") {
  test_invoked_even_if_destroyed();
}