// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/config.hpp>
#include <FredEmmott/bindline/detail/is_valid_ordering.hpp>
#include <FredEmmott/bindline/detail/meta/instantiate_from_tuple_t.hpp>
#include <FredEmmott/bindline/extension_api/ordering_requirements_t.hpp>

#include <array>
#include <concepts>
#include <functional>
#include <string>
#include <tuple>
#include <typeinfo>

namespace FredEmmott::bindline_extension_api {
struct bindable_t;
}

namespace FredEmmott::bindline_detail {

struct traced_bindline_marker_t {};

template <
  class TFn,
  std::derived_from<bindline_extension_api::bindable_t>... TTrace>
  requires std::same_as<TFn, std::decay_t<TFn>> && (sizeof...(TTrace) >= 1)
struct traced_bindline : traced_bindline_marker_t {
  TFn fn;

  using trace_tuple_t = std::tuple<TTrace...>;

  static constexpr bool is_traced_bindline_v = true;

  template <std::derived_from<bindline_extension_api::bindable_t> T>
  static constexpr std::tuple<T> rhs_tuple_fn() {
#if FREDEMMOTT_BINDLINE_CAN_STATIC_ASSERT_FALSE
    static_assert(false);
#endif
  };

  template <std::derived_from<traced_bindline_marker_t> T>
  static constexpr typename T::trace_tuple_t rhs_tuple_fn() {
#if FREDEMMOTT_BINDLINE_CAN_STATIC_ASSERT_FALSE
    static_assert(false);
#endif
  };

  template <class T>
  using rhs_tuple_t = decltype(rhs_tuple_fn<T>());

  template <class TNextFn, class TNextBindable>
  using append_rhs_tuple_t = decltype(std::tuple_cat(
    std::tuple {
      std::declval<TNextFn>(),
      std::declval<TTrace>()...,
    },
    std::declval<rhs_tuple_t<TNextBindable>>()));

  template <class TNextFn, class TNextBindable>
  using next_t = bindline_detail::instantiate_from_tuple_t<
    traced_bindline,
    append_rhs_tuple_t<TNextFn, TNextBindable>>;

  template <class... TArgs>
    requires std::invocable<TFn, TArgs...>
  constexpr decltype(auto) operator()(TArgs&&... args) const {
    static_assert_valid_ordering();
    return std::invoke(fn, std::forward<TArgs>(args)...);
  }

  static constexpr void static_assert_valid_ordering() {
    ::FredEmmott::bindline_detail::static_assert_valid_ordering<TTrace...>();
  }

  [[nodiscard]]
  static constexpr bool has_valid_ordering() {
    return ::FredEmmott::bindline_detail::is_valid_ordering_v<TTrace...>;
  }

  // Not constexpr because type_info.name() isn't constexpr
  static auto trace_as_string() {
    std::array<std::string, sizeof...(TTrace)> parts {typeid(TTrace).name()...};

    auto parts_bytes
      = (0 + ... + (std::string_view {typeid(TTrace).name()}.size()));
    constexpr auto separator_count = sizeof...(TTrace) - 1;
    constexpr std::string_view separator {", "};
    // No `-1` here as we build the string with trailing separators, then chomp
    // them off with the string_view when formatting
    constexpr auto separator_bytes = separator_count * separator.size();
    auto total_bytes = parts_bytes + separator_bytes;

    std::string buffer;
    buffer.resize(total_bytes + separator_bytes);
    buffer[0] = '[';

    size_t offset = 1;
    for (auto part: parts) {
      std::copy(part.begin(), part.end(), buffer.begin() + offset);
      offset += part.size();
      std::copy(separator.begin(), separator.end(), buffer.begin() + offset);
      offset += separator.size();
    }

    // not using std::format to support older G++ versions (e.g. GCC 11.4 in
    // Ubuntu 22.04
    buffer.resize(buffer.size() - separator.size());
    buffer.push_back(']');
    return buffer;
  };
};

}// namespace FredEmmott::bindline_detail