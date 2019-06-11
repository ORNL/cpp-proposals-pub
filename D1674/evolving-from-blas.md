# D1674R0: Evolving a Standard C++ Linear Algebra Library from the BLAS

* Mark Hoemmen (mhoemme@sandia.gov)
* 11 Mar 2019, 01 Apr 2019, 10 Jun 2019

## Introduction

Many different applications depend on linear algebra.  This includes
machine learning, data mining, web search, statistics, computer
graphics, medical imaging, geolocation and mapping, and physics-based
simulations.  Good implementations of seemingly trivial linear algebra
algorithms can perform asymptotically better than bad ones, and can
get much more accurate results.  This is why authors (not just us!)
have proposed adding linear algebra to the C++ Standard Library.

Linear algebra builds on well over 40 years of well-accepted
libraries, on which science and engineering applications depend.  In
fact, there is an actual multiple-language standard for a set of
fundamental operations, called the [Basic Linear Algebra Subprograms
(BLAS)
Standard](http://www.netlib.org/blas/blast-forum/blas-report.pdf).
The BLAS Standard defines a small set of hooks that vendors or experts
in computer architecture can optimize.  Application developers or
linear algebra algorithm experts can then build on these hooks to get
good performance for a variety of algorithms.  The only thing missing
from the BLAS is a modern C++ interface.  Nevertheless, its wide
availability and many highly optimized implementations make it a good
starting point for developing a standard C++ library.

In this paper, we will walk through the design iterations of a typical
C++ linear algebra library that builds on the BLAS.  Our point is to
show how developers might "grow" a C++ linear algebra library
incrementally, solving problems as encountered and gradually
increasing the level of abstraction.  This process will highlight
challenges that C++ developers using the BLAS currently face.  It will
also show features C++ developers need that the BLAS and its
derivatives (like the [LAPACK Fortran
library](http://www.netlib.org/lapack/) do not provide.  Finally, it
will give our views about how simple a C++ "BLAS wrapper" could be and
still be useful.

This paper implicitly argues for inclusion of linear algebra in the
C++ Standard Library.  It is meant to be read as part of the design
justification for our C++ Standard Library proposal D1673R0 ("A free
function linear algebra interface based on the BLAS").

We base this work on our years of experience writing and using linear
algebra libraries, and working with people who have done so for much
longer than we have.  Readers may wish to refer to
[P1417](wg21.link/p1417) for a history of linear algebra library
standardization; it cites first-hand histories of the development of
the BLAS, LINPACK, LAPACK, and Matlab.

## Wrapping the BLAS

Suppose that a developer wants to write an application in portable
C++, that needs to compute dense matrix-matrix products and some other
dense linear algebra operations efficiently.  They discover that
implementing matrix multiply with a na\"ive triply nested loop is slow
for their use cases, and want to try a library.

The C++ Standard Library currently lacks linear algebra operations.
However, our hypothetical C++ developer knows that the Basic Linear
Algebra Subroutines (BLAS) exists.  The BLAS is a standard -- not an
ISO standard like C++, but nevertheless agreed upon by discussion and
votes, by people from many institutions.  The BLAS has been used for
decades by many scientific and engineering libraries and applications.
Many vendors offer optimized implementations for their hardware.
Third parties have written their own optimized implementations from a
combination of well-understood first principles[^1] and automatic
performance tuning.[^2][^3] All this makes the BLAS attractive for our
developer.

The BLAS standard has both C and Fortran bindings, but the [reference
implementation](http://www.netlib.org/blas/#_reference_blas_version_3_8_0)
comes only in a Fortran implementation.  It's also slow; for example,
its matrix-matrix multiply routine uses nearly the same triply nested
loops that a naïve developer would write.  The intent of the BLAS is
that users who care about performance find optimized implementations,
either by hardware vendors or by projects like ATLAS[^2] or the
[GotoBLAS](https://www.tacc.utexas.edu/research-development/tacc-software/gotoblas2).

Suppose that our developer has found an optimized implementation of
the BLAS, and they want to call some of its routines from C++.  Here
are the first two steps they might take.

1. Access the BLAS library.
2. Write a generic BLAS wrapper in C++.

### Access the BLAS library

"Accessing" the BLAS library means both linking to it, and figuring
out how to call it from C++.  For the latter, our developer may start
either with the BLAS' C binding, or with its Fortran binding.

#### Linking to the library

Our developer has two options for the BLAS: its C binding, or its
Fortran binding.  In both cases, they must add the library or
libraries to their link line.  One implementation (Intel's Math Kernel
Library) offers so many options for this case that they offer a [web
site to generate the necessary link
line](https://software.intel.com/en-us/articles/intel-mkl-link-line-advisor).
The multiplicity of options comes in part from the vendor's interest
in supporting different architectures (32- or 64-bit), operating
systems, compilers, and link options.  However, the BLAS
implementation itself has options, like whether it uses OpenMP for
parallelism inside, or whether it uses an LP64 (32-bit `int`) or ILP64
(64-bit `int`) interface.  In the best case, getting these wrong could
cause link-time errors.  Use of the wrong interface through `extern`
declarations could cause run-time errors.

Some developers only need to support one platform, so they rarely need
to figure out how to link to the BLAS.  Other developers need to
support many different platforms, and get good performance on all of
them.  Such developers end up writing build system logic for helping
installers automatically detect and verify BLAS libraries.  The author
has some experience writing and maintaining such build system logic as
part of the [Trilinos](github.com/trilinos/Trilinos) project.

#### C binding

The C BLAS is not as widely available as Fortran BLAS, but many
vendors have optimized implementations of both, so requiring the C
BLAS is not unreasonable.  If they choose the C binding, its header
file will import many symbols into the global namespace.  Best
practice would be to write wrapper functions for the C BLAS functions
they want, and hide the C binding's header file include in a source
file.  If our developer later wants LAPACK functionality, such as
solving linear systems, they will discover that there is no standard C
LAPACK binding.  Implementations that provide a C BLAS generally also
allow calling the Fortran BLAS from the same application, but
developers would still need a C or C++ interface to LAPACK.

One advantage of the C BLAS binding is that it works for both
row-major and column-major matrix data layouts.  Native
multidimensional arrays in C and C++ have row major layout, but the
BLAS' Fortran binding only accepts column-major matrices.  If users
want code that works for both the Fortran and C BLAS interfaces, they
will need to use column-major matrices.  The Standard Library
currently has no way to represent column-major rank-2 arrays (but see
our proposal [P0009](wg21.link/p0009)).  This leads C++ developers to
one of the following solutions:

  - Write C-style code for column-major indexing (e.g., `A[i +
    stride*j]`);
  - use nonstandard matrix or array classes that use column-major
    storage; or
  - cleverly trick the BLAS into working with row-major matrices, by
    specifying that every operation use the transpose of the input
    matrix.

#### Fortran binding

The experience of developers of large scientific libraries is that the
C interface is less often available than the Fortran interface.  For
example, the Reference BLAS only comes in a Fortran version.  Thus,
the most portable approach is to rely on Fortran.  If our developer
links against a Fortran BLAS, they will face the difficult task of
deducing the library's Fortran BLAS ABI.  This is just close enough to
the C ABI to lead innocent developers astray.  For example, Fortran
compilers mangle the names of functions in different ways, that depend
both on the system and on the compiler.  Mangled names may be
all-capitals or lower-case, and many have zero to two underscores
somewhere at the beginning or end of the mangled name.  The BLAS may
have been built with a different compiler than the system's Fortran
compiler (if one is available), and may thus have a different mangling
scheme.  Fortran interfaces pass all input and output arguments,
including integers, by the C ABI equivalent of pointer, unless
otherwise specified.  Fortran ABIs differ in whether they return
complex numbers on the stack (by value) or by initial output argument,
and how they pass strings, both of which come up in the BLAS.  We have
encountered other issues: for instance, functions that claim to return
`float` might actually return `double`.  We have written hundreds of
lines of CMake code to deduce the BLAS ABI, have encountered all of
these situations, and expect to encounter novel ones in the future.
If our developer wants to use LAPACK as well as the BLAS, they will be
stuck with these problems.

In summary:

a. Users may need to deduce BLAS' Fortran ABI; this is hard.

b. Ignorance of some Fortran ABI issues (e.g., return of complex
   values) could cause bugs when porting code to different hardware or
   software.

Once our developer has deduced the Fortran BLAS ABI, they will need to
do the following:

a. Write `extern "C"` declarations for all the BLAS functions they
   want to call, using the deduced ABI to mangle function names and
   correctly handle return values (if applicable).

b. Hide the `extern "C"` declarations and write wrappers to avoid
   polluting the global namespace.

c. Change wrappers to pass integers by value instead of by pointer,
   so that users can pass in integer literals if desired.

### Generic C++ wrapper

Suppose that our developer has solved the problems of the above
section.  They have access to the BLAS in their source code, through a
wrapped C API.  They will next encounter three problems:

* this C API is not type safe;
* it is not generic; and
* it is tied to availability of an external BLAS library.

Our developer's next step for solving these problems would be to write
a generic C++ wrapper, with a fall-back implementation for unsupported
matrix or vector element types, or if the BLAS library is not
available.

Developers who just want to compute one matrix-matrix multiply for
matrices of `double` might be satisfied with a C interface to the
BLAS.  However, if they want to use this interface throughout their
code, they will discover the following problems.  First, the interface
is not type safe.  For example, the C BLAS doesn't know about
`std::complex`, and thus may take pointers to complex numbers as
`_Complex` or even `void*` (an option I have seen in C bindings of
BLAS libraries).  Second, the interface is not generic.  It only works
for four matrix or vector element types: `float`, `double`,
`complex<float>`, and `complex<double>`.  What if our developer wants
to compute with matrices of lower-precision or higher-precision
floating-point types, or custom types?  Most BLAS operations really
only need addition and multiplication, yet the BLAS does not work with
matrices of integers.  The function names also depend on the matrix or
vector element type, which hinders developing generic algorithms.
Third, what if the BLAS is not available?  Users might like a
fall-back implementation, even if it's slow, as a way to remove an
external library dependency or to test an external BLAS.

The logical solution is to write a generic C++ interface to BLAS
operations.  "Generic C++ interface" means that users can call some
function `GEMM` templated on the matrix element type `T`, and the
implementation will either dispatch to the appropriate BLAS call if
`T` is one of the four types that the BLAS supports, or fall back to a
default implementation otherwise.  The fall-back implementation can
even replace the BLAS library.

Libraries like the BLAS and LAPACK were written to be as generic as
possible.  (See our paper [P1417](wg21.link/p1417) for a survey of
their history and the authors' intentions.)  Thus, once one has
figured out the ABI issues, it's not too much more effort to write a
generic C++ wrapper.  For example,
[Trilinos](github.com/trilinos/Trilinos) has one, `Teuchos::BLAS`,
that has served it for the past decade and a half with few changes.

Some subtle issues remain.  For example, some corresponding interfaces
for real and complex numbers differ.  Our developer must decide
whether they should try to present a single interface for both, or
expose the differences.  We have not found this to be a major issue in
practice.

## Introduce C++ data structures for matrices & vectors

Many developers may find the above solution satisfactory.  For
example, the `Teuchos::BLAS` class in
[Trilinos](github.com/trilinos/Trilinos) is a generic C++ wrapper for
the BLAS, as we described above.  It has seen use in libraries and
applications since the mid 2000's, with only a few implementation and
interface changes.  However, many developers are not familiar with the
BLAS, and/or find its interface alien to C++.  The BLAS has no
encapsulation of matrices or vectors, and its functions take many
arguments in a mysterious order.  It's easy to mix up these arguments,
and doing so can cause memory corruption, without the benefit of
bounds or iterator checking that C++ libraries offer.  Furthermore,
users may need to work with column-major data layouts, which are not
idiomatic to C++ and may thus cause bugs.  This suggests that the next
step is to develop C++ data structures for matrices and vectors, and
extend the above BLAS wrapper to use them.

### BLAS routines take many, unencapsulated arguments

Neither the C nor the Fortran binding of the BLAS gives users a way to
encapsulate a matrix or vector in a single data structure.  The four
BLAS routines for matrix-matrix multiply all have the following form:
```
xGEMM(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC)
```
where the initial x is one of S, D, C, or Z, depending on the matrix
element type.  The arguments have the following types:

* `TRANSA` and `TRANSB` are character arrays of length at least 1;

* `M`, `N`, `K`, `LDA`, `LDB`, and `LDC` are integers;

* `ALPHA` and `BETA` are scalar values of the same type as the matrix
  element type, and

* `A`, `B`, and `C` are rank-2 Fortran arrays with run-time
  dimensions.

These routines can perform several different operations, which their
documentation represents as `C = alpha*op(A)*op(B) + beta*C`.  Here,

* `C` is the input and output matrix;

* `A` and `B` are the two input matrices;

* `C` is `M` by `N`, `A` is `M` by `K`, and `B` is `K` by `N`;

* and `op(X)` represents `X`, the transpose of `X`, or the conjugate
  transpose of `X`, depending on the corresponding `TRANSX` argument.
  `TRANSA` and `TRANSB` need not be the same.  For real matrix element
  types, the conjugate transpose and transpose mean the same thing.

The BLAS has a consistent system for ordering these arguments, that a
careful read of the [BLAS Quick Reference
chart](http://www.netlib.org/blas/blasqr.pdf) should suggest:

1. Character arrays that modify behavior, if any;

2. Matrix and/or vector dimensions; then

3. Constants and matrices / vectors, in the order in which they appear
   in the right-hand side of the algebraic expression to evaluate.
   After each matrix / vector comes its stride argument.

However, most users are not familiar with the BLAS' conventions.  We
_are_, and nevertheless we have found it easy to mix up these
arguments.  Users need to read carefully to see which of M, N, and K
go with A, B, or C.  Should they reverse these dimensions if taking
the transpose of A or B?  In some cases, the BLAS will check errors
for you and report the first argument (by number) that is wrong.  In
other cases, the BLAS may crash or get the wrong answer.  Since the
BLAS is a C or Fortran library, whatever debug bounds checking you
have on your arrays won't help.  It may not have been built with debug
symbols, so run-time debuggers may not help.  Developers who haven't
done a careful job wrapping the BLAS in a type-safe interface will
learn the hard way, for example if they mix up the order of arguments
and their integers get bitwise reinterpreted as pointers.

### Matrix and vector data structures

The C++ Standard Library does not currently provide a BLAS-compatible
matrix data structure, with column-major storage, including dimensions
and stride information.  The language's native two-dimensional arrays
cannot have run-time dimensions and also promise contiguous or
constant-stride (row-major or column-major) storage.  Other array-like
data structures in the Standard Library are only one dimensional.  The
`valarray` class and its slice classes were meant to serve as a
building block for vectors and matrices.  **[class.slice.overview]**
calls `slice` "a BLAS-like slice from an array."  However, none of
these classes can represent a matrix without additional information.

The lack of general multidimensional array data structures in C++ led
us to propose `mdspan` [(P0009)](wg21.link/p0009).  An `mdspan` with
the appropriate layout can represent a dense column-major matrix.
(P0009's `layout_left` corresponds to a column-major matrix with the
same stride between columns as the number of rows.  Its
`layout_stride` is too general, since it permits nonunit stride
between consecutive entries in a column.  It would not be hard to
write a "`layout_blas`" layout that generalizes `layout_left` to
permit a stride larger than the number of rows.  Our proposal D1673
does this.)  Thus, `mdspan` could encapsulate the pointer, dimensions,
and stride arguments to BLAS functions that represent a matrix.
Unlike `valarray` slices, `mdspan` can even represent dimensions and
strides as compile-time values.  We did not intend `mdspan` to satisfy
all the needs of a matrix data structure, but we think it makes sense
to use `mdspan` as the minimal common interface for many different
data structures to interact with libraries like the BLAS.

This suggests that the next stage of a C++ linear algebra library
would be a generic C++ wrapper for the BLAS, that takes matrices and
vectors as `mdspan` of the appropriate ranks.

## Some remaining performance issues

Suppose that at this point, you have a generic C++ BLAS wrapper that
takes `mdspan`.  It calls into the BLAS (C or Fortran) for layouts and
data types for which that is possible, and has fall-back
implementations for other layouts and data types.  Our developer could
even get rid of the BLAS library without losing functionality.  One
could write an implementation of BLAS 3 operations, like dense
matrix-matrix multiply, directly to a matrix abstraction like
`mdspan`, and still get performance close to that of a fully optimized
vendor-implemented BLAS library.  See Andrew Lumsdaine’s Kona 2019
presentation, for example [CITE].

The authors would be ecstatic to have a product like this available in
the Standard Library.  Nevertheless, some developers would encounter
performance issues that relate to limitations in the design of the
BLAS' interface itself.  Here are three such issues:

a. It is not optimized for tiny matrices and vectors.

b. It has no way for users to control composition of parallelism, such
   as what parallelism policy to use, or to control nested
   parallelism.

c. Implementations have no way to optimize across multiple linear
   algebra operations.

### Tiny matrices and vectors

The BLAS interface is not optimal for solving tiny problems as fast as
possible, for the following reasons:

1. The BLAS is a library; it cannot be inlined.

2. The BLAS standard requires error checking and reporting, through a
   user-replaceable subroutine `XERBLA`.  For small problems, error
   checking might take more time than actually solving the problem.

3. The BLAS takes arrays by pointer, with run-time dimensions.
   Neither is optimal for very small matrices and vectors.

4. The BLAS only solves one problem at a time; it does not have a
   "batched" interface for solving many small problems at a time.

It turns out that our hypothetical developer has already laid the
groundwork for solving the first three problems.  For example,
`mdspan` permits compile-time dimensions, or mixes of compile-time and
run-time dimensions.  Our developer could start with the C++
"fall-back" implementation of BLAS operations, and turn it into an
inlineable optimized implementation for certain combinations of
compile-time dimensions.

The `mdspan` class is a view.  Implementations store a pointer.  Thus,
it is not totally zero overhead for very small matrices or vectors
with compile-time dimensions.  A zero-overhead solution would *only*
store the data at run time, not a pointer to the data; `std::array` is
an example.  However, we could either fix this in `mdspan`, or use a
different data structure entirely.  Once we have C++ BLAS wrappers
that take `mdspan`, it's not much more work to support other matrix
and vector data structures.  This would also help our developer make
their linear algebra library more like the C++ Standard Library, in
that its algorithms would be decoupled from particular data
structures.

The `mdspan` class also gives developers efficient ways to represent
batches of linear algebra problems with the same dimensions.  For
example, one may store a batch of matrices as a rank-3 `mdspan`, where
one of the dimensions indexes into a particular matrix.  The various
layouts and possibility of writing a custom layout make it easier to
write efficient batched code.  We have some experience doing so in our
libraries [CITE].

### Composition of parallelism

Our developer may want to write a thread-parallel application.  What
happens if their BLAS implementation is thread parallel as well?  This
introduces two possible problems:

1. The BLAS' threads might fight with the application's threads, even
   while the application is not calling the BLAS.

2. The application may need to call the BLAS inside of a
   thread-parallel region.  It may want to specify a subset of the
   computer's parallel hardware resources on which the BLAS should
   run, or it may not care what the BLAS does, as long as it doesn't
   make the calling parallel code slower.

BLAS implementations may use their own thread parallelism inside their
library.  This may involve OpenMP (as with Intel's MKL) or a custom
Pthreads-based back-end.  The only way to know is to read the
implementation's documentation, and it may not be easy to control what
it does at run time.  (Some implementations use Unix-style environment
variables to set the number of threads, for example.)

What if our hypothetical developer wants to use thread parallelism in
their own code?  Their thread parallelism run-time system or
implementation might fight with the BLAS' threads, even if our
developer never calls the BLAS in a thread-parallel region.  For
example, one way to construct a thread pool is to pin threads to cores
and have each thread spin-lock until work arrives.  If both the BLAS
and our developer do that, the two thread pools will fight constantly
over resources.

BLAS implementations that use OpenMP generally play nicely with the
caller's use of OpenMP, as long as the caller uses the same compiler
and links with the same version of the OpenMP run-time library [CITE].
Even if the two thread pools play nicely together, what happens if our
developer calls the BLAS inside of a parallel region?  Intel's MKL
recognizes this by using the OpenMP API to determine whether it is
being called inside of a parallel region.  If so, it reverts to
sequential mode.  A smarter implementation could instead farm out work
to other threads, if it makes sense for good performance.  However,
solving this at a system level, without our developer needing to
change both their code and that of the BLAS implementation, is hard.
See, for example, the 2010 MIT PhD dissertation by Heidi Pan,
["Cooperative Hierarchical Resource Management for Efficient
Composition of Parallel Software."]
(http://lithe.eecs.berkeley.edu/pubs/pan-phd-thesis.pdf)

Our developer could help by providing an execution policy (in the C++
Standard Library sense) that tells the BLAS what subset of parallel
resources it may use.  This is a logical extension of the C++ linear
algebra interface we have been developing.  Just like C++ Standard
Algorithms, our developer's library could take optional execution
policies.  The optional execution policy parameter would also serve as
an extension point for an interface that supports hierarchical
parallelism (a "team-level BLAS").  That could also help with code
that wants to solve many tiny linear algebra problems in parallel (see
above section).

### Optimize across operations

The functions in the BLAS only perform one linear algebra operation at
a time.  However, in many cases one can improve performance by doing
multiple operations at a time.  That would let the implementation fuse
loops and reuse more data, and would also amortize parallel region
launch overhead in a parallel implementation.

#### Provide specialized fused kernels?

One way to do this is simply to expand the set of operations in the
interface, to include more specialized "fused kernels."  The BLAS
already does this; for example, matrix-vector multiply is equivalent
to a sequence of dot products.  BLAS 2 and 3 exist in part for this
reason.

This approach can work well if the set of operations to optimize is
small.  [CITE: Rich Vuduc's dissertation.]  The opaque interface to
fused kernels gives developers complete freedom to optimize.  However,
it also makes the interface bigger and harder to understand.  Users
may miss optimization opportunities because they failed to read that
last page of documentation with the fancy fused kernels.  Thus, the
more general the intended audience for a linear algebra library, the
less value specialized fused kernels may have.

#### Expression templates?

Many linear algebra libraries use expression templates to optimize
sequences of linear algebra operations.  See P1417 for an incomplete
list.  Expression templates have the advantage that they let users
write in a notation more like mathematics, while avoiding allocation
of temporaries.  A major motivation for P1385's use of arithmetic
operator overloading is that the game developer community considers
concise mathematical notation an essential feature of a linear algebra
library.

We will consider two examples of linear algebra expressions:

1. Compute the 2-norm of w, where w = alpha*x + beta*y + gamma*z (x,
   y, z, and w are vectors, and alpha, beta, and gamma are scalar
   constants).

2. Compute D = A*B*C, where A, B, and C are matrices with run-time
   dimensions, `operator*` indicates the matrix-matrix product, and
   B*C has many more columns than D.

```
// Example of (2): A*B*C*x: what if A is short and fat, B tall and
// skinny, C square?  intermediate vector could be longer than output
// vector -- need temporary space.  Matlab users just type that and let
// Matlab allocate intermediate temporaries.
```

##### First example

The first example is a classic use case for expression templates.  As
long as all vectors exist and have the same length, one can write an
expression templates library that computes the entire expression in a
single loop, without allocating any temporary vectors.  In fact, one
could even skip storage of the intermediate vector w, if one only
wanted its norm.

This example may suggest to some readers that we don't even need this
expression templates library.  What if a future version of Ranges were
to accept parallel execution policies, and work with
`transform_reduce`?  Wouldn't that make a expression templates library
for linear algebra just a pinch of syntactic sugar?  Such readers
might go on to question the value of a BLAS 1 interface.  Why do we
need dot products and norms?

Computing norms and dot products accurately and without unwarranted
underflow or overflow is tricky.  Consider the norm of a vector whose
elements are just a little bit larger than the square root of the
overflow threshold, for example.  The reference implementation of the
BLAS does extra scaling in its 2-norm and dot product implementations,
in order to avoid unwarranted overflow.  BLAS 2 and 3 operations are
like many dot products, and thus share these concerns.

Even if C++ already has the tools to implement something, if it's
tricky to implement well, that can justify separate standardization.
The Standard Library already has the special math functions, which
arguably have a smaller set of users than dot products and norms.

##### Second example

The second example shows the limits of expression templates in
optimizing linear algebra expressions.  The compiler can't know the
optimal order to perform the matrix-matrix multiplications.  Even if
it knew the dimensions at compile time, optimizing the general problem
of a chain of matrix multiplications is a nontrivial algorithm that
could be expensive to evaluate at compile time.  In this case, if the
compiler evaluates B*C first, it would need to allocate temporary
storage, since D is not large enough.  Introducing `operator*` for
matrix-matrix multiplcation thus forces the implementation to have
storage for temporary results.  That's fine for some users.  Others
could reach for named-function alternatives to arithmetic operators,
to write lower-level code that does not allocate temporary storage.

When LAPACK moved from Fortran 77 to a subset of Fortran 90 in the
2000's, LAPACK developers polled users to ask if they wanted the
interface to allocate scratch space internally (using Fortran 90
features).  Users rejected this option; they preferred to manage their
own scratch space.  Thus, having the option of a lower-level interface
is important for a general user community.

##### Expression rewriting with named functions

One way to avoid creation of temporaries in such expressions, is to
combine named functions with expression templates.  Such functions
would pass an optional "continuation object" into the next expression.
This approach would still enable high-level expression rewriting (like
Eigen or uBLAS).  It would also be agnostic to whether expression
rewriting happens at compile time or run time.  For example, for some
operations with large sparse matrices and dense vectors, it could make
sense to have a run-time expression rewriting engine.  Compile-time
expression templates might increase build time, so it would be helpful
to have the option not to do them.

For example, suppose that someone wants to evaluate the dot product of
two vectors y and z, where z = A*x for a matrix A and vector x.  In
Matlab, one would write `y'*(A*x)`.  In our hypothetical
named-function library, we could write something like this:

#+BEGIN_SRC C++
auto A_times_x = make_expression(z, prod(A, x));
double result = dot(y, A_times_x);
#+END_SRC

or perhaps like this instead:

#+BEGIN_SRC C++
double result = dot(y, prod(z, A, x));
#+END_SRC

The library would have the option to use z as temporary storage, but
would not be required to do so.

There are two issues with this approach.  First, users would need to
query the library and the particular expression to know whether they
need to allocate temporary storage.  Second, expression templates in
general have the issue of dangling references; see next section.

##### The dangling references problem

Expression templates implementations may suffer from the *dangling
references problem*.  That is, if expressions do not share ownership
of their "parent" linear algebra objects' data, then if one permits
(say) a matrix expression to escape a scope delimiting the lifetime of
its parent matrix, the expression will have a dangling reference to
its parent.  This is why, Eigen's documentation recommends against
assigning linear algebra expressions to `auto` variables.  This is an
old problem: `valarray` may have the same issue, if the implementation
takes the freedom afforded to it by the Standard to use expression
templates.

Copy elision makes it impossible for an expression type to tell
whether it's being returned from a scope.  Otherwise, the expression
type could do something to claim ownership.  One possible fix is for
linear algebra objects to have `shared_ptr`-like semantics, so that
expressions share ownership of the objects they use.  However, that
introduces overhead, especially for the case of tiny matrices and
vectors.  It's not clear how to solve this problem in general,
especially if linear algebra objects may have container semantics.

##### The aliasing problem and opting out of lazy evaluation

TODO finish this section

Suppose that `A` is a matrix, and users evaluate the matrix-matrix
multiplication `A = A*A`.  The expression `A = A + A` could work fine
with expression templates, since matrix addition has no dependencies
across different entries of the matrix.  Computing `A(i,j)` on the
left-hand side only requires reading the value `A(i,j)` on the
right-hand side.  In contrast, for the matrix multiplication `A =
A*A`, computing `A(i,j)` on the left-hand side requires `A(i,k)` and
`A(k,j)` on the right-hand side, for all valid `k`.  This makes it
impossible to compute `A = A*A` in place correctly; the implementation
needs a temporary matrix in order to get the right answer.
Implementations have a few options:

* They could *forbid* aliasing (possibly detecting it);

* they could allocate their own own temporary space; or

* they could simply not provide expressions that have this problem.
  Users would instead call a function and provide input and output
  argument.

C++ expression templates are one way to implement *lazy evaluation* of
arithmetic expressions involving linear algebra objects.  Natural
implementations of expression templates lead to fully lazy evaluation:
No actual computation happens until assignment.  However, multiple C++
linear algebra libraries have learned the lesson that fully lazy
evaluation does not always pay.  For example, for algorithms with
reuse, like dense matrix-matrix multiply, lazy evaluation can give
incorrect results for expressions where the output alias the input
(like `A = A * A` where `A` is a matrix).

This is why the Eigen library's expressions have an option
to turn off lazy evaluation, and do so by default for some kinds of
expressions.  Furthermore, allocating a temporary result and/or eager
evaluation of subexpressions may be faster in some cases.\footnote{See
  \url{http://eigen.tuxfamily.org/dox/TopicLazyEvaluation.html}.}
This is true not necessarily just for expressions whose computations
have significant reuse, like matrix-matrix multiply, but also for some
expressions that ``stream'' through the entries of vectors or
matrices.  For example, fusing too many loops may thrash the cache or
cause register spilling, so deciding whether to evaluate eagerly or
lazily may require hardware-specific information
\cite{siek2008build}. It's possible to encode many such compilation
decisions in a pure C++ library with architecture-specific
parameters.\footnote{See, e.g., the following 2013 Eigen presentation:
  \url{http://downloads.tuxfamily.org/eigen/eigen_CGLibs_Giugno_Pisa_2013.pdf}.
  For comparable \texttt{Boost.uBlas} optimizations, see Section
  \ref{S:uBlas}.}  However, this burdens the library with higher
implementation complexity and increased compilation time.  Library
designers may prefer a simpler interface that excludes expressions
with reuse (that have correctness issues with lazy evaluation) and
lets users decide where temporaries get allocated.

##### How general should expression rewriting be?

We might also ask how many users need to optimize many, different,
complicated expressions, at compile time.  Would they be better served
by specialized source-to-source translation tools?

Different application areas have different interpretations of "matrix"
vs. "vector," row vs. column vectors, and especially `operator*` (dot?
outer? Hadamard? element-wise? Kronecker?).  This makes introducing
arithmetic operators for matrices and vectors a bit risky.  Many users
want this, but it's important to get agreement on what the various
operators mean.

## Some remaining correctness issues

a. Does mixing precisions, real and complex, etc. do the right thing?
b. Some kinds of number types may call for a different interface.

### Mixing precisions in linear algebra expressions

Suppose that a linear algebra library implements arithmetic operators
on matrices, and users write "A * B" for a matrix A of
`complex<float>`, and a matrix B of `double`.  What should the matrix
element type of the returned matrix be?  The authors of P1385 assert
that the returned matrix's element type should be complex double,
since that would preserve accuracy.  Note that `common_type` does
*not* preserve accuracy in this case: the `common_type` of
`complex<float>` and `double` is `complex<float>`.  This is one reason
why P1385 needs matrix type promotion traits.

This issue may arise even if the linear algebra library never needs to
deduce the return type of an expression.  For example, users may need
care to avoid undesired promotions or compiler errors, such as mixing
superficially harmless `double` constants like 1.0 (or integers
promoted to `double` constants!) with matrices of non-floating-point
types.  Implementers of mixed-precision libraries also need to watch
out for bugs that silently reduce precision.  See e.g., [this bug in a
library to which I
contribute](https://github.com/kokkos/kokkos-kernels/issues/101).
Internal expressions in implementations may need to deduce
extended-precision types for intermediate values.  For example, a
double-precision expression may want to reach for `long double` or
`real128` types to reduce rounding error.  This is especially
important for the shorter floating-point types that people want to use
in machine learning.  This may call for traits machinery that doesn’t
exist in the C++ Standard Library yet.

There's no general solution to this problem.  Both users and
implementers of mixed-precision linear algebra need to watch out, and
implementations will need to consult with SG6 and possibly present new
proposals there.

### Different interface for some kinds of number types?

Given a generic linear algebra library, users will put all sorts of
number types into it.  However, the BLAS was designed for real and
complex floating-point numbers.  Does a BLAS-like interface make sense
for matrices of fixed-point numbers?  Should such matrices carry along
scaling factors, for example?  What about short floating-point types
that exclude Inf and NaN to make room for more finite numbers?  Would
that call for linear algebra operations that track underflow and
overflow?  How would such interfaces integrate with C++ versions of
libraries like LAPACK?  We are not experts on fixed-point arithmetic;
for us, these are all open questions.


## References

[^1]: K. Goto and R. A. van de Geijn, "Anatomy of high-performance
matrix multiplication", ACM Transactions of Mathematical Software
(TOMS), Vol. 34, No. 3, May 2008.

[^2]: J. Bilmes, K. Asanovíc, J. Demmel, D. Lam, and C. W. Chin,
"PHiPAC: A Portable, High-Performance, ANSI C Coding Methodology and
its application to Matrix Multiply," LAPACK Working Note 111, 1996.

[^3]: R. C. Whaley, A. Petitet, and J. Dongarra, "Automated Empirical
Optimization of Software and the ATLAS Project," Parallel Computing,
Vol. 27, No. 1-2, Jan. 2001, pp. 3-35.