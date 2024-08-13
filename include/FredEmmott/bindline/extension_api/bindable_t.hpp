// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <FredEmmott/bindline/config.hpp>
#include <FredEmmott/bindline/detail/concat_pipeline.hpp>

namespace FredEmmott::bindline_detail {
template <template <class...> class T, class TProjection, class... TArgs>
struct bindable_factory_t;
}

namespace FredEmmott::bindline_extension_api {

/** Base type for bindline components.
 *
 * When extending this class, you must implement the `bind_to()` method.
 */
struct bindable_t {
  static constexpr ordering_requirements_t ordering_requirements_v {
    ordering_requirements_t::no_ordering_requirements};

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

template <class TLeft, std::derived_from<bindable_t> TRight>
  requires(
    decltype(::FredEmmott::bindline_detail::concat_pipeline<TLeft, TRight>(
      std::declval<TLeft>(),
      std::declval<TRight>()))::has_valid_ordering())
[[nodiscard]]
constexpr auto operator|(TLeft&& lhs, TRight&& rhs) {
  return ::FredEmmott::bindline_detail::concat_pipeline(
    std::forward<TLeft>(lhs), std::forward<TRight>(rhs));
}

}// namespace FredEmmott::bindline_extension_api

namespace FredEmmott::bindline_extensions {
using ::FredEmmott::bindline_extension_api::bindable_t;
}

#include <FredEmmott/bindline/detail/bindable_factory_t.hpp>