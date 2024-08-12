
// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <string_view>

#include "../common/test_invocable_two_args.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bindline;

// This file is a good test of mixed pipelines too

TEST_CASE("std::invocable") {
  auto f = [](int, int) -> std::string_view { return "hello"; };
  auto tap = [](int, int) -> void {};

  auto bound = bind_tap(f, tap);
  auto pipeline = f | bind_tap(tap);
  STATIC_CHECK(std::is_invocable_v<decltype(bound), int, int>);
  STATIC_CHECK(std::is_invocable_v<decltype(pipeline), int, int>);

  test_invocable_two_args<decltype(bound), int>();
  test_invocable_two_args<decltype(bound), int>();

  STATIC_CHECK(
    std::is_invocable_r_v<std::string_view, decltype(bound), int, int>);
}

TEST_CASE("at start of pipeline") {
  bool invoked = false;
  auto f =
    [](int a, int b) {
      CHECK(a == 123);
      CHECK(b == 456);
    }
    | bind_tap([&invoked](const auto&... args) {
        CHECK(std::vector {args...} == std::vector {123, 456});
        invoked = true;
      })
    | bind_front(123) | bind_front(456);
  CHECK_FALSE(invoked);
  f();
  CHECK(invoked);
}

TEST_CASE("at end of pipeline") {
  bool invoked = false;
  auto f =
    [](int a, int b) {
      CHECK(a == 123);
      CHECK(b == 456);
    }
    | bind_front(123) | bind_front(456)
    | bind_tap([&invoked](const auto&... args) {
        CHECK(sizeof...(args) == 0);
        invoked = true;
      });
  CHECK_FALSE(invoked);
  f();
  CHECK(invoked);
}

TEST_CASE("with unbound args") {
  bool invoked = false;
  auto f =
    [](int a, int b) {
      CHECK(a == 123);
      CHECK(b == 456);
    }
    | bind_front(123) | bind_tap([&invoked](const auto&... args) {
        CHECK(std::vector {args...} == std::vector {456});
        invoked = true;
      });
  CHECK_FALSE(invoked);
  f(456);
  CHECK(invoked);
}

TEST_CASE("in middle of pipeline") {
  bool invoked = false;
  auto f =
    [](int a, int b) {
      CHECK(a == 123);
      CHECK(b == 456);
    }
    | bind_front(123) | bind_tap([&invoked](const auto&... args) {
        CHECK(std::vector {args...} == std::vector {456});
        invoked = true;
      })
    | bind_front(456);
  CHECK_FALSE(invoked);
  f();
  CHECK(invoked);
}

TEST_CASE("direct invocation") {
  bool invoked_inner {false};
  bool invoked_tap {false};
  auto inner = [&](auto v) {
    CHECK(v == 123);
    invoked_inner = true;
  };
  bind_tap(inner, [&](auto v) {
    CHECK(v == 123);
    invoked_tap = true;
  })(123);
  CHECK(invoked_tap);
  CHECK(invoked_inner);
}