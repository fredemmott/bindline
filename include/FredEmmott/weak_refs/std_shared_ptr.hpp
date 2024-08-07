// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <memory>

#include "base.hpp"

namespace FredEmmott::weak_refs_extensions {
template <class T>
concept std_shared_ptr = std::same_as<
  std::decay_t<T>,
  std::shared_ptr<std::decay_t<typename T::element_type>>>;

template <class T>
concept std_weak_ptr = std::same_as<
  std::decay_t<T>,
  std::weak_ptr<std::decay_t<typename T::element_type>>>;

template <std_shared_ptr T>
struct make_weak_ref_fn<T> {
  template <class TValue>
    requires std::same_as<std::decay_t<T>, std::decay_t<TValue>>
  static constexpr auto make(TValue&& value) {
    return std::weak_ptr<std::decay_t<typename T::element_type>> {
      std::forward<TValue>(value)};
  }
};

template <std_weak_ptr T>
struct lock_weak_ref_fn<T> {
  static constexpr auto lock(auto&& value) {
    return value.lock();
  }
};
}// namespace FredEmmott::weak_refs_extensions