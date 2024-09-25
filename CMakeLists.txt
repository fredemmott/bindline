# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

# As of 2024-05-08, this is the version available in Ubuntu LTS (22.04)
cmake_minimum_required(VERSION 3.22...3.30)

project(bindline VERSION 0.1 LANGUAGES CXX)

option(ENABLE_TESTS "Build and run tests" ${PROJECT_IS_TOP_LEVEL})

message(STATUS "${PROJECT_NAME} tests: ${ENABLE_TESTS}")

if (MSVC)
  add_compile_options("/EHsc")
endif()

set(REQUIRED_CXX_STANDARD 20)
if(ENABLE_TESTS)
    set(FORCE_CXX_STANDARD OFF CACHE STRING "Version of C++ to use for building the tests")
    if(FORCE_CXX_STANDARD)
        set(REQUIRED_CXX_STANDARD "${FORCE_CXX_STANDARD}")
    endif()
endif()
set(REQUIRED_COMPILE_FEATURES "cxx_std_${REQUIRED_CXX_STANDARD}")
message(STATUS "Required compile features: ${REQUIRED_COMPILE_FEATURES}")

include(GNUInstallDirs)

add_library(bindline INTERFACE
        tests/weak_refs/adl.cpp)
target_include_directories(bindline INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_compile_features(bindline INTERFACE ${REQUIRED_COMPILE_FEATURES})

if(ENABLE_TESTS)
    include(CTest)
    
    add_subdirectory(third-party)
    add_subdirectory(tests)
endif()

add_library(cppwinrt INTERFACE)
target_link_libraries(cppwinrt INTERFACE bindline)
add_library(weak_refs INTERFACE)
target_link_libraries(weak_refs INTERFACE bindline)

install(
  TARGETS bindline cppwinrt weak_refs
  EXPORT "${PROJECT_NAME}_Targets"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "bindlineConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion)
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake"
)

install(
  EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE "FredEmmott::"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)


install(
  FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/FredEmmott" DESTINATION include)

# Export for fetchcontent/add_subdirectory

add_library(FredEmmott::bindline ALIAS bindline)
add_library(FredEmmott::cppwinrt ALIAS cppwinrt)
add_library(FredEmmott::weak_refs ALIAS weak_refs)