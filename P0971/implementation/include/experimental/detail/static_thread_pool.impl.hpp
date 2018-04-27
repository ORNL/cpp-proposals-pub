
#ifndef ASYNC_PTR_EXPERIMENTAL_DETAIL_STATIC_THREAD_POOL_IMPL_HPP
#define ASYNC_PTR_EXPERIMENTAL_DETAIL_STATIC_THREAD_POOL_IMPL_HPP

#include <experimental/thread_pool>
#include <experimental/execution>

#ifndef ASYNC_PTR_USE_HPX_FOR_CONCURRENCY_TS
#  define ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS 1
#endif

#ifdef ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS

#include <experimental/future>

#include <boost/thread/executor.hpp>
#include "static_thread_pool_fwd.hpp"

namespace std {
namespace experimental {
inline namespace executors_v1 {

namespace detail {
class _static_thread_pool_executor {
  private:
    static_thread_pool* context_;
    std::shared_ptr<promise<void>> executors_done_promise_;

    explicit
    _static_thread_pool_executor(static_thread_pool* context)
      : context_(context),
        executors_done_promise_(context->executors_done_promise_)
    { }

    friend class experimental::executors_v1::static_thread_pool;

  public:
    template<class Function>
    std::experimental::future<invoke_result_t<decay_t<Function>>>
    twoway_execute(Function&& f) const {
      auto* pool = context_->_get_valid_impl_ptr();
      if(pool) {
        return boost::async(*pool, std::forward<Function>(f));
      }
      else {
        return std::experimental::make_exceptional_future<
          result_of_t<decay_t<Function>()>
        >(std::runtime_error("twoway_execute on stopped pool!"));
      }
    }

    template<class Function, class Future>
    std::experimental::future<result_of_t<decay_t<Function>(std::decay_t<Future>)>>
    then_execute(Function&& f, Future&& pred) const {
      auto* pool = context_->_get_valid_impl_ptr();
      if(pool) {
        return std::forward<Future>(pred).then(
          *pool, std::forward<Function>(f)
        );
      }
      else {
        return std::experimental::make_exceptional_future<
          result_of_t<decay_t<Function>(decay_t<Future>)>
        >(std::runtime_error{"then_execute on stopped pool!"});
      }
    };

    auto require(execution::oneway_t) { return *this; }
    auto require(execution::twoway_t) { return *this; }
    auto require(execution::then_t) { return *this; }
    auto require(execution::single_t) { return *this; }
    auto require(execution::possibly_blocking_t) { return *this; }
    auto require(execution::not_continuation_t) { return *this; }
    auto require(execution::not_outstanding_work_t) { return *this; }

    // TODO bulk operations?
    // auto require(execution::bulk_t) { return *this; }

};


} // namespace detail

static_thread_pool::static_thread_pool(size_t num_threads)
  : pool_impl_(std::make_optional<detail::_thread_pool_t>(num_threads)),
    executors_done_promise_(
      new promise<void>(),
      [](auto* to_del) {
        if(to_del) {
          try {
            to_del->set_value();
          }
          catch (std::experimental::future_error&) {
            // ??? What to do here?
          }
          delete to_del;
        }
      }
    ),
    n_threads_(num_threads)
{
  executors_done_trigger_ = executors_done_promise_->get_future();
}

void
static_thread_pool::attach() {
  assert(pool_impl_); // TODO throw
  bool did_execute = true;
  while(pool_impl_ and (!pool_impl_->closed() or did_execute)) {
    did_execute = pool->try_executing_one();
    pool = pool_impl_.get();
  }
}

//void
//static_thread_pool::stop() {
//  // note that it is stopped, so other work will be dropped on the floor(?)
//  stopped_.store(true, std::memory_order_release);
//  // swap in a null pool
//  detail::_thread_pool_t* expected = reinterpret_cast<detail::_thread_pool_t*>(reconstructing_pool_flag);
//  do {
//    // If a thread pool is being reconstructed, wait for it to be reconstructed
//    // before swapping in the nullptr
//    pool_impl_.compare_exchange_strong(
//      expected,
//      nullptr,
//      std::memory_order_acq_rel // this may not be right...
//    );
//  } while(intptr_t(expected) == reconstructing_pool_flag);
//
//  if(expected) {
//    stopped_pool_.store(expected, std::memory_order_release);
//  }
//
//}

void
static_thread_pool::wait() {
  if(executors_done_trigger_.valid()) {
    executors_done_trigger_.get();
  }
  pool_impl_->close();
  pool_impl_ = nullptr;

  // Don't allow any more submission to the underlying pool
  //detail::_thread_pool_t* expected = reinterpret_cast<detail::_thread_pool_t*>(reconstructing_pool_flag);
  //bool stored_null = false;
  //do {
  //  // If a thread pool is being reconstructed, wait for it to be reconstructed
  //  // before swapping in the nullptr
  //  stored_null = pool_impl_.compare_exchange_strong(
  //    expected,
  //    nullptr,
  //    std::memory_order_acq_rel // this may not be right...
  //  );
  //} while(intptr_t(expected) == reconstructing_pool_flag);

  //while(not stored_null and not expected) {
  //  stored_null = pool_impl_.compare_exchange_strong(
  //    expected, nullptr, std::memory_order_acq_rel
  //  );
  //}


  //if(expected) {
  //  // Wait on the work
  //  expected->close();

  //  // Delete the pool; it will be reconstructed if/when more work is submitted
  //  delete expected;
  //}
  //else {
  //  std::abort();
  //}

  //expected = nullptr;
  //while(not stopped_pool_.compare_exchange_strong(
  //  expected, nullptr,
  //  std::memory_order_acq_rel
  //));
  //if(expected) {
  //  expected->close();
  //  delete expected;
  //}

}

static_thread_pool::executor_type
static_thread_pool::executor() noexcept {
  return executor_type(this);
}

detail::_thread_pool_t*
static_thread_pool::_get_valid_impl_ptr() {
  return &*pool_impl_;
  //detail::_thread_pool_t* expected = reinterpret_cast<detail::_thread_pool_t*>(reconstructing_pool_flag);
  //do {
  //  pool_impl_.compare_exchange_strong(
  //    expected,
  //    nullptr,
  //    std::memory_order_acq_rel // this may not be right...
  //  );
  //} while(intptr_t(expected) == reconstructing_pool_flag);
  //if(expected == nullptr) {
  //  if(stopped_.load(std::memory_order_acquire)) {
  //    std::abort();
  //    return nullptr;
  //  }
  //  // we need to reconstruct and swap the value in
  //  auto* new_impl_ = new detail::_thread_pool_t(n_threads_);
  //  expected = reinterpret_cast<detail::_thread_pool_t*>(reconstructing_pool_flag);
  //  bool success = pool_impl_.compare_exchange_strong(
  //    expected,
  //    new_impl_,
  //    std::memory_order_release
  //  );
  //  // Should never fail
  //  if(!success) std::abort();
  //  return new_impl_;
  //}
  //else {
  //  // expected contains a living thread pool impl; return it
  //  return expected;
  //}
}

static_thread_pool::~static_thread_pool() {
  // No more work can be submitted after this get, because no handles
  // to do so exist
  //executors_done_promise_ = nullptr;
  //executors_done_trigger_.get();
  wait();
  //stopped_.store(true, std::memory_order_release);
}


} // inline namespace executors_v1
} // namespace experimental
} // namespace std

#endif // ASYNC_PTR_USE_BOOST_FOR_CONCURRENCY_TS

#endif // ASYNC_PTR_EXPERIMENTAL_DETAIL_STATIC_THREAD_POOL_IMPL_HPP