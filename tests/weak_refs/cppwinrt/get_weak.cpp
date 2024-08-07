// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <winrt/Windows.Foundation.h>

#include <FredEmmott/weak_refs.hpp>

#include <string>

#include <catch2/catch_test_macros.hpp>

using std::operator""s;

using namespace FredEmmott::weak_refs;
using namespace winrt::Windows::Foundation;

struct TestClass : winrt::implements<TestClass, IStringable> {
  winrt::hstring ToString() const noexcept {
    return winrt::to_hstring("Hello, "s + __FILE__);
  }

  void NotInWinRTInterface() {
  }
};

TEST_CASE("a WinRT implementation") {
  STATIC_CHECK(convertible_to_weak_ref<TestClass*>);
  STATIC_CHECK_FALSE(weak_ref<TestClass*>);
  STATIC_CHECK_FALSE(strong_ref<TestClass*>);

  auto rt = winrt::make<TestClass>();
  auto com = rt.as<TestClass>();
  auto raw = com.get();
  STATIC_REQUIRE(std::same_as<TestClass*, decltype(raw)>);

  auto weak = make_weak_ref(raw);
  STATIC_CHECK(convertible_to_weak_ref<decltype(weak)>);
  STATIC_CHECK(weak_ref<decltype(weak)>);
  STATIC_CHECK_FALSE(strong_ref<decltype(weak)>);

  auto strong = lock_weak_ref(weak);
  STATIC_CHECK(convertible_to_weak_ref<decltype(strong)>);
  STATIC_CHECK_FALSE(weak_ref<decltype(strong)>);
  STATIC_CHECK(strong_ref<decltype(strong)>);

  CHECK(winrt::to_string(strong->ToString()) == "Hello, "s + __FILE__);
  strong->NotInWinRTInterface();
}

struct TestNoWeakRef
  : winrt::implements<TestNoWeakRef, winrt::no_weak_ref, IStringable> {
  winrt::hstring ToString() const noexcept {
    return L"No weak refs!";
  }
};

TEST_CASE("A WinRT implementation with no_weak_ref") {
  STATIC_CHECK_FALSE(convertible_to_weak_ref<TestNoWeakRef*>);
  STATIC_CHECK_FALSE(weak_ref<TestNoWeakRef*>);
  STATIC_CHECK_FALSE(strong_ref<TestNoWeakRef*>);

  auto rt = winrt::make<TestNoWeakRef>();
  auto com = rt.as<TestNoWeakRef>();
  auto raw = com.get();
  STATIC_REQUIRE(std::same_as<TestNoWeakRef*, decltype(raw)>);
}