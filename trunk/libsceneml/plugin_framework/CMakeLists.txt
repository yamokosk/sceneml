
set( TARGET_NAME "plugin_framework" )

include_directories( ${PROJECT_SOURCE_DIR}/include
					 ${Boost_INCLUDE_DIRS})
link_directories( ${Boost_LIBRARY_DIRS} )
				  
# Gather all *.cpp and *.h files into their respective variables
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")



add_library( ${TARGET_NAME} STATIC ${SRCS})

#set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_NAME_DIR "${PROJECT_BASE_DIR}/mex/lib")
#target_link_libraries(${TARGET_NAME} muparser)

#install(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${PROJECT_BASE_DIR}/mex/lib)
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)