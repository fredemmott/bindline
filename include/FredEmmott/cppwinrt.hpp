// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "cppwinrt/config.hpp"

#include "cppwinrt/bind_context.hpp"
#include "cppwinrt/concepts.hpp"

#if FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT_GET_WEAK
#include "weak_refs/cppwinrt/get_weak.hpp"
#endif

#if __has_include(<FredEmmott.cppwinrt.footer-tweaks.hpp>)
#include <FredEmmott.cppwinrt.footer-tweaks.hpp>
#endif