
---
title: "Fix C++26 by making the symmetric and Hermitian rank-k and rank-2k updates consistent with the BLAS"
document: P3371R0
date: today
audience: LEWG
author:
  - name: Mark Hoemmen
    email: <mhoemmen@nvidia.com>
toc: true
---

# Authors

* Mark Hoemmen (mhoemmen@nvidia.com) (NVIDIA)

# Revision history

* Revision 0 to be submitted 2024-08-15

# Abstract

The four `std::linalg` functions `symmetric_matrix_rank_k_update`,  `hermitian_matrix_rank_k_update`, `symmetric_matrix_rank_2k_update`, and  `hermitian_matrix_rank_2k_update` currently have behavior inconsistent with their corresponding BLAS (Basic Linear Algebra Subroutines) routines.  Also, their behavior is inconsistent with `std::linalg`'s `matrix_product`, even though in mathematical terms they are special cases of a matrix-matrix product.

We propose fixing this by making two changes.  First, we will add "updating" overloads analogous to the updating overloads of `matrix_product`.  For example, `symmetric_matrix_rank_k_update(A, scaled(beta, C), C, upper_triangle)` will perform $C := \beta C + A A^T$.  Second, we will change the behavior of the existing overloads to be "overwriting," without changing the number of their parameters or how they are called.  For example, `symmetric_matrix_rank_k_update(A, C, upper_triangle)` will perform $C := A A^T$ instead of $C := C + A A^T$.

The second set of changes is a breaking change.  Thus, we must finish this before finalization of C++26.

# Motivation

Each function in std::linalg generally corresponds to one or more routines or functions in the original BLAS (Basic Linear Algebra Subroutines).  Every computation that the BLAS can do, a function in std::linalg should be able to do.

One std::linalg user <a href="https://github.com/kokkos/stdBLAS/issues/272#issuecomment-2248273146">reported</a> an exception to this rule.  The BLAS routine `DSYRK` (Double-precision SYmmetric Rank-K update) computes $C := \beta C + \alpha A A^T$, but the corresponding `std::linalg` function `symmetric_matrix_rank_k_update` only computes $C := C + \alpha A A^T$.  That is, `std::linalg` currently has no way to express this BLAS operation with a general $\beta$ scaling factor.  This issue applies to all of the following functions.

* `symmetric_matrix_rank_k_update`: computes $C := C + \alpha A A^T$ 
* `hermitian_matrix_rank_k_update`: computes $C := C + \alpha A A^H$
* `symmetric_matrix_rank_2k_update`: computes $C := C + \alpha A B^H + \alpha B A^H$
* `hermitian_matrix_rank_2k_update`: computes $C := C + \alpha A B^H + \bar{\alpha} B A^H$, where $\bar{\alpha}$ denotes the complex conjugate of $\alpha$

These functions implement special cases of matrix-matrix products.  The `matrix_product` function in `std::linalg` implements the general case of matrix-matrix products.  This function corresponds to the BLAS's `SGEMM`, `DGEMM`, `CGEMM`, and `ZGEMM`, which compute $C := \beta C + \alpha A B$, where $\alpha$ and $\beta$ are scaling factors.  The `matrix_product` function has two kinds of overloads:

1. *overwriting* ($C = A B$) and

2. *updating* ($C = E + A B$).

The updating overloads handle the general $\alpha$ and $\beta$ case by `matrix_product(scaled(alpha, A), B, scaled(beta, C), C)`.  The specification explicitly permits the input `scaled(beta, C)` to alias the output `C` (**[linalg.algs.blas3.gemm]** 10: "*Remarks*: `C` may alias `E`").  The `std::linalg` library provides overwriting and updating overloads so that it can do everything that the BLAS does, just in a more idiomatically C++ way.  Please see <a href="https://isocpp.org/files/papers/P1673R13.html#function-argument-aliasing-and-zero-scalar-multipliers">P1673R13 Section 10.3</a> ("Function argument aliasing and zero scalar multipliers") for a more detailed explanation.

The problem with functions like `symmetric_matrix_rank_k_update` is that they have the same _calling syntax_ as the overwriting version of `matrix_product`, but _semantics_ that differ from both the overwriting and the updating versions of `matrix_product`.  The current overloads are not overwriting, so we can't just fix this problem by introducing an "updating" overload for each function.

Incidentally, the fact that these functions have "update" in their name is not relevant, because that naming choice is original to the BLAS.  The BLAS calls its corresponding `xSYRK`, `xHERK`, `xSYR2K`, and `xHER2K` routines "{Symmetric, Hermitian} rank {one, two} update," even though setting $\beta = 0$ makes these routines "overwriting" in the sense of `std::linalg`.

# Proposed changes

We propose to fix this by making the four functions work just like `matrix_vector_product` or `matrix_product`.  This entails two changes.

1. We will add "updating" overloads with a new input matrix parameter `E`, analogous to the updating overloads of `matrix_product`.  For example, `symmetric_matrix_rank_k_update(A, E, C, upper_triangle)` will compute $C = E + A A^T$.  We will specifically permit `C` and `E` to alias, thus permitting the desired case where `E` is `scaled(beta, C)`.  The updating overloads will take `E` as an _`in-matrix`_, and will change `C` from a _`possibly-packed-inout-matrix`_ to a _`possibly-packed-out-matrix`_.

2. We will change the behavior of the existing overloads to be "overwriting," without changing how they are called.  For example, `symmetric_matrix_rank_k_update(A, C, upper_triangle)` will compute $C = A A^T$ instead of $C := C + A A^T$.  The overwriting overloads will change `C` from a _`possibly-packed-inout-matrix`_ to a _`possibly-packed-out-matrix`_.

This second set of changes is a breaking change.  It needs to be so that we can provide the overwriting behavior $C := \alpha A A^T$ that the corresponding BLAS routines already provide.  Thus, we must finish this before finalization of C++26.

Both sets of overloads still only write to the specified triangle (lower or upper) of the output matrix `C`.  As a result, the new updating overloads only read from that triangle of the input matrix `E`.  Therefore, even though `E` may be a different matrix than `C`, the updating overloads do not need an additional `Triangle t_E` parameter for `E`.  The `symmetric_*` functions interpret `E` as symmetric in the same way that they interpret `C` as symmetric, and the `hermitian_*` functions interpret `E` as Hermitian in the same way that they interpret `C` as Hermitian.

# We do NOT propose changing rank-1 or rank-2 update functions

The four functions we propose to change have the following rank-1 and rank-2 analogs, where $A$ denotes a symmetric or Hermitian matrix (depending on the function's name) and $x$ and $y$ denote vectors.

* `symmetric_matrix_rank_1_update`: computes $A := A + \alpha x x^T$
* `hermetian_matrix_rank_1_update`: computes $A := A + \alpha x x^H$
* `symmetric_matrix_rank_2_update`: computes $A := A + \alpha x y^T + \alpha y x^T$
* `hermitian_matrix_rank_2_update`: computes $A := A + \alpha x y^H + \bar{\alpha} x y^H$

We do NOT propose to change these functions analogously to the rank-k and rank-2k update functions.  This is because the BLAS routines corresponding to the rank-1 and rank-2 functions -- `xSYR`, `xHER`, `xSYR2`, and `xHER2` -- do not have a way to supply a $\beta$ scaling factor.  That is, these `std::linalg` functions can already do everything that their corresponding BLAS routines can do.  This is consistent with our design intent in <a href="https://isocpp.org/files/papers/P1673R13.html#function-argument-aliasing-and-zero-scalar-multipliers">Section 10.3 of P1673R3</a> for translating Fortran `INTENT(INOUT)` arguments into a C++ idiom.

> b. Else, if the BLAS function unconditionally updates (like `xGER`), we retain read-and-write behavior for that argument.
>
> c. Else, if the BLAS function uses a scalar `beta` argument to decide whether to read the output argument as well as write to it (like `xGEMM`), we provide two versions: a write-only [that is, "overwriting"] version (as if `beta` is zero), and a read-and-write [that is, "updating"] version (as if `beta` is nonzero).

The rank-1 and rank-2 update functions "unconditionally update," in the same way that the BLAS's general rank-1 update routine `xGER` does.  However, the BLAS's rank-k and rank-2k update functions "use a scalar `beta` argument...," so for consistency, it makes sense for `std::linalg` to provide both overwriting and updating versions.

Since we do not propose changing the symmetric and Hermitian rank-1 and rank-2 functions, we retain the exposition-only concept _`possibly-packed-inout-matrix`_, which they use to constrain their parameter `A`.

# Fixes for some wording issues

There are some wording issues in **[linalg.algs.blas3.rankk]** and **[linalg.algs.blas3.rank2k]**, specifically in the Mandates, Preconditions, and Complexity clauses.  These clauses do not reflect the design intent, which is expressed by the corresponding BLAS routines and the mathematical expressions in the current wording that express the functions' behavior.  We have not yet filed an LWG issue for these wording issues.  Given that the above changes will call for adjusting some parts of these clauses anyway (e.g., by changing `InOutMat` to `OutMat`), we have decided to propose "drive-by" fixes to these clauses in this proposal.  We nevertheless plan to file an LWG issue to fix both these and other wording issues we have found.

Many thanks (with permission) to Raffaele Solcà (CSCS Swiss National Supercomputing Centre, `raffaele.solca@cscs.ch`) for pointing out this issue and related issues that we plan to file as part of the aforementioned LWG issue.

# Wording

> Text in blockquotes is not proposed wording,
> but rather instructions for generating proposed wording.
> The � character is used to denote a placeholder section number
> which the editor shall determine.
>
> In **[version.syn]**, for the following definition,

```c++
#define __cpp_lib_linalg YYYYMML // also in <linalg>
```

> adjust the placeholder value `YYYYMML` as needed
> so as to denote this proposal's date of adoption.

## New exposition-only concept

> In the header `<linalg>` synopsis **[linalg.syn]**,
> immediately before the following,

```c++
  template<class T>
    concept @_possibly-packed-inout-matrix_@ = @_see below_@;   // exposition only
```

> add the following.

```c++
  template<class T>
    concept @_possibly-packed-out-matrix_@ = @_see below_@;   // exposition only
```

Then, to **[linalg.helpers.concepts]**, immediately before the following,

```c++
template<class T>
  concept @_possibly-packed-inout-matrix_@ =
    is-mdspan<T> && T::rank() == 2 &&
    is_assignable_v<typename T::reference, typename T::element_type> &&
    (T::is_always_unique() || is-layout-blas-packed<typename T::layout_type>);
```

> add the following definition of the exposition-only concept _`possibly-packed-out-matrix`_.

```c++
template<class T>
  concept @_possibly-packed-out-matrix_@ =
    is-mdspan<T> && T::rank() == 2 &&
    is_assignable_v<typename T::reference, typename T::element_type> &&
    (T::is_always_unique() || is-layout-blas-packed<typename T::layout_type>);
```

## Rank-k update functions in synopsis

> In the header `<linalg>` synopsis **[linalg.syn]**,
> change the following

```c++
  // rank-k symmetric matrix update
  template<class Scalar, @_in-matrix_@ InMat, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void symmetric_matrix_rank_k_update(Scalar alpha, InMat A, InOutMat C, Triangle t);
  template<class ExecutionPolicy, class Scalar,
           @_in-matrix_@ InMat, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void symmetric_matrix_rank_k_update(ExecutionPolicy&& exec,
                                        Scalar alpha, InMat A, InOutMat C, Triangle t);

  template<@_in-matrix_@ InMat, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void symmetric_matrix_rank_k_update(InMat A, InOutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix InMat_@, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void symmetric_matrix_rank_k_update(ExecutionPolicy&& exec,
                                        InMat A, InOutMat C, Triangle t);

  // rank-k Hermitian matrix update
  template<class Scalar, @_in-matrix_@ InMat, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void hermitian_matrix_rank_k_update(Scalar alpha, InMat A, InOutMat C, Triangle t);
  template<class ExecutionPolicy,
           class Scalar, @_in-matrix_@ InMat, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void hermitian_matrix_rank_k_update(ExecutionPolicy&& exec,
                                        Scalar alpha, InMat A, InOutMat C, Triangle t);

  template<@_in-matrix_@ InMat, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void hermitian_matrix_rank_k_update(InMat A, InOutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat, @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void hermitian_matrix_rank_k_update(ExecutionPolicy&& exec,
                                        InMat A, InOutMat C, Triangle t);
```

> to read as follows.

```c++
  // overwriting rank-k symmetric matrix update
  template<class Scalar,
           @_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      Scalar alpha, InMat A, OutMat C, Triangle t);
  template<class ExecutionPolicy, class Scalar,
           @_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      ExecutionPolicy&& exec, Scalar alpha, InMat A, OutMat C, Triangle t);
  template<@_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      InMat A, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      ExecutionPolicy&& exec, InMat A, OutMat C, Triangle t);

  // updating rank-k symmetric matrix update
  template<class Scalar,
           @_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      Scalar alpha,
      InMat1 A, InMat2 E, OutMat C, Triangle t);
  template<class ExecutionPolicy, class Scalar,
           @_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      ExecutionPolicy&& exec, Scalar alpha,
      InMat1 A, InMat2 E, OutMat C, Triangle t);
  template<@_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      InMat1 A, InMat2 E, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void symmetric_matrix_rank_k_update(
      ExecutionPolicy&& exec,
      InMat1 A, InMat2 E, OutMat C, Triangle t);

  // overwriting rank-k Hermitian matrix update
  template<class Scalar,
           @_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      Scalar alpha, InMat A, OutMat C, Triangle t);
  template<class ExecutionPolicy, class Scalar,
           @_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      ExecutionPolicy&& exec, Scalar alpha, InMat A, OutMat C, Triangle t);
  template<@_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      InMat A, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      ExecutionPolicy&& exec, InMat A, OutMat C, Triangle t);

  // updating rank-k Hermitian matrix update
  template<class Scalar,
           @_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      Scalar alpha,
      InMat1 A, InMat2 E, OutMat C, Triangle t);
  template<class ExecutionPolicy, class Scalar,
           @_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      ExecutionPolicy&& exec, Scalar alpha,
      InMat1 A, InMat2 E, OutMat C, Triangle t);
  template<@_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      InMat1 A, InMat2 E, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1,
           @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat,
           class Triangle>
    void hermitian_matrix_rank_k_update(
      ExecutionPolicy&& exec,
      InMat1 A, InMat2 E, OutMat C, Triangle t);
```

## Rank-2k update functions in synopsis

> In the header `<linalg>` synopsis **[linalg.syn]**,
> change the following

```c++
  // rank-2k symmetric matrix update
  template<@_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void symmetric_matrix_rank_2k_update(InMat1 A, InMat2 B, InOutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void symmetric_matrix_rank_2k_update(ExecutionPolicy&& exec,
                                         InMat1 A, InMat2 B, InOutMat C, Triangle t);

  // rank-2k Hermitian matrix update
  template<@_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void hermitian_matrix_rank_2k_update(InMat1 A, InMat2 B, InOutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-inout-matrix_@ InOutMat, class Triangle>
    void hermitian_matrix_rank_2k_update(ExecutionPolicy&& exec,
                                         InMat1 A, InMat2 B, InOutMat C, Triangle t);
```

> to read as follows.

```c++
  // overwriting rank-2k symmetric matrix update
  template<@_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void symmetric_matrix_rank_2k_update(InMat1 A, InMat2 B, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void symmetric_matrix_rank_2k_update(ExecutionPolicy&& exec,
                                         InMat1 A, InMat2 B, OutMat C, Triangle t);

  // updating rank-2k symmetric matrix update
  template<@_in-matrix_@ InMat1, @_in-matrix_@ InMat2, @_in-matrix_@ InMat3,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void symmetric_matrix_rank_2k_update(InMat1 A, InMat2 B, InMat3 E, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1, @_in-matrix_@ InMat2, @_in-matrix_@ InMat3,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void symmetric_matrix_rank_2k_update(ExecutionPolicy&& exec,
                                         InMat1 A, InMat2 B, InMat3 E, OutMat C, Triangle t);

  // overwriting rank-2k Hermitian matrix update
  template<@_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void hermitian_matrix_rank_2k_update(InMat1 A, InMat2 B, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1, @_in-matrix_@ InMat2,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void hermitian_matrix_rank_2k_update(ExecutionPolicy&& exec,
                                         InMat1 A, InMat2 B, OutMat C, Triangle t);

  // updating rank-2k Hermitian matrix update
  template<@_in-matrix_@ InMat1, @_in-matrix_@ InMat2, @_in-matrix_@ InMat3,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void hermitian_matrix_rank_2k_update(InMat1 A, InMat2 B, InMat3 E, OutMat C, Triangle t);
  template<class ExecutionPolicy,
           @_in-matrix_@ InMat1, @_in-matrix_@ InMat2, @_in-matrix_@ InMat3,
           @_possibly-packed-out-matrix_@ OutMat, class Triangle>
    void hermitian_matrix_rank_2k_update(ExecutionPolicy&& exec,
                                         InMat1 A, InMat2 B, InMat3 E, OutMat C, Triangle t);
```

## Specification of rank-k update functions

> Replace the entire contents of [linalg.algs.blas3.rankk] with the following.

<i>[Note:</i> These functions correspond to the BLAS functions
`xSYRK` and `xHERK`. <i>-- end note]</i>

[1]{.pnum} The following elements apply to all functions in [linalg.algs.blas3.rankk].

[2]{.pnum} *Mandates:*

  * [2.1]{.pnum} If `OutMat` has `layout_blas_packed` layout, then the
      layout's `Triangle` template argument has the same type as
      the function's `Triangle` template argument.

  * [2.2]{.pnum} _`possibly-multipliable`_`<decltype(A), decltype(transposed(A)), decltype(C)>` is `true`.

  * [2.3]{.pnum} _`possibly-addable`_`<decltype(C), decltype(E), decltype(C)>` is `true` for those overloads that take an `E` parameter.

[3]{.pnum} *Preconditions:*

  * [3.1]{.pnum} _`multipliable`_`(A, transposed(A), C)` is `true`.  <i>[Note:</i> This implies that `C` is square. <i>-- end note]</i>

  * [3.2]{.pnum} _`addable`_`(C, E, C)` is `true` for those overloads that take an `E` parameter.

[4]{.pnum} *Complexity:* $O($ `A.extent(0)` $\cdot$ `A.extent(1)` $\cdot$ `A.extent(0)` $)$.

```c++
template<class Scalar,
         @_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  Scalar alpha,
  InMat A,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         class Scalar,
         @_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  Scalar alpha,
  InMat A,
  OutMat C,
  Triangle t);
```

[5]{.pnum} *Effects:*
Computes $C = \alpha A A^T$,
where the scalar $\alpha$ is `alpha`.

```c++
template<@_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  InMat A,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  InMat A,
  OutMat C,
  Triangle t);
```

[6]{.pnum} *Effects:*
Computes $C = A A^T$.

```c++
template<class Scalar,
         @_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  Scalar alpha,
  InMat A,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         class Scalar,
         @_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  Scalar alpha,
  InMat A,
  OutMat C,
  Triangle t);
```

[7]{.pnum} *Effects:*
Computes $C = \alpha A A^H$,
where the scalar $\alpha$ is `alpha`.

```c++
template<@_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  InMat A,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  InMat A,
  OutMat C,
  Triangle t);
```

[8]{.pnum} *Effects:*
Computes $C = A A^H$.

```c++
template<class Scalar,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  Scalar alpha,
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         class Scalar,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  Scalar alpha,
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
```

[9]{.pnum} *Effects:*
Computes $C = E + \alpha A A^T$,
where the scalar $\alpha$ is `alpha`.

```c++
template<@_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
```

[10]{.pnum} *Effects:*
Computes $C = E + A A^T$.

```c++
template<class Scalar,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  Scalar alpha,
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         class Scalar,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  Scalar alpha,
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
```

[11]{.pnum} *Effects:*
Computes $C = E + \alpha A A^H$,
where the scalar $\alpha$ is `alpha`.

```c++
template<@_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  InMat1 A,
  InMat2 E,
  OutMat C,
  Triangle t);
```

[12]{.pnum} *Effects:*
Computes $C = E + A A^H$.

## Specification of rank-2k update functions

> Replace the entire contents of [linalg.algs.blas3.rank2k] with the following.

<i>[Note:</i> These functions correspond to the BLAS functions
`xSYR2K` and `xHER2K`. <i>-- end note]</i>

[1]{.pnum} The following elements apply to all functions in [linalg.algs.blas3.rank2k].

[2]{.pnum} *Mandates:*

  * [2.1]{.pnum} If `OutMat` has `layout_blas_packed` layout, then the
      layout's `Triangle` template argument has the same type as
      the function's `Triangle` template argument;

  * [2.2]{.pnum} _`possibly-multipliable`_`<decltype(A), decltype(transposed(B)), decltype(C)>` is `true`.

  * [2.3]{.pnum} _`possibly-multipliable`_`<decltype(B), decltype(transposed(A)), decltype(C)>` is `true`.

  * [2.4]{.pnum} _`possibly-addable`_`<decltype(C), decltype(E), decltype(C)>` is `true` for those overloads that take an `E` parameter.

[3]{.pnum} *Preconditions:*

  * [3.1]{.pnum} _`multipliable`_`(A, transposed(B), C)` is `true`.

  * [3.2]{.pnum} _`multipliable`_`(B, transposed(A), C)` is `true`.  <i>[Note:</i> This and the previous imply that `C` is square. <i>-- end note]</i>

  * [3.3]{.pnum} _`addable`_`(C, E, C)` is `true` for those overloads that take an `E` parameter.

[4]{.pnum} *Complexity:* $O($ `A.extent(0)` $\cdot$ `A.extent(1)` $\cdot$ `B.extent(0)` $)$

```c++
template<@_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_2k_update(
  InMat1 A,
  InMat2 B,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_2k_update(
  ExecutionPolicy&& exec,
  InMat1 A,
  InMat2 B,
  OutMat C,
  Triangle t);
```

[5]{.pnum} *Effects:* Computes $C = A B^T + B A^T$. 

```c++
template<@_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_2k_update(
  InMat1 A,
  InMat2 B,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_2k_update(
  ExecutionPolicy&& exec,
  InMat1 A,
  InMat2 B,
  OutMat C,
  Triangle t);
```

[6]{.pnum} *Effects:* Computes $C = A B^H + B A^H$.

```c++
template<@_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_in-matrix_@ InMat3,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_2k_update(
  InMat1 A,
  InMat2 B,
  InMat3 E,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_in-matrix_@ InMat3,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void symmetric_matrix_rank_2k_update(
  ExecutionPolicy&& exec,
  InMat1 A,
  InMat2 B,
  InMat3 E,
  OutMat C,
  Triangle t);
```

[7]{.pnum} *Effects:* Computes $C = E + A B^T + B A^T$. 

```c++
template<@_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_in-matrix_@ InMat3,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_2k_update(
  InMat1 A,
  InMat2 B,
  InMat3 E,
  OutMat C,
  Triangle t);
template<class ExecutionPolicy,
         @_in-matrix_@ InMat1,
         @_in-matrix_@ InMat2,
         @_in-matrix_@ InMat3,
         @_possibly-packed-out-matrix_@ OutMat,
         class Triangle>
void hermitian_matrix_rank_2k_update(
  ExecutionPolicy&& exec,
  InMat1 A,
  InMat2 B,
  InMat3 E,
  OutMat C,
  Triangle t);
```

[8]{.pnum} *Effects:* Computes $C = E + A B^H + B A^H$.
