// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

namespace FredEmmott::bindline_extension_api {

enum class ordering_requirements_t {
  no_ordering_requirements,
  invoke_before_context_switch,
  is_context_switch,
  invoke_after_context_switch,// e.g. converting a weak_ptr to a shared_ptr
};

}