// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <string>

#include "tests/common/test_invocable_two_args.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bindline;

TEST_CASE("std::invocable") {
  auto impl = [](int, int, int, int) {};
  {
    auto bound = bind_back(impl, 123);
    test_invocable_three_args<decltype(bound), int>();
    auto pipeline = impl | bind_back(123, 456);
    test_invocable_three_args<decltype(pipeline), int>();
  }
  {
    auto bound = bind_back(impl, 123, 456);
    test_invocable_two_args<decltype(bound), int>();
    auto pipeline = impl | bind_back(123, 456);
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
  auto bound = impl | bind_back(123, 456);
  CHECK(!invoked);
  bound();
  CHECK(invoked);
}

TEST_CASE("pipeline") {
  bool invoked = false;
  auto impl = [&invoked](std::string a, std::string b) {
    CHECK(!invoked);
    CHECK(a == "front");
    CHECK(b == "back");
    invoked = true;
  };
  auto bound = impl | bind_back("back") | bind_back("front");
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
  auto bound = impl | bind_back(456);
  CHECK(!invoked);
  bound(123);
  CHECK(invoked);
}

TEST_CASE("mixed with bind_front") {
  {
    std::vector<int> log;
    auto f = [&log](auto... args) { log = {args...}; } | bind_front(1)
      | bind_back(2) | bind_front(3) | bind_back(4);
    f(5);
    CHECK(log == std::vector<int> {1, 3, 5, 4, 2});
  }

  {
    std::vector<int> log;
    auto f = [&log](auto... args) { log = {args...}; } | bind_front(1, 2)
      | bind_back(3, 4) | bind_front(5) | bind_back(6);
    f(7);
    CHECK(log == std::vector<int> {1, 2, 5, 7, 6, 3, 4});
  }
}