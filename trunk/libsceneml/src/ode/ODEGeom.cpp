/*
 * ODEGeom.cpp
 *
 *  Created on: Jul 24, 2008
 *      Author: yamokosk
 */

#include "ODEGeom.h"

namespace sml {

namespace ode {

ODEGeom::ODEGeom() {

}

ODEGeom::ODEGeom(const std::string& name)
{

}

ODEGeom::~ODEGeom() {

}


void ODEGeom::_notifyMoved(void)
{
	assert(geomID_ && parentNode_);

	if ( getType() != dPlaneClass ) {
		Matrix T = parentNode_->_getFullTransform();

		dGeomSetPosition(geomID_, T(1,4), T(2,4)), T(3,4));
		//dGeomSetRotation(geomID_, rot);
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
	dGeomID geomID_ = NULL;

	// Construct the ODE object - based on type of course
	std::string type = params->getValue("type");

	if (!type.compare("box")) {
		float length = math::ExpressionFactory::getAsReal( params->getValue("length") );
		float width = math::ExpressionFactory::getAsReal( params->getValue("width") );
		float height = math::ExpressionFactory::getAsReal( params->getValue("height") );
		geomID_ = dCreateBox(NULL, length, width, height);
	} else if (!type.compare("ccylinder")) {
		float length = math::ExpressionFactory::getAsReal( params->getValue("length") );
		float radius = math::ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID_ = dCreateCCylinder(NULL, radius, length);
	} else if (!type.compare("cylinder")) {
		float length = math::ExpressionFactory::getAsReal( params->getValue("length") );
		float radius = math::ExpressionFactory::getAsReal( params->getValue("radius") );
		geomID_ = dCreateCylinder(NULL, radius, length);
	} else if (!type.compare("sphere")) {
		float radius = math::ExpressionFactory::getAsReal( params->getValue("radius");
		geomID_ = dCreateSphere(NULL, radius);
	} else if (!type.compare("plane")) {
		float nx = math::ExpressionFactory::getAsReal( params->getValue("normal_x") );
		float ny = math::ExpressionFactory::getAsReal( params->getValue("normal_y") );
		float nz = math::ExpressionFactory::getAsReal( params->getValue("normal_z") );
		float d = params->getValue("d");
		geomID_ = dCreatePlane(NULL, nx, ny, nz, d);
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
		geomID_ = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);
	} else {
		std::ostringstream msg;
		msg << type << " is an unrecognized geom type. Currently only stl and obj files are supported." << std::endl;
		// TODO change to SML error
		throw std::runtime_error(msg.str());
	}

	// At this point the geom has NOT been added to the correct space. First
	// we need to check if there is a transform between this geom and its body.
	// That effects who is added to what space.
	//TransformList_t tlist = parseTransform(thisGeomItem);
	CompositeTransformPtr pTransform(new CompositeTransform());
	parseTransform(thisGeomItem, pTransform.get());

	if (pTransform->size() > 0) {
		// There exists a transform between the body and geom.. therefore
		// we need a transform geom in ODE to accurately represent this.
		transID_ = dCreateGeomTransform(NULL);
		dGeomTransformSetGeom(transID_, geomID_);
		if (type.compare("plane")) dGeomSetBody(transID_, body->id());
			dSpaceAdd(spaceID, transID_);
		dGeomSetCategoryBits(transID_, 1);
		dGeomSetCollideBits(transID_, 1);
	} else {
		if (type.compare("plane")) dGeomSetBody(g, body->id());
		dSpaceAdd(spaceID, geomID_);
		dGeomSetCategoryBits(geomID_, 1);
		dGeomSetCollideBits(geomID_, 1);
	}

	ODEGeom* g = new ODEGeom("test");
	g->_setGeomID(geomID);
}

const std::string& ODEObjectFactory::getType(void) const
{

}

void ODEObjectFactory::destroyInstance(SceneObject* obj)
{

}

}

}
