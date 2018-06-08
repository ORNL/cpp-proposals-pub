#include <cstdio>
#include <complex>
#include <array>

#include "atomic_ref.hpp"

// may need to link with -latomic

namespace {

template <class T>
int test_generic(T v1, T v2)
{
  using atomic_ref = Foo::atomic_ref<T>;

  int errors = 0;

  T val;

  atomic_ref ref{val};

  ref = v1;
  if ( v1 != static_cast<T>(ref) ) {
    ++errors;
  }

  ref.store( v2 );
  if ( v2 != ref.load() ) {
    ++errors;
  }

  ref.store( v1, std::memory_order_relaxed );
  if ( v1 != ref.load( std::memory_order_relaxed ) ) {
    ++errors;
  }

  if( v1 != ref.exchange(v2) ) {
    ++errors;
  }

  if( v2 != ref.exchange(v1, std::memory_order_acq_rel) ) {
    ++errors;
  }

  T expected = v2;
  if( ref.compare_exchange_weak(expected, v1) ) {
    ++errors;
  }
  if( !ref.compare_exchange_strong(expected, v1) ) {
    ++errors;
  }

  expected = v2;
  if( ref.compare_exchange_weak(expected, v1, std::memory_order_acq_rel, std::memory_order_relaxed) ) {
    ++errors;
  }
  if( !ref.compare_exchange_strong(expected, v1, std::memory_order_acq_rel, std::memory_order_relaxed) ) {
    ++errors;
  }

  return errors;
}

template <class T>
int test_integral(T a, T b)
{
  int errors = 0;

  using atomic_ref = Foo::atomic_ref<T>;

  atomic_ref ref(a);

  ref += b;
  ref -= b;

  ref |= b;
  ref &= b;
  ref ^= b;

  ref.fetch_add(b, std::memory_order_relaxed );
  ref.fetch_sub(b, std::memory_order_relaxed );
  ref.fetch_and(b, std::memory_order_relaxed );
  ref.fetch_or(b, std::memory_order_relaxed );
  ref.fetch_xor(b, std::memory_order_relaxed );

  return errors;
}

template <class T>
int test_float(T a, T b)
{
  int errors = 0;

  using atomic_ref = Foo::atomic_ref<T>;

  atomic_ref ref(a);

  ref += b;
  ref -= b;

  ref.fetch_add(b, std::memory_order_relaxed );
  ref.fetch_sub(b, std::memory_order_relaxed );

  ref.fetch_add( std::nan("") );

  errors += std::isnan( ref.load() ) ? 0 : 1;

  return errors;
}

template <class T>
int test_pointer(T a, uintptr_t b)
{
  int errors = 0;

  using atomic_ref = Foo::atomic_ref<T>;

  atomic_ref ref(a);

  ref += b;
  ref -= b;

  ref.fetch_add(b, std::memory_order_relaxed );
  ref.fetch_sub(b, std::memory_order_relaxed );

  return errors;
}

} // namespace

int main()
{
  int num_errors = 0;

  {
    char a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "char", Foo::atomic_ref<char>::is_always_lock_free);
  }

  {
    signed char a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "signed char", Foo::atomic_ref<signed char>::is_always_lock_free);
  }

  {
    unsigned char a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "unsigned char", Foo::atomic_ref<unsigned char>::is_always_lock_free);
  }

  {
    short a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "short", Foo::atomic_ref<short>::is_always_lock_free);
  }

  {
    unsigned short a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "unsigned short", Foo::atomic_ref<unsigned short>::is_always_lock_free);
  }

  {
    int a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "int", Foo::atomic_ref<int>::is_always_lock_free);
  }

  {
    unsigned a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "unsigned int", Foo::atomic_ref<unsigned int>::is_always_lock_free);
  }

  {
    long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "long", Foo::atomic_ref<long>::is_always_lock_free);
  }

  {
    unsigned long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "unsigned long", Foo::atomic_ref<unsigned long>::is_always_lock_free);
  }

  {
    long long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "long long", Foo::atomic_ref<long long>::is_always_lock_free);
  }

  {
    unsigned long long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "unsigned long long", Foo::atomic_ref<unsigned long long>::is_always_lock_free);
  }

  {
    __int128_t a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "__int128_t", Foo::atomic_ref<__int128_t>::is_always_lock_free);
  }

  {
    __uint128_t a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
    printf("lockfree[%s]: %d\n", "__uint128_t", Foo::atomic_ref<__uint128_t>::is_always_lock_free);
  }

  {
    float a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_float( a ,b );
    printf("lockfree[%s]: %d\n", "float", Foo::atomic_ref<float>::is_always_lock_free);
  }

  {
    double a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_float( a ,b );
    printf("lockfree[%s]: %d\n", "double", Foo::atomic_ref<double>::is_always_lock_free);
  }

  {
    int a = 1, b = 64;
    int * a_ptr = &a;
    int * b_ptr = &b;
    num_errors += test_generic( a_ptr , b_ptr );
    num_errors += test_pointer( a, 10 );
    printf("lockfree[%s]: %d\n", "int*", Foo::atomic_ref<int*>::is_always_lock_free);
  }

  {
    std::complex<double> a{32,64}, b{128, 256};

    num_errors += test_generic( a ,b );
    printf("lockfree[%s]: %d\n", "complex<double>", Foo::atomic_ref<std::complex<double>>::is_always_lock_free);
  }
  
  {
    std::array<char,3> a{{(char)32,(char)64,(char)128}}, b{{(char)512,(char)1024,(char)2048}};

    num_errors += test_generic( a ,b );
    printf("lockfree[%s]: %d\n", "array<int,3>", Foo::atomic_ref<std::array<int,3>>::is_always_lock_free);
  }

  {
    std::array<int,3> a{{32,64,128}}, b{{512, 1024, 2048}};

    num_errors += test_generic( a ,b );
    printf("lockfree[%s]: %d\n", "array<int,3>", Foo::atomic_ref<std::array<int,3>>::is_always_lock_free);
  }

  {
    std::array<double,4> a{{32,64,128,256}}, b{{512, 1024, 2048, 4096}};

    num_errors += test_generic( a ,b );
    printf("lockfree[%s]: %d\n", "array<int,4>", Foo::atomic_ref<std::array<int,4>>::is_always_lock_free);
  }

  if (num_errors > 0) {
    printf("FAIL: num errors = = %d\n", num_errors );
  }
  else {
    printf("SUCCESS\n");
  }

  return 0;
}


