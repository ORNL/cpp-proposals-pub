
// Pulls std::experimental::future-like behavior (in terms of Concurrency TS v1)
// from boost::thread

#ifndef ASYNC_PTR_EXPERIMENTAL_FUTURE_IMPL_HPP
#define ASYNC_PTR_EXPERIMENTAL_FUTURE_IMPL_HPP

#ifndef ASYNC_PTR_USE_HPX_FOR_CONCURRENCY_TS
#  define ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS 1
#endif

#ifdef ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_BARRIER
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#define BOOST_THREAD_PROVIDES_EXECUTORS
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>

namespace std {
namespace experimental {

namespace future_provider = boost;
template <typename T> using promise = future_provider::promise<T>;
template <typename T> using future = future_provider::future<T>;
template <typename T> using shared_future = future_provider::shared_future<T>;

auto make_ready_future() { 
  return future_provider::make_ready_future(); 
}

template <typename T=void, typename... Args>
auto make_ready_future(Args&&... args) { 
  return future_provider::make_ready_future<T>(std::forward<Args>(args)...); 
}

template <typename T, typename Exception>
future<T> make_exceptional_future(Exception&& ex) {
  return future_provider::make_exceptional_future<T>(std::forward<Exception>(ex));
}

using future_error = boost::future_error;

} // end namespace experimental
} // end namespace std

#elif defined(ASYNC_PTR_USE_HPX_FOR_CONCURRENCY_TS)

#include <hpx/include/lcos.hpp>
#include <hpx/lcos/when_all.hpp>
#include <future>

namespace std {
namespace experimental {

namespace future_provider = hpx;
template <typename T> using promise = future_provider::promise<T>;
template <typename T> using shared_future = future_provider::shared_future<T>;

auto make_ready_future() { 
  return future_provider::make_ready_future(); 
}

template <typename T=void, typename... Args>
auto make_ready_future(Args&&... args) { 
  return future_provider::make_ready_future<T>(std::forward<Args>(args)...); 
}

using future_error = std::future_error;

} // end namespace experimental
} // end namespace std

#endif // ASYNC_PTR_USE_HPX_FOR_CONCURRENCY_TS


#endif // ASYNC_PTR_EXPERIMENTAL_FUTURE_IMPL_HPP