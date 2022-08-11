
---
title: "`Submdspan`"
document: PXXXX
date: today
audience: LEWG
author:
  - name: Christian Trott 
    email: <crtrott@sandia.gov>
  - name: Damien Lebrun-Grandie 
    email: <lebrungrandt@ornl.gov>
  - name: Mark Hoemmen 
    email: <mhoemmen@stellarscience.com>
toc: true
---


# Revision History
## Initial Version 2022-08 Mailing


# Description


Until one of the last revisions, the `mdspan` paper P0009 contained `submdspan`,
the subspan or "slicing" function
that returns a view of a subset of an existing `mdspan`.
This function was considered critical for the overall functionality of `mdspan`.
However, due to review time constraints it was removed from P0009 in order for
`mdspan` to be included in C++23. 

This paper restores `submdspan`.  It also expands on the original proposal by

* defining customization points so that `submdspan` can work
    with user-defined layout policies, and
* adding the ability to specify slices as compile-time values.

Creating subspans is an integral capability of many, if not all programming languages
with multi-dimensional arrays.  These include Fortran, Matlab, Python, and Python's NumPy extension.

Subspans are important because they enable code reuse.
For example, the inner loop in a dense matrix-vector product
actually represents a *dot product* -- an inner product of two vectors.
If one already has a function for such an inner product,
then a natural implementation would simply reuse that function.
The LAPACK linear algebra library depends on subspan reuse
for the performance of its one-sided "blocked" matrix factorizations
(Cholesky, LU, and QR).
These factorizations reuse textbook non-blocked algorithms
by calling them on groups of contiguous columns at a time.
This lets LAPACK spend as much time in dense matrix-matrix multiply
(or algorithms with analogous performance) as possible.

The following code demonstrates this code reuse feature of subspans.
Given a rank-3 `mdspan` representing a three-dimensional grid
of regularly spaced points in a rectangular prism,
it sets all elements on the surface of the 3-dimensional shape to zero.
It does so by reusing a function `zero_2d`.

```c++
// zero out all elements in an mdspan
template<class T, class E, class L, class A>
void zero_2d(mdspan<T,E,L,A> a) {
  static_assert(a.rank() == 2);
  for(int i=0; i<a.extent(0); i++)
    for(int j=0; j<a.extent(1); j++)
      a(i,j) = 0;
}

// zero out just the surface
template<class T, class E, class L, class A>
void zero_surface(mdspan<T,E,L,A> a) {
  static_assert(a.rank() == 3);
  zero_2d(submdspan(a, 0, full_extent, full_extent));
  zero_2d(submdspan(a, full_extent, 0, full_extent));
  zero_2d(submdspan(a, full_extent, full_extent, 0));
  zero_2d(submdspan(a, a.extent(0)-1, full_extent, full_extent));
  zero_2d(submdspan(a, full_extent, a.extent(1)-1, full_extent));
  zero_2d(submdspan(a, full_extent, full_extent, a.extent(2)-1));
}
```

## Design of `submdspan`

As previously proposed in an earlier revision of P0009, `submdspan` is a free function.
Its first parameter is an `mdspan` `x`,
and the remaining `x.rank()` parameters are slice specifiers,
one for each dimension of `x`.
The slice specifiers describe which elements of the range $[0,$`x.extent(d)`$)$ are part of the
multidimensional index space of the returned `mdspan`.

This leads to the following fundamental signature:

```c++
template<class T, class E, class L, class A,
         class ... SliceArgs)
auto submdspan(mdspan<T,E,L,A> x, SliceArgs ... args);
```

where `E.rank()` must be equal to `sizeof...(SliceArgs)`.


### Slice Specifiers

In P0009 we originally proposed three kinds of slice specifiers.

* A single integral value.  For each integral slice specifier given to `submdspan`,
    the rank of the resulting `mdspan` is one less than the rank of the input `mdspan`.
    The resulting multi-dimensional index space contains only elements of the
    original index space, where the particular index matches this slice specifier.
* Anything convertible to a `tuple<mdspan::index_type, mdspan::index_type>`. The resulting multi-dimensional index space
covers the begin-to-end subrange of elements in the original index space described by the `tuple`'s two values.
* An instance of the tag class `full_extent_t`.
    This includes the full range of indices in that extent in the returned subspan.

We propose here an additional kind of slice specifier,
the type `strided_index_range` for taking a strided range in an extent.
This type takes three values:

* the start index,
* the length of the subrange (*not* the end index),
* and a stride within that subrange.

This type uses the length instead of the end index,
so that the returned subspan can have a compile-time extent value,
even if it starts at a run-time offset value.
We use a struct with named fields instead of a `tuple`,
in order to avoid confusion with the order of the three values.

We also propose that any integral value
(on its own, in a `tuple`, or in `strided_index_range`)
can be specified as an `integral_constant`.
This ensures that the value can be baked into the return *type* of `submdspan`.
For example, layout mappings could be entirely compile time. 

Here are some simple examples for rank-1 `mdspan`.

```c++
int* ptr = ...;
int N = ...;
mdspan a(ptr, N);

// subspan of a single element
auto a_sub1 = submdspan(a, 1);
static_assert(decltype(a_sub1)::rank() == 0);
assert(&a_sub1() == &a(1));

// subrange
auto a_sub2 = submdspan(a, tuple{1, 4});
static_assert(decltype(a_sub2)::rank() == 1);
assert(&a_sub2(0) == &a(1));
assert(a_sub2.extent(0) == 3);

// subrange with stride
auto a_sub3 = submdspan(a, strided_index_range{1, 7, 2});
static_assert(decltype(a_sub3)::rank() == 1);
assert(&a_sub3(0) == &a(1));
assert(&a_sub3(3) == &a(7));
assert(a_sub3.extent(0) == 4);

// full range
auto a_sub4 = submdspan(a, full_extent);
static_assert(decltype(a_sub4)::rank() == 1);
assert(a_sub4(0) == a(0));
assert(a_sub4.extent(0) == a.extent(0));
```

In multi-dimensional use-cases these specifiers can be matched and mixed.

```c++
int* ptr = ...;
int N0 = ..., N1 = ..., N2 = ..., N3 = ..., N4 = ...;
mdspan a(ptr, N0, N1, N2, N3, N4);

auto a_sub = submdspan(a,full_extent_t(), 3, strided_index_range{2,N2-5, 2}, 4, tuple{3, N5-5});

// two integral specifiers so the rank is reduced by 2
static_assert(decltype(a_sub) == 3);
// 1st dimension is taking the whole extent
assert(a_sub.extent(0) == a.extent(0));
// the new 2nd dimension corresponds to the old 3rd dimension
assert(a_sub.extent(1) == (a.extent(2) - 5)/2);
assert(a_sub.stride(1) == a.stride(2)*2);
// the new 3rd dimension corresponds to the old 5th dimension
assert(a_sub.extent(2) == a.extent(4)-8);

assert(&a_sub(1,5,7) == &a(1, 3, 2+5*2, 4, 3+7));
```

### Customization Points

In order to create the new `mdspan` from an existing `mdspan` `src`, we need three things:

* the new mapping `sub_map`,

* the new accessor `sub_acc`, and

* the new data handle `sub_handle`.

Computing the new data handle is done via an *offset* and the original accessor's `offset` function,
while the new accessor is constructed from the old accessor.

That leaves the construction of the new mapping and the calculation of the *offset* handed to the `offset` function.
Both of those operations depend only on the old mapping and the slice specifiers.

In order to support calling `submdspan` on `mdspan` with custom layout policies, we need to introduce two customization
points for computing the mapping and the offset. Both take as input the original mapping, and the slice specifiers.

```c++
template<class Mapping, class ... SliceArgs>
auto submdspan_mapping(const Mapping&, SliceArgs...) { /* ... */ }

template<class Mapping, class ... SliceArgs>
size_t submdspan_offset(const Mapping&, SliceArgs...) { /* ... */ }
```

With these components we can sketch out the implementation of `submdspan`.

```c++
template<class T, class E, class L, class A,
         class ... SliceArgs)
auto submdspan(const mdspan<T,E,L,A>& src, SliceArgs ... args) {
  size_t sub_offset = submdspan_offset(src.mapping(), args...);
  auto sub_map = submdspan_mapping(src.mapping(), args...);
  typename A::offset_policy sub_acc(src.accessor());
  typename A::offset_policy::data_handle_type 
    sub_handle = src.accessor().offset(src.data_handle(), offset);
  return mdspan(sub_handle, sub_map, sub_acc);
}
```

To support custom layouts, `std::submdspan` calls `submdspan_offset` and `submdspan_mapping` using argument-dependent lookup.

However, not all layout mappings may support efficient slicing for all possible slice specifier combinations.
Thus, we do *not* propose to add these customization points to the layout policy requirements. 

### Making sure submdspan behavior meets expectations

The slice specifiers of `submdspan` completely determine two properties of its result:

1. the `extents` of the result, and
2. what elements of the input of `submdspan` are also represented in the result.

Both of these things are independent of the layout mapping policy.

The approach we described above orthogonalizes handling of accessors and mappings.
Thus, we can define both of the above properties via the multidimensional index spaces,
regardless of what it means to "refer to the same element."
(For example, accessors may use proxy references and data handle types other than C++ pointers.
This makes it hard to define what "same element" means.)
That will let us define pre-conditions for `submdspan` which specify the required behavior of any user-provided `submdspan_mapping`
and `submdspan_offset` function.

One function which can help with that, and additionally is needed to implement `submdspan_mapping` and `submdspan_offset` for
the layouts the standard provides, is a function to compute the submdspan's `extents`.
We will propose this function as a public function in the standard:

```c++
template<class IndexType, class ... Extents, class ... SliceArgs>
auto submdspan_extents(const extents<IndexType, Extents...>, SliceArgs ...);
```

The resulting `extents` object must have certain properties for logical correctness.

* The rank of the sub-extents is the rank of the original `extents` minus the number of pure integral arguments in `SliceArgs`.

* The extent of each remaining dimension is well defined by the `SliceArgs`.  It is the original extent if all the `SliceArgs` are `full_extent_t`.

For performance and preservation of compile-time knowledge, we also require the following.

* Any `full_extent_t` argument corresponding to a static extent, preserves that static extent.

* Generate a static extent when possible.  For example, providing a `tuple` of `integral_constant` as a slice specifier ensures that the corresponding extent is static.

# Wording


```c++
```

## Replace subsection 22.7.X [mdspan.submdspan] with the following

<b>24.7.� submdspan [mdspan.submdspan]</b>

<b>24.7.�.1 overview [mdspan.submdspan.overview]</b>

[1]{.pnum} The `submdspan` facilities create a new `mdspan` from an existing one,
   whose elements refer to a subset of the input `mdspan`.

```c++
namespace std {
  template<class BeginType, class LengthType, class StrideType>
    class strided_index_range;

  template<class IndexType, class... Extents, class... SliceSpecifiers>
    constexpr auto submdspan_extents(const extents<IndexType, Extents...>&,
                                     SliceSpecifiers ...);

  template<class E, class... SliceSpecifiers>
    constexpr auto submdspan_mapping(
      const layout_left::mapping<E>& src, 
      SliceSpecifiers ... slices) -> @_see below_@;

  template<class E, class... SliceSpecifiers>
    constexpr auto submdspan_mapping(
      const layout_right::mapping<E>& src, 
      SliceSpecifiers ... slices) -> @_see below_@;

  template<class E, class... SliceSpecifiers>
    constexpr auto submdspan_mapping(
      const layout_stride::mapping<E>& src, 
      SliceSpecifiers ... slices) -> @_see below_@;

  template<class E, class... SliceSpecifiers>
    constexpr size_t submdspan_offset(
      const layout_left::mapping<E>& src, 
      SliceSpecifiers ... slices);

  template<class E, class... SliceSpecifiers>
    constexpr size_t submdspan_offset(
      const layout_right::mapping<E>& src, 
      SliceSpecifiers ... slices);

  template<class E, class... SliceSpecifiers>
    constexpr size_t submdspan_offset(
      const layout_stride::mapping<E>& src, 
      SliceSpecifiers ... slices);

  // [mdspan.submdspan], submdspan creation
  template<class ElementType, class Extents, class LayoutPolicy,
           class AccessorPolicy, class... SliceSpecifiers>
    constexpr auto submdspan(
      const mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>& src,
      SliceSpecifiers...slices) -> @_see below_@;
}
```


[2]{.pnum} The `SliceSpecifier` template argument(s)
   and the corresponding value(s) of the arguments of `submdspan` after `src`
   determine the subset of `src` that the `mdspan` returned by `submdspan` views.

[3]{.pnum} Each `submdspan` overload specified in this subclause calls `submdspan_mapping` and `submdspan_offset` unqualified, so as to enable argument dependent lookup ([basic.lookup.argdep]).

<b>24.7.�.3 exposition only helpers [mdspan.submdspan.helpers]</b>

```c++
template<class T>
struct @_is-strided-index-range_@: false_type {};

template<class OffsetType, class ExtentType, class StrideType>
struct @_is-strided-index-range_@<strided_index_range<OffsetType, ExtentType, StrideType>>: true_type {];
```

[1]{.pnum} For each function defined in subsection [mdspan.submdspan] that takes a parameter pack named `slices` as an argument

  * [1.1]{.pnum} let `rank` be the number of elements in `slices`,

  * [1.1]{.pnum} let $s_k$ be the $k$-th element of `slices`,
  
  * [1.1]{.pnum} let $S_k$ be the type of the $s_k$, and

  * [1.1]{.pnum} let  _`map-rank`_ be an `array<size_t, rank>` such that for each `k` in the range of $[0,$ `rank`$)$ _`map-rank`_`[k]` equals:

    * `dynamic_extent` if `is_convertible_v<`$S_k$`, size_t>` is `true`, or else

    * the number of $S_j$ with $j < k$ such that `is_convertible_v<`$S_j$`, size_t>` is `false`.



```c++
template<class ... SliceSpecifiers>
size_t @_first_@_(size_t i, SliceSpecifiers... slices);
```

[2]{.pnum} *Returns:*

   * [2.1]{.pnum} if `is_convertible_v<`$S_r$`, size_t>` is `true`, $s_r$;

   * [2.2]{.pnum} otherwise, if `is_convertible_v<`$S_r$`, tuple<size_t, size_t>>` is `true`,
                  `get<0>(t)`, where `t` is the result of converting $s_r$ to `tuple<size_t, size_t>`;

   * [2.3]{.pnum} otherwise, if _`is-strided-index-range`_`<`$S_r$`>::value` is `true`, $s_r$`.offset()`;

   * [2.4]{.pnum} otherwise, `0`.

```c++
template<class Extents, class ... SliceSpecifiers>
size_t @_last_@_(size_t i, Extents& ext, SliceSpecifiers... slices);
```

[3]{.pnum} *Returns:*

   * [3.1]{.pnum} if `is_convertible_v<`$S_r$`, size_t>` is `true`, $s_r$ + 1,

   * [3.2]{.pnum} otherwise, if `is_convertible_v<`$S_r$`, tuple<size_t, size_t>>` is `true`,
                  `get<1>(t)`, where `t` is the result of converting $s_r$ to `tuple<size_t, size_t>`;

   * [3.3]{.pnum} otherwise, if _`is-strided-index-range`_`<`$S_r$`>::value` is `true`, $s_r$`.offset() + ` $s_r$`.extent()`;

   * [3.4]{.pnum} otherwise, `ext.extent(r)`.

```c++
template<class IndexType, int N, class ... SliceSpecifiers>
array<IndexType, sizeof...(SliceSpecifiers)> @_src-indicies_@(const array<IndexType, N>& idxs, SliceSpecifiers ... slices);
```

[4]{.pnum} *Returns:* an `array<IndexType, sizeof...(SliceSpecifiers)>` `src_idx` such that `src_idx[k]` equals 

  * [4.1]{.pnum} _`first_`_`(k, slices...)` for each `k`, where _`map-rank`_`[k]` equals `dynamic_extent`, otherwise

  * [4.2]{.pnum} _`first_`_`(k, slices...) + idx[`_`map-rank`_`[k]]`.


<b>24.7.�.3 `strided_index_range` [mdspan.submdspan.strided_index_range]</b>

```c++
template<class OffsetType, class ExtentType, class StrideType>
struct strided_index_range {

  strided_index_range(OffsetType offset, ExtentType extent, StrideType stride):
    _@_offset_@(offset), _@_extent_@(extent), _@_stride_@(stride) {}

  constexpr OffsetType offset() { return _@_offset_@; }
  constexpr ExtentType extent() { return _@_extent_@; }
  constexpr StrideType stride() { return _@_stride_@; }

  using offset_type = OffsetType;
  using extent_type = ExtentType;
  using stride_type = StrideType;

private:
  OffsetType _@_offset_@; // exposition only
  ExtentType _@_extent_@; // exposition only
  StrideType _@_stride_@; // exposition only
};
```


<b>24.7.�.3 sub extents function [mdspan.submdspan.extents]</b>

```c++
template<class IndexType, class ... Extents, class ... SliceSpecifiers>
auto submdspan_extents(const extents<IndexType, Extents...>& src_exts, SliceSpecifiers ... slices);
```

[1]{.pnum} *Constraints:*

   * [1.1]{.pnum} `sizeof...(slices)` equals `Extents::rank()`,

   * [1.2]{.pnum} For each rank index `k` of `src.extents()`,
  `is_convertible_v<`$S_k$`, size_t> || is_convertible_v<`$S_k$`, tuple<size_t, size_t>> || is_convertible_v<`$S_k$`, full_extent_t> || `_`is-strided-index-range`_`<`$S_k$`>::value` is `true`, 

[2]{.pnum} *Mandates:* For each rank index `k` of `src.extents()` only one of the following is `true`:
  `is_convertible_v<`$S_k$`, size_t>`, `is_convertible_v<`$S_k$`, tuple<size_t, size_t>>`, `is_convertible_v<`$S_k$`, full_extent_t>`, _`is-strided-index-range`_`<`$S_k$`>::value`.

[3]{.pnum} *Preconditions:* For each rank index `r` of `src.extents()`,
     `0 <= `_`first_`_`(r, slices...) && `_`first_`_`(r, slices...) <= `_`last_`_`(r, src_exts, slices...) && `_`last_`_`(r, src_exts, slices...) <= src_exts.extent(r)` is `true`.

[4]{.pnum} Let `SubExtents` be a specialization of `extents` such that:

  * [4.1]{.pnum} `SubExtents::rank()` equals the number of $k$ such that `is_convertible_v<`$S_k$`, size_t>` is `false`.

  * [4.2]{.pnum} For all rank index `k` of `Extents` such that _`map-rank`_`[k] != dynamic_extent` is `true` `SubExtents::static_extent(`_`map-rank`_`[k])` equals:

       * `Extents::static_extent(k)` if `is_convertible_v<`$S_k$`, full_extent_t>` is `true`, otherwise

       * `tuple_element<1, `$S_k$`>()-tuple_element<0, `$S_k$`>()` if $S_k$ is a `tuple` of two `integral_constant`, otherwise

       *  $S_k$`::extent_type()/`$S_k$`::stride_type()` if $S_k$ is a specialization of `strided_index_range`, whose `extent_type` and `stride_type` members are `integral_constant`, otherwise

       * `dynamic_extent`.

[5]{.pnum} *Returns:* a value of type `SubExtents` `ext` such that `ext.extent(`_`map-rank`_`[k])` equals _`last_`_`(k, src_exts, slices...) - `_`first_`_`(k, slices...)` for each `k` for which _`map-rank`_`[k] != dynamic_extent` is `true`.

<b>24.7.�.3 layout specializations [mdspan.submdspan.mapping]</b>

```c++
  template<class Extents, class... SliceSpecifiers>
    constexpr auto submdspan_mapping(
      const layout_left::mapping<Extents>& src, 
      SliceSpecifiers ... slices) -> @_see below_@;

  template<class Extents, class... SliceSpecifiers>
    constexpr auto submdspan_mapping(
      const layout_right::mapping<Extents>& src, 
      SliceSpecifiers ... slices) -> @_see below_@;

  template<class Extents, class... SliceSpecifiers>
    constexpr auto submdspan_mapping(
      const layout_stride::mapping<Extents>& src, 
      SliceSpecifiers ... slices) -> @_see below_@;
```

[1]{.pnum} *Constraints:*

   * [1.1]{.pnum} `sizeof...(slices)` equals `Extents::rank()`,

   * [1.2]{.pnum} For each rank index `k` of `src.extents()`,
  `is_convertible_v<`$S_k$`, size_t> || is_convertible_v<`$S_k$`, tuple<size_t, size_t>> || is_convertible_v<`$S_k$`, full_extent_t> || `_`is-strided-index-range`_`<`$S_k$`>::value` is `true`, 

[2]{.pnum} *Mandates:* For each rank index `k` of `src.extents()` only one of the following is `true`:
  `is_convertible_v<`$S_k$`, size_t>`, `is_convertible_v<`$S_k$`, tuple<size_t, size_t>>`, `is_convertible_v<`$S_k$`, full_extent_t>`, _`is-strided-index-range`_`<`$S_k$`>::value`.

[3]{.pnum} *Preconditions:* For each rank index `r` of `src.extents()`,
     `0 <= `_`first_`_`(r, slices...) && `_`first_`_`(r, slices...) <= `_`last_`_`(r, src.extents(), slices...) && `_`last_`_`(r, src.extents(), slices...) <= src.extent(r)` is `true`.

[4]{.pnum} Let `sub_ext` be the result of `submdspan_extents(src.extents(), slices...)` and let `SubExtents` be `decltype(sub_ext)`.

[5]{.pnum} Let `sub_strides` be an `array<SubExtents::index_type, SubExtents::rank()` such that `sub_strides[`_`map-rank`_`[k]] == src.stride(k)` is `true` for each rank index `k` of `src.extents()` for which _`map-rank`_`[k]` is not `dynamic_extent`. 

[6]{.pnum} *Returns:*

   * [6.1]{.pnum} `layout_left::mapping(sub_ext)`, if `decltype(src)::layout_type` is `layout_left` and 
                  for each `k` in the range $[0,$ `SubExtents::rank()-1`$)$ $S_k$ is `full_extent_t`, 
                  and for `k` equal `SubExtents::rank()-1` `is_convertible_v<`$S_k$`, tuple<size_t, size_t>> || is_convertible_v<`$S_k$`, full_extent_t> || `_`is-strided-index-range`_`<`$S_k$`>::value` is `true`, otherwise

   * [6.2]{.pnum} `layout_right::mapping(sub_ext)`, if `decltype(src)::layout_type` is `layout_right` and 
                  for each `k` in the range $[$ `Extents::rank() - SubExtents::rank()+1, Extents.rank()`$)$ $S_k$ is `full_extent_t`, 
                  and for `k` equal `Extents::rank()-SubExtents::rank()` `is_convertible_v<`$S_k$`, tuple<size_t, size_t>> || is_convertible_v<`$S_k$`, full_extent_t> || `_`is-strided-index-range`_`<`$S_k$`>::value` is `true`, otherwise

   * [6.3]{.pnum} `layout_stride::mapping(sub_ext, sub_strides)`.

<b>24.7.�.3 layout specializations [mdspan.submdspan.offset]</b>

```c++
  template<class Extents, class... SliceSpecifiers>
    constexpr size_t submdspan_offset(
      const layout_left::mapping<Extents>& src, 
      SliceSpecifiers ... slices);

  template<class Extents, class... SliceSpecifiers>
    constexpr size_t submdspan_mapping(
      const layout_right::mapping<Extents>& src, 
      SliceSpecifiers ... slices);

  template<class Extents, class... SliceSpecifiers>
    constexpr size_t submdspan_mapping(
      const layout_stride::mapping<Extents>& src, 
      SliceSpecifiers ... slices);
```

[1]{.pnum} *Constraints:*

   * [1.1]{.pnum} `sizeof...(slices)` equals `Extents::rank()`,

   * [1.2]{.pnum} For each rank index `k` of `src.extents()`,
  `is_convertible_v<`$S_k$`, size_t> || is_convertible_v<`$S_k$`, tuple<size_t, size_t>> || is_convertible_v<`$S_k$`, full_extent_t> || `_`is-strided-index-range`_`<`$S_k$`>::value` is `true`, 

[2]{.pnum} *Mandates:* For each rank index `k` of `src.extents()` only one of the following is `true`:
  `is_convertible_v<`$S_k$`, size_t>`, `is_convertible_v<`$S_k$`, tuple<size_t, size_t>>`, `is_convertible_v<`$S_k$`, full_extent_t>`, _`is-strided-index-range`_`<`$S_k$`>::value`.

[3]{.pnum} *Preconditions:* For each rank index `r` of `src.extents()`,
     `0 <= `_`first_`_`(r, slices...) && `_`first_`_`(r, slices...) <= `_`last_`_`(r, src.extents(), slices...) && `_`last_`_`(r, src.extents(), slices...) <= src.extent(r)` is `true`.

[4]{.pnum} *Effects:* Let `P` be a parameter pack such that `is_same_v<make_index_sequence<rank()>, index_sequence<P...>>` is `true`.
Equivalent to: `return map(`_`first_`_`[P]...);`



<b>24.7.�.3 function [mdspan.submdspan.submdspan]</b>

```c++
// [mdspan.submdspan], submdspan creation
template<class ElementType, class Extents, class LayoutPolicy,
         class AccessorPolicy, class... SliceSpecifiers>
  constexpr auto submdspan(
    const mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>& src,
    SliceSpecifiers...slices) -> @_see below_@;
```


[1]{.pnum} *Constraints:*

   * [1.1]{.pnum} `sizeof...(slices)` equals `Extents::rank()`,

   * [1.2]{.pnum} For each rank index `k` of `src.extents()`,
  `is_convertible_v<`$S_k$`, size_t> || is_convertible_v<`$S_k$`, tuple<size_t, size_t>> || is_convertible_v<`$S_k$`, full_extent_t> || `_`is-strided-index-range`_`<`$S_k$`>::value` is `true`, 

   * [1.3]{.pnum} `submdspan_offset(src.mapping(), slices...)` is well formed, and
   
   * [1.4]{.pnum} `submdspan_mapping(src.mapping(), slices...)` is well formed.

[2]{.pnum} *Mandates:* For each rank index `k` of `src.extents()` only one of the following is `true`:
  `is_convertible_v<`$S_k$`, size_t>`, `is_convertible_v<`$S_k$`, tuple<size_t, size_t>>`, `is_convertible_v<`$S_k$`, full_extent_t>`, _`is-strided-index-range`_`<`$S_k$`>::value`.

[3]{.pnum} *Preconditions:* Let `sub_map` be the result of `submdspan_mapping(src.mapping(), slices...)`, than

   * [3.1]{.pnum} For each rank index `r` of `src.extents()`,
     `0 <= `_`first_`_`(r, slices...) && `_`first_`_`(r, slices...) <= `_`last_`_`(r, src.extents(), slices...) && `_`last_`_`(r, src.extents(), slices...) <= src.extent(r)` is `true`.

   * [3.2]{.pnum} `sub_map.extents() == submdspan_extents(src.mapping(), slices...)` is `true`, and

   * [3.3]{.pnum} `is_same_v<decltype(sub_map.extents()), decltype(submdspan_extents(src.mapping(), slices...))>` is `true`, and

   * [3.4]{.pnum} for each integer pack `I` which is a multi dimensional index in `sub_map.extents()`, 
     `sub_map(I...) + submdspan_offset(src.mapping(), slices...) == src.mapping()(`_`src-indicies`_`(array{I...}, slices ...))` is `true`.

[4]{.pnum} *Effects:* Equivalent to 

```c++
  size_t offset = submdspan_offset(src.mapping(), args...);
  auto sub_map = submdspan_mapping(src.mapping(), args...);
  return mdspan(src.accessor().offset(src.data(), offset),
                sub_map,
                AccessPolicy::offset_policy(src.accessor()));
```

 
<!---
[5]{.pnum} *Remarks:*

   * [5.1]{.pnum} Let `SubExtents` be a specialization of `extents` such that:
  
      * `SubExtents::rank()` equals the number of $k$ such that `is_convertible_v<`$S_k$`, size_t>` is `false`.

      * For all rank index `k` of `Extents` such that _`map-rank`_`[k] != dynamic_extent` is `true` `SubExtents::static_extent(`_`map-rank`_`[k])` equals:

         * `Extents::static_extent(k)` if `is_convertible_v<`$S_k$`, full_extent_t>` is `true`, otherwise 

         * `dynamic_extent`.

   * [5.2]{.pnum} Let `SubMapping` be the `decltype(submdspan_mapping(src.mapping(),slices...))`.

   * [5.3]{.pnum} If `SubMapping::extents_type` is not `SubExtents` the program is illformed. 

   * [5.3]{.pnum} The return type is `mdspan<ElementType, SubExtents, typename SubMapping::layout_type, typename AccessorPolicy::offset_policy>`.



[3]{.pnum} Let `sub` be the return value of `submdspan(src, slices...)`,
let $s_k$ be the $k$-th element of `slices`, and
let $S_k$ be the type of the $k$-th element of `slices`.

[4]{.pnum} Let _`map-rank`_ be an `array<size_t, Extents::rank()>`
such that for each rank index `j` of `Extents` _`map-rank`_`[j]` equals:

   * [4.1]{.pnum} `dynamic_extent` if `is_convertible_v<`$S_j$`, size_t>` is `true`, or else

   * [4.2]{.pnum} the number of $S_k$ with $k < j$ such that `is_convertible_v<`$S_k$`, size_t>` is `false`.

[5]{.pnum} Let _`first`_ and _`last`_ be `array<size_t, Extents::rank()>`.
For each rank index `r` of `src.extents()`,
define the values of _`first`_`[r]` and _`last`_`[r]` as follows:

   * [5.1]{.pnum} if `is_convertible_v<`$S_r$`, size_t>` is `true`,
  then _`first`_`[r]` equals $s_r$, and
  _`last`_`[r]` equals _`first`_`[r]` + 1;

   * [5.2]{.pnum} otherwise, if `is_convertible_v<`$S_r$`, tuple<size_t, size_t>>` is `true`,
  then _`first`_`[r]` equals `get<0>(t)`, and _`last`_`[r]` equals `get<1>(t)`,
  where `t` is the result of converting $s_r$ to `tuple<size_t, size_t>`;

   * [5.3]{.pnum} otherwise, _`first`_`[r]` equals `0`, and _`last`_`[r]` equals `src.extent(r)`.



[9]{.pnum} *Effects:*

   * [9.1]{.pnum} Direct-non-list-initializes `sub.`_`acc_`_ with `src.accessor()`.

   * [9.2]{.pnum} Let `sub_offset` be `apply(src.mapping(), `_`first`_`)` if _`first`_`[r] < src.extent(r)` for all rank index `r` of `src.extents()`, and
                  `src.mapping().required_span_size()` otherwise. 
                  Direct-non-list-initializes `sub.`_`ptr_`_ with `src.accessor().offset(src.data(), sub_offset)`.
                  <i>[Note:</i> The condition protects against applying invalid indices to the source mapping in cases such as `submdspan(a, tuple{a.extent(0), a.extent(0)})`, where
                      _`first`_`[0] ==` _`last`_`[0]` and _`last`_`[0] == src.extent(0)`. <i>- end note]</i> 

[10]{.pnum} *Postconditions:*

   * [10.1]{.pnum} For $0\:\le$ `k` < `Extents::rank()`, if _`map-rank`_`[k] != dynamic_extent` is `true`, then
     `sub.extent(`_`map-rank`_`[k])` equals _`last`_`[k] - `_`first`_`[k]`.
   
   * [10.2]{.pnum} Let `j` be a multidimensional index in `sub.extents()`, let `J` be `array{static_cast<size_t>(j)...}`, let `I` be `array<size_t, decltype(src)::rank()>` such that
     `I[k] == `_`first`_`[k] + (`_`map-rank`_`[k]==dynamic_extent?0:J[`_`map-rank`_`[k]])` is `true`, then `sub[J]` and `src[I]` refer to the same element.

   * [10.3]{.pnum} If `src.is_strided()` is `true`, then `sub.is_strided()` is `true`.

   * [10.4]{.pnum} If `src.is_unique()` is `true`, then `sub.is_unique()` is `true`.

[11]{.pnum} *Remarks:* 

   * [11.1]{.pnum} Let `SubExtents` be a specialization of `extents` such that:
  
      * `SubExtents::rank()` equals the number of $k$ such that `is_convertible_v<`$S_k$`, size_t>` is `false`.

      * For all rank index `k` of `Extents` such that _`map-rank`_`[k] != dynamic_extent` is `true` `SubExtents::static_extent(`_`map-rank`_`[k])` equals:

         * `Extents::static_extent(k)` if `is_convertible_v<`$S_k$`, full_extent_t>` is `true`, otherwise 

         * `dynamic_extent`.

   * [11.2]{.pnum} Let `SubLayout` be a type that meets the requirements of layout mapping policy and:

      * if `LayoutPolicy` is not one of `layout_left`, `layout_right`, `layout_stride`, then `SubLayout` is implementation defined, otherwise
     
      * if `SubExtents::rank()` is `0`, then `SubLayout` is `LayoutPolicy`, otherwise

      * if `LayoutPolicy` is `layout_left`, `is_convertible_v<`$S_k$`, full_extent_t>` is `true` for all
    $k$ in the range $[0,$`SubExtents::rank()-1`$)$, and `is_convertible_v<`$S_k$`, size_t>` is `false` for $k$ equal `SubExtents::rank()-1`, then
    `SubLayout` is `layout_left`, otherwise
      
      * if `LayoutPolicy` is `layout_right`, `is_convertible_v<`$S_k$`, full_extent_t>` is `true` for all
    $k$ in the range $[$`Extents::rank()-SubExtents::rank()+1`$,$ `Extents::rank()`$)$ and `is_convertible_v<`$S_k$`, size_t>` is `false` for $k$ equal `Extents::rank()-SubExtents::rank()`, then
    `SubLayout` is `layout_right`, otherwise
    
      * `SubLayout` is `layout_stride`.

   * [11.3]{.pnum} The return type is `mdspan<ElementType, SubExtents, SubLayout, typename Accesssor::offset_policy>`.
-->

