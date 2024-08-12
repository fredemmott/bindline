// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bindline;

#define COMMON_BIND_TEST_PREFIX "invoking: "
#define bind_function_under_test ::FredEmmott::bindline::bind_refs_back

#include "../../weak_refs/common/bind.hpp"
#undef COMMON_BIND_TEST_PREFIX
#undef bind_function_under_test
#define COMMON_BIND_TEST_PREFIX "pipeline: "
template <class... Args>
auto bind_function_under_test(const auto& fn, Args&&... args) {
  return fn
    | ::FredEmmott::bindline::bind_refs_back(std::forward<Args>(args)...);
}

#include "../../weak_refs/common/bind.hpp"

TEST_CASE("partial application") {
  auto strong = std::make_shared<int>(123);
  bool invoked = false;
  ([&invoked](auto front, auto back) {
    CHECK(*back == 123);
    CHECK(front == 456);
    invoked = true;
  } | bind_refs_back(strong))(456);
  CHECK(invoked);
}

TEST_CASE("composition") {
  auto strong = std::make_shared<int>(123);
  bool invoked = false;
  auto f =
    [&invoked](auto front, auto mid, auto back) {
      CHECK(*back == 123);
      CHECK(front == 456);
      CHECK(mid == 789);
      invoked = true;
    }
    // Not using `bind_back()` to avoid making this test depend on
    // std::bind_back
    | bind_refs_back(strong) | bind_front(456);
  f(789);
  CHECK(invoked);
}