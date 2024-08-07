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

TEST_CASE("conversions") {
  std::shared_ptr<int> orig;

  auto weak = make_weak_ref(orig);
  STATIC_CHECK(convertible_to_weak_ref<decltype(weak)>);
  STATIC_CHECK(weak_ref<decltype(weak)>);
  STATIC_CHECK_FALSE(strong_ref<decltype(weak)>);
  STATIC_CHECK(
    std::same_as<decltype(weak), std::weak_ptr<decltype(orig)::element_type>>);

  auto strong = lock_weak_ref(weak);
  STATIC_CHECK(convertible_to_weak_ref<decltype(strong)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(strong)>);
  STATIC_CHECK(strong_ref<decltype(strong)>);
  STATIC_CHECK(
    std::
      same_as<decltype(strong), std::shared_ptr<decltype(orig)::element_type>>);

  STATIC_CHECK(std::same_as<decltype(orig), decltype(strong)>);
}

TEST_CASE("type aliases") {
  using TStrong = std::shared_ptr<int>;
  using TWeak = std::weak_ptr<int>;
  STATIC_CHECK(std::same_as<TWeak, weak_ref_t<TStrong>>);
  STATIC_CHECK(std::same_as<TStrong, strong_ref_t<TWeak>>);
}