// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <cstddef>
#include <tuple>

namespace FredEmmott::bindline_detail::inline meta {

template <size_t I, class... TArgs>
  requires(I < sizeof...(TArgs))
using parameter_pack_element_t = std::tuple_element_t<I, std::tuple<TArgs...>>;

}