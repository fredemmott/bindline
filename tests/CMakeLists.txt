# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(Catch)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_test_executable SOURCE)
    string(REPLACE "/" "-" TARGET "${SOURCE}")
    string(REPLACE ".cpp" "" TARGET "${TARGET}")

    set(options CPPWINRT_MAIN)
    set(valueArgs TEST_NAME)
    set(multiValueArgs SOURCES LINK_LIBRARIES)

    cmake_parse_arguments(
        TEST_ARG
        "${options}"
        "${valueArgs}"
        "${multiValueArgs}"
        ${ARGN})

    if(TEST_ARG_TEST_NAME)
        set(TEST_NAME "${TEST_ARG_TEST_NAME}")
    else()
        cmake_path(GET SOURCE STEM LAST_ONLY TEST_NAME)
        string(REPLACE "-" "::" TEST_NAME "${TEST_NAME}")
    endif()
    cmake_path(GET SOURCE PARENT_PATH TEST_PREFIX)
    string(REPLACE "/" "📂" TEST_PREFIX "${TEST_PREFIX}")
    string(APPEND TEST_PREFIX "📂${TEST_NAME}➡️")
        
    set(TARGET_ARGS ${TEST_ARG_UNPARSED_ARGUMENTS} ${SOURCE})

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

add_test_executable(weak_refs/concepts.cpp)

if(WIN32)
    add_test_executable(
        cppwinrt/concepts.cpp
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )

    add_test_executable(
        cppwinrt/bind_context/winrt-apartment_context.cpp
        CPPWINRT_MAIN
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )

    add_test_executable(
        cppwinrt/bind_context/wil-resume_foreground.cpp
        CPPWINRT_MAIN
        LINK_LIBRARIES
        cppwinrt::windows-sdk
        wil::wil
    )

    add_test_executable(
        cppwinrt/bind_context/winrt-resume_foreground.cpp
        CPPWINRT_MAIN
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )
endif()