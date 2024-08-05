// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <concepts>

namespace FredEmmott::weak_refs_extensions {

template <class T>
struct make_weak_ref_fn {
  /** Create a weak ref from the specified value.
   *
   *  We have the class defined but useless so that we:
   *  - have the ability to specialize this class
   *  - keep SFINAE for invocation failures
   *
   * Ideally this would be a `static operator()`, but that:
   * - requires C++23
   * - is not yet widely supported as of 2024-08-05
   *
   * Declaring this method as `delete` has no effect; it's only here to
   * document what specializations need to implement.
   *
   * @returns a `weak_ref`
   */
  static constexpr auto make(T&&) = delete;
};

template <class T>
struct lock_weak_ref_fn {
  static constexpr auto lock(T&&) = delete;
};

}// namespace FredEmmott::weak_refs_extensions