/*
 * Geom.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "Geom.h"

namespace sgode {

using namespace TinySG;
using namespace log4cxx;

Geom::Geom() :
	MovableObject(),
	geomID_(NULL),
	alpha_(1)
{
	rgb_[0] = 0.0;
	rgb_[1] = 0.0;
	rgb_[2] = 0.0;
}

Geom::~Geom()
{
	if (geomID_ != NULL) dGeomDestroy(geomID_);
}

Object* Geom::clone() const
{
	return (new Geom(*this));
}

void Geom::notifyMoved(void)
{
	assert(geomID_ && parentNode_);

	if ( getGeomClass() != dPlaneClass ) {
		ColumnVector pos = parentNode_->getDerivedPosition();
		dGeomSetPosition(geomID_, (dReal)pos(1), (dReal)pos(2), (dReal)pos(3));

		Quaternion q = parentNode_->getDerivedOrientation();
		dQuaternion dq = {0};
		dq[0] = (dReal)q.real();
		dq[1] = (dReal)q.R_component_2();
		dq[2] = (dReal)q.R_component_3();
		dq[3] = (dReal)q.R_component_4();
		dGeomSetQuaternion(geomID_, dq);
	}
}


//---------------------------------------------------------------------------------
LoggerPtr GeomFactory::logger(Logger::getLogger("GeomFactory"));

Object* GeomFactory::createInstanceImpl(const PropertyCollection* params)
{
	if ( params == NULL )
	{
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "Geom parameters were not specified.");
	}

	Geom* g = new Geom();

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
		std::string filename = params->getValue("filename");
		int pos = filename.find_last_of(".");
		std::string extension = filename.substr(pos+1);
		TinySG::Real scale = math::ExpressionFactory::getAsReal( params->getValue("scale") );

		// Create a new TriMesh object
		g->mesh_.reset( new TinySG::TriMesh );

		if (!extension.compare("obj"))
			TinySG::importOBJ(filename, g->mesh_.get());
		else if (!extension.compare("stl")) {
			TinySG::importSTL(filename, g->mesh_.get());
		else {
			SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "Unsupported mesh file extension.");
		}

		dTriMeshDataID Data = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSingle(Data,
			(void*)mesh->vertexData, mesh->vertexStride(), mesh->numVertices(),
			(void*)mesh->faceData, mesh->numFaces(), mesh->faceStride());
		geomID = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);
	} else {
		SML_EXCEPT(TinySG::Exception::ERR_INVALIDPARAMS, "Geom type " + type + " is an unknown type.");
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

void GeomFactory::destroyInstance(Object* obj)
{
	Geom* g = dynamic_cast<Geom*>(obj);
	delete g;
}

}
