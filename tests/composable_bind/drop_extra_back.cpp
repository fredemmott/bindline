// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/composable_bind.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

using namespace FredEmmott::composable_bind;

constexpr auto test_fn = [](int a, int b) { return a + b; };
using TFn = decltype(test_fn);

TEST_CASE("drop_extra_back_invocable") {
  STATIC_CHECK(drop_extra_back_invocable<TFn, int, int>);
  STATIC_CHECK(drop_extra_back_invocable<TFn, int, int, int>);
  STATIC_CHECK(drop_extra_back_invocable<TFn, int, int, int, int>);
  STATIC_CHECK(drop_extra_back_invocable<TFn, int, int, void*>);
  STATIC_CHECK(drop_extra_back_invocable<TFn, int, int, float*>);
  STATIC_CHECK_FALSE(drop_extra_back_invocable<TFn, int>);
  STATIC_CHECK_FALSE(drop_extra_back_invocable<TFn, int, float*>);
}

TEST_CASE("static evaluation") {
  constexpr int it = 2;
  STATIC_CHECK(drop_extra_back(test_fn)(1, 2) == 3);
  STATIC_CHECK(drop_extra_back(test_fn)(1, 2, 3) == 3);
  STATIC_CHECK(drop_extra_back(test_fn)(1, 2, it) == 3);
  STATIC_CHECK(drop_extra_back(test_fn)(1, it, 3) == 3);
  STATIC_CHECK(drop_extra_back(test_fn)(1, 2, 3, 4) == 3);
  STATIC_CHECK(drop_extra_back(test_fn)(1, 2, "i am not an int") == 3);
  STATIC_CHECK(drop_extra_back(test_fn)(1, 2, nullptr) == 3);
  STATIC_CHECK(drop_extra_back(test_fn)(1, 2, nullptr, "lolwhut") == 3);
}

TEST_CASE("pipeline static evaluation") {
  STATIC_CHECK((test_fn | drop_extra_back() | bind_front(1))(2, 3) == 3);
  STATIC_CHECK((drop_extra_back(test_fn) | bind_front(1))(2, 3) == 3);
  STATIC_CHECK((test_fn | bind_front(1) | drop_extra_back())(2, 3) == 3);
}

TEST_CASE("runtime evaluation") {
  const auto a = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  const auto b = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  const auto c = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  CHECK((test_fn | drop_extra_back())(a, b, c) == a + b);
  CHECK((test_fn | drop_extra_back())(a, b, c, a + b) == a + b);
  CHECK((test_fn | drop_extra_back() | bind_front(a))(a, b, c) == a + a);
}