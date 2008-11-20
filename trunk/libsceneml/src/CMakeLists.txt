
set( TARGET_NAME "tinysg" )

include_directories( ${PROJECT_SOURCE_DIR}/include
					 ${Boost_INCLUDE_DIRS}
					 ${MUPARSER_HOME}/include
					 ${LOG4CXX_INCLUDE_DIRS})
link_directories( ${LOG4CXX_LIBRARY_DIRS}
                  ${Boost_LIBRARY_DIRS}
				  ${MUPARSER_HOME}/lib )
				  
# Gather all *.cpp and *.h files into their respective variables
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_definitions(-DTIXML_USE_STL=1 )
add_definitions("-DTINYSG_VERSION_MAJOR=${TINYSG_VERSION_MAJOR}")
add_definitions("-DTINYSG_VERSION_MINOR=${TINYSG_VERSION_MINOR}")
add_definitions("-DTINYSG_VERSION_PATCH=${TINYSG_VERSION_PATCH}")
add_library( ${TARGET_NAME} SHARED ${SRCS})

#set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_NAME_DIR "${PROJECT_BASE_DIR}/mex/lib")
target_link_libraries(${TARGET_NAME} muparser ${LOG4CXX_LIBRARIES})

#install(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${PROJECT_BASE_DIR}/mex/lib)
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)