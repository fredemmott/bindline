---
title: Installation
nav_order: 1
---

# Installation

Bindline, cppwinrt, and weak_refs are intended to be added directly to your project rather than installed system-wide.

## Dependencies

The projects have no dependencies; however, if your project uses a custom version of C++/WinRT (e.g. from nuget, instead of the version included in the Windows SDK), you may want to make sure that any parts of your project that use bindline also use the correct version of C++/WinRT.

## Adding the header files to your project

There are several options:
- [Just add the files](#just-add-the-files)
- [CMake (FetchContent)](#cmake-fetchcontent)

### Just add the files

You can copy the source tree into your project, use a git submodule, or any other method of adding the files to the tree; once you've done this,  just add the `include` subfolder to your build system.

### CMake (FetchContent)

Once you have added the project via [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html), the following header-only CMake targets are defined:

- `FredEmmott::bindline`
- `FredEmmott::cppwinrt`
- `FredEmmott::weak_refs`

You can use these in your targets with [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html).

## Customizing the libraries

The default configuration is specified in:
- [include/FredEmmott/bindline/config.hpp](https://github.com/fredemmott/bindline/blob/main/include/FredEmmott/bindline/config.hpp)
- [include/FredEmmott/cppwinrt/config.hpp](https://github.com/fredemmott/bindline/blob/main/include/FredEmmott/cppwinrt/config.hpp)
- [include/FredEmmott/weak_refs/config.hpp](https://github.com/fredemmott/bindline/blob/main/include/FredEmmott/weak_refs/config.hpp)

You can override the defaults by creating the following header files anywhere in your compiler's include path:

- `FredEmmott.bindline.config-tweaks.hpp`
- `FredEmmott.cppwinrt.config-tweaks.hpp`
- `FredEmmott.weak_refs.config-tweaks.hpp`

## `use namespace`

The libraries are designed to be used cleanly via `use namespace`, though this is optional. The libraries can also be used with fully-qualified names, or they define sub-namespaces if you just want to `use` a subset of the functionality.

Most users will just want `use namespace FredEmmott::bindline;`