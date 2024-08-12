// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/composable_bind.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

using namespace FredEmmott::composable_bind;

constexpr auto test_fn = [](int a, int b) { return a + b; };
using TFn = decltype(test_fn);

TEST_CASE("static evaluation") {
  constexpr int it = 2;
  STATIC_CHECK(drop_n_back<0>(test_fn)(1, 2) == 3);
  STATIC_CHECK(drop_n_back<1>(test_fn)(1, 2, 3) == 3);
  STATIC_CHECK(drop_n_back<1>(test_fn)(1, 2, it) == 3);
  STATIC_CHECK(drop_n_back<1>(test_fn)(1, it, 3) == 3);
  STATIC_CHECK(drop_n_back<2>(test_fn)(1, 2, 3, 4) == 3);
  STATIC_CHECK(drop_n_back<1>(test_fn)(1, 2, "i am not an int") == 3);
  STATIC_CHECK(drop_n_back<1>(test_fn)(1, 2, nullptr) == 3);
  STATIC_CHECK(drop_n_back<2>(test_fn)(1, 2, nullptr, "lolwhut") == 3);
}

TEST_CASE("pipeline static evaluation") {
  STATIC_CHECK((test_fn | drop_n_back<1>() | bind_front(1))(2, 3) == 3);
  STATIC_CHECK((drop_n_back<1>(test_fn) | bind_front(1))(2, 3) == 3);
  STATIC_CHECK((test_fn | bind_front(1) | drop_n_back<1>())(2, 3) == 3);
}

TEST_CASE("std::invocable") {
  auto drop0 = drop_n_back<0>(test_fn);
  STATIC_CHECK(std::invocable<decltype(drop0), int, int>);
  STATIC_CHECK_FALSE(std::invocable<decltype(drop0), int, std::nullptr_t>);
  STATIC_CHECK_FALSE(std::invocable<decltype(drop0), int>);
  STATIC_CHECK_FALSE(std::invocable<decltype(drop0), int, int, int>);

  auto drop1 = drop_n_back<1>(test_fn);
  STATIC_CHECK(std::invocable<decltype(drop1), int, int, int>);
  STATIC_CHECK_FALSE(std::invocable<decltype(drop1), int, int>);
  STATIC_CHECK_FALSE(std::invocable<decltype(drop1), int, int, int, int>);
}

TEST_CASE("runtime evaluation") {
  const auto a = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  const auto b = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  const auto c = GENERATE(take(3, random(INT_MIN, INT_MAX)));
  CHECK((test_fn | drop_n_back<1>())(a, b, c) == a + b);
  CHECK((test_fn | drop_n_back<2>())(a, b, c, a + b) == a + b);
  CHECK((test_fn | drop_n_back<2>() | bind_front(a))(a, b, c) == a + a);
}