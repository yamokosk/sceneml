#ifndef _MEX_COMMON_H_FILE_
#define _MEX_COMMON_H_FILE_

#include <mex.h> // Required MATLAB includes
#include <string>
#include <sstream>
#include <map>
#include <stdexcept>

// defines
#define	MEX_LIBRARY_NAME	"SCENEML"
#define	SSWITCH( STRING )	switch( g_MapStringValues[STRING] )

enum StringValue { evNotDefined, evInit, evUpdate, evGetBody, evGetGeom, evGetAllGeoms, evInCollision, evGetContactData, evChangeProxBody, evEnd };

void InitializeStringValueMap();
void InitLibrary(void);
bool IsValidCommand(const std::string& cmd);
std::string GetCommand(int nrhs, const mxArray* mcmd);
void MexExitFcn(void);

#endif
