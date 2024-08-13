// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using namespace FredEmmott::bindline;
using namespace FredEmmott::bindline_extension_api;
using namespace Catch::Matchers;
using enum ordering_requirements_t;

template <ordering_requirements_t Order>
struct constrained_t : bindable_t {
  static constexpr auto ordering_requirements_v = Order;

  constexpr auto bind_to(auto&& f) const {
    return std::forward<decltype(f)>(f);
  }
};

struct before_context_switch_t : constrained_t<invoke_before_context_switch> {};
struct context_switch_t : constrained_t<is_context_switch> {};
struct after_context_switch_t : constrained_t<invoke_after_context_switch> {};

TEST_CASE("invocable with correct ordering") {
  size_t last_line = 0;
  auto f = [&](auto line) {
    last_line = static_cast<size_t>(line);
    return true;
  };

  CHECK((f | after_context_switch_t {} | context_switch_t {})(__LINE__));
  CHECK(last_line == __LINE__ - 1);
  CHECK((f | context_switch_t {} | before_context_switch_t {})(__LINE__));
  CHECK(last_line == __LINE__ - 1);
  CHECK(
    (f | after_context_switch_t {} | context_switch_t {}
     | before_context_switch_t {})(__LINE__));
  CHECK(last_line == __LINE__ - 1);

  CHECK(
    (f | after_context_switch_t {} | after_context_switch_t {}
     | context_switch_t {} | before_context_switch_t {}
     | before_context_switch_t {})(__LINE__));
}

TEST_CASE("trace") {
  auto trace = ([]() {} | after_context_switch_t {} | context_switch_t {}
                | before_context_switch_t {})
                 .trace_as_string();
  CHECK_THAT(
    trace,
    Matches(".*after_context_switch_t.*[^a-z_]context_switch_t.*before_context_"
            "switch_t.*"));
}

template <class... TArgs>
concept can_concat_pipelines
  = requires { ([]() {} | ... | std::declval<TArgs>()); };

TEST_CASE("failing validation") {
  // Let's check the concept actually works first :)
  STATIC_CHECK(can_concat_pipelines<after_context_switch_t, context_switch_t>);
  STATIC_CHECK_FALSE(
    can_concat_pipelines<context_switch_t, after_context_switch_t>);
  STATIC_CHECK_FALSE(
    can_concat_pipelines<before_context_switch_t, context_switch_t>);
}

TEST_CASE("suppression") {
  auto f = [&](int a, int b) { return a + b; };
  STATIC_CHECK_FALSE(
    can_concat_pipelines<before_context_switch_t, context_switch_t>);

  CHECK(
    (f | bind_suppress_validation(before_context_switch_t {})
     | context_switch_t {})(1, 2)
    == 3);
}