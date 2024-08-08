// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)

TEST_CASE(STR(FEATURE_TEST_VARIABLE_NAME)) {
  SKIP("CMake flag " STR(FEATURE_TEST_VARIABLE_NAME) " is not set");
}