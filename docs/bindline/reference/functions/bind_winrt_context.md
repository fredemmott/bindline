---
parent: Functions
ancestor: bindline
---

# `bind_winrt_context()`

```c++
[[nodiscard]]
constexpr auto bind_winrt_context(auto&& context);
[[nodiscard]]
constexpr auto bind_winrt_context(auto&& fn, auto&& context);
```

Wraps [`FredEmmott::cppwinrt::bind_context()`](../../../cppwinrt/reference/functions/bind_context.md). The result is a bindline component or functor that when executed switches to the provided context (`IDispatcherQueue` or `winrt::apartment_context`), then invokes the provided function.

## Usage

```c++
auto fn1 = bind_winrt_context(f, this->uiThread);
fn1(); // executes in this->uiThread
// Or, equivalently
auto fn2 = f | bind_winrt_context(this->uiThread);
fn2(); // executes in this->uiThread
```