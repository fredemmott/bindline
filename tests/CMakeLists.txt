# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(Catch)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_test_executable TARGET)
    set(options CPPWINRT_MAIN)
    set(valueArgs TEST_PREFIX)
    set(multiValueArgs SOURCES LINK_LIBRARIES)

    if("${TEST_ARG_TEST_PREFIX}")
        set(TEST_PREFIX "${CTEST_ARG_TEST_PREFIX}")
    else()
        string(REPLACE "-" "/" TEST_PREFIX "${TARGET}")
    endif()
    string(APPEND TEST_PREFIX "➡️")

    cmake_parse_arguments(
        TEST_ARG
        "${options}"
        ""
        "${multiValueArgs}"
        ${ARGN})
        
    set(TARGET_ARGS ${TEST_ARG_UNPARSED_ARGUMENTS} ${TEST_ARG_SOURCES})

    if(TEST_ARG_CPPWINRT_MAIN)
        list(APPEND TARGET_ARGS "catch2_cppwinrt_main.cpp")
        list(APPEND TEST_ARG_LINK_LIBRARIES "Catch2::Catch2")
    else()
        list(APPEND TEST_ARG_LINK_LIBRARIES "Catch2::Catch2WithMain")
    endif()


    add_executable(${TARGET} ${TARGET_ARGS})
    catch_discover_tests(
        ${TARGET}
        TEST_PREFIX "${TEST_PREFIX}"
        PROPERTIES
        SKIP_RETURN_CODE 4
    )

    target_link_libraries(
        ${TARGET}
        PRIVATE
        "${PROJECT_NAME}"
        ${TEST_ARG_LINK_LIBRARIES}
    )
endfunction()

if(WIN32)
    add_test_executable(
        cppwinrt-concepts
        SOURCES
        cppwinrt/concepts.cpp
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )

    add_test_executable(
        cppwinrt-bind_context
        CPPWINRT_MAIN
        SOURCES
        cppwinrt/bind_context.cpp
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )

    add_test_executable(
        cppwinrt-bind_context-DispatcherQueue
        CPPWINRT_MAIN
        SOURCES
        cppwinrt/bind_context-DispatcherQueue.cpp
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )

    add_test_executable(
        cppwinrt-bind_context-wil
        CPPWINRT_MAIN
        SOURCES
        cppwinrt/bind_context-wil.cpp
        LINK_LIBRARIES
        cppwinrt::windows-sdk
        wil::wil
    )
endif()