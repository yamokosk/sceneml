set(DEMO_PROGS test_meshes test_collision test_build_scene)
foreach( demo_prog ${DEMO_PROGS} )
	include_directories( ${ODE_HOME}/include
						${XERCES_HOME}/include
						${MUPARSER_HOME}/include
						${Boost_INCLUDE_DIRS}
						${PROJECT_SOURCE_DIR}/src )
	link_directories(${XERCES_HOME}/lib ${MUPARSER_HOME}/lib ${ODE_HOME}/lib ${PROJECT_SOURCE_DIR}/build/vs8/lib/debug)
	add_executable(${demo_prog} ${demo_prog}.cpp)
	target_link_libraries(${demo_prog} sceneml)
	install(PROGRAMS ${demo_prog}.exe DESTINATION ${PROJECT_BASE_DIR})
endforeach(demo_prog)