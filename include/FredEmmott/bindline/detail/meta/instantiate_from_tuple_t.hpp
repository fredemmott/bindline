// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace FredEmmott::bindline_detail::inline meta {
template <size_t... I>
struct instantiate_from_tuple_inner_t {
  instantiate_from_tuple_inner_t() = delete;
  constexpr instantiate_from_tuple_inner_t(std::index_sequence<I...>) {
  }

  template <template <class...> class T, class TTuple>
    requires(sizeof...(I) == std::tuple_size_v<TTuple>)
  using type = T<std::tuple_element_t<I, TTuple>...>;
};

template <template <class...> class T, class TTuple>
struct instantiate_from_tuple_outer_t {
  using inner_t = decltype(instantiate_from_tuple_inner_t {
    std::make_index_sequence<std::tuple_size_v<TTuple>> {}});
  using type = typename inner_t::template type<T, TTuple>;
};

template <template <class...> class T, class TTuple>
using instantiate_from_tuple_t =
  typename instantiate_from_tuple_outer_t<T, TTuple>::type;
}// namespace FredEmmott::bindline_detail::inline meta