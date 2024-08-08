// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <concepts>
#include <functional>
#include <tuple>

#include <catch2/catch_test_macros.hpp>

namespace FredEmmott::bind {

struct bind_component_t {
  constexpr auto bind_to(const auto& fn) = delete;
};

template <class TFunction, std::derived_from<bind_component_t> TComponent>
constexpr auto operator|(TFunction&& lhs, TComponent&& rhs) {
  return rhs.bind_to(std::forward<TFunction>(lhs));
}

}// namespace FredEmmott::bind