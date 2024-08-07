# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(Catch)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_test_executable SOURCE)
    set(TARGET "${SOURCE}")
    string(REPLACE "/" "-" TARGET "${TARGET}")
    string(REPLACE " " "-" TARGET "${TARGET}")
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

    if(WIN32 AND (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") AND (NOT "cppwinrt::windows-sdk" IN_LIST TEST_ARG_LINK_LIBRARIES))
        # The version of C++/WinRT included in the Windows SDK might not be usable without <Windows.h>
        #
        # e.g. for Clang, https://github.com/microsoft/cppwinrt/pull/1180
        #
        # Replace the `__has_include(<winrt/base.h>)` check
        target_compile_definitions("${TARGET}" PRIVATE "FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT=false")
    endif()
endfunction()

add_test_executable("weak_refs/raw pointers.cpp")
add_test_executable("weak_refs/std-enable_shared_from_this.cpp")
add_test_executable("weak_refs/std-shared_ptr.cpp")
add_test_executable("weak_refs/bind_refs_front.cpp")
add_test_executable("weak_refs/bind_maybe_refs_front.cpp")

if(WIN32)
    # --- cppwinrt ---

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
        "cppwinrt/bind_context/DispatcherQueue without WIL.cpp"
        CPPWINRT_MAIN
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )
    

    # --- weak_refs ---

    add_test_executable(
        "weak_refs/cppwinrt/winrt refs.cpp"
        CPPWINRT_MAIN
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )

    add_test_executable(
        "weak_refs/cppwinrt/get_weak.cpp"
        CPPWINRT_MAIN
        LINK_LIBRARIES
        cppwinrt::windows-sdk
    )
endif()