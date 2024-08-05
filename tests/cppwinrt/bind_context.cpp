// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <FredEmmott/cppwinrt.hpp>

static_assert(
  !FREDEMMOTT_CPPWINRT_ENABLE_WIL,
  "WIL support is covered in a separate test");

#include <pplawait.h>

#include <winrt/Windows.System.h>

#include <optional>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::cppwinrt;

concurrency::task<winrt::apartment_context> get_background_context(auto dq) {
  winrt::apartment_context foreground;
  CHECK(foreground);
  co_await winrt::resume_foreground(dq);
  winrt::apartment_context ret;
  CHECK(ret);
  co_await foreground;
  co_return ret;
}

TEST_CASE("winrt::apartment_context") {
  auto dqc = winrt::Windows::System::DispatcherQueueController::
    CreateOnDedicatedThread();

  const DWORD thisThreadID = GetCurrentThreadId();
  std::optional<DWORD> backgroundThreadID;

  const auto backgroundContext = get_background_context(dqc.DispatcherQueue()

                                                          )
                                   .get();
  CHECK(backgroundContext);

  winrt::handle event {CreateEvent(nullptr, false, false, nullptr)};
  HANDLE handles[] = {event.get()};

  auto f = bind_context(
    backgroundContext,
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

TEST_CASE("lifetime safety") {
  SKIP("not implemented - TODO");
}