---
parent: Type Aliases
ancestor: weak_refs
---

# `strong_ref_t`

`strong_ref_t<T>` is the type produced by calling `lock_weak_ref(T)` on a [`strong_ref`](../concepts/strong_ref.md). For example:

| `T`                    | `strong_ref_t<T>`    |
|------------------------|----------------------|
| `std::weak_ptr<int>`   | `std::shared_ptr<T>` |

