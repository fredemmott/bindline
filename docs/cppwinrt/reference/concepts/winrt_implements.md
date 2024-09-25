---
parent: Concepts
ancestor: cppwinrt
---

# `winrt_implements`

```c++
template <class T, class Interface>
concept winrt_implements = ...;
```

A class that extends `winrt::implements<..., Interface, ...`; for example:

- `winrt_implements<MyClass, winrt::IStringable>`
- `winrt_implements<MyClass, winrt::no_weak_ref>`