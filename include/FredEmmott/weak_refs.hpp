// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "weak_refs/config.hpp"

/*
 * Helpers for writing templates that deal with both
 * std::weak_ptr and winrt::weak_ref.
 *
 * # Concepts
 *
 * See `weak_refs/concepts.hpp` for details:
 *
 * - `strong_ref<T>`: e.g. `std::shared_ptr<T>`
 * - `weak_ref<T>`: e.g. `std::weak_ptr<T>`
 * - `convertible_to_weak_ref<T>`: either a `weak_ref<T>`, or something else
 *   that can be converted to one, e.g. a `strong_ref<T>`, or `this` when
 *   extending `std::enable_shared_from_this<T>`
 *
 * # Types
 *
 * - strong_ref_t<TWeak>: the corresponding strong type
 * - weak_ref_t<TStrong>: the corresponding weak type
 *
 * # Functions
 *
 * - `make_weak_ref()`: creates a `weak_ref`
 * - `lock_weak_ref()`: upgrades a `weak_ref` to a `strong_ref`
 * - `make_weak_refs()`: creates a tuple of `weak_ref`
 * - `lock_weak_refs()`: creates an std::optional of strong references
 * - `bind_maybe_refs_front()`: like `std::bind_front()`, but automatically
 *    converts `convertible_to_weak_ref()` parameters to weak, then back to
 *    strong
 * - `bind_front(maybe_refs, ...)`: tag-based ADL alias for
 *   `bind_maybe_refs_front()`
 */

#include "weak_refs/base.hpp"
#include "weak_refs/bind_maybe_refs_front.hpp"
#include "weak_refs/bind_refs_front.hpp"

#if FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT
#include "weak_refs/cppwinrt.hpp"
#if FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT_GET_WEAK
#include "weak_refs/cppwinrt/get_weak.hpp"
#endif
#endif

#if FREDEMMOTT_WEAK_REFS_ENABLE_STD_SHARED_PTR
#include "weak_refs/std_shared_ptr.hpp"
#endif
#if FREDEMMOTT_WEAK_REFS_ENABLE_WEAK_FROM_THIS
#include "weak_refs/weak_from_this.hpp"
#endif

#if 0
// FIXME: move to test
// Just for testing:
#include "bind.hpp"

namespace TestNS {
namespace {

struct TestBindFront : public std::enable_shared_from_this<TestBindFront> {
  void byval(int, std::shared_ptr<int>);
  void bycr(int, const std::shared_ptr<int>&);
};

void takes_convertible_callback(
  std::convertible_to<std::function<void(int, std::shared_ptr<int>)>> auto);
void takes_convertible_callback_extra_arg(
  std::convertible_to<
    std::function<void(int, std::shared_ptr<int>, int)>> auto);
void takes_regular_invocable_callback(
  std::regular_invocable<int, std::shared_ptr<int>> auto);
void takes_regular_invocable_callback_extra_arg(
  std::regular_invocable<int, std::shared_ptr<int>, int> auto);

void test_bind_front(TestBindFront* t, std::weak_ptr<int> weakInt) {
  namespace weak_refs = FredEmmott::weak_refs;
  using namespace FredEmmott::bind;
  using namespace weak_refs;

  auto std_byval = std::bind_front(&TestBindFront::byval, t);
  auto byval = weak_refs::bind_maybe_refs_front(&TestBindFront::byval, t);
  auto bycr = weak_refs::bind_maybe_refs_front(&TestBindFront::bycr, t);

  static_assert(std::invocable<decltype(std_byval), int, std::shared_ptr<int>>);
  static_assert(std::invocable<decltype(byval), int, std::shared_ptr<int>>);
  static_assert(std::invocable<decltype(bycr), int, std::shared_ptr<int>>);

  // Test ADL
  static_assert(
    std::same_as<
      decltype(weak_refs::bind_maybe_refs_front(&TestBindFront::byval, t)),
      decltype(bind_front(&TestBindFront::byval, maybe_refs, t))>);
  static_assert(
    !std::same_as<
      decltype(weak_refs::bind_maybe_refs_front(&TestBindFront::byval, t)),
      decltype(std::bind_front(&TestBindFront::byval, maybe_refs, t))>);
  static_assert(std::same_as<
                decltype(std::bind_front(&TestBindFront::byval, t)),
                decltype(bind_front(&TestBindFront::byval, t))>);

  takes_convertible_callback(bind_front(&TestBindFront::byval, t));
  takes_convertible_callback(bind_front(&TestBindFront::bycr, t));

  takes_convertible_callback(bind_front(&TestBindFront::byval, maybe_refs, t));
  takes_convertible_callback(bind_front(&TestBindFront::bycr, maybe_refs, t));

  takes_regular_invocable_callback(bind_front(&TestBindFront::byval, t));
  takes_regular_invocable_callback(bind_front(&TestBindFront::bycr, t));

  takes_regular_invocable_callback(
    bind_front(&TestBindFront::byval, maybe_refs, t));
  takes_regular_invocable_callback(
    bind_front(&TestBindFront::bycr, maybe_refs, t));

  static_assert(not std::invocable<decltype(std_byval), int>);
  static_assert(std::invocable<
                decltype(byval)::function_t,
                decltype(t),
                int,
                std::shared_ptr<int>>);
  static_assert(
    not std::invocable<decltype(byval)::function_t, decltype(t), int>);
  static_assert(not std::invocable<decltype(byval), int>);
}
}// namespace
}// namespace TestNS
#endif