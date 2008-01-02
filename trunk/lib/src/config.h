#ifndef _XODE_CONFIG_H_FILE_
#define _XODE_CONFIG_H_FILE_


#if defined(ODE_DLL) && defined(WIN32) && defined(_MSC_VER)
#define XODE_API __declspec( dllexport )
#elif !defined(ODE_DLL) && defined(WIN32) && defined(MSC_VER)
#define XODE_API __declspec( dllimport )
#else
#define XODE_API
#endif


#endif
