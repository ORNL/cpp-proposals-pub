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
  }

  {
    signed char a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    unsigned char a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    short a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    unsigned short a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    int a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    unsigned a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    unsigned long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    long long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    unsigned long long a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    __int128 a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    unsigned __int128 a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_integral( a ,b );
  }

  {
    float a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_float( a ,b );
  }

  {
    double a = 1, b = 64;
    num_errors += test_generic( a ,b );
    num_errors += test_float( a ,b );
  }

  {
    int a = 1, b = 64;
    int * a_ptr = &a;
    int * b_ptr = &b;
    num_errors += test_generic( a_ptr , b_ptr );
    num_errors += test_pointer( a, 10 );
  }

  {
    std::complex<double> a{32,64}, b{128, 256};

    num_errors += test_generic( a ,b );
  }

  {
    std::array<double,4> a{{32,64,128,256}}, b{{512, 1024, 2048, 4096}};

    num_errors += test_generic( a ,b );
  }

  if (num_errors > 0) {
    printf("FAIL: num errors = = %d\n", num_errors );
  }
  else {
    printf("SUCCESS\n");
  }

  return 0;
}


