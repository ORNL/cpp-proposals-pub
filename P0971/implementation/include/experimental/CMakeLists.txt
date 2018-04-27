set(async_ptr_experimental_HEADERS
  future
  async_ptr
  execution
  thread_pool
)

install(FILES ${async_ptr_detail_HEADERS} DESTINATION include/experimental)
