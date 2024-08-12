// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <winrt/Windows.Foundation.h>

#include <FredEmmott/cppwinrt.hpp>

#include <catch2/catch_test_macros.hpp>
#include <windows.foundation.h>

using namespace FredEmmott::cppwinrt;
using namespace winrt::Windows::Foundation;

// to disambiguate between ABI and winrt, e.g. IStringable
namespace winrt {
using namespace Windows::Foundation;
}
namespace ABI {
using namespace Windows::Foundation;
}

struct TestClass : winrt::implements<TestClass, IStringable> {
  virtual winrt::hstring ToString() const noexcept {
    return L"Hello, world";
  }
};

struct TestClassNoWeakRefLeadingMarker : winrt::implements<
                                           TestClassNoWeakRefLeadingMarker,
                                           winrt::no_weak_ref,
                                           winrt::IStringable> {
  virtual winrt::hstring ToString() const noexcept = 0;
};

struct TestClassNoWeakRefTrailingMarker : winrt::implements<
                                            TestClassNoWeakRefTrailingMarker,
                                            winrt::IStringable,
                                            winrt::no_weak_ref> {
  virtual winrt::hstring ToString() const noexcept = 0;
};

TEST_CASE("winrt_type<T>") {
  STATIC_CHECK(winrt_type<winrt::IInspectable>);
  STATIC_CHECK_FALSE(winrt_type<::IInspectable>);

  STATIC_CHECK(winrt_type<TestClass>);
  STATIC_CHECK_FALSE(winrt_type<int>);
  STATIC_CHECK_FALSE(winrt_type<std::shared_ptr<int>>);
  STATIC_CHECK_FALSE(winrt_type<::IUnknown*>);
  STATIC_CHECK_FALSE(winrt_type<winrt::com_ptr<::IUnknown>>);
  STATIC_CHECK_FALSE(
    winrt_type<decltype(winrt::make_weak(winrt::IInspectable {nullptr}))>);
}

TEST_CASE("winrt_interface") {
  STATIC_CHECK(winrt_interface<winrt::IStringable>);
  STATIC_CHECK_FALSE(winrt_interface<TestClass>);
  STATIC_CHECK_FALSE(winrt_interface<void*>);
}

TEST_CASE("winrt_implementation") {
  STATIC_CHECK_FALSE(winrt_implementation<winrt::IStringable>);
  STATIC_CHECK(winrt_implementation<TestClass>);
  STATIC_CHECK_FALSE(winrt_implementation<void*>);
}

TEST_CASE("winrt_implements") {
  STATIC_CHECK(winrt_implements<TestClass, winrt::IStringable>);
  STATIC_CHECK(
    winrt_implements<TestClassNoWeakRefLeadingMarker, winrt::IStringable>);

  STATIC_CHECK(
    winrt_implements<TestClassNoWeakRefLeadingMarker, winrt::no_weak_ref>);
  STATIC_CHECK(
    winrt_implements<TestClassNoWeakRefTrailingMarker, winrt::no_weak_ref>);
  STATIC_CHECK_FALSE(winrt_implements<TestClass, winrt::no_weak_ref>);
}

TEST_CASE("any_com_ptr<T>") {
  STATIC_CHECK(any_com_ptr<winrt::com_ptr<::IUnknown>>);
  STATIC_CHECK(any_com_ptr<winrt::com_ptr<TestClass>>);
}

TEST_CASE("winrt_com_ptr<T>") {
  SECTION("with a WinRT class") {
    auto rt = winrt::make<TestClass>();
    STATIC_CHECK_FALSE(winrt_com_ptr<decltype(rt)>);

    auto com = rt.as<TestClass>();
    STATIC_CHECK(std::same_as<decltype(com), winrt::com_ptr<TestClass>>);
    STATIC_CHECK(winrt_com_ptr<decltype(com)>);
    CHECK(com);
    CHECK(winrt::to_string(com->ToString()) == "Hello, world");

    STATIC_CHECK_FALSE(winrt_com_ptr<decltype(com.get())>);
  }

  SECTION("With a non-WinRT com_ptr") {
    STATIC_CHECK(any_com_ptr<winrt::com_ptr<::IUnknown>>);
    STATIC_CHECK_FALSE(winrt_com_ptr<winrt::com_ptr<::IUnknown>>);
  }

  SECTION("With a com_ptr created from a WinRT ABI type") {
    auto rt = winrt::make<TestClass>();
    STATIC_CHECK_FALSE(winrt_com_ptr<decltype(rt)>);
    winrt::com_ptr com {rt.as<ABI::IStringable>()};
    CHECK(com);
    STATIC_CHECK(any_com_ptr<decltype(com)>);
    STATIC_CHECK_FALSE(winrt_com_ptr<decltype(com)>);
  }
}

TEST_CASE("winrt_strong_ref<T>") {
  SECTION("With a WinRT object") {
    winrt::IInspectable it {nullptr};
    STATIC_CHECK(winrt_strong_ref<decltype(it)>);
    STATIC_CHECK(winrt_strong_ref<TestClass>);
    STATIC_CHECK(winrt_strong_ref<TestClassNoWeakRefLeadingMarker>);
    STATIC_CHECK(winrt_strong_ref<TestClassNoWeakRefTrailingMarker>);

    auto weak = winrt::make_weak(it);
    STATIC_CHECK_FALSE(winrt_strong_ref<decltype(weak)>);
    auto strong = weak.get();
    STATIC_CHECK(winrt_strong_ref<decltype(strong)>);
  }

  SECTION("Non-WinRT types") {
    STATIC_CHECK_FALSE(winrt_strong_ref<int>);
    STATIC_CHECK_FALSE(winrt_strong_ref<std::shared_ptr<int>>);
    STATIC_CHECK_FALSE(winrt_strong_ref<std::weak_ptr<int>>);
  }
}

TEST_CASE("winrt_weak_ref<T>") {
  SECTION("With a WinRT object") {
    winrt::IInspectable it {nullptr};
    STATIC_CHECK_FALSE(winrt_weak_ref<decltype(it)>);

    auto weak = winrt::make_weak(it);
    STATIC_CHECK(winrt_weak_ref<decltype(weak)>);
    STATIC_CHECK(
      std::same_as<winrt::weak_ref<winrt::IInspectable>, decltype(weak)>);

    auto strong = weak.get();
    STATIC_CHECK_FALSE(winrt_weak_ref<decltype(strong)>);
  }

  STATIC_CHECK_FALSE(winrt_weak_ref<winrt::IInspectable*>);

  SECTION("Non-WinRT types") {
    STATIC_CHECK_FALSE(winrt_weak_ref<std::weak_ptr<int>>);
    STATIC_CHECK_FALSE(winrt_weak_ref<int*>);
  }
}

TEST_CASE("raw_pointer<T>") {
  STATIC_CHECK(winrt_raw_pointer<winrt::IInspectable*>);
  STATIC_CHECK_FALSE(winrt_raw_pointer<winrt::IInspectable>);
}