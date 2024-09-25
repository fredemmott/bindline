// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <FredEmmott/weak_refs/extensions.hpp>

namespace FredEmmott::weak_refs_adl {

template <class T>
[[nodiscard]] decltype(make_weak_ref(std::declval<T>())) adl_make_weak_ref(
  T&& v) {
  return make_weak_ref(std::forward<T>(v));
}

template <class T>
[[nodiscard]] decltype(lock_weak_ref(std::declval<T>())) adl_lock_weak_ref(
  T&& v) {
  return lock_weak_ref(std::forward<T>(v));
}

}// namespace FredEmmott::weak_refs_adl

namespace FredEmmott::weak_refs_extensions {

template<class T>
concept adl_weak_ref = requires (T v)
{
  FredEmmott::weak_refs_adl::adl_lock_weak_ref(v);
};

template<class T>
concept adl_strong_ref = requires (T v)
{
  FredEmmott::weak_refs_adl::adl_make_weak_ref(v);
};

template <adl_strong_ref T>
struct make_weak_ref_fn<T> {
  template <class TValue>
    requires std::same_as<std::decay_t<T>, std::decay_t<TValue>>
  [[nodiscard]] static constexpr auto make(TValue&& value) {
    return ::FredEmmott::weak_refs_adl::adl_make_weak_ref(value);
  }
};

template <adl_weak_ref T>
struct lock_weak_ref_fn<T> {
  [[nodiscard]] static constexpr auto lock(auto&& value) {
    return ::FredEmmott::weak_refs_adl::adl_lock_weak_ref(value);
  }
};
}// namespace FredEmmott::weak_refs_extensions