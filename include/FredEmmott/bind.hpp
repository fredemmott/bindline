// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <functional>

#include "bind/bind_front.hpp"
#include "bind/bind_tap.hpp"
#include "bind/bindable_t.hpp"

#ifdef __cpp_lib_bind_back
#include "bind/bind_back.hpp"
#endif