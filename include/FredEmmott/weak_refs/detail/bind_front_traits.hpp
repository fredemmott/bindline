// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <tuple>

namespace FredEmmott::weak_refs_detail {

struct bind_front_traits {
  template <class... Unbound>
  [[nodiscard]]
  static constexpr auto make_args_tuple(
    const auto& strong_binds,
    Unbound&&... unbound) {
    return std::tuple_cat(
      strong_binds, std::forward_as_tuple(std::forward<Unbound>(unbound)...));
  }
};

}// namespace FredEmmott::weak_refs_detail