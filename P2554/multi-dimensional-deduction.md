# C-Array Interoperability of MDSpan

## The Issue

Currently one cannot construct an `mdspan` from a multidimensional c-array.

```c++
int a[3][2] = {1,2,3,4,5,6};
mdspan<int,dextents<2>> b(a,3,2); // fail
mdspan<int,extents<3,2>> c(a); // fail
```

It only works for 1D c-arrays, which decay to pointers.
However the deduction guide when using just the 1D c-array with no length argument,
would deduce a rank-0 `mdspan`, which may be surprising to users:

```c++
int a[6] = {1,2,3,4,5,6};
mdspan b(a);
static_assert(decltype(b)::rank()==0);
```

## Proposal

We propose to fix this issue by adding a templated constructor which is constrained to take a c-array, and add deduction guides for 
a single argument, one handling c-arrays and one pointers.

This would enable the following:

```c++
{
  int a[6] = {1,2,3,4,5,6};
  mdspan b(a);
  static_assert(decltype(b)::rank()==1);
  static_assert(decltype(b)::static_extent(0)==6);
}
{
  int a[2][3] = {1,2,3,4,5,6};
  mdspan b(a);
  static_assert(decltype(b)::rank()==2);
  static_assert(decltype(b)::static_extent(0)==2);
  static_assert(decltype(b)::static_extent(1)==3);
}
{
  int a[2][3] = {1,2,3,4,5,6};
  mdspan<int, extents<dynamic_extent,3>> b(a);
  static_assert(decltype(b)::rank()==2);
  // b.extent(0)==2
}
```

It would still be possible to do the following with 1D c-arrays, which can still utilize the decay to pointer pathway:
```c++
{
  int a[6] = {1,2,3,4,5,6};
  mdspan b(a,3);
  static_assert(decltype(b)::rank()==1);
  static_assert(decltype(b)::static_extent(0)==dynamic_extent;
  // b.extent(0)==3
}
{
  int a[6] = {1,2,3,4,5,6};
  mdspan<int,extents<3>> b(a);
  static_assert(decltype(b)::rank()==1);
  static_assert(decltype(b)::static_extent(0)==3;
}
{
  int a[2][3] = {1,2,3,4,5,6};
  mdspan b((int*)a, 5);
  static_assert(decltype(b)::rank()==1);
  // b.extent(0)==5
}
```

### Implementation

A modification of the `mdspan` reference implementation from https://github.com/kokkos/mdspan is available on godbolt: https://godbolt.org/z/GqzsK4rG9 
We plan to have this with a proper configure option in the main repo in the near future.

## Wording

### In SubSection 22.7.X.1 [mdspan.mdspan.overview]

#### *Add after*

```c++
constexpr mdspan(pointer p, const mapping_type& m, const accessor_type& a);
```

*the following constructor:*

```c++
template<class CArray>
constexpr mdspan(CArray);
```

#### *Add the following deduction guides after the class definition:*

```c++
template<class CArray>
mdspan(CArray)
  -> mdspan<see below>;
```

```c++
template<class Pointer>
mdspan(Pointer)
  -> mdspan<see below>;
```

### In SubSection 22.7.X.2 [mdspan.mdspan.cons]

#### *Insert after paragraph 14:*

```c++
template<class CArray>
constexpr mdspan(CArray a);
```

Let _`extract-extents`_ be the exposition only template class defined as:
```c++
template <class T, std::size_t... Exts>
struct extract-extents {
  using value_type   = T;
  using extents_type = std::experimental::extents<Exts...>;
};

template <class T, std::size_t... Exts>
struct @_extract-extents_@<T*, Exts...>
    : @_extract-extents_@<T, std::experimental::dynamic_extent, Exts...> {};

template <class T, std::size_t N, std::size_t... Exts>
struct @_extract-extents_@<T[N], Exts...>
    : @_extract-extents_@<T, Exts..., size_t{N}> {};
}
```
*Constraints:* 

  * `is_array_v<CArray>` is `true`
 
  * `rank_v<CArray>` equals `extents_type::rank()`

  * `typename mapping_type::layout_type` is `layout_right`.

  * `is_constructible_v<extents_type,typename detail::extract-extents<T>::extents_type>` is `true`.

  * `is_constructible_v<pointer,remove_all_extents_t<CArray>*>` is `true`.

  * `is_default_constructible_v<accessor_type>` is `true`.

*Effects:*

  * Direct-non-list-initializes `ptr_` with `a`.

  * Direct-non-list-initializes `map_` with `typename extract-extents<T>::extents_type()`.


#### *Insert after paragraph 19:*

```c++
template<class CArray>
mdspan(CArray)
  -> mdspan<see below>;
```
*Constraints:*

   * `is_array_v<CArray>` is `true`.

*Remarks:*
   
   * The deduced type is: `mdspan< std::remove_all_extents_t<T>, typename detail::extract-extents<T>::extents_type >`

```c++
template<class Pointer>
mdspan(Pointer)
  -> mdspan<see below>;
```

*Constraints:*

   * `is_array_v<Pointer>` is `false`.
   
   * `is_pointer_v<Pointer>` is `true`.

*Remarks:*
   
   * The deduced type is: `mdspan< std::remove_pointer_t<T>, extents<> >`

#### *Modify paragraph 20 to be:*

*Constraints:* `(is_convertible_v<Integrals, size_type> && ...)` is `true` and `sizeof...(Integrals)>0` is `true`.


