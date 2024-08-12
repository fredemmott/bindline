// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

using namespace FredEmmott::bindline;

constexpr auto test_fn = [](int a, int b) { return a + b; };
using TFn = decltype(test_fn);

TEST_CASE("std::invocable") {
  auto dropping = drop_extra_back(test_fn);
  using TDrop = decltype(dropping);
  STATIC_CHECK(std::invocable<TDrop, int, int>);
  STATIC_CHECK(std::invocable<TDrop, int, int, int>);
  STATIC_CHECK(std::invocable<TDrop, int, int, int, int>);
  STATIC_CHECK(std::invocable<TDrop, int, int, void*>);
  STATIC_CHECK(std::invocable<TDrop, int, int, float*>);
  STATIC_CHECK_FALSE(std::invocable<TDrop, int>);
  STATIC_CHECK_FALSE(std::invocable<TDrop, int, float*>);
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