CXXFLAGS="-O3 -std=c++17 -O3 -I../include -I./ -g"
WARNING_FLAGS="-Wall -Wextra -Werror"
clang++ -c test_main.cpp ${CXXFLAGS} ${WARNING_FLAGS}
clang++ -c test_extents.cpp ${CXXFLAGS} ${WARNING_FLAGS}
clang++ -c test_layouts.cpp ${CXXFLAGS} ${WARNING_FLAGS}
clang++ -c test_mdspan.cpp ${CXXFLAGS} ${WARNING_FLAGS}
clang++ -c test_subspan.cpp ${CXXFLAGS} ${WARNING_FLAGS}
clang++ -c gtest/gtest-all.cc ${CXXFLAGS}
clang++ -o test test_extents.o test_layouts.o test_mdspan.o test_subspan.o test_main.o gtest-all.o -O3 ${WARNING_FLAGS}
