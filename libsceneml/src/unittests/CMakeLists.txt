include_directories( ${Boost_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/src
					 ${NEWMAT_INCLUDE_DIRS}
					 ${MUPARSER_HOME}/include)
link_directories( ${Boost_LIBRARY_DIRS}
				  ${NEWMAT_LIBRARY_DIRS}
				  ${MUPARSER_HOME}/lib )

if ( NOT WIN32 )
    link_libraries( boost_unit_test_framework)
endif ( NOT WIN32 )

# TEST PROPERTIES
set(name test_properties)
set(srcs ${name}.cpp
		 ${PROJECT_SOURCE_DIR}/src/PropertyCollection.cpp
		 ${PROJECT_SOURCE_DIR}/src/Exception.cpp)
if ( WIN32 )
    add_executable(unit${name} ${srcs})
else ( WIN32 )
    add_executable(unit${name} ${srcs})
endif ( WIN32 )


# TEST MATH EXPRESSION
set(name test_mathexpression)
set(srcs ${name}.cpp
		 ${PROJECT_SOURCE_DIR}/src/math/MathExpression.cpp
		 ${PROJECT_SOURCE_DIR}/src/Exception.cpp)
if ( WIN32 )
    add_executable(unit${name} ${srcs})
    target_link_libraries(unit${name} debug muparser_static_D optimized muparser_static
                                  debug newmatD optimized newmat)
else ( WIN32 )
    add_executable(unit${name} ${srcs})
    target_link_libraries(unit${name} muparser newmat)
endif ( WIN32 )

# TEST PROPERTIES
set(name test_nodevariable)
set(srcs ${name}.cpp
         ${PROJECT_SOURCE_DIR}/src/Root.cpp
		 ${PROJECT_SOURCE_DIR}/src/SceneMgr.cpp
		 ${PROJECT_SOURCE_DIR}/src/Node.cpp
		 ${PROJECT_SOURCE_DIR}/src/Variable.cpp
		 ${PROJECT_SOURCE_DIR}/src/PropertyCollection.cpp
		 ${PROJECT_SOURCE_DIR}/src/CommandStack.cpp
		 ${PROJECT_SOURCE_DIR}/src/math/Quaternion.cpp
		 ${PROJECT_SOURCE_DIR}/src/SceneObject.cpp
		 ${PROJECT_SOURCE_DIR}/src/Exception.cpp)
if ( WIN32 )
    add_executable(unit${name} ${srcs})
    target_link_libraries(unit${name} debug libmuparserD optimized libmuparser
                                  debug newmatD optimized newmat)
else ( WIN32 )
    add_executable(unit${name} ${srcs})
    target_link_libraries(unit${name} muparser newmat)
endif ( WIN32 )

# TEST OBSERVER
set(name test_observer)
set(srcs ${name}.cpp
		 ${PROJECT_SOURCE_DIR}/src/Exception.cpp)
if ( WIN32 )
    add_executable(unit${name} ${srcs})
else ( WIN32 )
    add_executable(unit${name} ${srcs})
endif ( WIN32 )

# TEST OBSERVER
set(name test_mathfactories)
set(srcs ${name}.cpp
         ${PROJECT_SOURCE_DIR}/src/math/Quaternion.cpp
		 ${PROJECT_SOURCE_DIR}/src/Exception.cpp)
if ( WIN32 )
    add_executable(unit${name} ${srcs})
    target_link_libraries(unit${name} debug newmatD optimized newmat)
else ( WIN32 )
    add_executable(unit${name} ${srcs})
    target_link_libraries(unit${name} newmat)
endif ( WIN32 )