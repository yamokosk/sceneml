/*
 * Geom.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "Geom.h"

namespace smlode {

using namespace sml;

Geom::Geom() :
	SceneObject(),
	geomID_(NULL),
	alpha_(1)
{

}

Geom::Geom(const std::string& name) :
	SceneObject(name),
	geomID_(NULL),
	alpha_(1)
{

}

Geom::~Geom()
{
	if (geomID_)
		dGeomDestroy(geomID_);
}


void Geom::_notifyMoved(void)
{
	assert(geomID_ && parentNode_);

	if ( _getGeomClass() != dPlaneClass ) {
		ColumnVector pos = parentNode_->_getDerivedPosition();
		dGeomSetPosition(geomID_, (dReal)pos(1), (dReal)pos(2), (dReal)pos(3));

		Quaternion q = parentNode_->_getDerivedOrientation();
		dQuaternion dq = {0};
		dq[0] = (dReal)q.real();
		dq[1] = (dReal)q.R_component_2();
		dq[2] = (dReal)q.R_component_3();
		dq[3] = (dReal)q.R_component_4();
		dGeomSetQuaternion(geomID_, dq);
	}
}

//void Geom::_notifySpace(Space* space)
//{
	// Tell ODE Geom its part of a new space... is this necessary?
//}

ODEObjectFactory::ODEObjectFactory()
{

}

ODEObjectFactory::~ODEObjectFactory()
{

}

SceneObject* ODEObjectFactory::createInstanceImpl(const std::string& name, const PropertyCollection* params)
{
	Geom* g = new Geom( name );

	dGeomID geomID = NULL;

	// Construct the ODE object - based on type of course
	std::string type = params->getValue("type");

	if (!type.compare("box")) {
		sml::Real length = ExpressionFactory::getAsReal( params->getValue("length") );
		sml::Real width = ExpressionFactory::getAsReal( params->getValue("width") );
		sml::Real height = ExpressionFactory::getAsReal( params->getValue("height") );
		geomID = dCreateBox(NULL, (dReal)length, (dReal)width, (dReal)height);
	} else if (!type.compare("ccylinder")) {
		sml::Real length = ExpressionFactory::getAsReal( params->getValue("length") );
		sml::Real radius = ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateCCylinder(NULL, (dReal)radius, (dReal)length);
	} else if (!type.compare("cylinder")) {
		sml::Real length = ExpressionFactory::getAsReal( params->getValue("length") );
		sml::Real radius = ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateCylinder(NULL, (dReal)radius, (dReal)length);
	} else if (!type.compare("sphere")) {
		sml::Real radius = ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID = dCreateSphere(NULL, (dReal)radius);
	} else if (!type.compare("plane")) {
		sml::Real nx = ExpressionFactory::getAsReal( params->getValue("normal_x") );
		sml::Real ny = ExpressionFactory::getAsReal( params->getValue("normal_y") );
		sml::Real nz = ExpressionFactory::getAsReal( params->getValue("normal_z") );
		sml::Real d = ExpressionFactory::getAsReal( params->getValue("d") );
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
		geomID = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);*/
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
