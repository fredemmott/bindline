// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "../bind/bind_front.hpp"
#include "../weak_refs/bind_maybe_refs_front.hpp"
#include "../weak_refs/bind_refs_front.hpp"

namespace FredEmmott::bind_detail {

// Markers for tag-based dispatch
struct maybe_refs_t {};
struct only_refs_t {};

template <class Fn, class... Args>
constexpr auto adl_bind_front(maybe_refs_t, Fn&& fn, Args&&... args) {
  return weak_refs::bind_maybe_refs_front(
    std::forward<Fn>(fn), std::forward<Args>(args)...);
}

template <class Fn, class... Args>
constexpr auto adl_bind_front(only_refs_t, Fn&& fn, Args&&... args) {
  return weak_refs::bind_refs_front(
    std::forward<Fn>(fn), std::forward<Args>(args)...);
}
}// namespace FredEmmott::bind_detail

namespace FredEmmott::bind::inline weak_refs_bind_tags {

// Markers for tag-based dispatch
constexpr bind_detail::maybe_refs_t maybe_refs {};
constexpr bind_detail::only_refs_t only_refs {};

}// namespace FredEmmott::bind