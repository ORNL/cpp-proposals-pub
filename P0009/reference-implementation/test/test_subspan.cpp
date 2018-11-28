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

using namespace std::experimental::fundamentals_v3;

class subspan_ : public ::testing::Test {
protected:
  static void SetUpTestCase() {
  }

  static void TearDownTestCase() {
  }
};

TEST_F(subspan_,static_extent_deduction) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  typedef detail::subspan_deduce_extents<extents_type,ptrdiff_t,std::pair<int,int>,all_type,all_type,ptrdiff_t>::extents_type new_extents_type;

  ASSERT_EQ(new_extents_type::rank(),3);
  ASSERT_EQ(new_extents_type::rank_dynamic(),2);
  ASSERT_EQ(new_extents_type::static_extent(0),dynamic_extent);
  ASSERT_EQ(new_extents_type::static_extent(1),3);
  ASSERT_EQ(new_extents_type::static_extent(2),dynamic_extent);
}

TEST_F(subspan_,dynamic_extent_deduction) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  typedef detail::subspan_deduce_extents<extents_type,ptrdiff_t,std::pair<int,int>,all_type,all_type,ptrdiff_t> sub_extent_deduce_type;
  typedef sub_extent_deduce_type::extents_type new_extents_type;

  std::array<ptrdiff_t,extents_type::rank()> strides;
  ptrdiff_t offset;
  extents_type e(4,2);

  new_extents_type sub_extents = sub_extent_deduce_type::create_sub_extents(e,strides,offset,2,std::pair<int,int>(1,3),all_type(),all_type(),0);
  ASSERT_EQ(sub_extents.static_extent(0),dynamic_extent);
  ASSERT_EQ(sub_extents.static_extent(1),3);
  ASSERT_EQ(sub_extents.static_extent(2),dynamic_extent);
  ASSERT_EQ(sub_extents.extent(0),2);
  ASSERT_EQ(sub_extents.extent(1),3);
  ASSERT_EQ(sub_extents.extent(2),2);
}

TEST_F(subspan_,strides_deduction_layout_right) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  typedef detail::subspan_deduce_extents<extents_type,ptrdiff_t,std::pair<int,int>,all_type,all_type,ptrdiff_t> sub_extent_deduce_type;
  //typedef sub_extent_deduce_type::extents_type new_extents_type;

  std::array<ptrdiff_t,extents_type::rank()> strides;
  ptrdiff_t offset = 0;
  extents_type e(4,2);
  layout_right::mapping<extents_type> map(e);
  for(size_t r=0; r<e.rank(); r++)
    strides[r] = map.stride(r);

  sub_extent_deduce_type::create_sub_extents(e,strides,offset,2,std::pair<int,int>(1,3),all_type(),all_type(),0);
  ASSERT_EQ(strides[0],map.stride(1));
  ASSERT_EQ(strides[1],map.stride(2));
  ASSERT_EQ(strides[2],map.stride(3));
}


TEST_F(subspan_,strides_deduction_layout_left) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  typedef detail::subspan_deduce_extents<extents_type,ptrdiff_t,std::pair<int,int>,all_type,all_type,ptrdiff_t> sub_extent_deduce_type;
  //typedef sub_extent_deduce_type::extents_type new_extents_type;

  std::array<ptrdiff_t,extents_type::rank()> strides;
  ptrdiff_t offset = 0;
  extents_type e(4,2);
  layout_left::mapping<extents_type> map(e);
  for(size_t r=0; r<e.rank(); r++)
    strides[r] = map.stride(r);

  sub_extent_deduce_type::create_sub_extents(e,strides,offset,2,std::pair<int,int>(1,3),all_type(),all_type(),0);
  ASSERT_EQ(strides[0],map.stride(1));
  ASSERT_EQ(strides[1],map.stride(2));
  ASSERT_EQ(strides[2],map.stride(3));
}

TEST_F(subspan_,offset_deduction_layout_right) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  typedef detail::subspan_deduce_extents<extents_type,ptrdiff_t,std::pair<int,int>,all_type,all_type,ptrdiff_t> sub_extent_deduce_type;
  //typedef sub_extent_deduce_type::extents_type new_extents_type;

  std::array<ptrdiff_t,extents_type::rank()> strides;
  ptrdiff_t offset = 0;
  extents_type e(4,2);
  layout_right::mapping<extents_type> map(e);
  for(size_t r=0; r<e.rank(); r++)
    strides[r] = map.stride(r);

  sub_extent_deduce_type::create_sub_extents(e,strides,offset,2,std::pair<int,int>(1,3),all_type(),all_type(),0);
  ASSERT_EQ(offset,2*map.stride(0)+map.stride(1));
}

TEST_F(subspan_,offset_deduction_layout_left) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  typedef detail::subspan_deduce_extents<extents_type,ptrdiff_t,std::pair<int,int>,all_type,all_type,ptrdiff_t> sub_extent_deduce_type;
  //typedef sub_extent_deduce_type::extents_type new_extents_type;

  std::array<ptrdiff_t,extents_type::rank()> strides;
  ptrdiff_t offset = 0;
  extents_type e(4,2);
  layout_left::mapping<extents_type> map(e);
  for(size_t r=0; r<e.rank(); r++)
    strides[r] = map.stride(r);

  sub_extent_deduce_type::create_sub_extents(e,strides,offset,2,std::pair<int,int>(1,3),all_type(),all_type(),0);
  ASSERT_EQ(offset,2*map.stride(0)+map.stride(1));
}

TEST_F(subspan_,basic_mdspan_layout_right) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  extents_type e(4,2);
  layout_right::mapping<extents_type> map(e);
  int* ptr = new int[map.required_span_size()];
  typedef basic_mdspan<int,extents_type,layout_right,accessor_basic<int> > mdspan_type;

  mdspan_type a(ptr,e);

  auto sub = subspan(a,ptrdiff_t(2),std::pair<int,int>(1,3),all_type(),all_type(),ptrdiff_t(0));
  ASSERT_EQ(sub.rank(),3);
  ASSERT_EQ(sub.rank_dynamic(),2);
  ASSERT_EQ(sub.static_extent(0),dynamic_extent);
  ASSERT_EQ(sub.static_extent(1),3);
  ASSERT_EQ(sub.static_extent(2),dynamic_extent);
  ASSERT_EQ(sub.extent(0),2);
  ASSERT_EQ(sub.extent(1),3);
  ASSERT_EQ(sub.extent(2),2);
  ASSERT_EQ(sub.is_contiguous()?1:0,1);
  ASSERT_EQ(sub.stride(0),a.stride(1));
  ASSERT_EQ(sub.stride(1),a.stride(2));
  ASSERT_EQ(sub.stride(2),a.stride(3));

  for(int i0=0; i0<a.extent(0); i0++)
  for(int i1=0; i1<a.extent(1); i1++)
  for(int i2=0; i2<a.extent(2); i2++)
  for(int i3=0; i3<a.extent(3); i3++)
  for(int i4=0; i4<a.extent(4); i4++)
    a(i0,i1,i2,i3,i4) = i0*10000+i1*1000+i2*100+i3*10+i4;

  for(int i0=0; i0<sub.extent(0); i0++)
  for(int i1=0; i1<sub.extent(1); i1++)
  for(int i2=0; i2<sub.extent(2); i2++)
    ASSERT_EQ(sub(i0,i1,i2),2*10000 + (i0+1)*1000+i1*100+i2*10);
  delete [] ptr;
}

TEST_F(subspan_,basic_mdspan_layout_left) {
  typedef extents<5,dynamic_extent,3,dynamic_extent,1> extents_type;
  extents_type e(4,2);
  layout_left::mapping<extents_type> map(e);
  int* ptr = new int[map.required_span_size()];
  typedef basic_mdspan<int,extents_type,layout_left,accessor_basic<int> > mdspan_type;

  mdspan_type a(ptr,e);

  auto sub = subspan(a,ptrdiff_t(2),std::pair<int,int>(1,3),all_type(),all_type(),ptrdiff_t(0));
  ASSERT_EQ(sub.rank(),3);
  ASSERT_EQ(sub.rank_dynamic(),2);
  ASSERT_EQ(sub.static_extent(0),dynamic_extent);
  ASSERT_EQ(sub.static_extent(1),3);
  ASSERT_EQ(sub.static_extent(2),dynamic_extent);
  ASSERT_EQ(sub.extent(0),2);
  ASSERT_EQ(sub.extent(1),3);
  ASSERT_EQ(sub.extent(2),2);
  ASSERT_EQ(sub.is_contiguous()?1:0,0);
  ASSERT_EQ(sub.stride(0),a.stride(1));
  ASSERT_EQ(sub.stride(1),a.stride(2));
  ASSERT_EQ(sub.stride(2),a.stride(3));

  for(int i0=0; i0<a.extent(0); i0++)
  for(int i1=0; i1<a.extent(1); i1++)
  for(int i2=0; i2<a.extent(2); i2++)
  for(int i3=0; i3<a.extent(3); i3++)
  for(int i4=0; i4<a.extent(4); i4++)
    a(i0,i1,i2,i3,i4) = i0*10000+i1*1000+i2*100+i3*10+i4;

  for(int i0=0; i0<sub.extent(0); i0++)
  for(int i1=0; i1<sub.extent(1); i1++)
  for(int i2=0; i2<sub.extent(2); i2++)
    ASSERT_EQ(sub(i0,i1,i2),2*10000 + (i0+1)*1000+i1*100+i2*10);
  delete [] ptr;
}

//TEST_F(subspan_,reduce_to_rank_0) {
//}
