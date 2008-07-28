#if (PERFORM_UNIT_TESTS)
	add_subdirectory(unittests)
	add_subdirectory(plugins)
#endif (PERFORM_UNIT_TESTS)

# Gather all *.cpp and *.h files into their respective variables
#file(GLOB SRCS "*.cpp")
#set(SRCS xml_scene_builder.cpp
#		 xml_attributes.cpp
#		 transform.cpp
#		 STLImport.cpp
#		 sceneobjects.cpp
#		 scene.cpp
#		 Primitive.cpp
#		 pose_estimation.cpp
#		 OBJImport.cpp
#		 matrix.cpp)
#file(GLOB HEADERS "*.h")
#set(HEADERS sceneml.h
#		 sceneobjects.h
#		 scene_builder.h
#		 scene.h
#		 Primitive.h
#		 pose_estimation.h
#		 MeshImport.h
#		 matrix.h
#		 config.h
#		 common.h
#		 transform.h
#		 xml_scene_builder.h
#		 xml_attributes.h)

# Set paths to dependencies
#if (APPLE)
#    include_directories( ${NEWMAT_HOME}/newmat )
#else (APPLE)
#    include_directories( ${NEWMAT_HOME}/include )
#endif (APPLE)
#
#include_directories( ${ODE_HOME}/include
#					 ${XERCES_HOME}/include
#					 ${MUPARSER_HOME}/include
#					 ${Boost_INCLUDE_DIRS}
#					 ${PROJECT_SOURCE_DIR}/src )
#link_directories( ${XERCES_HOME}/lib
# 			   ${ODE_HOME}/lib
# 			   ${MUPARSER_HOME}/lib
# 			   ${NEWMAT_HOME}/lib	)
#
## Create target
#if (BUILD_STATIC_LIB)
#	add_library( ${TARGET_NAME} STATIC ${SRCS})
#else (BUILD_STATIC_LIB)
#	add_library( ${TARGET_NAME} STATIC ${SRCS})
#endif (BUILD_STATIC_LIB)
#
#set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_NAME_DIR "${PROJECT_BASE_DIR}/mex/lib")
#target_link_libraries( ${TARGET_NAME} ${REQUIRED_LIBS})
#
#if (BUILD_STATIC_LIB)
#	install(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION ${PROJECT_BASE_DIR}/mex/lib)
#else (BUILD_STATIC_LIB)
#	install(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${PROJECT_BASE_DIR}/mex/lib)
#endif (BUILD_STATIC_LIB)
#
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)
#install(FILES ${HEADERS} DESTINATION ${PROJECT_BASE_DIR}/mex/include)