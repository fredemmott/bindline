// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/base.h>

#include "detail/is_implements.hpp"

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