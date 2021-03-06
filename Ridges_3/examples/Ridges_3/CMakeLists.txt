# This is the CMake script for compiling a CGAL application.
cmake_minimum_required(VERSION 3.1...3.15)
project( Ridges_3_Examples )



find_package(CGAL QUIET )

if ( CGAL_FOUND )

  # use either Eigen
  find_package(Eigen3 3.1.0) #(requires 3.1.0 or greater)
  if (EIGEN3_FOUND)

    # Link with Boost.ProgramOptions (optional)
    find_package(Boost QUIET COMPONENTS program_options)
    if(TARGET Boost::program_options)
      set(Boost_PROGRAM_OPTIONS_LIBRARY Boost::program_options)
    endif()
    if(Boost_PROGRAM_OPTIONS_FOUND)
      if( CGAL_AUTO_LINK_ENABLED )
        message( STATUS "Boost.ProgramOptions library: found" )
      else()
        message( STATUS "Boost.ProgramOptions library: ${Boost_PROGRAM_OPTIONS_LIBRARY}" )
      endif()
      add_definitions( "-DCGAL_USE_BOOST_PROGRAM_OPTIONS" )
      list(APPEND CGAL_3RD_PARTY_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
   endif()

   create_single_source_cgal_program( Compute_Ridges_Umbilics.cpp)
   CGAL_target_use_Eigen(Compute_Ridges_Umbilics)
   create_single_source_cgal_program( Ridges_Umbilics_SM.cpp)
   CGAL_target_use_Eigen(Ridges_Umbilics_SM)
   create_single_source_cgal_program( Ridges_Umbilics_LCC.cpp)
   CGAL_target_use_Eigen(Ridges_Umbilics_LCC)

  else()

    message(STATUS "NOTICE: This program requires Eigen 3.1 (or greater) and will not be compiled.")

  endif()

else()

    message(STATUS "NOTICE: This program requires the CGAL library, and will not be compiled.")

endif()
