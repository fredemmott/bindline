// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <winrt/base.h>

#include <FredEmmott/cppwinrt/config.hpp>

#if FREDEMMOTT_CPPWINRT_ENABLE_WIL
#include <wil/cppwinrt_helpers.h>
#endif

#include <concepts>
#include <memory>

namespace FredEmmott::cppwinrt_detail {
struct impossible_argument_t {
  impossible_argument_t() = delete;
};

}// namespace FredEmmott::cppwinrt_detail

#if FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND
namespace winrt {
// Avoid undefined function compile errors if none of the definitions have been
// included
inline void resume_foreground(
  const FredEmmott::cppwinrt_detail::impossible_argument_t&) {
}
};// namespace winrt
#endif

namespace FredEmmott::cppwinrt_detail {

template <class A, class B>
concept different_to = !std::same_as<A, B>;

auto switch_context(const winrt::apartment_context& ctx) {
  return ctx;
}

#if FREDEMMOTT_CPPWINRT_ENABLE_WINRT_RESUME_FOREGROUND
template <class T>
concept cppwinrt_dispatcherqueue = requires(T v) {
  { winrt::resume_foreground(v) } -> different_to<void>;
};

template <cppwinrt_dispatcherqueue T>
auto switch_context(T&& v) {
  return winrt::resume_foreground(std::forward<T>(v));
}
#else
template <class T>
concept cppwinrt_dispatcherqueue = false;
#endif

#if FREDEMMOTT_CPPWINRT_ENABLE_WIL
template <class T>
concept wil_thread = (!cppwinrt_dispatcherqueue<T>)
  && requires(T v) { wil::resume_foreground(v); };

template <wil_thread T>
auto switch_context(T&& v) {
  return wil::resume_foreground(std::forward<T>(v));
}
#endif

template <class T>
concept switchable_context = requires(T v) { switch_context(v); };

template <switchable_context TContext, class TFn>
struct context_binder {
 public:
  using function_t = TFn;

  context_binder() = delete;
  template <std::convertible_to<TContext> InitContext, class InitFn>
  context_binder(InitContext&& context, InitFn&& fn)
    : mContext(std::forward<InitContext>(context)),
      mFn(std::forward<InitFn>(fn)) {
  }

  // Not using perfect forwarding for the arguments as this is an async
  // invocation/coroutine - we **MUST** copy the arguments.
  template <class... UnboundArgs>
    requires std::invocable<TFn, UnboundArgs...>
  void operator()(const UnboundArgs&... unboundArgs) const {
    dispatch_coro(mContext, mFn, unboundArgs...);
  }

 private:
  TContext mContext;
  TFn mFn;

  template <class... UnboundArgs>
    requires std::invocable<TFn, UnboundArgs...>
  static winrt::fire_and_forget
  dispatch_coro(TContext context, TFn fn, UnboundArgs... unboundArgs) {
    static_assert(!std::is_reference_v<decltype(context)>);
    static_assert(!std::is_reference_v<decltype(fn)>);
    static_assert((!std::is_reference_v<decltype(unboundArgs)> && ...));
    co_await switch_context(context);
    std::invoke(fn, unboundArgs...);
  }
};

}// namespace FredEmmott::cppwinrt_detail

namespace FredEmmott::cppwinrt {

template <cppwinrt_detail::switchable_context Context, class F>
auto bind_context(Context&& context, F&& f) {
  return cppwinrt_detail::
    context_binder<std::decay_t<Context>, std::decay_t<F>> {
      std::forward<Context>(context), std::forward<F>(f)};
}

}// namespace FredEmmott::cppwinrt