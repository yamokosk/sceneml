/*
 * CoordinateTransform.cpp
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#include "CoordinateTransform.h"
#include "math/Util.h"

namespace TinySG
{

using namespace log4cxx;

LoggerPtr CoordinateTransform::logger(Logger::getLogger("TinySG.CoordinateTransform"));

// ------------------------------------------------------------------
CoordinateTransform::CoordinateTransform() :
	orientation_( QuaternionFactory::IDENTITY ),
	position_( VectorFactory::Vector3( ZERO ) ),
	scale_( VectorFactory::Vector3( ONES ) ),
	cachedTransform_( MatrixFactory::Matrix4x4( IDENTITY ) ),
	cachedTransformOutOfDate_(false)
{
	LOG4CXX_TRACE(logger, "CoordinateTransform constructor called.");
}

CoordinateTransform::~CoordinateTransform()
{

}

void CoordinateTransform::setOrientation (const Quaternion &q)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );
	orientation_ = q;
	cachedTransformOutOfDate_ = true;
}

void CoordinateTransform::setOrientation (Real w, Real x, Real y, Real z)
{
	Quaternion q(w, x, y, z);
	setOrientation(q);
}

void CoordinateTransform::setPosition(const ColumnVector &pos)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );
	position_ = pos;
	cachedTransformOutOfDate_ = true;
}

void CoordinateTransform::setPosition(Real x, Real y, Real z)
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );
	ColumnVector v(3); v << x << y << z;
	setPosition(v);
}

void CoordinateTransform::setScale(const ColumnVector &s)
{
	scale_ = s;
	cachedTransformOutOfDate_ = true;
}

void CoordinateTransform::setScale(Real x, Real y, Real z)
{
	ColumnVector s(3); s << x << y << z;
	setScale(s);
}

/*
ReturnMatrix CoordinateTransform::getLocalAxes (void) const
{
	ColumnVector axisX = VectorFactory::Vector3( UNIT_X );
	axisX = orientation_ * axisX;

	ColumnVector axisY = VectorFactory::Vector3( UNIT_Y );
	axisY = orientation_ * axisY;

	ColumnVector axisZ = VectorFactory::Vector3( UNIT_Z );
	axisZ = orientation_ * axisZ;

	// Concatenate columns to form matrix
	Matrix ret = MatrixFactory::Matrix4x4( IDENTITY );
	ret.SubMatrix(1,3,1,3) = (axisX & axisY & axisZ);
	ret.Release(); return ret;
}*/

const Matrix& CoordinateTransform::getFullTransform() const
{
	if (cachedTransformOutOfDate_)
	{
		LOG4CXX_TRACE(logger, "Cached transform out of date. Calling update method.");
		updateCachedTransform();
	} else {
		LOG4CXX_TRACE(logger, "Cached transform is OK.");
	}
	return cachedTransform_;
}

void CoordinateTransform::updateCachedTransform() const
{
	LOG4CXX_TRACE(logger, "Entering " << __FUNCTION__ );

	cachedTransform_ = RotFromQuaternion( getOrientation() );
	cachedTransform_.SubMatrix(1,3,4,4) = getPosition();
	cachedTransformOutOfDate_ = false;
}

} // namespace TinySG
