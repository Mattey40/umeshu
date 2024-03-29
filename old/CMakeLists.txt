# Copyright (c) 2010 Vladimir Chalupecky
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

cmake_minimum_required( VERSION 2.6 )

project( umeshu )

set( PACKAGE_BUGREPORT "vladimir.chalupecky@gmail.com" )
set( PACKAGE_NAME "Umeshu" )
set( PACKAGE_TARNAME "umeshu" )
set( PACKAGE_VERSION "0.0.1" )
set( PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall" )
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}" )
  message( FATAL_ERROR "In-source build prohibited." )
endif()

set( USE_FAST_GEOMETRIC_TESTS "off" CACHE BOOL "Use fast, but inexact version of geometric tests")

find_package( GLIB2 REQUIRED )

include_directories(
    ${GLIB_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src
    )

set( LIBS ${GLIB_LIBRARY} )

set( umeshu_SOURCES
    src/box2.c
    src/edge.c
    src/element.c
    src/mesh.c
    src/mesh_delaunay.c
    src/mesh_output.c
    src/mesh_refine.c
    src/mesh_relax.c
    src/mesh_smooth.c
    src/point2.c
    src/polygon.c
    src/polygon_triangulate.c
    src/predicates.c
    src/node.c
    predicates_init.h
    )

add_definitions( -DG_LOG_DOMAIN=\"umeshu\" )

add_library( umeshu ${umeshu_SOURCES} )
add_subdirectory( examples )

########### Tests ##############################################################
enable_testing()
add_subdirectory( tests )

########### Generate predicates_init.h #########################################
add_executable( predicates_init src/predicates_init.c )
add_custom_command(
   OUTPUT predicates_init.h
   COMMAND predicates_init > predicates_init.h
   DEPENDS predicates_init
   )

########### Doxygen Documentation ##############################################
find_package(Doxygen)
if( DOXYGEN_FOUND )
    set( DOXYGEN_SOURCE_DIR ${delaunay_SOURCE_DIR}/src ${delaunay_SOURCE_DIR}/examples )
    string( REGEX REPLACE ";" " " DOXYGEN_INPUT_LIST "${DOXYGEN_SOURCE_DIR}" )
    configure_file( Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    add_custom_target( docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )
endif()

########### config.h tests #####################################################
include(CheckIncludeFiles)
include(CheckCSourceCompiles)

check_include_files( fpu_control.h HAVE_FPU_CONTROL_H )
check_c_source_compiles("
    #warning test
    int main( void ) { return 0; }"
    CPP_HAS_WARNING )

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    )

