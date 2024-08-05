include(CTest)

add_executable(
    stub_test
    stub_test.cpp
)

target_link_libraries(stub_test PRIVATE "${PROJECT_NAME}")

set(
    NUGET_PACKAGES
    "Microsoft.Windows.CppWinRT_2.0.240405.15"
    "Microsoft.Windows.ImplementationLibrary_1.0.240122.1"
)

set_target_properties(
    stub_test
    PROPERTIES
    CXX_EXTENSIONS OFF
    VS_PACKAGE_REFERENCES "${NUGET_PACKAGES}"
    VS_GLOBAL_NuGetTargetMoniker "native,Version=v0.0"
)

include(CTest)

add_test(
    NAME stub_test 
    COMMAND "$<TARGET_FILE:stub_test>"
)