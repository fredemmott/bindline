// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace FredEmmott::bindline_detail {

// TODO (C++26): Delet This.
//
// Use parameter pack indexing instead.
template <size_t... I>
struct drop_last_impl_t {
  drop_last_impl_t() = delete;
  constexpr drop_last_impl_t(std::index_sequence<I...>) {
  }

  template <template <class...> class T, class... TArgs>
  using next_t = T<std::tuple_element_t<I, std::tuple<TArgs...>>...>;

  template <class TIn>
  static constexpr decltype(auto) next_tuple(TIn&& in) {
    return std::forward_as_tuple(
      std::forward<std::tuple_element_t<I, TIn>>(get<I>(in))...);
  }
};

template <class... TArgs>
struct drop_last_seq_t {
  template <class TFirst, class... TRest>
  static std::index_sequence_for<TRest...> seq_t_fn(TFirst&&, TRest&&...);
  static std::index_sequence<> seq_t_fn();

  using type = decltype(seq_t_fn(std::declval<TArgs>()...));
};

struct drop_last_t {
  template <class... TArgs>
  using seq_t = typename drop_last_seq_t<TArgs...>::type;

  template <class... TArgs>
  using impl_t = decltype(drop_last_impl_t(seq_t<TArgs...> {}));

  template <template <class...> class T, class... TArgs>
  using next_t = typename impl_t<TArgs...>::template next_t<T, TArgs...>;

  template <size_t ToDrop = 1, class... TArgs>
  static constexpr decltype(auto) next_tuple(std::tuple<TArgs...>&& args) {
    static_assert(ToDrop > 0);
    auto drop_one = impl_t<TArgs...>::next_tuple(std::move(args));
    if constexpr (ToDrop == 1) {
      return drop_one;
    } else {
      return drop_last_t::next_tuple<ToDrop - 1>(std::move(drop_one));
    }
  }

  template <size_t ToDrop = 1, class... TArgs>
  static constexpr decltype(auto) make_tuple(TArgs&&... args) {
    static_assert(ToDrop > 0);
    return drop_last_t::next_tuple<ToDrop>(
      std::forward_as_tuple(std::forward<TArgs>(args)...));
  }
};

static_assert(drop_last_t::make_tuple(1, 2, 3) == std::tuple {1, 2});
static_assert(drop_last_t::make_tuple<1>(1, 2, 3) == std::tuple {1, 2});
static_assert(drop_last_t::make_tuple<2>(1, 2, 3) == std::tuple {1});
static_assert(drop_last_t::make_tuple<3>(1, 2, 3) == std::tuple {});

}// namespace FredEmmott::bindline_detail