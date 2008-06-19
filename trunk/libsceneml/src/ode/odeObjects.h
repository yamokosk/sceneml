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

#include <sceneobjects.h>

#define GEOM_COLLIDE_OFF	0
#define GEOM_COLLIDE_ON		1

namespace smlode {

// forward declaration of the Geom Pimpl
class GeomImpl;
 
//! Geometry class
/** @ingroup xode 
 *  The geometry class is used for any scene object has some non-zero volume. Notice that this class is not
 *  intended for general use as its constructor is protected. Object creation is provided by the Scene class.
 */
class Geom : public SceneObjectBase
{
	friend class Scene;
public:
	virtual ~Geom() { if (pImpl_) delete pImpl_; }
	
	// The following come from SceneObjectBase and must be defined
	
	//! Returns the objects position in world coordinates
	ReturnMatrix getGlobalPosition() const {return pImpl_->globalPosition();}
	//! Returns the objects rotation matrix in world coordinates
	ReturnMatrix getGlobalRotation() const {return pImpl_->globalRotation();}
	//! Returns the objects transformation matrix in world coordinates
	ReturnMatrix getGlobalTransformation() const {return pImpl_->globalTransformation();}
	//! Set the objects position in world coordinates
	void setGlobalPosition(const ColumnVector& pos) {pImpl_->globalPosition(pos);}
	//! Set the objects rotation in world coordinates
	void setGlobalRotation(const Matrix& rot) {pImpl_->globalPosition(rot);}
	//! Notify this object that its world pose is invalid
	void invalidate() {pImpl_->invalidate();}
	//! Instruct object to re-validate its world pose
	void validate() {pImpl_->validate();}

private:
	Geom(const sml::PropertyCollection& props);
	
	// Internal methods
	GeomImpl pImpl_;
};


// forward declaration of the Body Pimpl
class BodyImpl;

//! Body class
/** @ingroup xode 
 *  The body class is used to model volumeless objects. Its really provided to model placeable coordinate systems.
 *  Notice that this class is not intended for general use as its constructor is protected. Object creation is provided 
 *  by the Scene class.
 */
class Body : public SceneObjectBase
{
	friend class Scene;
public:
	virtual ~Body() { if (pImpl_) delete pImpl_; }
	
	// The following come from SceneObjectBase and must be defined
	
	//! Returns the objects position in world coordinates
	ReturnMatrix getGlobalPosition() const {return pImpl_->globalPosition();}
	//! Returns the objects rotation matrix in world coordinates
	ReturnMatrix getGlobalRotation() const {return pImpl_->globalRotation();}
	//! Returns the objects transformation matrix in world coordinates
	ReturnMatrix getGlobalTransformation() const {return pImpl_->globalTransformation();}
	//! Set the objects position in world coordinates
	void setGlobalPosition(const ColumnVector& pos) {pImpl_->globalPosition(pos);}
	//! Set the objects rotation in world coordinates
	void setGlobalRotation(const Matrix& rot) {pImpl_->globalPosition(rot);}
	//! Notify this object that its world pose is invalid
	void invalidate() {pImpl_->invalidate();}
	//! Instruct object to re-validate its world pose
	void validate();

private:
	Body(const sml::PropertyCollection& props);
	
	// Internal methods
	BodyImpl pImpl_;
};

} // Namespace