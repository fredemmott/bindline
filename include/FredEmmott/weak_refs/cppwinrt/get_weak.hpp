// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../cppwinrt/concepts.hpp"
#include "../cppwinrt.hpp"

namespace FredEmmott::weak_refs_extensions {

using namespace FredEmmott::cppwinrt::cppwinrt_concepts;

template <class T>
concept has_winrt_get_weak
  = winrt_raw_pointer<T> && requires(std::decay_t<T> v) {
      {
        v->get_weak()
      }
      -> std::same_as<winrt::weak_ref<std::decay_t<std::remove_pointer_t<T>>>>;
      {
        std::declval<typename std::remove_pointer_t<T>::is_weak_ref_source>()
      } -> std::same_as<std::true_type>;
    };

template <winrt_raw_pointer T>
struct make_weak_ref_fn<T> {
  static constexpr auto make(auto&& value) {
    return value->get_weak();
  }
};

// FIXME: test winrt::com_ptr()
}// namespace FredEmmott::weak_refs_extensions