
#include <experimental/async_ptr>
#include <experimental/execution>
#include <experimental/thread_pool>

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

int main() {
  static_thread_pool pool(1);
  {
    auto ex = pool.executor();

    auto f = ex.twoway_execute([]() {
      return 42;
    });

    auto t = ex.then_execute([](future<int> g) {
      std::cout << g.get() << std::endl;
    }, std::move(f));


    //t.wait();

  }

  pool.wait();

  return 0;
}

