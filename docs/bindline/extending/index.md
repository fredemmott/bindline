---
title: Extending
parent: bindline
---

# Extending `FredEmmott::bindline`

You can add additional bindline components by extending
`FredEmmott::bindline_extension_api::bindable_t`:
- you *must* implement the `bind_to()` method
- you *may* override the `ordering_requirements_v` static constant

For details, see the [reference documentation for `bindable_t`](../reference/classes/bindable_t.md).

## Example

```c++
namespace TestNS {
template <int N>
class int_front_binder_t : public ::FredEmmott::bindline_extension_api::bindable_t {
public:
  template<class TFn>
  [[nodiscard]]
  constexpr auto bind_to(TFn&& fn) const {
    return std::bind_front(fn, N);
  }
};
}
```

This class can then be used in a bindline:

```c++
auto bound = fn | int_front_binder_t<123> {};
```

If you want to offer an API that is consistent with the built-in functions, you can implement a factory function that optionally takes the wrapped invocable as its first argument:

```c++
// Support `fn | bind_int_front<123>()`
template<int N>
auto bind_int_front() {
  return int_front_binder_t<N> {};
}

// Support `bind_int_front<123>(fn)`
template<int N, class TFn>
auto bind_int_front(TFn&& fn) {
  return std::forward<TFn>(fn) | bind_int_front<N>();
}
```

If it is not possible to distinguish between the two intended usages via overloads, see:
- [`bindable_t::make()`](../reference/classes/bindable_t.md#bindable_tmakettargs)
- [`bindable_t::make_projected()`](../reference/classes/bindable_t.md#bindable_tmake_projectedttprojection-targs)
