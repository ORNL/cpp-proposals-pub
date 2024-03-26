
---
title: "Add transposed special cases for P2642 layouts"
document: D????
date: 2024/03/26
audience: LEWG
author:
  - name: Mark Hoemmen
    email: <mhoemmen@nvidia.com>
toc: true
---

# Authors

* Mark Hoemmen (mhoemmen@nvidia.com) (NVIDIA)

# Revision history

* Revision 0 to be submitted for the post-Tokyo mailing before 2024/04/16

# Abstract

We propose to change the C++ Working Paper
so that `linalg::transposed` includes special cases
for `layout_left_padded` and `layout_right_padded`.
These are the two mdspan layouts proposed by P2642R6,
which was voted into the C++ Working Draft at the Tokyo 2024 WG21 meeting.
This change will make it easier for `linalg` implementations
to optimize for these two layouts by dispatching
to an existing optimized C or Fortran BLAS.
Delaying this until after C++26 would be a breaking change.

# Before and after comparison

## Example

The following example shows how this proposal
changes the return type of `transposed`.

```c++ 
// optimized overload
extern void some_algorithm(
  mdspan<const float, dextents<size_t, 2>, layout_right_padded<dynamic_extent>> A_T,
  mdspan<const float, dextents<size_t, 2>, layout_left_padded<dynamic_extent>> B,
  mdspan<float, dextents<size_t, 2>, layout_left_padded<dynamic_extent>> C);

template<class GenericFallBackLayout>
void some_algorithm(
  mdspan<const float, dextents<size_t, 2>, GenericFallBackLayout> A_T,
  mdspan<const float, dextents<size_t, 2>, layout_left_padded<dynamic_extent>> B,
  mdspan<float, dextents<size_t, 2>, layout_left_padded<dynamic_extent>> C)
{
  // ... slow generic code ...
}

void some_function(size_t N) {
  vector<float> A_storage(4 * N * N);
  vector<float> B_storage(4 * N * N);
  vector<float> C_storage(4 * N * N);

  // A, B, and C are each a 2N x 2N matrix.
  auto mapping = layout_left::mapping{extents{2 * N, 2 * N}};
  mdspan A{A_storage.data(), mapping};
  mdspan B{B_storage.data(), mapping};
  mdspan C{C_storage.data(), mapping};

  // ... fill A and B with useful values ...

  // A_00, B_00, and C_00 each view the upper left
  // N x N submatrix of its "parent."
  mdspan A_00 = submdspan(A, tuple{0, N}, tuple{0, N});
  mdspan B_00 = submdspan(B, tuple{0, N}, tuple{0, N});
  mdspan C_00 = submdspan(C, tuple{0, N}, tuple{0, N});

  // Approval of P2642R6 added this to the C++ Working Draft.
  static_assert(is_same_v<
    decltype(A_00)::layout_type,
    layout_left_padded<dynamic_extent>>);
  static_assert(A_00.stride(0) == 1); // compile-time value

  mdspan A_00_T = linalg::transposed(A_00);
  some_algorithm(A_00_T, B_00, C_00);
}
```

## Before this proposal

1. `decltype(A_00_T)::layout_type` is `layout_transposed<layout_left_padded<dynamic_extent>>`.

2. Generic overload of `some_algorithm` is called.

## After this proposal

1. `decltype(A_00_T)::layout_type` is `layout_right_padded<dynamic_extent>`. 

2. The statement `static_assert(A_00_T.stride(1) == 1);` is well formed.

3. Optimized overload of `some_algorithm` is called.

# Design justification

## What the C++ Working Draft currently does

WG21 voted P1673R13 into the C++ Working Draft at the Kona 2023 WG21 meeting.
P1673 introduces the `linalg::transposed` function,
which takes a rank-2 `mdspan` and returns a read-only `mdspan`
representing the transpose of its input.
The *transpose* of a rank-2 mdspan `A` is a rank-2 mdspan `AT`
such that `A[i, j]` refers to the same element as `AT[j, i]`
for all `i, j` in the domain of `A`.

A key feature of P1673 is that it can represent
a read-only transpose without copying or moving data.
For `layout_left`, `layout_right`, and `layout_stride`,
P1673 just reverses the extents and strides.
For a `layout_left` input, "reversing the strides"
means `layout_right`, and vice versa.
For all other layouts, P1673 uses a nested layout
called `layout_transpose` whose `operator()` invokes
the original layout with indices reversed.
This is the "fall-back" path that usually prevents
P1673 implementations from optimizing algorithms
by dispatching to optimized precompiled functions.

WG21 voted P2642R6 into the C++ Working Draft at the Tokyo 2024 WG21 meeting.
P2642R6 adds two layouts, `layout_left_padded` and `layout_right_padded`.
The data layouts described by these two class templates
are exactly the two layouts understood by the C BLAS
(Basic Linear Algebra Subroutines), as explained in P1673 and P1674.
BLAS (Basic Linear Algebra Subroutines) implementations
can optimize transpose of input matrices in these layouts
without copying data, just by reversing extents
and retaining the one input stride
(what the BLAS calls the matrix's "leading dimension").
P1673 intends for implementations to optimize algorithms
by dispatching to an existing optimized BLAS.
Therefore, it's reasonable to expect P1673 implementations
to optimize for `layout_left_padded` and `layout_right_padded`.
The way to do that would be for `transposed`
of a `layout_left_padded<PaddingValue>` `mdspan`
to return a `layout_right_padded<PaddingValue>` `mdspan`
with extents swapped and the one "padding stride" copied over,
and vice versa for `transposed`
of a `layout_right_padded<PaddingValue>` `mdspan`.
However, the C++ Working Draft currently handles those two layouts
with the "fall-back" `layout_transpose` case.

## P1673 originally included this optimization

Earlier versions of P1673 defined two `mdspan` layouts,
`layout_blas_general<column_major_t>` and `layout_blas_general<row_major_t>`.
P1673's `transposed` function originally included
special cases for those two layouts,
as one can see in P1673R9's [linalg.transp.transposed].
Version R10 of P1673 moved those layouts to P2642
and renamed them `layout_left_padded` and `layout_right_padded`, respectively.
P167310 removed these special cases from `transposed`
so that P2642 and P1673 could make progress separately.
However, P1673's authors always intended
to optimize `transposed` for those layouts.
WG21 voted P2642R6 into the C++ Working Draft at the Tokyo 2024 WG21 meeting,
so now it's possible to carry out that intent.

## Delaying until after C++26 would be a breaking change

The type of the layout of the `mdspan` returned by `transposed` is observable
and is specified in the current wording.
Therefore, delaying this change until after C++26 would be a breaking change.

## What happens if we don't do this?

One possibility is that P1673 implementations will not optimize
for `layout_left_padded` and `layout_right_padded` `mdspan`.
That would be unfortunate, because those are exactly the layouts
that the BLAS Standard supports.
This would hinder adoption of P1673 algorithms.

Another possibility is that implementations of P1673's algorithms
could optimize by adding special cases for 
`layout_transpose<layout_left_padded<PaddingValue>>` and
`layout_transpose<layout_right_padded<PaddingValue>>` input `mdspan`.
This should not hinder run-time optimization
by dispatch to an existing optimized BLAS.
However, it would complicate the implementation
and possibly add compile-time cost by introducing
more internal overloads and/or specializations.
Furthermore, it would have the same effects for users
who want to use `transposed` with their own
P1673-like linear algebra algorithms.

# Alternative: `transposed_mapping` customization point

In the previous section, we mentioned that users
may want to use `transposed` with their own
P1673-like linear algebra algorithms.
A reviewer suggested that we make it possible for users
to optimize `transposed` for their user-defined layouts,
by adding a `transposed_mapping` customization point.
This would be analogous to the `submdspan_mapping` customization point
that approval of P2630R4 (`submdspan`) added to the C++ Working Draft.
This design would have the following advantages.

1. It would be easier to specify the wording of `transposed`.
    Instead of its current list of special cases,
    it would look more like the `submdspan` wording
    that puts all the layout-specific behavior in the customization point.

2. Implementations that provide implementation-specific layouts
    could optimize `transposed` for those layouts.

3. Users could use `transposed` with their custom P1673-like algorithms
    and implement optimizations for their user-defined layouts.

Here are some reasons why WG21 might _not_ want to do this.

1. It would reserve yet another customization point name.

2. It would not help P1673 implementations optimize for user-defined layouts.

3. `submdspan_mapping` enables functionality --
    the ability to slice `mdspan` with user-defined layouts --
    while `transposed_mapping` would only enable (or simplify) optimizations.

4. LEWG has already seen the proposed design over several reviews
    (the last being the 2022/07/05 telecon review of P1673R9),
    but has not yet had a chance to review
    this alternative customization point design.

We would like LEWG to poll this design alternative.

# Wording

> Text in blockquotes is not proposed wording, but rather instructions for generating proposed wording.
>
> Change [linalg.transp.transposed] paragraph 3 ("Let `ReturnExtents` be ...") by inserting the following subparagraphs after subparagraph 3.2 ("otherwise, `layout_left` ...") and before current subparagraph 3.3 ("otherwise, `layout_stride` ...", to be renumbered to paragraph 3.5), and renumbering subparagraphs and subsubparagraphs within paragraph 3 thereafter.

[3.3]{.pnum} otherwise, `layout_right_padded<PaddingValue>` if `Layout` is `layout_left_padded<PaddingValue>` for some `size_t` value `PaddingValue`;

[3.4]{.pnum} otherwise, `layout_left_padded<PaddingValue>` if `Layout` is `layout_right_padded<PaddingValue>` for some `size_t` value `PaddingValue`;

> Change [linalg.transp.transposed] paragraph 4 (*Returns* clause of `transposed`) by inserting the following subparagraphs after subparagraph 4.1 (for `Layout` being `layout_left`, `layout_right`, or a specialization of `layout_blas_packed`) and before current subparagraph 4.2 (for `Layout` being `layout_stride`, to be renumbered to subparagraph 4.4), and renumbering subparagraphs within paragraph 4 thereafter.

[4.2]{.pnum} otherwise,
    `R(a.data_handle(), ReturnMapping(`_`transpose-extents`_`(a.mapping().extents()), a.mapping().stride(1)), a.accessor())`
    if `Layout` is `layout_left_padded<PaddingValue>`
    for some `size_t` value `PaddingValue`;

[4.3]{.pnum} otherwise,
    `R(a.data_handle(), ReturnMapping(`_`transpose-extents`_`(a.mapping().extents()), a.stride(0)), a.accessor())`
    if `Layout` is `layout_right_padded<PaddingValue>`
    for some `size_t` value `PaddingValue`;
