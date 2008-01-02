#ifndef _TRANSFORM_H_FILE_
#define _TRANSFORM_H_FILE_

#include <map>
#include <list>
#include <string>
#include <vector>
#include <ode/ode.h>
#include <stdexcept>

namespace xode {


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
