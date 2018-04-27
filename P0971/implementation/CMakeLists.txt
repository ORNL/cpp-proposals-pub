
cmake_minimum_required(VERSION 3.1.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

project(async_ptr VERSION 1.0)

option(USE_HPX "Use Concurrency TS implementation provided by HPX" OFF)

if(NOT USE_HPX)

  find_package(Boost 1.65 REQUIRED
    COMPONENTS thread
  )

  if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found.  Boost is required because it has "
      "a working implementation of (basically) the Concurrency TS v1.  Set "
      "BOOST_ROOT to a valid installation of Boost 1.65 with (at least) the "
      "thread library."
    )
  endif()

  include_directories( ${Boost_INCLUDE_DIRS} )
  link_directories( ${Boost_LIBRARY_DIRS} )

else()
  find_package(HPX 1.0 REQUIRED)
  set(CMAKE_CXX_FLAGS "-DASYNC_PTR_USE_HPX_FOR_CONCURRENCY_TS ${CMAKE_CXX_FLAGS}")
  include_directories( ${HPX_INCLUDE_DIRS} )
  link_directories( ${HPX_LIBRARY_DIR} )
  include(HPX_SetupTarget)
endif()

add_subdirectory(include)

function(setup_target tgt)
  set_property(TARGET ${tgt} PROPERTY CXX_STANDARD_REQUIRED 17)
  if(NOT USE_HPX)
    target_link_libraries( ${tgt} LINK_PUBLIC ${Boost_LIBRARIES} )
  else()
    hpx_setup_target(${tgt})
  endif()
endfunction()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

add_subdirectory(examples)
