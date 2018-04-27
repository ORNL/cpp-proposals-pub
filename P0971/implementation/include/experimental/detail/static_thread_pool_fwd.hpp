
#ifndef ASYNC_PTR_EXPERIMENTAL_DETAIL_STATIC_THREAD_POOL_FWD_HPP
#define ASYNC_PTR_EXPERIMENTAL_DETAIL_STATIC_THREAD_POOL_FWD_HPP

#ifndef ASYNC_PTR_USE_HPX_FOR_CONCURRENCY_TS
#  define ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS 1
#endif

namespace std {
namespace experimental {
inline namespace executors_v1 {

namespace detail {
class _static_thread_pool_executor;
} // namespace detail

} // inline namespace executors_v1
} // namespace experimental
} // namespace std

#ifdef ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS

#include <experimental/thread_pool>
#include <experimental/execution>

#include <boost/thread/executor.hpp>
#include <boost/thread/executors/thread_executor.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>

namespace std {
namespace experimental {
inline namespace executors_v1 {
namespace detail {

using _thread_pool_t = boost::basic_thread_pool;

} // namespace detail
} // inline namespace executors_v1
} // namespace experimental
} // namespace std

#endif // ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS

#endif // ASYNC_PTR_EXPERIMENTAL_DETAIL_STATIC_THREAD_POOL_FWD_HPP