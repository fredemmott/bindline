# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(Catch)

function(add_test_executable TARGET)
    add_executable(${TARGET} ${ARGN})
    catch_discover_tests(${TARGET})

    target_link_libraries(
        ${TARGET}
        PRIVATE
        "${PROJECT_NAME}"
        Catch2::Catch2WithMain
    )
    set_target_properties(
        ${TARGET}
        PROPERTIES
        CXX_EXTENSIONS OFF
    )
endfunction()

if(WIN32)
  add_test_executable(cppwinrt-concepts cppwinrt/concepts.cpp)
  target_link_libraries(cppwinrt-concepts PRIVATE cppwinrt::windows-sdk)
endif()