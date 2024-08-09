// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bind.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bind;

#define bind_function_under_test ::FredEmmott::bind::bind_refs_front

#include "../../weak_refs/common/bind.hpp"

TEST_CASE("partial application") {
  auto strong = std::make_shared<int>(123);
  bool invoked = false;
  ([&invoked](auto a, auto b) {
    CHECK(*a == 123);
    CHECK(b == 456);
    invoked = true;
  } | bind_refs_front(strong))(456);
  CHECK(invoked);
}

TEST_CASE("composition") {
  auto strong = std::make_shared<int>(123);
  bool invoked = false;
  ([&invoked](auto a, auto b, auto c) {
    CHECK(*a == 123);
    CHECK(b == 456);
    CHECK(c == 789);
    invoked = true;
  } | bind_refs_front(strong)
   | bind_front(456))(789);
  CHECK(invoked);
}