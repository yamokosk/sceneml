include_directories( ${Boost_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/src
					 ${NEWMAT_INCLUDE_DIRS}
					 ${MUPARSER_HOME}/include
					 ${ODE_HOME}/include)
link_directories( ${Boost_LIBRARY_DIRS}
				  ${NEWMAT_LIBRARY_DIRS}
				  ${MUPARSER_HOME}/lib
				  ${ODE_HOME}/lib )

# TEST ODE
file(GLOB SML_SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB MATH_SRCS ${PROJECT_SOURCE_DIR}/src/math/*.cpp)
file(GLOB ODE_SRCS ${PROJECT_SOURCE_DIR}/src/plugins/*.cpp)
set(srcs verify.cpp ${SML_SRCS} ${MATH_SRCS} ${ODE_SRCS})

if ( WIN32 )
    add_executable(test_ode ${srcs})
    target_link_libraries(test_ode debug muparser_static_D optimized muparser_static
                                  debug newmatD optimized newmat
                                  debug ode_static_D optimized ode_static)
else ( WIN32 )
    add_executable(test_ode ${srcs})
    target_link_libraries(test_ode muparser newmat ode)
endif ( WIN32 )