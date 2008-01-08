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

# Build options based on OS
if (WIN32)
	add_definitions(-D_STANDARD_)
endif (WIN32)

if (APPLE)
	# Nothing for now
endif (APPLE)

if ( UNIX AND NOT APPLE )
	# Nothing for now
endif ( UNIX AND NOT APPLE )

# ------------------------------------------------------------------------------
# Require packages
# ------------------------------------------------------------------------------
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

include ( ${PROJECT_SOURCE_DIR}/cmake/FindNewmat.cmake )
if (NOT NEWMAT_FOUND)
	message( WARNING " Newmat is missing.")
	set(DEPENDENCY_CHECK 0)
endif (NOT NEWMAT_FOUND)
	

# ------------------------------------------------------------------------------
# Finally, recurse in the sub directories
# ------------------------------------------------------------------------------
if (NOT DEPENDENCY_CHECK)
	message( ERROR " ${TARGET_NAME} is missing a required dependency. Skipping it." )
else (NOT DEPENDENCY_CHECK)
	# We have all of our dependencies... so build it!
	subdirs( src )
	
	if (BUILD_TESTS)
		subdirs( tests )
	endif (BUILD_TESTS)
	include ( ${PROJECT_SOURCE_DIR}/cmake/WriteMatlabCompile.cmake )
endif (NOT DEPENDENCY_CHECK)
