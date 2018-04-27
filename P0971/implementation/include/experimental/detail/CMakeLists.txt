set(async_ptr_experimental_detail_HEADERS
  future.impl.hpp
  async_ptr.impl.hpp
  static_thread_pool.impl.hpp
  async_ptr_control_base.hpp
)

install(FILES ${async_ptr_experimental_detail_HEADERS} DESTINATION include/detail)
