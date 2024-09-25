---
title: Introduction
nav_order: 0
layout: home
---

# Introduction

Bindline is a suite of several *header-only libraries* which are designed to avoid common mistakes with asynchronous code, e.g. event handlers and coroutines.

- `FredEmmott::bindline`: a function composition library
- `FredEmmott::weak_refs`: concepts and functional helpers for working with strong and weak references, e.g. `std::shared_ptr()` and `std::weak_ptr()`
- `FredEmmott::cppwinrt`: concepts and functional helpers for working with [C++/WinRT](https://aka.ms/cppwinrt)

## Requirements

- C++20 is required
- additional features are available with C++23
- these libraries are tested with recent versions of Clang, Visual Studio, and G++

This project only aims to support recent compilers and standards; in particular, it is only tested with the versions of compilers that are available in GitHub's runners.

## Example

Event handlers, callbacks, and asynchronously-called code often have repetitive boilerplate for safety, or to fit a given signature; bindline aims to make this less error-prone:

```c++
auto f = &MyClass::some_func
  // Store an `std::shared_ptr` or `winrt::weak_ref`, but when invoking:
  // 1. convert back to a strong ref
  // 2. return without invoking if the ref is stale (i.e. `nullptr`)
  | bind_refs_front(this)
  // Automatically switch thread when invoked
  | bind_winrt_context(winrt_apartment_context_or_dispatcher_queue())
  // if some_func() takes N parameters, require invocing with N + 2
  // parameters; this can be useful for event handlers
  | drop_n_back<2>();

f(args...);
```