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
         class ContainerPolicy = @*see-below*@>
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

The details of this design are included below, along with the accompanying logic and an exploration of alternative designs.  In a manner exactly analogous to `mdspan`, we also propose the convenience type alias template `mdarray`, defined as:

```cpp
template <class T, ptrdiff_t... Extents>
  using mdarray = basic_mdarray<T, extents<Extents...>>;
```

## `Extents` Design Reused

As with `basic_mdspan`, the `Extents` template parameter to `basic_mdarray` shall be a template instantiation of `std::extents`, as described in [@P0009R9].  The concerns addressed by this aspect of the design are exactly the same in `basic_mdarray` and `basic_mdspan`, so using the same form and mechanism seems like the right thing to do here.

## `LayoutPolicy` Design Reused

While not quite as straightforward, the decision to use the same design for `LayoutPolicy` from `basic_mdspan` in `basic_mdarray` is still quite obviously the best choice.  The only piece that's a bit of a less perfect fit is the `is_contiguous()` and `is_always_contiguous()` requirements.  While non-contiguous use cases for `basic_mdspan` are quite common (e.g., `subspan()`), non-contiguous use cases for `basic_mdarray` are expected to be a bit more arcane.  Nonetheless, reasonable use cases do exist (for instance, padding of the fast-running dimension in anticipation of a resize operation), and the reduction in cognitive load due to concept reuse certainly justifies reusing `LayoutPolicy` for `basic_mdarray`.

## `AccessorPolicy` Replaced by `ContainerPolicy`

By far the most complicated aspect of the design for `basic_mdarray` is the analog of the `AccessorPolicy` in `basic_mdspan`.  The `AccessorPolicy` for `basic_mdspan` is clearly designed with non-owning semantics in mind--it provides a `pointer` type, a `reference` type, and a means of converting from a pointer and an offset to a reference.  Beyond the lack of an allocation mechanism (that would be needed by `basic_mdarray`), the `AccessorPolicy` requirements address concerns normally addressed by the allocation mechanism itself.  For instance, the C++ named requirements for `Allocator` allow for the provision of the `pointer` type to `std::vector` and other containers.  Arguably, consistency between `basic_mdarray` and standard library containers is far more important than with `basic_mdspan` in this respect.  Several approaches to addressing this incongruity are discussed below.

### Expected Behavior of Motivating Use Cases

Regardless of the form of the solution, there are several use cases where we have a clear understanding of how we want them to work.  As alluded to above, perhaps *the* most important motivating use case for `mdarray` is that of small, fixed-size extents.  Consider a fictitious (not proposed) function, *get-underlying-container*, that somehow retrieves the underlying storage of an `mdarray`.  For an `mdarray` of entirely fixed sizes, we would expect the default implementation to return something that is (at the very least) convertible to `array` of the correct size:

```cpp
auto a = mdarray<int, 3, 3>;
std::array<int, 9> data = @*get-underlying-container*@(a); 
```

(Whether or not a reference to the underlying container should be obtainable is slightly less clear, though we see no reason why this should not be allowed.)  The default for an `mdarray` with variable extents is only slightly less clear, though it should almost certainly meet the requirements of *contiguous container* (**[container.requirements.general]**/13).  The default model for *contiguous container* in the standard library is `vector`, so an entirely reasonable outcome would be to have:

```cpp
auto a = mdarray<int, 3, dynamic_extent>;
std::vector<int> data = @*get-underlying-container*@(a); 
```

TODO finish this

### Analogs in the Standard Library:  Container Adapters

Perhaps the best analogs for what `basic_mdarray` is doing with respect to allocation and ownership are the container adaptors (**[container.adaptors]**), since they imbue additional semantics to what is otherwise an ordinary container.  These all take a `Container` template parameter, which defaults to `deque` for `stack` and `queue`, and to `vector` for `priority_queue`.  The allocation concern is thus delegated to the container concept, reducing the cognitive load associated with the design.  While this design approach overconstrains the template parameter slightly (i.e., not all of the requirements of the `Container` concept are needed by the container adaptors), the simplicity arising from concept reuse more than justifies the cost of the extra constraints.

It is difficult to say whether the use of `Container` directly, as with the container adaptors, is also the correct approach for `basic_mdarray`.  There are pieces of information that may need to be customized in some very reasonable use cases that are not provided by the standard container concept.  The most important of these is the ability to produce a semantically consistent `AccessorPolicy` when creating a `basic_mdspan` that refers to a `basic_mdarray`.  (Interoperability between `basic_mdspan` and `basic_mdarray` is considered a critical design requirement because of the nearly complete overlap in the set of algorithms that operate on them.)  For instance, given a `Container` instance `c` and an `AccessorPolicy` instance `a`, the behavior of `a.access(p, n)` should be consistent with the behavior of `c[n]` for a `basic_mdspan` wrapping `a` that is a view of a `basic_mdarray` wrapping `c` (if `p` is `c.begin()`).  But because `c[n]` is part of the container requirements and thus may encapsulate any arbitrary mapping from an offset of `c.begin()` to a reference, the only reasonable means of preserving these semantics is to reference the original container directly in the corresponding `AccessorPolicy`.  In other words, the signature for the `view()` method of `mdarray` would need to look something like (ignoring, for the moment, whether the name for the type of the accessor is specified or implementation-defined):

```cpp
template<class ElementType,
         class Extents,
         class LayoutPolicy,
         class Container>
struct basic_mdarray {
 /* ... */
 basic_mdspan<
   ElementType, Extents, LayoutPolicy,
   container_reference_accessor<Container>>
 view() const noexcept;
 /* ... */
};

template <class Container>
struct container_reference_accessor {
  using pointer = Container*;
  /* ... */
  template <class Integer>
  reference access(pointer p, Integer offset) {
    return (*p)[offset];
  }
  /* ... */
};
```

But this approach comes at the cost of an additional indirection (one for the pointer to the container, and one for the container dereference itself), which is likely unacceptable cost in a facility designed to target performance-sensitive use cases.  The situation for the `offset` requirement (which is used by `subspan`) is potentially even worse for arbitrary non-contiguous containers, adding up to one indirection per invocation of `subspan`.





