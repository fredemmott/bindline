---
parent: Functions
ancestor: bindline
---

# `bind_back()`

```c++
// Returns object that is both a functor and a bindline
template<class... Args>
[[nodiscard]]
constexpr auto bind_back(Args&&...);
```

This function wraps `std::bind_back()`, and can optionally be composed into a bind pipeline (bindline).

As this function wraps `std::bind_back()`, this function is only available when building with C++23 or newer.

## Example

```c++
// Option 1: use similarly to `std::bind_back()`
auto bound = FredEmmott::bindline::bind_back(fn, "foo");
bound(123); // invoke fn(123, "foo");
// Option 2: construct a bindline
auto bound2 = fn | FredEmmott::bindline::bind_back("foo");
bound2(123); // also invoke fn(123, "foo");
```

If the first argument is invocable, the intended usage is ambiguous until either `operator()` or `operator|` is called, so the returned object can be used for either or both:

```c++
auto bound = FredEmmott::bindline::bind_back(fn, "foo");
bound(); // invoke fn("foo");
auto bound2 = fn2 | bound;
bound2() // invoke fn2(fn, "foo");
```
