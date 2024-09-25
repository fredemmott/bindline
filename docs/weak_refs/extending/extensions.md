---
parent: Extending
---

# Extensions

`FredEmmott::weak_refs` can be extended to support new types by creating a pair of extension classes; if you control/own the namespace containing the type, [ADL](adl.md) is an easier alternative.

All weak_ref/strong_ref types are supported via extensions; for example, weak_refs's built-in support for the following is via bundled extensions:
- `std::shared_ptr`
- C++/WinRT
- Argument-Dependent Lookup (ADL)

An 'extension' is just a name for a pair of template class specializations in the `FredEmmott::weak_refs_extensions` namespace:

```c++
#include <FredEmmott/weak_refs/extensions.hpp>

template<>
struct make_weak_ref_fn<SomeNS::SomeStrongRef> {
  auto make(auto value) {
    return somehow_convert_to_weak(value);
  }
};

template<>
struct lock_weak_ref_fn<SomeNS::SomeWeakRef> {
  auto lock(auto value) {
    return somehow_convert_to_strong(value);
  }
};
```

Most extensions partially specialize the classes, using C++20 concepts to match appropriate types.