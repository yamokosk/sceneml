# Locate Roboop home

# This module defines
# ROBOOP_HOME where to find include, lib, bin, etc.
# ROBOOP_FOUND, If set to 0, don't try to use Roboop.

# start with 'not found'
SET( ODE_FOUND 0 CACHE BOOL "Do we have ODE?" )

FIND_PATH( ODE_HOME_INCLUDE_ODE ode.h
  $ENV{ODE_HOME}/include/ode
  # Test standard installation points
  /usr/local/include/ode
 )

# NOTE: if ORCA2_HOME_INCLUDE_ORCA is set to *-NOTFOUND it will evaluate to FALSE
IF ( ODE_HOME_INCLUDE_ODE )
	SET( ODE_FOUND 1 CACHE BOOL "Do we have ODE?" FORCE )

	# strip 'file' to get rid of 'include'
	GET_FILENAME_COMPONENT( ODE_HOME_INCLUDE ${ODE_HOME_INCLUDE_ODE} PATH )
	GET_FILENAME_COMPONENT( ODE_HOME ${ODE_HOME_INCLUDE} PATH )
	
ENDIF ( ODE_HOME_INCLUDE_ODE )
