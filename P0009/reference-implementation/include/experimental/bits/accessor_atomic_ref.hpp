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

#include <type_traits>

template<class T>
class atomic_ref {};

template<class T>
struct accessor_atomic {

  static_assert( std::is_trivially_copyable<T>::value );

	using element_type = T;
	using reference    = atomic_ref<T> ;
	using pointer      = T*;
	using offset       = accessor_atomic ;

  constexpr accessor_atomic() noexcept = default ;
  constexpr accessor_atomic( accessor_atomic && ) noexcept = default ;
  constexpr accessor_atomic( const accessor_atomic & ) noexcept = default ;
  accessor_atomic & operator =( accessor_atomic && ) noexcept = default ;
  accessor_atomic & operator =( const accessor_atomic & ) noexcept = default ;

  explicit constexpr accessor_atomic( pointer other ) noexcept
	  : ptr(other)
		{ assert( 0 == reinterpret_cast<uintptr_t>(ptr) % reference::required_alignment ); };

  constexpr reference operator[]( size_t i ) const noexcept
	  { return reference( ptr[i] ); }

  constexpr offset operator+( size_t i ) const noexcept
	  { return offset(ptr+i); }

  constexpr operator pointer() const
	  { assert(false /* cannot access raw data outside of atomic */); }
};

