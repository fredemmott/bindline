// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include "check_forwards_arguments.hpp"

TEST_CASE("forwarding arguments") {
  auto dqc = DispatcherQueueController::CreateOnDedicatedThread();
  auto dq = dqc.DispatcherQueue();

  check_forwards_arguments(dq);

  dqc.ShutdownQueueAsync().get();
}