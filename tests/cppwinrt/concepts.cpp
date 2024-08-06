// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <FredEmmott/cppwinrt.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::cppwinrt;

TEST_CASE("cppwinrt_type<T>") {
  STATIC_CHECK(cppwinrt_type<winrt::Windows::Foundation::IInspectable>);
  STATIC_CHECK_FALSE(cppwinrt_type<int>);
  STATIC_CHECK_FALSE(cppwinrt_type<std::shared_ptr<int>>);
  STATIC_CHECK_FALSE(cppwinrt_type<IUnknown*>);
  STATIC_CHECK_FALSE(cppwinrt_type<winrt::com_ptr<IUnknown>>);
  STATIC_CHECK_FALSE(cppwinrt_type<decltype(winrt::make_weak(
                       winrt::Windows::Foundation::IInspectable {nullptr}))>);
}

TEST_CASE("cppwinrt_ptr<T>", "[!shouldfail]") {
  STATIC_CHECK(cppwinrt_ptr<winrt::Windows::Foundation::IInspectable*>);
  // Add more tests before fixing this :)
  CHECK_FALSE(cppwinrt_ptr<winrt::Windows::Foundation::IInspectable>);
  SKIP("Known bug: #11");
  // STATIC_CHECK_FALSE(cppwinrt_ptr<winrt::Windows::Foundation::IInspectable>);
}

TEST_CASE("com_ptr<T>") {
  SKIP("Untested - TODO");
}

TEST_CASE("cppwinrt_strong_ref<T>") {
  SECTION("With a WinRT object") {
    winrt::Windows::Foundation::IInspectable it {nullptr};
    STATIC_CHECK(cppwinrt_strong_ref<decltype(it)>);

    auto weak = winrt::make_weak(it);
    STATIC_CHECK_FALSE(cppwinrt_strong_ref<decltype(weak)>);
    auto strong = weak.get();
    STATIC_CHECK(cppwinrt_strong_ref<decltype(strong)>);
  }

  SECTION("Non-WinRT types") {
    STATIC_CHECK_FALSE(cppwinrt_strong_ref<int>);
    STATIC_CHECK_FALSE(cppwinrt_strong_ref<std::shared_ptr<int>>);
    STATIC_CHECK_FALSE(cppwinrt_strong_ref<std::weak_ptr<int>>);
  }
}

TEST_CASE("cppwinrt_weak_ref<T>") {
  SECTION("With a WinRT object") {
    winrt::Windows::Foundation::IInspectable it {nullptr};
    STATIC_CHECK_FALSE(cppwinrt_weak_ref<decltype(it)>);

    auto weak = winrt::make_weak(it);
    STATIC_CHECK(cppwinrt_weak_ref<decltype(weak)>);
    STATIC_CHECK(std::same_as<
                 winrt::weak_ref<winrt::Windows::Foundation::IInspectable>,
                 decltype(weak)>);

    auto strong = weak.get();
    STATIC_CHECK_FALSE(cppwinrt_weak_ref<decltype(strong)>);
  }

  STATIC_CHECK_FALSE(
    cppwinrt_weak_ref<winrt::Windows::Foundation::IInspectable*>);

  SECTION("Non-WinRT types") {
    STATIC_CHECK_FALSE(cppwinrt_weak_ref<std::weak_ptr<int>>);
    STATIC_CHECK_FALSE(cppwinrt_weak_ref<int*>);
  }
}

TEST_CASE("raw_pointer<T>") {
  SKIP("Untested - TODO");
}