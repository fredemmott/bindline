---
parent: Functions
ancestor: weak_refs
---

# `bind_refs_front()`

```c++
template<class Fn, convertible_to_weak_ref... Args>
auto bind_refs_front(Fn&&, Args&&...);
```

This function is similar to `std::bind_front()`, however:
- it calls [`make_weak_ref()`](make_weak_ref.md) on `Args...` before storing them in the functor
- it calls [`lock_weak_ref()`](lock_weak_ref.md) on the stored arguments before invoking `Fn`
- it does not call `Fn` unless all the strong references are truthy
- it passes the live strong references to `Fn`, along with any other arguments at runtime

If multiple references are bound, they do not need to be similar types; they just need to all be `convertible_to_weak_ref`.

# Example

```c++
auto foo = std::make_shared<std::string_view>("foo");
auto bound = bind_refs_front(fn, foo);
bound(123); // invokes `fn(foo, 123)`

// As `bound` stores a `weak_ref`, this sets the `shared_ptr`'s reference count
// to 0...
foo.reset(nullptr);
// ... and as the weak_refs are now expired, this *does not* call `fn`:
bound(123);
```