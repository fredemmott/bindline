---
title: Extending
parent: weak_refs
---

# Extending `FredEmmott::weak_refs`

`FredEmmott::weak_refs` can be extended to support additional types; there are two ways to do this:

- if you control the namespace that the type is in, [use argument-dependent lookup (ADL)](adl.md)
- otherwise, [create an extension](extensions)