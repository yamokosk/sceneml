set(UNIT_TESTS signal_test variable_test)

include_directories( ${Boost_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/src
					 ${NEWMAT_HOME}/include )
link_directories( ${Boost_INCLUDE_DIRS}/lib
				  ${NEWMAT_HOME}/lib )

foreach(test ${UNIT_TESTS})
	add_executable(${test} ${test}.cpp)
	target_link_libraries(${test} newmat)
endforeach(test)
	