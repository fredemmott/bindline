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
struct context_binder_inner
  : std::enable_shared_from_this<context_binder_inner<TContext, TFn>> {
 public:
  using function_t = TFn;

  context_binder_inner() = delete;
  template <std::convertible_to<TContext> InitContext, class InitFn>
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
  TContext mContext;
  TFn mFn;
};

template <switchable_context TContext, class TFn>
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

template <cppwinrt_detail::switchable_context Context, class F>
auto bind_context(Context&& context, F&& f) {
  return cppwinrt_detail::
    context_binder_outer<std::decay_t<Context>, std::decay_t<F>> {
      std::forward<Context>(context), std::forward<F>(f)};
}

}// namespace FredEmmott::cppwinrt