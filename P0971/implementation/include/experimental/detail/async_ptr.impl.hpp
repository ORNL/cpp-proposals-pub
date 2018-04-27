
#ifndef ASYNC_PTR_EXPERIMENTAL_DETAIL_ASYNC_PTR_IMPL_HPP
#define ASYNC_PTR_EXPERIMENTAL_DETAIL_ASYNC_PTR_IMPL_HPP

#include <experimental/future>
#include <experimental/async_ptr>

#include <memory>
#include <iostream>

namespace std {
namespace experimental {

void detail::_async_ptr_control_base::_throw_if_cannot_read(const char* where) {
  if(not this->state_.can_read) {
    read_promise_->set_exception(std::make_exception_ptr(
      bad_access_of_borrowed_value(where)
    ));
    // For now:
    std::cerr << "Invalid async_ptr or const_async_ptr read via " << where << std::endl;
    std::abort();
  }
}

void detail::_async_ptr_control_base::_throw_if_cannot_write(const char* where) {
  if(not this->state_.can_write) {
    write_promise_->set_exception(std::make_exception_ptr(
      bad_access_of_borrowed_value(where)
    ));
    // For now:
    std::cerr << "invalid async_ptr or const_async_ptr write via " << where << std::endl;
    std::abort();
  }
}

std::shared_ptr<promise<void>>
detail::_async_ptr_control_base::_make_chained_promise_ptr(
  shared_ptr<promise<void>> chained_from
) {
  // make a write promise that also holds our write promise (and releases
  // both our promise and theirs when it's destroyed)
  return std::shared_ptr<promise<void>>(
    new promise<void>(),
      [chained_from](auto make_ready_then_delete) mutable {
        try {
          make_ready_then_delete->set_value();
        }
        catch (...) {
          // An exception was already set on this; propagate it
          // TODO figure out how to get the exception
          chained_from->set_exception(std::make_exception_ptr(
            bad_access_of_borrowed_value("unknown")
          ));
        }
        // release our copy of chained_from from enclosing scope
        chained_from = nullptr;
        delete make_ready_then_delete;
      }
  );
}

template <typename T>
template <typename Callable, typename DerefBool, typename ConstBool>
void
async_ptr<T>::_do_borrow_async(Callable&& callable, DerefBool, ConstBool) {
  // Do the borrow
  if constexpr (not ConstBool::value) {
    auto this_borrowed = _make_borrowed_ptr();
    auto wait_on_future = make_ready_future().share();
    auto wait_on_read_future = make_ready_future().share();
    // Swap it with the write future so that it will be ready when the callable starts
    std::swap(wait_on_future, this_borrowed.write_future_);
    std::swap(wait_on_read_future, this_borrowed.read_future_);
    // when everyone who says "can't read" and "can't write" is done, run the continuation
    auto done_future = when_all(wait_on_future, wait_on_read_future).then([
      callable=std::forward<Callable>(callable), this_borrowed=std::move(this_borrowed)
    ](auto&&... /*ignored*/) mutable {
      if constexpr(DerefBool::value) {
        std::move(callable)(this_borrowed.get_value());
      }
      else {
        std::move(callable)(std::move(this_borrowed));
      }
    });
  }
  else { // ConstBool::value == true
    // Do the borrow
    auto this_const_borrowed = _make_const_borrowed_ptr();
    // make the new future to be swapped with the one we're supposed to wait on
    auto wait_on_future = make_ready_future().share();
    // Swap it with the write future so that it will be ready when the callable starts
    std::swap(wait_on_future, this_const_borrowed.read_future_);
    // when everyone who says "can't read" is done, run the callable
    auto done_future = wait_on_future.then([
      callable=std::forward<Callable>(callable), this_const_borrowed=std::move(this_const_borrowed)
    ](auto&&... /*ignored*/) mutable {
      if constexpr(DerefBool::value) {
        std::move(callable)(this_const_borrowed.get_value());
      }
      else {
        std::move(callable)(std::move(this_const_borrowed));
      }
    });
  }
};


template <typename T>
async_ptr<T>::async_ptr()
  : _async_ptr_control_base(),
    data_(std::make_shared<T>())
{ }


template <typename T>
template <typename Callable>
void
async_ptr<T>::borrow_async(Callable&& callable) {
  _do_borrow_async(std::forward<Callable>(callable),
    /*deref=*/ std::false_type{}, /*const=*/ std::false_type{}
  );
}

template <typename T>
template <typename Callable>
void
async_ptr<T>::borrow_value_async(Callable&& callable) {
  _do_borrow_async(std::forward<Callable>(callable),
    /*deref=*/ std::true_type{}, /*const=*/ std::false_type{}
  );
}

template <typename T>
template <typename Callable>
void
async_ptr<T>::const_borrow_async(Callable&& callable) {
  _do_borrow_async(std::forward<Callable>(callable),
    /*deref=*/ std::false_type{}, /*const=*/ std::true_type{}
  );
}

template <typename T>
template <typename Callable>
void
async_ptr<T>::const_borrow_value_async(Callable&& callable) {
  _do_borrow_async(std::forward<Callable>(callable),
    /*deref=*/ std::false_type{}, /*const=*/ std::true_type{}
  );
}

template <typename T>
async_ptr<T>::~async_ptr() {
  if(state_.wait_on_destruction) {
    // need to do waiting here so that destruction of T happens after waiting
    if(read_future_.valid()) read_future_.get();
    if(write_future_.valid()) write_future_.get();
  }
}

template <typename T>
async_ptr<T>&
async_ptr<T>::operator=(nullptr_t) {
  data_ = nullptr;
  _nullify();
  return *this;
}

template <typename T>
template <typename Callable, typename DerefBool>
void
const_async_ptr<T>::_do_borrow_async(Callable&& callable, DerefBool) {
  // Do the borrow
  auto this_const_borrowed = _make_const_borrowed_ptr();
  // make the new future to be swapped with the one we're supposed to wait on
  auto wait_on_future = make_ready_future().share();
  // Swap it with the write future so that it will be ready when the callable starts
  std::swap(wait_on_future, this_const_borrowed.read_future_);
  // when everyone who says "can't read" is done, run the callable
  auto done_future = wait_on_future.then([
    callable=std::forward<Callable>(callable), this_borrowed=std::move(this_const_borrowed)
  ](auto&&... /*ignored*/) mutable {
    if constexpr(DerefBool::value) {
      std::move(callable)(this_const_borrowed.get_const_value());
    }
    else {
      std::move(callable)(std::move(this_const_borrowed));
    }
  });
}

template <typename T>
const_async_ptr<T>&
const_async_ptr<T>::operator=(nullptr_t) {
  data_ = nullptr;
  _nullify();
  return *this;
}

template <typename T>
template <typename Callable>
void
const_async_ptr<T>::const_borrow_async(Callable&& callable) {
  _do_borrow_async(std::forward<Callable>(callable), std::false_type{});
}

template <typename T>
template <typename Callable>
void
const_async_ptr<T>::const_borrow_value_async(Callable&& callable) {
  _do_borrow_async(std::forward<Callable>(callable), std::true_type{});
};

template <typename T>
const_async_ptr<T>::~const_async_ptr() {
  if(state_.wait_on_destruction) {
    // need to do waiting here so that destruction of T happens after waiting
    if(read_future_.valid()) read_future_.get();
    if(write_future_.valid()) write_future_.get();
  }
}

template <typename... AsyncPtrTs>
template <typename Callable, typename DerefBool, size_t... Idxs>
void
detail::with_all_result_base<AsyncPtrTs...>::_do_borrow_async(
  Callable&& callable, DerefBool, integer_sequence<size_t, Idxs...>
) {
  // Do the borrows
  auto do_borrow = [](auto& ptr) { return ptr._make_default_borrowed_ptr(); };
  auto these_borrowed = std::make_tuple(do_borrow(std::get<Idxs>(ptrs_))...);

  // swap in ready futures and return the wait_on_futures
  auto do_swap = [](auto& ptr_borrowed) {
    auto wait_on_read_future = make_ready_future().share();
    std::swap(wait_on_read_future, ptr_borrowed.read_future_);
    auto wait_on_future = make_ready_future().share();
    std::swap(wait_on_future, ptr_borrowed.write_future_);
    return std::make_pair(wait_on_read_future, wait_on_future);
  };
  auto wait_on_future_pairs = std::make_tuple(do_swap(std::get<Idxs>(these_borrowed))...);
  auto wait_on_futures = std::make_tuple(
    std::move(std::get<0>(std::get<Idxs>(wait_on_future_pairs)))...,
    std::move(std::get<1>(std::get<Idxs>(wait_on_future_pairs)))...
  );

  // when all of them have gotten write permissions, run the callable:
  auto when_all_wrapped = [](auto&&... fs) { return when_all(std::move(fs)...); };
  auto done_future = std::apply(when_all_wrapped, std::move(wait_on_futures)).then([
    callable=std::forward<Callable>(callable), these_borrowed=std::move(these_borrowed)
  ](auto&&... /*ignored*/) mutable {
    if constexpr (DerefBool::value) {
      // Deduction guide for tuple doesn't use decltype deduction, so we need
      // to do it manually here:
      using value_refs_tuple_t = tuple<decltype(std::get<Idxs>(these_borrowed)._get_value_with_minimum_constness())...>;
      auto value_refs_tuple = value_refs_tuple_t(std::get<Idxs>(these_borrowed)._get_value_with_minimum_constness()...);
      // No need to move; all entries in tuple should be references
      static_assert(conjunction_v<is_lvalue_reference<tuple_element_t<Idxs, decay_t<decltype(value_refs_tuple)>>>...>);
      std::apply(std::move(callable), value_refs_tuple);
    }
    else {
      std::apply(std::move(callable), std::move(these_borrowed));
    }
  });
}

namespace detail {

struct make_async_ptr_attorney {
  template <typename T, typename... Args>
  static inline constexpr auto _do_make_ptr(Args&&... args) {
    return async_ptr<T>(in_place, std::forward<Args>(args)...);
  }
};

} // end namespace detail

template <typename T, typename... Args>
auto
make_async_ptr(Args&&... args) {
  return detail::make_async_ptr_attorney::template _do_make_ptr<T>(std::forward<Args>(args)...);
}

} // end namespace experimental
} // end namespace std


#endif //ASYNC_PTR_EXPERIMENTAL_DETAIL_ASYNC_PTR_IMPL_HPP
