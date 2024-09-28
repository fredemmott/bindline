---
title: ADL
parent: Extending
ancestor: weak_refs
---

# Argument-Dependent Lookup (ADL)

To extend `FredEmmott::weak_refs` to support a type in a namespace that you control/own, add `lock_weak_ref()` and `make_weak_ref()` functions to the same namespace.

For example:

```c++
namespace TestNS {
struct StrongInt {
  int value {};

  constexpr bool operator==(const StrongInt&) const noexcept = default;
};

struct WeakInt {
  int value {};

  constexpr bool operator==(const WeakInt&) const noexcept = default;
};

auto make_weak_ref(const StrongInt v) {
  return WeakInt {v.value};
}

auto lock_weak_ref(const WeakInt v) {
  return StrongInt {v.value};
}
}// namespace TestNS
```

To add support for a type in a namespace you do not control/own, add [an extension](extensions.md) instead.