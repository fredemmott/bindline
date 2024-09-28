# Bindline

Bindline is a suite of C++20/23 header-only libraries for safer asynchronous code, such as event handlers, other callbacks, and coroutines.

- `FredEmmott::bindline`: a library for composing functions
- `FredEmmott::weak_refs`: concepts and functional helpers for working with strong and weak references, e.g. `std::shared_ptr()` and `std::weak_ptr()`
- `FredEmmott::cppwinrt`: concepts and functional helpers for working with [C++/WinRT](https://aka.ms/cppwinrt)

These libraries can be used separately - in particular, Windows and C++/WinRT are not required - but are designed to work together:

C++20 is required, but more features are available with C++23.

For more information, see [the project website](https://bindline.fredemmott.com).

## Example

Event handlers, callbacks, and asynchronously-called code often have repetitive boilerplate for safety, or to fit a given signature; bindline aims to make this less error-prone and more easily-customizable:

```c++
auto f = &MyClass::some_func
  // Store an `std::shared_ptr` or `winrt::weak_ref`, but when invoking:
  // 1. convert back to a strong ref
  // 2. return without invoking if the ref is stale (i.e. `nullptr`)
  | bind_refs_front(this)
  // Automatically switch thread when invoked
  | bind_winrt_context(winrt_apartment_context_or_dispatcher_queue())
  // if some_func() takes N parameters, require invocing with N + 2
  // parameters; this can be useful for event handlers
  | drop_n_back<2>();

f(args...);
```

## License

This project is [MIT-licensed](LICENSE).