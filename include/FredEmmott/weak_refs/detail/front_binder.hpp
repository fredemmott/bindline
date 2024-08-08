// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <tuple>
#include <utility>

#include "../concepts.hpp"
#include "../lock_weak_ref.hpp"
#include "../make_weak_ref.hpp"
#include "static_const.hpp"

namespace FredEmmott::weak_refs_detail {

using namespace FredEmmott::weak_refs;

template <class T>
auto weak_or_passthrough(T&& arg) {
  if constexpr (weak_refs::convertible_to_weak_ref<std::decay_t<T>>) {
    return weak_refs::make_weak_ref(arg);
  } else {
    return std::decay_t<T> {std::forward<T>(arg)};
  }
}

template <class T>
auto strong_or_passthrough(T&& arg) {
  if constexpr (weak_refs::weak_ref<std::decay_t<T>>) {
    return weak_refs::lock_weak_ref(arg);
  } else {
    return std::decay_t<T> {std::forward<T>(arg)};
  }
}

template <class T>
using strong_or_passthrough_t
  = decltype(strong_or_passthrough(std::declval<T>()));
template <class T>
using weak_or_passthrough_t = decltype(weak_or_passthrough(std::declval<T>()));

template <class TFirst, class TFn>
  requires std::is_member_function_pointer_v<TFn>
decltype(std::addressof(
  *std::declval<strong_or_passthrough_t<weak_or_passthrough_t<TFirst>>>()))
bound_arg_firstfn(TFn&&);
template <class TFirst>
strong_or_passthrough_t<weak_or_passthrough_t<TFirst>> bound_arg_firstfn(...);

template <class TFn, class TFirst>
using bound_arg_first_t
  = decltype(bound_arg_firstfn<TFirst>(std::declval<TFn>()));
template <class T>
using bound_arg_rest_t = strong_or_passthrough_t<weak_or_passthrough_t<T>>;

template <class TFn, class TFirst, class... TRest>
struct front_binder {
  using function_t = TFn;

  front_binder() = delete;
  template <class TInitFn, class TInitFirst, class... TInitRest>
  front_binder(TInitFn&& fn, TInitFirst&& first, TInitRest&&... rest)
    : mFn(std::forward<TInitFn>(fn)),
      mFirst(weak_or_passthrough(std::forward<TInitFirst>(first))),
      mRest(std::make_tuple(
        weak_or_passthrough(std::forward<TInitRest>(rest))...)) {
    static_assert(
      weak_ref<weak_or_passthrough_t<TFirst>>
        && (weak_ref<weak_or_passthrough_t<TRest>> && ...),
      "all bound arguments must be convertible to weak_refs");
  }

  template <class... UnboundArgs>
    requires std::invocable<
      TFn,
      bound_arg_first_t<TFn, TFirst>,
      bound_arg_rest_t<TRest>...,
      UnboundArgs...>
  void operator()(UnboundArgs&&... unboundArgs) const {
    auto strong_first = strong_or_passthrough(mFirst);
    if (!strong_first) {
      return;
    }

    auto strong_rest = std::apply(
      []<class... WeakRest>(WeakRest&&... rest) {
        return std::make_tuple(
          strong_or_passthrough(std::forward<WeakRest>(rest))...);
      },
      mRest);
    const auto all_live = std::apply(
      [](const auto&... rest) {
        static_assert((strong_ref<decltype(rest)> && ...));
        return (static_cast<bool>(rest) && ...);
      },
      strong_rest);
    if (!all_live) {
      return;
    }

    std::apply(
      mFn,
      std::tuple_cat(
        std::make_tuple(bound_first(strong_first)),
        strong_rest,
        std::make_tuple(std::forward<UnboundArgs>(unboundArgs)...)));
  }

 private:
  const TFn mFn;
  const weak_or_passthrough_t<TFirst> mFirst;
  const std::tuple<weak_or_passthrough_t<TRest>...> mRest;

  static bound_arg_first_t<TFn, TFirst> bound_first(
    const strong_or_passthrough_t<weak_or_passthrough_t<TFirst>>& strong) {
    if constexpr (std::is_member_function_pointer_v<TFn>) {
      return std::addressof(*strong);
    } else {
      return strong;
    }
  }
};

}// namespace FredEmmott::weak_refs_detail