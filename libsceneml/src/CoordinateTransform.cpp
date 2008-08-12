/*
 * CoordinateTransform.cpp
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#include <CoordinateTransform.h>
#include <math/Util.h>

namespace TinySG
{
// ------------------------------------------------------------------
CoordinateTransform::CoordinateTransform() :
	cachedTransform_( MatrixFactory::Matrix4x4( IDENTITY ) ),
	cachedTransformOutOfDate_(true),
	listener_(NULL)
{

}

CoordinateTransform::~CoordinateTransform()
{

}

inline
void CoordinateTransform::_updateTransform()
{
	// Default behavior is to notify our listener that we have changed
	if (listener_) listener_->_notifyUpdate();
}

const Matrix& CoordinateTransform::getTransform()
{
	if (cachedTransformOutOfDate_)
	{
		this->_updateTransform();
		cachedTransformOutOfDate_ = false;
	}
	return cachedTransform_;
}

void CoordinateTransform::setListener(CoordinateTransform* ct)
{
	listener_ = ct;
}

inline
void CoordinateTransform::_notifyUpdate()
{
	// Base class does nothing. Meant to be overridden by child classes.
}


// ------------------------------------------------------------------
CompositeTransform::CompositeTransform()
	: CoordinateTransform()
{

}

CompositeTransform::~CompositeTransform()
{

}

inline
void CompositeTransform::add(CoordinateTransform t)
{
	// Let this transform know we are listening
	t.setListener(this);
	// Add new transform to the collection
	transforms_.push_back(t);
	// Invalidate the cached transform
	cachedTransformOutOfDate_ = true;
}

inline
size_t CompositeTransform::size()
{
	return transforms_.size();
}

void CompositeTransform::_notifyUpdate()
{
	// One of the CoordinateTransform objects of this composition has
	// changed and they let us know it. Easiest thing to do is simply
	// invalidate the composite's cached transform.. which then forces
	// the composite to get the latest changes from its collection of
	// transforms.
	cachedTransformOutOfDate_ = true;
}

void CompositeTransform::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	// Reset the transform matrix
	cachedTransform_ = MatrixFactory::Matrix4x4( IDENTITY );

	CoordinateTransformList::iterator it = transforms_.begin();
	for (; it != transforms_.end(); ++it)
	{
		cachedTransform_ *= (*it).getTransform();
	}
}

// ------------------------------------------------------------------
MarkerTransform::MarkerTransform() :
	CoordinateTransform()
{

}

MarkerTransform::~MarkerTransform()
{

}

void MarkerTransform::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	// Number of local and global coords is ASSUMED to be the same.. no error
	// checking done here
	//int nNumCoords = localCoords_.size();

	// Allocate temp storage for markers and fill up with data
	//dRealPtr lCoords( new dReal[3 * nNumCoords] );
	//dRealPtr gCoords( new dReal[3 * nNumCoords] );

	//for (int n=0; n < nNumCoords; ++n)
	//{
		//dReal* lCoord = localCoords_[n], gCoord = globalCoords[n];

	//	memcpy( (lCoords.get()+n*3), localCoords_[n].get(), 3*sizeof(dReal) );
	//	memcpy( (gCoords.get()+n*3), globalCoords_[n].get(), 3*sizeof(dReal) );
	//}

	// Do estimation and get answer
	//SVDEstimator estimator;
	//estimator.estimate(gCoords.get(), lCoords.get(), nNumCoords);
	//estimator.getPose(tmatrix_);
}


// ------------------------------------------------------------------
SimpleRotation::SimpleRotation(int axisType, Real ang) :
	CoordinateTransform(),
	angle_(ang),
	axisNumber_(axisType)
{

}

SimpleRotation::~SimpleRotation()
{

}

void SimpleRotation::setAngle(Real ang)
{
	angle_ = ang;
	cachedTransformOutOfDate_ = true;
}

void SimpleRotation::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	// Reset matrix
	cachedTransform_ = RotFromAngleAxis(angle_, VectorFactory::Vector3(axisNumber_) );
}

// ------------------------------------------------------------------
SimpleTranslation::SimpleTranslation(Real x, Real y, Real z) :
	CoordinateTransform(),
	x_(x), y_(y), z_(z)
{

}

SimpleTranslation::~SimpleTranslation()
{

}

void SimpleTranslation::setVector(Real x, Real y, Real z)
{
	x_ = x; y_ = y; z_ = z;
	cachedTransformOutOfDate_ = true;
}

void SimpleTranslation::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	cachedTransform_(1,4) = x_;
	cachedTransform_(2,4) = y_;
	cachedTransform_(3,4) = z_;
}

// ------------------------------------------------------------------
EulerRotation::EulerRotation(int seqType, Real tx, Real ty, Real tz) :
	CoordinateTransform(),
	tx_(tx), ty_(ty), tz_(tz),
	seqType_(seqType)
{

}

EulerRotation::~EulerRotation()
{

}

void EulerRotation::setAngles(Real tx, Real ty, Real tz)
{
	tx_ = tx; ty_ = ty; tz_ = tz;
	cachedTransformOutOfDate_ = true;
}

void EulerRotation::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	cachedTransform_ = RotFromEulerSequence(seqType_, tx_, ty_, tz_);
}

} // namespace TinySG
