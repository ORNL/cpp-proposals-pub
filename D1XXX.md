---
title: "`mdarray`: An Owning Multidimensional Array Analog of `mdspan`"
document: D1XXXR0
date: 2019-05-28
audience: Library Evolution
author:
  - name: David Hollman
    email: <dshollm@sandia.gov>
  - name: Christian Trott
    email: <crtrott@sandia.gov>
  - name: Mark Hoemmen
    email: <mhoemme@sandia.gov>
  - name: Daniel Sunderland
    email: <dsunder@sandia.gov>
---

# Motivation

[@P0009R9] introduced a non-owning multidimensional array abstraction that has been refined over many revisions and is expected to be merged into the C++ working draft early in the C++23 cycle.  However, there are many owning use cases where `mdspan` is not ideal.  In particular, for use cases with small, fixed-size dimensions, the non-owning semantics of `mdspan` may represent a significant pessimization, precluding optimizations that arise from the removal of the non-owning indirection (such as storing the data in registers).  

# Design

One major goal of the design for `mdarray` is to parallel the design of `mdspan` as much as possible (but no more), with the goals of reducing cognitive load for users already familiar with `mdspan` and of incorporating the lessons learned from the half decade of experience on [@P0009R9].

## Design Overview

In brief, the analogy to `basic_mdspan` can be seen in the declaration of the proposed design for `basic_mdarray`:

```cpp
template<class ElementType,
         class Extents,
         class LayoutPolicy = layout_right,
         class ContainerPolicy = @_see-below_@>
  class basic_mdarray;
```

This intentionally parallels the design of `basic_mdspan` in [@P0009R9], which has the signature:

```cpp
template<class ElementType,
         class Extents,
         class LayoutPolicy = layout_right,
         class AccessorPolicy = accessor_basic<ElementType>>
  class basic_mdspan;
```

The details of this design are included below, along with the accompanying logic and an exploration of alternative designs.

## `Extents` Design Reused

As with `basic_mdspan`, the `Extents` template parameter to `basic_mdarray` shall be a template instantiation of `std::extents`, as described in [@P0009R9].  The concerns addressed by this aspect of the design are exactly the same in `basic_mdarray` and `basic_mdspan`, so using the same form and mechanism seems like the right thing to do here.

## `LayoutPolicy` Design Reused

While not quite as straightforward, the decision to use the same design for `LayoutPolicy` from `basic_mdspan` in `basic_mdarray` is still quite obviously the best choice.  The only piece that's a bit of a less perfect fit is the `is_contiguous()` and `is_always_contiguous()` requirements.  While non-contiguous use cases for `basic_mdspan` are quite common (e.g., `subspan()`), non-contiguous use cases for `basic_mdarray` are expected to be a bit more arcane.  Nonetheless, reasonable use cases do exist (for instance, padding of the fast-running dimension in anticipation of a resize operation), and the reduction in cognitive load due to concept reuse certainly justifies reusing `LayoutPolicy` for `basic_mdarray`.

## `AccessorPolicy` Replaced by `ContainerPolicy`

By far the most complicated aspect of the design for `basic_mdarray` is the analog of the `AccessorPolicy` in `basic_mdspan`.  The `AccessorPolicy` for `basic_mdspan` is clearly designed with non-owning semantics in mind--it provides a `pointer` type, a `reference` type, and a means of converting from a pointer to a 







