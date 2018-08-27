#include<mdspan>
#include<cstdio>
#include<gtest/gtest.h>

using std::experimental::fundamentals_v3::extents;
using std::experimental::fundamentals_v3::dynamic_extent;
using std::experimental::fundamentals_v3::layout_right;
using std::experimental::fundamentals_v3::layout_left;

class layouts_ : public ::testing::Test {
protected:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }
};

template<class Layout, ptrdiff_t ... E_STATIC>
struct test_layouts {

  typedef Layout layout_type;
  typedef extents<E_STATIC...> extents_type;
  typedef typename Layout::template mapping<extents_type> mapping_type;

  mapping_type my_mapping_explicit,my_mapping_array,my_mapping_copy;
 
  template<class ... E>
  test_layouts(E ... e) {
    my_mapping_explicit = mapping_type(e...);
    //my_mapping_array = <E_STATIC...>(std::array<ptrdiff_t,2>({{e}...}));
    my_mapping_copy = mapping_type(my_mapping_explicit);
  }

  void check_rank(ptrdiff_t r) {
    ASSERT_EQ(my_mapping_explicit.extents().rank(),r);
    //ASSERT_EQ(my_mapping_array.extents().rank(),r);
    ASSERT_EQ(my_mapping_copy.extents().rank(),r);
  }
  void check_rank_dynamic(ptrdiff_t r) {
    ASSERT_EQ(my_mapping_explicit.extents().rank_dynamic(),r);
    //ASSERT_EQ(my_mapping_array.extents().rank_dynamic(),r);
    ASSERT_EQ(my_mapping_copy.extents().rank_dynamic(),r);
  }
  template<class ... E>
  void check_extents(E ... e) {
    std::array<ptrdiff_t,extents_type::rank()> a({{e...}});
    for(size_t r = 0; r<extents_type::rank(); r++) {
      ASSERT_EQ(my_mapping_explicit.extents().extent(r),a[r]);
      //ASSERT_EQ(my_mapping_array.extents().extent(r),a[r]);
      ASSERT_EQ(my_mapping_copy.extents().extent(r),a[r]);
    }
  }
  template<class ... E>
  void check_strides(E ... e) {
    std::array<ptrdiff_t,extents_type::rank()> a({{e...}});
    for(size_t r = 0; r<extents_type::rank(); r++) {
      ASSERT_EQ(my_mapping_explicit.stride(r),a[r]);
      //ASSERT_EQ(my_mapping_array.stride(r),a[r]);
      ASSERT_EQ(my_mapping_copy.stride(r),a[r]);
    }
  }
  void check_required_span_size(ptrdiff_t size) {
    ASSERT_EQ(my_mapping_explicit.required_span_size(),size);
    //ASSERT_EQ(my_mapping_array.required_span_size(),size);
    ASSERT_EQ(my_mapping_copy.required_span_size(),size);
  }
 
  void check_properties(bool always_unique, bool always_contiguous, bool always_strided,
                        bool unique, bool contiguous, bool strided) {
    ASSERT_EQ(my_mapping_explicit.is_always_unique()?1:0,always_unique?1:0);
    ASSERT_EQ(my_mapping_explicit.is_always_contiguous()?1:0,always_contiguous?1:0);
    ASSERT_EQ(my_mapping_explicit.is_always_strided()?1:0,always_strided?1:0);
    ASSERT_EQ(my_mapping_explicit.is_unique()?1:0,unique?1:0);
    ASSERT_EQ(my_mapping_explicit.is_contiguous()?1:0,contiguous?1:0);
    ASSERT_EQ(my_mapping_explicit.is_strided()?1:0,strided?1:0);
    ASSERT_EQ(my_mapping_copy.is_always_unique()?1:0,always_unique?1:0);
    ASSERT_EQ(my_mapping_copy.is_always_contiguous()?1:0,always_contiguous?1:0);
    ASSERT_EQ(my_mapping_copy.is_always_strided()?1:0,always_strided?1:0);
    ASSERT_EQ(my_mapping_copy.is_unique()?1:0,unique?1:0);
    ASSERT_EQ(my_mapping_copy.is_contiguous()?1:0,contiguous?1:0);
    ASSERT_EQ(my_mapping_copy.is_strided()?1:0,strided?1:0);
  } 

  template<class ... E>
  void check_operator(ptrdiff_t offset, E ... e) {
    //std::array<ptrdiff_t,extents_type::rank()> a({{e...}});
    ASSERT_EQ(my_mapping_explicit(e...),offset);
    ASSERT_EQ(my_mapping_copy(e...),offset);
  }

};

TEST_F(layouts_,construction_right) {
  test_layouts<layout_right,5,dynamic_extent,3,dynamic_extent,1> test(4,2);

  test.check_rank(5);
  test.check_rank_dynamic(2);
  test.check_extents(5,4,3,2,1);
  test.check_strides(24,6,2,1,1);

}

TEST_F(layouts_,construction_left) {
  test_layouts<layout_left,5,dynamic_extent,3,dynamic_extent,1> test(4,2);

  test.check_rank(5);
  test.check_rank_dynamic(2);
  test.check_extents(5,4,3,2,1);
  test.check_strides(1,5,20,60,120);
}

TEST_F(layouts_,properties_right) {
  test_layouts<layout_right,5,dynamic_extent,3,dynamic_extent,1> test(4,2);

  test.check_properties(true,true,true,true,true,true);
}

TEST_F(layouts_,properties_left) {
  test_layouts<layout_left,5,dynamic_extent,3,dynamic_extent,1> test(4,2);

  test.check_properties(true,true,true,true,true,true);
}

TEST_F(layouts_,operator_right) {
  test_layouts<layout_right,5,dynamic_extent,3,dynamic_extent,1> test(4,2);
   
  test.check_operator(107,4,1,2,1,0);
  test.check_operator(0,0,0,0,0,0);
}

TEST_F(layouts_,operator_left) {
  test_layouts<layout_left,5,dynamic_extent,3,dynamic_extent,1> test(4,2);
   
  test.check_operator(109,4,1,2,1,0);
  test.check_operator(0,0,0,0,0,0);
}


