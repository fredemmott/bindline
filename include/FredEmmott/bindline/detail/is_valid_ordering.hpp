// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/detail/meta/parameter_pack_element_t.hpp>
#include <FredEmmott/bindline/detail/meta/parameter_pack_slice.hpp>
#include <FredEmmott/bindline/extension_api/ordering_requirements_t.hpp>

namespace FredEmmott::bindline_detail::inline validation {

enum class VerifyBehavior {
  ReturnBool,
  StaticAssert,
};

template <class... TLeading>
struct is_valid_order_leading_t {
  template <VerifyBehavior Behavior>
  static constexpr bool verify_not_invoked_before_context_switch() {
    using enum FredEmmott::bindline_extension_api::ordering_requirements_t;
    if constexpr (Behavior == VerifyBehavior::ReturnBool) {
      return (
        (TLeading::ordering_requirements_v != invoke_before_context_switch)
        && ...);
    } else {
      static_assert(
        ((TLeading::ordering_requirements_v != invoke_before_context_switch)
         && ...),
        "T must be invoked before the context switch; this means it must be "
        "later in the bindline");
    }

    return true;
  }

  template <class T, VerifyBehavior Behavior>
  static constexpr bool verify() {
    using enum FredEmmott::bindline_extension_api::ordering_requirements_t;
    if constexpr (T::ordering_requirements_v != is_context_switch) {
      return true;
    } else {
      return verify_not_invoked_before_context_switch<Behavior>();
    }
  }
};

template <class... TTrailing>
struct is_valid_order_trailing_t {
  template <VerifyBehavior Behavior>
  static constexpr bool verify_not_invoked_after_context_switch() {
    using enum FredEmmott::bindline_extension_api::ordering_requirements_t;
    if constexpr (Behavior == VerifyBehavior::ReturnBool) {
      return (
        (TTrailing::ordering_requirements_v != invoke_after_context_switch)
        && ...);
    } else {
      static_assert(
        ((TTrailing::ordering_requirements_v != invoke_after_context_switch)
         && ...),
        "T must be invoked after the context switch; this means it must be "
        "earlier in the bindline");
    }

    return true;
  }

  template <class T, VerifyBehavior Behavior>
  static constexpr bool verify() {
    using enum FredEmmott::bindline_extension_api::ordering_requirements_t;
    if constexpr (T::ordering_requirements_v != is_context_switch) {
      return true;
    } else {
      return verify_not_invoked_after_context_switch<Behavior>();
    }
  }
};

template <size_t... Is>
struct is_valid_order_outer_t {
  is_valid_order_outer_t() = delete;
  constexpr is_valid_order_outer_t(std::index_sequence<Is...>) {
  }

  template <VerifyBehavior Behavior, size_t I, class... TArgs>
    requires(I < sizeof...(Is) && I < sizeof...(TArgs))
  static constexpr bool is_valid_single() {
    using namespace FredEmmott::bindline_detail::meta;

    using T = parameter_pack_element_t<I, TArgs...>;

    if constexpr (I > 0) {
      using leading_t = instantiate_from_parameter_pack_slice_t<
        0,
        I,
        is_valid_order_leading_t,
        TArgs...>;
      if constexpr (!leading_t::template verify<T, Behavior>()) {
        return false;
      }
    }

    if constexpr (I + 1 < sizeof...(TArgs)) {
      using trailing_t = instantiate_from_parameter_pack_slice_t<
        I + 1,
        sizeof...(TArgs) - (I + 1),
        is_valid_order_trailing_t,
        TArgs...>;
      if constexpr (!trailing_t::template verify<T, Behavior>()) {
        return false;
      }
    }

    return true;
  }

  template <class... TArgs>
    requires(sizeof...(Is) == sizeof...(TArgs))
  [[nodiscard]]
  static constexpr bool is_valid() {
    return (is_valid_single<VerifyBehavior::ReturnBool, Is, TArgs...>() && ...);
  }

  template <class... TArgs>
    requires(sizeof...(Is) == sizeof...(TArgs))
  static constexpr void verify() {
    (is_valid_single<VerifyBehavior::StaticAssert, Is, TArgs...>(), ...);
  }
};

template <class... TArgs>
constexpr bool is_valid_ordering_v = decltype(is_valid_order_outer_t {
  std::index_sequence_for<TArgs...> {}})::template is_valid<TArgs...>();

template <class... TArgs>
constexpr void static_assert_valid_ordering() {
  decltype(is_valid_order_outer_t {
    std::index_sequence_for<TArgs...> {}})::template verify<TArgs...>();
};

}// namespace FredEmmott::bindline_detail::inline validation