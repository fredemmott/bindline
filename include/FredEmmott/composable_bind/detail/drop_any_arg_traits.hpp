// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <cstddef>

namespace FredEmmott::composable_bind_detail {

struct drop_any_arg_traits {
  template <size_t DropCount, class T>
  static constexpr bool can_drop_v = true;

  static constexpr size_t minimum_dropped_v = 0;
};

}// namespace FredEmmott::composable_bind_detail