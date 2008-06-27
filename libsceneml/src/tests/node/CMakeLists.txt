include_directories( ${Boost_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/src
					 ${NEWMAT_HOME}/include )
link_directories( ${Boost_INCLUDE_DIRS}/lib
				  ${NEWMAT_HOME}/lib )

# TEST CONSTANTS
set(name test_node)
set(srcs test_node.cpp
		 ${PROJECT_SOURCE_DIR}/src/smlNode.cpp
		 ${PROJECT_SOURCE_DIR}/src/smlSceneMgr.cpp
		 ${PROJECT_SOURCE_DIR}/src/smlException.cpp)
add_executable(${name} ${srcs})
if (WIN32)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${name} newmatD)
	else (CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${name} newmat)
	endif (CMAKE_BUILD_TYPE MATCHES Debug)
else (WIN32)
	target_link_libraries(${name} newmat)
endif (WIN32)
