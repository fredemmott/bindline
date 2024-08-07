// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>

[[nodiscard]]
inline bool always_suspends(auto context) {
  struct {
    std::mutex mutex;
    std::condition_variable cv;
    bool invoked {false};
    size_t returned = 0;
    std::optional<bool> did_suspend_first;
  } state;

  auto fn_under_test = bind_context(context, [&state]() {
    state.invoked = true;

    std::unique_lock lock(state.mutex);
    state.returned++;
    state.cv.notify_one();
  });
  auto test_fn = std::bind_front(
    [](auto fnUnderTest, auto context, auto state) -> winrt::fire_and_forget {
      auto invoke_and_notify = [=]() {
        (*fnUnderTest)();
        std::unique_lock lock(state->mutex);
        state->did_suspend_first = !state->invoked;
        state->returned++;
        state->cv.notify_one();
      };
      if constexpr (FredEmmott::cppwinrt_detail::awaitable_context<
                      decltype(*context)>) {
        co_await FredEmmott::cppwinrt_detail::switch_context_awaitable(
          *context);
        invoke_and_notify();
      } else {
        context->TryEnqueue(invoke_and_notify);
      }
      co_return;
    },
    &fn_under_test,
    &context,
    &state);
  test_fn();

  std::unique_lock lock(state.mutex);
  state.cv.wait(lock, [&]() { return state.returned == 2; });

  REQUIRE(state.did_suspend_first.has_value());

  CHECK(state.invoked);
  return state.did_suspend_first.value();
}