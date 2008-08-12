enable_testing()
include_directories( ${PROJECT_SOURCE_DIR}/src
					 ${CPPUNIT_INCLUDE_DIRS}
					 ${Boost_INCLUDE_DIRS}
					 ${NEWMAT_INCLUDE_DIRS}
					 ${MUPARSER_HOME}/include)
link_directories( ${CPPUNIT_LIBRARY_DIRS}
                  ${Boost_LIBRARY_DIRS}
				  ${NEWMAT_LIBRARY_DIRS}
				  ${MUPARSER_HOME}/lib )
if ( WIN32 )
    link_libraries( libcppunit.dll.a )
else ( WIN32 )
    link_libraries( ${CPPUNIT_LIBRARIES} )
endif ( WIN32 )

file(GLOB UnitTests_SRCS "*Test.cpp" )
foreach(test ${UnitTests_SRCS})
    get_filename_component(TestName ${test} NAME_WE)
    #message("Adding test ${TestName}")
    include( ${TestName}.inc )
    set( srcs main.cpp ${test} ${test_srcs} )
    #message(STATUS "srcs = ${srcs}")
    add_executable(${TestName} ${srcs})
    if ( WIN32 )
        target_link_libraries(${TestName} debug muparser_static_D optimized muparser_static
                                      debug newmatD optimized newmat)
    else ( WIN32 )
        target_link_libraries(${TestName} muparser newmat)
    endif ( WIN32 )
    add_test(${TestName} ${TestName}.exe)
endforeach(test)