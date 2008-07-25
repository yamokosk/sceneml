/*
 * ODEGeom.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "ODEGeom.h"

namespace sml {

namespace ode {

ODEGeom::ODEGeom() :
	SceneObject(),
	geomID_(NULL),
	alpha_(1)
{

}

ODEGeom::ODEGeom(const std::string& name) :
	SceneObject(name),
	geomID_(NULL),
	alpha_(1)
{

}

ODEGeom::~ODEGeom()
{
	if (geomID_)
		dGeomDestroy(geomID_);
}


void ODEGeom::_notifyMoved(void)
{
	assert(geomID_ && parentNode_);

	if ( getType() != dPlaneClass ) {
		ColumnVector pos = parentNode_->_getDerivedPosition();
		dGeomSetPosition(geomID_, pos(1), pos(2), pos(3));

		Quaternion q = parentNode_->_getDerivedOrientation();
		dQuaternion dq = {0};
		dq[0] = q.real();
		dq[1] = q.R_component_2();
		dq[2] = q.R_component_3();
		dq[3] = q.R_component_4();
		dGeomSetQuaternion(geomID_, dq);
	}
}

//void ODEGeom::_notifySpace(Space* space)
//{
	// Tell ODE Geom its part of a new space... is this necessary?
//}

ODEObjectFactory::ODEObjectFactory()
{

}

ODEObjectFactory::~ODEObjectFactory()
{

}

SceneObject* ODEObjectFactory::createInstanceImpl(const std::string& name, const PropertyCollection* params = 0)
{
	ODEGeom* g = new ODEGeom( name );

	dGeomID geomID = NULL;

	// Construct the ODE object - based on type of course
	std::string type = params->getValue("type");

	if (!type.compare("box")) {
		float length = math::ExpressionFactory::getAsReal( params->getValue("length") );
		float width = math::ExpressionFactory::getAsReal( params->getValue("width") );
		float height = math::ExpressionFactory::getAsReal( params->getValue("height") );
		geomID = dCreateBox(NULL, length, width, height);
	} else if (!type.compare("ccylinder")) {
		float length = math::ExpressionFactory::getAsReal( params->getValue("length") );
		float radius = math::ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateCCylinder(NULL, radius, length);
	} else if (!type.compare("cylinder")) {
		float length = math::ExpressionFactory::getAsReal( params->getValue("length") );
		float radius = math::ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateCylinder(NULL, radius, length);
	} else if (!type.compare("sphere")) {
		float radius = math::ExpressionFactory::getAsReal( params->getValue("radius");
		geomID = dCreateSphere(NULL, radius);
	} else if (!type.compare("plane")) {
		float nx = math::ExpressionFactory::getAsReal( params->getValue("normal_x") );
		float ny = math::ExpressionFactory::getAsReal( params->getValue("normal_y") );
		float nz = math::ExpressionFactory::getAsReal( params->getValue("normal_z") );
		float d = params->getValue("d");
		geomID = dCreatePlane(NULL, nx, ny, nz, d);
	} else if (!type.compare("mesh")) {
		// Got to make a TriMeshObj!.. tough one.
		std::string filename = params->getValue("filename");
		int pos = filename.find_last_of(".");
		std::string extension = filename.substr(pos+1);
		float scale = math::ExpressionFactory::getAsReal( params->getValue("scale") );

		if (!extension.compare("obj")) {
			mesh.reset(new POLYHEDRON());
			mesh->filename = filename;
			if ( importOBJ(mesh.get(), scale) != 0 )
			{
				std::ostringstream msg;
				msg << "importOBJ(): Returned an error!" << std::endl;
				// TODO change to SML error
				throw std::runtime_error(msg.str());
			}
		} else if (!extension.compare("stl")) {
			mesh.reset(new POLYHEDRON());
			mesh->filename = filename;
			if ( importSTL(mesh.get(), scale) != 0 )
			{
				std::ostringstream msg;
				msg << "importOBJ(): Returned an error!" << std::endl;
				// TODO change to SML error
				throw std::runtime_error(msg.str());
			}
		} else {
			std::ostringstream msg;
			msg << extension << " is an unrecognized 3D model type. Currently only stl and obj files are supported." << std::endl;
			// TODO change to SML error
			throw std::runtime_error(msg.str());
		}

		dTriMeshDataID Data = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSingle(Data,
			(void*)mesh->vertices.get(), mesh->vertex_stride, mesh->vertex_count,
			(void*)mesh->indices.get(),mesh->index_count,mesh->index_stride);
		geomID = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);
	} else {
		std::ostringstream msg;
		msg << type << " is an unrecognized geom type. Currently only stl and obj files are supported." << std::endl;
		// TODO change to SML error
		throw std::runtime_error(msg.str());
	}

	//if (type.compare("plane")) dGeomSetBody(g, body->id());
	//dSpaceAdd(spaceID, geomID_);
	//dGeomSetCategoryBits(geomID_, 1);
	//dGeomSetCollideBits(geomID_, 1);

	g->_setGeomID(geomID);
	return g;
}

std::string ODEObjectFactory::getType(void) const
{
	return std::string("ODE");
}

void ODEObjectFactory::destroyInstance(SceneObject* obj)
{
	delete obj;
}

}

}
