clang++ -c test_main.cpp -std=c++17 -O3 -I../include -I./
clang++ -c test_extents.cpp -std=c++17 -O3 -I../include -I./
clang++ -c test_layouts.cpp -std=c++17 -O3 -I../include -I./
clang++ -c gtest/gtest-all.cc -O3 -I./
clang++ -o test test_extents.o test_layouts.o test_main.o gtest-all.o

