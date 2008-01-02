INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src/libs )

#LINK_LIBRARIES( matheval )
#ADD_EXECUTABLE( test_matheval test_matheval.cpp )

#LINK_LIBRARIES( xode )
#ADD_EXECUTABLE( test_build_scene test_build_scene.cpp )

#LINK_LIBRARIES( xode )
#ADD_EXECUTABLE( test_ogre_scene test_ogre_scene.cpp )

LINK_LIBRARIES( xode )
ADD_EXECUTABLE( test_collision test_collision.cpp )

LINK_LIBRARIES( xode )
ADD_EXECUTABLE( test_meshes test_meshes.cpp )
