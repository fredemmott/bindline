// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bind.hpp>

#include <string>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bind;

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