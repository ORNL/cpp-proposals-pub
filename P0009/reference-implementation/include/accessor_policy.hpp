//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

namespace std {
namespace experimental {
namespace fundamentals_v3 {

// [mdspan.accessor.basic]
template<class ElementType>
class accessor_basic;


template<class ElementType>
struct accessor_basic {
  using element_type  = ElementType;
  using pointer       = ElementType*;
  using handle_type   = ElementType*;
  using offset_policy = accessor_basic;
  using reference     = ElementType&;

  static typename offset_policy::handle_type
    offset( handle_type h , size_t i ) noexcept
      { return h+i; }

  static reference deref( handle_type h , size_t i ) noexcept
    { return h[i]; }

  static pointer decay( handle_type h ) noexcept
    { return h; }
};

}}} // std::experimental::fundamentals_v3