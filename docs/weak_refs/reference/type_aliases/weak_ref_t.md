---
parent: Type Aliases
---

# `weak_ref_t`

`weak_ref_t<T>` is the type produced by `make_weak_ref(T)` on a [`convertible_to_weak_ref`](../concepts/convertible_to_weak_ref.md). For example:

| `T`                    | `weak_ref_t<T>`    |
|------------------------|--------------------|
| `std::shared_ptr<int>` | `std::weak_ptr<T>` |
| `std::weak_ptr<int>`   | `std::weak_ptr<T>` |

