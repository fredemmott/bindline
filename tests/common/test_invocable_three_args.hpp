// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <concepts>

#include "impossible_class.hpp"
#include <catch2/catch_test_macros.hpp>

template <class TFn, class TA, class TB = TA, class TC = TA>
void test_invocable_three_args() {
  STATIC_CHECK_FALSE(std::invocable<TFn>);
  STATIC_CHECK_FALSE(std::invocable<TFn, TA>);
  STATIC_CHECK_FALSE(std::invocable<TFn, TA, TB>);
  STATIC_CHECK(std::invocable<TFn, TA, TB, TC>);
  STATIC_CHECK_FALSE(std::invocable<TFn, TA, TB, impossible_class>);
  STATIC_CHECK_FALSE(std::invocable<TFn, TA, TB, impossible_class>);
}