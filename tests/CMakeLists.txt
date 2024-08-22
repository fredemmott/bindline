# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(Catch)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(IS_GITHUB_ACTIONS "Use github-action-specific output syntax etc" OFF)
option(ENABLE_ALL_WARNINGS "Enable all compiler warnings" ON)
option(ENABLE_WARNINGS_AS_ERRORS "All compiler warnings are errors" ON)

if(ENABLE_ALL_WARNINGS)
    # We need to find the Windows SDK so we don't turn on /Wall /Werror there
    GET_FILENAME_COMPONENT(
        WINDOWS_KITS_ROOT
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]"
        ABSOLUTE CACHE
    )
endif()

if(MSVC)
    set(
        WALL_FLAG
        # /Wall includes style choices; let's start here and opt-in to more
        /W4
        # C4514: unreferenced inline function was removed
        #
        # Expected as we don't test every function in every file
        /wd4514
    )
    # Clang-Cl maps `/Wall` to `-Weverything` instead of `-Wall`; the problem is
    # that this means your C++20 project will get all of the 'this feature is
    # not compatible with C++98' warnings.
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        list(REMOVE_ITEM WALL_FLAG /Wall)
        list(APPEND WALL_FLAG /clang:-Wall)
    endif()
else()
    set(WALL_FLAG -Wall)
    set(WERROR_FLAG -Werror)
endif()

macro(WARN MESSAGE_TEXT)
    message(WARNING "⚠️ ${MESSAGE_TEXT}")
    if(IS_GITHUB_ACTIONS)
        execute_process(
            COMMAND
            "${CMAKE_COMMAND}" -E echo "::warning::${MESSAGE_TEXT}"
        )
    endif()
endmacro()

function(add_test_executable SOURCE)
    set(options CPPWINRT_MAIN WIN32)
    set(valueArgs TEST_NAME REQUIRED_FLAGS)
    set(multiValueArgs SOURCES LINK_LIBRARIES CXX_COMPILE_FEATURES)

    cmake_parse_arguments(
        TEST_ARG
        "${options}"
        "${valueArgs}"
        "${multiValueArgs}"
        ${ARGN})

    if (TEST_ARG_WIN32)
        list(APPEND TEST_ARG_REQUIRED_FLAGS "WIN32")
    endif ()

    set(TARGET "${SOURCE}")
    string(REPLACE "/" "-" TARGET "${TARGET}")
    string(REPLACE " " "-" TARGET "${TARGET}")
    string(REPLACE ".cpp" "" TARGET "${TARGET}")

    if(TEST_ARG_TEST_NAME)
        set(TEST_NAME "${TEST_ARG_TEST_NAME}")
    else()
        cmake_path(GET SOURCE STEM LAST_ONLY TEST_NAME)
        string(REPLACE "-" "::" TEST_NAME "${TEST_NAME}")
    endif()
    cmake_path(GET SOURCE PARENT_PATH TEST_PREFIX)
    string(REPLACE "/" "📂" TEST_PREFIX "${TEST_PREFIX}")
    string(APPEND TEST_PREFIX "📂${TEST_NAME}➡️")

    if(TEST_ARG_REQUIRED_FLAGS AND NOT "${${TEST_ARG_REQUIRED_FLAGS}}")
        if(TEST_ARG_REQUIRED_FLAGS STREQUAL "WIN32")
            # No point warning on 'requires Windows' things.
            return()
        endif()
        set(DISABLED_BECAUSE_FLAG ${TEST_ARG_REQUIRED_FLAGS})
        WARN("Skipping test ${SOURCE} because ${TEST_ARG_REQUIRED_FLAGS} is not set")
        set(SOURCE "cmake/skip-stub.cpp")
    endif()

    set(TARGET_ARGS ${TEST_ARG_UNPARSED_ARGUMENTS} ${SOURCE})

    if(TEST_ARG_CPPWINRT_MAIN)
        list(APPEND TARGET_ARGS "catch2_cppwinrt_main.cpp")
        list(APPEND TEST_ARG_LINK_LIBRARIES "Catch2::Catch2")
    else()
        list(APPEND TEST_ARG_LINK_LIBRARIES "Catch2::Catch2WithMain")
    endif()


    add_executable(${TARGET} ${TARGET_ARGS})

    if(DISABLED_BECAUSE_FLAG)
        target_compile_definitions(${TARGET} PRIVATE "FEATURE_TEST_VARIABLE_NAME=${TEST_ARG_REQUIRED_FLAGS}")
    endif()

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
    # Allow `#include <tests/...>`
    target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/..")

    if(ENABLE_ALL_WARNINGS)
        target_compile_options(${TARGET} PRIVATE ${WALL_FLAG})
        # We don't actually want to include anything from these directory, but we do want to flag them as 'external'
        # so -Wall -Werror don't apply to them
        target_include_directories(
            ${TARGET}
            SYSTEM
            PRIVATE
            ${FETCHCONTENT_BASE_DIR}
            ${WINDOWS_KITS_ROOT}
        )
    endif()
    if(ENABLE_WARNINGS_AS_ERRORS)
        target_compile_options(${TARGET} PRIVATE ${WERROR_FLAG})
    endif()

    if(TEST_ARG_CXX_COMPILE_FEATURES)
        target_compile_features(${TARGET} PRIVATE "${TEST_ARG_CXX_COMPILE_FEATURES}")
    endif()

    if(WIN32 AND (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") AND (NOT "cppwinrt::windows-sdk" IN_LIST TEST_ARG_LINK_LIBRARIES))
        # The version of C++/WinRT included in the Windows SDK might not be usable without <Windows.h>
        #
        # e.g. for Clang, https://github.com/microsoft/cppwinrt/pull/1180
        #
        # Replace the `__has_include(<winrt/base.h>)` check
        target_compile_definitions(
            "${TARGET}"
            PRIVATE
            "FREDEMMOTT_BINDLINE_ENABLE_CPPWINRT=false"
            "FREDEMMOTT_WEAK_REFS_ENABLE_CPPWINRT=false"
        )
    endif()
endfunction()

if(CMAKE_VERSION VERSION_LESS "3.25")
    macro(try_compile TEST_NAME)
        WARN("Skipping ${TEST_NAME} test because this version of CMake is too old.")
    endmacro()
endif()

try_compile(
    HAVE_STD_BIND_BACK
    SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/have-std-bind_back.cpp"
    CXX_STANDARD ${REQUIRED_CXX_STANDARD}
)
##### cppwinrt #####

add_test_executable("cppwinrt/concepts.cpp" WIN32 LINK_LIBRARIES cppwinrt::windows-sdk)
add_test_executable(
    "cppwinrt/bind_context/winrt-apartment_context.cpp"
    WIN32 CPPWINRT_MAIN
    LINK_LIBRARIES cppwinrt::windows-sdk
)
add_test_executable(
    "cppwinrt/bind_context/wil-resume_foreground.cpp"
    WIN32 CPPWINRT_MAIN
    LINK_LIBRARIES cppwinrt::windows-sdk wil::wil
)
add_test_executable(
    "cppwinrt/bind_context/DispatcherQueue without WIL.cpp"
    WIN32 CPPWINRT_MAIN LINK_LIBRARIES cppwinrt::windows-sdk
)

##### weak_refs #####

add_test_executable("weak_refs/raw pointers.cpp")
add_test_executable("weak_refs/std-enable_shared_from_this.cpp")
add_test_executable("weak_refs/std-shared_ptr.cpp")
add_test_executable("weak_refs/bind_refs_back.cpp")
add_test_executable("weak_refs/bind_refs_front.cpp")
add_test_executable("weak_refs/strong_ref_reseater.cpp")

add_test_executable(
    "weak_refs/cppwinrt/winrt refs.cpp"
    WIN32 CPPWINRT_MAIN
    LINK_LIBRARIES cppwinrt::windows-sdk
)

add_test_executable(
    "weak_refs/cppwinrt/get_weak.cpp"
    WIN32 CPPWINRT_MAIN
    LINK_LIBRARIES cppwinrt::windows-sdk
)

##### bindline #####

add_test_executable("bindline/bind_front.cpp")
add_test_executable("bindline/bind_tap.cpp")
add_test_executable("bindline/detail-meta.cpp")
add_test_executable("bindline/direct invocation.cpp")
add_test_executable("bindline/drop_extra_back.cpp")
add_test_executable("bindline/drop_n_back.cpp")
add_test_executable("bindline/ordering requirements.cpp")

add_test_executable("bindline/bind_back.cpp" REQUIRED_FLAGS HAVE_STD_BIND_BACK)

add_test_executable("bindline/weak_refs/bind_refs_back.cpp")
add_test_executable("bindline/weak_refs/bind_refs_front.cpp")

add_test_executable(
    "bindline/cppwinrt/bind_winrt_context.cpp"
    WIN32 CPPWINRT_MAIN
    LINK_LIBRARIES cppwinrt::windows-sdk
)
add_test_executable(
    "bindline/cppwinrt/drop_winrt_event_args.cpp"
    WIN32 CPPWINRT_MAIN
    LINK_LIBRARIES cppwinrt::windows-sdk
)

add_test_executable(
    "bindline/cppwinrt/ordering requirements.cpp"
    WIN32 CPPWINRT_MAIN
    LINK_LIBRARIES cppwinrt::windows-sdk
)