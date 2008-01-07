# Gather all *.cpp and *.h files into their respective variables
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

# Set paths to dependencies
include_directories( ${ODE_HOME}/include
					 ${XERCES_HOME}/include
					 ${MUPARSER_HOME}/include
					 ${Boost_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/src )
link_directories( ${XERCES_HOME}/lib
				  ${ODE_HOME}/lib
				  ${MUPARSER_HOME}/lib	)

# Create target
add_library( ${TARGET_NAME}	STATIC ${SRCS})
if (WIN32)
	#add_definitions (-DXML_LIBRARY)
	target_link_libraries( ${TARGET_NAME} odeD xerces-c_2D muparser_static)
else (WIN32)
	target_link_libraries( ${TARGET_NAME} ode xerces-c muparser)
endif (WIN32)

install(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION ${PROJECT_BASE_DIR}/mex/lib)
install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)