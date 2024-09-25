// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>
#include <tests/common/test_invocable_three_args.hpp>
#include <tests/common/test_invocable_two_args.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bindline;

TEST_CASE("std::invocable") {
  auto impl = [](int, int, int, int) {};
  {
    auto bound = bind_front(impl, 123);
    test_invocable_three_args<decltype(bound), int>();
    auto pipeline = impl | bind_front(123);
    test_invocable_three_args<decltype(pipeline), int>();
  }
  {
    auto bound = bind_front(impl, 123, 456);
    test_invocable_two_args<decltype(bound), int>();
    auto pipeline = impl | bind_front(123, 456);
    test_invocable_two_args<decltype(pipeline), int>();
  }
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

TEST_CASE("partially bound") {
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

TEST_CASE("non-temporary partial pipeline") {
  bool invoked = false;
  auto impl = [&invoked](std::string a, std::string b) {
    CHECK(!invoked);
    CHECK(a == "front");
    CHECK(b == "back");
    invoked = true;
  };
  auto partial = impl | bind_front("front");
  auto full = partial | bind_front("back");
  CHECK(!invoked);
  full();
  CHECK(invoked);
}
