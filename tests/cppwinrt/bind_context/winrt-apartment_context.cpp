// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <FredEmmott/cppwinrt.hpp>

static_assert(
  !FREDEMMOTT_CPPWINRT_ENABLE_WIL,
  "WIL support is covered in a separate test");

#include <pplawait.h>

#include <optional>

#include <catch2/catch_test_macros.hpp>

// Must be *after* <FredEmmott/cppwinrt.hpp>
#include <winrt/Windows.System.h>

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::System;

#include "../../common/test_invocable_two_args.hpp"
#include "common/always_suspends.hpp"
#include "common/check_forwards_arguments.hpp"

template <class T>
concept valid_context = requires(T ctx) { bind_context([]() {}, ctx); };

concurrency::task<winrt::apartment_context> get_background_context(auto dq) {
  winrt::apartment_context foreground;
  CHECK(foreground);
  co_await winrt::resume_foreground(dq);
  winrt::apartment_context ret;
  CHECK(ret);
  co_await foreground;
  co_return ret;
}

TEST_CASE("std::invocable") {
  auto add = [](int a, int b) { return a + b; };
  using TBound = decltype(bind_context(add, winrt::apartment_context {}));
  test_invocable_two_args<TBound, int>();
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
    std::bind_front(
      [](auto idPtr, auto event) {
        *idPtr = GetCurrentThreadId();
        SetEvent(event);
      },
      &backgroundThreadID,
      event.get()),
    otherThread);

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
  // This is inconsistent with using the associated DispatcherQueue as the
  // context, as there isn't necessarily going to be a DispatcherQueue
  // associated with the `winrt::apartment_context`. Deferral requires a queue,
  // and as we might not have one, we can't defer.
  //
  // Instead of deferring if we can, test that the behavior is consistent for
  // all `winrt::apartment_context`s, even once that do have a DispatcherQueue.
  auto dqc = winrt::Windows::System::DispatcherQueueController::
    CreateOnDedicatedThread();
  const auto otherThread = get_background_context(dqc.DispatcherQueue()).get();
  CHECK_FALSE(always_suspends(otherThread));

  dqc.ShutdownQueueAsync().get();
}

TEST_CASE("forwarding arguments") {
  auto dqc = winrt::Windows::System::DispatcherQueueController::
    CreateOnDedicatedThread();
  const auto otherThread = get_background_context(dqc.DispatcherQueue()).get();
  CHECK_FALSE(always_suspends(otherThread));

  check_forwards_arguments(otherThread);

  dqc.ShutdownQueueAsync().get();
}