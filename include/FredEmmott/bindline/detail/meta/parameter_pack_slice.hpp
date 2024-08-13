// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace FredEmmott::bindline_detail::inline meta {

template <size_t First, size_t... I, class... TArgs>
decltype(auto) constexpr parameter_pack_slice_inner(
  std::index_sequence<I...>,
  TArgs&&... args) {
  auto args_tuple = std::forward_as_tuple(std::forward<TArgs>(args)...);
  return std::forward_as_tuple(
    std::forward<std::tuple_element_t<I + First, decltype(args_tuple)>>(
      std::get<I + First>(args_tuple))...);
}

/// std::tuple { TArgs...[First], ..., TArgs[First + Count] }
template <size_t First, size_t Count, class... TArgs>
  requires(First + Count <= sizeof...(TArgs))
decltype(auto) constexpr parameter_pack_slice(TArgs&&... args) {
  return parameter_pack_slice_inner<First>(
    std::make_index_sequence<Count> {}, std::forward<TArgs>(args)...);
}

template <size_t... I>
struct instantiate_from_parameter_pack_slice_inner_t {
  instantiate_from_parameter_pack_slice_inner_t() = delete;
  constexpr instantiate_from_parameter_pack_slice_inner_t(
    std::index_sequence<I...>) {
  }

  template <
    size_t First,
    size_t Count,
    template <class...>
    class T,
    class... TArgs>
    requires(Count == sizeof...(I)) && (First + Count <= sizeof...(TArgs))
  using next_t = T<std::tuple_element_t<First + I, std::tuple<TArgs...>>...>;
};

template <
  size_t First,
  size_t Count,
  template <class...>
  class T,
  class... TArgs>
struct instantiate_from_parameter_pack_slice_outer_t {
  using inner_t = decltype(instantiate_from_parameter_pack_slice_inner_t {
    std::make_index_sequence<Count> {}});

  using next_t = typename inner_t::template next_t<First, Count, T, TArgs...>;
};

/// Typename T<TArgs[First], ..., TArgs...[First + Count]>
template <
  size_t First,
  size_t Count,
  template <class...>
  class T,
  class... TArgs>
using instantiate_from_parameter_pack_slice_t =
  typename instantiate_from_parameter_pack_slice_outer_t<
    First,
    Count,
    T,
    TArgs...>::next_t;

}// namespace FredEmmott::bindline_detail::inline meta