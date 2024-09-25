---
parent: Concepts
ancestor: cppwinrt
---

# `any_com_ptr`

```c++
template <class T>
concept any_com_ptr = ...;
```

Any `winrt::com_ptr<>`; use [`winrt_com_ptr`](winrt_com_ptr.md) instead if you want a `winrt::com_ptr<>` to a [`winrt_type`](winrt_type.md).