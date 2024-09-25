---
parent: Functions
ancestor: weak_refs
---

# `lock_weak_ref()`

```c++
template<weak_ref T>
strong_ref_t<T> lock_weak_ref(T&& v);
```

Creates a [`strong_ref`](../concepts/strong_ref.md) from a [`weak_ref`](../concepts/weak_ref.md); for example:

| `T` | `lock_weak_ref(T)` |
|-|-|
| `std::weak_ptr<T>` | `std::shared_ptr<T>` |
| `winrt::weak_ref<T>` | `winrt::com_ptr<T>` |