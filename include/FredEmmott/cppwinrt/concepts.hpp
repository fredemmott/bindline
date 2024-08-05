// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <winrt/Windows.Foundation.h>

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {
template <class T>
concept cppwinrt_type = std::
  convertible_to<std::decay_t<T>, winrt::Windows::Foundation::IInspectable>;

template <class T>
concept cppwinrt_ptr = cppwinrt_type<std::decay_t<std::remove_pointer_t<T>>>;

template <class T>
concept cppwinrt_com_ptr
  = cppwinrt_ptr<std::decay_t<decltype(std::declval<T>().get())>>
  && std::same_as<
      std::decay_t<T>,
      winrt::com_ptr<std::decay_t<decltype(*std::declval<T>().get())>>>;

template <class T>
concept cppwinrt_strong_ref = cppwinrt_com_ptr<T>
  || (cppwinrt_type<T> && requires(T v) { winrt::make_weak(v); });

template <class T>
concept cppwinrt_weak_ref
  = (cppwinrt_strong_ref<decltype(std::declval<T>().get())>
     || cppwinrt_com_ptr<decltype(std::declval<T>().get())>)
  && std::same_as<
      std::decay_t<T>,
      std::decay_t<decltype(winrt::make_weak(std::declval<T>().get()))>>;

template <class T>
concept cppwinrt_raw_pointer = std::is_pointer_v<T> && requires(T v) {
  { v->get_weak() } -> cppwinrt_weak_ref;
};

}// namespace FredEmmott::cppwinrt::inline cppwinrt_concepts