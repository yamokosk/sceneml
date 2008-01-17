# Locate Newmat home

# This module defines
# NEWMAT_HOME where to find include, lib, bin, etc.
# NEWMAT_FOUND, If set to 0, don't try to use Newmat.

# start with 'not found'
SET( NEWMAT_FOUND 0 CACHE BOOL "Do we have Newmat?" )

FIND_PATH( NEWMAT_HOME_INCLUDE newmat.h
  $ENV{NEWMAT_HOME}/include
  # Test standard installation points
  /opt/roboop-1.31.0/newmat
  c:/roboop-1.31.0/newmat
)

IF ( NEWMAT_HOME_INCLUDE )
	SET( NEWMAT_FOUND 1 CACHE BOOL "Do we have Newmat?" FORCE )

	# strip 'file' to get rid of 'include'
	GET_FILENAME_COMPONENT( NEWMAT_HOME ${NEWMAT_HOME_INCLUDE} PATH )
ENDIF ( NEWMAT_HOME_INCLUDE )
