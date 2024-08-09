// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bind.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bind;

TEST_CASE("bind_front without a pipeline") {
  std::vector<std::pair<int, int>> log {};

  auto f = [&log](auto front, auto back) { log.push_back({front, back}); };
  // Let's check the basics
  f(123, __LINE__);
  CHECK(log.back() == std::pair {123, __LINE__ - 1});
  // Okie, check it as part of a pipeline front
  (f | bind_front(123))(__LINE__);
  CHECK(log.back() == std::pair {123, __LINE__ - 1});
  // ... and now without :)
  bind_front(f, 123)(__LINE__);
  CHECK(log.back() == std::pair {123, __LINE__ - 1});
}

TEST_CASE("bind_back without a pipeline") {
#ifndef __cpp_lib_bind_back
  SKIP("std::bind_back is not available in this build");
#else
  std::vector<std::pair<int, int>> log {};

  auto f = [&log](auto front, auto back) { log.push_back({front, back}); };
  // Let's check the basics
  f(123, __LINE__);
  CHECK(log.back() == std::pair {123, __LINE__ - 1});
  // Okie, check it as part of a pipeline back
  (f | bind_back(123))(__LINE__);
  CHECK(log.back() == std::pair {__LINE__ - 1, 123});
  // ... and now without :)
  bind_back(f, 123)(__LINE__);
  CHECK(log.back() == std::pair {__LINE__ - 1, 123});
#endif
}