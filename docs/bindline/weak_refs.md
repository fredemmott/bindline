---
title: weak_refs
parent: bindline
---

# Using `bindline` with `weak_refs`

`bindline` provides two key functions that wrap `weak_ref`'s functionality:

- [`FredEmmott::bindline::bind_refs_front()`](reference/functions/bind_refs_front.md)
- [`FredEmmott::bindline::bind_refs_back()`](reference/functions/bind_refs_back.md)

These behave similarly to the functions in `FredEmmott::weak_refs()`, however like all 'bindables' in bindline, they can optionally be composed in a pipeline:

```c++
using namespace FredEmmott::bindline;
// Create a functor that stores a weak_ref to `ref`, converts it to a strong_ref, then passes it to `f`
auto fn1 = bind_refs_front(f, ref);
fn1();
// Do the same thing but with a bind pipeline (bindline):
auto fn2 = f | bind_refs_front(ref);
fn2();
```

Most users will just want `using namespace FredEmmott::bindline;`. As `FredEmmott::weak_refs` contains identically named functions, if you wish to use other components of `weak_refs` such as the type aliases or concepts, you can either use qualified names, or pull in the sub-namespaces of `weak_refs`, e.g.:

```c++
// Pull in all of bindline
using namespace FredEmmott::bindline;
// Explicitly pull in some specific features from weak_refs; don't pull in the whole library,
// as we'll then have ambiguous function references for `bind_refs_front()` and `bind_refs_back()`
using namespace FredEmmott::weak_refs::weak_refs_concepts;
using namespace FredEmmott::weak_refs::weak_refs_ref_types;
using namespace FredEmmott::weak_refs::weak_refs_reseaters;
```

For details see:
- [the bindline introduction](index.md)
- [the `weak_ref` library documentation](../weak_refs/index.md)
- [`FredEmmott::weak_refs::bind_refs_front()`](../weak_refs/reference/functions/bind_refs_front.md)
- [`FredEmmott::weak_refs::bind_refs_back()`](../weak_refs/reference/functions/bind_refs_back.md)
