// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <FredEmmott/cppwinrt.hpp>

static_assert(
  !FREDEMMOTT_CPPWINRT_ENABLE_WIL,
  "WIL support is covered in a separate test");
static_assert(!FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND);

#include <pplawait.h>

#include <optional>

#include <catch2/catch_test_macros.hpp>

// Must be *after* <FredEmmott/cppwinrt.hpp>
#include <winrt/Windows.System.h>

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::System;

#include "common/always_suspends.hpp"

template <class T>
concept valid_context = requires(T ctx) { bind_context(ctx, []() {}); };

concurrency::task<winrt::apartment_context> get_background_context(auto dq) {
  winrt::apartment_context foreground;
  CHECK(foreground);
  co_await winrt::resume_foreground(dq);
  winrt::apartment_context ret;
  CHECK(ret);
  co_await foreground;
  co_return ret;
}

TEST_CASE("switch to given winrt::apartment_context") {
  STATIC_REQUIRE(valid_context<winrt::apartment_context>);

  auto dqc = winrt::Windows::System::DispatcherQueueController::
    CreateOnDedicatedThread();

  const DWORD thisThreadID = GetCurrentThreadId();
  std::optional<DWORD> backgroundThreadID;

  const auto otherThread = get_background_context(dqc.DispatcherQueue()).get();
  CHECK(otherThread);

  winrt::handle event {CreateEvent(nullptr, false, false, nullptr)};
  HANDLE handles[] = {event.get()};

  auto f = bind_context(
    otherThread,
    std::bind_front(
      [](auto idPtr, auto event) {
        *idPtr = GetCurrentThreadId();
        SetEvent(event);
      },
      &backgroundThreadID,
      event.get()));

  f();
  while (MsgWaitForMultipleObjects(1, handles, false, INFINITE, QS_ALLEVENTS)
         != WAIT_OBJECT_0) {
  }

  CHECK(thisThreadID == GetCurrentThreadId());
  CHECK(backgroundThreadID);
  CHECK(backgroundThreadID != thisThreadID);

  dqc.ShutdownQueueAsync().get();
}

TEST_CASE("doesn't suspend if already in the correct thread") {
  // This is inconsistent - see
  // https://github.com/fredemmott/weak_refs/issues/12
  auto dqc = winrt::Windows::System::DispatcherQueueController::
    CreateOnDedicatedThread();
  const auto otherThread = get_background_context(dqc.DispatcherQueue()).get();
  CHECK_FALSE(always_suspends(otherThread));

  dqc.ShutdownQueueAsync().get();
}

TEST_CASE("can't switch to a DispatcherQueue") {
  STATIC_CHECK_FALSE(valid_context<winrt::Windows::System::DispatcherQueue>);
}