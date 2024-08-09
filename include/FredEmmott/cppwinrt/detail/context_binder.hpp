// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once
#include "../config.hpp"

#include <winrt/base.h>

#if FREDEMMOTT_CPPWINRT_ENABLE_WIL
#include <wil/cppwinrt_helpers.h>
#endif

#include <concepts>
#include <functional>
#include <memory>

namespace FredEmmott::cppwinrt_detail {

template <class A, class B>
concept different_to = !std::same_as<A, B>;

auto switch_context_awaitable(const winrt::apartment_context& ctx) {
  return ctx;
}

#if FREDEMMOTT_CPPWINRT_ENABLE_WIL
template <class T>
concept wil_thread = requires(T v) { wil::resume_foreground(v); };

template <wil_thread T>
auto switch_context_awaitable(T&& v) {
  return wil::resume_foreground(std::forward<T>(v));
}
#endif

template <class T>
concept dispatcher_queue = requires(T v) { v.TryEnqueue([]() {}); };

template <class T>
concept awaitable_context = requires(T v) { switch_context_awaitable(v); };

template <class T>
concept switchable_context = dispatcher_queue<T> || awaitable_context<T>;

template <class TFn, switchable_context TContext>
struct context_binder {
 public:
  using function_t = TFn;
  static constexpr bool use_coro_v = awaitable_context<TContext>;
  static constexpr bool use_tryenqueue_v
    = dispatcher_queue<TContext> && !awaitable_context<TContext>;

  context_binder() = delete;
  template <std::convertible_to<TContext> InitContext, class InitFn>
  constexpr context_binder(InitFn&& fn, InitContext&& context)
    : mContext(std::forward<InitContext>(context)),
      mFn(std::forward<InitFn>(fn)) {
  }

  // Not using perfect forwarding for the arguments as this is an async
  // invocation/coroutine - we **MUST** copy the arguments.
  template <class... UnboundArgs>
    requires std::invocable<TFn, UnboundArgs...>
  void operator()(const UnboundArgs&... unboundArgs) const {
    if constexpr (use_coro_v) {
      dispatch_coro(mContext, mFn, unboundArgs...);
    } else if constexpr (use_tryenqueue_v) {
      mContext.TryEnqueue(std::bind_front(mFn, unboundArgs...));
    } else {
      // Should be unreachable due to `switchable_context` requirement
      static_assert(false, "Don't know how to invoke in supplied context");
    }
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
    co_await switch_context_awaitable(context);
    std::invoke(fn, unboundArgs...);
  }
};

}// namespace FredEmmott::cppwinrt_detail