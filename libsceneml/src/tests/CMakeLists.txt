set(UNIT_TESTS signal_test)

include_directories( ${Boost_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/src )
link_directories( ${Boost_INCLUDE_DIRS}/lib )
					 
foreach(test ${UNIT_TESTS})
	add_executable(${test} ${test}.cpp)
endforeach(test)
	