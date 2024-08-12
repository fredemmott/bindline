// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <winrt/Windows.Foundation.h>

#include <FredEmmott/weak_refs.hpp>

#include <string>

#include <catch2/catch_test_macros.hpp>

using std::operator""s;

using namespace FredEmmott::weak_refs;
using namespace winrt::Windows::Foundation;

#if defined(_MSVC_LANG) && !defined(__clang__)
// These are warnings about various constructors and assignment operators being
// implicitly defined as deleted, which are C++/WinRT's responsibility
#pragma warning(push)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 5026)
#pragma warning(disable : 5027)
#endif

struct TestClass : winrt::implements<TestClass, IStringable> {
  winrt::hstring ToString() const noexcept {
    return winrt::to_hstring("Hello, "s + __FILE__);
  }

  void NotInWinRTInterface() {
  }
};

struct TestNoWeakRef
  : winrt::implements<TestNoWeakRef, winrt::no_weak_ref, IStringable> {
  winrt::hstring ToString() const noexcept {
    return L"No weak refs!";
  }
};

#if defined(_MSVC_LANG) && !defined(__clang__)
#pragma warning(pop)
#endif

TEST_CASE("a WinRT implementation") {
  STATIC_CHECK(convertible_to_weak_ref<TestClass*>);
  STATIC_CHECK_FALSE(weak_ref<TestClass*>);
  STATIC_CHECK_FALSE(strong_ref<TestClass*>);

  auto rt = winrt::make<TestClass>();
  auto com = rt.as<TestClass>();
  CHECK(com);
  CHECK(winrt::to_string(com->ToString()) == "Hello, "s + __FILE__);
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

TEST_CASE("A WinRT implementation with no_weak_ref") {
  STATIC_CHECK_FALSE(convertible_to_weak_ref<TestNoWeakRef*>);
  STATIC_CHECK_FALSE(weak_ref<TestNoWeakRef*>);
  STATIC_CHECK_FALSE(strong_ref<TestNoWeakRef*>);

  auto rt = winrt::make<TestNoWeakRef>();
  auto com = rt.as<TestNoWeakRef>();
  auto raw = com.get();
  STATIC_REQUIRE(std::same_as<TestNoWeakRef*, decltype(raw)>);
}