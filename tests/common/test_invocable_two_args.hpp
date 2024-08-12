// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <concepts>

#include <catch2/catch_test_macros.hpp>

struct impossible_class {
  impossible_class() = delete;
};

template <class TFn, class TArg>
void test_invocable_two_args() {
  STATIC_CHECK_FALSE(std::invocable<TFn>);
  STATIC_CHECK_FALSE(std::invocable<TFn, TArg>);
  STATIC_CHECK(std::invocable<TFn, TArg, TArg>);
  STATIC_CHECK_FALSE(std::invocable<TFn, TArg, TArg, TArg>);
  STATIC_CHECK_FALSE(std::invocable<TFn, TArg, impossible_class>);
}