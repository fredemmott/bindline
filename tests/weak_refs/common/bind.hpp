// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>
#include <tests/common/test_invocable_two_args.hpp>

#include <catch2/catch_test_macros.hpp>

#ifndef COMMON_BIND_TEST_PREFIX
#define COMMON_BIND_TEST_PREFIX
#endif

using namespace FredEmmott::weak_refs;

TEST_CASE(COMMON_BIND_TEST_PREFIX "with a single shared_ptr") {
  auto a = std::make_shared<int>(123);

  bool invoked = false;
  auto f = bind_function_under_test(
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

TEST_CASE(COMMON_BIND_TEST_PREFIX "std::invocable") {
  auto a = std::make_shared<int>(1);
  auto b = std::make_shared<int>(2);
  auto c = std::make_shared<int>(0);

  using TArg = decltype(a);
  auto f = bind_function_under_test(
    [](TArg a, TArg b, TArg c) {
      // Require pointers
      CHECK(*a + *b + *c == 3);
    },
    a);

  using TFn = decltype(f);
  test_invocable_two_args<TFn, TArg>();
}

TEST_CASE(COMMON_BIND_TEST_PREFIX "with two shared_ptrs") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(456);

  bool invoked = false;
  auto f = bind_function_under_test(
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

TEST_CASE(COMMON_BIND_TEST_PREFIX "unseating the first shared_ptr") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(456);

  bool invoked = false;
  auto f = bind_function_under_test(
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

TEST_CASE(COMMON_BIND_TEST_PREFIX "unseating the second shared_ptr") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(456);
  a = std::make_shared<int>(123);
  bool invoked = false;
  auto f = bind_function_under_test(
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

TEST_CASE(COMMON_BIND_TEST_PREFIX "promoting weak_ptrs") {
  auto strong = std::make_shared<int>(123);

  bool invoked = false;
  bind_function_under_test(
    [&invoked](auto it) {
      STATIC_CHECK(std::same_as<decltype(it), std::shared_ptr<int>>);
      CHECK(*it == 123);
      invoked = true;
    },
    std::weak_ptr {strong})();
  CHECK(invoked);
}

TEST_CASE(COMMON_BIND_TEST_PREFIX "stale weak_ptrs") {
  auto strong = std::make_shared<int>(123);
  std::weak_ptr weak {strong};

  bool invoked = false;
  auto f = bind_function_under_test(
    [&invoked](auto it) {
      CHECK(false);
      invoked = true;
    },
    weak);
  CHECK(!invoked);
  strong = nullptr;
  f();
  CHECK(!invoked);
}

#ifndef COMMON_BIND_HAVE_INCLUDED
struct TestClass {
  size_t count = 0;
  void increment() {
    ++count;
  }

  void always_fail() {
    CHECK(false);
  }
};
#endif

TEST_CASE(COMMON_BIND_TEST_PREFIX "Member function pointer with a shared_ptr") {
  auto counter = std::make_shared<TestClass>();

  bind_function_under_test(&TestClass::increment, counter)();
  CHECK(counter->count == 1);

  bind_function_under_test(&TestClass::increment, std::weak_ptr {counter})();
  CHECK(counter->count == 2);

  auto f = bind_function_under_test(&TestClass::always_fail, counter);
  counter = nullptr;
  f();
}

#ifndef COMMON_BIND_HAVE_INCLUDED
struct TestClassESFT : public TestClass,
                       public std::enable_shared_from_this<TestClassESFT> {};
#endif

TEST_CASE(COMMON_BIND_TEST_PREFIX
          "Member function pointer with std::enable_shared_from_this") {
  auto counter = std::make_shared<TestClassESFT>();
  bind_function_under_test(&TestClass::increment, counter)();
  CHECK(counter->count == 1);

  bind_function_under_test(&TestClass::increment, counter.get())();
  CHECK(counter->count == 2);

  auto f = bind_function_under_test(&TestClass::always_fail, counter.get());
  counter = nullptr;
  f();
}

#define COMMON_BIND_HAVE_INCLUDED
