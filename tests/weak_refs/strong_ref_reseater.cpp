// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#include <FredEmmott/weak_refs.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::weak_refs;

TEST_CASE("no args") {
  { strong_ref_reseater unchecked; }
  SUCCEED("destructor did not throw");

  strong_ref_reseater checked;
  CHECK(checked.reseat());
}

TEST_CASE("single arg, single ref") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(123);

  { strong_ref_reseater unchecked(&a); };
  SUCCEED("destructor did not throw");
  CHECK(!a);

  strong_ref_reseater checked(&b);
  CHECK_FALSE(checked.reseat());
  CHECK(!b);
}

TEST_CASE("single arg, multiple ref") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(123);
  auto aa = a;
  auto bb = b;

  { strong_ref_reseater unchecked(&a); };
  SUCCEED("destructor did not throw");
  CHECK(a);
  CHECK(aa);

  strong_ref_reseater checked(&b);
  CHECK(checked.reseat());
  CHECK(b);
  CHECK(bb);

  CHECK(*a == *b);
  CHECK(*a == 123);
}

TEST_CASE("multi arg, single ref") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(123);
  auto c = std::make_shared<int>(123);
  auto d = std::make_shared<int>(123);

  { strong_ref_reseater unchecked(&a, &b); };
  SUCCEED("destructor did not throw");
  CHECK(!a);
  CHECK(!b);

  strong_ref_reseater checked(&c, &d);
  CHECK_FALSE(checked.reseat());
  CHECK(!c);
  CHECK(!d);
}

TEST_CASE("multi arg, multi ref") {
  auto a = std::make_shared<int>(123);
  auto b = std::make_shared<int>(123);
  auto c = std::make_shared<int>(123);
  auto d = std::make_shared<int>(123);
  auto aa = a;
  auto bb = b;
  auto cc = c;
  auto dd = d;

  { strong_ref_reseater unchecked(&a, &b); };
  SUCCEED("destructor did not throw");
  CHECK(a);
  CHECK(b);

  strong_ref_reseater checked(&c, &d);
  CHECK(checked.reseat());
  CHECK(c);
  CHECK(d);
}