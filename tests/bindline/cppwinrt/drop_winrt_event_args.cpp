// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

using namespace FredEmmott::bindline;

constexpr auto test_fn = [](int a, int b) { return a + b; };
using TFn = decltype(test_fn);

namespace {
winrt::Windows::Foundation::IInspectable Sender {nullptr};
winrt::Windows::Foundation::IInspectable Args {nullptr};
using TSender = decltype(Sender);
using TArgs = decltype(Args);
}// namespace

template <class TFn>
void test_static_invocable() {
  STATIC_CHECK(std::invocable<TFn, int, int, TSender, TArgs>);
  STATIC_CHECK_FALSE(std::invocable<TFn, int, int>);
  STATIC_CHECK_FALSE(std::invocable<TFn, int, int, int, TSender, TArgs>);
  STATIC_CHECK_FALSE(std::invocable<TFn, int, int, TSender, TArgs, int>);
  STATIC_CHECK_FALSE(std::invocable<TFn, int, std::nullptr_t>);
}

TEST_CASE("std::invocable") {
  auto dropped = drop_winrt_event_args(test_fn);
  test_static_invocable<decltype(dropped)>();
}

TEST_CASE("pipeline std::invocable") {
  auto dropped = test_fn | drop_winrt_event_args();
  test_static_invocable<decltype(dropped)>();
}

TEST_CASE("static evaluation") {
  STATIC_CHECK(drop_winrt_event_args(test_fn)(1, 2, Sender, Args) == 3);
}

TEST_CASE("pipeline static evaluation") {
  STATIC_CHECK((test_fn | drop_winrt_event_args())(1, 2, Sender, Args) == 3);
}

TEST_CASE("runtime evaluation") {
  const auto a = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  const auto b = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  const auto c = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  CHECK((test_fn | drop_winrt_event_args())(a, b, Sender, Args) == a + b);
}