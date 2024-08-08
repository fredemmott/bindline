
// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bind.hpp>

#include <format>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bind;

// This whole file is a good test of mixed pipelines too

TEST_CASE("at start of pipeline") {
  bool invoked = false;
  auto f =
    [](int a, int b) {
      CHECK(a == 123);
      CHECK(b == 456);
    }
    | bind_tap([&invoked](const auto&... args) {
        const auto str = (std::to_string(args) + ...);
        CHECK(str == "123456");
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
        CHECK(sizeof...(args) == 1);
        CHECK((std::to_string(args) + ...) == "456");
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
        CHECK(sizeof...(args) == 1);
        CHECK((std::to_string(args) + ...) == "456");
        invoked = true;
      })
    | bind_front(456);
  CHECK_FALSE(invoked);
  f();
  CHECK(invoked);
}