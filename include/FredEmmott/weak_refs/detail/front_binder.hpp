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

template <class TFn, weak_ref... TBinds>
  requires(sizeof...(TBinds) >= 1)
struct front_binder {
  using function_t = TFn;

  front_binder() = delete;
  template <class TInitFn, class... TInitBinds>
  front_binder(TInitFn&& fn, TInitBinds... binds)
    : mFn(std::forward<TInitFn>(fn)),
      mBinds(
        std::make_tuple(make_weak_ref(std::forward<TInitBinds>(binds))...)) {
  }

  template <class... UnboundArgs>
    requires std::invocable<TFn, strong_ref_t<TBinds>..., UnboundArgs...>
  void operator()(UnboundArgs&&... unboundArgs) const {
    auto strong_binds = std::apply(
      [](const auto&... binds) {
        return std::make_tuple(lock_weak_ref(binds)...);
      },
      mBinds);
    const auto all_live = std::apply(
      [](const auto&... binds) {
        static_assert((strong_ref<decltype(binds)> && ...));
        return (static_cast<bool>(binds) && ...);
      },
      strong_binds);
    if (!all_live) {
      return;
    }

    std::apply(
      mFn,
      std::tuple_cat(
        strong_binds,
        std::make_tuple(std::forward<UnboundArgs>(unboundArgs)...)));
  }

 private:
  const TFn mFn;
  const std::tuple<TBinds...> mBinds;
};

}// namespace FredEmmott::weak_refs_detail