
#find_package(GTest REQUIRED)

add_executable(test_all
  test_main.cpp
  test_extents.cpp
  test_layouts.cpp
  test_mdspan.cpp
  test_subspan.cpp
  gtest/gtest-all.cc
)

target_include_directories(
  test_all
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(test_all mdspan)

