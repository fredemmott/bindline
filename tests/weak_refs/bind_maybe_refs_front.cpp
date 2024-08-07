// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

template <class... Args>
auto bind_function_under_test(Args&&... args) {
  return bind_maybe_refs_front(std::forward<Args>(args)...);
}

#include "common/bind_refs.hpp"