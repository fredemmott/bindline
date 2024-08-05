// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#ifndef OPENKNEEBOARD_CPPWINRT_USE_WIL
#define OPENKNEEBOARD_CPPWINRT_USE_WIL (__has_include(<wil/cppwinrt_helpers.h>))
#endif

#include <winrt/base.h>

#if OPENKNEEBOARD_CPPWINRT_USE_WIL
#include <wil/cppwinrt_helpers.h>
#endif

#include <memory>

namespace FredEmmott::cppwinrt_detail {
#if OPENKNEEBOARD_CPPWINRT_USE_WIL
template <class T>
concept wil_thread = requires(T v) { wil::resume_foreground(v); };
#endif

template <class TContext, class TFn>
struct context_binder_inner
  : std::enable_shared_from_this<context_binder_inner<TContext, TFn>> {
 public:
  using function_t = TFn;

  context_binder_inner() = delete;
  template <class InitContext, class InitFn>
  context_binder_inner(InitContext&& context, InitFn&& fn)
    : mContext(std::forward<InitContext>(context)),
      mFn(std::forward<InitFn>(fn)) {
  }

  // Not using perfect forwarding for the arguments as this is a coroutine - so
  // we **MUST** copy the arguments.
  template <class... UnboundArgs>
    requires std::invocable<TFn, UnboundArgs...>
  winrt::fire_and_forget operator()(UnboundArgs... unboundArgs) const {
    auto weak = this->weak_from_this();
    auto ctx = mContext;
    co_await switch_context(ctx);
    if (auto self = weak.lock()) {
      std::invoke(mFn, unboundArgs...);
    }
  }

 private:
  static auto switch_context(const winrt::apartment_context& ctx) {
    return ctx;
  }
#if OPENKNEEBOARD_CPPWINRT_USE_WIL
  template <wil_thread Context>
  static auto switch_context(Context&& ctx) {
    return wil::resume_foreground(std::forward<Context>(ctx));
  }
#endif

  TContext mContext;
  TFn mFn;
};

template <class TContext, class TFn>
struct context_binder_outer {
  using function_t = TFn;

  context_binder_outer() = delete;
  template <class... Args>
  context_binder_outer(Args&&... args) {
    mInner = std::make_shared<context_binder_inner<TContext, TFn>>(
      std::forward<Args>(args)...);
  }

  template <class... UnboundArgs>
  void operator()(UnboundArgs&&... unboundArgs) const {
    std::invoke(*mInner, std::forward<UnboundArgs>(unboundArgs)...);
  }

 private:
  std::shared_ptr<context_binder_inner<TContext, TFn>> mInner;
};

}// namespace FredEmmott::cppwinrt_detail

namespace FredEmmott::cppwinrt {

template <class Context, class F>
auto bind_context(Context&& context, F&& f) {
  return cppwinrt_detail::
    context_binder_outer<std::decay_t<Context>, std::decay_t<F>> {
      std::forward<Context>(context), std::forward<F>(f)};
}

}// namespace FredEmmott::cppwinrt