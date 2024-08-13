// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/bindline/detail/drop_back_invoke_t.hpp>
#include <FredEmmott/bindline/extension_api/bindable_t.hpp>

namespace FredEmmott::bindline_extension_api {

/** A bindable that drops arguments from the end of the list.
 *
 * `TDropTraits` must be a struct with the following members:
 * ```c++
 * template<size_t DropCount, class T>
 * static constexpr bool can_drop_v;
 *
 * static constexpr size_t minimum_dropped_v
 * ```
 *
 * An argument will be dropped from the back if and only if:
 *  - all later arguments have already been dropped
 *  - the wrapped function is not invocable with fewer dropped arguments, unless
 *    fewer than `minimum_dropped_v` arguments have already been dropped
 *  - `can_drop_v<DropCount, T>` is true.
 *   - `DropCount` is how many arguments have already been dropped
 *   - `T` is the type of the argument
 *
 * This is valid if and only if at least `minimum_dropped_v` arguments are
 * dropped from the back.
 *
 * For examples, see:
 * - `FredEmmott::bindline_detail::drop_any_arg_traits`
 * - `FredEmmott::bindline_detail::drop_n_traits`
 * - `FredEmmott::bindline_detail::drop_winrt_event_args_traits`
 */
template <class TDropTraits>
struct drop_back_bindable_t : bindable_t {
  template <class TBindFn>
  [[nodiscard]]
  constexpr auto bind_to(TBindFn&& fn) const {
    return std::bind_front(
      []<class TCallFn, class... TArgs>(TCallFn&& fn, TArgs&&... args) constexpr
        requires FredEmmott::bindline_detail::drop_back_invocable_entrypoint<
                   TDropTraits,
                   TCallFn,
                   TArgs...>
      {
        return FredEmmott::bindline_detail::
          drop_back_invoke_entrypoint_t<TDropTraits, TCallFn, TArgs...>::invoke(
            std::forward<TCallFn>(fn), std::forward<TArgs>(args)...);
      },
      std::forward<TBindFn>(fn));
  }
};

template <class TDropTraits, class TFn, class... TArgs>
concept drop_back_invocable = ::FredEmmott::bindline_detail::
  drop_back_invocable_entrypoint<TDropTraits, TFn, TArgs...>;

}// namespace FredEmmott::bindline_extension_api