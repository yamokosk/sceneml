#include "mex_common.h"

// sceneml includes
#include <sceneml.h>

// Global scene pointer
sceneml::ScenePtr g_scene;
extern std::map<std::string, StringValue> g_MapStringValues;
extern bool g_bLibraryIsInit;

#define NUM_BODY_FIELDS 6
const char *fnames_body[] = {"name", "proxBody", "distBodies", "geoms", "T_prox_body", "T_world_body"};
#define NUM_GEOM_FIELDS 9
const char *fnames_geom[] = {"name", "type", "body", "T_body_geom", "T_world_geom", "ghandle", "params", "fv", "color"};
#define NUM_GEOM_BOX_FIELDS 3
const char *fnames_geom_box[] = {"length", "width", "height"};
#define NUM_GEOM_SPHERE_FIELDS 1
const char *fnames_geom_sphere[] = {"radius"};
#define NUM_GEOM_CAPSULE_FIELDS 2
const char *fnames_geom_capsule[] = {"radius", "length"};
#define NUM_GEOM_CYLINDER_FIELDS 2
const char *fnames_geom_cylinder[] = {"radius", "length"};
#define NUM_GEOM_TRIMESH_FIELDS 1
const char *fnames_geom_trimesh[] = {"filename"};
#define NUM_GEOM_PLANE_FIELDS 4
const char *fnames_geom_plane[] = {"normal_x", "normal_y", "normal_z", "d"};
#define NUM_CONTACT_FIELDS 5
const char *fnames_contact[] = {"position", "normal", "depth", "geom1", "geom2"};
#define NUM_FV_FIELDS 2
const char *fnames_fv[] = {"faces", "vertices"};

#define MAX(x,y) ((x > y) ? x : y)
#define _T(i,j) T[(i)*4+(j)]
#define _MX(i,j) MX[(j)*4+(i)]

void dMatrix4ToMxArray(double *MX, const dMatrix4 T)
{
	for (int r=0; r<4; ++r) {
		for (int c=0; c<4; ++c) _MX(r,c) = _T(r,c);
	}
}

void dVector3ToMxArray(double *MX, const dVector3 V)
{
	for (int r=0; r<3; ++r) {
		//mexPrintf("V[%d] = %f\n", r, V[r]);
		MX[r] = V[r]; 
		//mexPrintf("MX[%d] = %f\n", r, MX[r]);
	}
}

void dGeomToMxArray(mxArray *array[], sceneml::Geom *obj)
{
	// Name
	array[0] = mxCreateString( obj->getName().c_str() );
	// Type
	array[1] = mxCreateDoubleScalar( obj->getType() );
	// Body
	array[2] = mxCreateString( obj->getProxObj()->getName().c_str() );
	// T_body_geom
	array[3] = mxCreateDoubleMatrix( 4, 4, mxREAL );
	// T_world_geom
	array[4] = mxCreateDoubleMatrix( 4, 4, mxREAL );
	// ghandle - I am not creating this graphics handle. Just a place holder...
	array[5] = mxCreateDoubleScalar( 0.0 );
	// params
	switch ( obj->getType() ) {
		case dSphereClass:
		{
			array[6] = mxCreateStructMatrix(1, 1, NUM_GEOM_SPHERE_FIELDS, fnames_geom_sphere);
			mxSetField(array[6], 0, "radius", mxCreateDoubleScalar( dGeomSphereGetRadius(obj->id()) ) );
			array[7] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dBoxClass:
		{
			array[6] = mxCreateStructMatrix(1, 1, NUM_GEOM_BOX_FIELDS, fnames_geom_box);
			dVector3 lengths;
			dGeomBoxGetLengths(obj->id(), lengths);
			for (int n=0; n < NUM_GEOM_BOX_FIELDS; ++n) 
				mxSetField(array[6], 0, fnames_geom_box[n], mxCreateDoubleScalar( lengths[n] ) );
			array[7] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dCapsuleClass:
		{
			array[6] = mxCreateStructMatrix(1, 1, NUM_GEOM_CAPSULE_FIELDS, fnames_geom_capsule);
			dReal length, radius;
			dGeomCapsuleGetParams(obj->id(), &radius, &length);
			mxSetField(array[6], 0, "length", mxCreateDoubleScalar( length ) );
			mxSetField(array[6], 0, "radius", mxCreateDoubleScalar( radius ) );
			array[7] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dCylinderClass:
		{
			array[6] = mxCreateStructMatrix(1, 1, NUM_GEOM_CYLINDER_FIELDS, fnames_geom_cylinder);
			dReal length, radius;
			dGeomCylinderGetParams(obj->id(), &radius, &length);
			mxSetField(array[6], 0, "length", mxCreateDoubleScalar( length ) );
			mxSetField(array[6], 0, "radius", mxCreateDoubleScalar( radius ) );
			array[7] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dPlaneClass:
		{
			array[6] = mxCreateStructMatrix(1, 1, NUM_GEOM_PLANE_FIELDS, fnames_geom_plane);
			dVector4 params;
			dGeomPlaneGetParams(obj->id(), params);
			for (int n=0; n < NUM_GEOM_PLANE_FIELDS; ++n) 
				mxSetField(array[6], 0, fnames_geom_plane[n], mxCreateDoubleScalar( params[n] ) );
			array[7] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dGeomTransformClass:
		{
			array[6] = mxCreateDoubleScalar( 0 );
			array[7] = mxCreateDoubleScalar( 0.0 );
			break;
		}
		case dTriMeshClass:
		{
			array[6] = mxCreateStructMatrix(1, 1, NUM_GEOM_TRIMESH_FIELDS, fnames_geom_trimesh);
			mxSetField(array[6], 0, "filename", mxCreateString( obj->getMesh()->filename.c_str()) );
			
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
			
			array[7] = mxCreateStructMatrix(1, 1, NUM_FV_FIELDS, fnames_fv);
			for (int n=0; n < NUM_FV_FIELDS; ++n) 
				mxSetField(array[7], 0, fnames_fv[n], tmp[n] );
			
			break;
		}
		default:
		{
			std::ostringstream msg;
			msg << mexFunctionName() << "::" << __FUNCTION__ << "(): Unknown geom type: " <<  obj->getType() << ".";
			throw std::runtime_error(msg.str());
		}					
	}
	
	// Color info
	array[8] = mxCreateDoubleMatrix( 1, 3, mxREAL );
	const dReal *rgb = obj->getColor();
	dVector3ToMxArray(mxGetPr(array[8]), rgb);
	
	// Copy the matrix data from ODE into Matlab arrays
	dMatrix4 T; double *pMatrix = NULL; 
	
	// T_body_geom
	obj->getLocalTransformation(T);
	dMatrix4ToMxArray(mxGetPr(array[3]), T);
	
	obj->getGlobalTransformation(T);
	dMatrix4ToMxArray(mxGetPr(array[4]), T);	
}

//! Entry point
/** @param nlhs number of LHS arguments.
  */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (!g_bLibraryIsInit) InitLibrary();
	
	// Get command type
	std::string type = GetCommand(nrhs, prhs[0]);
		
	try {
	
		SSWITCH( type )
		{
			case evInit:
			{
				if (g_scene.get() != NULL) g_scene.release();
				
				// XML scene description filename in prhs[1]
				char* cfname = mxArrayToString(prhs[1]);
                				
				sceneml::SceneDirector sceneDirector;
				sceneml::XMLSceneBuilder xmlSceneBuilder(cfname);
				sceneDirector.SetSceneBuilder( &xmlSceneBuilder );
				sceneDirector.ConstructScene();
			
				g_scene = sceneDirector.GetScene();
				g_scene->update();
				mxFree(cfname);
				break;
			}
			case evUpdate:
			{
				if (!mxIsCell(prhs[1])) {
					std::ostringstream msg;
					msg << mexFunctionName() << "::" << __FUNCTION__ << "(): Second argument must be a cell array.";
					throw std::runtime_error(msg.str());
				}
				int subs[2], ind;
				//sceneml::ValueList_t values;
				
				for (int r=0; r < mxGetM(prhs[1]); ++r) {
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
				
				break;
			}
			case evGetBody:
			{
				// Get sceneml object
				char* name = mxArrayToString(prhs[1]);
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
				plhs[0] = mxCreateStructMatrix(1, 1, NUM_BODY_FIELDS, fnames_body);
				for (int n=0; n < NUM_BODY_FIELDS; ++n) {
					mxSetField(plhs[0], 0, fnames_body[n], tmp[n]);	
				}
									
				break;
			}
			case evGetGeom:
			{
				// Get sceneml object
				char* name = mxArrayToString(prhs[1]);
				sceneml::Geom *obj = g_scene->getGeom(name);
				mxFree(name);
				
				// Create tmp mxArray to collect all the outputs
				mxArray *tmp[NUM_GEOM_FIELDS];
				dGeomToMxArray(tmp, obj);				
				
				// Create output structure for Matlab and populate
				plhs[0] = mxCreateStructMatrix(1, 1, NUM_GEOM_FIELDS, fnames_geom);
				for (int n=0; n < NUM_GEOM_FIELDS; ++n) {
					mxSetField(plhs[0], 0, fnames_geom[n], tmp[n]);	
				}
									
				break;
			}
			case evGetAllGeoms:
			{
				// Get sceneml/ode object
				sceneml::GeomPtrList_t allGeoms = g_scene->getAllGeoms();
				sceneml::GeomPtrList_t::iterator it = allGeoms.begin();
				
				// Create output structure for Matlab and populate
				//plhs[0] = mxCreateStructMatrix(allGeoms.size(), 1, NUM_GEOM_FIELDS, fnames_geom);
				plhs[0] = mxCreateCellMatrix( allGeoms.size(), 1 );
				
				for (int row=0; it != allGeoms.end(); ++row, ++it) {
					// Create tmp mxArray to collect all the outputs
					mxArray *tmp[NUM_GEOM_FIELDS];
					dGeomToMxArray(tmp, (*it).get());
					
					mxArray *structure = mxCreateStructMatrix(1, 1, NUM_GEOM_FIELDS, fnames_geom);
					for (int n=0; n < NUM_GEOM_FIELDS; ++n)
						mxSetField(structure, 0, fnames_geom[n], tmp[n]);
					
					mxSetCell(plhs[0], row, structure);	
				}	
				break;
			}
			case evInCollision:
			{
				plhs[0] = mxCreateLogicalScalar( g_scene->inCollision() );
				break;
			}
			case evGetContactData:
			{
				// Get sceneml/ode object
				sceneml::ContactGeoms_t contactData = g_scene->getContactData();

				// Create output structure for Matlab and populate
				plhs[0] = mxCreateStructMatrix(contactData.size(), 1, NUM_CONTACT_FIELDS, fnames_contact);
								
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
						mxSetField(plhs[0], row, fnames_contact[n], tmp[n]);	
				}
			}
			case evSetProxBody:
			{
				// Get body
				char* name = mxArrayToString(prhs[1]);
				sceneml::Body *body = g_scene->getBody(name);
				mxFree(name);
				
				// Get new parent
				char* parent = mxArrayToString(prhs[1]);
				sceneml::Body *new_parent = g_scene->getBody(parent);
				mxFree(parent);
				
				// Set new parent
				body->setProxObj(new_parent);
				
				// Last step.. instruct the library to update all objects global transformations
				g_scene->update();
				
				break;
			}
		}
	
	} catch (const std::runtime_error& e) {
		std::ostringstream msg;
		msg << e.what() << std::endl;
		msg << mexFunctionName() << "::" << __FUNCTION__ << "(): Failure during operation " << type;
		mexErrMsgTxt(msg.str().c_str());
	} catch (...) {
		std::ostringstream msg;
		msg << mexFunctionName() << "::" << __FUNCTION__ << "(): Unknown failure during operation " << type;
		mexErrMsgTxt(msg.str().c_str());
	}
	
	return;
}


void MexExitFcn(void)
{
	g_scene.release();
	mexWarnMsgTxt("mode library being unloaded.");
}
