/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License, 
 * or (at your option) any later version. The text of the GNU Lesser General 
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for 
 * more details.
 *
 *************************************************************************/
 
#ifndef _SCENEML_MEX_COMMAND_H_FILE_
#define _SCENEML_MEX_COMMAND_H_FILE_

/*
 *	The following defines commands available via the mex interface.
 */
#define COMMANDS_DEF COMMAND(LOADSCENE, "LoadScene")			\
                     COMMAND(UPDATE, "Update")					\
                     COMMAND(GETBODY, "GetBody")				\
					 COMMAND(GETGEOM, "GetGeom")				\
					 COMMAND(GETALLGEOMS, "GetAllGeoms")		\
					 COMMAND(INCOLLISION, "InCollision")		\
					 COMMAND(GETCONTACTDATA, "GetContactData")	\
					 COMMAND(SETPROXBODY, "SetProxBody")		\
					 COMMAND(GETVARNAMES, "GetVarNames")		\
					 COMMAND(GETSCENEAABB, "GetSceneAABB")
					 
enum command_indices {
#define	COMMAND(ID, name) COMMAND_##ID,
	COMMANDS_DEF
#undef COMMAND
    COMMAND_COUNT /* The number of existing commands */
};

/*
 * .. and their respective handling functions.
 */
void handler_LOADSCENE (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_UPDATE (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GETBODY (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GETGEOM (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GETBODY (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GETALLGEOMS (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_INCOLLISION (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GETCONTACTDATA (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_SETPROXBODY (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GETVARNAMES (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GETSCENEAABB (int, mxArray *plhs[], int, const mxArray *prhs[]);

typedef void (*command_handler_t)(int, mxArray *plhs[], int, const mxArray *prhs[]);

// Various utility functions
int getCommandID( const mxArray* );
void dMatrix4ToMxArray(double *MX, const dMatrix4 T);
void dVector3ToMxArray(double *MX, const dVector3 V);
void dGeomToMxArray(mxArray *array[], sceneml::Geom *obj);

#endif