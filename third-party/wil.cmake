# Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
# SPDX-License-Identifier: MIT

include(FetchContent)

FetchContent_Declare(
    wil
    URL https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.240803.1
    URL_HASH SHA256=FBC8F63269C99BC551E41E48D258B9F011BBF4A5C3FA3F706307D5EBCF70B087
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(wil)

add_library(wil::wil INTERFACE IMPORTED GLOBAL)
target_include_directories(
    wil::wil
    INTERFACE
    "${wil_SOURCE_DIR}/include"
)