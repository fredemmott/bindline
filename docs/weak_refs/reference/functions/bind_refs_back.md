---
parent: Functions
---

# `bind_refs_back()`

```c++
template<class Fn, convertible_to_weak_ref... Args>
auto bind_refs_back(Fn&&, Args&&...);
```

This function is similar to `std::bind_back()`, however:
- it calls [`make_weak_ref()`](make_weak_ref.md) on `Args...` before storing them in the functor
- it calls [`lock_weak_ref()`](lock_weak_ref.md) on the stored arguments before invoking `Fn`
- it does not call `Fn` unless all the strong references are truthy
- it passes the live strong references to `Fn`, along with any other arguments at runtime
