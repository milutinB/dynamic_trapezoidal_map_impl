# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

project( main )


cmake_minimum_required(VERSION 2.8.11)

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

# include for local directory

# include for local package


# Creating entries for target: main
# ############################

add_executable( main  BoundingBox.cpp Cut.cpp main.cpp Node.cpp Segment.cpp Tree.cpp )

add_to_cached_list( CGAL_EXECUTABLE_TARGETS main )

# Link the executable to CGAL and third-party libraries
target_link_libraries(main   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )

