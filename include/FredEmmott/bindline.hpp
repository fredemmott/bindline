// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "bindline/config.hpp"

#include <functional>

#include "bindline/bind_front.hpp"
#include "bindline/bind_tap.hpp"
#include "bindline/bindable_t.hpp"
#include "bindline/drop_extra_back.hpp"
#include "bindline/drop_n_back.hpp"

#if FREDEMMOTT_BINDLINE_ENABLE_BIND_BACK
#include "bindline/bind_back.hpp"
#endif

#if FREDEMMOTT_BINDLINE_ENABLE_WEAK_REFS
#include "bindline/weak_refs.hpp"
#endif

#if FREDEMMOTT_BINDLINE_ENABLE_CPPWINRT
#include "bindline/cppwinrt.hpp"
#include "bindline/cppwinrt/drop_winrt_event_args.hpp"
#endif

#if __has_include(<FredEmmott.bindline.footer-tweaks.hpp>)
#include <FredEmmott.bindline.footer-tweaks.hpp>
#endif