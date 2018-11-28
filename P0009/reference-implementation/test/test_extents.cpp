//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2014) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Kokkos is licensed under 3-clause BSD terms of use:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
//
// ************************************************************************
//@HEADER

#include<experimental/mdspan>
#include<cstdio>
#include"gtest/gtest.h"

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

  test_extents() {
    my_extents_explicit = extents<E_STATIC...>();
    my_extents_array = extents<E_STATIC...>(std::array<ptrdiff_t,0>());
    my_extents_copy = extents<E_STATIC...>(my_extents_explicit);
  }

  template<class ... E>
  test_extents(E ... e) {
    my_extents_explicit = extents<E_STATIC...>(e...);
    my_extents_array = extents<E_STATIC...>(std::array<ptrdiff_t,2>({{e...}}));
    my_extents_copy = extents<E_STATIC...>(my_extents_explicit);
  }

  void check_rank(ptrdiff_t r) {
    ASSERT_EQ(my_extents_explicit.rank(),r);
    ASSERT_EQ(my_extents_array.rank(),r);
    ASSERT_EQ(my_extents_copy.rank(),r);
  }
  void check_rank_dynamic(ptrdiff_t r) {
    ASSERT_EQ(my_extents_explicit.rank_dynamic(),r);
    ASSERT_EQ(my_extents_array.rank_dynamic(),r);
    ASSERT_EQ(my_extents_copy.rank_dynamic(),r);
  }
  template<class ... E>
  void check_extents(E ... e) {
    std::array<ptrdiff_t,extents_type::rank()> s({{E_STATIC...}});
    std::array<ptrdiff_t,extents_type::rank()> a({{e...}});
    for(size_t r = 0; r<extents_type::rank(); r++) {
      ASSERT_EQ(my_extents_explicit.static_extent(r),s[r]);
      ASSERT_EQ(my_extents_explicit.extent(r),a[r]);

      ASSERT_EQ(my_extents_array.static_extent(r),s[r]);
      ASSERT_EQ(my_extents_array.extent(r),a[r]);

      ASSERT_EQ(my_extents_copy.static_extent(r),s[r]);
      ASSERT_EQ(my_extents_copy.extent(r),a[r]);
    }
    ASSERT_EQ(my_extents_explicit.static_extent(extents_type::rank()+1),1);
    ASSERT_EQ(my_extents_explicit.extent(extents_type::rank()+1),1);

    ASSERT_EQ(my_extents_array.static_extent(extents_type::rank()+1),1);
    ASSERT_EQ(my_extents_array.extent(extents_type::rank()+1),1);

    ASSERT_EQ(my_extents_copy.static_extent(extents_type::rank()+1),1);
    ASSERT_EQ(my_extents_copy.extent(extents_type::rank()+1),1);
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
  for(size_t r=0; r<5; r++)
    ASSERT_EQ(e2.extent(r),e1.extent(r));
  extents<dynamic_extent,dynamic_extent,dynamic_extent,dynamic_extent,dynamic_extent> e3(9,8,7,6,5);
  for(int r=0; r<5; r++)
    ASSERT_EQ(e3.extent(r),9-r);
  e3=e1;
  for(int r=0; r<5; r++)
    ASSERT_EQ(e3.extent(r),e1.extent(r));
}

