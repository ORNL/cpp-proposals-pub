//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2014) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Kokkos is licensed under 3-clause BSD terms of use:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
//
// ************************************************************************
//@HEADER

#ifndef STD_EXPERIMENTAL_FUNDAMENTALS_V3_MDSPAN_HEADER
#define STD_EXPERIMENTAL_FUNDAMENTALS_V3_MDSPAN_HEADER

#line 1 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#1
//@HEADER
#line 2 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#2
// ************************************************************************
#line 3 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#3
//
#line 4 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#4
//                        Kokkos v. 2.0
#line 5 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#5
//              Copyright (2014) Sandia Corporation
#line 6 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#6
//
#line 7 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#7
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#line 8 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#8
// the U.S. Government retains certain rights in this software.
#line 9 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#9
//
#line 10 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#10
// Kokkos is licensed under 3-clause BSD terms of use:
#line 11 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#11
//
#line 12 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#12
// Redistribution and use in source and binary forms, with or without
#line 13 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#13
// modification, are permitted provided that the following conditions are
#line 14 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#14
// met:
#line 15 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#15
//
#line 16 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#16
// 1. Redistributions of source code must retain the above copyright
#line 17 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#17
// notice, this list of conditions and the following disclaimer.
#line 18 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#18
//
#line 19 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#19
// 2. Redistributions in binary form must reproduce the above copyright
#line 20 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#20
// notice, this list of conditions and the following disclaimer in the
#line 21 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#21
// documentation and/or other materials provided with the distribution.
#line 22 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#22
//
#line 23 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#23
// 3. Neither the name of the Corporation nor the names of the
#line 24 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#24
// contributors may be used to endorse or promote products derived from
#line 25 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#25
// this software without specific prior written permission.
#line 26 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#26
//
#line 27 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#27
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
#line 28 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#28
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#line 29 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#29
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#line 30 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#30
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
#line 31 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#31
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#line 32 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#32
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#line 33 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#33
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#line 34 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#34
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#line 35 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#35
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#line 36 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#36
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#line 37 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#37
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#line 38 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#38
//
#line 39 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#39
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
#line 40 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#40
//
#line 41 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#41
// ************************************************************************
#line 42 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#42
//@HEADER
#line 43 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#43

#line 44 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#44
#include <cstddef> // std::ptrdiff_t
#line 45 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#45
#include <array> // std::array
#line 46 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#46

#line 47 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#47
namespace std {
#line 48 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#48
namespace experimental {
#line 49 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#49
inline namespace fundamentals_v3 {
#line 50 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#50

#line 51 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#51
enum : std::ptrdiff_t { dynamic_extent = -1 };
#line 52 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#52

#line 53 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#53

#line 54 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#54
// [mdspan.extents]
#line 55 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#55
template< std::ptrdiff_t ... StaticExtents >
#line 56 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#56
class extents;
#line 57 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#57

#line 58 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#58
// [mdspan.extents.compare]
#line 59 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#59
template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
#line 60 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#60
constexpr bool operator==(const extents<LHS...>& lhs,
#line 61 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#61
                          const extents<RHS...>& rhs) noexcept;
#line 62 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#62

#line 63 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#63
template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
#line 64 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#64
constexpr bool operator!=(const extents<LHS...>& lhs,
#line 65 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#65
                          const extents<RHS...>& rhs) noexcept;
#line 66 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#66

#line 67 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#67

#line 68 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#68
}}}
#line 69 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#69

#line 70 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#70
//--------------------------------------------------------------------------
#line 71 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#71
//--------------------------------------------------------------------------
#line 72 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#72

#line 73 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#73
namespace std {
#line 74 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#74
namespace experimental {
#line 75 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#75
inline namespace fundamentals_v3 {
#line 76 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#76
namespace detail {
#line 77 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#77
  template< int R, std::ptrdiff_t ... StaticExtents >
#line 78 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#78
  struct extents_analyse;
#line 79 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#79

#line 80 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#80
  template< int R, std::ptrdiff_t E0, std::ptrdiff_t ... StaticExtents >
#line 81 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#81
  struct extents_analyse<R,E0,StaticExtents...> {
#line 82 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#82

#line 83 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#83
    typedef extents_analyse<R-1,StaticExtents...> next_extents_analyse;
#line 84 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#84

#line 85 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#85
    static constexpr std::size_t rank() noexcept { return next_extents_analyse::rank()+1; }
#line 86 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#86
    static constexpr std::size_t rank_dynamic() noexcept { return next_extents_analyse::rank_dynamic(); }
#line 87 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#87

#line 88 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#88
    next_extents_analyse next;
#line 89 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#89

#line 90 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#90
    extents_analyse():next() {};
#line 91 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#91
 
#line 92 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#92
    template<class...DynamicExtents>
#line 93 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#93
    extents_analyse(DynamicExtents...de):next(de...) {}
#line 94 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#94

#line 95 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#95
    template<std::size_t Rank>
#line 96 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#96
    extents_analyse(const array<std::ptrdiff_t,Rank>& de,const std::size_t r):next(de,r) {}
#line 97 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#97

#line 98 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#98
    template<std::ptrdiff_t...OtherStaticExtents>
#line 99 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#99
    extents_analyse(extents_analyse<R,OtherStaticExtents...> rhs):next(rhs.next) {}    
#line 100 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#100

#line 101 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#101
    template<std::ptrdiff_t...OtherStaticExtents>
#line 102 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#102
    extents_analyse operator= (extents_analyse<R,OtherStaticExtents...> rhs) {
#line 103 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#103
      next = rhs.next;
#line 104 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#104
      return *this;
#line 105 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#105
    }
#line 106 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#106
    
#line 107 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#107
    constexpr std::ptrdiff_t extent(const std::size_t r) const noexcept {
#line 108 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#108
      if(r==R) return E0;
#line 109 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#109
      return next.extent(r); 
#line 110 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#110
    }
#line 111 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#111
    static constexpr std::ptrdiff_t static_extent(const std::size_t r) noexcept {
#line 112 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#112
      if(r==R) return E0;
#line 113 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#113
      return next_extents_analyse::static_extent(r);
#line 114 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#114
    }
#line 115 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#115
  };
#line 116 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#116

#line 117 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#117
  template< int R, std::ptrdiff_t ... StaticExtents >
#line 118 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#118
  struct extents_analyse<R,dynamic_extent,StaticExtents...> {
#line 119 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#119
    typedef extents_analyse<R-1,StaticExtents...> next_extents_analyse;
#line 120 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#120

#line 121 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#121
    static constexpr std::size_t rank() noexcept { return next_extents_analyse::rank()+1; }
#line 122 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#122
    static constexpr std::size_t rank_dynamic() noexcept { return next_extents_analyse::rank_dynamic()+1; }
#line 123 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#123

#line 124 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#124
    next_extents_analyse next;
#line 125 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#125
    std::ptrdiff_t this_extent;
#line 126 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#126

#line 127 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#127
    extents_analyse():next(),this_extent(0) {}
#line 128 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#128

#line 129 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#129
    template<class...DynamicExtents>
#line 130 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#130
    extents_analyse(std::ptrdiff_t E, DynamicExtents...de):next(de...),this_extent(E) {}
#line 131 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#131

#line 132 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#132
    template<std::size_t Rank>
#line 133 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#133
    extents_analyse(const array<std::ptrdiff_t,Rank>& de, const std::size_t r):next(de,r+1),this_extent(de[r]) {}
#line 134 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#134

#line 135 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#135
    template<std::ptrdiff_t...OtherStaticExtents>
#line 136 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#136
    extents_analyse(extents_analyse<R,OtherStaticExtents...> rhs):next(rhs.next),this_extent(rhs.extent(R)) {}    
#line 137 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#137

#line 138 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#138
    template<std::ptrdiff_t...OtherStaticExtents>
#line 139 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#139
    extents_analyse & operator= (extents_analyse<R,OtherStaticExtents...> rhs) {
#line 140 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#140
      next = rhs.next;
#line 141 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#141
      this_extent = rhs.extent(R);
#line 142 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#142
      return *this;
#line 143 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#143
    }    
#line 144 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#144

#line 145 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#145
    constexpr std::ptrdiff_t extent(const std::size_t r) const noexcept {
#line 146 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#146
      if(r==R) return this_extent; 
#line 147 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#147
      else return next.extent(r);
#line 148 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#148
    }
#line 149 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#149
    static constexpr std::ptrdiff_t static_extent(const std::size_t r) noexcept {
#line 150 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#150
      if(r==R) return dynamic_extent;
#line 151 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#151
      return next_extents_analyse::static_extent(r);
#line 152 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#152
    }
#line 153 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#153
  };
#line 154 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#154

#line 155 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#155
  template<>
#line 156 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#156
  struct extents_analyse<0> {
#line 157 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#157
    static constexpr std::size_t rank() noexcept { return 0; }
#line 158 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#158
    static constexpr std::size_t rank_dynamic() noexcept { return 0; }
#line 159 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#159

#line 160 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#160
    extents_analyse() {}
#line 161 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#161

#line 162 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#162
    template<std::size_t Rank>
#line 163 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#163
    extents_analyse(const array<std::ptrdiff_t,Rank>&, const std::size_t) {}
#line 164 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#164

#line 165 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#165
    //extents_analyse & operator=(extents_analyse) = default;
#line 166 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#166

#line 167 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#167
    constexpr std::ptrdiff_t extent(const std::size_t) const noexcept {
#line 168 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#168
      return 1;
#line 169 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#169
    }
#line 170 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#170
    static constexpr std::ptrdiff_t static_extent(const std::size_t) noexcept {
#line 171 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#171
      return 1;
#line 172 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#172
    }
#line 173 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#173

#line 174 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#174
  };
#line 175 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#175
}
#line 176 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#176

#line 177 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#177
template< std::ptrdiff_t ... StaticExtents >
#line 178 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#178
class extents
#line 179 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#179
{
#line 180 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#180
private:
#line 181 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#181

#line 182 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#182
  template< std::ptrdiff_t... > friend class extents ;
#line 183 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#183

#line 184 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#184
  typedef detail::extents_analyse<sizeof...(StaticExtents),StaticExtents...> extents_analyse_t;
#line 185 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#185
  extents_analyse_t impl;
#line 186 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#186
public:
#line 187 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#187

#line 188 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#188
  using index_type = std::ptrdiff_t ;
#line 189 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#189

#line 190 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#190
  constexpr extents() noexcept {}
#line 191 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#191

#line 192 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#192
  constexpr extents( extents && ) noexcept = default ;
#line 193 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#193

#line 194 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#194
  constexpr extents( const extents & ) noexcept = default ;
#line 195 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#195

#line 196 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#196
  template< class ... IndexType >
#line 197 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#197
  constexpr extents( std::ptrdiff_t dn,
#line 198 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#198
                              IndexType ... DynamicExtents ) noexcept
#line 199 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#199
    : impl( dn , DynamicExtents... ) 
#line 200 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#200
    { static_assert( 1+sizeof...(DynamicExtents) == rank_dynamic() , "" ); }
#line 201 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#201

#line 202 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#202
  constexpr extents( const array<std::ptrdiff_t,extents_analyse_t::rank_dynamic()> dynamic_extents) noexcept
#line 203 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#203
    : impl(dynamic_extents,0) {}
#line 204 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#204

#line 205 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#205
  template<std::ptrdiff_t... OtherStaticExtents>
#line 206 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#206
  extents( const extents<OtherStaticExtents...>& other )
#line 207 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#207
    : impl( other.impl ) {}
#line 208 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#208

#line 209 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#209
  extents & operator = ( extents && ) noexcept = default;
#line 210 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#210

#line 211 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#211
  extents & operator = ( const extents & ) noexcept = default;
#line 212 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#212

#line 213 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#213
  template<std::ptrdiff_t... OtherStaticExtents>
#line 214 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#214
  extents & operator = ( const extents<OtherStaticExtents...>& other )
#line 215 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#215
    { impl = other.impl; return *this ; }
#line 216 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#216

#line 217 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#217
  ~extents() = default ;
#line 218 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#218

#line 219 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#219
  // [mdspan.extents.obs]
#line 220 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#220

#line 221 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#221
  static constexpr std::size_t rank() noexcept
#line 222 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#222
    { return sizeof...(StaticExtents); }
#line 223 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#223

#line 224 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#224
  static constexpr std::size_t rank_dynamic() noexcept 
#line 225 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#225
    { return extents_analyse_t::rank_dynamic() ; }
#line 226 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#226

#line 227 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#227
  static constexpr index_type static_extent(std::size_t k) noexcept
#line 228 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#228
    { return extents_analyse_t::static_extent(rank()-k); }
#line 229 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#229

#line 230 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#230
  constexpr index_type extent(std::size_t k) const noexcept
#line 231 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#231
    { return impl.extent(rank()-k); }
#line 232 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#232

#line 233 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#233
};
#line 234 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#234

#line 235 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#235
template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
#line 236 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#236
constexpr bool operator==(const extents<LHS...>& lhs,
#line 237 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#237
                          const extents<RHS...>& rhs) noexcept { 
#line 238 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#238
  bool equal = lhs.rank() == rhs.rank();
#line 239 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#239
  for(std::size_t r = 0; r<lhs.rank(); r++)
#line 240 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#240
    equal = equal && ( lhs.extent(r) == rhs.extent(r) ); 
#line 241 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#241
  return equal; 
#line 242 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#242
}
#line 243 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#243

#line 244 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#244
template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
#line 245 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#245
constexpr bool operator!=(const extents<LHS...>& lhs,
#line 246 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#246
                          const extents<RHS...>& rhs) noexcept { 
#line 247 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#247
  return !(lhs==rhs);
#line 248 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#248
}
#line 249 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#249

#line 250 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#250
}}} // std::experimental::fundamentals_v3
#line 251 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#251

#line 252 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#252
//--------------------------------------------------------------------------
#line 253 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/extents.hpp#253
//--------------------------------------------------------------------------
#line 1 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#1
//@HEADER
#line 2 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#2
// ************************************************************************
#line 3 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#3
//
#line 4 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#4
//                        Kokkos v. 2.0
#line 5 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#5
//              Copyright (2014) Sandia Corporation
#line 6 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#6
//
#line 7 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#7
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#line 8 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#8
// the U.S. Government retains certain rights in this software.
#line 9 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#9
//
#line 10 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#10
// Kokkos is licensed under 3-clause BSD terms of use:
#line 11 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#11
//
#line 12 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#12
// Redistribution and use in source and binary forms, with or without
#line 13 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#13
// modification, are permitted provided that the following conditions are
#line 14 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#14
// met:
#line 15 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#15
//
#line 16 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#16
// 1. Redistributions of source code must retain the above copyright
#line 17 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#17
// notice, this list of conditions and the following disclaimer.
#line 18 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#18
//
#line 19 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#19
// 2. Redistributions in binary form must reproduce the above copyright
#line 20 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#20
// notice, this list of conditions and the following disclaimer in the
#line 21 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#21
// documentation and/or other materials provided with the distribution.
#line 22 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#22
//
#line 23 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#23
// 3. Neither the name of the Corporation nor the names of the
#line 24 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#24
// contributors may be used to endorse or promote products derived from
#line 25 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#25
// this software without specific prior written permission.
#line 26 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#26
//
#line 27 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#27
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
#line 28 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#28
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#line 29 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#29
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#line 30 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#30
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
#line 31 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#31
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#line 32 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#32
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#line 33 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#33
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#line 34 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#34
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#line 35 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#35
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#line 36 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#36
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#line 37 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#37
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#line 38 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#38
//
#line 39 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#39
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
#line 40 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#40
//
#line 41 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#41
// ************************************************************************
#line 42 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#42
//@HEADER
#line 43 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#43

#line 44 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#44
//--------------------------------------------------------------------------
#line 45 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#45
//--------------------------------------------------------------------------
#line 46 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#46

#line 47 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#47
namespace std {
#line 48 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#48
namespace experimental {
#line 49 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#49
inline namespace fundamentals_v3 {
#line 50 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#50

#line 51 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#51
// [mdspan.layout]
#line 52 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#52
class layout_right ;
#line 53 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#53
class layout_left ;
#line 54 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#54
class layout_stride ;
#line 55 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#55

#line 56 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#56
}}}
#line 57 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#57

#line 58 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#58
//--------------------------------------------------------------------------
#line 59 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#59
//--------------------------------------------------------------------------
#line 60 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#60

#line 61 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#61

#line 62 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#62
namespace std {
#line 63 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#63
namespace experimental {
#line 64 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#64
inline namespace fundamentals_v3 {
#line 65 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#65

#line 66 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#66
class layout_right {
#line 67 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#67

#line 68 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#68
public:
#line 69 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#69
  template<class Extents>
#line 70 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#70
  class mapping {
#line 71 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#71
  private:
#line 72 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#72

#line 73 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#73
    Extents m_extents ;
#line 74 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#74

#line 75 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#75
  public:
#line 76 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#76

#line 77 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#77
    using index_type = ptrdiff_t ;
#line 78 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#78
    using extents_type = Extents ;
#line 79 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#79

#line 80 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#80
    constexpr mapping() noexcept = default ;
#line 81 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#81

#line 82 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#82
    constexpr mapping( mapping && ) noexcept = default ;
#line 83 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#83

#line 84 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#84
    constexpr mapping( const mapping & ) noexcept = default ;
#line 85 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#85

#line 86 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#86
    mapping & operator = ( mapping && ) noexcept = default ;
#line 87 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#87

#line 88 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#88
    mapping & operator = ( const mapping & ) noexcept = default ;
#line 89 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#89

#line 90 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#90
    constexpr mapping( const Extents & ext ) noexcept
#line 91 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#91
      : m_extents( ext ) {}
#line 92 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#92

#line 93 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#93
    constexpr const Extents & extents() const noexcept { return m_extents ; }
#line 94 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#94

#line 95 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#95
  private:
#line 96 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#96

#line 97 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#97
    // ( ( ( ( i0 ) * N1 + i1 ) * N2 + i2 ) * N3 + i3 ) ...
#line 98 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#98

#line 99 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#99
    static constexpr index_type
#line 100 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#100
    offset( const size_t , const ptrdiff_t sum)
#line 101 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#101
      { return sum; }
#line 102 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#102

#line 103 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#103
    template<class ... Indices >
#line 104 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#104
    inline constexpr index_type
#line 105 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#105
    offset( const size_t r, ptrdiff_t sum, const index_type i, Indices... indices) const noexcept
#line 106 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#106
      {
#line 107 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#107
        return offset( r+1 , sum * m_extents.extent(r) + i, indices...);
#line 108 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#108
      }
#line 109 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#109

#line 110 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#110
  public:
#line 111 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#111

#line 112 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#112
    constexpr index_type required_span_size() const noexcept { 
#line 113 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#113
      index_type size = 1;
#line 114 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#114
      for(size_t r=0; r<m_extents.rank(); r++)
#line 115 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#115
        size *= m_extents.extent(r);
#line 116 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#116
      return size;
#line 117 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#117
    } 
#line 118 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#118

#line 119 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#119
    template<class ... Indices >
#line 120 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#120
    constexpr
#line 121 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#121
    typename enable_if<sizeof...(Indices) == Extents::rank(),index_type>::type
#line 122 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#122
    operator()( Indices ... indices ) const noexcept 
#line 123 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#123
      { return offset( 0, 0, indices... ); }
#line 124 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#124

#line 125 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#125
    static constexpr bool is_always_unique()     noexcept { return true ; }
#line 126 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#126
    static constexpr bool is_always_contiguous() noexcept { return true ; }
#line 127 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#127
    static constexpr bool is_always_strided()    noexcept { return true ; }
#line 128 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#128

#line 129 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#129
    constexpr bool is_unique()     const noexcept { return true ; }
#line 130 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#130
    constexpr bool is_contiguous() const noexcept { return true ; }
#line 131 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#131
    constexpr bool is_strided()    const noexcept { return true ; }
#line 132 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#132

#line 133 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#133
    constexpr index_type stride(const size_t R) const noexcept { 
#line 134 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#134
      ptrdiff_t stride_ = 1;
#line 135 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#135
      for(size_t r = m_extents.rank()-1; r>R; r--)
#line 136 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#136
        stride_ *= m_extents.extent(r);
#line 137 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#137
      return stride_;
#line 138 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#138
    }
#line 139 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#139

#line 140 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#140
  }; // class mapping
#line 141 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#141

#line 142 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#142
}; // class layout_right
#line 143 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#143

#line 144 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#144
}}} // experimental::fundamentals_v3
#line 145 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#145

#line 146 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#146
//----------------------------------------------------------------------------
#line 147 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#147

#line 148 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#148
namespace std {
#line 149 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#149
namespace experimental {
#line 150 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#150
inline namespace fundamentals_v3 {
#line 151 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#151

#line 152 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#152
class layout_left {
#line 153 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#153
public:
#line 154 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#154
  template<class Extents>
#line 155 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#155
  class mapping {
#line 156 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#156
  private:
#line 157 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#157

#line 158 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#158
    Extents m_extents ;
#line 159 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#159

#line 160 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#160
  public:
#line 161 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#161

#line 162 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#162
    using index_type = ptrdiff_t ;
#line 163 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#163
    using extents_type = Extents ;
#line 164 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#164

#line 165 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#165
    constexpr mapping() noexcept = default ;
#line 166 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#166

#line 167 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#167
    constexpr mapping( mapping && ) noexcept = default ;
#line 168 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#168

#line 169 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#169
    constexpr mapping( const mapping & ) noexcept = default ;
#line 170 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#170

#line 171 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#171
    mapping & operator = ( mapping && ) noexcept = default ;
#line 172 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#172

#line 173 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#173
    mapping & operator = ( const mapping & ) noexcept = default ;
#line 174 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#174

#line 175 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#175
    constexpr mapping( const Extents & ext ) noexcept
#line 176 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#176
      : m_extents( ext ) {}
#line 177 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#177

#line 178 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#178
    constexpr const Extents & extents() const noexcept { return m_extents ; }
#line 179 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#179

#line 180 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#180
  private:
#line 181 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#181

#line 182 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#182
    // ( i0 + N0 * ( i1 + N1 * ( i2 + N2 * ( ... ) ) ) )
#line 183 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#183

#line 184 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#184
    static constexpr index_type
#line 185 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#185
    offset( size_t ) noexcept
#line 186 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#186
      { return 0 ; }
#line 187 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#187

#line 188 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#188
    template<class ... IndexType >
#line 189 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#189
    constexpr index_type
#line 190 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#190
    offset( const size_t r, index_type i, IndexType... indices ) const noexcept
#line 191 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#191
      { return i + m_extents.extent(r) * offset( r+1, indices... ); }
#line 192 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#192

#line 193 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#193
  public:
#line 194 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#194

#line 195 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#195
    constexpr index_type required_span_size() const noexcept {
#line 196 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#196
      ptrdiff_t size = 1;
#line 197 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#197
      for(size_t r = 0; r<m_extents.rank(); r++)
#line 198 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#198
        size *= m_extents.extent(r);
#line 199 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#199
      return size;
#line 200 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#200
    }
#line 201 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#201

#line 202 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#202
    template<class ... Indices >
#line 203 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#203
    constexpr
#line 204 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#204
    typename enable_if<sizeof...(Indices) == Extents::rank(),index_type>::type
#line 205 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#205
    operator()( Indices ... indices ) const noexcept
#line 206 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#206
      { return offset( 0, indices... ); }
#line 207 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#207

#line 208 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#208
    static constexpr bool is_always_unique()     noexcept { return true ; }
#line 209 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#209
    static constexpr bool is_always_contiguous() noexcept { return true ; }
#line 210 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#210
    static constexpr bool is_always_strided()    noexcept { return true ; }
#line 211 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#211

#line 212 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#212
    constexpr bool is_unique()     const noexcept { return true ; }
#line 213 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#213
    constexpr bool is_contiguous() const noexcept { return true ; }
#line 214 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#214
    constexpr bool is_strided()    const noexcept { return true ; }
#line 215 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#215

#line 216 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#216
    constexpr index_type stride(const size_t R) const noexcept {
#line 217 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#217
      ptrdiff_t stride_ = 1;
#line 218 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#218
      for(size_t r = 0; r<R; r++)
#line 219 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#219
        stride_ *= m_extents.extent(r);
#line 220 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#220
      return stride_;
#line 221 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#221
    }
#line 222 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#222

#line 223 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#223
  }; // class mapping
#line 224 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#224

#line 225 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#225
}; // class layout_left
#line 226 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#226

#line 227 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#227
}}} // experimental::fundamentals_v3
#line 228 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#228

#line 229 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#229
//----------------------------------------------------------------------------
#line 230 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#230
//----------------------------------------------------------------------------
#line 231 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#231

#line 232 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#232
namespace std {
#line 233 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#233
namespace experimental {
#line 234 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#234
inline namespace fundamentals_v3 {
#line 235 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#235

#line 236 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#236
class layout_stride {
#line 237 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#237
public:
#line 238 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#238

#line 239 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#239
  template<class Extents>
#line 240 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#240
  class mapping {
#line 241 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#241
  private:
#line 242 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#242

#line 243 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#243
    using stride_t = array<ptrdiff_t,Extents::rank()> ;
#line 244 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#244

#line 245 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#245
    Extents   m_extents ;
#line 246 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#246
    stride_t  m_stride ;
#line 247 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#247
    int       m_contig ;
#line 248 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#248

#line 249 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#249
  public:
#line 250 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#250

#line 251 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#251
    using index_type = ptrdiff_t ;
#line 252 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#252
    using extents_type = Extents ;
#line 253 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#253

#line 254 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#254
    constexpr mapping() noexcept = default ;
#line 255 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#255

#line 256 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#256
    constexpr mapping( mapping && ) noexcept = default ;
#line 257 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#257

#line 258 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#258
    constexpr mapping( const mapping & ) noexcept = default ;
#line 259 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#259

#line 260 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#260
    mapping & operator = ( mapping && ) noexcept = default ;
#line 261 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#261

#line 262 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#262
    mapping & operator = ( const mapping & ) noexcept = default ;
#line 263 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#263

#line 264 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#264
    mapping( const Extents & ext, const stride_t & str ) noexcept
#line 265 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#265
      : m_extents(ext), m_stride(str), m_contig(1)
#line 266 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#266
      {
#line 267 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#267
        int p[ Extents::rank() ? Extents::rank() : 1 ];
#line 268 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#268

#line 269 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#269
        // Fill permutation such that
#line 270 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#270
        //   m_stride[ p[i] ] <= m_stride[ p[i+1] ]
#line 271 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#271
        //
#line 272 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#272
        for ( size_t i = 0 ; i < Extents::rank() ; ++i ) {
#line 273 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#273

#line 274 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#274
          int j = i ;
#line 275 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#275

#line 276 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#276
          while ( j && m_stride[i] < m_stride[ p[j-1] ] )
#line 277 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#277
           { p[j] = p[j-1] ; --j ; }
#line 278 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#278

#line 279 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#279
          p[j] = i ;
#line 280 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#280
        }
#line 281 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#281

#line 282 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#282
        for ( size_t i = 1 ; i < Extents::rank() ; ++i ) {
#line 283 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#283
          const int j = p[i-1];
#line 284 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#284
          const int k = p[i];
#line 285 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#285
          const index_type prev = m_stride[j] * m_extents.extent(j);
#line 286 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#286
          if ( m_stride[k] != prev ) { m_contig = 0 ; }
#line 287 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#287
        }
#line 288 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#288
      }
#line 289 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#289

#line 290 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#290
    constexpr const Extents & extents() const noexcept { return m_extents ; }
#line 291 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#291

#line 292 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#292
  private:
#line 293 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#293

#line 294 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#294
    // i0 * N0 + i1 * N1 + i2 * N2 + ...
#line 295 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#295

#line 296 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#296
    constexpr index_type
#line 297 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#297
    offset(size_t) const noexcept
#line 298 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#298
      { return 0 ; }
#line 299 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#299

#line 300 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#300
    template<class... IndexType >
#line 301 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#301
    constexpr index_type
#line 302 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#302
    offset( const size_t K, const index_type i, IndexType... indices ) const noexcept
#line 303 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#303
      { return i * m_stride[K] + offset(K+1,indices...); }
#line 304 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#304

#line 305 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#305
  public:
#line 306 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#306

#line 307 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#307
    index_type required_span_size() const noexcept
#line 308 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#308
      {
#line 309 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#309
        index_type max = 0 ;
#line 310 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#310
        for ( size_t i = 0 ; i < Extents::rank() ; ++i )
#line 311 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#311
          max += m_stride[i] * ( m_extents.extent(i) - 1 );
#line 312 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#312
        return max ;
#line 313 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#313
      }
#line 314 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#314

#line 315 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#315
    template<class ... Indices >
#line 316 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#316
    constexpr
#line 317 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#317
    typename enable_if<sizeof...(Indices) == Extents::rank(),index_type>::type
#line 318 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#318
    operator()( Indices ... indices ) const noexcept
#line 319 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#319
      { return offset(0, indices... ); }
#line 320 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#320

#line 321 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#321

#line 322 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#322
    static constexpr bool is_always_unique()     noexcept { return true ; }
#line 323 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#323
    static constexpr bool is_always_contiguous() noexcept { return false ; }
#line 324 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#324
    static constexpr bool is_always_strided()    noexcept { return true ; }
#line 325 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#325

#line 326 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#326
    constexpr bool is_unique()     const noexcept { return true ; }
#line 327 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#327
    constexpr bool is_contiguous() const noexcept { return m_contig ; }
#line 328 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#328
    constexpr bool is_strided()    const noexcept { return true ; }
#line 329 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#329

#line 330 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#330
    constexpr index_type stride(size_t r) const noexcept
#line 331 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#331
      { return m_stride[r]; }
#line 332 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#332

#line 333 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#333
  }; // class mapping
#line 334 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#334

#line 335 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#335
}; // class layout_stride
#line 336 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#336

#line 337 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/layouts.hpp#337
}}} // experimental::fundamentals_v3
#line 1 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#1
//@HEADER
#line 2 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#2
// ************************************************************************
#line 3 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#3
//
#line 4 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#4
//                        Kokkos v. 2.0
#line 5 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#5
//              Copyright (2014) Sandia Corporation
#line 6 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#6
//
#line 7 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#7
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#line 8 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#8
// the U.S. Government retains certain rights in this software.
#line 9 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#9
//
#line 10 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#10
// Kokkos is licensed under 3-clause BSD terms of use:
#line 11 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#11
//
#line 12 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#12
// Redistribution and use in source and binary forms, with or without
#line 13 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#13
// modification, are permitted provided that the following conditions are
#line 14 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#14
// met:
#line 15 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#15
//
#line 16 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#16
// 1. Redistributions of source code must retain the above copyright
#line 17 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#17
// notice, this list of conditions and the following disclaimer.
#line 18 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#18
//
#line 19 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#19
// 2. Redistributions in binary form must reproduce the above copyright
#line 20 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#20
// notice, this list of conditions and the following disclaimer in the
#line 21 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#21
// documentation and/or other materials provided with the distribution.
#line 22 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#22
//
#line 23 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#23
// 3. Neither the name of the Corporation nor the names of the
#line 24 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#24
// contributors may be used to endorse or promote products derived from
#line 25 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#25
// this software without specific prior written permission.
#line 26 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#26
//
#line 27 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#27
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
#line 28 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#28
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#line 29 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#29
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#line 30 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#30
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
#line 31 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#31
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#line 32 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#32
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#line 33 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#33
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#line 34 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#34
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#line 35 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#35
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#line 36 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#36
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#line 37 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#37
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#line 38 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#38
//
#line 39 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#39
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
#line 40 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#40
//
#line 41 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#41
// ************************************************************************
#line 42 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#42
//@HEADER
#line 43 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#43

#line 44 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#44
//--------------------------------------------------------------------------
#line 45 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#45
//--------------------------------------------------------------------------
#line 46 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#46

#line 47 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#47
namespace std {
#line 48 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#48
namespace experimental {
#line 49 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#49
inline namespace fundamentals_v3 {
#line 50 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#50

#line 51 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#51
// [mdspan.accessor.basic]
#line 52 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#52
template<class ElementType>
#line 53 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#53
class accessor_basic;
#line 54 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#54

#line 55 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#55

#line 56 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#56
template<class ElementType>
#line 57 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#57
class accessor_basic {
#line 58 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#58
public:
#line 59 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#59
  using element_type  = ElementType;
#line 60 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#60
  using pointer       = ElementType*;
#line 61 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#61
  using offset_policy = accessor_basic;
#line 62 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#62
  using reference     = ElementType&;
#line 63 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#63

#line 64 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#64
  constexpr typename offset_policy::pointer
#line 65 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#65
    offset( pointer p , ptrdiff_t i ) const noexcept
#line 66 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#66
      { return typename offset_policy::pointer(p+i); }
#line 67 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#67

#line 68 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#68
  constexpr reference access( pointer p , ptrdiff_t i ) const noexcept
#line 69 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#69
    { return p[i]; }
#line 70 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#70

#line 71 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#71
  constexpr ElementType* decay( pointer p ) const noexcept
#line 72 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#72
    { return p; }
#line 73 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#73
};
#line 74 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#74

#line 75 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/accessor_policy.hpp#75
}}} // std::experimental::fundamentals_v3
#line 1 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#1
//@HEADER
#line 2 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#2
// ************************************************************************
#line 3 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#3
//
#line 4 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#4
//                        Kokkos v. 2.0
#line 5 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#5
//              Copyright (2014) Sandia Corporation
#line 6 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#6
//
#line 7 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#7
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#line 8 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#8
// the U.S. Government retains certain rights in this software.
#line 9 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#9
//
#line 10 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#10
// Kokkos is licensed under 3-clause BSD terms of use:
#line 11 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#11
//
#line 12 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#12
// Redistribution and use in source and binary forms, with or without
#line 13 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#13
// modification, are permitted provided that the following conditions are
#line 14 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#14
// met:
#line 15 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#15
//
#line 16 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#16
// 1. Redistributions of source code must retain the above copyright
#line 17 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#17
// notice, this list of conditions and the following disclaimer.
#line 18 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#18
//
#line 19 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#19
// 2. Redistributions in binary form must reproduce the above copyright
#line 20 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#20
// notice, this list of conditions and the following disclaimer in the
#line 21 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#21
// documentation and/or other materials provided with the distribution.
#line 22 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#22
//
#line 23 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#23
// 3. Neither the name of the Corporation nor the names of the
#line 24 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#24
// contributors may be used to endorse or promote products derived from
#line 25 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#25
// this software without specific prior written permission.
#line 26 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#26
//
#line 27 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#27
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
#line 28 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#28
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#line 29 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#29
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#line 30 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#30
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
#line 31 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#31
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#line 32 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#32
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#line 33 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#33
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#line 34 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#34
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#line 35 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#35
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#line 36 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#36
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#line 37 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#37
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#line 38 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#38
//
#line 39 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#39
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
#line 40 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#40
//
#line 41 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#41
// ************************************************************************
#line 42 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#42
//@HEADER
#line 43 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#43

#line 44 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#44

#line 45 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#45

#line 46 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#46
//--------------------------------------------------------------------------
#line 47 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#47
//--------------------------------------------------------------------------
#line 48 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#48

#line 49 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#49
namespace std {
#line 50 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#50
namespace experimental {
#line 51 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#51
inline namespace fundamentals_v3 {
#line 52 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#52

#line 53 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#53

#line 54 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#54

#line 55 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#55
// [mdspan.basic]
#line 56 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#56
template<class ElementType,
#line 57 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#57
         class Extents,
#line 58 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#58
         class LayoutPolicy = layout_right,
#line 59 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#59
         class AccessorPolicy = accessor_basic<ElementType> >
#line 60 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#60
class basic_mdspan ;
#line 61 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#61

#line 62 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#62
// [msspan.subspan]
#line 63 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#63

#line 64 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#64
class all_type { public: constexpr explicit all_type() = default; };
#line 65 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#65

#line 66 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#66
/* inline */ constexpr all_type all ;
#line 67 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#67

#line 68 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#68
}}} // experimental::fundamentals_v3
#line 69 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#69

#line 70 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#70

#line 71 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#71

#line 72 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#72

#line 73 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#73
//--------------------------------------------------------------------------
#line 74 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#74
//--------------------------------------------------------------------------
#line 75 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#75

#line 76 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#76
namespace std {
#line 77 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#77
namespace experimental {
#line 78 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#78
inline namespace fundamentals_v3 {
#line 79 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#79

#line 80 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#80
template<class ElementType, class Extents, class LayoutPolicy, class AccessorPolicy>
#line 81 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#81
class basic_mdspan {
#line 82 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#82
public:
#line 83 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#83

#line 84 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#84
  // Domain and codomain types
#line 85 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#85

#line 86 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#86
  using extents_type     = Extents ;
#line 87 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#87
  using layout_type      = LayoutPolicy ;
#line 88 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#88
  using accessor_type    = AccessorPolicy ;
#line 89 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#89
  using mapping_type     = typename layout_type::template mapping<extents_type> ;
#line 90 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#90
  using element_type     = typename accessor_type::element_type ;
#line 91 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#91
  using value_type       = typename remove_cv<element_type>::type ;
#line 92 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#92
  using index_type       = ptrdiff_t ;
#line 93 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#93
  using difference_type  = ptrdiff_t ;
#line 94 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#94
  using pointer          = typename accessor_type::pointer;
#line 95 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#95
  using reference        = typename accessor_type::reference;
#line 96 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#96

#line 97 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#97
  // [mdspan.basic.cons]
#line 98 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#98

#line 99 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#99
  constexpr basic_mdspan() noexcept : acc_(), map_(), ptr_() {}
#line 100 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#100

#line 101 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#101
  constexpr basic_mdspan(basic_mdspan&& other) noexcept = default;
#line 102 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#102

#line 103 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#103
  constexpr basic_mdspan(const basic_mdspan & other) noexcept = default;
#line 104 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#104

#line 105 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#105
  basic_mdspan& operator=(const basic_mdspan & other) noexcept = default;
#line 106 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#106

#line 107 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#107
  basic_mdspan& operator=(basic_mdspan&& other) noexcept = default;
#line 108 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#108

#line 109 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#109
  template<class OtherElementType,
#line 110 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#110
           class OtherExtents,
#line 111 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#111
           class OtherLayoutPolicy,
#line 112 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#112
           class OtherAccessor>
#line 113 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#113
  constexpr basic_mdspan(
#line 114 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#114
    const basic_mdspan<OtherElementType,
#line 115 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#115
                       OtherExtents,
#line 116 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#116
                       OtherLayoutPolicy,
#line 117 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#117
                       OtherAccessor> & rhs ) noexcept
#line 118 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#118
    : acc_( rhs.acc_ )
#line 119 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#119
    , map_( rhs.map_ )
#line 120 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#120
    , ptr_( rhs.ptr_ )
#line 121 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#121
    {}
#line 122 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#122

#line 123 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#123
  template<class OtherElementType,
#line 124 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#124
           class OtherExtents,
#line 125 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#125
           class OtherLayoutPolicy,
#line 126 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#126
           class OtherAccessor>
#line 127 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#127
  basic_mdspan & operator = (
#line 128 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#128
    const basic_mdspan<OtherElementType,
#line 129 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#129
                       OtherExtents,
#line 130 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#130
                       OtherLayoutPolicy,
#line 131 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#131
                       OtherAccessor> & rhs ) noexcept
#line 132 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#132
    { acc_ = rhs.acc_ ; map_ = rhs.map_ ; ptr_ = rhs.ptr_ ; return *this ; }
#line 133 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#133

#line 134 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#134
  template<class... IndexType >
#line 135 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#135
  explicit constexpr basic_mdspan
#line 136 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#136
    ( pointer ptr , IndexType ... DynamicExtents ) noexcept
#line 137 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#137
    : acc_(accessor_type()), map_( extents_type(DynamicExtents...) ), ptr_(ptr) {}
#line 138 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#138

#line 139 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#139
  constexpr basic_mdspan( pointer ptr , const array<ptrdiff_t,extents_type::rank_dynamic()> dynamic_extents)
#line 140 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#140
    : acc_(accessor_type()), map_( extents_type(dynamic_extents)), ptr_(ptr) {}
#line 141 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#141

#line 142 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#142
  constexpr basic_mdspan( pointer ptr , const mapping_type m ) noexcept
#line 143 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#143
    : acc_(accessor_type()), map_( m ), ptr_(ptr) {}
#line 144 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#144
  
#line 145 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#145
  constexpr basic_mdspan( pointer ptr , const mapping_type m , const accessor_type a ) noexcept
#line 146 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#146
    : acc_(a), map_( m ), ptr_(ptr) {}
#line 147 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#147

#line 148 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#148
  // [mdspan.basic.mapping]
#line 149 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#149

#line 150 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#150
  // Enforce rank() <= sizeof...(IndexType)
#line 151 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#151
  template<class... IndexType >
#line 152 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#152
  constexpr typename enable_if<sizeof...(IndexType)==extents_type::rank(),reference>::type
#line 153 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#153
  operator()( IndexType... indices) const noexcept
#line 154 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#154
    { return acc_.access( ptr_ , map_( indices... ) ); }
#line 155 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#155

#line 156 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#156
  // Enforce rank() == 1
#line 157 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#157
  template<class IndexType>
#line 158 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#158
  constexpr typename enable_if<is_integral<IndexType>::value && 1==extents_type::rank(),reference>::type
#line 159 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#159
  operator[]( const IndexType i ) const noexcept
#line 160 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#160
    { return acc_.access( ptr_ , map_(i) ); }
#line 161 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#161

#line 162 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#162
  // [mdspan.basic.domobs]
#line 163 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#163

#line 164 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#164
  static constexpr int rank() noexcept
#line 165 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#165
    { return extents_type::rank(); }
#line 166 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#166

#line 167 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#167
  static constexpr int rank_dynamic() noexcept
#line 168 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#168
    { return extents_type::rank_dynamic(); }
#line 169 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#169

#line 170 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#170
  constexpr index_type static_extent( size_t k ) const noexcept
#line 171 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#171
    { return map_.extents().static_extent( k ); }
#line 172 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#172

#line 173 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#173
  constexpr index_type extent( int k ) const noexcept
#line 174 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#174
    { return map_.extents().extent( k ); }
#line 175 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#175

#line 176 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#176
  constexpr const extents_type & extents() const noexcept
#line 177 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#177
    { return map_.extents(); }
#line 178 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#178

#line 179 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#179
  // [mdspan.basic.codomain]
#line 180 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#180

#line 181 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#181
  // ------------------------------
#line 182 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#182

#line 183 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#183
//  constexpr fundamentals_v3::span<element_type> span() const noexcept
#line 184 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#184
//    { return fundamentals_v3::span<element_type>(acc_.decay(ptr_),map_.required_span_size()); }
#line 185 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#185

#line 186 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#186
  // ------------------------------
#line 187 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#187

#line 188 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#188
  // [mdspan.basic.obs]
#line 189 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#189

#line 190 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#190
  static constexpr bool is_always_unique()     noexcept { return mapping_type::is_always_unique(); }
#line 191 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#191
  static constexpr bool is_always_strided()    noexcept { return mapping_type::is_always_strided(); }
#line 192 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#192
  static constexpr bool is_always_contiguous() noexcept { return mapping_type::is_always_contiguous(); }
#line 193 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#193

#line 194 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#194
  constexpr bool is_unique() const noexcept  { return map_.is_unique(); }
#line 195 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#195
  constexpr bool is_strided() const noexcept { return map_.is_strided(); }
#line 196 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#196
  constexpr bool is_contiguous() const noexcept {return map_.is_contiguous();}
#line 197 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#197

#line 198 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#198
  constexpr index_type stride( size_t r ) const noexcept
#line 199 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#199
    { return map_.stride(r); }
#line 200 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#200

#line 201 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#201
  constexpr mapping_type mapping() const noexcept { return map_ ; }
#line 202 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#202

#line 203 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#203
  constexpr accessor_type accessor() const noexcept { return acc_ ; } 
#line 204 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#204

#line 205 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#205
  constexpr pointer data() const noexcept { return ptr_ ; } 
#line 206 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#206
private:
#line 207 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#207

#line 208 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#208
  accessor_type acc_ ;
#line 209 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#209
  mapping_type map_ ;
#line 210 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#210
  pointer  ptr_ ;
#line 211 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#211
};
#line 212 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#212

#line 213 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#213

#line 214 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#214
template<class T, ptrdiff_t... Indices>
#line 215 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#215
using mdspan = basic_mdspan<T,extents<Indices...>,layout_right,accessor_basic<T> > ;
#line 216 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#216

#line 217 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#217
}}} // experimental::fundamentals_v3
#line 218 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#218

#line 219 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#219

#line 220 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/mdspan.hpp#220

#line 1 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#1
//@HEADER
#line 2 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#2
// ************************************************************************
#line 3 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#3
//
#line 4 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#4
//                        Kokkos v. 2.0
#line 5 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#5
//              Copyright (2014) Sandia Corporation
#line 6 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#6
//
#line 7 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#7
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#line 8 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#8
// the U.S. Government retains certain rights in this software.
#line 9 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#9
//
#line 10 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#10
// Kokkos is licensed under 3-clause BSD terms of use:
#line 11 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#11
//
#line 12 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#12
// Redistribution and use in source and binary forms, with or without
#line 13 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#13
// modification, are permitted provided that the following conditions are
#line 14 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#14
// met:
#line 15 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#15
//
#line 16 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#16
// 1. Redistributions of source code must retain the above copyright
#line 17 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#17
// notice, this list of conditions and the following disclaimer.
#line 18 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#18
//
#line 19 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#19
// 2. Redistributions in binary form must reproduce the above copyright
#line 20 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#20
// notice, this list of conditions and the following disclaimer in the
#line 21 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#21
// documentation and/or other materials provided with the distribution.
#line 22 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#22
//
#line 23 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#23
// 3. Neither the name of the Corporation nor the names of the
#line 24 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#24
// contributors may be used to endorse or promote products derived from
#line 25 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#25
// this software without specific prior written permission.
#line 26 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#26
//
#line 27 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#27
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
#line 28 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#28
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#line 29 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#29
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#line 30 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#30
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
#line 31 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#31
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#line 32 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#32
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#line 33 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#33
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#line 34 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#34
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#line 35 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#35
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#line 36 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#36
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#line 37 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#37
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#line 38 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#38
//
#line 39 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#39
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
#line 40 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#40
//
#line 41 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#41
// ************************************************************************
#line 42 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#42
//@HEADER
#line 43 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#43

#line 44 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#44
//----------------------------------------------------------------------------
#line 45 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#45
//----------------------------------------------------------------------------
#line 46 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#46

#line 47 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#47
namespace std {
#line 48 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#48
namespace experimental {
#line 49 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#49
inline namespace fundamentals_v3 {
#line 50 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#50
namespace detail {
#line 51 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#51

#line 52 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#52
template<class ExtentsNew, class ExtentsOld, class ... SliceSpecifiers>
#line 53 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#53
struct compose_new_extents;
#line 54 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#54

#line 55 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#55
template<ptrdiff_t ... ExtentsNew, ptrdiff_t E0, ptrdiff_t ... ExtentsOld, class ... SliceSpecifiers>
#line 56 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#56
struct compose_new_extents<extents<ExtentsNew...>,extents<E0,ExtentsOld...>,all_type,SliceSpecifiers...> {
#line 57 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#57
  typedef compose_new_extents<extents<ExtentsNew...,E0>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents;
#line 58 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#58
  typedef typename next_compose_new_extents::extents_type extents_type; 
#line 59 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#59

#line 60 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#60
  template<class OrgExtents, class ... DynamicExtents>
#line 61 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#61
  static constexpr extents_type create_sub_extents(const OrgExtents e,  
#line 62 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#62
                        array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset,
#line 63 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#63
                                                   all_type, SliceSpecifiers...s, DynamicExtents...de) {
#line 64 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#64
    strides[sizeof...(ExtentsNew)] = strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
#line 65 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#65
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...);
#line 66 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#66
  }
#line 67 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#67
};
#line 68 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#68
template<ptrdiff_t ... ExtentsNew, ptrdiff_t ... ExtentsOld, class ... SliceSpecifiers>
#line 69 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#69
struct compose_new_extents<extents<ExtentsNew...>,extents<dynamic_extent,ExtentsOld...>,all_type,SliceSpecifiers...> {
#line 70 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#70
  typedef compose_new_extents<extents<ExtentsNew...,dynamic_extent>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents;
#line 71 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#71
  typedef typename next_compose_new_extents::extents_type extents_type; 
#line 72 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#72

#line 73 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#73
  template<class OrgExtents, class ... DynamicExtents>
#line 74 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#74
  static constexpr extents_type create_sub_extents(const OrgExtents e,  
#line 75 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#75
                        array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset,
#line 76 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#76
                                                   all_type, SliceSpecifiers...s, DynamicExtents...de) {
#line 77 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#77
    strides[sizeof...(ExtentsNew)] = strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
#line 78 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#78
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...,e.extent(OrgExtents::rank()-sizeof...(SliceSpecifiers)-1));
#line 79 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#79
  }
#line 80 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#80
};
#line 81 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#81

#line 82 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#82
template<ptrdiff_t ... ExtentsNew, ptrdiff_t E0, ptrdiff_t ... ExtentsOld, class IT, class ... SliceSpecifiers>
#line 83 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#83
struct compose_new_extents<extents<ExtentsNew...>,extents<E0,ExtentsOld...>,pair<IT,IT>,SliceSpecifiers...> {
#line 84 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#84
  typedef compose_new_extents<extents<ExtentsNew...,dynamic_extent>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents; 
#line 85 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#85
  typedef typename next_compose_new_extents::extents_type extents_type; 
#line 86 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#86

#line 87 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#87
  template<class OrgExtents, class ... DynamicExtents>
#line 88 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#88
  static constexpr extents_type create_sub_extents(const OrgExtents e, array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset,
#line 89 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#89
                                                   pair<IT,IT> p, SliceSpecifiers ... s, DynamicExtents...de) {
#line 90 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#90
    strides[sizeof...(ExtentsNew)] = strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
#line 91 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#91
    offset += p.first*strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
#line 92 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#92
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...,ptrdiff_t(p.second-p.first));
#line 93 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#93
  }
#line 94 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#94
};
#line 95 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#95

#line 96 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#96
template<ptrdiff_t ... ExtentsNew, ptrdiff_t E0, ptrdiff_t ... ExtentsOld, class IT, class ... SliceSpecifiers>
#line 97 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#97
struct compose_new_extents<extents<ExtentsNew...>,extents<E0,ExtentsOld...>,IT,SliceSpecifiers...> {
#line 98 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#98
  typedef compose_new_extents<extents<ExtentsNew...>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents;
#line 99 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#99
  typedef typename next_compose_new_extents::extents_type extents_type; 
#line 100 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#100

#line 101 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#101
  template<class OrgExtents, class ... DynamicExtents>
#line 102 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#102
  static constexpr extents_type create_sub_extents(const OrgExtents e, array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset, 
#line 103 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#103
                                                   const ptrdiff_t v, SliceSpecifiers...s,DynamicExtents...de) {
#line 104 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#104
    offset += v*strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
#line 105 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#105
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...);
#line 106 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#106
  }
#line 107 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#107
};
#line 108 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#108

#line 109 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#109
template<ptrdiff_t ... ExtentsNew>
#line 110 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#110
struct compose_new_extents<extents<ExtentsNew...>,extents<>> {
#line 111 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#111
  typedef extents<ExtentsNew...> extents_type; 
#line 112 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#112

#line 113 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#113
  template<class OrgExtents, class ... DynamicExtents>
#line 114 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#114
  static constexpr extents_type create_sub_extents(const OrgExtents, array<ptrdiff_t,OrgExtents::rank()>, ptrdiff_t, DynamicExtents...de) {
#line 115 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#115
    return extents_type(de...);
#line 116 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#116
  }
#line 117 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#117
  template<class OrgExtents>
#line 118 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#118
  static constexpr extents_type create_sub_extents(const OrgExtents, array<ptrdiff_t,OrgExtents::rank()>, ptrdiff_t) {
#line 119 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#119
    return extents_type();
#line 120 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#120
  }
#line 121 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#121
};
#line 122 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#122

#line 123 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#123

#line 124 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#124
template<class Extents, class...SliceSpecifiers>
#line 125 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#125
struct subspan_deduce_extents {
#line 126 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#126
  typedef typename compose_new_extents<extents<>,Extents,SliceSpecifiers...>::extents_type extents_type;
#line 127 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#127
  typedef array<ptrdiff_t,Extents::rank()> stride_type;
#line 128 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#128
  static constexpr extents_type create_sub_extents(const Extents e,stride_type& strides, ptrdiff_t& offset, SliceSpecifiers...s) {
#line 129 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#129
    return compose_new_extents<extents<>,Extents,SliceSpecifiers...>::create_sub_extents(e,strides,offset,s...);
#line 130 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#130
  }
#line 131 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#131
};
#line 132 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#132

#line 133 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#133
}
#line 134 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#134

#line 135 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#135
template<class ElementType, class Extents, class LayoutPolicy,
#line 136 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#136
           class AccessorPolicy, class... SliceSpecifiers>
#line 137 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#137
    basic_mdspan<ElementType, typename detail::subspan_deduce_extents<Extents,SliceSpecifiers...>::extents_type,
#line 138 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#138
                 layout_stride, typename AccessorPolicy::offset_policy >
#line 139 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#139
      subspan(const basic_mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>& src, SliceSpecifiers ... slices) noexcept {
#line 140 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#140
    typedef typename detail::subspan_deduce_extents<Extents,SliceSpecifiers...>::extents_type sub_extents_type;
#line 141 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#141
    typedef typename AccessorPolicy::offset_policy sub_accessor_policy;
#line 142 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#142
    typedef basic_mdspan<ElementType,sub_extents_type,layout_stride,sub_accessor_policy> sub_mdspan_type;
#line 143 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#143

#line 144 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#144
    array<ptrdiff_t,Extents::rank()> strides;
#line 145 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#145
    for(size_t r = 0; r<Extents::rank(); r++)
#line 146 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#146
      strides[r] = src.stride(r);
#line 147 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#147

#line 148 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#148
    ptrdiff_t offset = 0;
#line 149 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#149
    sub_extents_type sub_extents = detail::subspan_deduce_extents<Extents,SliceSpecifiers...>::create_sub_extents(src.extents(),strides,offset,slices...);
#line 150 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#150

#line 151 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#151
    array<ptrdiff_t,sub_extents_type::rank()> sub_strides;
#line 152 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#152
    for(size_t r = 0; r<sub_extents_type::rank(); r++)
#line 153 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#153
      sub_strides[r] = strides[r];
#line 154 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#154

#line 155 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#155
    typename AccessorPolicy::offset_policy::pointer ptr = src.accessor().offset(src.data(),offset);    
#line 156 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#156
    return sub_mdspan_type(ptr,typename sub_mdspan_type::mapping_type(sub_extents,sub_strides));
#line 157 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#157
  }
#line 158 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#158

#line 159 https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits/subspan.hpp#159
}}}

#include <cassert>
#include <type_traits>
#include <utility>
#include <array>
#include <initializer_list>

#endif
