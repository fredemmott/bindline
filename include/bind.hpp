// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#ifndef OPENKNEEBOARD_BIND_ENABLE_CPPWINRT
#define OPENKNEEBOARD_BIND_ENABLE_CPPWINRT (__has_include(<winrt/base.h>))
#endif

#ifndef OPENKNEEBOARD_BIND_ENABLE_WEAK_REFS
#define OPENKNEEBOARD_BIND_ENABLE_WEAK_REFS true
#endif

#include "bind/bind_front.hpp"

#if OPENKNEEBOARD_BIND_ENABLE_CPPWINRT
#include "bind/cppwinrt/bind_winrt_context.hpp"
#include "bind/cppwinrt/discard_winrt_event_args.hpp"
#endif

#if OPENKNEEBOARD_BIND_ENABLE_WEAK_REFS
#include "bind/weak_refs.hpp"
#endif