# Test CMake version
CMAKE_MINIMUM_REQUIRED( VERSION 2.4 FATAL_ERROR )
# Older versions have a different signature, check it manually just to make sure
IF ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.4 )
	MESSAGE( FATAL_ERROR	"Orca2 requires at least CMake v2.4. You are running v${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}. Please upgrade." )
ENDIF ( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.4 )

# ---------------------------------------------------------------------------------------------------------------------------
# Project information - name, version, and the project's cmake directory
# ---------------------------------------------------------------------------------------------------------------------------
PROJECT( sceneml )
SET( PROJECT_VERSION	1.0.0	CACHE STRING "Version of 'SceneML' distribution" )
SET( SCENEML_INSTALL_ENV_VAR	SCENEML_INSTALL )

# Reflect some info back to the builder
MESSAGE( STATUS "Setting project name to ${PROJECT_NAME}" )
MESSAGE( STATUS "Setting project version to ${PROJECT_VERSION}" )

# ---------------------------------------------------------------------------------------------------------------------------
# Defaults for big source code switches 
# (these are defaults. after the user modifies these in GUI they stay in cache)
# ---------------------------------------------------------------------------------------------------------------------------
OPTION( BUILD_TESTS   "Turn me off to disable compilation of all tests"				ON )

# The rest of the setup is done by this script
INCLUDE( cmake/global_setup.cmake )
