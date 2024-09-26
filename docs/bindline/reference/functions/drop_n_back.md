---
parent: Functions
ancestor: bindline
---

# `drop_n_back()`

```c++
// Returns bindline
template<size_t N>
[[nodiscard]]
constexpr auto drop_n_back();

// Returns functor
template<size_t N, class TFn>
[[nodiscard]]
constexpr auto drop_n_back(TFn&& f);
```

Returns a bindline or functor that calls the original function, but without the last `N` parameters when invoked.

## Example

```c++
auto fn1 = drop_n_back<2>(f);
fn1(1, 2, 3); // invokes f(1);
// Or, equivalently:
auto fn2 = f | drop_n_back<2>();
fn2(1, 2, 3); // also invokes f(1);
```

Prefer [`drop_winrt_event_args()`](drop_winrt_event_args.md) over `drop_n_back<2>()` for C++/WinRT event handlers.