// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <future>
#include <latch>

inline void check_forwards_arguments(auto context) {
  size_t last_line = 0;

  std::promise<void> promise;

  auto fn = [&](auto a, auto b, auto c, auto line) {
    CHECK(std::vector<int> {a, b, c} == std::vector {123, 456, 789});
    last_line = static_cast<size_t>(line);
    promise.set_value();
  };

  fn(123, 456, 789, __LINE__);

  promise = {};
  bind_context(fn, context)(123, 456, 789, __LINE__);
  promise.get_future().wait();
  CHECK(last_line == __LINE__ - 2);
}