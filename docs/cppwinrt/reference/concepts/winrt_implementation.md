---
parent: Concepts
ancestor: cppwinrt
---

# `winrt_implementation`

```c++
template <class T>
concept winrt_implementation = winrt_type<T> && ...;
```

A WinRT implementation class, as opposed to a [`winrt_interface`](winrt_interface.md).