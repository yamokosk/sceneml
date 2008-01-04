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

#ifndef _SCENEML_TRANSFORM_H_FILE_
#define _SCENEML_TRANSFORM_H_FILE_

#include <ode/ode.h>
#include <vector>
#include <string>
#include <list>

namespace sceneml {


class CoordinateTransform
{
public:
	virtual const dReal* compute() = 0;
protected:
	dMatrix4 tmatrix_;
};


class SimpleTransform : public CoordinateTransform
{
public:
	SimpleTransform(const std::string& type, const std::string& subtype) : 
		CoordinateTransform(), 
		type_(type), 
		subtype_(subtype), 
		data_(NULL) {}
	virtual ~SimpleTransform();
	void setData(dReal* data) {data_ = data;};
	virtual const dReal* compute();
private:
	SimpleTransform() {}; // Private default constructor - type and subtype must be set!
	std::string type_;
	std::string subtype_;
	dReal* data_;
};


class MarkerTransform : public CoordinateTransform
{
public:
	virtual ~MarkerTransform();
	void addLocalCoord(dReal* lc) {localCoords_.push_back(lc);}
	void addGlobalCoord(dReal* gc) {globalCoords_.push_back(gc);}
	virtual const dReal* compute();
private:
	std::vector< dReal* > globalCoords_;
	std::vector< dReal* > localCoords_;
};


typedef std::list<CoordinateTransform*> CoordinateTransformList_t;


class CompositeTransform : public CoordinateTransform
{
public:
	virtual ~CompositeTransform();
	//! Adds the transform to the composition.
	void add(CoordinateTransform *t) {childTransforms_.push_back(t);};
	//! Computes the composite transform
	virtual const dReal* compute();
	//! Get the current transformation matrix - do not compute it
	const dReal* getTMatrix() {return tmatrix_;}
	//! Get number of child transforms
	int size() {return childTransforms_.size();}
private:
	//Collection of child transforms.
	CoordinateTransformList_t childTransforms_;
};


};

#endif
