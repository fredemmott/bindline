// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../bindable_t.hpp"
#include "../detail/drop_back_invoke_t.hpp"

namespace FredEmmott::composable_bind_extension_api {

template <class TDropTraits>
struct drop_back_bindable_t : bindable_t {
  template <class TBindFn>
  [[nodiscard]]
  constexpr auto bind_to(TBindFn&& fn) const {
    return std::bind_front(
      []<class TCallFn, class... TArgs>(
        TCallFn&& fn, TArgs&&... args) constexpr {
        return FredEmmott::composable_bind_detail::
          drop_back_invoke_entrypoint_t<TDropTraits, TCallFn, TArgs...>::invoke(
            std::forward<TCallFn>(fn), std::forward<TArgs>(args)...);
      },
      std::forward<TBindFn>(fn));
  }
};

template <class TDropTraits, class TFn, class... TArgs>
concept drop_back_invocable = ::FredEmmott::composable_bind_detail::
  drop_back_invocable_entrypoint<TDropTraits, TFn, TArgs...>;

}// namespace FredEmmott::composable_bind_extension_api