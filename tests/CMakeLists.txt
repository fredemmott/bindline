# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(Catch)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    add_library(
        catch2_cppwinrt_main
        STATIC
        EXCLUDE_FROM_ALL
        catch2_cppwinrt_main.cpp
    )
    target_link_libraries(
        catch2_cppwinrt_main
        PRIVATE
        Catch2::Catch2
        cppwinrt::windows-sdk
    )

    # We don't explicitly set these on the actual tests as we want to test that
    # they inherit the requirement from the library target. As we're not
    # linking against the main library here, we need to - otherwise we'll end
    # up on whatever the dependency's minimum C++ is, and may get ABI issues.
    #
    # As of 2024-08-05, without this:
    # - we'd build this target with C++17
    # - on MSVC, we'll get a coroutines ABI mismatch error when linking with
    #   C++20 objects
    target_compile_features(
        catch2_cppwinrt_main
        PRIVATE
        ${REQUIRED_COMPILE_FEATURES}
    )
endif()

function(add_test_executable TARGET)
    set(options CPPWINRT_MAIN)
    set(multiValueArgs SOURCES LINK_LIBRARIES)

    cmake_parse_arguments(
        TEST_ARG
        "${options}"
        ""
        "${multiValueArgs}"
        ${ARGN})
        
    set(TARGET_ARGS ${TEST_ARG_UNPARSED_ARGUMENTS} ${TEST_ARG_SOURCES})

    if(TEST_ARG_CPPWINRT_MAIN)
        list(APPEND TEST_ARG_LINK_LIBRARIES "Catch2::Catch2" catch2_cppwinrt_main)
    else()
        list(APPEND TEST_ARG_LINK_LIBRARIES "Catch2::Catch2WithMain")
    endif()


    add_executable(${TARGET} ${TARGET_ARGS})
    catch_discover_tests(${TARGET})

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
        cppwinrt-bind_context-wil
        CPPWINRT_MAIN
        SOURCES
        cppwinrt/bind_context-wil.cpp
        LINK_LIBRARIES
        cppwinrt::windows-sdk
        wil::wil
    )
endif()