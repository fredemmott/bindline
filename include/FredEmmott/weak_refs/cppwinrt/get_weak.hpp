// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../../cppwinrt/concepts.hpp"
#include "../cppwinrt.hpp"

namespace FredEmmott::weak_refs_extensions {

using namespace FredEmmott::cppwinrt::cppwinrt_concepts;

template <winrt_raw_pointer T>
  requires(!winrt_implements<std::remove_pointer<T>, winrt::no_weak_ref>)
struct make_weak_ref_fn<T> {
  static constexpr auto make(auto&& value) {
    return value->get_weak();
  }
};

// FIXME: test winrt::com_ptr()
}// namespace FredEmmott::weak_refs_extensions