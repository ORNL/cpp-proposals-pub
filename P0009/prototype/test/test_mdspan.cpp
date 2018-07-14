
#include <iostream>
#include <utility>
#include <memory>
#include <mdspan>

//----------------------------------------------------------------------------

template<class ElementType>
struct shared_ptr_offset : private std::shared_ptr<ElementType[]> {
private:
  using base = std::shared_ptr<ElementType[]> ;
	size_t off ;
public:
  shared_ptr_offset() = default ;
  shared_ptr_offset( shared_ptr_offset && ) = default ;
  shared_ptr_offset( const shared_ptr_offset & ) = default ;
  shared_ptr_offset & operator = ( shared_ptr_offset && ) = default ;
  shared_ptr_offset & operator = ( const shared_ptr_offset & ) = default ;

  shared_ptr_offset( const base & S , size_t N ) noexcept
	  : base(S), off(N) {}

  ElementType & operator[]( size_t i ) const noexcept
	  { return base::operator[][off+i]; };

  ElementType * get() const { return base::get()+off; }

  shared_ptr_offset operator+(size_t i) const noexcept
	  { return shared_ptr_offset( ( const base &)(*this) , off + i ); }
};

template<class ElementType,bool HasOffset=false>
struct access_policy_shared_ptr {
  using element_type  = ElementType;
  using pointer       = ElementType*;
  using handle_type   = typename std::conditional<HasOffset,shared_ptr_offset<ElementType>,std::shared_ptr<ElementType[]> >::type ;
  using reference     = ElementType&;
  using offset_policy = access_policy_shared_ptr<ElementType,true>;

  static typename offset_policy::handle_type
    offset( const handle_type & h , size_t i ) noexcept
      { return typename offset_policy::handle_type(h,i); }

  static reference deref( const handle_type & h , size_t i ) noexcept
    { return h[i]; }

  static pointer decay( const handle_type & h ) noexcept
    { return h.get(); }
};

//----------------------------------------------------------------------------

void test_extents()
{
  using namespace std::experimental::fundamentals_v3 ;

  {
    constexpr extents<2,4,8> exA ;

    static_assert( exA.rank() == 3 , "" );
    static_assert( exA.rank_dynamic() == 0 , "" );
	  static_assert( exA.static_extent(0) == 2 , "" );
	  static_assert( exA.static_extent(1) == 4 , "" );
	  static_assert( exA.static_extent(2) == 8 , "" );
	  static_assert( exA.extent(0) == 2 , "" );
	  static_assert( exA.extent(1) == 4 , "" );
	  static_assert( exA.extent(2) == 8 , "" );
	}

  {
    constexpr extents<2,dynamic_extent,dynamic_extent> exB(4,8);

    static_assert( exB.rank() == 3 , "" );
    static_assert( exB.rank_dynamic() == 2 , "" );
	  static_assert( exB.static_extent(0) == 2 , "" );
	  static_assert( exB.static_extent(1) == dynamic_extent , "" );
	  static_assert( exB.static_extent(2) == dynamic_extent , "" );
	  static_assert( exB.extent(0) == 2 , "" );
	  static_assert( exB.extent(1) == 4 , "" );
	  static_assert( exB.extent(2) == 8 , "" );
	}

  {
    constexpr extents<2,dynamic_extent,dynamic_extent> exC ;

    static_assert( exC.rank() == 3 , "" );
    static_assert( exC.rank_dynamic() == 2 , "" );
	  static_assert( exC.static_extent(0) == 2 , "" );
	  static_assert( exC.static_extent(1) == dynamic_extent , "" );
	  static_assert( exC.static_extent(2) == dynamic_extent , "" );
	  static_assert( exC.extent(0) == 2 , "" );
	  static_assert( exC.extent(1) == 0 , "" );
	  static_assert( exC.extent(2) == 0 , "" );
	}

  {
    constexpr extents<2,4,8> exA ;
    const extents<2,dynamic_extent,dynamic_extent> exB(exA);

    std::cout << "{ " << exB.extent(0) 
              << " , " << exB.extent(1) 
              << " , " << exB.extent(2) 
              << " ; " << exB.extent(3) 
              << " , " << exB.extent(3) 
              << " , " << exB.extent(4) 
							<< " }" << std::endl ;

    static_assert( exB.rank() == 3 , "" );
    static_assert( exB.rank_dynamic() == 2 , "" );
	  static_assert( exB.static_extent(0) == 2 , "" );
	  static_assert( exB.static_extent(1) == dynamic_extent , "" );
	  static_assert( exB.static_extent(2) == dynamic_extent , "" );
	  assert( exB.extent(0) == 2 );
	  assert( exB.extent(1) == 4 );
	  assert( exB.extent(2) == 8 );
	}
}

void test_layout()
{
  using namespace std::experimental::fundamentals_v3 ;

  {
	  using ext = extents<2,dynamic_extent,dynamic_extent>;
		using lmap = typename layout_right::template mapping< ext > ;

		constexpr ext exB(4,8);
		constexpr lmap lb( exB );

    assert( lb.extents() == exB );
    static_assert( lb.is_always_unique , "" );
    static_assert( lb.is_always_contiguous , "" );
    static_assert( lb.is_always_strided , "" );
		static_assert( lb.required_span_size() == exB.extent(0) * exB.extent(1) * exB.extent(2) , "" );

		static_assert( lb.stride(0) == exB.extent(1) * exB.extent(2) , "" );
		static_assert( lb.stride(1) == exB.extent(2) , "" );
		static_assert( lb.stride(2) == 1 , "" );
    
		static_assert( lb(0,0,0) == 0 , "" );
		static_assert( lb(0,0,1) == lb.stride(2) , "" );
		static_assert( lb(0,1,0) == lb.stride(1) , "" );
		static_assert( lb(1,0,0) == lb.stride(0) , "" );
	}

  {
	  using ext = extents<2,dynamic_extent,dynamic_extent>;
		using lmap = typename layout_left::template mapping< ext > ;

		constexpr ext exB(4,8);
		constexpr lmap lb( exB );

    assert( lb.extents() == exB );
    static_assert( lb.is_always_unique , "" );
    static_assert( lb.is_always_contiguous , "" );
    static_assert( lb.is_always_strided , "" );
		static_assert( lb.required_span_size() == exB.extent(0) * exB.extent(1) * exB.extent(2) , "" );

		static_assert( lb.stride(0) == 1 , "" );
		static_assert( lb.stride(1) == exB.extent(0) , "" );
		static_assert( lb.stride(2) == exB.extent(0) * exB.extent(1) , "" );
    
		static_assert( lb(0,0,0) == 0 , "" );
		static_assert( lb(1,0,0) == lb.stride(0) , "" );
		static_assert( lb(0,1,0) == lb.stride(1) , "" );
		static_assert( lb(0,0,1) == lb.stride(2) , "" );
	}
}

void test_accessor()
{
  using namespace std::experimental::fundamentals_v3 ;

  static_assert( std::is_same<int&,typename accessor_basic<int>::reference >::value , "" );

  using iacc = typename aligned_access_policy<int,8>::handle_type;
  using iref = typename aligned_access_policy<int,8>::reference;

	alignas(8) int x = 42 ;

	iacc px( & x );
	iref rx = x ;

	assert( px[0] == 42 );
	assert( rx == 42 );
}

void test_mdspan()
{
  using namespace std::experimental::fundamentals_v3 ;

  {
    using imd_t = mdspan<int,2,dynamic_extent,dynamic_extent> ;

    enum : size_t { LEN = imd_t::required_span_size(4,8) };
 
    int buffer[ LEN ] = {0};

	  imd_t imd( buffer , 4 , 8 );

    int val = 0 ;
    for ( int k = 0 ; k < imd.extent(2) ; ++k )
    for ( int j = 0 ; j < imd.extent(1) ; ++j )
    for ( int i = 0 ; i < imd.extent(0) ; ++i )
	    imd(i,j,k) = val++ ;

	  std::cout << std::endl << "right{" ;
	  for ( size_t i = 0 ; i < LEN ; ++i )
	    std::cout << " " << buffer[i];
	  std::cout << " }" << std::endl ;
	}

  {
    using imd_t = basic_mdspan<int,extents<2,dynamic_extent,dynamic_extent>,layout_left> ;

    enum : size_t { LEN = imd_t::required_span_size(4,8) };
 
    int buffer[ LEN ] = {0};

	  imd_t imd( buffer , 4 , 8 );

    int val = 0 ;
    for ( int k = 0 ; k < imd.extent(2) ; ++k )
    for ( int j = 0 ; j < imd.extent(1) ; ++j )
    for ( int i = 0 ; i < imd.extent(0) ; ++i )
	    imd(i,j,k) = val++ ;

	  std::cout << std::endl << "left{" ;
	  for ( size_t i = 0 ; i < LEN ; ++i )
	    std::cout << " " << buffer[i];
	  std::cout << " }" << std::endl ;
	}
}

void test_mdspan_plugin()
{
  using namespace std::experimental::fundamentals_v3 ;

  using matrix_type = basic_mdspan<double,extents<dynamic_extent,dynamic_extent>,layout_left,access_policy_shared_ptr<double> >;

  std::shared_ptr<double[]> buf( new double[100] );

  matrix_type m(buf,10,10);
}

int main()
{
  test_extents();
  test_layout();
	test_accessor();
	test_mdspan();
	test_mdspan_plugin();
	return 0 ;
}

