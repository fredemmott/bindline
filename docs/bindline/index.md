---
title: bindline
nav_order: 2
---

# `FredEmmott::bindline`

`FredEmmott::bindline` is a header-only library to compose functions in a
readable way, similar to `std::ranges::views`. It is most useful in
asynchronous code, such as event handlers, other callbacks, and coroutines.

This library was originally an internal implementation detail of [OpenKneeboard](https://openkneeboard.com), which uses it heavily.

## Examples

```c++
auto f = &MyClass::some_func
  // Store an `std::weak_ptr` or `winrt::weak_ref`, but when invoking:
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

Common errors can be automatically detected at compile-time:

```c++
// This is fine:
f | bind_refs_front(this) | bind_winrt_context(ctx);

/* This is an error:
 *
 * functions on the right wrap functions on the left, and you probably want to
 * promote the weak ref to a strong ref _after_ switching threads:
 */
f | bind_winrt_context(ctx) | bind_refs_front(this);
```

For details, see [documentation for validation](validation.md).

## Terminology

- a *functor* is an object that can be invoked, i.e. implements `operator()`
- a pipeline of composed functions is a *bindline*