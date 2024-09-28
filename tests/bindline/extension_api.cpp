// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline.hpp>

#include <functional>
#include <catch2/catch_test_macros.hpp>

namespace TestNS {
template <int N>
class int_front_binder_t : public ::FredEmmott::bindline_extension_api::bindable_t {
public:
  template<class TFn>
  [[nodiscard]] constexpr auto bind_to(TFn&& fn) const {
    return std::bind_front(fn, N);
  }
};

template<int N>
auto bind_int_front() {
  return int_front_binder_t<N> {};
}

template<int N, class TFn>
auto bind_int_front(TFn&& fn) {
  return std::forward<TFn>(fn) | bind_int_front<N>();
}

}// namespace TestNS

TEST_CASE("No pipeline") {
  using namespace TestNS;

  size_t invokeCount = 0;
  int value = 0;
  auto fn = [&](const int v) { ++invokeCount; value = v; };
  auto bound = bind_int_front<123>(fn);
  REQUIRE(invokeCount == 0);
  REQUIRE(value == 0);
  bound();
  REQUIRE(invokeCount == 1);
  REQUIRE(value == 123);
}

TEST_CASE("Pipeline") {
  using namespace TestNS;

  size_t invokeCount = 0;
  int value = 0;
  auto fn = [&](const int v) { ++invokeCount; value = v; };
  auto bound = fn | bind_int_front<123>();
  REQUIRE(invokeCount == 0);
  REQUIRE(value == 0);
  bound();
  REQUIRE(invokeCount == 1);
  REQUIRE(value == 123);
}
