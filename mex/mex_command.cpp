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

#include "mex_common.h"
#include "mex_command.h"
#include "mex_error.h"
#include "mex_struct_def.h"


// Global scene pointer
extern sceneml::ScenePtr g_scene;

const char *command_names[] = {
#define	COMMAND(ID, name) name ,
	COMMANDS_DEF
#undef COMMAND
	NULL
};

command_handler_t command_handlers[] = {
#define COMMAND(ID, name) &handler_##ID ,
	COMMANDS_DEF
#undef COMMAND
    NULL
};

void handler_LOADSCENE (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}
	
	// Release old scene if we need to
	if (g_scene.get() != NULL) g_scene.release();
	
	// XML scene description filename in RHS_ARG_2
	char* cfname = mxArrayToString( RHS_ARG_2 );
	
	sceneml::SceneDirector sceneDirector;
	sceneml::XMLSceneBuilder xmlSceneBuilder(cfname);
	sceneDirector.SetSceneBuilder( &xmlSceneBuilder );
	sceneDirector.ConstructScene();
	
	g_scene = sceneDirector.GetScene();
	g_scene->update();
	mxFree(cfname);
	
	if ( nlhs > 1 )
	{
		dReal aabb[6] = {0};
		g_scene->computeSceneAABB(aabb);
	
		LHS_ARG_2 = mxCreateDoubleMatrix( 1, 6, mxREAL );
		double *ptr = mxGetPr(LHS_ARG_2);
	
		for (int n=0; n < 6; ++n) ptr[n] = aabb[n];
	}
}

void handler_UPDATE (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	if ( !mxIsCell( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a cell array.");
	}
	
	int subs[2], ind;
	for (int r=0; r < mxGetM( RHS_ARG_2 ); ++r) {
		// Get var name
		subs[0] = r; subs[1] = 0;
		ind = mxCalcSingleSubscript(prhs[1], 2, subs);
		mxArray *var = mxGetCell(prhs[1], ind);
		char* name = mxArrayToString(var);
		
		// Get var value
		subs[0] = r; subs[1] = 1;
		ind = mxCalcSingleSubscript(prhs[1], 2, subs);
		mxArray *value = mxGetCell(prhs[1], ind);
		int nvalues = MAX(mxGetM(value), mxGetN(value));					
		
		// Convert to library expected types
		if (nvalues == 3) {
			double *ptr = mxGetPr(value);
			g_scene->setMutableValue(name, ptr[0], ptr[1], ptr[2]);
		} else {
			g_scene->setMutableValue(name, (float)mxGetScalar(value) );
		}
		
		// Clean up
		mxFree(name);
	}
	
	g_scene->update();
}

void handler_GETBODY (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a character array.");
	}
	
	// Get sceneml object
	char* name = mxArrayToString( RHS_ARG_2 );
	sceneml::Body *obj = g_scene->getBody(name);
	mxFree(name);
	
	// Create tmp mxArray to collect all the outputs
	mxArray *tmp[NUM_BODY_FIELDS];
	
	// Name
	tmp[0] = mxCreateString( obj->getName().c_str() );
	// Prox body
	tmp[1] = mxCreateString( obj->getProxObj()->getName().c_str() );
	// Dist bodies
	tmp[2] = mxCreateCellMatrix( obj->getNumDistBodies(), 1 );
	// Geoms
	tmp[3] = mxCreateCellMatrix( obj->getNumGeoms(), 1 );
	// T_prox_body
	tmp[4] = mxCreateDoubleMatrix( 4, 4, mxREAL );
	// T_world_body
	tmp[5] = mxCreateDoubleMatrix( 4, 4, mxREAL );
	
	// Fill up the Cell arrays
	for (int ind=0; ind < obj->getNumDistBodies(); ++ind)
	{
		sceneml::Body *distBody = obj->getDistBody(ind);
		mxArray *str = mxCreateString( distBody->getName().c_str() );
		mxSetCell(tmp[2], ind, str);
	}
	
	for (int ind=0; ind < obj->getNumGeoms(); ++ind)
	{
		sceneml::Geom *geom = obj->getGeom(ind);
		mxArray *str = mxCreateString( geom->getName().c_str() );
		mxSetCell(tmp[3], ind, str);
	}
	
	// Copy the matrix data from ODE into Matlab arrays
	dMatrix4 T; double *pMatrix = NULL; 
	
	// T_prox_body
	obj->getLocalTransformation(T);
	dMatrix4ToMxArray(mxGetPr(tmp[4]), T);
	
	// T_world_body
	obj->getGlobalTransformation(T);
	dMatrix4ToMxArray(mxGetPr(tmp[5]), T);
	
	// Create output structure for Matlab and populate
	LHS_ARG_1 = mxCreateStructMatrix(1, 1, NUM_BODY_FIELDS, fnames_body);
	for (int n=0; n < NUM_BODY_FIELDS; ++n) {
		mxSetField(plhs[0], 0, fnames_body[n], tmp[n]);	
	}
}

void handler_GETGEOM (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a character array.");
	}
	
	// Get sceneml object
	char* name = mxArrayToString( RHS_ARG_2 );
	sceneml::Geom *obj = g_scene->getGeom(name);
	mxFree(name);
	
	// Create tmp mxArray to collect all the outputs
	mxArray *tmp[NUM_GEOM_FIELDS];
	dGeomToMxArray(tmp, obj);				
	
	// Create output structure for Matlab and populate
	LHS_ARG_1 = mxCreateStructMatrix(1, 1, NUM_GEOM_FIELDS, fnames_geom);
	for (int n=0; n < NUM_GEOM_FIELDS; ++n) {
		mxSetField(LHS_ARG_1, 0, fnames_geom[n], tmp[n]);	
	}
}

void handler_GETALLGEOMS (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	// Get sceneml/ode object
	sceneml::GeomPtrList_t allGeoms = g_scene->getAllGeoms();
	sceneml::GeomPtrList_t::iterator it = allGeoms.begin();
	
	// Create output structure for Matlab and populate
	LHS_ARG_1 = mxCreateCellMatrix( allGeoms.size(), 1 );
	
	for (int row=0; it != allGeoms.end(); ++row, ++it) {
		// Create tmp mxArray to collect all the outputs
		mxArray *tmp[NUM_GEOM_FIELDS];
		dGeomToMxArray(tmp, (*it).get());
		
		mxArray *structure = mxCreateStructMatrix(1, 1, NUM_GEOM_FIELDS, fnames_geom);
		for (int n=0; n < NUM_GEOM_FIELDS; ++n)
		mxSetField(structure, 0, fnames_geom[n], tmp[n]);
		
		mxSetCell(LHS_ARG_1, row, structure);	
	}
}

void handler_INCOLLISION (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	LHS_ARG_1 = mxCreateLogicalScalar( g_scene->inCollision() );
}

void handler_GETCONTACTDATA (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	// Get sceneml/ode object
	sceneml::ContactGeoms_t contactData = g_scene->getContactData();

	// Create output structure for Matlab and populate
	LHS_ARG_1 = mxCreateStructMatrix(contactData.size(), 1, NUM_CONTACT_FIELDS, fnames_contact);
	
	for (int row = 0; row < contactData.size(); ++row) {
		// Create tmp mxArray to collect all the outputs
		mxArray *tmp[NUM_CONTACT_FIELDS];
		
		// Position
		tmp[0] = mxCreateDoubleMatrix( 3, 1, mxREAL );
		// Normal
		tmp[1] = mxCreateDoubleMatrix( 3, 1, mxREAL );
		// Depth
		tmp[2] = mxCreateDoubleScalar( contactData[row].depth );
		// Geom #1
		tmp[3] = mxCreateString( g_scene->getGeomByID( contactData[row].g1 ).c_str() );
		// Geom #2
		tmp[4] = mxCreateString( g_scene->getGeomByID( contactData[row].g2 ).c_str() );
		
		// Copy the vector data into the appropriate Matlab containers
		dVector3ToMxArray( mxGetPr(tmp[0]), contactData[row].pos );
		dVector3ToMxArray( mxGetPr(tmp[1]), contactData[row].normal );
		
		for (int n=0; n < NUM_CONTACT_FIELDS; ++n)
			mxSetField(LHS_ARG_1, row, fnames_contact[n], tmp[n]);	
	}
}

void handler_SETPROXBODY (int nlhs, mxArray *plhs[], int rhs, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a character array.");
	}
	
	if ( !mxIsChar( RHS_ARG_3 ) ) {
		ERROR_MSG(INVALID_ARG, "Third argument must be a character array.");
	}
	
	// Get body
	char* name = mxArrayToString( RHS_ARG_2 );
	sceneml::Body *body = g_scene->getBody(name);
	mxFree(name);
	
	// Get new parent
	char* parent = mxArrayToString( RHS_ARG_3 );
	sceneml::Body *new_parent = g_scene->getBody(parent);
	mxFree(parent);
	
	// Set new parent
	body->setProxObj(new_parent);
	
	// Last step.. instruct the library to update all objects global transformations
	g_scene->update();
}

void handler_GETVARNAMES (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	std::list< std::string > varnames = g_scene->getVarNames();
	LHS_ARG_1 = mxCreateCellMatrix(varnames.size(), 1);
	
	std::list< std::string >::iterator it; int index = 0;
	for (it = varnames.begin(); it != varnames.end(); ++it, ++index)
		mxSetCell(LHS_ARG_1, index, mxCreateString( (*it).c_str() ));
}

void handler_GETSCENEAABB (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	dReal aabb[6] = {0};
	g_scene->computeSceneAABB(aabb);
	
	LHS_ARG_1 = mxCreateDoubleMatrix( 1, 6, mxREAL );
	double *ptr = mxGetPr(LHS_ARG_1);
	
	for (int n=0; n < 6; ++n) ptr[n] = aabb[n];
}


int getCommandID( const mxArray *arg )
{
	if ( !mxIsChar( arg ) ) {
		ERROR_MSG(INVALID_ARG, "First argument must be the command type (char array).");
	}
	
	char* cmdname = mxArrayToString( arg );
	
	for (int n=0; n < COMMAND_COUNT; ++n)
	{
		if ( strcmp(cmdname, command_names[n]) == 0 ) {
			mxFree(cmdname);
			return n;
		}
	}
	
	mxFree(cmdname);
	ERROR_MSG(INVALID_ARG, "Command not found.");
}

#define _dT(i,j) dT[(i)*4+(j)]
#define _MX(i,j) MX[(j)*4+(i)]
void dMatrix4ToMxArray(double *MX, const dMatrix4 dT)
{
	for (int r=0; r<4; ++r) {
		for (int c=0; c<4; ++c) _MX(r,c) = _dT(r,c);
	}
}

void dVector3ToMxArray(double *MX, const dVector3 V)
{
	for (int r=0; r<3; ++r) {
		MX[r] = V[r]; 
	}
}

void dGeomToMxArray(mxArray *array[], sceneml::Geom *obj)
{
	// Name
	array[GEOM_NAME] = mxCreateString( obj->getName().c_str() );
	// Type
	array[GEOM_TYPE] = mxCreateDoubleScalar( obj->getType() );
	// Body
	array[GEOM_BODY] = mxCreateString( obj->getProxObj()->getName().c_str() );
	// T_body_geom
	array[GEOM_TBODYGEOM] = mxCreateDoubleMatrix( 4, 4, mxREAL );
	// T_world_geom
	array[GEOM_TWORLDGEOM] = mxCreateDoubleMatrix( 4, 4, mxREAL );
	// ghandle - I am not creating this graphics handle. Just a place holder...
	array[GEOM_GHANDLE] = mxCreateDoubleScalar( 0.0 );
	// params
	switch ( obj->getType() ) {
		case dSphereClass:
		{
			array[GEOM_PARAMS] = mxCreateStructMatrix(1, 1, NUM_GEOM_SPHERE_FIELDS, fnames_geom_sphere);
			mxSetField(array[GEOM_PARAMS], 0, fnames_geom_sphere[GEOM_SPHERE_RADIUS], 
				mxCreateDoubleScalar( dGeomSphereGetRadius(obj->id()) ) );
			array[GEOM_FV] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dBoxClass:
		{
			array[GEOM_PARAMS] = mxCreateStructMatrix(1, 1, NUM_GEOM_BOX_FIELDS, fnames_geom_box);
			dVector3 lengths;
			dGeomBoxGetLengths(obj->id(), lengths);
			for (int n=0; n < NUM_GEOM_BOX_FIELDS; ++n) 
				mxSetField(array[GEOM_PARAMS], 0, fnames_geom_box[n], mxCreateDoubleScalar( lengths[n] ) );
			array[GEOM_FV] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dCapsuleClass:
		{
			array[GEOM_PARAMS] = mxCreateStructMatrix(1, 1, NUM_GEOM_CAPSULE_FIELDS, fnames_geom_capsule);
			dReal length, radius;
			dGeomCapsuleGetParams(obj->id(), &radius, &length);
			mxSetField(array[GEOM_PARAMS], 0, fnames_geom_capsule[GEOM_CAPSULE_LENGTH], 
				mxCreateDoubleScalar( length ) );
			mxSetField(array[GEOM_PARAMS], 0, fnames_geom_capsule[GEOM_CAPSULE_RADIUS], 
				mxCreateDoubleScalar( radius ) );
			array[GEOM_FV] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dCylinderClass:
		{
			array[GEOM_PARAMS] = mxCreateStructMatrix(1, 1, NUM_GEOM_CYLINDER_FIELDS, fnames_geom_cylinder);
			dReal length, radius;
			dGeomCylinderGetParams(obj->id(), &radius, &length);
			mxSetField(array[GEOM_PARAMS], 0, fnames_geom_cylinder[GEOM_CYLINDER_LENGTH],
				mxCreateDoubleScalar( length ) );
			mxSetField(array[GEOM_PARAMS], 0, fnames_geom_cylinder[GEOM_CYLINDER_RADIUS], 
				mxCreateDoubleScalar( radius ) );
			array[GEOM_FV] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dPlaneClass:
		{
			array[GEOM_PARAMS] = mxCreateStructMatrix(1, 1, NUM_GEOM_PLANE_FIELDS, fnames_geom_plane);
			dVector4 params;
			dGeomPlaneGetParams(obj->id(), params);
			for (int n=0; n < NUM_GEOM_PLANE_FIELDS; ++n) 
				mxSetField(array[GEOM_PARAMS], 0, fnames_geom_plane[n], mxCreateDoubleScalar( params[n] ) );
			array[GEOM_FV] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dGeomTransformClass:
		{
			array[GEOM_PARAMS] = mxCreateDoubleScalar( 0 );
			array[GEOM_FV] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dTriMeshClass:
		{
			array[GEOM_PARAMS] = mxCreateStructMatrix(1, 1, NUM_GEOM_TRIMESH_FIELDS, fnames_geom_trimesh);
			mxSetField(array[GEOM_PARAMS], 0, fnames_geom_trimesh[GEOM_TRIMESH_FILENAME], 
				mxCreateString( obj->getMesh()->filename.c_str()) );
			
			// Get number of faces from Polyhedron object
			int nfaces = obj->getMesh()->index_count;
			int nverts = obj->getMesh()->vertex_count;
					
			// And create storage for face/vertex data
			mxArray *tmp[2];
			
			// Get mesh data using dGeomTriMeshGetTriangle()
			tmp[0] = mxCreateDoubleMatrix(nfaces, 3, mxREAL); // Face index storage
			tmp[1] = mxCreateDoubleMatrix(nfaces*3, 3, mxREAL); // Vertex storage
			double *pFace = mxGetPr(tmp[0]);
			double *pVertex = mxGetPr(tmp[1]);

			for (int n=0; n < nfaces; ++n) {
				dVector3 v0, v1, v2;
				dGeomTriMeshGetTriangle(obj->id(), n, &v0, &v1, &v2);

				for (int i=0; i < 3; ++i) {
					pVertex[n*3 + 0 + i*nfaces*3] = v0[i];
					pVertex[n*3 + 1 + i*nfaces*3] = v1[i];
					pVertex[n*3 + 2 + i*nfaces*3] = v2[i];
					pFace[n + i*nfaces] = n*3 + i + 1;
				}
			}
			
			array[GEOM_FV] = mxCreateStructMatrix(1, 1, NUM_FV_FIELDS, fnames_fv);
			for (int n=0; n < NUM_FV_FIELDS; ++n) 
				mxSetField(array[GEOM_FV], 0, fnames_fv[n], tmp[n] );
			
			break;
		}
		default:
		{
			std::ostringstream msg;
			msg << "Runtime error: Unknown geom type - " <<  obj->getType() << ".";
			ERROR_MSG( RUNTIME_ERROR, msg.str().c_str() );
		}					
	}
	
	// Color info
	array[GEOM_COLOR] = mxCreateDoubleMatrix( 1, 3, mxREAL );
	const dReal *rgb = obj->getColor();
	dVector3ToMxArray(mxGetPr(array[8]), rgb);
	
	// Copy the matrix data from ODE into Matlab arrays
	dMatrix4 T; double *pMatrix = NULL; 
	
	// T_body_geom
	obj->getLocalTransformation(T);
	dMatrix4ToMxArray(mxGetPr(array[GEOM_TBODYGEOM]), T);
	
	obj->getGlobalTransformation(T);
	dMatrix4ToMxArray(mxGetPr(array[GEOM_TWORLDGEOM]), T);	
}
