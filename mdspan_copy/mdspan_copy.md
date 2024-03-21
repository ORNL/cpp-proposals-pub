---
title: "Copy and fill for `mdspan`"
date: today
---

# Motivation

C++23 introduced `mdspan` ([@P0009R18]), a nonowning multidmensional array abstraction that has a customizable layout. Layout customization was originally motivated in [@P0009R18] with considerations for interoperability and performance, particularly on different architectures. Moreover, [@P2630R4] introduced `submdspan`, a slicing function that can yield arbitrarily strided layouts. Without standard library support, copying efficiently between mdspans with mixes of complex layouts is challenging for users.

Many applications, including high-performance computing (HPC), image processing, computer graphics, etc that benefit from `mdspan` also would benefit from basic memory operations provided in standard algorithms such as copy and fill. Indeed, the authors found that a copy algorithm would have been quite useful in their implementation of the copying `mdarray` ([@P1684]) constructor.

However, existing standard library facilities are not sufficient here. Currently, `mdspan` does not have iterators or ranges that represent the span of the `mdspan`. Additionally, it's not entirely clear what this would entail.

Moreover, the manner in which an `mdspan` is copied (or filled) is highly performance sensitive, particularly in regards to caching behavior when traversing mdspan memory. A naïve user implementation is easy to get wrong in addition to being tedious for higher rank `mdspan`s. Ideally, an implementation would be free to use information about the layout of the `mdspan` known at compile time to perform optimizations; e.g. a continuous span `mdspan` copy for trivial types could be implementeed with a `memcpy`.

Finally, providing these generic algorithms would also enable these operations for types that are representable by `mdspan`. For example, this would naturally include `mdarray`, which is convertible to `mdspan`, or for user-defined types whose view of memory corresponds to `mdspans` (e.g. an image class or something similar).

## Safety

Due to the closed nature of `mdspan` extents, copy operations can be checked by the implementation to prevent past-the-end writes. This is an advantage the proposed copy operation has over the existing operations in the standard.

# Design

The main design direction of this proposal is to provide methods for copying and filling `mdspan`s that may have differing layouts and accessors, while allowing implementations to provide efficient implementations for special cases. For example, if a copy occurs between two `mdspan`s with the same layout mapping type that is contiguous and both use `default_accessor`, the intention is that this could be implemented by a single `memcpy`.

Furthermore, accessors as a customization point should be enabled, as with any other `mdspan` operation. For example, a custom accessor that checks a condition inside of the `access` method should still work and check that condition. It's worth noting that there may be a high sensitivity of how much implementations able to optimize if provided custom accessors. For example, optimizations could be disabled if using a custom accessor that is identical to the default accessor.

Finally, there is some question as to whether `copy` and `fill` should return a value when applied to `mdspan`, as the iterator and ranged-based algorithms do. We believe that `mdspan` copy and fill should return void, as there is no past-the-end iterator that they could reasonable return.

## What the proposal does not include

* `std::move`: Perhaps this should be included for completeness's sake. However, it doesn't seem applicable to the typical usage of `mdspan`.
* `(copy|fill)_n`: As a multidimensional view `mdspan` does not in general follow a specific ordering. Memory ordering may not be obvious to calling code, so it's not even clear how these would work. Any applications intending to copy a subset of `mdspan` should use call `copy` on the result of `submdspan`.
* `copy_backward`: As above, there is no specific ordering. A similar effect could be achieved via transformations with a custom layout, similar to `layout_transpose` in [@P1673].
* Other algorithms, include `std::for_each`. `for_each` in particular is a desirable but brings in many unanswered questions that should be addressed in a different paper.

# Wording

```c++
template<class SrcElementType, class SrcExtents, class SrcLayoutPolicy, class SrcAccessorPolicy,
         class DstElementType, class DstExtents, class DstLayoutPolicy, class DstAccessorPolicy>
void copy(const mdspan<SrcElementType, SrcExtents, SrcLayoutPolicy, SrcAccessorPolicy> &src, const mdspan<DstElementType, DstExtents, DstLayoutPolicy, DstAccessorPolicy> &dst)

template<class ExecutionPolicy, class SrcElementType, class SrcExtents, class SrcLayoutPolicy, class SrcAccessorPolicy,
         class DstElementType, class DstExtents, class DstLayoutPolicy, class DstAccessorPolicy>
void copy(ExecutionPolicy&& policy, const mdspan<SrcElementType, SrcExtents, SrcLayoutPolicy, SrcAccessorPolicy> &src, const mdspan<DstElementType, DstExtents, DstLayoutPolicy, DstAccessorPolicy> &dst)
```

[1]{.pnum} *Constraints:*

  * [1.1]{.pnum} `std::is_assignable_v<typename mdspan<SrcElementType, SrcExtents, SrcLayoutPolicy, SrcAccessorPolicy>::reference, typename mdspan<DstElementType, DstExtents, DstLayoutPolicy, DstAccessorPolicy>::reference>`

  * [1.2]{.pnum} `mdspan<SrcElementType, SrcExtents, SrcLayoutPolicy, SrcAccessorPolicy>::rank() == mdspan<DstElementType, DstExtents, DstLayoutPolicy, DstAccessorPolicy>::rank()`

[2]{.pnum} *Preconditions:*

  * [2.1]{.pnum} `src.extent(r) <= dst.extent(r)` for every rank index `r` of `dst`

  * [2.2]{.pnum} `dst.is_unique()`

  * [2.3]{.pnum} there is no unique multidimensional index `i...` in `src.extents()`, and no unique multidimensional index `j...` in `dst.extents()` such that `src.accessor().offset(src.data_handle(), src.mapping(i...)) == dst.accessor().offset(dst.data_handle(), dst.mapping(j...))`

[3]{.pnum} *Effects:* for all unique multidimensional indices `i...` in `src.extents()`, assigns `src[i...]` to `dst[i...]`


```c++
template<class ElementType, class Extents, class LayoutPolicy, class AccessorPolicy, class T>
void fill(const mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy> &dst, const T &value)

template<class ExecutionPolicy, class ElementType, class Extents, class LayoutPolicy, class AccessorPolicy, class T>
void fill(ExecutionPolicy&& policy, const mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy> &dst, const T &value)
```

[4]{.pnum} *Constraints:* `std::is_assignable_v<typename mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>::reference, const T &T>`

[5]{.pnum} *Preconditions:* `dst.is_unique()`

[6]{.pnum} *Effects:* for all unique multidimensional indices `i...` in `dst.extents()`, assigns `value` to `dst[i...]`
