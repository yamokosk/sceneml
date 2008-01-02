#include <xode/mex/common.h>

// xode includes
#include <xode/xode.h>

// Global scene pointer
std::auto_ptr<xode::Scene> g_scene;
extern std::map<std::string, StringValue> g_MapStringValues;
extern bool g_bLibraryIsInit;

#define NUM_BODY_FIELDS 6
const char *fnames_body[] = {"name", "proxBody", "distBodies", "geoms", "T_prox_body", "T_world_body"};
#define NUM_GEOM_FIELDS 6
const char *fnames_geom[] = {"name", "type", "body", "T_body_geom", "T_world_geom", "ghandle"};

#define _T(i,j) T[(i)*4+(j)]
#define _MX(i,j) MX[(j)*4+(i)]

void dMatrix4ToMxArray(double *MX, const dMatrix4 T)
{
	for (int r=0; r<4; ++r) {
		for (int c=0; c<4; ++c) _MX(r,c) = _T(r,c);
	}
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
				// XML scene description filename in prhs[1]
				char* cfname = mxArrayToString(prhs[1]);
				std::string filename(cfname);
				mxFree(cfname);
				
				xode::SceneDirector sceneDirector;
				xode::XMLSceneBuilder xmlSceneBuilder(filename);
				sceneDirector.SetSceneBuilder( &xmlSceneBuilder );
				sceneDirector.ConstructScene();
			
				g_scene = sceneDirector.GetScene();
				g_scene.get()->update();
				break;
			}
			case evUpdate:
			{
				if (!mxIsCell(prhs[1])) {
					std::ostringstream msg;
					msg << mexFunctionName() << "::" << __FUNCTION__ << "(): Second argument must be a cell array.";
					throw std::runtime_error(msg.str());
				}
				mwIndex subs[2], ind;
				xode::ValueList_t values;
				
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
					
					// Convert to library expected types
					values.push_back( xode::NameValue(name, mxGetScalar(value)) );
					
					// Clean up
					mxFree(name);
				}
				
				g_scene.get()->setMutableVars(values);
				g_scene.get()->update();
				
				break;
			}
			case evGetBody:
			{
				// Get xode object
				char* name = mxArrayToString(prhs[1]);
				xode::Body *obj = g_scene.get()->getBody(name);
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
					xode::Body *distBody = obj->getDistBody(ind);
					mxArray *str = mxCreateString( distBody->getName().c_str() );
					mxSetCell(tmp[2], ind, str);
				}
				
				for (int ind=0; ind < obj->getNumGeoms(); ++ind)
				{
					xode::Geom *geom = obj->getGeom(ind);
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
				// Get xode object
				char* name = mxArrayToString(prhs[1]);
				xode::Geom *obj = g_scene.get()->getGeom(name);
				mxFree(name);
				
				// Create tmp mxArray to collect all the outputs
				mxArray *tmp[NUM_GEOM_FIELDS];
				
				// Name
				tmp[0] = mxCreateString( obj->getName().c_str() );
				// Type
				tmp[1] = mxCreateDoubleScalar( obj->getType() );
				// Body
				tmp[2] = mxCreateString( obj->getProxObj()->getName().c_str() );
				// T_body_geom
				tmp[3] = mxCreateDoubleMatrix( 4, 4, mxREAL );
				// T_world_geom
				tmp[4] = mxCreateDoubleMatrix( 4, 4, mxREAL );
				// ghandle - I am not creating this graphics handle. Just a place holder...
				tmp[5] = mxCreateDoubleScalar( 0.0 );
				
				// Copy the matrix data from ODE into Matlab arrays
				dMatrix4 T; double *pMatrix = NULL; 
				
				// T_body_geom
				obj->getLocalTransformation(T);
				dMatrix4ToMxArray(mxGetPr(tmp[3]), T);
				
				obj->getGlobalTransformation(T);
				dMatrix4ToMxArray(mxGetPr(tmp[4]), T);
				
				// Create output structure for Matlab and populate
				plhs[0] = mxCreateStructMatrix(1, 1, NUM_GEOM_FIELDS, fnames_geom);
				for (int n=0; n < NUM_GEOM_FIELDS; ++n) {
					mxSetField(plhs[0], 0, fnames_geom[n], tmp[n]);	
				}
									
				break;
			}
			case evCollide:
			{
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
	mexWarnMsgTxt("mode library being unloaded.");
}
