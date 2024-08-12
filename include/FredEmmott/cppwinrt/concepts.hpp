// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/base.h>

#include "concepts/winrt_implementation.hpp"
#include "concepts/winrt_interface.hpp"
#include "concepts/winrt_type.hpp"
#include "detail/is_implements.hpp"

namespace FredEmmott::cppwinrt::inline cppwinrt_concepts {

/** A class that extends `winrt::implements<... Interface, ...>`
 *
 * For example:
 * - `winrt_implements<MyClass, winrt::IStringable>`
 * - `winrt_implements<MyClass, winrt::no_weak_ref>`
 */
template <class T, class Interface>
concept winrt_implements = winrt_type<T>
  && ::FredEmmott::cppwinrt_detail::is_implements_v<T, Interface>;

/** A raw pointer to a `winrt_type` */
template <class T>
concept winrt_raw_pointer
  = std::is_pointer_v<T> && winrt_type<std::remove_pointer_t<T>>;

/** Any `winrt::com_ptr`.
 *
 * Use `winrt_com_ptr<T>` instead if you want a `winrt::com_ptr<>` to a
 * `winrt_type`.
 */
template <class T>
concept any_com_ptr = std::same_as<
  std::decay_t<T>,
  winrt::com_ptr<std::decay_t<decltype(*std::declval<T>().get())>>>;

/** A `winrt::com_ptr<>` to a `winrt_type`.
 *
 * This is mostly used when converting implementation types to and from weak
 * references.
 */
template <class T>
concept winrt_com_ptr
  = winrt_raw_pointer<std::decay_t<decltype(std::declval<T>().get())>>
  && any_com_ptr<T>;

/// An owning reference to a `winrt_type`.
template <class T>
concept winrt_strong_ref = winrt_com_ptr<T> || winrt_type<T>;

/** A non-owning reference to a `winrt_type`.
 *
 * A non-owning WinRT reference can be converted to an owning reference
 * (`winrt_strong_ref`) by calling `get()`; the result may be null.
 */
template <class T>
concept winrt_weak_ref
  = (winrt_strong_ref<decltype(std::declval<T>().get())>
     || winrt_com_ptr<decltype(std::declval<T>().get())>)
  && std::same_as<
      std::decay_t<T>,
      std::decay_t<decltype(winrt::make_weak(std::declval<T>().get()))>>;

}// namespace FredEmmott::cppwinrt::inline cppwinrt_concepts