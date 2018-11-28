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

#include <cstddef> // std::ptrdiff_t
#include <array> // std::array

namespace std {
namespace experimental {
inline namespace fundamentals_v3 {

enum : std::ptrdiff_t { dynamic_extent = -1 };


// [mdspan.extents]
template< std::ptrdiff_t ... StaticExtents >
class extents;

// [mdspan.extents.compare]
template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
constexpr bool operator==(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept;

template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
constexpr bool operator!=(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept;


}}}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace detail {
  template< int R, std::ptrdiff_t ... StaticExtents >
  struct extents_analyse;

  template< int R, std::ptrdiff_t E0, std::ptrdiff_t ... StaticExtents >
  struct extents_analyse<R,E0,StaticExtents...> {

    typedef extents_analyse<R-1,StaticExtents...> next_extents_analyse;

    static constexpr std::size_t rank() noexcept { return next_extents_analyse::rank()+1; }
    static constexpr std::size_t rank_dynamic() noexcept { return next_extents_analyse::rank_dynamic(); }

    next_extents_analyse next;

    extents_analyse():next() {};
 
    template<class...DynamicExtents>
    extents_analyse(DynamicExtents...de):next(de...) {}

    template<std::size_t Rank>
    extents_analyse(const array<std::ptrdiff_t,Rank>& de,const std::size_t r):next(de,r) {}

    template<std::ptrdiff_t...OtherStaticExtents>
    extents_analyse(extents_analyse<R,OtherStaticExtents...> rhs):next(rhs.next) {}    

    template<std::ptrdiff_t...OtherStaticExtents>
    extents_analyse operator= (extents_analyse<R,OtherStaticExtents...> rhs) {
      next = rhs.next;
      return *this;
    }
    
    constexpr std::ptrdiff_t extent(const std::size_t r) const noexcept {
      if(r==R) return E0;
      return next.extent(r); 
    }
    static constexpr std::ptrdiff_t static_extent(const std::size_t r) noexcept {
      if(r==R) return E0;
      return next_extents_analyse::static_extent(r);
    }
  };

  template< int R, std::ptrdiff_t ... StaticExtents >
  struct extents_analyse<R,dynamic_extent,StaticExtents...> {
    typedef extents_analyse<R-1,StaticExtents...> next_extents_analyse;

    static constexpr std::size_t rank() noexcept { return next_extents_analyse::rank()+1; }
    static constexpr std::size_t rank_dynamic() noexcept { return next_extents_analyse::rank_dynamic()+1; }

    next_extents_analyse next;
    std::ptrdiff_t this_extent;

    extents_analyse():next(),this_extent(0) {}

    template<class...DynamicExtents>
    extents_analyse(std::ptrdiff_t E, DynamicExtents...de):next(de...),this_extent(E) {}

    template<std::size_t Rank>
    extents_analyse(const array<std::ptrdiff_t,Rank>& de, const std::size_t r):next(de,r+1),this_extent(de[r]) {}

    template<std::ptrdiff_t...OtherStaticExtents>
    extents_analyse(extents_analyse<R,OtherStaticExtents...> rhs):next(rhs.next),this_extent(rhs.extent(R)) {}    

    template<std::ptrdiff_t...OtherStaticExtents>
    extents_analyse & operator= (extents_analyse<R,OtherStaticExtents...> rhs) {
      next = rhs.next;
      this_extent = rhs.extent(R);
      return *this;
    }    

    constexpr std::ptrdiff_t extent(const std::size_t r) const noexcept {
      if(r==R) return this_extent; 
      else return next.extent(r);
    }
    static constexpr std::ptrdiff_t static_extent(const std::size_t r) noexcept {
      if(r==R) return dynamic_extent;
      return next_extents_analyse::static_extent(r);
    }
  };

  template<>
  struct extents_analyse<0> {
    static constexpr std::size_t rank() noexcept { return 0; }
    static constexpr std::size_t rank_dynamic() noexcept { return 0; }

    extents_analyse() {}

    template<std::size_t Rank>
    extents_analyse(const array<std::ptrdiff_t,Rank>&, const std::size_t) {}

    //extents_analyse & operator=(extents_analyse) = default;

    constexpr std::ptrdiff_t extent(const std::size_t) const noexcept {
      return 1;
    }
    static constexpr std::ptrdiff_t static_extent(const std::size_t) noexcept {
      return 1;
    }

  };
}

template< std::ptrdiff_t ... StaticExtents >
class extents
{
private:

  template< std::ptrdiff_t... > friend class extents ;

  typedef detail::extents_analyse<sizeof...(StaticExtents),StaticExtents...> extents_analyse_t;
  extents_analyse_t impl;
public:

  using index_type = std::ptrdiff_t ;

  constexpr extents() noexcept {}

  constexpr extents( extents && ) noexcept = default ;

  constexpr extents( const extents & ) noexcept = default ;

  template< class ... IndexType >
  constexpr extents( std::ptrdiff_t dn,
                              IndexType ... DynamicExtents ) noexcept
    : impl( dn , DynamicExtents... ) 
    { static_assert( 1+sizeof...(DynamicExtents) == rank_dynamic() , "" ); }

  constexpr extents( const array<std::ptrdiff_t,extents_analyse_t::rank_dynamic()> dynamic_extents) noexcept
    : impl(dynamic_extents,0) {}

  template<std::ptrdiff_t... OtherStaticExtents>
  extents( const extents<OtherStaticExtents...>& other )
    : impl( other.impl ) {}

  extents & operator = ( extents && ) noexcept = default;

  extents & operator = ( const extents & ) noexcept = default;

  template<std::ptrdiff_t... OtherStaticExtents>
  extents & operator = ( const extents<OtherStaticExtents...>& other )
    { impl = other.impl; return *this ; }

  ~extents() = default ;

  // [mdspan.extents.obs]

  static constexpr std::size_t rank() noexcept
    { return sizeof...(StaticExtents); }

  static constexpr std::size_t rank_dynamic() noexcept 
    { return extents_analyse_t::rank_dynamic() ; }

  static constexpr index_type static_extent(std::size_t k) noexcept
    { return extents_analyse_t::static_extent(rank()-k); }

  constexpr index_type extent(std::size_t k) const noexcept
    { return impl.extent(rank()-k); }

};

template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
constexpr bool operator==(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept { 
  bool equal = lhs.rank() == rhs.rank();
  for(std::size_t r = 0; r<lhs.rank(); r++)
    equal = equal && ( lhs.extent(r) == rhs.extent(r) ); 
  return equal; 
}

template<std::ptrdiff_t... LHS, std::ptrdiff_t... RHS>
constexpr bool operator!=(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept { 
  return !(lhs==rhs);
}

}}} // std::experimental::fundamentals_v3

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
