// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <winrt/Windows.System.h>

#include <FredEmmott/cppwinrt.hpp>

static_assert(FREDEMMOTT_CPPWINRT_ENABLE_WIL);

#include <future>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

TEST_CASE("basic") {
  SKIP("TODO - not implemented");
}

/** Test that if the `bind_context()` functor is destructed while waiting, it
 * doesn't attempt to call its delegate.
 *
 * While this functionality isn't WIL-specific, the test is:
 * - DispatcherQueues hold a lock while executing a callback
 * - enqueueing requries communication with the DQ thread
 * - this communication requires access to the same lock
 * - so, we can't actually block or suspend the DQ thread while setting things
 * up
 *
 * Instead, we need to keep the DispatcherQueue alive and looping, but we need
 * to stop it from resuming our `bind_context()` functors until they've been
 * destructed.
 *
 * To do this, we just `wil::resume_foreground(dq, HighPriority)` in a loop; we
 * use `wil::resume_foreground()` because it will **always** re-enter the
 * DispatcherQueue loop, but `winrt::resume_foreground()` will immediately
 * return if we're already in the correct thread, turning our 'blocker' into a
 * busy-loop which actually blocks the thread.
 */
TEST_CASE("lifetime safety") {
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
        co_await wil::resume_foreground(dq, DispatcherQueuePriority::High);
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

  auto retained
    = bind_context(dq, std::bind_front(impl, &state.invoked_retained));
  retained();
  {
    auto unretained
      = bind_context(dq, std::bind_front(impl, &state.invoked_unretained));
    unretained();
  }

  {
    std::unique_lock lock(state.mutex);
    state.dispatched = true;
    state.cv.notify_all();
  }

  ([](auto dq) -> IAsyncAction { co_await winrt::resume_foreground(dq); })(dq)
    .get();

  CHECK_FALSE(state.blocked);
  CHECK(state.invoked_retained);
  CHECK_FALSE(state.invoked_unretained);

  dqc.ShutdownQueueAsync().get();
}