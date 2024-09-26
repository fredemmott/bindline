---
parent: Functions
ancestor: bindline
---

# `bind_front()`

```c++
template<class... Args>
[[nodiscard]]
constexpr auto bind_front(Args&&...);
```

This function wraps `std::bind_front()`, and can optionally be composed into a bind pipeline (bindline).

## Usage

```c++
// Option 1: use similarly to `std::bind_front()`
auto bound = FredEmmott::bindline::bind_front(fn, "foo");
bound(123); // invoke fn("foo", 123);
// Option 2: construct a bindline
auto bound2 = fn | FredEmmott::bindline::bind_front("foo");
bound2(123); // also invoke fn("foo", 123);
```

If the first argument is invocable, the intended usage is ambiguous until either `operator()` or `operator|` is called, so the returned object can be used for either or both:

```c++
auto bound = FredEmmott::bindline::bind_front(fn, "foo");
bound(); // invoke fn("foo");
auto bound2 = fn2 | bound;
bound2() // invoke fn2(fn, "foo");
```