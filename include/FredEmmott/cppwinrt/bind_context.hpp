// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include "config.hpp"

#include "detail/context_binder.hpp"

namespace FredEmmott::cppwinrt {

template <class F, cppwinrt_detail::switchable_context Context>
auto bind_context(F&& f, Context&& context) {
  return cppwinrt_detail::
    context_binder<std::decay_t<F>, std::decay_t<Context>> {
      std::forward<F>(f),
      std::forward<Context>(context),
    };
}

}// namespace FredEmmott::cppwinrt