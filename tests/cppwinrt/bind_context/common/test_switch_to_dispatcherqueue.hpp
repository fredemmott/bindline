// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <future>

inline void test_switch_to_dispatcherqueue() {
  auto dqc = DispatcherQueueController::CreateOnDedicatedThread();

  const DWORD thisThreadID = GetCurrentThreadId();
  std::optional<DWORD> otherThreadID;

  std::promise<void> promise;

  auto f = bind_context(
    std::bind_front(
      [&promise](auto idPtr) {
        *idPtr = GetCurrentThreadId();
        promise.set_value();
      },
      &otherThreadID),
    dqc.DispatcherQueue());
  f();

  promise.get_future().wait();

  REQUIRE(thisThreadID == GetCurrentThreadId());
  CHECK(otherThreadID);
  CHECK(otherThreadID != thisThreadID);

  dqc.ShutdownQueueAsync().get();
}