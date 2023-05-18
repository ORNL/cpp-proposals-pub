---
title: "Extending Linear Algebra Support to Batched Operations"
document: 
date: today
audience: SG1, SG6, LEWGI
author:
  - name: Mark Hoemmen 
    email: <mhoemmen@nvidia.com>
  - name: Kim Liegeois
    email: <knliege@sandia.gov>
  - name: Christian Trott 
    email: <crtrott@sandia.gov>
toc: true
---

# Revision History

## Initial Version 2023-05 Mailing

- Initial version for SG review

# Abstract

We propose extending P1673
("A free function linear algebra interface based on the BLAS")
to support "batched" linear algebra, that is,
solving multiple independent problems all at once.
The initial version of this proposal discusses the interface changes to P1673
that would be needed for batched linear algebra.

# Motivation

## What is batched linear algebra?

"Batched" linear algebra functions solve
many independent linear algebra problems all at once, in a single function call.  For example, a "batched GEMM" computes multiple matrix-matrix multiplies at once.
Batched linear algebra interfaces have the following advantages.

* By exposing the user's intent to solve many problems at once,
    they expose much more potential parallelism and vectorization
    than a single very small problem has (Dongarra 2018),
    and amortize the overhead of representing each problem
    as an argument of a function call.

* They are useful for many different fields,
    including machine learning, science, and engineering.
    For a long list of applications that benefit, see Dongarra 2018.

* Hardware vendors such as NVIDIA, Intel, and AMD
    currently offer optimized software libraries
    to support batched linear algebra,
    and hardware features to accelerate it.

* There is an ongoing
    <a href="http://icl.utk.edu/bblas/">interface standardization effort</a>,
    in which we participate.

It's possible to use existing non-batched libraries,
like the BLAS and LAPACK, to solve small linear algebra problems.
However, these libraries were designed to solve one large problem at a time.
For example, they check input arguments for consistency on every call,
which could take longer than the actual algorithm for a tiny matrix.
Batched libraries can and do amortize these checks.
These libraries also take each array input and output as a pointer,
run-time dimensions, and run-time strides.
Some problems are small enough that each problem's data
takes no more space than a pointer to the data.
Our non-batched linear algebra library proposal P1673
can use mdspan's layout mapping
to encode dimensions and/or strides as compile-time constants,
but `mdspan` still requires a run-time pointer or data handle.
Batching up multiple inputs into a single `mdspan`
amortizes the overhead of representing each problem.

The interface of batched linear algebra operations
matters a lot for performance, but may constrain generality.
For example, requiring a specific data layout
and constraining all matrices to have the same dimensions
makes vectorization easier,
but applications in sparse multifrontal matrix factorizations
may produce dense matrices of different dimensions.
Vendors have different interfaces with different levels of generality.
For a survey of different interface options, see Relton et al. 2016.
This proposal briefly summarizes different interface options
and explains why we chose what we did.

The `mdspan` data structure makes it easy to represent a batch
of linear algebra objects, and to optimize their data layout.
With few exceptions, the extension of P1673 to support batched operations
will not require new function names or interface changes.
Only the requirements on functions will change.
Output arguments can have an additional rank, representing the batch mode.
If so, then the leftmost extent will refer to the batch dimension.
Input arguments may also have an additional rank to match;
if they do not, the function will use ("broadcast") the same
input argument for all the output arguments in the batch.

## Why we should extend support to batched linear algebra

High-performance implementation is not just a parallel loop over non-batched function calls.

SIMD interleaved layout -- even if nested C++17 parallel algorithms worked perfectly, they could not optimize this case, because the layout crosses multiple independent problems.

# Existing Practice

## Vendor Libraries

mkl blah blah, cublas blah blah, rocblas blah blah

## Open Source Practice

KokkosKernels blah blah, BLAS proposal blah blah, Magma??

# Design discussion

## Interface options

### Representing dimensions and strides

For a summary of C interface options, see Relton et al. 2016.
This technical report establishes vocabulary to describe
different kinds of batched BLAS interfaces.
The most important interface design choice is how to represent
the dimensions and strides of the vector and matrix input(s) and output.
We collectively call the dimensions and strides the "metadata."
Relton et al. identify three main options.

1. "Fixed": same metadata for all the problems; packed data

2. "Variable": different metadata for all the problems; "array of problems"

3. "Group": "array of fixed" (multiple instances of fixed; each instance may have different metadata from the other instances)

Allowing the metadata to vary for different problems
makes parallelization and vectorization more challenging.
Furthermore, optimizing the fixed case well is a prerequisite
for high-performance variable and group implementations.
Thus, we focus for now on the fixed case.

Relton et al. 2016 further subdivides the fixed interface into three options,
depending on how the interface represents each batch argument.

a. "P2P" (pointer to pointer): each batch argument is an array of pointers.

b. "Strided": each batch argument is packed into a single array, with a fixed element stride (space) between the start of each input in the batch.

c. "Interleaved": for example, if the batch index is `k`, then the `i, j` element of all the problems (or, possibly only SIMD width number of problems) are stored contiguously.

The P2P interface would require extra packing and unpacking of pointers,
and therefore extra overhead.  In practice, users often want to represent
a batch as a "pre-packed" array with a particular layout,
so they waste time and code with the P2P interface.
Even though P2P could be used for the fixed interface,
it is more useful for the variable or group interface.
Thus, we exclude the P2P option for now.

The `mdspan` class lets us naturally combine "strided" and "interleaved"
into a single interface by using different, possibly custom mdspan layouts.
Each batch parameter would become a single `mdspan`,
with an extra rank representing the batch mode
(the index of a problem within the batch).  

We may want to add new layouts that bake more information into the layout at compile time.  We may also wish to add `aligned_accessor`.

### Representing scaling factors

Some BLAS functions take scaling factors.
For example, a single matrix-matrix multiply computes
`C = beta * C + alpha * A * B`
for matrices `A`, `B`, and `C` and scalars `alpha` and `beta`.
Batched linear algebra has a design choice:
should the different problems in a batch
use the same or different scaling factors?

P1673 expresses scaling factors with an accessor `accessor_scaled`,
that users access mainly by calling the `scaled` function.
For example, `scaled(alpha, A)` represents the product
of the (scalar) scaling factor `alpha` and the matrix `A`.

If we want to use the same scaling factor for all the problems in a batch,
we can use `accessor_scaled` and `scaled` without interface changes.
However, if we want to use a different scaling factor for each problem,
we would either need to let `accessor_scaled`
hold an `mdspan` of the scaling factors,
or change all the function interfaces
to take separate `mdspan` parameters for the scaling factors.
We favor changing `accessor_scaled`,
as it would maintain interface consistency.
We could also retain the existing `accessor_scaled` that holds a scalar,
so that `scaled` could take either a single scaling factor
or an `mdspan` of scaling factors.
This would be consistent with the "broadcast" approach
described elsewhere in this proposal.

### Representing the result of a reduction (dot product or norm)

The Batched BLAS interface specification (Dongarra 2018) omits "reduction"-like operations -- dot products and norms -- that return a single value.

The original P1673 design had reductions write to an output reference
(or rank-0 mdspan), with the intent that this could be generalized
to an output rank-1 (the batch mode) mdspan.
LEWG and previous Study Groups asked P1673 authors
to make reductions look like `std::reduce`:
returning a value, instead of writing to an output argument.
This interface is easier to understand
and more consistent with the Standard Library for the non-batched case.
However, it means that the batched interface
cannot be made fully consistent with the non-batched interface.
(This is because we do not permit P1673 or its extensions
to allocate and return arrays of elements.
It is an essential feature of P1673, as it was of the BLAS and LAPACK,
that it can be implemented and used without dynamic memory allocation.
Implementations may still choose to do dynamic memory allocation internally,
but they are not required to do so.)

This gives us two design options.

1. Overload reduction functions for the batched case
    to return `void` and take an output `mdspan`.

2. Omit reductions from the batched interface.

We favor the second approach:
adding overloads of P1673 reduction-like functions that return `void`
and write the reduction results to an output `mdspan`.
This has the disadvantage that the batched and non-batched versions
of the same function would no longer take the same number of arguments.
However, there should be no ambiguity between the two cases.

The Batched BLAS interface proposal (Dongarra 2018)
takes the second option: it simply omits batched reductions.
In some cases, users could replace the missing features with other functions.
For example, a batched dot product $x^T y_1$, $x^T y_2$, $\dots$, $x^T y_K$
could be expressed as a non-batched matrix-vector product,
and a batched dot product $x_1^T y_1$, $x_2^T y_2$, $\dots$, $x_K^T y_K$
could be expressed as a batched matrix multiply,
where each problem in the batch is 1 by $N$ times 1 by $N$
(where $N$ is the number of elements in each vector).
However, this approach has four issues.

1. The authors' experience is that special cases
    of more general BLAS functions
    (e.g., 1 by $N$ times 1 by $N$ matrix multiplies)
    may not perform as well as more specialized BLAS functions
    (e.g, dot products).

2. Some reduction functions cannot naturally be represented this way.
    The max-norm (or index of the max absolute value,
    which is what the BLAS computes) is an example.

3. Other reduction functions could be represented with existing operations,
    but doing so efficiently may be difficult.
    For example, a batched 1-norm could be represented
    as the dot product of an elementwise absolute value of a matrix,
    with a vector whose elements are all the value 1.
    One could represent the elementwise absolute value lazily
    with an accessor, and a vector of all ones efficiently
    with a nonunique layout.  However, the resulting code
    might not be efficient.

4. Even if some batched reduction functions
    could be represented with existing operations,
    doing so may sacrifice accuracy or correctness guarantees
    due to rounding error.  For example, the batched 2-norm
    could be represented as a batched dot product of each vector by itself,
    followed by an elementwise square root.
    However, the result would be more prone to underflow or overflow,
    since the batched dot product would be working with squares of elements.

### Representing broadcast parameters

If the output `mdspan` has an extra rank,
then we assume that users want to do a batched computation,
and we treat the leftmost extent as the batch extent
(the index of a problem in the batch).
Any input `mdspan` with an extra rank
also has its leftmost extent treated as the batch extent.

Users may want to repeat a single input for all the problems in a batch.
For example, users may want to perform matrix-vector multiply
with the same matrix, but different input and output vectors.
We call the repeated single input a "broadcast" parameter.
This feature minimizes storage requirements and overhead.
Output (or input/output) `mdspan` cannot be broadcast.

We represent broadcast input parameters
as an input `mdspan` without the extra batch extent.
That is, the input's rank is the same as it would have been
in the non-batched case.
This interpretation is unambiguous for all P1673 functions.

We could have chosen to express broadcast parameters
by requiring that they have the "extra" batch extent,
but using a nonunique layout:
for example, a strided layout with stride zero in the batch mode.
(`layout_stride` does not permit this,
but general strided layouts can have zero strides.)
Users can still do this with our proposal.
However, we consider nonunique layouts an expert mdspan feature
that is more challenging for users to implement.
We also do not want to add such layouts to the Standard Library,
as we think broadcast parameters
have a natural representation without them.

# Suggested poll questions

1. Should broadcast input parameters be represented by `mdspan` with one less rank (than the batched case would normally require), or by some other method?

# References

* Samuel D. Relton, Pedro Valero-Lara, and Mawussi Zounon,
    ["A Comparison of Potential Interfaces for Batched BLAS Computations,"](http://www.nlafet.eu/wp-content/uploads/2016/01/NLAFET-WN5-Relton-ValeroLara-Zounon-161111.pdf) NLAFET Working Note 5, August 2016.

* Jack Dongarra, Iain Duff, Mark Gates, Azzam Haidar, Sven Hammarling, Nicholas J. Higham, Jonathan Hogg, Pedro Valero Lara, Piotr Luszczek, Mawussi Zounon, Samuel D. Relton, Stanimire Tomov, Timothy Costa, and Sarah Knepper,
    ["Batched BLAS (Basic Linear Algebra Subprograms) 2018 Specification,"](https://www.icl.utk.edu/files/publications/2018/icl-utk-1170-2018.pdf)
    July 2018.
