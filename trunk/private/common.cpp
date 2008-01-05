#include "common.h"

std::map<std::string, StringValue> g_MapStringValues;
bool g_bLibraryIsInit;

void InitLibrary(void)
{
	InitializeStringValueMap(); // Initialize string/switch map

	if ( mexAtExit(MexExitFcn) ) mexPrintf("Error! myExitFcn not registered properly");
	
	g_bLibraryIsInit = true;
}

void InitializeStringValueMap()
{
	g_MapStringValues["Init"] = evInit;
	g_MapStringValues["Update"] = evUpdate;
	g_MapStringValues["GetBody"] = evGetBody;
	g_MapStringValues["GetGeom"] = evGetGeom;
	g_MapStringValues["GetAllGeoms"] = evGetAllGeoms;
	g_MapStringValues["InCollision"] = evInCollision;
	g_MapStringValues["GetContactData"] = evGetContactData;
	g_MapStringValues["End"] = evEnd;
}

bool IsValidCommand(const std::string& cmd)
{
	std::map<std::string, StringValue>::iterator it = g_MapStringValues.find(cmd);
	if (it != g_MapStringValues.end()) return true;
	else return false;
}

std::string GetCommand(int nrhs, const mxArray* mcmd)
{
	// Check for minimum number of inputs
	if ( !(mxIsChar( mcmd )) ) {
		std::ostringstream msg;
		msg << __FUNCTION__ << ": First argument must specify the operation of the library.";
		mexErrMsgTxt(msg.str().c_str());
	}
	char* ccmd = mxArrayToString(mcmd);
	std::string scmd(ccmd);
	mxFree(ccmd);

	if (!IsValidCommand(scmd)) {
		std::ostringstream msg;
		msg << __FUNCTION__ << ": Command " << scmd << " does not exist.";
		mexErrMsgTxt(msg.str().c_str());
	}

	return scmd;
}
