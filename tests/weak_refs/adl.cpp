// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

namespace TestNS {
struct StrongInt {
  int value {};

  constexpr bool operator==(const StrongInt&) const noexcept = default;
};

struct WeakInt {
  int value {};

  constexpr bool operator==(const WeakInt&) const noexcept = default;
};

auto make_weak_ref(const StrongInt v) {
  return WeakInt {v.value};
}

auto lock_weak_ref(const WeakInt v) {
  return StrongInt {v.value};
}
}// namespace TestNS

using StrongInt = TestNS::StrongInt;
using WeakInt = TestNS::WeakInt;

TEST_CASE("concepts") {
  using namespace FredEmmott::weak_refs;
  STATIC_REQUIRE(weak_ref<WeakInt>);
  STATIC_REQUIRE(strong_ref<StrongInt>);

  STATIC_REQUIRE_FALSE(strong_ref<WeakInt>);
  STATIC_REQUIRE_FALSE(weak_ref<StrongInt>);

  STATIC_REQUIRE(convertible_to_weak_ref<WeakInt>);
  STATIC_REQUIRE(convertible_to_weak_ref<StrongInt>);
}

TEST_CASE("locked WeakInt is StrongInt") {
  REQUIRE(FredEmmott::weak_refs::lock_weak_ref(WeakInt { 3 }) == StrongInt { 3 });
}

TEST_CASE("weakened StrongInt is WeakInt") {
  REQUIRE(FredEmmott::weak_refs::make_weak_ref(StrongInt { 3 }) == WeakInt { 3 });
}

TEST_CASE("weak_ref of WeakInt is WeakInt") {
  REQUIRE(WeakInt {3} == FredEmmott::weak_refs::make_weak_ref(WeakInt {3}));
}
