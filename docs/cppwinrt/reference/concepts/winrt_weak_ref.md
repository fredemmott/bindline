---
parent: Concepts
ancestor: cppwinrt
---

# `winrt_weak_ref`

```c++
template <class T>
concept winrt_weak_ref = ...;
```

A non-owning reference to a `winrt_type`; see [`weak_refs::weak_ref`](../../../weak_refs/reference/concepts/weak_ref.md).

A `winrt_weak_ref` can be converted to an owning reference (`winrt_strong_ref`) by calling `.get()`; the result *may* be `null`.