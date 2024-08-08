# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

# FIXME when CMake 3.25+ is OK: wrap the whole thing in a block()

set(CMAKE_CXX_STANDARD_REQUIRED ${REQUIRED_CXX_STANDARD})

include(catch2.cmake)

if(WIN32)
    include(cppwinrt.cmake)
    include(wil.cmake)
endif()