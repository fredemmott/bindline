---
parent: Concepts
ancestor: cppwinrt
---

# `winrt_interface`

```c++
template <class T>
concept winrt_interface = winrt_type<T> && !winrt_implementation<T>; 
```

A WinRT interface type, as opposed to a [`winrt_implementation`](winrt_implementation.md).

This *can not* be used to test if a WinRT implementation type implements a particular interface; use [`winrt_implements`](winrt_implements.md) for that.