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
- orthogonalization of pointer, access and layout allows for maximum flexibility

### Simple Access Modifications

- Example 1: attach `std::assume_aligned` to an array
  - `std::assume_aligned` is used locally in access, not part of pointer type!
  - offset_policy can't propagate `std::assume_aligned` since it might not be aligned
  - https://godbolt.org/z/Gbas5Esvf
  
```c++
template<class T, size_t Align>
struct alligned_accessor {
  using element_type = T;
  using pointer =  T*;
  using reference = T&;
  
  // arbitrary offsetted pointer is not aligned 
  // so just return default_accessor
  using offset_policy = default_accessor<T>;

  reference access(const pointer p, int i) const {
    pointer p_align = std::assume_aligned<Align>(p);
    return p_align[i];
  }

  pointer offset(const pointer p, int i) const {
    return p+i;
  }

  operator std::default_accessor<T>() const { return std::default_accessor<T>{}; }
};

using aligned_mdsp = std::mdspan<double,std::dextents<2>,
                          std::layout_right,aligned_accessor<double,16>>;
                          
void bar(std::mdspan<double,std::dextents<1>> a) {
  for(int i=0; i<a.extent(0); i++) a[i] = 1.*i;
}

void foo(aligned_mdsp a, aligned_mdsp b) {
   // aligned access
   for(int i=0; i<a.extent(0)-1; i++)
     for(int j=0; j<a.extent(1); j++)
       a[i,j] += b[i,j];
   // last column may not be aligned
   // through offset_policy this will just get default_accessor
   auto a_last = std::submdspan(a,a.extent(0)-1,std::full_extent);
   bar(a_last);
}
```

### Make Typesafe GPU Accesses

- Want to encode MemorySpace in an mdspan, i.e. from which execution context is it accessible
  - enables us to catch when accessing GPU only memory on host
  - enables overload for host vs GPU dispatch
  - use `default_accessor` as type erasure
  - https://godbolt.org/z/zWhKKeMef
  
```c++
struct GPUSpace {};
struct HostSpace {};

template<class T, class MemSpace>
struct memspace_accessor {
  using element_type = T;
  using pointer =  T*;
  using reference = T&;
  using offset_policy = memspace_accessor;

  reference access(const pointer p, int i) const {
    #if defined(__CUDA_ARCH__) || defined(__HIP_DEVICE_COMPILE__)
    static_assert(std::is_same_v<MemSpace,GPUSpace>,"Accessing Host Memory from GPU");
    #else
    static_assert(std::is_same_v<MemSpace,HostSpace>,"Accessing GPU Memory from Host");    
    #endif
    return p[i];
  }

  pointer offset(const pointer p, int i) const {
    return p+i;
  }

  // Converting from and to default_accessor
  memspace_accessor(std::default_accessor<T>) {}
  operator std::default_accessor<T>() const { return std::default_accessor<T>{}; }
};

template<class MDS>
void vector_add(MDS a, std::enable_if_t<std::is_same_v<typename MDS::accessor_type, 
                memspace_accessor<MDS::element_type,GPUSpace>,MDS> b) {
   // dispatch to GPU
   // ...
   // this would fail:
   // for(int i=0; i<a.extent(0); i++)
   //   a(i) += b(i);
}

template<class MDS>
void vector_add(MDS a, std::enable_if_t<!std::is_same_v<typename MDS::accessor_type, 
                memspace_accessor<MDS::element_type,GPUSpace>,MDS> b) {
   // run on host
   for(int i=0; i<a.extent(0); i++)
     a(i) += b(i);
}
```


