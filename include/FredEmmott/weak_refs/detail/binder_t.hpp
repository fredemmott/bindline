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

template <class TTraits, class TFn, weak_ref... TBinds>
  requires(sizeof...(TBinds) >= 1)
struct binder_t {
  static_assert(std::same_as<TFn, std::decay_t<TFn>>);
  static_assert((std::same_as<TBinds, std::decay_t<TBinds>> && ...));

  binder_t() = delete;
  template <class TInitFn, class... TInitBinds>
  constexpr binder_t(TInitFn&& fn, TInitBinds... binds)
    : mFn(std::forward<TInitFn>(fn)),
      mBinds(make_weak_ref(std::forward<TInitBinds>(binds))...) {
  }

  using strong_binds_t
    = std::tuple<decltype(lock_weak_ref(std::declval<TBinds>()))...>;

  template <class... Unbound>
  using final_args_t = decltype(TTraits::make_args_tuple(
    std::declval<strong_binds_t>(),
    std::declval<Unbound>()...));

  template <class... TArgs>
    requires std::invocable<TFn, TArgs...>
  static std::true_type can_apply_t(std::tuple<TArgs...>) {
    return {};
  };
  static std::false_type can_apply_t(...) {
    return {};
  };

  template <class TArgs>
  static constexpr bool can_apply_v
    = decltype(can_apply_t(std::declval<TArgs>()))::value;

  template <class... UnboundArgs>
    requires(can_apply_v<final_args_t<UnboundArgs...>>)
  constexpr void operator()(UnboundArgs&&... unboundArgs) const {
    strong_binds_t strong_binds = std::apply(
      []<class... CallArgs>(CallArgs&&... binds) -> decltype(auto) {
        return std::make_tuple(lock_weak_ref(std::forward<CallArgs>(binds))...);
      },
      mBinds);
    const auto all_live = std::apply(
      []<strong_ref... CallArgs>(CallArgs&&... binds) {
        return (static_cast<bool>(std::forward<CallArgs>(binds)) && ...);
      },
      strong_binds);
    if (!all_live) {
      return;
    }

    final_args_t<UnboundArgs...> final_args = TTraits::make_args_tuple(
      std::move(strong_binds), std::forward<UnboundArgs>(unboundArgs)...);

    std::apply(mFn, std::move(final_args));
  }

 private:
  TFn mFn;
  std::tuple<TBinds...> mBinds;
};

}// namespace FredEmmott::weak_refs_detail