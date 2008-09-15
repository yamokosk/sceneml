/*
 * CoordinateTransform.h
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#ifndef COORDINATETRANSFORM_H_
#define COORDINATETRANSFORM_H_

// Standard includes
#include <list>

// Boost
#include <boost/shared_ptr.hpp>

// Logging
#include <log4cxx/logger.h>

// Internal
#include <math/Matrix.h>
#include <Observer.h>

namespace TinySG
{

class CoordinateTransform : public Observer
{
	static log4cxx::LoggerPtr logger;

public:
	CoordinateTransform();
	virtual ~CoordinateTransform();

	//! Returns the cached transform and recalculates if necessary
	const Matrix& getTransform();
	//! Hack attempt to squeeze an observer pattern into this compositional pattern.
	void setListener(CoordinateTransform* ct);
	//! Internal method to notify this object that somebody he was listening to changed.
	virtual void _notifyUpdate();
	//! Method from Observer
	virtual void update(int hint);

protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
	//! Cached transform
	Matrix cachedTransform_;
	//! Flag to indicate cached transform needs to be recalculated
	bool cachedTransformOutOfDate_;
	/*
	 * author yamokosk
	 *
	 * This listener pointer is an attempt to hack an observer pattern into
	 * this composition pattern. I want the leaf classes of this compositional
	 * tree structure to be able to notify their composite owner that they
	 * have changed. This all being done to avoid the computational costs of
	 * recalculating the cached transform in a compositional object.
	 */
	//! Pointer to another CoordinateTransform to notify we have changed.
	CoordinateTransform* listener_;
};

typedef boost::shared_ptr<CoordinateTransform> CoordinateTransformPtr;
#define NEW_TRANSFORM(x) \
	TinySG::CoordinateTransformPtr( new x )


class CompositeTransform : public CoordinateTransform
{
	static log4cxx::LoggerPtr logger;

public:
	typedef std::list<CoordinateTransformPtr> CoordinateTransformList;

	CompositeTransform();
	virtual ~CompositeTransform();
	//! Adds the transform to the composition.
	void add(CoordinateTransformPtr t);
	//! Get number of child transforms
	size_t size();
	//! Inherited from CoordinateTransform
	virtual void _notifyUpdate();
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
private:
	//Collection of child transforms.
	CoordinateTransformList transforms_;
};


class MarkerTransform : public CoordinateTransform
{
	static log4cxx::LoggerPtr logger;

public:
	MarkerTransform();
	virtual ~MarkerTransform();

	//! Method from Observer - needs to override CoordinateTransform behavior
	virtual void update(int hint);
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
};


class SimpleRotation : public CoordinateTransform
{
	static log4cxx::LoggerPtr logger;

public:
	SimpleRotation(int axisType, Real ang);
	virtual ~SimpleRotation();
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
private:
	int axisNumber_;
};


class SimpleTranslation : public CoordinateTransform
{
	static log4cxx::LoggerPtr logger;

public:
	SimpleTranslation(Real x, Real y, Real z);
	virtual ~SimpleTranslation();
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
};


class EulerRotation : public CoordinateTransform
{
	static log4cxx::LoggerPtr logger;

public:
	EulerRotation(int seqType, Real tx, Real ty, Real tz);
	virtual ~EulerRotation();
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
private:
	int seqType_;
};


} // namespace TinySG

#endif /* COORDINATETRANSFORM_H_ */
