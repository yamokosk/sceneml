include_directories( ${Boost_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/src
					 ${NEWMAT_INCLUDE_DIRS})
link_directories( ${Boost_INCLUDE_DIRS}/lib
				  ${NEWMAT_LIBRARY_DIRS} )
link_libraries( ${NEWMAT_LIBRARIES} boost_unit_test_framework)

# TEST PROPERTIES
set(name test_properties)
set(srcs ${name}.cpp
		 ${PROJECT_SOURCE_DIR}/src/PropertyCollection.cpp
		 ${PROJECT_SOURCE_DIR}/src/Exception.cpp)
add_executable(${name} ${srcs})
