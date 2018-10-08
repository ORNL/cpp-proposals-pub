
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(metabench)

metabench_add_dataset(layout_left_access_const
  "layout_left/access.cpp.erb" "[2, 3, 4, 5, 6]"
)
metabench_add_dataset(layout_right_access_const
  "layout_right/access.cpp.erb" "[2, 3, 4, 5, 6]"
)
target_link_libraries(layout_left_access_const PUBLIC mdspan)
target_link_libraries(layout_right_access_const PUBLIC mdspan)
metabench_add_chart(const_access DATASETS layout_left_access_const layout_right_access_const)

################################################################################
