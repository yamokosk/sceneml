# Locate MuParser home

# This module defines
# MUPARSER_HOME where to find include, lib, bin, etc.
# MUPARSER_FOUND, If set to 0, don't try to use Xerces.

# start with 'not found'
SET( MUPARSER_FOUND 0 CACHE BOOL "Do we have MuParser?" )

FIND_PATH( MUPARSER_HOME_INCLUDE muParser.h
  $ENV{MUPARSER_HOME}/include
  # Standard linux installation points
  # Standard windows installation points
  C:/muparser/include
)

IF ( MUPARSER_HOME_INCLUDE )
	SET( MUPARSER_FOUND 1 CACHE BOOL "Do we have MuParser?" FORCE )

	# strip to get rid of 'include'
	GET_FILENAME_COMPONENT( MUPARSER_HOME ${MUPARSER_HOME_INCLUDE} PATH )
ENDIF ( MUPARSER_HOME_INCLUDE )