---
parent: Classes
---

# `strong_ref_reseater`

`strong_ref_reseater` is a class defined in the `FredEmmott::weak_refs::weak_refs_reseaters` namespace, which is inlined
into the `FredEmmott::weak_refs` namespace. This means that it is usable with either:

```c++
// Pull in the whole library...
use namespace FredEmmott::weak_refs;
// ... or just the reseater classes:
use namespace FredEmmott::weak_refs::weak_refs_reseaters;
```

## Synopsis

```c++
template<strong_ref... Ts>
class strong_ref_reseater {
  strong_ref_reseater(Ts*...);
  ~strong_ref_reseater();
  
  [[nodiscard]] bool reseat();
 
 private: 
  // ...
};
```

Releases `strong_refs` (by assigning `nullptr`), and attempts to restore them when `reseat()` is called, or the reseater
is destroyed.

**WARNING**: if any of the stored `weak_ref`s expired before `reseat()` or destruction, the `strong_refs` will be
invalid after `reseat()` and destruction.

## Example usage

```c++
auto self = shared_from_this();
self->foo();
{
  strong_ref_reseater reseater(&self);
  co_await resume_after(std::chrono::seconds(1));
  if (!reseater.reseat()) {
    // `std::shared_ptr`'s refcount hit zero during that second
    co_return;
  }
}
// As `reseat()` returned true, `self` is definitely valid.
self->bar();
```

## Methods

### `strong_ref_reseater::strong_ref_reseater(Ts*...)`

- calls [`make_weak_ref()`](../functions/make_weak_ref.md) on each element in the parameter pack
- releases each `strong_ref` (by assigning `nullptr`)
- stores both the `weak_ref` and the pointer

### `strong_ref_reseater::~strong_ref_reseater()`

Calls [`reseat()`](#strong_ref_reseaterrelease) if it has not already been called. Some of the `strong_ref`s may be
invalid after, if the stored `weak_ref`s expired during the reseater's lifetime.

### `strong_ref_reseater::reseat()`

Calls [`lock_weak_ref()`](../functions/lock_weak_ref.md) on each stored `weak_ref`, and assigns back to the shared
`strong_ref*`s.

Returns true if all of the `strong_ref`s are valid; for example, it will return false if any of the stored `weak_refs`
expired between the reseater being created and `reseat()` being called.