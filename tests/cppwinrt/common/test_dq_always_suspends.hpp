// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "always_suspends.hpp"

inline void test_dq_always_suspends() {
  auto dqc = DispatcherQueueController::CreateOnDedicatedThread();
  auto dq = dqc.DispatcherQueue();

  CHECK(always_suspends(dq));

  dqc.ShutdownQueueAsync().get();
}