// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <functional>
#include <mutex>

void test_does_not_invoke_if_destroyed() {
  auto dqc = DispatcherQueueController::CreateOnDedicatedThread();
  auto dq = dqc.DispatcherQueue();

  struct {
    std::mutex mutex;
    std::condition_variable cv;
    bool blocked = false;
    bool dispatched = false;
    bool invoked_retained = false;
    bool invoked_unretained = false;
  } state;

  dq.TryEnqueue(std::bind_front(
    [](auto dq, auto state) -> IAsyncAction {
      {
        std::unique_lock lock(state->mutex);
        state->blocked = true;
        state->cv.notify_all();
      }
      while (!state->dispatched) {
#if FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND
        co_await winrt::resume_foreground(dq, DispatcherQueuePriority::High);
#else
        co_await wil::resume_foreground(dq, DispatcherQueuePriority::High);
#endif
      }
      {
        std::unique_lock lock(state->mutex);
        state->blocked = false;
        state->cv.notify_all();
      }
    },
    dq,
    &state));

  {
    std::unique_lock lock(state.mutex);
    state.cv.wait(lock, [&]() { return state.blocked; });
  }

  auto impl = [state = &state](auto invoked) {
    std::unique_lock lock(state->mutex);
    state->cv.wait(lock, [=]() { return state->dispatched; });
    CHECK_FALSE(state->blocked);
    *invoked = true;
    state->cv.notify_all();
  };

  {
    auto unretained
      = bind_context(dq, std::bind_front(impl, &state.invoked_unretained));
    unretained();
  }
  auto retained
    = bind_context(dq, std::bind_front(impl, &state.invoked_retained));
  retained();

  {
    std::unique_lock lock(state.mutex);
    state.dispatched = true;
    state.cv.notify_all();
  }

  ([](auto dq) -> IAsyncAction {
    co_await winrt::resume_foreground(dq, DispatcherQueuePriority::Low);
  })(dq)
    .get();

  CHECK_FALSE(state.blocked);
  CHECK(state.invoked_retained);
  CHECK_FALSE(state.invoked_unretained);

  dqc.ShutdownQueueAsync().get();
}