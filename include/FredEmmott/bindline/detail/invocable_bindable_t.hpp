// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/bindable_t.hpp>

#include <atomic>
#include <concepts>

namespace FredEmmott::bindline_detail {

template <template <class...> class TNext, class TFirst, class... TRest>
struct invocable_bindable_t : ::FredEmmott::bindline::bindable_t {
  static_assert(
    std::same_as<TFirst, std::decay_t<TFirst>>
    && (std::same_as<TRest, std::decay_t<TRest>> && ...));

  using bindable_t = TNext<TFirst, TRest...>;
  using invocable_t = TNext<TRest...>;

  invocable_bindable_t() = delete;
  invocable_bindable_t(const invocable_bindable_t&) = default;
  invocable_bindable_t(invocable_bindable_t&&) = default;

  invocable_bindable_t& operator=(const invocable_bindable_t&) = delete;
  invocable_bindable_t& operator=(invocable_bindable_t&&) = delete;

  template <class... TInit>
    requires std::constructible_from<TNext<TFirst, TRest...>, TFirst, TRest...>
    && std::constructible_from<TNext<TRest...>, TRest...>
  constexpr invocable_bindable_t(TInit&&... args)
    : mArgs(std::forward_as_tuple(std::forward<TInit>(args)...)) {
  }

  template <class TFn>
    requires requires(bindable_t bindable, TFn fn) { bindable.bind_to(fn); }
  [[nodiscard]]
  constexpr auto bind_to(const TFn& fn) const {
    const auto next = std::apply(
      []<class... NextArgs>(NextArgs&&... nextArgs) {
        return bindable_t {std::forward<NextArgs>(nextArgs)...};
      },
      mArgs);

    return next.bind_to(fn);
  }

  template <class... Unbound>
    requires std::invocable<
      decltype(std::declval<invocable_t>().bind_to(std::declval<TFirst>())),
      Unbound...>
  [[nodiscard]]
  constexpr auto operator()(Unbound&&... unbound) const {
    const auto next = std::apply(
      []<class... NextArgs>(auto, NextArgs&&... nextArgs) {
        // Ignored first arg is the function we're invoking
        return invocable_t {std::forward<NextArgs>(nextArgs)...};
      },
      mArgs);
    return std::invoke(
      next.bind_to(get<0>(mArgs)), std::forward<Unbound>(unbound)...);
  }

 private:
  std::tuple<TFirst, TRest...> mArgs;
};

}// namespace FredEmmott::bindline_detail