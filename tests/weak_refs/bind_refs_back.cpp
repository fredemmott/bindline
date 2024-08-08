// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

template <class... Args>
auto bind_function_under_test(Args&&... args) {
  return bind_refs_back(std::forward<Args>(args)...);
}

#include "common/bind.hpp"

TEST_CASE("partial application") {
  auto strong = std::make_shared<int>(123);
  bool invoked = false;
  bind_refs_back(
    [&invoked](auto a, auto b) {
      CHECK(a == 456);
      CHECK(*b == 123);
      invoked = true;
    },
    strong)(456);
  CHECK(invoked);
}