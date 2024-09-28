---
parent: Functions
ancestor: bindline
---

# `bind_suppress_validation()`

```c++
// Returns a bindline
[[nodiscard]]
constexpr auto bind_suppress_validation(auto&& component);
```

This function returns a bindline component that wraps another component, disabling validation.

## Example

```c++
/* This is an error:
 *
 * functions on the right wrap functions on the left, and you probably want to
 * promote the weak ref to a strong ref _after_ switching threads:
 */
f | bind_winrt_context(ctx) | bind_refs_front(this);

// Nope, I know what I'm doing:
f | bind_winrt_context(ctx) | bind_suppress_validation(bind_refs_front(this));
```