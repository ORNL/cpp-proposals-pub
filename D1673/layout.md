The tag `upper_triangle_t` indicates that algorithms or other users of
the view with that tag ("viewer") will only access the upper triangle
of the matrix being viewed ("viewee") by the viewer.  That is, if the
viewee is `A`, then the viewee will only access the element `A(i,j)`
if `i >= j`.

This is also subject to the restrictions of
`implicit_unit_diagonal_t` if that tag is also applied; see below.

The tag `lower_triangle_t` indicates that algorithms or other users of
the view with that tag ("viewer") will only access the lower triangle
of the matrix being viewed ("viewee") by the viewer.  That is, if the
viewee is `A`, then the viewee will only access the element `A(i,j)`
if `i <= j`.  This is also subject to the restrictions of
`implicit_unit_diagonal_t` if that tag is also applied; see below.

##### Packed storage tags

```c++
struct unpacked_storage_t {};
constexpr unpacked_storage_t unpacked_storage =
  unpacked_storage_t ();

struct packed_storage_t {};
constexpr packed_storage_t packed_storage =
  packed_storage_t ();
```

These tag classes specify whether the matrix uses a "packed" storage
format.

The `unpacked_storage_t` tag indicates that the matrix to which the
tag is applied has any of the unpacked storage formats, that is, any
format other than the packed format described in this section.

The `packed_storage_t` tag indicates that the matrix to which the tag
is applied has a packed storage format, which we describe in this
section.

The actual "packed" format depends on

* whether the format represents the upper or lower triangle of the
  matrix (see "Triangle tags" above), and

* whether the format uses column-major or row-major ordering to store
  its entries (see "Storage order tags" above).

---

#### New unpacked symmetric and triangular layouts

These layouts make sense for rank-2 and rank-3 `mdspan` and `mdarray`.
In the rank-3 case, they would represent a batch of matrices, where
the leftmost dimension indicates which matrix.

##### Requirements

Throughout this Clause, where the template parameters are not
constrained, the names of template parameters are used to express type
requirements.

  * `Triangle` is either `upper_triangle_t` or `lower_triangle_t`.

  * `DiagonalStorage` is either `implicit_unit_diagonal_t` or
    `explicit_diagonal_t`.

  * `BaseLayout` is a unique, contiguous, and strided layout.

  * `Extents` is a specialization of `extents` (see P0009R9).

##### Unpacked symmetric layout

```c++
template<class Triangle,
         class BaseLayout>
class layout_blas_symmetric {
public:
  using triangle_type = Triangle;
  using base_layout_type = BaseLayout;

  template<class Extents>
  class mapping {
  public:
    using base_mapping_type = base_layout_type::template mapping<Extents>;

    constexpr mapping() noexcept;
    constexpr mapping(const mapping&) noexcept;
    constexpr mapping(mapping&&) noexcept;

    constexpr mapping(const base_mapping_type& base_mapping) noexcept :
      base_mapping_ (base_mapping) {}

    template<class OtherExtents>
      constexpr mapping(const mapping<OtherExtents>& other);

    mapping& operator=() noexcept = default;
    mapping& operator=(const mapping&) noexcept = default;

    template<class OtherExtents>
      constexpr mapping& operator=(const mapping<OtherExtents>& other);

    Extents extents() const noexcept;

    const base_mapping_type base_mapping() const {
      return base_mapping_;
    }

    constexpr typename Extents::index_type required_span_size() const noexcept;

    template<class... Indices>
      typename Extents::index_type operator()(Indices... is) const;

    static constexpr bool is_always_unique() noexcept;
    static constexpr bool is_always_contiguous() noexcept;
    static constexpr bool is_always_strided() noexcept;

    constexpr bool is_unique() const noexcept;
    constexpr bool is_contiguous() noexcept;
    constexpr bool is_strided() noexcept;

    template<class OtherExtents>
      constexpr bool
      operator==(const mapping<OtherExtents>& other) const noexcept;
    template<class OtherExtents>
      constexpr bool
      operator!=(const mapping<OtherExtents>& other) const noexcept;

  private:
    base_mapping_type base_mapping_; // <i>exposition only</i>
  };
};
```

* *Constraints:* In `mapping`, `Extents::rank()` is at least 2.

We will need to fill in the details here.  Most of them can be copied
from P0009.  We will highlight a few key features that differ from the
other layouts in P0009.

###### `layout_blas_symmetric::mapping` operations

```c++
constexpr typename Extents::index_type
required_span_size() const noexcept;
```

* *Returns:* The product of `base_mapping().extents().extent(r)`
   for all `r` in the range `[0, extents().rank())`.

```c++
template<class... Indices>
  typename Extents::index_type operator()(Indices... is) const;
```

Let `i,j` be the last two (rightmost) indices in the `is` parameter
pack.  Let `is_rev` denote the parameter pack that is the same as
`is`, except that the last two (rightmost) indices appear in reverse
order.

* *Constraints:*

  * `sizeof...(Indices)` equals `Extents::rank()`, and

  * `(is_convertible_v<Indices, typename Extents::index_type> && ...)`
    is `true`.

* *Requires:* `0 <= array{is...}[r] < extents().extent(r)` for all `r`
  in the range `[0, extents().rank())`.

* *Returns:*

  * If `Triangle` is `upper_triangle_t`, then:

    * If `i` <= `j`, then returns `base_mapping_(is)`;

    * Else, returns `base_mapping_(is_rev)`.

  * If `Triangle` is `lower_triangle_t`, then:

    * If `i` >= `j`, then returns `base_mapping_(is)`;

    * Else, returns `base_mapping_(is_rev)`.

* *Throws:* Nothing.

*[Note:* Algorithms may avoid the overhead of checking and swapping
indices by accessing the base mapping. --*end note]*


```c++
static constexpr bool is_always_unique() noexcept;
static constexpr bool is_always_contiguous() noexcept;
static constexpr bool is_always_strided() noexcept;
```

* *Returns:* false.

```c++
constexpr bool is_unique() const noexcept;
constexpr bool is_contiguous() noexcept;
constexpr bool is_strided() noexcept;
```

* *Returns:* `true` if and only if the product of the extents is less
  than or equal to 1.

##### Unpacked triangular layout

```c++
template<class Triangle,
         class DiagonalStorage,
         class BaseLayout>
class layout_blas_triangular {
public:
  using triangle_type = Triangle;
  using diagonal_storage_type = DiagonalStorage;
  using base_layout_type = BaseLayout;

  template<class Extents>
  class mapping {
  public:
    using base_mapping_type = base_layout_type::template mapping<Extents>;

    constexpr mapping() noexcept;
    constexpr mapping(const mapping&) noexcept;
    constexpr mapping(mapping&&) noexcept;

    constexpr mapping(const base_mapping_type& base_mapping) noexcept :
      base_mapping_ (base_mapping) {}

    template<class OtherExtents>
      constexpr mapping(const mapping<OtherExtents>& other);

    mapping& operator=() noexcept = default;
    mapping& operator=(const mapping&) noexcept = default;

    template<class OtherExtents>
      constexpr mapping& operator=(const mapping<OtherExtents>& other);

    Extents extents() const noexcept;

    const base_mapping_type base_mapping() const {
      return base_mapping_;
    }

    constexpr typename Extents::index_type required_span_size() const noexcept;

    template<class... Indices>
      typename Extents::index_type operator()(Indices... is) const;

    static constexpr bool is_always_unique() noexcept;
    static constexpr bool is_always_contiguous() noexcept;
    static constexpr bool is_always_strided() noexcept;

    constexpr bool is_unique() const noexcept;
    constexpr bool is_contiguous() noexcept;
    constexpr bool is_strided() noexcept;

    template<class OtherExtents>
      constexpr bool
      operator==(const mapping<OtherExtents>& other) const noexcept;
    template<class OtherExtents>
      constexpr bool
      operator!=(const mapping<OtherExtents>& other) const noexcept;

  private:
    base_mapping_type base_mapping_; // <i>exposition only</i>
  };
};
```

* *Constraints:* In `mapping`, `Extents::rank()` is at least 2.

We will need to fill in the details here.  Most of them can be copied
from P0009.  We will highlight a few key features that differ from the
other layouts in P0009.  There is also a major design question about
accesses "outside the triangle":

1. Should they be forbidden; or

2. should they return some flag value?  If so, should the Accessor

   a. treat that as an error code, or

   b. should it interpret the flag value as "return one if on the
      diagonal, else return zero"?

Approach 2.b lends itself nicely to generic programming, but only for
read-only access to the matrix.  Thus, we prefer (1) as the simplest
approach.

###### `layout_blas_triangular::mapping` operations

```c++
template<class... Indices>
  typename Extents::index_type operator()(Indices... is) const;
```

Let `i,j` be the last two (rightmost) indices in the `is` parameter
pack.

* *Constraints:*

  * `sizeof...(Indices)` equals `Extents::rank()`, and

  * `(is_convertible_v<Indices, typename Extents::index_type> && ...)`
    is `true`.

* *Requires:* **(SEE DESIGN QUESTION ABOVE)**

  * `0 <= array{is...}[r] < extents().extent(r)` for all `r` in the
    range `[0, extents().rank())`.

  * If `Triangle` is `upper_triangle_t`, then `i` is less than or
    equal to `j`.

  * If `Triangle` is `lower_triangle_t`, then `i` is greater than or
    equal to `j`.

  * If `DiagonalStorage` is `implicit_unit_diagonal_t`, then `i` does
    not equal `j`.

* *Returns:*

  * If `Triangle` is `upper_triangle_t`, then:

    * If `DiagonalStorage` is `implicit_unit_diagonal_t`, then:

      * If `i` < `j`, then returns `base_mapping_(is)`;

    * Else, if `DiagonalStorage` is `explicit_diagonal_t`, then:

      * If `i` <= `j`, then returns `base_mapping_(is)`.

  * If `Triangle` is `lower_triangle_t`, then:

    * If `DiagonalStorage` is `implicit_unit_diagonal_t`, then:

      * If `i` > `j`, then returns `base_mapping_(is)`;

    * Else, if `DiagonalStorage` is `explicit_diagonal_t`, then:

      * If `i` >= `j`, then returns `base_mapping_(is)`.

* *Throws:* Nothing.

*[Note:* Algorithms may avoid any index checking overhead by accessing
the base mapping. --*end note]*


---
### Layout views

BLAS functions often take an existing matrix or vector data structure,
and "view" a subset of its elements as another data structure.  For
example, the triangular solve functions take a square matrix in
General (GE) storage format, and view either its upper or lower
triangle (TR format).  Matrix factorizations exploit this.  For
instance, LU (Gaussian elimination) computes the lower triangular
factor L with an implicitly stored diagonal of ones ("unit diagonal"),
so that it can overwrite the original matrix A with both L and the
upper triangular factor U.

One of our design goals is to reduce the number of BLAS function
parameters.  We can eliminate the `UPLO` (upper or lower triangle) and
`DIAG` (implicit unit diagonal or explicitly stored diagonal)
parameters by letting callers annotate an existing General-format
matrix argument with a new layout that encapsulates both parameters.

#### Requirements

Throughout this Clause, where the template parameters are not
constrained, the names of template parameters are used to express type
requirements.

  * `Extents::rank()` is at least 2.

  * `BaseLayout` is a unique, contiguous, and strided layout.

  * `Triangle` is either `upper_triangle_t` or `lower_triangle_t`.

  * `DiagonalStorage` is either `implicit_unit_diagonal_t` or
    `explicit_diagonal_t`.

  * `StorageOrder` is either `column_major_t` or `row_major_t`.

#### Create an unpacked symmetric view of an existing object

```c++
// View the input's upper triangle

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_symmetric<
    upper_triangle_t,
    BaseLayout>,
  Accessor>
symmetric_view(
  basic_mdspan<EltType, Extents, BaseLayout, Accessor> m,
  upper_triangle_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_symmetric<
    upper_triangle_t,
    BaseLayout>,
  Accessor>
symmetric_view(
  basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  upper_triangle_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<const EltType, Extents,
  layout_blas_symmetric<
    upper_triangle_t,
    BaseLayout>,
  Accessor>
symmetric_view(
  const basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  upper_triangle_t);

// View the input's lower triangle

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_symmetric<
    lower_triangle_t,
    BaseLayout>,
  Accessor>
symmetric_view(
  basic_mdspan<EltType, Extents, BaseLayout, Accessor> m,
  lower_triangle_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_symmetric<
    lower_triangle_t,
    BaseLayout>,
  Accessor>
symmetric_view(
  basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  lower_triangle_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<const EltType, Extents,
  layout_blas_symmetric<
    lower_triangle_t,
    BaseLayout>,
  Accessor>
symmetric_view(
  const basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  lower_triangle_t);
```

#### Create an unpacked triangular view of an existing object

```c++
// Upper triangular, explicit diagonal

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    upper_triangle_t,
    explicit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdspan<EltType, Extents, BaseLayout, Accessor> m,
  upper_triangle_t,
  explicit_diagonal_t d = explicit_diagonal);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    upper_triangle_t,
    explicit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  upper_triangle_t,
  explicit_diagonal_t d = explicit_diagonal);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<const EltType, Extents,
  layout_blas_triangular<
    upper_triangle_t,
    explicit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  const basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  upper_triangle_t,
  explicit_diagonal_t d = explicit_diagonal);

// Upper triangular, implicit unit diagonal

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    upper_triangle_t,
    implicit_unit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdspan<EltType, Extents, BaseLayout, Accessor> m,
  upper_triangle_t,
  implicit_unit_diagonal_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    upper_triangle_t,
    implicit_unit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  upper_triangle_t,
  implicit_unit_diagonal_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<const EltType, Extents,
  layout_blas_triangular<
    upper_triangle_t,
    implicit_unit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  const basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  upper_triangle_t,
  implicit_unit_diagonal_t);

// Lower triangular, implicit unit diagonal

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    lower_triangle_t,
    implicit_unit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdspan<EltType, Extents, BaseLayout, Accessor> m,
  lower_triangle_t,
  implicit_unit_diagonal_t d = implicit_unit_diagonal);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    lower_triangle_t,
    implicit_unit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  lower_triangle_t,
  implicit_unit_diagonal_t d = implicit_unit_diagonal);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<const EltType, Extents,
  layout_blas_triangular<
    lower_triangle_t,
    implicit_unit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  const basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  lower_triangle_t,
  implicit_unit_diagonal_t d = implicit_unit_diagonal);

// Lower triangular, explicit diagonal

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    lower_triangle_t,
    explicit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdspan<EltType, Extents, BaseLayout, Accessor> m,
  lower_triangle_t,
  explicit_diagonal_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<EltType, Extents,
  layout_blas_triangular<
    lower_triangle_t,
    explicit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  lower_triangle_t,
  explicit_diagonal_t);

template<class EltType, class Extents, class BaseLayout,
         class Accessor>
constexpr basic_mdspan<const EltType, Extents,
  layout_blas_triangular<
    lower_triangle_t,
    explicit_diagonal_t,
    BaseLayout>,
  Accessor>
triangular_view(
  const basic_mdarray<EltType, Extents, BaseLayout, Accessor>& m,
  lower_triangle_t,
  explicit_diagonal_t);
```

---


  * `DiagonalStorage` is either `implicit_unit_diagonal_t` or
    `explicit_diagonal_t`.

---


*[Note:* These functions correspond to the BLAS functions `xGEMV`,
`xGBMV`, `xHEMV`, `xHBMV`, `xHPMV`, `xSYMV`, `xSBMV`, `xSPMV`, and
`xTRMV`. --*end note]*

---


### `triangular_view` redefines `mdspan` "domain"; what to do?

The `triangular_view` function returns an `basic_mdspan`.  However,
its "domain" -- the set of its valid index pairs `i,j` -- does not, in
general, include all pairs in the Cartesian product of its extents.
This differs from the definition of *domain* in
[P0009](wg21.link/p0009).  In what follows, we will call this an
*incomplete domain*.

It may help to compare this with sparse matrix data structures,
especially the class of sparse matrix data structures that forbid
"structure changes."  The sparse matrix comes with a set of
multi-indices -- the "populated elements" P -- that is a subset of the
Cartesian product of extents E.  At all multi-indices in E but not in
P, the matrix elements are zero.  Users may read from them, but not
write to them.

We could design the unpacked layout `layout_blas_triangular` in the
same way, by having its mapping and accessor return zero values for
index pairs not in the triangle.  The mapping would map index pairs
not in the triangle to some flag value that the accessor would
interpret as "return a read-only zero value."  This would take extra
machinery and overhead in the accessor (a different `reference_type`
with a branch inside), but it's possible.  Just like with
`layout_blas_symmetric`, algorithms could avoid this overhead by
extracting the "base layout" but respecting the triangular
assumptions.

The packed layouts do not have this issue.  All index pairs in the
Cartesian product of the extents map (nonuniquely) to a valid index in
the codomain.  For index pairs outside the triangle, the codomain
index is wrong, but it's still valid.

Here are options for addressing this issue.  We order them by our
decreasing preference.

1. Add a new "incomplete" category of layout mapping policy to the
   categories in P0009.  Forbid users of `layout_blas_triangular` (and
   possibly also `layout_blas_symmetric`) layouts from accessing index
   pairs outside the layout's triangle.  The layouts will not remap
   forbidden indices.

2. Add a new "incomplete" category of layout mapping policy to the
   categories in P0009.  Make `layout_blas_triangular` return
   unwriteable zero (whatever that means) for index pairs outside the
   triangle.  Algorithms may get the "base layout" to avoid index
   remapping and accessor overhead.

3. Change the algorithms to take "triangle" and "diagonal storage"
   information as separate arguments.  Do not redefine *domain*.

We do not prefer (3), because (1) and (2) open the door to future
expansion of matrix storage formats.

---