#ifndef ATOMIC_REF_HPP
#define ATOMIC_REF_HPP

#include <atomic>
#include <type_traits>
#include <cstdint>

#if defined( ATOMIC_REF_ENABLE_FORCEINLINE )
#define ATOMIC_REF_FORCEINLINE __attribute__((always_inline))
#else
#define ATOMIC_REF_FORCEINLINE inline
#endif

namespace std { namespace experimental {

template < typename T, typename Enable=void >
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
  explicit atomic_ref( T & obj )
    : ptr_{&obj}
  {}

  ATOMIC_REF_FORCEINLINE
  atomic_ref( const atomic_ref & ref )
    : ptr_{ref.ptr_}
  {}

  ATOMIC_REF_FORCEINLINE
  T operator=( T desired ) const noexcept
  {
    store(desired);
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  operator T() const noexcept
  {
    return load();
  }

  ATOMIC_REF_FORCEINLINE
  bool is_lock_free() const noexcept
  {
    return __atomic_is_lock_free( sizeof(T), ptr_ );
  }

  ATOMIC_REF_FORCEINLINE
  void store( T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    __atomic_store( ptr_, &desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    T result;
    __atomic_load( ptr_, &result, static_cast<int>(order) );
    return result;
  }

  ATOMIC_REF_FORCEINLINE
  T exchange( T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    T result;
    __atomic_exchange( ptr_, &desired, &result, static_cast<int>(order) );
    return result;
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( T& expected, T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, true, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( T& expected, T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, false, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( T& expected, T desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, true, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( T& expected, T desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange( ptr_, &expected, &desired, false, static_cast<int>(success), static_cast<int>(failure) );
  }
};




template < typename T>
struct atomic_ref< T, typename std::enable_if< std::is_integral<T>::value, void>::type >
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
  explicit atomic_ref( T & obj )
    : ptr_{&obj}
  {}

  ATOMIC_REF_FORCEINLINE
  atomic_ref( const atomic_ref & ref )
    : ptr_{ref.ptr_}
  {}

  ATOMIC_REF_FORCEINLINE
  T operator=( T desired ) const noexcept
  {
    store(desired);
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  operator T() const noexcept
  {
    return load();
  }

  ATOMIC_REF_FORCEINLINE
  bool is_lock_free() const noexcept
  {
    return __atomic_is_lock_free( sizeof(T), ptr_ );
  }

  ATOMIC_REF_FORCEINLINE
  void store( T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    __atomic_store_n( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_load_n( ptr_, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T exchange( T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_exchange( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( T& expected, T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( T& expected, T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( T& expected, T desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( T& expected, T desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  T fetch_add( T val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_add( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T fetch_sub( T val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_sub( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T fetch_and( T val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_and( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T fetch_or( T val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_or( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T fetch_xor( T val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_xor( ptr_, val, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T operator++() const noexcept
  {
    return __atomic_fetch_add( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator++(int) const noexcept
  {
    return __atomic_add_fetch( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator--() const noexcept
  {
    return __atomic_fetch_sub( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator--(int) const noexcept
  {
    return __atomic_sub_fetch( ptr_, 1, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator+=( T val ) const noexcept
  {
    return __atomic_add_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator-=( T val ) const noexcept
  {
    return __atomic_sub_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator&=( T val ) const noexcept
  {
    return __atomic_and_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator|=( T val ) const noexcept
  {
    return __atomic_or_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator^=( T val ) const noexcept
  {
    return __atomic_xor_fetch( ptr_, val, __ATOMIC_SEQ_CST);
  }
};




template < typename T>
struct atomic_ref< T, typename std::enable_if< std::is_pointer<T>::value, void>::type >
{
private:
  T * ptr_;

  static_assert( std::is_trivially_copyable<T>::value
               , "Error: atomic_ref<T> requires a trivially copyable type" );

  static constexpr uintptr_t stride = static_cast<uintptr_t>(sizeof(T));

public:
  using value_type = T;
  static constexpr bool   is_always_lock_free = __atomic_always_lock_free( sizeof(T), 0);
  static constexpr size_t required_alignment  = std::alignment_of<T>::value;

  atomic_ref() = delete;
  atomic_ref & operator=( const atomic_ref & ) = delete;

  ATOMIC_REF_FORCEINLINE
  explicit atomic_ref( T & obj )
    : ptr_{&obj}
  {}

  ATOMIC_REF_FORCEINLINE
  atomic_ref( const atomic_ref & ref )
    : ptr_{ref.ptr_}
  {}

  ATOMIC_REF_FORCEINLINE
  T operator=( T desired ) const noexcept
  {
    store(desired);
    return desired;
  }

  ATOMIC_REF_FORCEINLINE
  operator T() const noexcept
  {
    return load();
  }

  ATOMIC_REF_FORCEINLINE
  bool is_lock_free() const noexcept
  {
    return __atomic_is_lock_free( sizeof(T), ptr_ );
  }

  ATOMIC_REF_FORCEINLINE
  void store( T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    __atomic_store_n( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T load( std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_load_n( ptr_, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T exchange( T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_exchange( ptr_, desired, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( T& expected, T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( T& expected, T desired, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(order), static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_weak( T& expected, T desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, true, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  bool compare_exchange_strong( T& expected, T desired, std::memory_order success, std::memory_order failure ) const noexcept
  {
    return __atomic_compare_exchange_n( ptr_, &expected, desired, false, static_cast<int>(success), static_cast<int>(failure) );
  }

  ATOMIC_REF_FORCEINLINE
  T fetch_add( uintptr_t val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_add( ptr_, val*stride, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T fetch_sub( uintptr_t val, std::memory_order order = std::memory_order_seq_cst ) const noexcept
  {
    return __atomic_fetch_sub( ptr_, val*stride, static_cast<int>(order) );
  }

  ATOMIC_REF_FORCEINLINE
  T operator++() const noexcept
  {
    return __atomic_fetch_add( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator++(int) const noexcept
  {
    return __atomic_add_fetch( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator--() const noexcept
  {
    return __atomic_fetch_sub( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator--(int) const noexcept
  {
    return __atomic_sub_fetch( ptr_, stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator+=( uintptr_t val ) const noexcept
  {
    return __atomic_add_fetch( ptr_, val*stride, __ATOMIC_SEQ_CST);
  }

  ATOMIC_REF_FORCEINLINE
  T operator-=( uintptr_t val ) const noexcept
  {
    return __atomic_sub_fetch( ptr_, val*stride, __ATOMIC_SEQ_CST);
  }
};

}} // namespace std::experimental


#endif // ATOMIC_REF_HPP
