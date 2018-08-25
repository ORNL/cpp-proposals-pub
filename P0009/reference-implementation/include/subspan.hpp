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
struct is_slice_range< T, typename enable_if< is_integral<T>::value >::type > : public false_type {};

// assuming size == 2
template<class T>
struct is_slice_range< initializer_list<T>, typename enable_if< is_integral<T>::value >::type > : public true_type {};

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
	  conditional< is_same<LayoutPolicy,layout_left>::value ||
	                    is_same<LayoutPolicy,layout_right>::value ||
	                    is_same<LayoutPolicy,layout_stride>::value ,
										  layout_stride , void >::type ;

	using type = basic_mdspan<ElementType, typename extents_dynamic<Rank>::type, layout, Accessor > ;
};




}}}}
