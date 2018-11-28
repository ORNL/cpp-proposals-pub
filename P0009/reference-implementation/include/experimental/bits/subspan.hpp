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

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace detail {

template<class ExtentsNew, class ExtentsOld, class ... SliceSpecifiers>
struct compose_new_extents;

template<ptrdiff_t ... ExtentsNew, ptrdiff_t E0, ptrdiff_t ... ExtentsOld, class ... SliceSpecifiers>
struct compose_new_extents<extents<ExtentsNew...>,extents<E0,ExtentsOld...>,all_type,SliceSpecifiers...> {
  typedef compose_new_extents<extents<ExtentsNew...,E0>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents;
  typedef typename next_compose_new_extents::extents_type extents_type; 

  template<class OrgExtents, class ... DynamicExtents>
  static constexpr extents_type create_sub_extents(const OrgExtents e,  
                        array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset,
                                                   all_type, SliceSpecifiers...s, DynamicExtents...de) {
    strides[sizeof...(ExtentsNew)] = strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...);
  }
};
template<ptrdiff_t ... ExtentsNew, ptrdiff_t ... ExtentsOld, class ... SliceSpecifiers>
struct compose_new_extents<extents<ExtentsNew...>,extents<dynamic_extent,ExtentsOld...>,all_type,SliceSpecifiers...> {
  typedef compose_new_extents<extents<ExtentsNew...,dynamic_extent>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents;
  typedef typename next_compose_new_extents::extents_type extents_type; 

  template<class OrgExtents, class ... DynamicExtents>
  static constexpr extents_type create_sub_extents(const OrgExtents e,  
                        array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset,
                                                   all_type, SliceSpecifiers...s, DynamicExtents...de) {
    strides[sizeof...(ExtentsNew)] = strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...,e.extent(OrgExtents::rank()-sizeof...(SliceSpecifiers)-1));
  }
};

template<ptrdiff_t ... ExtentsNew, ptrdiff_t E0, ptrdiff_t ... ExtentsOld, class IT, class ... SliceSpecifiers>
struct compose_new_extents<extents<ExtentsNew...>,extents<E0,ExtentsOld...>,pair<IT,IT>,SliceSpecifiers...> {
  typedef compose_new_extents<extents<ExtentsNew...,dynamic_extent>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents; 
  typedef typename next_compose_new_extents::extents_type extents_type; 

  template<class OrgExtents, class ... DynamicExtents>
  static constexpr extents_type create_sub_extents(const OrgExtents e, array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset,
                                                   pair<IT,IT> p, SliceSpecifiers ... s, DynamicExtents...de) {
    strides[sizeof...(ExtentsNew)] = strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
    offset += p.first*strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...,ptrdiff_t(p.second-p.first));
  }
};

template<ptrdiff_t ... ExtentsNew, ptrdiff_t E0, ptrdiff_t ... ExtentsOld, class IT, class ... SliceSpecifiers>
struct compose_new_extents<extents<ExtentsNew...>,extents<E0,ExtentsOld...>,IT,SliceSpecifiers...> {
  typedef compose_new_extents<extents<ExtentsNew...>,extents<ExtentsOld...>,SliceSpecifiers...> next_compose_new_extents;
  typedef typename next_compose_new_extents::extents_type extents_type; 

  template<class OrgExtents, class ... DynamicExtents>
  static constexpr extents_type create_sub_extents(const OrgExtents e, array<ptrdiff_t,OrgExtents::rank()>& strides, ptrdiff_t& offset, 
                                                   const ptrdiff_t v, SliceSpecifiers...s,DynamicExtents...de) {
    offset += v*strides[OrgExtents::rank()-sizeof...(SliceSpecifiers)-1];
    return next_compose_new_extents::create_sub_extents(e,strides,offset,s...,de...);
  }
};

template<ptrdiff_t ... ExtentsNew>
struct compose_new_extents<extents<ExtentsNew...>,extents<>> {
  typedef extents<ExtentsNew...> extents_type; 

  template<class OrgExtents, class ... DynamicExtents>
  static constexpr extents_type create_sub_extents(const OrgExtents, array<ptrdiff_t,OrgExtents::rank()>, ptrdiff_t, DynamicExtents...de) {
    return extents_type(de...);
  }
  template<class OrgExtents>
  static constexpr extents_type create_sub_extents(const OrgExtents, array<ptrdiff_t,OrgExtents::rank()>, ptrdiff_t) {
    return extents_type();
  }
};


template<class Extents, class...SliceSpecifiers>
struct subspan_deduce_extents {
  typedef typename compose_new_extents<extents<>,Extents,SliceSpecifiers...>::extents_type extents_type;
  typedef array<ptrdiff_t,Extents::rank()> stride_type;
  static constexpr extents_type create_sub_extents(const Extents e,stride_type& strides, ptrdiff_t& offset, SliceSpecifiers...s) {
    return compose_new_extents<extents<>,Extents,SliceSpecifiers...>::create_sub_extents(e,strides,offset,s...);
  }
};

}

template<class ElementType, class Extents, class LayoutPolicy,
           class AccessorPolicy, class... SliceSpecifiers>
    basic_mdspan<ElementType, typename detail::subspan_deduce_extents<Extents,SliceSpecifiers...>::extents_type,
                 layout_stride, typename AccessorPolicy::offset_policy >
      subspan(const basic_mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>& src, SliceSpecifiers ... slices) noexcept {
    typedef typename detail::subspan_deduce_extents<Extents,SliceSpecifiers...>::extents_type sub_extents_type;
    typedef typename AccessorPolicy::offset_policy sub_accessor_policy;
    typedef basic_mdspan<ElementType,sub_extents_type,layout_stride,sub_accessor_policy> sub_mdspan_type;

    array<ptrdiff_t,Extents::rank()> strides;
    for(size_t r = 0; r<Extents::rank(); r++)
      strides[r] = src.stride(r);

    ptrdiff_t offset = 0;
    sub_extents_type sub_extents = detail::subspan_deduce_extents<Extents,SliceSpecifiers...>::create_sub_extents(src.extents(),strides,offset,slices...);

    array<ptrdiff_t,sub_extents_type::rank()> sub_strides;
    for(size_t r = 0; r<sub_extents_type::rank(); r++)
      sub_strides[r] = strides[r];

    typename AccessorPolicy::offset_policy::pointer ptr = src.accessor().offset(src.data(),offset);    
    return sub_mdspan_type(ptr,typename sub_mdspan_type::mapping_type(sub_extents,sub_strides));
  }

}}}
