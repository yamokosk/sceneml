set( TARGET_NAME "sgode" )

add_subdirectory( unittest )

#if ( UNITTESTS_PASSED )

include_directories( ${PROJECT_SOURCE_DIR}/include
					 ${Boost_INCLUDE_DIRS}
					 ${LOG4CXX_INCLUDE_DIRS})
link_directories( ${LOG4CXX_LIBRARY_DIRS}
                  ${Boost_LIBRARY_DIRS} )
				  
file(GLOB PLUGIN_SOURCE "*.cpp")
file(GLOB PLUGIN_HEADERS "*.h")

add_library( ${TARGET_NAME} SHARED ${PLUGIN_SOURCE})

#set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_NAME_DIR "${PROJECT_BASE_DIR}/mex/lib")
target_link_libraries(${TARGET_NAME} ode ${LOG4CXX_LIBRARIES})


#install(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${PROJECT_BASE_DIR}/mex/lib)
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
	
#endif ( UNITTESTS_PAST )