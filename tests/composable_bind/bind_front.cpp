// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/composable_bind.hpp>

#include "../common/test_invocable_two_args.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::composable_bind;

TEST_CASE("std::invocable") {
  auto impl = [](int a, int b, int c) {};
  auto bound = bind_front(impl, 123);
  STATIC_CHECK(test_invocable_two_args<decltype(bound), int>);
  auto pipeline = impl | bind_front(123);
  STATIC_CHECK(test_invocable_two_args<decltype(pipeline), int>);
}

TEST_CASE("all args at once") {
  bool invoked = false;
  auto impl = [&invoked](auto a, auto b) {
    CHECK(!invoked);
    CHECK(a == 123);
    CHECK(b == 456);
    invoked = true;
  };
  auto bound = impl | bind_front(123, 456);
  CHECK(!invoked);
  bound();
  CHECK(invoked);
}

TEST_CASE("pipeline") {
  bool invoked = false;
  auto impl = [&invoked](auto a, auto b) {
    CHECK(!invoked);
    CHECK(a == 123);
    CHECK(b == 456);
    invoked = true;
  };
  auto bound = impl | bind_front(123) | bind_front(456);
  CHECK(!invoked);
  bound();
  CHECK(invoked);
}

TEST_CASE("partial") {
  bool invoked = false;
  auto impl = [&invoked](auto a, auto b) {
    CHECK(!invoked);
    CHECK(a == 123);
    CHECK(b == 456);
    invoked = true;
  };
  auto bound = impl | bind_front(123);
  CHECK(!invoked);
  bound(456);
  CHECK(invoked);
}