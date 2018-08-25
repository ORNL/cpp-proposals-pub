namespace std {
namespace experimental {
namespace fundamentals_v3 {

enum : ptrdiff_t { dynamic_extent = -1 };


// [mdspan.extents]
template< ptrdiff_t ... StaticExtents >
class extents;

// [mdspan.extents.compare]
template<ptrdiff_t... LHS, ptrdiff_t... RHS>
HOST_DEVICE
constexpr bool operator==(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept;

template<ptrdiff_t... LHS, ptrdiff_t... RHS>
HOST_DEVICE
constexpr bool operator!=(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept;

}}}

#include <mdspan_helper.hpp>

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

template< ptrdiff_t ... StaticExtents >
class extents : private detail::extents_helper<0,StaticExtents...>
{
private:

  template< ptrdiff_t... > friend class extents ;

  using helper  = detail::extents_helper<0,StaticExtents...> ;

public:

  using index_type = ptrdiff_t ;

  HOST_DEVICE
  constexpr extents() noexcept : helper() {}

  HOST_DEVICE
  constexpr extents( extents && ) noexcept = default ;

  HOST_DEVICE
  constexpr extents( const extents & ) noexcept = default ;

  template< class ... IndexType >
  constexpr explicit extents( ptrdiff_t dn,
                              IndexType ... DynamicExtents ) noexcept
    : helper( dn , DynamicExtents... ) 
    { static_assert( 1+sizeof...(DynamicExtents) == helper::RankDynamic , "" ); }

  template<ptrdiff_t... OtherStaticExtents>
  extents( const extents<OtherStaticExtents...>& other )
    : helper( (const detail::extents_helper<0,OtherStaticExtents...> &) other ) {}

  HOST_DEVICE
  extents & operator = ( extents && ) noexcept = default;

  HOST_DEVICE
  extents & operator = ( const extents & ) noexcept = default;

  template<ptrdiff_t... OtherStaticExtents>
  extents & operator = ( const extents<OtherStaticExtents...>& other )
    { helper::operator=( (const detail::extents_helper<0,OtherStaticExtents...> &) other ); return *this ; }

  HOST_DEVICE
  ~extents() = default ;

  // [mdspan.extents.obs]

  HOST_DEVICE
  static constexpr size_t rank() noexcept
    { return sizeof...(StaticExtents); }

  HOST_DEVICE
  static constexpr size_t rank_dynamic() noexcept 
    { return helper::RankDynamic ; }

  HOST_DEVICE
  static constexpr index_type static_extent(size_t k) noexcept
    { return helper::static_extent(k); }

  HOST_DEVICE
  constexpr index_type extent(size_t k) const noexcept
    { return helper::extent(k); }

  // implementation details

    template<size_t K>
    HOST_DEVICE
    constexpr index_type extent() const noexcept
      { return helper::template extent<K>(); }

    HOST_DEVICE constexpr index_type extent() const noexcept
      { return helper::N ; }

    HOST_DEVICE constexpr const typename helper::next_t & next() const noexcept
      { return (const typename helper::next_t &) (*this); };

    HOST_DEVICE constexpr index_type product(size_t i, size_t j) const noexcept
      { return helper::product(i,j); }

    template<ptrdiff_t... OtherStaticExtents>
    HOST_DEVICE constexpr bool
    equal( const extents<OtherStaticExtents...>& other ) const noexcept
      { return rank() == other.rank() && helper::equal( other ); }
};

//----------------------------------------------------------------------------

template<ptrdiff_t... LHS, ptrdiff_t... RHS>
HOST_DEVICE
constexpr bool operator==(const extents<LHS...>& lhs,
                          const extents<RHS...>& rhs) noexcept
{ return lhs.equal(rhs); }

}}} // std::experimental::fundamentals_v3

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------