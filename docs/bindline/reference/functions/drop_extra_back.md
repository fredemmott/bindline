---
parent: Functions
ancestor: bindline
---

# `drop_extra_back()`

```c++
[[nodiscard]]
constexpr auto drop_extra_back();
[[nodiscard]]
template<class TFn>
constexpr auto drop_extra_back(TFn&& f);
```

Returns a functor or bind pipeline (bindline) that attempts to invoke `f` with all the callee arguments, and tries again with fewer arguments (removing arguments from then end) until the function is invocable.

[`drop_n_back()`](drop_n_back.md) is strongly encouraged instead where practical.