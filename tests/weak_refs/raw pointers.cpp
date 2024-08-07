// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

TEST_CASE("concepts") {
  STATIC_CHECK(!strong_ref<int*>);
  STATIC_CHECK(!weak_ref<int*>);
  STATIC_CHECK(!convertible_to_weak_ref<int*>);
}