namespace std {
namespace experimental {
namespace fundamentals_v3 {

enum : ptrdiff_t { dynamic_extent = -1 };


// [mdspan.extents]
template< ptrdiff_t ... StaticExtents >
class extents;

// [mdspan.extents.compare]
template<ptrdiff_t... LHS, ptrdiff_t... RHS>
constexpr bool operator==(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept;

template<ptrdiff_t... LHS, ptrdiff_t... RHS>
constexpr bool operator!=(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept;


}}}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {
namespace detail {
  template< int R, ptrdiff_t ... StaticExtents >
  struct extents_analyse;

  template< int R, ptrdiff_t E0, ptrdiff_t ... StaticExtents >
  struct extents_analyse<R,E0,StaticExtents...> {

    typedef extents_analyse<R-1,StaticExtents...> next_extents_analyse;

    static constexpr size_t rank() noexcept { return next_extents_analyse::rank()+1; }
    static constexpr size_t rank_dynamic() noexcept { return next_extents_analyse::rank_dynamic(); }

    next_extents_analyse next;

    extents_analyse():next() {};
 
    template<class...DynamicExtents>
    extents_analyse(DynamicExtents...de):next(de...) {}

    template<ptrdiff_t...OtherStaticExtents>
    extents_analyse(extents_analyse<R,OtherStaticExtents...> rhs):next(rhs.next) {}    

    template<ptrdiff_t...OtherStaticExtents>
    extents_analyse operator= (extents_analyse<R,OtherStaticExtents...> rhs) {
      next = rhs.next;
      return *this;
    }
    
    constexpr ptrdiff_t extent(const size_t r) const noexcept {
      if(r==R) return E0;
      return next.extent(r); 
    }
    static constexpr ptrdiff_t static_extent(const size_t r) noexcept {
      if(r==R) return E0;
      return next_extents_analyse::static_extent(r);
    }
  };

  template< size_t R, ptrdiff_t ... StaticExtents >
  struct extents_analyse<R,dynamic_extent,StaticExtents...> {
    typedef extents_analyse<R-1,StaticExtents...> next_extents_analyse;

    static constexpr size_t rank() noexcept { return next_extents_analyse::rank()+1; }
    static constexpr size_t rank_dynamic() noexcept { return next_extents_analyse::rank_dynamic()+1; }

    next_extents_analyse next;
    ptrdiff_t this_extent;

    extents_analyse():next(),this_extent(0) {}

    template<class...DynamicExtents>
    extents_analyse(ptrdiff_t E, DynamicExtents...de):next(de...),this_extent(E) {}

    template<ptrdiff_t...OtherStaticExtents>
    extents_analyse(extents_analyse<R,OtherStaticExtents...> rhs):next(rhs.next),this_extent(rhs.extent(R)) {}    

    template<ptrdiff_t...OtherStaticExtents>
    extents_analyse & operator= (extents_analyse<R,OtherStaticExtents...> rhs) {
      next = rhs.next;
      this_extent = rhs.extent(R);
      return *this;
    }    

    constexpr ptrdiff_t extent(const size_t r) const noexcept {
      if(r==R) return this_extent; 
      else return next.extent(r);
    }
    static constexpr ptrdiff_t static_extent(const size_t r) noexcept {
      if(r==R) return dynamic_extent;
      return next_extents_analyse::static_extent(r);
    }
  };

  template<>
  struct extents_analyse<0> {
    static constexpr size_t rank() noexcept { return 0; }
    static constexpr size_t rank_dynamic() noexcept { return 0; }

    extents_analyse() {}

    //extents_analyse & operator=(extents_analyse) = default;

    constexpr ptrdiff_t extent(const size_t) const noexcept {
      return -1;
    }
    static constexpr ptrdiff_t static_extent(const size_t) noexcept {
      return -1;
    }

  };
}

template< ptrdiff_t ... StaticExtents >
class extents
{
private:

  template< ptrdiff_t... > friend class extents ;

  typedef detail::extents_analyse<sizeof...(StaticExtents),StaticExtents...> extents_analyse_t;
  extents_analyse_t impl;
public:

  using index_type = ptrdiff_t ;

  constexpr extents() noexcept {}

  constexpr extents( extents && ) noexcept = default ;

  constexpr extents( const extents & ) noexcept = default ;

  template< class ... IndexType >
  constexpr explicit extents( ptrdiff_t dn,
                              IndexType ... DynamicExtents ) noexcept
    : impl( dn , DynamicExtents... ) 
    { static_assert( 1+sizeof...(DynamicExtents) == rank_dynamic() , "" ); }

  template<ptrdiff_t... OtherStaticExtents>
  extents( const extents<OtherStaticExtents...>& other )
    : impl( other.impl ) {}

  extents & operator = ( extents && ) noexcept = default;

  extents & operator = ( const extents & ) noexcept = default;

  template<ptrdiff_t... OtherStaticExtents>
  extents & operator = ( const extents<OtherStaticExtents...>& other )
    { impl = other.impl; return *this ; }

  ~extents() = default ;

  // [mdspan.extents.obs]

  static constexpr size_t rank() noexcept
    { return sizeof...(StaticExtents); }

  static constexpr size_t rank_dynamic() noexcept 
    { return extents_analyse_t::rank_dynamic() ; }

  static constexpr index_type static_extent(size_t k) noexcept
    { return extents_analyse_t::static_extent(rank()-k); }

  constexpr index_type extent(size_t k) const noexcept
    { return impl.extent(rank()-k); }

};

template<ptrdiff_t... LHS, ptrdiff_t... RHS>
constexpr bool operator==(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept { 
  bool equal = lhs.rank() == rhs.rank();
  for(size_t r = 0; r<lhs.rank(); r++)
    equal = equal && ( lhs.extent(r) == rhs.extent(r) ); 
  return equal; 
}

template<ptrdiff_t... LHS, ptrdiff_t... RHS>
constexpr bool operator!=(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept { 
  return !(lhs==rhs);
}

}}} // std::experimental::fundamentals_v3

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
