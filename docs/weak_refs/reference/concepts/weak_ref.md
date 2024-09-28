---
parent: Concepts
ancestor: weak_refs
---

# `weak_ref`

A `weak_ref`:

- is a non-owning-reference, i.e. a live `weak_ref` *can* become an expired `weak_ref` due to action at a distance
- can be converted to an owning `strong_ref`.

For example:

| `Type`               | `strong_ref<Type>` |
|----------------------|--------------------|
| `std::weak_ptr<T>`   | ✅                  |
| `std::shared_ptr<T>` | ❌                  |
| `T*`                 | ❌                  |


## Example

```c++
#include <FredEmmott/weak_refs.hpp>

using namespace FredEmmott::weak_refs;

static_assert(weak_ref<std::weak_ptr<int>>);
static_assert(!weak_ref<std::shared_ptr<int>>);
static_assert(!weak_ref<int*>);
```
