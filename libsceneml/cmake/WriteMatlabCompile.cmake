###########################################################
#	Write the Matlab compile script to compile the MEX interface to SceneML
###########################################################
#INCLUDE( ${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake )
#INCLUDE( ${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake )

set( COMPILE_M ${PROJECT_BASE_DIR}/mex/compile.m )
set( SRCS	sceneml.cpp mex_common.cpp )

# Only write config.h once
if ( WROTE_COMPILE_M )
	message( STATUS "Not writing compile.m -- wrote it previously" )
else ( WROTE_COMPILE_M )  
	message( STATUS "Writing compile.m" )
	set( WROTE_COMPILE_M TRUE CACHE BOOL "Wrote compileScene.m" )

	file( WRITE ${COMPILE_M}	"% compile.m - Generated by CMakeLists.txt \n" )
	file( APPEND ${COMPILE_M}	"clear mex\n\n" )
	
	file( APPEND ${COMPILE_M}	"files = {")
	
	foreach( source_file ${SRCS} )
		file( APPEND ${COMPILE_M}	"'${source_file}', ")
	endforeach( source_file ${SRCS} )
	file( APPEND ${COMPILE_M}	"};\n")

	file( APPEND ${COMPILE_M}	"libname = '${TARGET_NAME}';\n")
	if (WIN32)
		file( APPEND ${COMPILE_M}	"% must change /MD to /ML in mexopts.bat\n")
		file( APPEND ${COMPILE_M}	"mexflags = '-O -DWIN32';\n")
		file( APPEND ${COMPILE_M}	"mexflags = [mexflags ' -Iinclude -I${ODE_HOME}/include -I${Boost_INCLUDE_DIRS} -I${XERCES_HOME}/include'];\n")
		file( APPEND ${COMPILE_M}	"mexflags = [mexflags ' -Llib -L${ODE_HOME}/lib/releaselib -L${MUPARSER_HOME}/lib -L${XERCES_HOME}/lib'];\n")
		file( APPEND ${COMPILE_M}	"mexflags = [mexflags sprintf(' -l%s -lode -lxerces-c_2 -lmuparser', libname)];\n")
	else (WIN32)
		file( APPEND ${COMPILE_M}	"mexflags = [mexflags '-O -I/include -L/lib -L${ODE_HOME}/lib'];\n")
		file( APPEND ${COMPILE_M}	"mexflags = [mexflags sprintf('-l%s -lode', libname)];\n")
	endif (WIN32)
	
	file( APPEND ${COMPILE_M}	"for file = files\n")
	file( APPEND ${COMPILE_M}	"\tmexflags = [mexflags ' ' char(file)];\n")
	file( APPEND ${COMPILE_M}	"end\n\n")
	
	file( APPEND ${COMPILE_M}	"mexstr = ['mex ' mexflags];\n")
	file( APPEND ${COMPILE_M}	"fprintf('%s\\n', mexstr);\n")
	file( APPEND ${COMPILE_M}	"eval(mexstr);\n\n")
	file( APPEND ${COMPILE_M}	"clear mexflags libname files;\n")
endif ( WROTE_COMPILE_M )  
