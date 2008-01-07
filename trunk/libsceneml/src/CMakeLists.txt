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
				  ${MUPARSER_HOME}/lib	)
if (WIN32)
	link_directories( ${ODE_HOME}/lib/releaselib )
else (WIN32)
	link_directories( ${ODE_HOME}/lib )
endif (WIN32)

# Create target
add_library( ${TARGET_NAME}	STATIC ${SRCS})
if (WIN32)
	target_link_libraries( ${TARGET_NAME} ode xerces-c_2 muparser)
else (WIN32)
	target_link_libraries( ${TARGET_NAME} ode xerces-c muparser)
endif (WIN32)

install(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION ${PROJECT_BASE_DIR}/private/lib)
install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/private/include)
install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/private/include)