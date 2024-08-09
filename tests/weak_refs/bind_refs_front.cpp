// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

template <class... Args>
auto bind_function_under_test(const auto& fn, Args&&... args) {
  return bind_refs_front(fn, std::forward<Args>(args)...);
}

#include "common/bind.hpp"

TEST_CASE("partial application") {
  auto strong = std::make_shared<int>(123);
  bool invoked = false;
  bind_refs_front(
    [&invoked](auto a, auto b) {
      CHECK(*a == 123);
      CHECK(b == 456);
      invoked = true;
    },
    strong)(456);
  CHECK(invoked);
}