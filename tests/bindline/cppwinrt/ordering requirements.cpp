// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::bindline;
using namespace FredEmmott::bindline_extension_api;
using enum ordering_requirements_t;

template <auto Order>
struct constrained_t : bindable_t {
  static constexpr auto ordering_requirements_v = Order;

  constexpr auto bind_to(auto&& f) const {
    return std::forward<decltype(f)>(f);
  }
};

struct before_context_switch_t : constrained_t<invoke_before_context_switch> {};
struct context_switch_t : constrained_t<is_context_switch> {};
struct after_context_switch_t : constrained_t<invoke_after_context_switch> {};

template <class... TArgs>
concept can_concat_pipelines
  = requires { ([]() {} | ... | std::declval<TArgs>()); };

TEST_CASE("bind_winrt_context()") {
  using TContext = decltype(bind_winrt_context(winrt::apartment_context {}));
  STATIC_CHECK(can_concat_pipelines<
               after_context_switch_t,
               TContext,
               before_context_switch_t>);
  STATIC_CHECK_FALSE(can_concat_pipelines<before_context_switch_t, TContext>);
  STATIC_CHECK_FALSE(can_concat_pipelines<TContext, after_context_switch_t>);
}

TEST_CASE("bind_winrt_context() combined with weak_refs") {
  auto shared = std::make_shared<int>(123);
  auto strong = std::make_shared<int>(123);
  winrt::apartment_context ctx {};
  STATIC_CHECK(can_concat_pipelines<
               decltype(bind_refs_front(shared)),
               decltype(bind_winrt_context(ctx))>);
  STATIC_CHECK_FALSE(can_concat_pipelines<
                     decltype(bind_winrt_context(ctx)),
                     decltype(bind_refs_front(shared))>);
  STATIC_CHECK(can_concat_pipelines<
               decltype(bind_suppress_validation(bind_refs_front(shared))),
               decltype(bind_winrt_context(ctx))>);
}