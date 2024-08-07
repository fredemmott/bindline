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

TEST_CASE("conversions") {
  SKIP("TODO: not implemented");
}