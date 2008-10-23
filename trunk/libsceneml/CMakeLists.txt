cmake_minimum_required( VERSION 2.4 FATAL_ERROR )
# Older versions have a different signature, check it manually just to make sure
if ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.4 )
	message( FATAL_ERROR	"SceneML requires at least CMake v2.4. You are running v${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}. Please upgrade." )
endif ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.4 )

# ---------------------------------------------------------------------------------------------------------------------------
# Project information - name, version, and the project's cmake directory
# ---------------------------------------------------------------------------------------------------------------------------
project( sceneml )
set( PROJECT_VERSION	1.0.0	CACHE STRING "Version of 'SceneML' distribution" )
set(TARGET_NAME "sceneml")

# ---------------------------------------------------------------------------------------------------------------------------
# Project options
# ---------------------------------------------------------------------------------------------------------------------------
option( BUILD_TESTS "Try to build the tests of the library." OFF )
option( BUILD_MATLAB "Build the mex interface to the SceneML library." ON )
option( BUILD_STATIC_LIB "Build static library?" ON)
option( PERFORM_UNIT_TESTS "Perform unit tests." ON)

# Easier to read error messages in Eclipse CDT
IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# Throwing another bone at eclipse - helps "discover" include paths
SET(CMAKE_VERBOSE_MAKEFILE ON)


# ------------------------------------------------------------------------------
# Project directories
# ------------------------------------------------------------------------------
GET_FILENAME_COMPONENT( PROJECT_BASE_DIR ${PROJECT_SOURCE_DIR} PATH )

# ------------------------------------------------------------------------------
# Set build type and other build options
# ------------------------------------------------------------------------------
#set(CMAKE_BUILD_TYPE Release)

# Build options based on processor
if( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "Pentium" )
	if( MSVC )
		add_definitions(arch:SSE)
	endif( MSVC )
	
	if ( CMAKE_COMPILER_IS_GNUCXX )
		add_definitions(-march=pentium4)
	endif ( CMAKE_COMPILER_IS_GNUCXX )
endif( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "Pentium" )

set(CMAKE_BUILD_TYPE "Debug")

# ------------------------------------------------------------------------------
# Require packages
# ------------------------------------------------------------------------------
message( STATUS " == Dependencies == " )
set( DEPENDENCY_CHECK 1 )
include ( ${PROJECT_SOURCE_DIR}/cmake/FindODE.cmake )
if (NOT ODE_FOUND)
	message( WARNING " ODE is missing.")
	set(DEPENDENCY_CHECK 0)
endif (NOT ODE_FOUND)

include ( ${PROJECT_SOURCE_DIR}/cmake/FindMuParser.cmake )
if (NOT MUPARSER_FOUND)
	message( WARNING " MuParser is missing.")
	set(DEPENDENCY_CHECK 0)
endif (NOT MUPARSER_FOUND)

include ( ${PROJECT_SOURCE_DIR}/cmake/FindXerces.cmake )
if (NOT XERCES_FOUND)
	message( WARNING " Xerces-C is missing.")
	set(DEPENDENCY_CHECK 0)
endif (NOT XERCES_FOUND)

find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
	message( WARNING " Boost is missing." )
	set(DEPENDENCY_CHECK 0)
endif (NOT Boost_FOUND)

find_package(PkgConfig REQUIRED)
pkg_check_modules (NEWMAT REQUIRED newmat)
message( STATUS "Found Newmat." )
message( STATUS "	Include dirs: ${NEWMAT_INCLUDE_DIRS}" )
message( STATUS "	Library dirs: ${NEWMAT_LIBRARY_DIRS}" )
message( STATUS "	Libs: ${NEWMAT_LIBRARIES}" )
message( STATUS "	LDFlags: ${NEWMAT_LDFLAGS}" )
message( STATUS "	CFlags: ${NEWMAT_CFLAGS}" )

pkg_check_modules (CPPUNIT REQUIRED cppunit)
message( STATUS "Found CppUnit." )
message( STATUS "	Include dirs: ${CPPUNIT_INCLUDE_DIRS}" )
message( STATUS "	Library dirs: ${CPPUNIT_LIBRARY_DIRS}" )
message( STATUS "	Libs: ${CPPUNIT_LIBRARIES}" )
message( STATUS "	LDFlags: ${CPPUNIT_LDFLAGS}" )
message( STATUS "	CFlags: ${CPPUNIT_CFLAGS}" )

pkg_check_modules (LOG4CXX REQUIRED liblog4cxx)
message( STATUS "Found log4cxx." )
message( STATUS "	Include dirs: ${LOG4CXX_INCLUDE_DIRS}" )
message( STATUS "	Library dirs: ${LOG4CXX_LIBRARY_DIRS}" )
message( STATUS "	Libs: ${LOG4CXX_LIBRARIES}" )
message( STATUS "	LDFlags: ${LOG4CXX_LDFLAGS}" )
message( STATUS "	CFlags: ${LOG4CXX_CFLAGS}" )

# ------------------------------------------------------------------------------
# Build unittests
# ------------------------------------------------------------------------------
#enable_testing()

#if ( NOT WIN32 AND NOT APPLE)
#	MEMORYCHECK_COMMAND:FILEPATH=/usr/bin/valgrind
#endif ( NOT WIN32 AND NOT APPLE)

#include_directories( ${PROJECT_SOURCE_DIR}/src
#					 ${CPPUNIT_INCLUDE_DIRS}
#					 ${Boost_INCLUDE_DIRS}
#					 ${NEWMAT_INCLUDE_DIRS}
#					 ${MUPARSER_HOME}/include)
#link_directories( ${CPPUNIT_LIBRARY_DIRS}
#                  ${Boost_LIBRARY_DIRS}
#				  ${NEWMAT_LIBRARY_DIRS}
#				  ${MUPARSER_HOME}/lib )
#link_libraries( ${CPPUNIT_LIBRARIES} )
#file(GLOB UnitTests_SRCS "${PROJECT_SOURCE_DIR}/unittest/*Test.cpp" )

#foreach(test ${UnitTests_SRCS})
#    get_filename_component(TestName ${test} NAME_WE)
#    include( ${PROJECT_SOURCE_DIR}/unittest/${TestName}.inc )
#    set( srcs ${PROJECT_SOURCE_DIR}/unittest/main.cpp ${test} ${test_srcs} )
#    add_executable(${TestName} ${srcs})
#    if ( WIN32 )
#        target_link_libraries(${TestName} debug muparser_static_D optimized muparser_static
#                                      debug newmatD optimized newmat)
#    else ( WIN32 )
#        target_link_libraries(${TestName} muparser newmat)
#    endif ( WIN32 )
#    add_test(${TestName} ${TestName}${CMAKE_EXECUTABLE_SUFFIX} )
#endforeach(test)


# ------------------------------------------------------------------------------
# Finally, recurse in the sub directories
# ------------------------------------------------------------------------------
if (NOT DEPENDENCY_CHECK)
	message( ERROR " ${TARGET_NAME} is missing a required dependency. Skipping it." )
else (NOT DEPENDENCY_CHECK)
	# Set variable which contains all the libraries we need
	if (WIN32)
		add_definitions (-DXML_LIBRARY)
		set(REQUIRED_LIBS ode_static xerces-c_static_2 muparser_static newmat)
	else (WIN32)
		set(REQUIRED_LIBS ode xerces-c muparser newmat)
	endif (WIN32)

	# We have all of our dependencies... so build it!
	#subdirs( src )
	add_subdirectory(unittest)
	add_subdirectory(plugins)
	#if (BUILD_TESTS)
	#	subdirs( tests )
	#endif (BUILD_TESTS)
	include ( ${PROJECT_SOURCE_DIR}/cmake/WriteMatlabCompile.cmake )
endif (NOT DEPENDENCY_CHECK)
