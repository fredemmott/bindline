// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

// Must be *before* cppwinrt.hpp
#include <winrt/Windows.System.h>

#define FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND true

#include <FredEmmott/cppwinrt.hpp>

static_assert(
  !FREDEMMOTT_CPPWINRT_ENABLE_WIL,
  "WIL support is covered in a separate test");

#include <functional>
#include <optional>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::cppwinrt;

template <class T>
concept valid_context = requires(T ctx) { bind_context(ctx, []() {}); };

TEST_CASE("winrt::Windows::System::DispatcherQueue") {
  STATIC_REQUIRE(valid_context<winrt::Windows::System::DispatcherQueue>);

  auto dqc = winrt::Windows::System::DispatcherQueueController::
    CreateOnDedicatedThread();

  const DWORD thisThreadID = GetCurrentThreadId();
  std::optional<DWORD> otherThreadID;

  winrt::handle event {CreateEvent(nullptr, false, false, nullptr)};
  HANDLE handles[] = {event.get()};

  auto f = bind_context(
    dqc.DispatcherQueue(),
    std::bind_front(
      [](auto idPtr, auto event) {
        *idPtr = GetCurrentThreadId();
        SetEvent(event);
      },
      &otherThreadID,
      event.get()));

  f();
  while (MsgWaitForMultipleObjects(1, handles, false, INFINITE, QS_ALLEVENTS)
         != WAIT_OBJECT_0) {
  }

  CHECK(thisThreadID == GetCurrentThreadId());
  CHECK(otherThreadID);
  CHECK(otherThreadID != thisThreadID);

  dqc.ShutdownQueueAsync().get();
}