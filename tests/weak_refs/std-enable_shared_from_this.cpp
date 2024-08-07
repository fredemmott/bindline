// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

class TestClass : public std::enable_shared_from_this<TestClass> {};

TEST_CASE("concepts") {
  using T = TestClass*;
  STATIC_CHECK_FALSE(strong_ref<T>);
  STATIC_CHECK_FALSE(weak_ref<T>);
  STATIC_CHECK(convertible_to_weak_ref<T>);
}

TEST_CASE("type alias") {
  STATIC_CHECK(std::same_as<std::weak_ptr<TestClass>, weak_ref_t<TestClass*>>);
}

TEST_CASE("conversions") {
  auto orig = std::make_shared<TestClass>();
  STATIC_CHECK(strong_ref<decltype(orig)>);

  auto raw = orig.get();
  STATIC_REQUIRE(std::same_as<TestClass*, decltype(raw)>);

  STATIC_CHECK_FALSE(strong_ref<decltype(raw)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(raw)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(raw)>);

  auto weak = make_weak_ref(raw);
  STATIC_REQUIRE(std::same_as<std::weak_ptr<TestClass>, decltype(weak)>);

  STATIC_CHECK_FALSE(strong_ref<decltype(weak)>);
  STATIC_CHECK(weak_ref<decltype(weak)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(weak)>);

  auto strong = lock_weak_ref(weak);
  STATIC_REQUIRE(std::same_as<std::shared_ptr<TestClass>, decltype(strong)>);

  STATIC_CHECK(strong_ref<decltype(strong)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(strong)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(strong)>);
}