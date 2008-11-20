enable_testing()
include_directories( ${PROJECT_SOURCE_DIR}/src
					 ${PROJECT_SOURCE_DIR}/include
					 ${CPPUNIT_INCLUDE_DIRS}
					 ${Boost_INCLUDE_DIRS}
					 ${MUPARSER_HOME}/include
					 ${LOG4CXX_INCLUDE_DIRS})
link_directories( ${CPPUNIT_LIBRARY_DIRS}
				  ${LOG4CXX_LIBRARY_DIRS}
                  ${Boost_LIBRARY_DIRS}
				  ${MUPARSER_HOME}/lib )
if ( WIN32 )
    link_libraries( libcppunit.dll.a )
else ( WIN32 )
    link_libraries( ${CPPUNIT_LIBRARIES} ${LOG4CXX_LIBRARIES})
endif ( WIN32 )


add_definitions("-DTINYSG_VERSION_MAJOR=${TINYSG_VERSION_MAJOR}")
add_definitions("-DTINYSG_VERSION_MINOR=${TINYSG_VERSION_MINOR}")
add_definitions("-DTINYSG_VERSION_PATCH=${TINYSG_VERSION_PATCH}")

file(GLOB UnitTests_SRCS "*Test.cpp" )
foreach(test ${UnitTests_SRCS})
    get_filename_component(TestName ${test} NAME_WE)
    #message("Adding test ${TestName}")
    include( ${TestName}.inc )
    if (build_test)
		set( srcs main.cpp ${test} ${test_srcs} )
		#message(STATUS "srcs = ${srcs}")
		add_executable(${TestName} ${srcs})
		if ( WIN32 )
		    target_link_libraries(${TestName} debug muparser_static_D optimized muparser_static)
		else ( WIN32 )
		    target_link_libraries(${TestName} muparser)
		endif ( WIN32 )
		add_test(${TestName} ${TestName}${CMAKE_EXECUTABLE_SUFFIX} )
    endif (build_test)
endforeach(test)

set( UNITTESTS_PASSED TRUE )