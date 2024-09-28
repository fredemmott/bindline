---
parent: Classes
ancestor: bindline
---

# `bindable_t`

`FredEmmott::bindline_extension_api::bindable_t` is the base class for all components in a `bindline`; it is used by [extensions](../../extending/index.md).

## Synopsis

```c++
namespace FredEmmott::bindline_extension_api {
struct bindable_t {
  static constexpr ordering_requirements_t ordering_requirements_v;

  template<class TFn>    
  [[nodiscard]]
  constexpr auto bind_to(TFn&& fn) const = delete;
  
  template <template <class...> class T, class TProjection, class... TArgs>
  [[nodiscard]]
  static constexpr auto make_projected(TProjection&&, TArgs&&...);
  
  template <template <class...> class T, class... TArgs>
  [[nodiscard]]
  static constexpr auto make(TArgs&&... args);
  }
};
}
```


## Example

See [the introduction to extending `bindline`](../../extending/index.md).

## Constants

### `bindable_t::ordering_requirements_v`

The ordering constraint, if any; defaults to `no_ordering_requirements`.

Valid values are:

```c++
namespace FredEmmott::bindline_extension_api {
enum class ordering_requirements_t {
  no_ordering_requirements,
  invoke_before_context_switch,
  is_context_switch,
  invoke_after_context_switch,
};
}
```

## Methods

### `bindable_t::bind_to(TFn&&)`

Returns a new function that wraps the parameter with the desired behavior. For example, `bindline::bind_front()` returns the result of calling `std::bind_front()` with the stored parameters.

### `bindable_t::make<T>(TArgs&&...)`

A convenience wrapper around [`make_projected()`](#bindable_tmake_projectedttprojection-targs), using `std::identity` as the projection.

### `bindable_t::make_projected<T>(TProjection&&, TArgs&&...)`

Returns an object that is both a functor and a bindline component, wrapping the `bindable_t` variadic template class `T`.

This is useful if the intended usage (`fn | some_bindable(args...)` vs `some_bindable(fn, args...)`) can not be inferred from the number of arguments or their types, such as when a function is both potentially a valid argument, and a valid left hand side of a bindline.

For example, [`bind_refs_front()`](../functions/bind_refs_front) is implemented with `make_projected()`, with a projection that calls [`weak_refs::make_weak_ref()`](../../../weak_refs/reference/functions/make_weak_ref.md).