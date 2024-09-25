---
parent: Concepts
---

# `convertible_to_weak_ref`

A `convertible_to_weak_ref` is anything that can be converted to a `weak_ref`; this includes:
- a [`weak_ref`](weak_ref.md)
- a [`strong_ref`](strong_ref.md)
- `T*` if `T` extends `std::enable_shared_from_this<T>`

## Usage

```c++
#include <FredEmmott/weak_refs.hpp>

using namespace FredEmmott::weak_refs;

static_assert(convertible_to_weak_ref<std::shared_ptr<int>>);
static_assert(convertible_to_weak_ref<std::weak_ptr<int>>);
static_assert(!convertible_to_weak_ref<int*>);
```
