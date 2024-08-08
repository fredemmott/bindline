// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "weak_refs/config.hpp"

#include "weak_refs/bind_refs_back.hpp"
#include "weak_refs/bind_refs_front.hpp"
#include "weak_refs/concepts.hpp"

#if FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT
#include "weak_refs/cppwinrt.hpp"
#endif

#if FREDEMMOTT_WEAK_REFS_ENABLE_STD_SHARED_PTR
#include "weak_refs/std_shared_ptr.hpp"
#endif

#if FREDEMMOTT_WEAK_REFS_ENABLE_WEAK_FROM_THIS
#include "weak_refs/weak_from_this.hpp"
#endif