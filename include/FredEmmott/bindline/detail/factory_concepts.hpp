// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "invocable_bindable_t.hpp"

namespace FredEmmott::bindline_detail {

template <template <class...> class T, class... TArgs>
concept template_instantiable_from = requires { typename T<TArgs...>; };

template <template <class...> class T, class... TArgs>
concept bindable_constructible_from
  = template_instantiable_from<T, std::decay_t<TArgs>...>
  && std::constructible_from<T<std::decay_t<TArgs>...>, TArgs...>
  && std::
    derived_from<T<TArgs...>, ::FredEmmott::bindline_extension_api::bindable_t>;

template <class T, class TOther>
concept decays_to = std::same_as<std::decay_t<T>, std::decay_t<TOther>>;

template <class T, class TProjection>
concept projectable = std::invocable<TProjection, T>
  && !decays_to<void, std::invoke_result_t<TProjection, T>>;

static_assert(projectable<int, std::identity>);
static_assert(projectable<decltype([]() {}), std::identity>);

template <class TProjection, projectable<TProjection> TArg>
using projected_t = std::decay_t<std::invoke_result_t<TProjection, TArg>>;

template <template <class...> class T, class TProjection, class... TArgs>
concept is_bindable = (projectable<TArgs, TProjection> && ...)
  && bindable_constructible_from<T, projected_t<TProjection, TArgs>...>;

template <
  template <class...>
  class T,
  class TProjection,
  class TFirst = void,
  class... TRest>
concept is_invocable
  = (!std::same_as<TFirst, void>) && is_bindable<T, TProjection, TRest...>
  && requires(T<projected_t<TProjection, TRest>...> v, TFirst f) {
       v.bind_to(f);
     };
}// namespace FredEmmott::bindline_detail