#ifndef ASYNC_PTR_EXPERIMENTAL_DETAIL_ASYNC_PTR_BASE_HPP
#define ASYNC_PTR_EXPERIMENTAL_DETAIL_ASYNC_PTR_BASE_HPP

#include <experimental/future>
#include <experimental/type_traits>

#include <memory>

namespace std {
namespace experimental {

// Forward declarations

template <typename T>
struct async_ptr;

template <typename T>
struct const_async_ptr;

template <typename... Ptrs>
struct with_all_result;

template <typename... Ptrs>
struct with_all_values_result;

// EXPERIMENTAL
class async_ptr_context;

namespace detail {

// Forward declaration
template <typename T>
struct as_const_wrapper;

struct _async_ptr_control_base {
  public:

    shared_ptr<promise<void>> write_promise_;
    shared_future<void> write_future_;
    shared_ptr<promise<void>> read_promise_;
    shared_future<void> read_future_;

    // TODO handle case of move after destruction of context???
    // EXPERIMENTAL
    async_ptr_context* context_ = nullptr;

    struct async_ptr_state {
      // this can be a bitfield in 20; don't feel like avoiding initializers until then
      bool can_read /*: 1*/ = true;
      bool can_write /*: 1*/ = true;
      bool wait_on_destruction /*: 1*/ = true;
    } state_ = { };

    _async_ptr_control_base()
      : write_future_(std::experimental::make_ready_future().share()),
        write_promise_(nullptr),
        read_promise_(nullptr),
        read_future_(std::experimental::make_ready_future().share())
    { }

    _async_ptr_control_base(std::nullptr_t) noexcept
      : write_future_(),
        write_promise_(nullptr),
        read_future_(),
        read_promise_(nullptr),
        state_{false, false, false}
    { }

    _async_ptr_control_base(_async_ptr_control_base&& other) = default;

    //  : write_future_(std::move(other.write_future_)),
    //    write_promise_(std::move(other.write_promise_)),
    //    read_future_(std::move(other.read_future_)),
    //    read_promise_(std::move(other.read_promise_)),
    //    context_(std::move(other.context_)),
    //    state_(std::move(other.state_))
    //{
    //  other.context_ = nullptr;
    //  if(context_) {
    //    std::lock_guard<std::mutex> lg(context_->wait_on_mtx_);
    //    size_t erased = context_->to_wait_on_.erase(&other);
    //    assert(erased == 1);
    //    context_->to_wait_on_.insert(this);
    //  }
    //}

    shared_ptr<promise<void>>
    _make_chained_promise_ptr(shared_ptr<promise<void>> chained_from);

    void _throw_if_cannot_read(const char*);

    void _throw_if_cannot_write(const char*);

    void _nullify() {
      write_promise_ = nullptr;
      read_promise_ = nullptr;
      state_.can_read = false;
      state_.can_write = false;
      if(state_.wait_on_destruction) {
        read_future_.get();
        write_future_.get();
      }
    }

    auto _make_chained_future(shared_future<void> const& f1, shared_future<void> const& f2) {
      // not very efficient, but we'll have better ways to do this in the future
      // I guess...
      return f1.then([f2](auto&&... /*ignored*/){ f2.wait(); });
    }

    auto _do_const_borrow(_async_ptr_control_base& rv) {
      // If this holds a write promise to borrowed-from pointer in its outer
      // scope, the borrowed pointer also needs to hold that promise
      rv.write_promise_ = _make_chained_promise_ptr(write_promise_);
      // We should still wait on anyone who says we can't read:
      rv.read_future_ = read_future_;
      // *but* the read promise should remain null, since reads are non-exlusive
      // also we shouldn't wait for write_future_ since we don't need to and
      // consider a promise not to write (and doing so would cause us to be
      // unable to run concurrently with other reads)

      // Pointers borrowed from the continuation should not do any writes until
      // the promise from the reading borrower is fullfilled.
      // Anything after us should also wait on anything that says rv can't write
      write_future_ = _make_chained_future(write_future_, rv.write_promise_->get_future().share());
      // `this` still holds the ability to create write tasks, so
      // this.write_promise_ is unchanged.
      // a const pointer only says "can't write" to pointers borrowed from the
      // continuation, so we don't update the
      // `rv.read_promise_`/`this->read_future_` pair that says "can't read"

      // update the state
      // rv.state_.can_read_value defaults to true
      rv.state_.can_write = false;
      // All of the waiting will be handled by this (or an ancestor of this), so
      // no need for the borrowed ptr to wait when it is destroyed
      rv.state_.wait_on_destruction = false;

      state_.can_write = false;
      // state_.can_read_value is unchanged
      // state_.wait_on_destruction is unchanged
    }

};

template <typename Ptr>
struct _is_valid_variadic_with_all_parameter {
  static constexpr auto value = false;
 };

template <typename T>
struct _is_valid_variadic_with_all_parameter<async_ptr<T>&> {
  static constexpr auto value = true;
};

template <typename T>
struct _is_valid_variadic_with_all_parameter<const_async_ptr<T>&> {
  static constexpr auto value = true;
};

template <typename T>
struct _is_valid_variadic_with_all_parameter<detail::as_const_wrapper<T>&&> {
  static constexpr auto value = true;
};

template <typename... Ptrs>
struct _with_all_variadic_enable_if_return
  : std::enable_if<std::conjunction_v<
      _is_valid_variadic_with_all_parameter<Ptrs>...
    >, with_all_result<Ptrs...>>
{ };

template <typename... Ptrs>
struct _with_all_values_variadic_enable_if_return
  : std::enable_if<std::conjunction_v<
      _is_valid_variadic_with_all_parameter<Ptrs>...
    >, with_all_values_result<Ptrs...>>
{ };

template <typename... Ptrs>
using _with_all_variadic_enable_if_return_t = typename _with_all_variadic_enable_if_return<Ptrs...>::type;

template <typename... Ptrs>
using _with_all_values_variadic_enable_if_return_t = typename _with_all_values_variadic_enable_if_return<Ptrs...>::type;

struct _nat { };

template <typename TT, typename Del>
using _deleter_archetype = decltype(std::declval<Del>()(std::declval<TT*>()));
template <typename T, typename Y, typename Deleter>
inline static constexpr auto _deleter_ctor_condition_v = is_convertible_v<Y*, T*>
  and is_detected_v<_deleter_archetype, T, Deleter>
  and is_move_constructible_v<Deleter>;


} // end namespace detail
} // end namespace experimental
} // end namespace std

#endif //ASYNC_PTR_EXPERIMENTAL_DETAIL_ASYNC_PTR_BASE_HPP
