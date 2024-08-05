// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <winrt/base.h>

#include <catch2/catch_session.hpp>

int main(int argc, char* argv[]) {
  winrt::init_apartment();
  return Catch::Session().run(argc, argv);
}