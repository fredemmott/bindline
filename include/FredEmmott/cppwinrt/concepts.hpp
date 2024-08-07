// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/base.h>

namespace FredEmmott::cppwinrt_detail {

template <class T, class Find, class...>
struct is_implements_t;

template <class T, class Find, class First, class... Rest>
struct is_implements_t<T, Find, winrt::implements<First, Rest...>> {
  static consteval bool test() {
    if constexpr (std::same_as<Find, First>) {
      return true;
    }

    if constexpr (sizeof...(Rest) == 0) {
      return false;
    } else {
      return
        typename is_implements_t<T, Find, winrt::implements<Rest...>>::test();
    }
  }
};

template <class T, class Find>
constexpr bool is_implements_v
  = is_implements_t<T, Find, typename T::implements_type>::test();

}// namespace FredEmmott::cppwinrt_detail

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {

template <class T>
concept winrt_type = std::
  convertible_to<std::decay_t<T>, winrt::Windows::Foundation::IInspectable>;

// This uses the `cppwinrt_` prefix instead of `winrt_` as it does not require
// that `T` is a Windows Runtime tpye.
template <class T, class Marker>
concept cppwinrt_implements_type
  = ::FredEmmott::cppwinrt_detail::is_implements_v<T, Marker>;

template <class T, class Marker>
concept winrt_implements_type
  = winrt_type<T> && cppwinrt_implements_type<T, Marker>;

template <class T>
concept winrt_raw_pointer
  = std::is_pointer_v<T> && winrt_type<std::remove_pointer_t<T>>;

template <class T>
concept any_com_ptr = std::same_as<
  std::decay_t<T>,
  winrt::com_ptr<std::decay_t<decltype(*std::declval<T>().get())>>>;

template <class T>
concept winrt_com_ptr
  = winrt_raw_pointer<std::decay_t<decltype(std::declval<T>().get())>>
  && any_com_ptr<T>;

template <class T>
concept winrt_strong_ref = winrt_com_ptr<T>
  || (winrt_type<T> && requires(T v) { winrt::make_weak(v); });

template <class T>
concept winrt_weak_ref
  = (winrt_strong_ref<decltype(std::declval<T>().get())>
     || winrt_com_ptr<decltype(std::declval<T>().get())>)
  && std::same_as<
      std::decay_t<T>,
      std::decay_t<decltype(winrt::make_weak(std::declval<T>().get()))>>;

}// namespace FredEmmott::cppwinrt::inline cppwinrt_concepts