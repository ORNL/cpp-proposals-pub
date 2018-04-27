
#include <experimental/async_ptr>

#if defined(ASYNC_PTR_USE_HPX_FOR_CONCURRENCY_TS)
#include <hpx/hpx_main.hpp>
#endif

#include <memory>
#include <type_traits>
#include <iostream>
#include <utility>
#include <cstdlib>

#include <thread> // for sleeping; seems to work well enough with boost::thread

using namespace std::experimental;
using namespace std;

//// Same problem as exceptions, but with assertions
//void _strong_assert(char const* assertion, bool value, char const* file, int line) {
//  if(!value) {
//    fprintf(stderr, "%s:%d: Assertion failed: %s\n", file, line, assertion);
//    fflush(stderr);
//    abort();
//  }
//}
//#define strong_assert(expr) _strong_assert(#expr, expr, __FILE__, __LINE__);

// For debugging purposes
void sleep_for_milliseconds(size_t time) {
  // boost seems to be fine with this
  std::this_thread::sleep_for(std::chrono::milliseconds{time});
}




//==============================================================================
// main
//==============================================================================

//void foo_func(borrowed_async_ptr<int> data) {
//  data.borrow_async(...);
//}

int main() {

  async_ptr<int> foo;
  foo.set_value(42);
  auto bar = std::experimental::make_async_ptr<int>(73);

  //foo_func(foo.borrow());

  foo.borrow_async([](auto foo) {
    std::printf("setting foo to 123\n");
    foo.set_value(123);
    foo = nullptr; // no more work on foo
    sleep_for_milliseconds(500);
  });

  bar.borrow_async([](auto bar) {
    std::printf("setting bar to 456\n");
    bar.set_value(456);

    bar.const_borrow_async([](auto bar) {
      sleep_for_milliseconds(1000);
      std::printf("bar is %d\n", bar.get_const_value());
    });

    std::printf("in continuation, bar is %d\n", bar.get_const_value());

    // Runtime error:
    // bar.set_value(1234);

  });

  // Runtime error:
  // foo.set_value(1234);

  with_all(foo, as_const(bar)).borrow_async([](auto foo, auto bar) {
    std::printf("foo is %d, bar is %d\n", foo.get_value(), bar.get_const_value());
  });

  with_all_values(foo, bar).borrow_async([](auto& foo, auto& bar) {
    foo = 1;
    bar = 2;
  });

  with_all_values(as_const(foo), as_const(bar)).borrow_async([](auto const& foo, auto const& bar) {
    std::printf("foo is %d, bar is %d\n", foo, bar);
  });

  return 0;

}

//==============================================================================
// TODOs:
// - return an async_ptr<T> from a borrow_async?
// - reset()?
// - Allocator/Deleter propagation to shared_ptr member
// - exception propagation?
// - expanded member types?
// - operator overloads: comparison?
// - operator overloads: << ? (see shared ptr)
// - operator overloads: bool?
// - pointer cast non-member functions?
// - swap as member function?
// - finish-like closure (avoiding somewhat sketchy destructor semantics?)
// - executor support?
// - deferred construction version (i.e., created-as-borrowed)?
// - .get_future() ???
// Paper notes:
// - don't forget to discuss trade-offs between const_async_pointer as its own
//   type vs. as async_ptr<const T> specialization
//==============================================================================
