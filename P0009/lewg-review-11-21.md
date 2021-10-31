# MDSPAN

- in the works since 2014
  - coauthors from US National Labs, US NAVY, NVIDIA, AMD, StellarScience, Swiss Super Computing Centre  
- we believe this is ready for C++23
- reference implementation at [kokkos/mdspan](https://github.com/kokkos/mdspan)
  - using this to replace the predecssor data structure in Kokkos - which is used by hundreds of HPC projects
  - also used for P1673 (Linear Algebra) implementation [kokkos/stdblas](https://github.com/kokkos/stdblas)

## Primary Changes Since Last Review

- added back in design discussion
- incorproated P2299r3
  - introduced `dextents` alias for all dynamic extents
  - Remove `mdspan` alias and renamed `basic_mdspan` to `mdspan`: combination of `dextents` and deduction guides largely replaces the utility of a short cut alias
  - added `mdspan` deduction guides
- use `operator[]` in `mdspan` instead of `operator()`
- cleanup of `nothrow` and `throw` clauses
- improved `submdspan` wording (this is in R14 post the R13 which was in the mailing)

### Design Discussion

- Why do we need multidimensional arrays?
- Why are existing C++ data structures not enough?
- Mixing Compile-Time and Run-Time extents.
- Why custom memory layouts?
- Why custom accessors?
- Subspan Support
- Why propose a multidimensional array view before a container?

### P2299r3 Changes 

- Discussed extensively in last meeting
- pure dynamic extents:
```c++
void foo(double* a_ptr, int N, int M) {
  // Previously
  // return mdspan<double,dynamic_extent,dynamic_extent> a(a_ptr, N,M);
  // Now
  return mdspan(a_ptr, N, M);
}
```
- mixed dynamic/static extents
```c++
auto foo(double* a_ptr, int N, int M) {
  // Previously
  // return mdspan<double,dynamic_extent,dynamic_extent,3>(a_ptr, N,M);
  // Now
  return mdspan(a_ptr, extents<dynamic_extent,dynamic_extent,3>(N, M));
}
```

### operator[]
- [P2186R6](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2128r6.pdf) approved in plenary for C++23
- intention to use that operator was mentioned for a few revisions already
- previously special for rank-1 `operator[]` now used for all ranks (including rank-0)
- note: this means mdspan is not quite drop-in replacement for existing multi dimensional C++ arrays
  - but simple wrapper/subclass could provide both `operator()` and `operator[]` and forward the former to the latter
  - automatic refactoring tools can be used

## Viability of customization points
- One of the primary design concerns of `mdspan` are customization points
- 

