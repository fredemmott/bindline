---
parent: Functions
ancestor: cppwinrt
---

# `bind_context()`

```c++
template<class Fn, class Context>
auto bind_context(Fn&&, Context&&);
```

Creates a functor that invokes the parameter in the specified context (thread); the returned functor will take the same arguments as `Fn`.

If `context` is a `DispatcherQueue`:
- `wil::resume_foreground()` will be used if available
- without WIL, `DispatcherQueue::TryEnqueue()` will be used
- `f` will *always* be enqueued, even if it is already on the correct thread; this is so that behavior is consistent regardless of original thread, and for consistency with the behavior of `wil::resume_foreground()`

If `Context` is a `winrt::apartment_context`:
- `f` will be immediately if we're already on the correct thread; this is  because there may not be a queue available
- otherwise, it will only be invoked if the context corresponds to a thread which is running a C++/WinRT-compatible event loop