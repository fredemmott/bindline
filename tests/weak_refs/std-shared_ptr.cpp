// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

TEST_CASE("std::shared_ptr<T> concepts") {
  using T = std::shared_ptr<int>;
  STATIC_CHECK(strong_ref<T>);
  STATIC_CHECK_FALSE(weak_ref<T>);
  STATIC_CHECK(convertible_to_weak_ref<T>);
}

TEST_CASE("std::weak_ptr<T> concepts") {
  using T = std::weak_ptr<int>;
  STATIC_CHECK_FALSE(strong_ref<T>);
  STATIC_CHECK(weak_ref<T>);
  STATIC_CHECK(convertible_to_weak_ref<T>);
}