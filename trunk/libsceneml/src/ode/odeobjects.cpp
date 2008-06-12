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

#ifndef _ODE_OBJECTS_H_FILE_
#define _ODE_OBJECTS_H_FILE_

#include "odeobjects.h"

namespace smlode {

/**************************************************************************
 *	Geom implementation
 **************************************************************************/
class GeomImpl
{
public:
	GeomImpl(const sml::PropertyCollection& props);
	virtual ~GeomImpl();
	
	//! Returns the objects position in world coordinates
	ReturnMatrix globalPosition() const;
	//! Set the objects position in world coordinates
	void globalPosition(const ColumnVector& pos);
	//! Returns the objects rotation matrix in world coordinates
	ReturnMatrix globalRotation() const;
	//! Set the objects rotation in world coordinates
	void globalRotation(const Matrix& rot);
	//! Returns the objects transformation matrix in world coordinates
	ReturnMatrix globalTransformation() const;
	//! Notify this object that its world pose is invalid
	void invalidate();
	//! Instruct object to re-validate its world pose
	void validate();
	
private:
	sml::PropertyCollection properties_;
	
	void constructODEObject();
	
	//! Get the geom type according to ODE
	int getType() const;
	dBodyID getBody() const;
	/** Returns the ODE pointer associated with this object. */
	dGeomID id() const {return geomID_;}
	/** Returns the transform ODE geom object associated with this object.
	  * Note that it is possible for this pointer to be NULL. */
	dGeomID tid() const {return transID_;}
	void setMesh(PolyhedronPtr p) {mesh_ = p;}
	const POLYHEDRON* getMesh() const {return mesh_.get();}
	void setColor(dReal r, dReal g, dReal b) {rgb_[0] = r; rgb_[0] = g; rgb_[0] = b;}
	void setColor(dVector3 rgb) { for(int n=0; n<3; ++n) rgb_[n] = rgb[n]; }
	const dReal* getColor() const {return rgb_;}
	void setAlpha(const dReal alpha) {alpha_ = alpha;}
	dReal getAlpha() const {return alpha_;}
	void setCollisionCheck( const int check ) { 
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		dGeomSetCategoryBits(gid, check); dGeomSetCollideBits(gid, check); 
	}
	int getCollisionCheck() const {
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		return (int)dGeomGetCollideBits(gid);
	}

	void computeLocalTransform();

	//! ODE object pointer
	dGeomID geomID_;
	//! ODE object pointer
	dGeomID transID_;
	//! Geom mesh data
	PolyhedronPtr mesh_; // Owner of this pointer
	//! Geom color
	dVector3 rgb_;
	//! Geom alpha (transperancy)
	dReal alpha_;
};

GeomImpl::GeomImpl(const sml::PropertyCollection& props) :
	geomID_(NULL),
	transID_(NULL)
{
	// Check properties
	//props.getValue("");

	this->constructODEObject();
}

//! Basic destructor
GeomImpl::~GeomImpl() 
{
	//if (mesh_ != NULL) delete mesh_;
	
	//dGeomDestroy(id());
	//if (tid()) dGeomDestroy(tid());
}

dBodyID GeomImpl::getBody() const
{
	if (getType() != dPlaneClass) {
		if (tid() != NULL)
			return dGeomGetBody(tid());
		else
			return dGeomGetBody(id());
	} else return NULL;
}

int GeomImpl::getType() const
{
	return dGeomGetClass(geomID_);
}

/** Compute the local transformation.
  * This function is overloaded from its base class. ODE is keeping track of
  * each objects global position but when we have a transformed geom, we need
  * to tell ODE about changes in the transformation between the regular and
  * transformed geom. */
void GeomImpl::computeLocalTransform()
{
	this->SceneObjectBase::computeLocalTransform();
	
	if (tid() != NULL)
	{
		dMatrix3 rot;
		this->getLocalRotation(rot);
		dGeomSetRotation(id(), rot);
		
		dVector3 pos;
		this->getLocalPosition(pos);
		dGeomSetPosition(id(), pos[0], pos[1], pos[2]);
	}
}

ReturnMatrix GeomImpl::globalPosition() const
{
	dVector3 pos = {0.0};
	
	if (getType() != dPlaneClass) {
		if (tid() == NULL) {
			// Transform ID does not exist so just return the real geoms position
			pos = dGeomGetPosition(id());
		} else {
			// Need to do a little math...
			//	P_wcs_geomID_ = R_wcs_t * P_t_tg + P_wcs_t;
			memcpy(pos, dGeomGetPosition( tid() ), sizeof(dVector3));
			dMULTIPLYADD0_331(pos, dGeomGetRotation(tid()), dGeomGetPosition(id()));
		}
	}
	
	ColumnVector ret(3) = 0.0;
	ret << pos;
	ret.release();
	return ret;
}

void GeomImpl::globalPosition(const ColumnVector& pos)
{
	if (getType() != dPlaneClass) {
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		dGeomSetPosition(gid, pos(0), pos(2), pos(3));
	}
}

ReturnMatrix GeomImpl::globalRotation() const
{
	dMatrix3 rot = {0.0};
	dRSetIdentity(rot);
	
	if (getType() != dPlaneClass) {
		if (tid() == NULL) {
			// Transform ID does not exist so just return the real geoms position
			memcpy(rot, dGeomGetRotation(id()), sizeof(dMatrix3));
		} else {
			// Need to do a little math...
			//	R_wcs_geomID_ = R_wcs_t * R_t_g;
			dMULTIPLY0_333(rot, dGeomGetRotation(tid()), dGeomGetRotation(id()) );
		}
	}
	
	Matrix ret = IdentityMatrix(3);
	ret << rot;
	ret.release();
	return ret;
}

void GeomImpl::globalRotation(const Matrix& rot)
{
	if (getType() != dPlaneClass) {
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		dGeomSetRotation(gid, rot);
	}
}

ReturnMatrix GeomImpl::globalTransformation() const
{
	dMatrix4 T; dTSetIdentity(T);
	
	if (getType() != dPlaneClass) {
		dMatrix3 rot; dVector3 pos;
		getGlobalRotation(rot);
		getGlobalPosition(pos);
		dTFromRAndPos(T, rot, pos);
	}
	
	Matrix ret = IdentityMatrix(4);
	ret << rot;
	ret.release();
	return ret;
}

void GeomImpl::invalidate()
{
	validWorldPose_ = false;
}

void GeomImpl::validate()
{
	if ( getPoseValidity() || getType() == dPlaneClass ) {
		// My world pose has not been invalidated by anyone, therefore there 
		// is nothing to do and I can just return.
		return;
	} else {
		// Someone has invalidated my local transformation, therefore I need
		// to compute a new one, then update my world transformation by asking
		// my prox body for his.
		computeLocalTransform(); // Should only ever have to do this once!
		validWorldPose_ = true; // Don't forget to set this!
	}
}

void GeomImpl::constructODEObject()
{
	// Construct the ODE object - based on type of course
	std::string type = props.getValAsStr("type");
	
	if (!type.compare("box")) {
		float length = props.getValAsReal("length");
		float width = props.getValAsReal("width");
		float height = props.getValAsReal("height");
		geomID_ = dCreateBox(NULL, length, width, height);
	} else if (!type.compare("ccylinder")) {
		float length = props.getValAsReal("length");
		float radius = props.getValAsReal("radius");
		geomID_ = dCreateCCylinder(NULL, radius, length);
	} else if (!type.compare("cylinder")) {
		float length = props.getValAsReal("length");
		float radius = props.getValAsReal("radius");
		geomID_ = dCreateCylinder(NULL, radius, length);
	} else if (!type.compare("sphere")) {
		float radius = props.getValAsReal("radius");
		geomID_ = dCreateSphere(NULL, radius);
	} else if (!type.compare("plane")) {
		float nx = props.getValAsReal("normal_x");
		float ny = props.getValAsReal("normal_y");
		float nz = props.getValAsReal("normal_z");
		float d = props.getValAsReal("d");
		geomID_ = dCreatePlane(NULL, nx, ny, nz, d);
	} else if (!type.compare("mesh")) {				
		// Got to make a TriMeshObj!.. tough one.
		std::string filename = props.getValAsStr("filename");
		int pos = filename.find_last_of(".");
		std::string extension = filename.substr(pos+1);
		float scale = props.getValAsReal("scale");
	
		if (!extension.compare("obj")) {
			mesh.reset(new POLYHEDRON());
			mesh->filename = filename;
			if ( importOBJ(mesh.get(), scale) != 0 ) 
			{
				std::ostringstream msg;
				msg << "importOBJ(): Returned an error!" << std::endl;
				throw std::runtime_error(msg.str());
			}
		} else if (!extension.compare("stl")) {
			mesh.reset(new POLYHEDRON());
			mesh->filename = filename;
			if ( importSTL(mesh.get(), scale) != 0 )
			{
				std::ostringstream msg;
				msg << "importOBJ(): Returned an error!" << std::endl;
				throw std::runtime_error(msg.str());
			}
		} else {
			std::ostringstream msg;
			msg << extension << " is an unrecognized 3D model type. Currently only stl and obj files are supported." << std::endl;
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
}
/**************************************************************************/

Geom::Geom(const sml::PropertyCollection& props) :
	pImpl_(NULL)
{
	pImpl_ = new GeomImpl(props);
}



/**************************************************************************
 *	Body implementation
 **************************************************************************/
class BodyImpl
{
public:
	BodyImpl(const sml::PropertyCollection& props);
	virtual ~BodyImpl();
	
	//! Returns the objects position in world coordinates
	ReturnMatrix globalPosition() const;
	//! Set the objects position in world coordinates
	void globalPosition(const ColumnVector& pos);
	//! Returns the objects rotation matrix in world coordinates
	ReturnMatrix globalRotation() const;
	//! Set the objects rotation in world coordinates
	void globalRotation(const Matrix& rot);
	//! Returns the objects transformation matrix in world coordinates
	ReturnMatrix globalTransformation() const;
	//! Notify this object that its world pose is invalid
	void invalidate();
	//! Instruct object to re-validate its world pose
	void validate();

private:
	// Methods specific to this class
	//! Returns the body ODE pointer associated with this object.
	dBodyID id() const {return bodyID_;}
	//! Associate a distal body with this body
	void addDistBody(Body* body) {distBodies_.push_back(body);}
	//! Associate a geom with this body
	void addGeom(Geom* geom) {geomList_.push_back(geom);}
	/** Gets the number of distal bodies */
	int getNumDistBodies() {return distBodies_.size();}
	/** Gets the number of geoms */
	int getNumGeoms() {return geomList_.size();}
	/** Gets distal body assocaited with index n */
	Body* getDistBody(int n);
	/** Gets geom associated with index n */
	Geom* getGeom(int n);

	//! Private constructor
	Body(dBodyID);
		
	//! List of distal bodies
	BodyList_t distBodies_;
	//! List of geoms
	GeomList_t geomList_;
	//! ODE object pointer
	dBodyID bodyID_;
};

BodyImpl::BodyImpl(const sml::PropertyCollection& props) :
	bodyID_(NULL)
{
	bodyID_ = dBodyCreate(scene_->getWorld());
}

/** Basic destructor */
BodyImpl::~BodyImpl()
{
	//if (id() != NULL) dBodyDestroy(id());
}

ReturnMatrix BodyImpl::globalPosition() const
{
	dVector3 pos;
	
	if (id() != NULL) {
		memcpy(pos, dBodyGetPosition(id()), sizeof(dVector3));
	} else {
		memset(pos, 0, sizeof(dVector3));
	}
	
	ColumnVector ret(3) = 0.0;
	ret << pos;
	ret.release();
	return ret;
}

//! Returns the objects rotation matrix in world coordinates
void BodyImpl::getGlobalRotation(dMatrix3 rot) const
{
	if (id() != NULL) {
		memcpy(rot, dBodyGetRotation(id()), sizeof(dMatrix3));
	} else {
		dRSetIdentity(rot);
	}
}

void BodyImpl::getGlobalTransformation(dMatrix4 T) const
{
	if (id() != NULL) {
		dMatrix3 rot; dVector3 pos;
		getGlobalRotation(rot);
		getGlobalPosition(pos);
		dTFromRAndPos(T, rot, pos);
	} else dTSetIdentity(T);
}

void BodyImpl::setGlobalPosition(const dVector3 pos)
{
	if (id() != NULL) dBodySetPosition(id(), pos[0], pos[1], pos[2]);
}

void BodyImpl::setGlobalRotation(const dMatrix3 rot)
{
	if (id() != NULL) dBodySetRotation(id(), rot);
}

void BodyImpl::invalidate()
{
	BodyList_t::iterator bi = distBodies_.begin();
	for (; bi != distBodies_.end(); ++bi) (*bi)->invalidate();

	GeomList_t::iterator gi = geomList_.begin();
	for (; gi != geomList_.end(); ++gi) (*gi)->invalidate();
	
	validWorldPose_ = false;
}

void BodyImpl::validate()
{
	// My world pose has not been invalidated by anyone, therefore there 
	// is nothing to do and I can just return.
	if (!getPoseValidity()) {
		// Someone has invalidated my local transformation, therefore I need
		// to compute a new one, then update my world transformation by asking
		// my prox body for his.
//		std::cout << this->name_ << ": Computing local transform" << std::endl;
		computeLocalTransform();
		Body* prox = (Body*)getProxObj();
		
		if (prox != NULL ) {
			prox->validate(); // Make sure his is also valid (essentially sets up recursion)
			dVector3 P_prox_obj, P_wcs_prox, P_wcs_obj;
			dMatrix3 R_prox_obj, R_wcs_prox, R_wcs_obj;
			
			// Get some data..
			prox->getGlobalPosition(P_wcs_prox);
			prox->getGlobalRotation(R_wcs_prox);
			this->getLocalPosition(P_prox_obj);
			this->getLocalRotation(R_prox_obj);
			
			// .. and do some calculations
			memcpy(P_wcs_obj, P_wcs_prox, sizeof(dVector3));
			dMULTIPLYADD0_331(P_wcs_obj, R_wcs_prox, P_prox_obj);
			dMULTIPLY0_333(R_wcs_obj, R_wcs_prox, R_prox_obj);
			
			// Inform ODE of the Body changes
			this->setGlobalPosition(P_wcs_obj);
			this->setGlobalRotation(R_wcs_obj);
		}
		validWorldPose_ = true; // Don't forget to set this!
	}
}

Body* BodyImpl::getDistBody(int n)
{
	//BodyList_t::iterator it = distBodies_.begin();
	//for (int i=0; i < n; ++n) it++;
	//return (*it);

	if (n < distBodies_.size()) {
		return distBodies_[n];
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Index exceeded number of dist bodies associated with this body.";
		throw std::runtime_error(msg.str());
	}
}

Geom* BodyImpl::getGeom(int n)
{
	//GeomList_t::iterator it = geomList_.begin();
	//for (int i=0; i < n; ++n) it++;
	//return (*it);

	if (n < geomList_.size()) {
		return geomList_[n];
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Index exceeded number of geoms associated with this body.";
		throw std::runtime_error(msg.str());
	}
}
/**************************************************************************/

Body::Body(const sml::PropertyCollection& props) :
	pImpl_(NULL)
{
	pImpl_ = new BodyImpl(props);
}

} // Namespace