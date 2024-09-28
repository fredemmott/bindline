# Bindline

Bindline is a suite of several *header-only libraries* which are designed to avoid common mistakes with asynchronous code, e.g. event handlers and coroutines.

- `FredEmmott::bindline`: a function composition library
- `FredEmmott::weak_refs`: concepts and functional helpers for working with strong and weak references, e.g. `std::shared_ptr()` and `std::weak_ptr()`
- `FredEmmott::cppwinrt`: concepts and functional helpers for working with [C++/WinRT](https://aka.ms/cppwinrt)

These libraries can be used separately - in particular, Windows and C++/WinRT are not required - but are designed to work together:

C++20 is required, but more features are available with C++23.

For details, see [the project website](https://bindline.fredemmott.com).

## License

This project is [MIT-licensed](LICENSE).