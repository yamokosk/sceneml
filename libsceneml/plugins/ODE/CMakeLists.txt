set( TARGET_NAME "sgode" )

add_subdirectory( unittest )

if ( UNITTESTS_PASSED )

	file(GLOB PLUGIN_SOURCE "*.cpp")
	file(GLOB PLUGIN_HEADERS "*.h")
	
	add_library( ${TARGET_NAME} SHARED ${PLUGIN_SOURCE})
	
	#set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_NAME_DIR "${PROJECT_BASE_DIR}/mex/lib")
	if ( WIN32 )
        target_link_libraries(${TestName} debug odeD optimized ode)
    else ( WIN32 )
        target_link_libraries(${TestName} ode)
    endif ( WIN32 )
	
	install(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${PROJECT_BASE_DIR}/mex/lib)
	install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
	install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
	
endif ( UNITTESTS_PAST )