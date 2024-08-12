// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <concepts>

#include <catch2/catch_test_macros.hpp>

template <class TFn>
void test_invocable_int_int() {
  STATIC_CHECK_FALSE(std::invocable<TFn>);
  STATIC_CHECK_FALSE(std::invocable<TFn, int>);
  STATIC_CHECK(std::invocable<TFn, int, int>);
  STATIC_CHECK_FALSE(std::invocable<TFn, int, int, int>);
  STATIC_CHECK_FALSE(std::invocable<TFn, int, std::nullptr_t>);
}