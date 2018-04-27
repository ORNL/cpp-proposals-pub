set(async_ptr_HEADERS
  experimental/detail/future.impl.hpp
)

install(FILES ${async_ptr_HEADERS} DESTINATION include)

add_subdirectory(experimental)

