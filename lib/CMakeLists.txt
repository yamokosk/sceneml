set(TARGET_NAME "xode")
option( BUILD_${TARGET_NAME} "Try to build the ${TARGET_NAME} library" ON )
option( BUILD_${TARGET_NAME}_MEX "Try to build mex interface to ${TARGET_NAME}" ON)

if ( BUILD_${TARGET_NAME} )
	message( STATUS "_${TARGET_NAME}_" )
	set(DEPENDENCY_CHECK_XODE 1)
	
	# Dependency checks - Boost, ODE, matheval, OIS, Ogre, xerces
	include ( ${PROJECT_CMAKE_DIR}/FindODE.cmake )
	if (NOT ODE_FOUND)
		message( WARNING " ODE is missing.")
		set(DEPENDENCY_CHECK_XODE 0)
	endif (NOT ODE_FOUND)
	
	find_package(Boost REQUIRED)
	if (NOT Boost_FOUND)
		message( WARNING " Boost is missing." )
		set(DEPENDENCY_CHECK_XODE 0)
	endif (NOT Boost_FOUND)
	
	include( ${PROJECT_CMAKE_DIR}/FindXerces.cmake )
	if (NOT XERCES_FOUND)
		message( WARNING " Xerces is missing.")
		set(DEPENDENCY_CHECK_XODE 0)
	endif (NOT XERCES_FOUND)
	
	#include( ${PROJECT_CMAKE_DIR}/FindMathEval.cmake )
	#if (NOT MATHEVAL_FOUND)
	#	message( WARNING " MATHEVAL is missing.")
	#	set(DEPENDENCY_CHECK_XODE 0)
	#endif (NOT MATHEVAL_FOUND)
	
	include ( ${PROJECT_CMAKE_DIR}/FindNewmat.cmake )
	if (NOT NEWMAT_FOUND)
		message( WARNING " Newmat is missing.")
		set(DEPENDENCY_CHECK_XODE 0)
	endif (NOT NEWMAT_FOUND)
	
	# Finally check the OS.. we can't build this library in Windows yet
	if( OS_WIN )
		message( WARNING " XODE is currently only available for Linux")
		set(DEPENDENCY_CHECK_XODE 0)
	endif ( OS_WIN )
	
	if (NOT DEPENDENCY_CHECK_XODE)
		message( ERROR " ${TARGET_NAME} is missing a required dependency. Skipping it." )
	else (NOT DEPENDENCY_CHECK_XODE)
		# We have all of our dependencies... so build it!
		include_directories( ${PROJECT_SOURCE_DIR}/src/libs 
						 ${Boost_INCLUDE_DIRS}
						 ${ODE_HOME}/include
						 ${NEWMAT_HOME}/newmat
		)
		
		link_directories( ${ODE_HOME}/lib
					   ${NEWMAT_HOME}/lib
					   /usr/lib 
					   /usr/local/lib
		)
		
		set( SRCS	matrix.cpp
			transform.cpp
			sceneobjects.cpp 
			scene.cpp
			xml_attributes.cpp 
			xml_scene_builder.cpp
			Primitive.cpp
			OBJImport.cpp
			STLImport.cpp
		)
	
		global_add_library( ${TARGET_NAME} SHARED ${SRCS} )
		target_link_libraries( ${TARGET_NAME} newmat odegim xerces-c methods muparser)
				
		install_files( /include/${TARGET_NAME} .*\\.h$ )
	
		if ( BUILD_TESTS )
			add_subdirectory ( tests )
		endif ( BUILD_TESTS )
		
		#if (BUILD_${TARGET_NAME}_MEX)
		#	add_subdirectory ( mex )
		#endif (BUILD_${TARGET_NAME}_MEX)
		
	endif (NOT DEPENDENCY_CHECK_XODE)
endif ( BUILD_${TARGET_NAME} )
