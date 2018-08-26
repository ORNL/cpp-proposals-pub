#include<mdspan>
#include<cstdio>
#include<gtest/gtest.h>

using std::experimental::fundamentals_v3::extents;
using std::experimental::fundamentals_v3::dynamic_extent;

class extents_ : public ::testing::Test {
protected:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }
};

template<ptrdiff_t ... E_STATIC>
struct test_extents {

  typedef extents<E_STATIC...> extents_type;

  extents_type my_extents_explicit,my_extents_array,my_extents_copy;
 
  template<class ... E>
  test_extents(E ... e) {
    my_extents_explicit = extents<E_STATIC...>(e...);
    //my_extents_array = extents<E_STATIC...>(std::array<ptrdiff_t,2>({{e}...}));
    my_extents_copy = extents<E_STATIC...>(my_extents_explicit);
  }

  void check_rank(ptrdiff_t r) {
    ASSERT_EQ(my_extents_explicit.rank(),r);
    //ASSERT_EQ(my_extents_array.rank(),r);
    ASSERT_EQ(my_extents_copy.rank(),r);
  }
  void check_rank_dynamic(ptrdiff_t r) {
    ASSERT_EQ(my_extents_explicit.rank_dynamic(),r);
    //ASSERT_EQ(my_extents_array.rank_dynamic(),r);
    ASSERT_EQ(my_extents_copy.rank_dynamic(),r);
  }
  template<class ... E>
  void check_extents(E ... e) {
    std::array<ptrdiff_t,extents_type::rank()> a({{e...}});
    for(int r = 0; r<extents_type::rank(); r++) {
      ASSERT_EQ(my_extents_explicit.extent(r),a[r]);
      //ASSERT_EQ(my_extents_array.extent(r),a[r]);
      ASSERT_EQ(my_extents_copy.extent(r),a[r]);
    }
  }
  
};

TEST_F(extents_,construction) {
  test_extents<5,dynamic_extent,3,dynamic_extent,1> test(4,2);

  test.check_rank(5);
  test.check_rank_dynamic(2);
  test.check_extents(5,4,3,2,1);

}

TEST_F(extents_,static_only) {
   test_extents<5,4,3> test;
   test.check_rank(3);
   test.check_rank_dynamic(0);
   test.check_extents(5,4,3);
}

TEST_F(extents_,rank_0) {
  test_extents<> test;
  test.check_rank(0);
  test.check_rank_dynamic(0);
}
TEST_F(extents_,assignment) {
  extents<5,dynamic_extent,3,dynamic_extent,1> e1(4,2);
  extents<5,4,3,2,1> e2;
  e2 = e1;
  for(int r=0; r<5; r++)
    ASSERT_EQ(e2.extent(r),e1.extent(r));
  extents<dynamic_extent,dynamic_extent,dynamic_extent,dynamic_extent,dynamic_extent> e3(9,8,7,6,5);
  for(int r=0; r<5; r++)
    ASSERT_EQ(e3.extent(r),9-r);
  e3=e1;
  for(int r=0; r<5; r++)
    ASSERT_EQ(e3.extent(r),e1.extent(r));
}

