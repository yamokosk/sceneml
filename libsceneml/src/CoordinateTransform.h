/*
 * CoordinateTransform.h
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#ifndef COORDINATETRANSFORM_H_
#define COORDINATETRANSFORM_H_

// Standard includes
#include <vector>

#include <math/Matrix.h>
#include <Observer.h>

namespace TinySG
{

class CoordinateTransform
{
public:
	CoordinateTransform();
	virtual ~CoordinateTransform();

	//! Returns the cached transform and recalculates if necessary
	const Matrix& getTransform();
	//! Hack attempt to squeeze an observer pattern into this compositional pattern.
	void setListener(CoordinateTransform* ct);
	//! Internal method to notify this object that somebody he was listening to changed.
	virtual void _notifyUpdate();

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


class CompositeTransform : public CoordinateTransform
{
public:
	typedef std::vector<CoordinateTransform> CoordinateTransformList;

	CompositeTransform();
	virtual ~CompositeTransform();
	//! Adds the transform to the composition.
	void add(CoordinateTransform t);
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
public:
	MarkerTransform();
	~MarkerTransform();

protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
};


class SimpleRotation : public CoordinateTransform
{
public:
	SimpleRotation(int axisType, Real ang);
	virtual ~SimpleRotation();
	void setAngle(Real ang);
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
private:
	Real angle_;
	int axisNumber_;
};


class SimpleTranslation : public CoordinateTransform
{
public:
	SimpleTranslation(Real x, Real y, Real z);
	virtual ~SimpleTranslation();
	void setVector(Real x, Real y, Real z);
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
private:
	Real x_, y_, z_;
};


class EulerRotation : public CoordinateTransform
{
public:
	EulerRotation(int seqType, Real tx, Real ty, Real tz);
	virtual ~EulerRotation();
	void setAngles(Real x, Real y, Real z);
protected:
	//! Internal method called to update the cached transform
	virtual void _updateTransform();
private:
	Real tx_, ty_, tz_;
	int seqType_;
};


} // namespace TinySG

#endif /* COORDINATETRANSFORM_H_ */
