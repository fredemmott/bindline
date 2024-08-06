// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

// Must be *before* cppwinrt.hpp
#include <winrt/Windows.System.h>

#define FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND true

#include <FredEmmott/cppwinrt.hpp>

#include <functional>
#include <future>
#include <optional>

#include <catch2/catch_test_macros.hpp>

static_assert(
  !FREDEMMOTT_CPPWINRT_ENABLE_WIL,
  "WIL support is covered in a separate test");

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

#include "common/test_switch_to_dispatcherqueue.hpp"

TEST_CASE("switch to DispatcherQueue thread") {
  test_switch_to_dispatcherqueue();
}

TEST_CASE("does not suspend if already in correct thread") {
  SKIP("TODO - not implemented");
}