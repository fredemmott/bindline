// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#define FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT_GET_WEAK false

#include <winrt/Windows.Foundation.h>

#include <FredEmmott/weak_refs.hpp>

#include <string>

#include <catch2/catch_test_macros.hpp>

using std::operator""s;

using namespace FredEmmott::weak_refs;
using namespace winrt::Windows::Foundation;

TEST_CASE("a WinRT interface") {
  STATIC_CHECK(strong_ref<IStringable>);
  STATIC_CHECK_FALSE(weak_ref<IStringable>);
  STATIC_CHECK(convertible_to_weak_ref<IStringable>);

  IStringable it {nullptr};
  STATIC_CHECK(strong_ref<decltype(it)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(it)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(it)>);

  auto weak = make_weak_ref(it);
  STATIC_CHECK_FALSE(strong_ref<decltype(weak)>);
  STATIC_CHECK(weak_ref<decltype(weak)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(weak)>);

  auto strong = lock_weak_ref(weak);
  STATIC_CHECK(strong_ref<decltype(strong)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(strong)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(strong)>);
}

struct TestClass : winrt::implements<TestClass, IStringable> {
  winrt::hstring ToString() const noexcept {
    return winrt::to_hstring("Hello, "s + __FILE__);
  }
};

TEST_CASE("a WinRT implementation") {
  auto it = winrt::make<TestClass>();
  STATIC_CHECK(strong_ref<decltype(it)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(it)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(it)>);

  auto weak = make_weak_ref(it);
  STATIC_CHECK_FALSE(strong_ref<decltype(weak)>);
  STATIC_CHECK(weak_ref<decltype(weak)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(weak)>);

  auto strong = lock_weak_ref(weak);
  STATIC_CHECK(strong_ref<decltype(strong)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(strong)>);
  STATIC_CHECK(convertible_to_weak_ref<decltype(strong)>);

  CHECK(winrt::to_string(strong.ToString()) == "Hello, "s + __FILE__);
}