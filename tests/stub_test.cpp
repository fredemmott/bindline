// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// It compiles, ship it ;)

#include <FredEmmott/weak_refs.hpp>
#include <FredEmmott/bind.hpp>

static_assert(FREDEMMOTT_BIND_ENABLE_CPPWINRT);
static_assert(FREDEMMOTT_CPPWINRT_ENABLE_WIL);
static_assert(FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT);

int main() {
    return 0;
}