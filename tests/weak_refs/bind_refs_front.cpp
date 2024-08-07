// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

TEST_CASE("with std::shared_ptr") {
  auto a = std::make_shared<int>(123);

  {
    bool invoked = false;
    auto f = bind_refs_front(
      [&invoked](auto a) {
        CHECK(a);
        CHECK(*a == 123);
        invoked = true;
      },
      a);
    CHECK(!invoked);
    f();
    CHECK(invoked);
  }

  auto b = std::make_shared<int>(456);
  {
    bool invoked = false;
    auto f = bind_refs_front(
      [&invoked](auto a, auto b) {
        CHECK(a);
        CHECK(*a == 123);
        CHECK(b);
        CHECK(*b == 456);
        invoked = true;
      },
      a,
      b);
    CHECK(!invoked);
    f();
    CHECK(invoked);
  }

  // Unseat first
  {
    bool invoked = false;
    auto f = bind_refs_front(
      [&invoked](auto a, auto b) {
        CHECK(false);
        invoked = true;
      },
      a,
      b);
    CHECK(!invoked);
    a = nullptr;
    f();
    CHECK(!invoked);
  }

  // Unseat second
  {
    a = std::make_shared<int>(123);
    bool invoked = false;
    auto f = bind_refs_front(
      [&invoked](auto a, auto b) {
        CHECK(false);
        invoked = true;
      },
      a,
      b);
    CHECK(!invoked);
    b = nullptr;
    f();
    CHECK(!invoked);
  }
}