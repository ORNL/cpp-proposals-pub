//------------------------------------------------------------------------------
// std::experimental::atomic_ref
//
// reference implementation for compilers which support GNU atomic builtins
// https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
//
//------------------------------------------------------------------------------

#ifndef ATOMIC_REF_HPP
#define ATOMIC_REF_HPP

#include <atomic>
#include <type_traits>
#include <cstdint>
#include <cmath>


#if defined( _MSC_VER ) //msvc
  #error "Error: MSVC not currently supported"
#endif

#ifndef ATOMIC_REF_FORCEINLINE
  #define ATOMIC_REF_FORCEINLINE inline __attribute__((always_inline))
#endif

static_assert(  (__ATOMIC_RELAXED == static_cast<int>(std::memory_order_relaxed ))
             && (__ATOMIC_CONSUME == static_cast<int>(std::memory_order_consume ))
             && (__ATOMIC_ACQUIRE == static_cast<int>(std::memory_order_acquire ))
             && (__ATOMIC_RELEASE == static_cast<int>(std::memory_order_release ))
             && (__ATOMIC_ACQ_REL == static_cast<int>(std::memory_order_acq_rel ))
             && (__ATOMIC_SEQ_CST == static_cast<int>(std::memory_order_seq_cst ))
             , "Error: std::memory_order values are not equivalent to builtins"
             );

namespace std {
inline namespace is_20 {

//------------------------------------------------------------------------------
// Generic type
//------------------------------------------------------------------------------
template < class T, class Enable=void >
struct atomic_ref
{
private:
  T * ptr_;

  static_assert( std::is_trivially_copyable<T>::value
               , "Error: atomic_ref<T> requires a trivially copyable type" );

public:
  using value_type = T;
  static constexpr bool   is_always_lock_free = __atomic_always_lock_free( sizeof(T), 0);
  static constexpr size_t required_alignment  = std::alignment_of<T>::value;

  atomic_ref() = delete;
  atomic_ref & operator=( const atomic_ref & ) = delete;

  ATOMIC_REF_FORCEINLINE
  explicit atomic_ref( value_type & obj )
    : ptr_{&obj}
  {}

  ATOMIC_REF_FORCEINLINE
  atomic_ref( const atomic_ref & ref )
    : ptr_{ref.ptr_}
  {}

  ATOMIC_REF_FORCEINLINE
  value_type operator=( value_type desired ) const noexcept
  {
    store(desired);
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  operator value_type() const noexcept
  {
    return load();
  }

  ATOMIC_REF_FORCEINLINE
  bool is_lock_free() const noexcept
  {
    return __atomic_is_lock_free( sizeof(value_type), ptr_ );
  }

  ATOMIC_REF_FORCEINLINE
  void store( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    __atomic_store( ptr_, &desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    value_type result;
    __atomic_load( ptr_, &result, static_cast<int>(order) );
    return result;
  }

  ATOMIC_REF_FORCEINLINE
  value_type exchange( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    value_type result;
    __atomic_exchange( ptr_, &desired, &result, static_cast<int>(order) );
    return result;
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, true, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, false, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, true, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, false, static_cast<int>(success), static_cast<int>(failure) );
  }
};




//------------------------------------------------------------------------------
// Integral type
//------------------------------------------------------------------------------
template < class T>
struct atomic_ref< T, typename std::enable_if< std::is_integral<T>::value, void>::type >
{
private:
  T * ptr_;

  static_assert( std::is_trivially_copyable<T>::value
               , "Error: atomic_ref<T> requires a trivially copyable type" );

public:
  using value_type = T;
  using difference_type = value_type;
  static constexpr bool   is_always_lock_free = __atomic_always_lock_free( sizeof(T), 0);
  static constexpr size_t required_alignment  = std::alignment_of<T>::value;

  atomic_ref() = delete;
  atomic_ref & operator=( const atomic_ref & ) = delete;

  ATOMIC_REF_FORCEINLINE
  explicit atomic_ref( value_type & obj )
    : ptr_{&obj}
  {}

  ATOMIC_REF_FORCEINLINE
  atomic_ref( const atomic_ref & ref )
    : ptr_{ref.ptr_}
  {}

  ATOMIC_REF_FORCEINLINE
  value_type operator=( value_type desired ) const noexcept
  {
    store(desired);
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  operator value_type() const noexcept
  {
    return load();
  }

  ATOMIC_REF_FORCEINLINE
  bool is_lock_free() const noexcept
  {
    return __atomic_is_lock_free( sizeof(value_type), ptr_ );
  }

  ATOMIC_REF_FORCEINLINE
  void store( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    __atomic_store_n( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_load_n( ptr_, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type exchange( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_exchange_n( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_add( difference_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_add( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_sub( difference_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_sub( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_and( value_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_and( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_or( value_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_or( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_xor( value_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_xor( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator++() const noexcept
  {
    return __atomic_fetch_add( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator++(int) const noexcept
  {
    return __atomic_add_fetch( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator--() const noexcept
  {
    return __atomic_fetch_sub( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator--(int) const noexcept
  {
    return __atomic_sub_fetch( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator+=( difference_type val ) const noexcept
  {
    return __atomic_add_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator-=( difference_type val ) const noexcept
  {
    return __atomic_sub_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator&=( value_type val ) const noexcept
  {
    return __atomic_and_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator|=( value_type val ) const noexcept
  {
    return __atomic_or_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator^=( value_type val ) const noexcept
  {
    return __atomic_xor_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }
};


//------------------------------------------------------------------------------
// Floating type
//------------------------------------------------------------------------------
template < class T>
struct atomic_ref< T, typename std::enable_if< std::is_floating_point<T>::value, void>::type >
{
private:
  T * ptr_;

  static_assert( std::is_trivially_copyable<T>::value
               , "Error: atomic_ref<T> requires a trivially copyable type" );

public:
  using value_type = T;
  using difference_type = value_type;
  static constexpr bool   is_always_lock_free = __atomic_always_lock_free( sizeof(T), 0);
  static constexpr size_t required_alignment  = std::alignment_of<T>::value;

  atomic_ref() = delete;
  atomic_ref & operator=( const atomic_ref & ) = delete;

  ATOMIC_REF_FORCEINLINE
  explicit atomic_ref( value_type & obj )
    : ptr_{&obj}
  {}

  ATOMIC_REF_FORCEINLINE
  atomic_ref( const atomic_ref & ref )
    : ptr_{ref.ptr_}
  {}

  ATOMIC_REF_FORCEINLINE
  value_type operator=( value_type desired ) const noexcept
  {
    store(desired);
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  operator value_type() const noexcept
  {
    return load();
  }

  ATOMIC_REF_FORCEINLINE
  bool is_lock_free() const noexcept
  {
    return __atomic_is_lock_free( sizeof(value_type), ptr_ );
  }

  ATOMIC_REF_FORCEINLINE
  void store( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    __atomic_store( ptr_, &desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    value_type result;
    __atomic_load( ptr_, &result, static_cast<int>(order) );
    return result;
  }

  ATOMIC_REF_FORCEINLINE
  value_type exchange( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    value_type result;
    __atomic_exchange( ptr_, &desired, &result, static_cast<int>(order) );
    return result;
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, true, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, false, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, true, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, false, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_add( difference_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    value_type expected = load( std::memory_order_relaxed );
    value_type desired  = expected + val;

    while ( !compare_exchange_weak( expected, desired, order, std::memory_order_relaxed ) ) {
      desired  = expected + val;
      if (std::isnan(expected)) break;
    }
    return expected;
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_sub( difference_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    value_type expected = load( std::memory_order_relaxed );
    value_type desired  = expected - val;

    while ( !compare_exchange_weak( expected, desired, order, std::memory_order_relaxed ) ) {
      desired  = expected - val;
      if (std::isnan(expected)) break;
    }
    return expected;
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator+=( difference_type val ) const noexcept
  {
    value_type expected = load( std::memory_order_relaxed );
    value_type desired  = expected + val;

    while ( !compare_exchange_weak( expected, desired, std::memory_order_seq_cst, std::memory_order_relaxed ) ) {
      desired  = expected + val;
      if (std::isnan(expected)) break;
    }
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator-=( difference_type val ) const noexcept
  {
    value_type expected = load( std::memory_order_relaxed );
    value_type desired  = expected - val;

    while ( !compare_exchange_weak( expected, desired, std::memory_order_seq_cst, std::memory_order_relaxed ) ) {
      desired  = expected - val;
      if (std::isnan(expected)) break;
    }
    return desired;
  }
};


//------------------------------------------------------------------------------
// Pointer type
//------------------------------------------------------------------------------
template < class T>
struct atomic_ref< T*, void>
{
private:
  T ** ptr_;

  static_assert( std::is_trivially_copyable<T*>::value
               , "Error: atomic_ref<T> requires a trivially copyable type" );

  static constexpr uintptr_t stride = static_cast<uintptr_t>(sizeof(T));

public:
  using value_type = T*;
  using difference_type = ptrdiff_t;
  static constexpr bool   is_always_lock_free = __atomic_always_lock_free( sizeof(T*), 0);
  static constexpr size_t required_alignment  = std::alignment_of<T*>::value;

  atomic_ref() = delete;
  atomic_ref & operator=( const atomic_ref & ) = delete;

  ATOMIC_REF_FORCEINLINE
  explicit atomic_ref( value_type & obj )
    : ptr_{&obj}
  {}

  ATOMIC_REF_FORCEINLINE
  atomic_ref( const atomic_ref & ref )
    : ptr_{ref.ptr_}
  {}

  ATOMIC_REF_FORCEINLINE
  value_type operator=( value_type desired ) const noexcept
  {
    store(desired);
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  operator value_type() const noexcept
  {
    return load();
  }

  ATOMIC_REF_FORCEINLINE
  bool is_lock_free() const noexcept
  {
    return __atomic_is_lock_free( sizeof(value_type), ptr_ );
  }

  ATOMIC_REF_FORCEINLINE
  void store( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    __atomic_store_n( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_load_n( ptr_, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type exchange( value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_exchange_n( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( value_type& expected, value_type desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_add( difference_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return val >= 0
         ? __atomic_fetch_add( ptr_, static_cast<uintptr_t>(val*stride), static_cast<int>(order) )
         : __atomic_fetch_sub( ptr_, static_cast<uintptr_t>(-val*stride), static_cast<int>(order) )
         ;

  }

  ATOMIC_REF_FORCEINLINE
  value_type fetch_sub( difference_type val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return val >= 0
         ? __atomic_fetch_sub( ptr_, static_cast<uintptr_t>(val*stride), static_cast<int>(order) )
         : __atomic_fetch_add( ptr_, static_cast<uintptr_t>(-val*stride), static_cast<int>(order) )
         ;
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator++() const noexcept
  {
    return __atomic_fetch_add( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator++(int) const noexcept
  {
    return __atomic_add_fetch( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator--() const noexcept
  {
    return __atomic_fetch_sub( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator--(int) const noexcept
  {
    return __atomic_sub_fetch( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator+=( difference_type val ) const noexcept
  {
    return val >= 0
         ? __atomic_fetch_add( ptr_, static_cast<uintptr_t>(val*stride), __ATOMIC_SEQ_CST )
         : __atomic_fetch_sub( ptr_, static_cast<uintptr_t>(-val*stride), __ATOMIC_SEQ_CST )
         ;
  }

  ATOMIC_REF_FORCEINLINE
  value_type operator-=( difference_type val ) const noexcept
  {
    return val >= 0
         ? __atomic_fetch_sub( ptr_, static_cast<uintptr_t>(val*stride), __ATOMIC_SEQ_CST )
         : __atomic_fetch_add( ptr_, static_cast<uintptr_t>(-val*stride), __ATOMIC_SEQ_CST )
         ;
  }
};


} // inline namespace is_20
} // namespace std


#endif // ATOMIC_REF_HPP
