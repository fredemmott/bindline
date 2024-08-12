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

// Just make sure the GET_WEAK definition took effect
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
};

struct TestNoWeakRef
  : winrt::implements<TestNoWeakRef, IStringable, winrt::no_weak_ref> {
  winrt::hstring ToString() const noexcept {
    return winrt::to_hstring("Hello, "s + __FILE__);
  }
};

#if defined(_MSVC_LANG) && !defined(__clang__)
#pragma warning(pop)
#endif

TEST_CASE("a WinRT implementation") {
  // make sure we're not accidentally testing the pointer path
  STATIC_CHECK_FALSE(convertible_to_weak_ref<TestClass*>);

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

TEST_CASE("a class that implements winrt::no_weak_ref") {
  STATIC_CHECK_FALSE(convertible_to_weak_ref<TestNoWeakRef>);

  // While C++/WinRT considers it a strong reference, it is a strong reference
  // that can not be converted to a weak reference. For the `weak_refs` library,
  // all strong references can be converted to weak references, so while it's a
  // winrt_strong_ref, it isn't a strong_ref :)
  STATIC_CHECK(FredEmmott::cppwinrt::winrt_strong_ref<TestNoWeakRef>);
  STATIC_CHECK_FALSE(strong_ref<TestNoWeakRef>);
}