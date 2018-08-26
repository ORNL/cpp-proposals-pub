//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

// [mdspan.layout]
class layout_right ;
class layout_left ;
class layout_stride ;

}}}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


namespace std {
namespace experimental {
namespace fundamentals_v3 {

struct layout_none {

  template<class Extents>
  class mapping {
  private:

    static_assert( Extents::rank() <= 1 , "" );

    Extents m_extents ;

  public:

    using index_type = ptrdiff_t ;

    HOST_DEVICE
    constexpr mapping() noexcept = default ;

    HOST_DEVICE
    constexpr mapping( mapping && ) noexcept = default ;

    HOST_DEVICE
    constexpr mapping( const mapping & ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( mapping && ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( const mapping & ) noexcept = default ;

    HOST_DEVICE
    constexpr mapping( const Extents & ext ) noexcept
      : m_extents( ext ) {}

    constexpr const Extents & extents() const noexcept { return m_extents ; }

    template<class... Indices>
    constexpr mapping( Indices... DynamicExtents ) noexcept
      : m_extents( DynamicExtents... ) {}

    constexpr index_type required_span_size() const noexcept
      { return m_extents.extent(0); }

    constexpr index_type operator()() const noexcept { return 0 ; }
    constexpr index_type operator()( index_type i ) const noexcept { return i ; }

    static constexpr bool is_always_unique()     noexcept { return true ; }
    static constexpr bool is_always_contiguous() noexcept { return true ; }
    static constexpr bool is_always_strided()    noexcept { return true ; }

    constexpr bool is_unique()     const noexcept { return true ; }
    constexpr bool is_contiguous() const noexcept { return true ; }
    constexpr bool is_strided()    const noexcept { return true ; }

    static constexpr index_type stride(size_t) noexcept { return 1 ; }
  }; // struct mapping

}; // struct layout_none

}}} // experimental::fundamentals_v3

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

struct layout_right {

  template<class Extents>
  class mapping {
  private:

    Extents m_extents ;

  public:

    using index_type = ptrdiff_t ;
    using extents_type = Extents ;

    HOST_DEVICE
    constexpr mapping() noexcept = default ;

    HOST_DEVICE
    constexpr mapping( mapping && ) noexcept = default ;

    HOST_DEVICE
    constexpr mapping( const mapping & ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( mapping && ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( const mapping & ) noexcept = default ;

    HOST_DEVICE
    constexpr mapping( const Extents & ext ) noexcept
      : m_extents( ext ) {}

    constexpr const Extents & extents() const noexcept { return m_extents ; }

    template<class... Indices>
    constexpr mapping( Indices... DynamicExtents ) noexcept
      : m_extents( DynamicExtents... ) {}

  private:

    // ( ( ( ( i0 ) * N1 + i1 ) * N2 + i2 ) * N3 + i3 ) ...

    template<class Ext, class ... Indices >
    static constexpr index_type
    offset( const Ext & ext,
            index_type sum,
            index_type i) noexcept
      { return sum * ext.extent() + i ; }

    template<class Ext, class ... Indices >
    static constexpr index_type
    offset( const Ext & ext,
            index_type sum,
            index_type i,
            Indices... indices ) noexcept
      {
        return mapping::offset( ext.next(), sum * ext.extent() + i, indices...);
      }

  public:

    constexpr index_type required_span_size() const noexcept
      { return m_extents.product(0,m_extents.rank()); }

    template<class ... Indices >
    constexpr
    typename enable_if<sizeof...(Indices) == Extents::rank(),index_type>::type
    operator()( Indices ... indices ) const noexcept
      { return mapping::offset( m_extents, 0, indices... ); }

    static constexpr bool is_always_unique()     noexcept { return true ; }
    static constexpr bool is_always_contiguous() noexcept { return true ; }
    static constexpr bool is_always_strided()    noexcept { return true ; }

    constexpr bool is_unique()     const noexcept { return true ; }
    constexpr bool is_contiguous() const noexcept { return true ; }
    constexpr bool is_strided()    const noexcept { return true ; }

    constexpr index_type stride(size_t r) const noexcept
      { return m_extents.product(r+1,m_extents.rank()); }

  }; // struct mapping

}; // struct layout_right

}}} // experimental::fundamentals_v3

//----------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

struct layout_left {

  template<class Extents>
  class mapping {
  private:

    Extents m_extents ;

  public:

    using index_type = ptrdiff_t ;
    using extents_type = Extents ;

    HOST_DEVICE
    constexpr mapping() noexcept = default ;

    HOST_DEVICE
    constexpr mapping( mapping && ) noexcept = default ;

    HOST_DEVICE
    constexpr mapping( const mapping & ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( mapping && ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( const mapping & ) noexcept = default ;

    HOST_DEVICE
    constexpr mapping( const Extents & ext ) noexcept
      : m_extents( ext ) {}

    constexpr const Extents & extents() const noexcept { return m_extents ; }

    template<class... Indices>
    constexpr mapping( Indices... DynamicExtents ) noexcept
      : m_extents( DynamicExtents... ) {}

  private:

    // ( i0 + N0 * ( i1 + N1 * ( i2 + N2 * ( ... ) ) ) )

    template<class Ext >
    HOST_DEVICE
    static constexpr index_type
    offset( const Ext & ) noexcept
      { return 0 ; }

    template<class Ext , class ... IndexType >
    HOST_DEVICE
    static constexpr index_type
    offset( const Ext & ext, index_type i, IndexType... indices ) noexcept
      { return i + ext.extent() * mapping::offset( ext.next(), indices... ); }

  public:

    constexpr index_type required_span_size() const noexcept
      { return m_extents.product(0,m_extents.rank()); }

    template<class ... Indices >
    constexpr
    typename enable_if<sizeof...(Indices) == Extents::rank(),index_type>::type
    operator()( Indices ... indices ) const noexcept
      { return mapping::offset( m_extents, indices... ); }

/*
    template<class Index0, class Index1, class Index2 >
    typename enable_if< is_integral<Index0>::value &&
                             is_integral<Index1>::value &&
                             is_integral<Index2>::value &&
                             3 == Extents::rank() , index_type >::type
    operator()( Index0 i0 , Index1 i1 , Index2 i2 ) const noexcept
      { return i0 + m_extents.template extent<0>() * (
               i1 + m_extents.template extent<1>() * ( i2 ) ); }
*/

    static constexpr bool is_always_unique()     noexcept { return true ; }
    static constexpr bool is_always_contiguous() noexcept { return true ; }
    static constexpr bool is_always_strided()    noexcept { return true ; }

    constexpr bool is_unique()     const noexcept { return true ; }
    constexpr bool is_contiguous() const noexcept { return true ; }
    constexpr bool is_strided()    const noexcept { return true ; }

    constexpr index_type stride(size_t r) const noexcept
      { return m_extents.product(0,r); }

  }; // struct mapping

}; // struct layout_left

}}} // experimental::fundamentals_v3

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

struct layout_stride {

  template<class Extents>
  class mapping {
  private:

    using stride_t = array<ptrdiff_t,Extents::rank()> ;

    Extents   m_extents ;
    stride_t  m_stride ;
    int       m_contig ;

  public:

    using index_type = ptrdiff_t ;
    using extents_type = Extents ;

    HOST_DEVICE
    constexpr mapping() noexcept = default ;

    HOST_DEVICE
    constexpr mapping( mapping && ) noexcept = default ;

    HOST_DEVICE
    constexpr mapping( const mapping & ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( mapping && ) noexcept = default ;

    HOST_DEVICE
    mapping & operator = ( const mapping & ) noexcept = default ;

    HOST_DEVICE
    mapping( const Extents & ext, const stride_t & str ) noexcept
      : m_extents(ext), m_stride(str), m_contig(1)
      {
        int p[ Extents::rank() ? Extents::rank() : 1 ];

        // Fill permutation such that
        //   m_stride[ p[i] ] <= m_stride[ p[i+1] ]
        //
        for ( size_t i = 0 ; i < Extents::rank() ; ++i ) {

          int j = i ;

          while ( j && m_stride[i] < m_stride[ p[j-1] ] )
           { p[j] = p[j-1] ; --j ; }

          p[j] = i ;
        }

        for ( size_t i = 1 ; i < Extents::rank() ; ++i ) {
          const int j = p[i-1];
          const int k = p[i];
          const index_type prev = m_stride[j] * m_extents.extent(j);
          if ( m_stride[k] != prev ) { m_contig = 0 ; }
        }
      }

    constexpr const Extents & extents() const noexcept { return m_extents ; }

  private:

    // i0 * N0 + i1 * N1 + i2 * N2 + ...

    HOST_DEVICE
    constexpr index_type
    offset(int) const noexcept
      { return 0 ; }

    template<class... IndexType >
    HOST_DEVICE
    constexpr index_type
    offset( const int K, const index_type i, IndexType... indices ) const noexcept
      { return i * m_stride[K] + mapping::offset(K+1,indices...); }

  public:

    HOST_DEVICE
    index_type required_span_size() const noexcept
      {
        index_type max = 0 ;
        for ( size_t i = 0 ; i < Extents::rank() ; ++i )
          max += m_stride[i] * ( m_extents.extent(i) - 1 );
        return max ;
      }

    template<class ... Indices >
    constexpr
    typename enable_if<sizeof...(Indices) == Extents::rank(),index_type>::type
    operator()( Indices ... indices ) const noexcept
      { return mapping::offset(0, indices... ); }

/*
    template<class Index0, class Index1, class Index2 >
    typename enable_if< is_integral<Index0>::value &&
                             is_integral<Index1>::value &&
                             is_integral<Index2>::value &&
                             3 == Extents::rank() , index_type >::type
    operator()( Index0 i0 , Index1 i1 , Index2 i2 ) const noexcept
      { return i0 * m_stride_t[0] +
               i1 * m_stride_t[1] +
               i2 * m_stride_t[2] ; }
*/

    static constexpr bool is_always_unique()     noexcept { return true ; }
    static constexpr bool is_always_contiguous() noexcept { return false ; }
    static constexpr bool is_always_strided()    noexcept { return true ; }

    constexpr bool is_unique()     const noexcept { return true ; }
    constexpr bool is_contiguous() const noexcept { return m_contig ; }
    constexpr bool is_strided()    const noexcept { return true ; }

    constexpr index_type stride(size_t r) const noexcept
      { return m_stride[r]; }

  }; // struct mapping

}; // struct layout_stride

}}} // experimental::fundamentals_v3
