// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

TEST_CASE("with a single shared_ptr") {
  auto a = std::make_shared<int>(123);

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

TEST_CASE("with two shared_ptrs") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(456);

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

TEST_CASE("unseating the first shared_ptr") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(456);

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

TEST_CASE("unseating the second shared_ptr") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(456);
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

TEST_CASE("promoting weak_ptrs") {
  auto strong = std::make_shared<int>(123);

  bool invoked = false;
  bind_refs_front(
    [&invoked](auto it) {
      STATIC_CHECK(std::same_as<decltype(it), std::shared_ptr<int>>);
      CHECK(*it == 123);
      invoked = true;
    },
    std::weak_ptr {strong})();
  CHECK(invoked);
}

TEST_CASE("stale weak_ptrs") {
  auto strong = std::make_shared<int>(123);

  bool invoked = false;
  auto f = bind_refs_front(
    [&invoked](auto it) {
      CHECK(false);
      invoked = true;
    },
    std::weak_ptr {strong});
  CHECK(!invoked);
  strong = nullptr;
  f();
  CHECK(!invoked);
}

struct TestClass {
  size_t count = 0;
  void increment() {
    ++count;
  }

  void always_fail() {
    CHECK(false);
  }
};

TEST_CASE("Member function pointer with a shared_ptr") {
  auto counter = std::make_shared<TestClass>();

  bind_refs_front(&TestClass::increment, counter)();
  CHECK(counter->count == 1);

  bind_refs_front(&TestClass::increment, std::weak_ptr {counter})();
  CHECK(counter->count == 2);

  auto f = bind_refs_front(&TestClass::always_fail, counter);
  counter = nullptr;
  f();
}

struct TestClassESFT : public TestClass,
                       public std::enable_shared_from_this<TestClassESFT> {};

TEST_CASE("Member function pointer with std::enable_shared_from_this") {
  auto counter = std::make_shared<TestClassESFT>();
  bind_refs_front(&TestClass::increment, counter)();
  CHECK(counter->count == 1);

  bind_refs_front(&TestClass::increment, counter.get())();
  CHECK(counter->count == 2);

  auto f = bind_refs_front(&TestClass::always_fail, counter.get());
  counter = nullptr;
  f();
}
