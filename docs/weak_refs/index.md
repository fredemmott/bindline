---
title: weak_refs
nav_order: 4
---

# `FredEmmott::weak_refs`

`FredEmmott::weak_refs` is a header-only library for:
- writing code that transparently supports multiple kinds of strong/weak refs
- creating functors that store a weak reference, but invoke with a valid strong reference

For example, code using `FredEmmott::weak_refs` can write code that transparently supports both 
the `std::shared_ptr<T>`/`std::weak_ptr<T>` combination, and the `winrt::com_ptr<T>`/`winrt::weak_ref<T>` combination.

## Terminology

- a [`strong_ref`](reference/concepts/strong_ref.md) is an owning-reference
- a [`weak_ref`](reference/concepts/weak_ref.md) is a non-owning reference that can be converted to a `strong_ref`
- *locking* a `weak_ref` produces a `strong_ref`
- an invalid `weak_ref` or `strong_ref` is *expired*

A *expired* ref can usually be produced by:
- locking an expired `weak_ref`
- making a `weak_ref` from an expired `strong_ref`
- explicit initialization from `nullptr`
- holding an unexpired `weak_ref`, and destroying all the corresponding `strong_ref`s

For example, an `std::shared_ptr<T>` is a `strong_ref`, and an `std::weak_ptr<T>` is a `weak_ref`.

## Using `FredEmmott::weak_refs` with `FredEmmott::bindline`

If you are using `FredEmmott::bindline`, you probably want to start with [bindline's weak_refs integration](../bindline/weak_refs.md) instead of using weak_refs directly; keep reading this page if you want to use weak_ref without bindline, or if you want to extend weak_refs.

## Using `FredEmmott::weak_refs` without `FredEmmott::bindline`

### Basic Usage

```c++
#include <FredEmmott/weak_refs.hpp>
using namespace FredEmmott::weak_refs;

void test(const strong_ref auto& strong) {
  auto weak = make_weak_ref(strong);
  auto strong2 = lock_weak_ref(weak);
}
```

This will lead to the following conversions depending on the input:

| `strong`                           | `weak`               | `strong2`            |
|------------------------------------|----------------------|----------------------|
| `std::shared_ptr<T>`               | `std::weak_ptr<T>`   | `std::shared_ptr<T>` |
| `std::enable_shared_from_this<T>*` | `std::weak_ptr<T>`   | `std::shared_ptr<T>` |
| `winrt::com_ptr<T>`                | `winrt::weak_ref<T>` | `winrt::com_ptr<T>`  |

Similar conversions will be made for other types, such as Windows Runtime objects.

For details, see the API reference for:
- [`lock_weak_ref()`](reference/functions/lock_weak_ref.md)
- [`make_weak_ref()`](reference/functions/make_weak_ref.md)

### Function Binding

```c++
#include <FredEmmott/weak_refs.hpp>
using namespace FredEmmott::weak_refs;

void test(const strong_ref auto& strong) {
  // The functor `f` contains a weak reference to the object pointed to by `strong`
  auto f = bind_refs_front(
    [](auto self) { /* `self` is a `strong_ref` */ } },
    strong);
  // The weak reference is converted back to a strong reference, then - the lambda is invoked
  // if and only if the reference is valid; for example, the lambda will not be invoked if
  // the stored value is an expired `std::weak_ptr`
  f();
}
```

For details, see the API reference for:
- [`bind_refs_back()`](reference/functions/bind_refs_back.md)
- [`bind_refs_front()`](reference/functions/bind_refs_front.md)

## Additional Features

`FredEmmott::weak_refs` also contains useful classes, type aliases, and C++20 concepts; see [the API reference](reference/index.md) for details.

## Supporting Additional Types

`FredEmmott::weak_refs` can be [extended](extending/index.md) to support additional types. All types that are supported out of the box are internally implemented via the extension system.