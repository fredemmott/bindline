# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(catch2.cmake)

if(WIN32)
    include(cppwinrt.cmake)
    include(wil.cmake)
endif()