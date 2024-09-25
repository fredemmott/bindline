---
parent: Functions
---

# `make_weak_ref()`

```c++
template<convertible_to_weak_ref T>
weak_ref_t<T> make_weak_ref(T&& v);
```

Creates a [`weak_ref`](../concepts/weak_ref.md) from a [`convertible_to_weak_ref`](../concepts/convertible_to_wewak_ref.md); for example:

| `T` | `make_weak_ref(T)` |
|-|-|
| `std::shared_ptr<T>` | `std::weak_ptr<T>` |
| `std::weak_ptr<T>` | `std::weak_ptr<T>` |
| `std::enable_shared_from_this<T>*` | `std::weak_ptr<T>` |