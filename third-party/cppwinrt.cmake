# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(FetchContent)

FetchContent_Declare(
    cppwinrt
    URL https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.240405.15
    URL_HASH SHA256=E889007B5D9235931E7340DDF737D2C346EEBDD23C619F1F4F2426A2AAE47180
)
FetchContent_MakeAvailable(cppwinrt)

add_executable(cppwinrt::cppwinrt IMPORTED)
set_target_properties(
    cppwinrt::cppwinrt 
    PROPERTIES
    IMPORTED_LOCATION "${cppwinrt_SOURCE_DIR}/bin/cppwinrt.exe"
)

set(WINDOWS_SDK_WINRT_OUTPUT "${cppwinrt_BINARY_DIR}/windows-sdk")

add_custom_command(
    OUTPUT "${WINDOWS_SDK_WINRT_OUTPUT}/winrt/base.h"
    COMMAND
    "$<TARGET_FILE:cppwinrt::cppwinrt>"
    -in sdk
    -output "${WINDOWS_SDK_WINRT_OUTPUT}"
)
add_custom_target(
    cppwinrt-windows-sdk-gen
    DEPENDS
    "${WINDOWS_SDK_WINRT_OUTPUT}/winrt/base.h"
)

add_library(
    cppwinrt-windows-sdk
    INTERFACE
    EXCLUDE_FROM_ALL
)
add_dependencies(cppwinrt-windows-sdk INTERFACE cppwinrt-windows-sdk-gen)
target_include_directories(cppwinrt-windows-sdk INTERFACE "${WINDOWS_SDK_WINRT_OUTPUT}")
target_link_libraries(cppwinrt-windows-sdk INTERFACE "WindowsApp")

add_library(cppwinrt::windows-sdk ALIAS cppwinrt-windows-sdk)
