// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

// It compiles, ship it ;)

#include <FredEmmott/weak_refs.hpp>
#include <FredEmmott/bind.hpp>

#if _WIN32
static_assert(FREDEMMOTT_CPPWINRT_ENABLE_WIL);
static_assert(FREDEMMOTT_BIND_ENABLE_CPPWINRT);
static_assert(FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT);
#else
// UNDEFINED: static_assert(FREDEMMOTT_CPPWINRT_ENABLE_WIL);
static_assert(!FREDEMMOTT_BIND_ENABLE_CPPWINRT);
static_assert(!FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT);
#endif

int main() {
    return 0;
}