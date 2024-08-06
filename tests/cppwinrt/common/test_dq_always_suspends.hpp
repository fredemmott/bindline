// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>

inline void test_dq_always_suspends() {
  auto dqc = DispatcherQueueController::CreateOnDedicatedThread();
  auto dq = dqc.DispatcherQueue();
  struct {
    std::mutex mutex;
    std::condition_variable cv;
    bool invoked {false};
    size_t returned = 0;
    std::optional<bool> did_suspend_first;
  } state;

  auto f = bind_context(dq, [&state]() {
    state.invoked = true;

    std::unique_lock lock(state.mutex);
    state.returned++;
    state.cv.notify_one();
  });
  dq.TryEnqueue([f, dq, &state]() {
    f();
    std::unique_lock lock(state.mutex);
    state.did_suspend_first = !state.invoked;
    state.returned++;
    state.cv.notify_one();
  });

  std::unique_lock lock(state.mutex);
  state.cv.wait(lock, [&]() { return state.returned == 2; });

  REQUIRE(state.did_suspend_first.has_value());

  CHECK(state.invoked);
  CHECK(state.did_suspend_first.value());
  dqc.ShutdownQueueAsync().get();
}