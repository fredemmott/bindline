// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/base.h>

namespace FredEmmott::cppwinrt_detail {

/** This appears to require use of `winrt::impl` as:
 * - there aren't requirements/enable_if on the relevant functions, especially
 * for testing `no_weak_ref`; static assertions lead to compile failures even in
 * concept requirements.
 * - the types/values/... on the implements_type are private
 */
template <class T, class Marker>
consteval bool has_marker() {
  using markers_t
    = winrt::impl::filter<winrt::impl::is_marker, typename T::implements_type>;
  // Using has_interface_traits as a generic type-equality trait
  using equality_marker_t = typename winrt::impl::has_interface_traits<Marker>;
  constexpr equality_marker_t equality_marker {};
  return markers_t::find(equality_marker);
};
}// namespace FredEmmott::cppwinrt_detail

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {

template <class T>
concept winrt_type = std::
  convertible_to<std::decay_t<T>, winrt::Windows::Foundation::IInspectable>;

template <class T, class Marker>
concept marked_winrt_type
  = winrt_type<T> && ::FredEmmott::cppwinrt_detail::has_marker<T, Marker>();

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