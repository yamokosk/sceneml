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
option( BUILD_TESTS "Try to build the tests library" OFF )
option( BUILD_SHARED "Build the SceneML library shared." ON )
option( BUILD_TEST "Build the SceneML test programs." OFF )
option( BUILD_DEBUG "Build a debug version of the library." OFF )
option( BUILD_MATLAB "Build the mex interface to the SceneML library." ON )

# ------------------------------------------------------------------------------
# Project directories
# ------------------------------------------------------------------------------
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

# Use environmental variable if it is set
set( PROJECT_INSTALL_DIR $ENV{${PROJECT_INSTALL_ENV_VAR}} )
# If environment variable SCENEML_INSTALL is NOT set, use defaults
if( NOT PROJECT_INSTALL_DIR )
	if ( NOT WIN32 )
		# Linux and friends: if environment variable ORCA2_INSTALL is not set, e.g. /opt/orca-1.2.3
		set( PROJECT_INSTALL_DIR /opt/${PROJECT_NAME}-${PROJECT_VERSION} )
	else ( NOT WIN32 )
		# Windows: e.g. C:\orca-1.2.3
		set( PROJECT_INSTALL_DIR C:/${PROJECT_NAME}-${PROJECT_VERSION} )
	endif ( NOT WIN32 )
endif( NOT PROJECT_INSTALL_DIR )
set( CMAKE_INSTALL_PREFIX ${PROJECT_INSTALL_DIR} CACHE PATH "Installation directory" FORCE )
message( STATUS "Setting installation directory to ${CMAKE_INSTALL_PREFIX}" )

# ------------------------------------------------------------------------------
# Set build type and other build options
# ------------------------------------------------------------------------------
# Build type
if( BUILD_DEBUG )
	message(STATUS "Building debug version of SceneML")
	set(CMAKE_BUILD_TYPE Debug)
else( BUILD_DEBUG )
	message(STATUS "Building release version of SceneML")
	set(CMAKE_BUILD_TYPE Release)
endif( BUILD_DEBUG )

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
	if ( BUILD_SHARED )
		add_definitions(-DSCENEML_DLL)
	endif ( BUILD_SHARED )
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

	
# ------------------------------------------------------------------------------
# Library types
# ------------------------------------------------------------------------------
if( BUILD_SHARED )
	set( LIBRARY_TYPE	SHARED )
else( BUILD_SHARED )
	set( LIBRARY_TYPE	STATIC )
endif( BUILD_SHARED )

# ------------------------------------------------------------------------------
# Installation preferences
# ------------------------------------------------------------------------------
set( CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib )

# Finally, recurse in the sub directories
if (NOT DEPENDENCY_CHECK)
	message( ERROR " ${TARGET_NAME} is missing a required dependency. Skipping it." )
else (NOT DEPENDENCY_CHECK)
	# We have all of our dependencies... so build it!
	subdirs( src )
	
	if (BUILD_TESTS)
		subdirs( tests )
	endif (BUILD_TESTS)
endif (NOT DEPENDENCY_CHECK)