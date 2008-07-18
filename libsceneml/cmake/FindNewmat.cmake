# Locate Newmat home

# This module defines
# NEWMAT_HOME where to find include, lib, bin, etc.
# NEWMAT_FOUND, If set to 0, don't try to use Newmat.

find_package(PkgConfig REQUIRED)
pkg_check_modules (NEWMAT REQUIRED newmat)
message( STATUS "Found Newmat." )
message( STATUS "	Include dirs: ${NEWMAT_INCLUDE_DIRS}" )
message( STATUS "	Library dirs: ${NEWMAT_LIBRARY_DIRS}" )
message( STATUS "	Libs: ${NEWMAT_LIBRARIES}" )
message( STATUS "	LDFlags: ${NEWMAT_LDFLAGS}" )
message( STATUS "	CFlags: ${NEWMAT_CFLAGS}" )