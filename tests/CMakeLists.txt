# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT
include(CTest)

add_executable(
    stub_test
    stub_test.cpp
)

target_link_libraries(
    stub_test
    PRIVATE
    "${PROJECT_NAME}"
    cppwinrt::windows-sdk
    wil::wil
)

set_target_properties(
    stub_test
    PROPERTIES
    CXX_EXTENSIONS OFF
)

include(CTest)

add_test(
    NAME stub_test 
    COMMAND "$<TARGET_FILE:stub_test>"
)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")