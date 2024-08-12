// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bindline;

#define COMMON_BIND_TEST_PREFIX "invoking: "
#define bind_function_under_test ::FredEmmott::bindline::bind_refs_front

#include <tests/weak_refs/common/bind.hpp>
#undef COMMON_BIND_TEST_PREFIX
#undef bind_function_under_test
#define COMMON_BIND_TEST_PREFIX "pipeline: "
template <class... Args>
auto bind_function_under_test(const auto& fn, Args&&... args) {
  return fn
    | ::FredEmmott::bindline::bind_refs_front(std::forward<Args>(args)...);
}

#include <tests/weak_refs/common/bind.hpp>

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
  auto f =
    [&invoked](auto a, auto b, auto c) {
      CHECK(*a == 123);
      CHECK(b == 456);
      CHECK(c == 789);
      invoked = true;
    }
    | bind_refs_front(strong) | bind_front(456);
  f(789);
  CHECK(invoked);
}