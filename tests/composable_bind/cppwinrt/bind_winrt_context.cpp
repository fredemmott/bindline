// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

// Must be *before* cppwinrt.hpp
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>

#include <FredEmmott/composable_bind.hpp>
#include <FredEmmott/cppwinrt/concepts.hpp>

#include <iostream>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::composable_bind;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

template <class TContext, class TFn>
auto bind_context(TFn&& fn, TContext&& context) {
  return bind_winrt_context(
    std::forward<TFn>(fn), std::forward<TContext>(context));
}

#include "../../cppwinrt/bind_context/common/test_dq_always_suspends.hpp"
#include "../../cppwinrt/bind_context/common/test_dq_forwards_arguments.hpp"
#include "../../cppwinrt/bind_context/common/test_invocable_int_int.hpp"
#include "../../cppwinrt/bind_context/common/test_invoked_even_if_destroyed.hpp"
#include "../../cppwinrt/bind_context/common/test_switch_to_dispatcherqueue.hpp"

TEST_CASE("switch to DispatcherQueue thread") {
  STATIC_CHECK_FALSE(
    decltype(bind_context([]() {}, DispatcherQueue {nullptr}))::use_coro_v);
  STATIC_CHECK(decltype(bind_context(
    []() {}, DispatcherQueue {nullptr}))::use_tryenqueue_v);

  test_switch_to_dispatcherqueue();
}

TEST_CASE("suspends even if already in correct thread") {
  test_dq_always_suspends();
}

TEST_CASE("std::invocable") {
  auto f = [](int a, int b) { return a + b; };

  using TBind = decltype(bind_winrt_context(f, winrt::apartment_context {}));
  test_invocable_int_int<TBind>();

  using TPipeline
    = decltype(f | bind_winrt_context(winrt::apartment_context {}));
  test_invocable_int_int<TPipeline>();
}

TEST_CASE("invokes callback, even if functor destroyed while waiting") {
  test_invoked_even_if_destroyed();
}

TEST_CASE("pipelining") {
  auto dqc = DispatcherQueueController::CreateOnDedicatedThread();
  auto dq = dqc.DispatcherQueue();

  std::promise<void> promise;

  auto f =
    [&](auto a, auto b, auto c) {
      CHECK(a == 123);
      CHECK(b == 456);
      CHECK(c == 789);
      promise.set_value();
    }
    | bind_winrt_context(dq) | bind_front(123) | bind_front(456);
  f(789);
  promise.get_future().wait();

  dqc.ShutdownQueueAsync().get();
}