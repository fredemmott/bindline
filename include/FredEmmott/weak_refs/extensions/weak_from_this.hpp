// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <concepts>
#include <memory>

#include "../concepts.hpp"
#include "../extensions.hpp"

namespace FredEmmott::weak_refs_extensions {

template <class T>
concept with_weak_from_this = std::is_pointer_v<T> && requires(T x) {
  { x->weak_from_this() } -> ::FredEmmott::weak_refs::weak_ref;
};

static_assert(!with_weak_from_this<std::shared_ptr<int>>);

template <with_weak_from_this T>
struct make_weak_ref_fn<T> {
  [[nodiscard]]
  static constexpr auto make(auto&& value) {
    return value->weak_from_this();
  }
};

}// namespace FredEmmott::weak_refs_extensions