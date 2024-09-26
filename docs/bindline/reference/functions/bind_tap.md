---
parent: Functions
ancestor: bindline
---

# `bind_tap()`

```c++
// Returns bindline
[[nodiscard]]
constexpr auto bind_tap(auto&& tap);

// Returns functor
[[nodiscard]]
constexpr auto bind_tap(auto&& f, auto&& tap);
```

Returns a functor or bindline that executes the `tap` before executing the input callable; arguments and return value are unchanged.

This is useful for debugging, as it can be inserted in the middle of a bindline without modifying it.

# Example
```c++
fn1 = fn | bind_tap([](auto arg) { std::println("fn called with: {}", arg) } | ....;
fn1();
```