


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {



// [mdspan.basic]
template<class ElementType,
         class Extents,
         class LayoutPolicy = layout_right,
         class AccessorPolicy = accessor_basic<ElementType> >
class basic_mdspan ;

// [msspan.subspan]

namespace detail {
template<class ElementType,
         class Extents,
         class LayoutPolicy,
         class AccessorPolicy,
         class ... SliceSpecifiers>
class subspan_deduction;
}

template<class ElementType,
         class Extents,
         class LayoutPolicy,
         class AccessorPolicy,
         class ... SliceSpecifiers>
HOST_DEVICE
  typename detail::subspan_deduction<ElementType,
                                     Extents,
                                     LayoutPolicy,
                                     AccessorPolicy,
                                     SliceSpecifiers...>::type
subspan(const basic_mdspan<ElementType,Extents,LayoutPolicy,AccessorPolicy> &,
        SliceSpecifiers...) noexcept ;

class all_type { public: constexpr explicit all_type() = default; };

/* inline */ constexpr all_type all ;

}}} // experimental::fundamentals_v3




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

template<class T, size_t N>
class aligned_accessor {
public:
  static_assert( ( 0 == ( N & ( N - 1 ))), "" );
  static_assert( ( 0 == ( N % sizeof(T))), "" );

  enum : size_t { align = N };

  using element_type = T;
  using reference    = T&;
  using pointer      = T*;
  using offset       = T*;

  constexpr aligned_accessor() noexcept {};
  constexpr aligned_accessor( const aligned_accessor & ) noexcept = default ;

  aligned_accessor( aligned_accessor && ) = delete ;
  aligned_accessor operator = ( aligned_accessor && ) = delete ;
  aligned_accessor operator = ( const aligned_accessor & ) = delete ;

  explicit aligned_accessor( T * other ) noexcept
    : ptr(other)
    {
      // Verify pointer alignment:
      assert( 0 == reinterpret_cast<uintptr_t>(ptr) % N );
    }

  // constexpr operator [[aligned(N)]] T* () const noexcept { return ptr };

  constexpr operator T*() const noexcept
    { return ptr; }

  constexpr reference operator[]( size_t i ) const noexcept
    { return ptr[i]; }

  // Offsetting looses the alignment attribute
  constexpr offset operator+( size_t i ) const noexcept
    { return ptr+i; }

private:

  // [[aligned(N)]] T * const ptr = 0 ;
  T * const ptr = 0 ;
};

template<class ElementType, size_t N>
struct aligned_access_policy {
  using element_type  = ElementType;
  using pointer       = ElementType*;
  using reference     = typename pointer::reference;
  using offset_policy = accessor_basic<ElementType>;

  static typename offset_policy::pointer
    offset( const pointer & h , size_t i ) noexcept
      { return h+i; }

  static reference deref( const pointer & h , size_t i ) noexcept
    { return h[i]; }

  static pointer decay( const pointer & h ) noexcept
    { return (pointer)h; }
};

}}} // experimental::fundamentals_v3

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

template<class ElementType, class Extents, class LayoutPolicy, class AccessorPolicy>
class basic_mdspan {
public:

  // Domain and codomain types

  using extents_type     = Extents ;
  using layout_type      = LayoutPolicy ;
  using accessor_type    = AccessorPolicy ;
  using mapping_type     = typename layout_type::template mapping<extents_type> ;
  using element_type     = typename accessor_type::element_type ;
  using value_type       = typename remove_cv<element_type>::type ;
  using index_type       = ptrdiff_t ;
  using difference_type  = ptrdiff_t ;
  using pointer          = typename accessor_type::pointer;
  using reference        = typename accessor_type::reference;

  // [mdspan.basic.cons]

  HOST_DEVICE
  constexpr basic_mdspan() noexcept : acc_(), map_(), ptr_() {}

  HOST_DEVICE
  constexpr basic_mdspan(basic_mdspan&& other) noexcept = default;

  HOST_DEVICE
  constexpr basic_mdspan(const basic_mdspan & other) noexcept = default;

  HOST_DEVICE
  basic_mdspan& operator=(const basic_mdspan & other) noexcept = default;

  HOST_DEVICE
  basic_mdspan& operator=(basic_mdspan&& other) noexcept = default;

  template<class OtherElementType,
           class OtherExtents,
           class OtherLayoutPolicy,
           class OtherAccessor>
  constexpr basic_mdspan(
    const basic_mdspan<OtherElementType,
                       OtherExtents,
                       OtherLayoutPolicy,
                       OtherAccessor> & rhs ) noexcept
    : acc_( rhs.acc_ )
    , map_( rhs.map_ )
    , ptr_( rhs.ptr_ )
    {}

  template<class OtherElementType,
           class OtherExtents,
           class OtherLayoutPolicy,
           class OtherAccessor>
  basic_mdspan & operator = (
    const basic_mdspan<OtherElementType,
                       OtherExtents,
                       OtherLayoutPolicy,
                       OtherAccessor> & rhs ) noexcept
    { acc_ = rhs.acc_ ; map_ = rhs.map_ ; ptr_ = rhs.ptr_ ; return *this ; }

  template<class... IndexType >
  explicit constexpr basic_mdspan
    ( pointer ptr , IndexType ... DynamicExtents ) noexcept
    : ptr_(ptr), acc_(accessor_type()), map_( DynamicExtents... ) {}

  constexpr basic_mdspan( pointer ptr , const mapping_type m ) noexcept
    : ptr_(ptr), acc_(accessor_type()), map_( m ) {}
  
  constexpr basic_mdspan( pointer ptr , const mapping_type m , const accessor_type a ) noexcept
    : ptr_(ptr), acc_(a), map_( m ) {}

  // [mdspan.basic.mapping]

  // Enforce rank() <= sizeof...(IndexType)
  template<class... IndexType >
  HOST_DEVICE
  constexpr
  typename enable_if<sizeof...(IndexType)==extents_type::rank(),reference>::type
  operator()( IndexType... indices) const noexcept
    { return acc_.access( ptr_ , map_( indices... ) ); }

  // Enforce rank() == 1
  template<class IndexType>
  HOST_DEVICE
  constexpr
  typename enable_if<is_integral<IndexType>::value && 1==extents_type::rank(),reference>::type
  operator[]( const IndexType i ) const noexcept
    { return acc_( ptr_ , map_(i) ); }

  // [mdspan.basic.domobs]

  static constexpr int rank() noexcept
    { return extents_type::rank(); }

  static constexpr int rank_dynamic() noexcept
    { return extents_type::rank_dynamic(); }

  constexpr index_type static_extent( size_t k ) const noexcept
    { return map_.extents().static_extent( k ); }

  constexpr index_type extent( int k ) const noexcept
    { return map_.extents().extent( k ); }

  constexpr const extents_type & extents() const noexcept
    { return map_.extents(); }

  // [mdspan.basic.codomain]

  // ------------------------------

//  constexpr fundamentals_v3::span<element_type> span() const noexcept
//	  { return fundamentals_v3::span<element_type>((pointer)acc_,map_.extents().extent(0)); }

  // ------------------------------

  // [mdspan.basic.obs]

  static constexpr bool is_always_unique()     noexcept { return mapping_type::is_always_unique(); }
  static constexpr bool is_always_strided()    noexcept { return mapping_type::is_always_strided(); }
  static constexpr bool is_always_contiguous() noexcept { return mapping_type::is_always_contiguous(); }

  HOST_DEVICE
  constexpr bool is_unique() const noexcept  { return map_.is_unique(); }
  HOST_DEVICE
  constexpr bool is_strided() const noexcept { return map_.is_strided(); }
  HOST_DEVICE
  constexpr bool is_contiguous() const noexcept {return map_.is_contiguous();}

  HOST_DEVICE
  constexpr index_type stride( size_t r ) const noexcept
    { return map_.stride(r); }

  HOST_DEVICE
  constexpr mapping_type mapping() const noexcept { return map_ ; }

  HOST_DEVICE
  constexpr accessor_type accessor() const noexcept { return acc_ ; } 

private:

  accessor_type acc_ ;
  mapping_type map_ ;
  pointer  ptr_ ;
};


template<class T, ptrdiff_t... Indices>
using mdspan = basic_mdspan<T,extents<Indices...>,layout_right,accessor_basic<T> > ;

}}} // experimental::fundamentals_v3

//--------------------------------------------------------------------------

#if 0

namespace std {
namespace experimental {
namespace fundamentals_v3 {

template< class MDSPAN , typename ... SliceSpecs >
mdspan< typename MDSPAN::element_type
      , typename detail::sub_extents_deduction
          < typename MDSPAN::properties::extents
          , SliceSpecs...
          >::type
      , layout_stride
      >
subspan( MDSPAN const & a , SliceSpecs const & ... slice_specs )
{
  typedef typename MDSPAN::properties::extents
    extents_input ;

  typedef detail::sub_extents_deduction< extents_input , SliceSpecs...  >
    deduction ;

  typedef typename deduction::type
    extents_output ;

  typedef
    mdspan< typename MDSPAN::element_type
          , extents_output
          , layout_stride
          > return_type ;

  constexpr int output_rank = extents_output::rank();

  ptrdiff_t offset = a.offset( detail::slices_begin( slice_specs )... );

  ptrdiff_t dyn[ output_rank ? output_rank : 1 ];
  ptrdiff_t str[ output_rank ? output_rank : 1 ];

  deduction::get( dyn , str , a , slice_specs... );

  typedef typename
    detail::mdspan_mapping< extents_output , layout_stride >::type
      mapping ;
  
  return return_type( a.data() + offset , mapping( dyn , str ) );
}

}}} // experimental::fundamentals_v3

#endif /* #if 0 */



