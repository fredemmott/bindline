// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

template <class... Args>
auto bind_function_under_test(Args&&... args) {
  return bind_maybe_refs_front(std::forward<Args>(args)...);
}

#include "common/bind_refs.hpp"

TEST_CASE("with a mix of refs and not-refs") {
  auto ref = std::make_shared<int>(123);
  auto not_ref = std::make_unique<int>(456);

  bool invoked = false;
  bind_maybe_refs_front(
    [&invoked](auto ref, auto not_ref) {
      STATIC_CHECK(std::same_as<decltype(ref), std::shared_ptr<int>>);
      STATIC_CHECK(std::same_as<decltype(not_ref), int*>);
      CHECK(ref);
      CHECK(*ref == 123);
      CHECK(*not_ref == 456);
      invoked = true;
    },
    ref,
    not_ref.get())();
  CHECK(invoked);
}