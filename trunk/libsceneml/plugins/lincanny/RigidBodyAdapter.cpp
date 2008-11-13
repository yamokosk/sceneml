/*
 * RigidBodyAdapter.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "RigidBodyAdapter.h"

namespace lincanny {

using namespace TinySG;
using namespace log4cxx;

RigidBodyAdapter::RigidBodyAdapter() :
	MovableObject(),
	geomID_(NULL),
	alpha_(1)
{
	rgb_[0] = 0.0;
	rgb_[1] = 0.0;
	rgb_[2] = 0.0;
}

RigidBodyAdapter::~RigidBodyAdapter()
{
	if (geomID_ != NULL) dGeomDestroy(geomID_);
}

Object* RigidBodyAdapter::clone() const
{
	return (new RigidBodyAdapter(*this));
}

void RigidBodyAdapter::notifyMoved(void)
{
	assert(geomID_ && parentNode_);

	ColumnVector pos = parentNode_->getDerivedPosition();
	Quaternion q = parentNode_->getDerivedOrientation();

	body_->setTransform(pos(1), pos(2), pos(3), tx, ty, tz);
}


//---------------------------------------------------------------------------------
LoggerPtr RigidBodyAdapterFactory::logger(Logger::getLogger("RigidBodyAdapterFactory"));

Object* RigidBodyAdapterFactory::createInstanceImpl(const PropertyCollection* params)
{
	if ( params == NULL )
	{
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "RigidBodyAdapter parameters were not specified.");
	}

	RigidBodyAdapter* g = new RigidBodyAdapter();

	dGeomID geomID = NULL;

	// Construct the ODE object - based on type of course
	std::string type = params->getValue("type");

	LOG4CXX_DEBUG(logger, "Got type: " << type);

	if (!type.compare("box")) {
		TinySG::Real length = ExpressionFactory::getAsReal( params->getValue("length") );
		TinySG::Real width = ExpressionFactory::getAsReal( params->getValue("width") );
		TinySG::Real height = ExpressionFactory::getAsReal( params->getValue("height") );
		geomID = dCreateBox(NULL, (dReal)length, (dReal)width, (dReal)height);
		LOG4CXX_DEBUG(logger, "Created a " << type);
	} else if (!type.compare("ccylinder")) {
		TinySG::Real length = ExpressionFactory::getAsReal( params->getValue("length") );
		TinySG::Real radius = ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateCCylinder(NULL, (dReal)radius, (dReal)length);
	} else if (!type.compare("cylinder")) {
		TinySG::Real length = ExpressionFactory::getAsReal( params->getValue("length") );
		TinySG::Real radius = ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateCylinder(NULL, (dReal)radius, (dReal)length);
	} else if (!type.compare("sphere")) {
		TinySG::Real radius = ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateSphere(NULL, (dReal)radius);
	} else if (!type.compare("plane")) {
		TinySG::Real nx = ExpressionFactory::getAsReal( params->getValue("normal_x") );
		TinySG::Real ny = ExpressionFactory::getAsReal( params->getValue("normal_y") );
		TinySG::Real nz = ExpressionFactory::getAsReal( params->getValue("normal_z") );
		TinySG::Real d = ExpressionFactory::getAsReal( params->getValue("d") );
		geomID = dCreatePlane(NULL, (dReal)nx, (dReal)ny, (dReal)nz, (dReal)d);
	} else if (!type.compare("mesh")) {
		// Got to make a TriMeshObj!.. tough one.
		/*std::string filename = params->getValue("filename");
		int pos = filename.find_last_of(".");
		std::string extension = filename.substr(pos+1);
		math::Real scale = math::ExpressionFactory::getAsReal( params->getValue("scale") );

		if (!extension.compare("obj")) {
			mesh.reset(new POLYHEDRON());
			mesh->filename = filename;
			if ( importOBJ(mesh.get(), scale) != 0 )
			{
				std::ostringstream msg;
				msg << "importOBJ(): Returned an error!" << std::endl;
				// TODO change to TinySG error
				throw std::runtime_error(msg.str());
			}
		} else if (!extension.compare("stl")) {
			mesh.reset(new POLYHEDRON());
			mesh->filename = filename;
			if ( importSTL(mesh.get(), scale) != 0 )
			{
				std::ostringstream msg;
				msg << "importOBJ(): Returned an error!" << std::endl;
				// TODO change to TinySG error
				throw std::runtime_error(msg.str());
			}
		} else {
			std::ostringstream msg;
			msg << extension << " is an unrecognized 3D model type. Currently only stl and obj files are supported." << std::endl;
			// TODO change to TinySG error
			throw std::runtime_error(msg.str());
		}

		dTriMeshDataID Data = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSingle(Data,
			(void*)mesh->vertices.get(), mesh->vertex_stride, mesh->vertex_count,
			(void*)mesh->indices.get(),mesh->index_count,mesh->index_stride);
		geomID = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);*/
	} else {
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "RigidBodyAdapter type " + type + " is an unknown type.");
	}

	if ( params->hasProperty("position") )
	{
		ColumnVector pos = ExpressionFactory::getAsVector( params->getValue("position"), 3 );
		dGeomSetPosition(geomID, pos(1), pos(2), pos(3));
	}
	//if (type.compare("plane")) dGeomSetBody(g, body->id());
	//dGeomSetCategoryBits(geomID_, 1);
	//dGeomSetCollideBits(geomID_, 1);

	g->geomID_ = geomID;
	return g;
}

void RigidBodyAdapterFactory::destroyInstance(Object* obj)
{
	RigidBodyAdapter* g = dynamic_cast<RigidBodyAdapter*>(obj);
	delete g;
}

}
