// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/config.hpp>
#include <FredEmmott/bindline/detail/is_valid_ordering.hpp>
#include <FredEmmott/bindline/extension_api/ordering_requirements_t.hpp>

#include <array>
#include <concepts>
#include <functional>
#include <string>
#include <tuple>
#include <utility>

namespace FredEmmott::bindline_detail {
template <template <class...> class T, class TProjection, class... TArgs>
struct bindable_factory_t;
}

namespace FredEmmott::bindline {

/** Base type for bindline components.
 *
 * When extending this class, you must implement the `bind_to()` method.
 */
struct bindable_t {
  static constexpr FredEmmott::bindline_extension_api::ordering_requirements_t
    ordering_requirements_v {
      FredEmmott::bindline_extension_api::ordering_requirements_t::
        no_ordering_requirements};

  /** Wrap a callable
   *
   * @param fn the callable to be wrapped
   * @returns the wrapped callable
   */
  [[nodiscard]]
  constexpr auto bind_to(auto&& fn) const
    = delete;

  /** Return a callable that is also bindable.
   *
   * bindables can usually be used in two ways:
   *
   * - `bind_x(f, args...)`
   * - `f | bind_x(args...)`
   *
   * It's usually clearest to define two overloads of `bind_x()` to handle these
   * cases, but when callables are valid arguments, this becomes ambiguous. This
   * function addresses this ambiguity based on later usage.
   *
   * For example:
   * - `bind_front()` and `bind_back()` use this with the `std::identity`
   *    projection
   * - `bind_refs_front()` and `bind_refs_back()` use this with a lambda
   *    projection that creates weak references
   *
   * @param T a `bindable_t` template class to wrap
   * @param TProjection a conversion functor class; you may want `std::identity`
   * @param TArgs the arguments passed to the `bind_x()` function
   *
   * @returns an object where:
   *  - the first argument is treated as a function if `operator()` is called
   *  - the first argument is treated as another argument if `bind_to()` is
   *    called
   */
  template <template <class...> class T, class TProjection, class... TArgs>
  [[nodiscard]]
  static constexpr auto make_projected(
    TProjection&& projection,
    TArgs&&... args) {
    return ::FredEmmott::bindline_detail::
      bindable_factory_t<T, std::decay_t<TProjection>, std::decay_t<TArgs>...>::
        make(
          std::forward<TProjection>(projection), std::forward<TArgs>(args)...);
  }

  /// Convenience wrapper for `make_projected()` using `std::identity()`
  template <template <class...> class T, class... TArgs>
  [[nodiscard]]
  static constexpr auto make(TArgs&&... args) {
    return make_projected<T, std::identity, TArgs...>(
      {}, std::forward<TArgs>(args)...);
  }
};

struct traced_bindline_marker_t {};

template <size_t... I>
struct instantiate_from_tuple_inner_t {
  instantiate_from_tuple_inner_t() = delete;
  constexpr instantiate_from_tuple_inner_t(std::index_sequence<I...>) {
  }

  template <template <class...> class T, class TTuple>
    requires(sizeof...(I) == std::tuple_size_v<TTuple>)
  using type = T<std::tuple_element_t<I, TTuple>...>;
};

template <class TTuple>
using inner_t = decltype(instantiate_from_tuple_inner_t {
  std::make_index_sequence<std::tuple_size_v<TTuple>> {}});

template <template <class...> class T, class TTuple>
using instantiate_from_tuple_t =
  typename inner_t<TTuple>::template type<T, TTuple>;

template <class TFn, std::derived_from<bindable_t>... TTrace>
  requires std::same_as<TFn, std::decay_t<TFn>> && (sizeof...(TTrace) >= 1)
struct traced_bindline : traced_bindline_marker_t {
  TFn fn;

  using trace_tuple_t = std::tuple<TTrace...>;

  static constexpr bool is_traced_bindline_v = true;

  template <std::derived_from<bindable_t> T>
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
  using next_t = instantiate_from_tuple_t<
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

template <class TLeft, class TBindable, class TBound>
constexpr auto wrap_with_trace(TBound&& bound) {
  if constexpr (std::derived_from<TLeft, traced_bindline_marker_t>) {
    using next_t = typename TLeft::template next_t<TBound, TBindable>;
    return next_t {.fn = std::forward<TBound>(bound)};
  } else {
    return traced_bindline<std::decay_t<TBound>, TBindable> {
      .fn = std::forward<TBound>(bound),
    };
  }
}

template <class T>
constexpr decltype(auto) unwrap(T&& lhs) {
  if constexpr (std::derived_from<T, traced_bindline_marker_t>) {
    return std::move(lhs.fn);
  } else {
    return std::forward<T>(lhs);
  }
}

template <class TLeft, class TRight>
constexpr auto concat_pipeline(TLeft&& lhs, TRight&& rhs) {
  auto bound = rhs.bind_to(unwrap(std::forward<TLeft>(lhs)));
  return wrap_with_trace<TLeft, TRight>(std::move(bound));
}

template <class TLeft, std::derived_from<bindable_t> TRight>
  requires(decltype(concat_pipeline<TLeft, TRight>(
    std::declval<TLeft>(),
    std::declval<TRight>()))::has_valid_ordering())
[[nodiscard]]
constexpr auto operator|(TLeft&& lhs, TRight&& rhs) {
  return concat_pipeline(std::forward<TLeft>(lhs), std::forward<TRight>(rhs));
}

}// namespace FredEmmott::bindline

namespace FredEmmott::bindline_extension_api {
using ::FredEmmott::bindline::bindable_t;
}

#include "detail/bindable_factory_t.hpp"