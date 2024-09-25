---
parent: Concepts
---

# `strong_ref`

A `strong_ref`:

- is an owning-reference, i.e. a live `strong_ref` can not become a non-live `strong_ref` unless it is explicitly cleared
- can be converted to a non-owning `weak_ref`.

For example:

| `Type`               | `strong_ref<Type>` |
|----------------------|--------------------|
| `std::shared_ptr<T>` | ✅                  |
| `T*`                 | ❌                  |
| `std::weak_ptr<T>`   | ❌                  |


## Usage

```c++
#include <FredEmmott/weak_refs.hpp>

using namespace FredEmmott::weak_refs;

static_assert(strong_ref<std::shared_ptr<int>>);
static_assert(!strong_ref<std::weak_ptr<int>>);
static_assert(!strong_ref<int*>);
```