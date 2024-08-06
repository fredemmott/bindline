// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <memory>

#include "base.hpp"

namespace FredEmmott::weak_refs_extensions {
namespace weak_refs = ::FredEmmott::weak_refs;

template <class T>
concept with_weak_from_this = std::is_pointer_v<T> && requires(T x) {
  { x->weak_from_this() } -> weak_refs::weak_ref;
};

static_assert(!with_weak_from_this<std::shared_ptr<int>>);

template <with_weak_from_this T>
struct make_weak_ref_fn<T> {
  static constexpr auto make(auto&& value) {
    return value->weak_from_this();
  }
};

}// namespace FredEmmott::weak_refs_extensions

#if 0
//FIXME TODO: split to test file

#include "std_shared_ptr.hpp"

namespace FredEmmott::weak_refs_test {
using namespace FredEmmott::weak_refs;
struct TestWeakFromThis
  : public std::enable_shared_from_this<TestWeakFromThis> {};

static_assert(
  std::same_as<weak_ref_t<TestWeakFromThis*>, std::weak_ptr<TestWeakFromThis>>);

static_assert(convertible_to_weak_ref<TestWeakFromThis*>);
static_assert(!strong_ref<TestWeakFromThis*>);

}// namespace FredEmmott::weak_refs_test
#endif
