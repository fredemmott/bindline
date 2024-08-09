// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "composable_bind/config.hpp"

#include <functional>

#include "composable_bind/bind_front.hpp"
#include "composable_bind/bind_tap.hpp"
#include "composable_bind/bindable_t.hpp"

#if FREDEMMOTT_COMPOSABLE_BIND_ENABLE_BIND_BACK
#include "composable_bind/bind_back.hpp"
#endif

#if FREDEMMOTT_COMPOSABLE_BIND_ENABLE_WEAK_REFS
#include "composable_bind/weak_refs.hpp"
#endif

#if FREDEMMOTT_COMPOSABLE_BIND_ENABLE_CPPWINRT
#include "composable_bind/cppwinrt.hpp"
#endif

#if __has_include(<FredEmmott.composable_bind.footer-tweaks.hpp>)
#include <FredEmmott.composable_bind.footer-tweaks.hpp>
#endif