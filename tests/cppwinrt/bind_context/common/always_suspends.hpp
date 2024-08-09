// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <functional>
#include <latch>
#include <optional>

[[nodiscard]]
inline bool always_suspends(auto context) {
  struct {
    std::latch latch {2};
    bool invoked {false};
    std::optional<bool> did_suspend_first;
  } state;

  auto fn_under_test = bind_context(
    [&state]() {
      state.invoked = true;
      state.latch.count_down();
    },
    context);
  auto test_fn = std::bind_front(
    [](auto fnUnderTest, auto context, auto state) -> winrt::fire_and_forget {
      auto invoke_and_notify = [=]() {
        (*fnUnderTest)();
        state->did_suspend_first = !state->invoked;
        state->latch.count_down();
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

  state.latch.wait();

  REQUIRE(state.did_suspend_first.has_value());

  CHECK(state.invoked);
  return state.did_suspend_first.value();
}