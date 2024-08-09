// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bind/config.hpp"

#include <functional>

#include "bind/bind_front.hpp"
#include "bind/bind_tap.hpp"
#include "bind/bindable_t.hpp"

#if FREDEMMOTT_BIND_ENABLE_BIND_BACK
#include "bind/bind_back.hpp"
#endif

#if FREDEMMOTT_BIND_ENABLE_WEAK_REFS
#include "bind/weak_refs/bind_refs_front.hpp"
#endif