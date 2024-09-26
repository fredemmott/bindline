---
parent: Functions
ancestor: bindline
---

# `drop_winrt_event_args()`

```c++
// Returns a bindline
[[nodiscard]]
constexpr auto drop_winrt_event_args();

// Returns a functor
[[nodiscard]]
constexpr auto drop_winrt_event_args(auto&& Fn);
```

Returns a functor or bindline that calls the original function, but without the last two arguments.

The last two arguments *must* be WinRT types (see [the `winrt_type` concept](../../../cppwinrt/reference/concepts/winrt_type.md)).

This is a more restricted (and safer) alternative to [`drop_n_back<2>()`](drop_n_back.md) intended for C++/WinRT event handlers.