
namespace std {
namespace experimental {
namespace fundamentals_v3 {
namespace detail {

template< size_t R , ptrdiff_t StaticExtent >
struct extent_value
{
  enum : ptrdiff_t { N = StaticExtent };

	HOST_DEVICE
	constexpr extent_value() noexcept {}

	HOST_DEVICE
	constexpr explicit extent_value( ptrdiff_t ) noexcept {}
};

template<size_t R >
struct extent_value<R,dynamic_extent> {
  ptrdiff_t N ;

	HOST_DEVICE
	constexpr extent_value() noexcept : N(0) {}

	HOST_DEVICE
	constexpr explicit extent_value( ptrdiff_t dn ) noexcept : N(dn) {}
};

template< size_t R , ptrdiff_t ... StaticExtents >
class extents_helper ;

// End of StaticExtents... expansion
template< size_t R >
class extents_helper<R> {
public:
  using index_type = ptrdiff_t ;
  enum : size_t { RankDynamic = 0 };
  enum : ptrdiff_t { N = 1 };

	HOST_DEVICE
  static constexpr index_type static_extent( size_t ) noexcept
	  { return N ; }

	HOST_DEVICE
  static constexpr index_type extent( size_t ) noexcept
	  { return N ; }

  template<size_t>
	HOST_DEVICE
	constexpr index_type extent() const noexcept { return N ; }

	HOST_DEVICE
	constexpr index_type extent() const noexcept { return N ; }

	HOST_DEVICE
  constexpr const extents_helper & next() const noexcept { return *this ; }

	HOST_DEVICE
	constexpr index_type product(size_t,size_t) const noexcept { return N ; }

	HOST_DEVICE
	bool equal( const extents_helper & ) const noexcept { return true ; }
};

// Iteration 'R' of StaticExtents... expansion
template< size_t R , ptrdiff_t SN , ptrdiff_t ... TailSN >
class extents_helper<R,SN,TailSN...> 
  : public extent_value<R,SN>,
    public extents_helper<R+1,TailSN...> {
public:
  using index_type = ptrdiff_t ;
	using val_t  = extent_value<R,SN>;
  using next_t = extents_helper<R+1,TailSN...>;

  using val_t::N ;

  enum : size_t { is_dynamic = SN == dynamic_extent ? 1 : 0 };
  enum : size_t { RankDynamic = next_t::RankDynamic + is_dynamic };

  HOST_DEVICE
	constexpr extents_helper() noexcept = default ;

  HOST_DEVICE
	constexpr extents_helper( extents_helper && ) noexcept = default ;

  HOST_DEVICE
	constexpr extents_helper( const extents_helper & ) noexcept = default ;

  HOST_DEVICE
  extents_helper & operator = ( extents_helper && ) noexcept = default ;

  HOST_DEVICE
  extents_helper & operator = ( const extents_helper & ) noexcept = default ;

  // Constructor to peel dynamic extent
  template<class... TailDN >
  HOST_DEVICE
	constexpr extents_helper( std::true_type , index_type dn, TailDN... tail )
	  : val_t(dn), next_t( tail...)
		{}

  // Constructor to skip dynamic extent
  template<class...Indices>
  HOST_DEVICE
	constexpr extents_helper( std::false_type , Indices... indices )
	  : val_t(), next_t( indices...)
		{}

  // Constructor with dynamic extents
  template<class... Indices>
  HOST_DEVICE
	constexpr extents_helper( ptrdiff_t dn , Indices... indices )
	  : extents_helper(std::integral_constant<bool,is_dynamic>(), dn, indices... ) {}

  template<ptrdiff_t... OtherStaticExtents>
	HOST_DEVICE
	constexpr extents_helper(
	  const extents_helper<R,OtherStaticExtents...> & other ) noexcept
		: val_t( other.N )
		, next_t( (const typename extents_helper<R,OtherStaticExtents...>::next_t &) other )
		{}

  template<ptrdiff_t... OtherStaticExtents>
	HOST_DEVICE
	extents_helper & operator = (
	  const extents_helper<R,OtherStaticExtents...> & other ) noexcept
		{
		  val_t::operator=( val_t(other.N) );
			next_t::operator=( (const typename extents_helper<R,OtherStaticExtents...>::next_t &) other );
		  assert( N == other.N );
			return *this ;
		}

	HOST_DEVICE
  static constexpr index_type static_extent( size_t k ) noexcept
	  { return k == 0 ? SN : next_t::static_extent(k-1); }

	HOST_DEVICE
  constexpr index_type extent( size_t k ) const noexcept
		{ return k == 0 ? N : next_t::extent(k-1); }

  template<size_t K>
	HOST_DEVICE
	constexpr index_type extent() const noexcept
	  { return K == R ? N : next_t::template extent<K>(); }

	HOST_DEVICE
	constexpr index_type extent() const noexcept { return N ; }

	HOST_DEVICE
	constexpr const next_t & next() const noexcept
	  { return (const next_t &)(*this); }

	HOST_DEVICE
	constexpr index_type product(size_t i, size_t j) const noexcept
	  { return ( i <= R && R < j ? N : 1 ) * next_t::product(i,j); }

  template<ptrdiff_t... OtherStaticExtents>
	HOST_DEVICE
	bool equal( const extents_helper<R,OtherStaticExtents...> & other ) const noexcept
		{ return ( N == other.N ) && ( next().equal( other.next() ) ); }
};

}}}}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {
namespace detail {

template<size_t R>
struct extents_dynamic ;

template<>
struct extents_dynamic<1> {
  using type = extents<dynamic_extent> ;
};
template<>
struct extents_dynamic<2> {
  using type = extents<dynamic_extent,dynamic_extent> ;
};
template<>
struct extents_dynamic<3> {
  using type = extents<dynamic_extent,dynamic_extent,dynamic_extent> ;
};

template<class Slice, class Enable = void> struct is_slice_range ;

template<class T>
struct is_slice_range< T, typename std::enable_if< std::is_integral<T>::value >::type > : public std::false_type {};

// assuming size == 2
template<class T>
struct is_slice_range< std::initializer_list<T>, typename std::enable_if< std::is_integral<T>::value >::type > : public std::true_type {};

template<class ElementType,
         class Extents,
         class LayoutPolicy,
         class Accessor,
         class ... SliceSpecifiers>
struct subspan_deduction {

  static_assert( sizeof...(SliceSpecifiers) == Extents::rank() , "" );

	static constexpr size_t sum() noexcept { return 0 ; }

	template<class... Flag>
	static constexpr size_t sum( size_t i , Flag... flag ) noexcept
	  { return i + sum( flag... ); }

  enum : size_t { Rank = sum( is_slice_range<SliceSpecifiers>::value... ) };

  using layout = typename
	  std::conditional< std::is_same<LayoutPolicy,layout_left>::value ||
	                    std::is_same<LayoutPolicy,layout_right>::value ||
	                    std::is_same<LayoutPolicy,layout_stride>::value ,
										  layout_stride , void >::type ;

	using type = basic_mdspan<ElementType, typename extents_dynamic<Rank>::type, layout, Accessor > ;
};



}}}}

