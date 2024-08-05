// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// Must be included before winrt/base.h
#include <Unknwn.h>

#include <FredEmmott/cppwinrt.hpp>

static_assert(FREDEMMOTT_CPPWINRT_ENABLE_WIL);

#include <catch2/catch_test_macros.hpp>

using namespace FredEmmott::cppwinrt;