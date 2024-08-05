# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25)
project(FredEmmott_WeakRefs VERSION 0.1 LANGUAGES CXX)

option(ENABLE_TESTS "Build and run tests" ${PROJECT_IS_TOP_LEVEL})

message(STATUS "${PROJECT_NAME} tests: ${ENABLE_TESTS}")

add_library("${PROJECT_NAME}" INTERFACE)
target_include_directories("${PROJECT_NAME}" INTERFACE include)
target_compile_features("${PROJECT_NAME}" INTERFACE cxx_std_20)

add_library(FredEmmott::bind ALIAS "${PROJECT_NAME}")
add_library(FredEmmott::cppwinrt ALIAS "${PROJECT_NAME}")
add_library(FredEmmott::weak_refs ALIAS "${PROJECT_NAME}")

if(ENABLE_TESTS)
    add_subdirectory(third-party)
    add_subdirectory(tests)
endif()