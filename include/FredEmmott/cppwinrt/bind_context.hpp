// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include "config.hpp"

#include "detail/context_binder.hpp"

namespace FredEmmott::cppwinrt {

/** Create a callable that invokes the parameter in the specified thread.
 *
 * The returned callable will take the same arguments as `f`.
 *
 * If `context` is a `DispatcherQueue`:
 * - `wil::resume_foreground()` will be used if available
 * - without WIL, `DispatcherQueue::TryEnqueue()` will be used
 * - `f` will *always* be enqueueed, even if it is already on the correct
 *   thread; this is so that behavior is consistent regardless of original
 *   thread, and for consistency with the behavior of `wil::resume_foreground()`
 *
 * If `Context` is a `winrt::apartment_context`:
 * - `f` will be immediately if we're already on the correct thread; this is
 *   because there may not be a queue available
 * - otherwise, it will only be invoked if the context corresponds to a thread
 *   which is running a C++/WinRT-compatible event loop
 *
 * @param F a callable
 * @param Context a `winrt::apartment_context` or one of the various
 *  `DispatcherQueue`s.
 */
template <class F, cppwinrt_detail::switchable_context Context>
[[nodiscard]]
auto bind_context(F&& f, Context&& context) {
  return cppwinrt_detail::
    context_binder<std::decay_t<F>, std::decay_t<Context>> {
      std::forward<F>(f),
      std::forward<Context>(context),
    };
}

}// namespace FredEmmott::cppwinrt