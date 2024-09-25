---
parent: Concepts
ancestor: cppwinrt
---

# `winrt_com_ptr`

```c++
template <class T>
concept winrt_com_ptr = ...;
```

A `winrt::com_ptr<>` to a [`winrt_type`](winrt_type.md); use [`any_com_ptr`](any_com_ptr.md) instead if you want to match any `winrt::com_ptr<>`, including pointers to non-WinRT COM pointers.