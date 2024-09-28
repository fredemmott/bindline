---
parent: Functions
ancestor: bindline
---

# `bind_refs_front()`

```c++
// Returns object that is both a functor and a bindline
template<class... Args>
[[nodiscard]]
constexpr auto bind_refs_front(Args&&...);
```

This function wraps [`FredEmmott::weak_refs::bind_refs_front()`](../../../weak_refs/reference/functions/bind_refs_front.md), and can optionally be composed into a bind pipeline (bindline).

## Example

```c++
auto foo = std::make_shared<std::string_view>("foo");
// Option 1: use similarly to `weak_refs::bind_refs_front()`
auto bound = FredEmmott::bindline::bind_refs_front(fn, foo);
bound(123); // invoke fn(foo, 123);
// Option 2: construct a bindline
auto bound2 = fn | FredEmmott::bindline::bind_refs_front(fn, foo);
bound2(123); // also invoke fn(foo, 123);

// As `bound` and `bound2` store a `weak_ref`, this makes `foo`'s refcount
// reach 0
foo.reset(nullptr);
// as the refcount is 0, the stored `weak_ref`'s have expired, so neither of these
// actually call `fn()`:
bound(123);
bound2(123);
```

If the first argument is invocable, the intended usage is ambiguous until either `operator()` or `operator|` is called, so the returned object can be used for either or both:

```c++
auto foo = std::make_shared<std::string_view>("foo");
auto bound = FredEmmott::bindline::bind_refs_front(fn, foo);
bound(); // invoke fn(foo);
auto bound2 = fn2 | bound;
bound2() // invoke fn2(fn, foo);
```
