{\rtf1\ansi\ansicpg1252\cocoartf949\cocoasubrtf330
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
\margl1440\margr1440\vieww9000\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\ql\qnatural\pardirnatural

\f0\fs24 \cf0 ##\
## AddUnitTest - Register specified project as a unit test\
##\
##   project        - the name of the project to register\
##\
MACRO( AddUnitTest project )\
	GET_TARGET_PROPERTY( testExecutable $\{project\} LOCATION )\
\
	STRING(REGEX REPLACE "\\\\$\\\\(.*\\\\)"\
"\\$\{CTEST_CONFIGURATION_TYPE\}"\
			testExecutable "$\{testExecutable\}")\
	ADD_TEST( $\{project\} $\{testExecutable\} )\
ENDMACRO( AddUnitTest )\
}