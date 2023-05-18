---
title: "Extending Linear Algebra Support to Batched Operations"
document: 
date: today
audience: LWG
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

# What is batched linear algebra?

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

# Why we should extend support to batched linear algebra

1. Many applications need to solve 

1. Essential for performance of many small linear algebra problems

2. High-performance implementation is not just a parallel loop over non-batched function calls

SIMD interleaved layout -- even if nested C++17 parallel algorithms worked perfectly, they could not optimize this case, because the layout crosses multiple independent problems.

# Existing Practice

## Vendor Libraries

mkl blah blah, cublas blah blah, rocblas blah blah

## Open Source Practice

KokkosKernels blah blah, BLAS proposal blah blah, Magma??

# Extending std::linalg

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

### Representing 

### What about alpha and beta?

P1673 expresses this with an accessor (result of `scaled`).
Could an accessor view an mdspan?
It's possible, but we might also consider either requiring all alpha and beta to be the same, or adding new overloads.

2018 specification (see above) omits reductions (dot product and norms).

P1673 originally had reductions write to an output reference, with the intent that this could be generalized to an output rank-1 (the batch mode) mdspan.  LEWG and previous SGs asked P1673 authors to make reductions look like `std::reduce`: returning a value, instead of writing to an output reference.  This interface is easier to understand for the non-batched case, but it means that the batched interface cannot be made fully consistent with the non-batched interface.  (This is because we do not permit stdblas to allocate and return arrays of elements.)

Options:

1. Overload to take an output mdspan
2. Omit reductions from the batched interface (Batched BLAS spec takes this approach)

### What about broadcasts?

Example: matrix-vector multiply with the same matrix, but different input and output vectors.

Broadcast layouts for input are effectively strided layouts with stride zero in the broadcast mode.  (`layout_stride` does not permit this, but general strided layouts can have zero strides.)

Interpretation via overloads is unambiguous (if there's an extra mode in an mdspan

leftmost mode (???) is the batch mode)

## Principle

Higher rank arguments

## Challenge 1: Reductions

output argument

## Challenge 2: Multiple Scaling factors

extra mdspan args?

# References

* Samuel D. Relton, Pedro Valero-Lara, and Mawussi Zounon,
    ["A Comparison of Potential Interfaces for Batched BLAS Computations,"](http://www.nlafet.eu/wp-content/uploads/2016/01/NLAFET-WN5-Relton-ValeroLara-Zounon-161111.pdf) NLAFET Working Note 5, August 2016.

* Jack Dongarra, Iain Duff, Mark Gates, Azzam Haidar, Sven Hammarling, Nicholas J. Higham, Jonathan Hogg, Pedro Valero Lara, Piotr Luszczek, Mawussi Zounon, Samuel D. Relton, Stanimire Tomov, Timothy Costa, and Sarah Knepper,
    ["Batched BLAS (Basic Linear Algebra Subprograms) 2018 Specification,"](https://www.icl.utk.edu/files/publications/2018/icl-utk-1170-2018.pdf)
    July 2018.
