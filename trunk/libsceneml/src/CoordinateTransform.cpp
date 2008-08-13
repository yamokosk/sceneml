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

using namespace log4cxx;

LoggerPtr CoordinateTransform::logger(Logger::getLogger("TinySG.CoordinateTransform"));
LoggerPtr CompositeTransform::logger(Logger::getLogger("TinySG.CompositeTransform"));
LoggerPtr MarkerTransform::logger(Logger::getLogger("TinySG.MarkerTransform"));
LoggerPtr SimpleRotation::logger(Logger::getLogger("TinySG.SimpleRotation"));
LoggerPtr EulerRotation::logger(Logger::getLogger("TinySG.EulerRotation"));
LoggerPtr SimpleTranslation::logger(Logger::getLogger("TinySG.SimpleTranslation"));

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
	//LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);

	// Default behavior is to notify our listener that we have changed
	//if (listener_) {
	//	listener_->_notifyUpdate();
	//	LOG4CXX_DEBUG(logger, "Notified listener that I am about to update.");
	//}
}

const Matrix& CoordinateTransform::getTransform()
{
	if (cachedTransformOutOfDate_)
	{
		LOG4CXX_DEBUG(logger, "Cached transform out of date. Calling update method.");
		this->_updateTransform();
		cachedTransformOutOfDate_ = false;
	} else {
		LOG4CXX_DEBUG(logger, "Cached transform is OK.");
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
	/*CoordinateTransformList::iterator it = transforms_.begin();
	for (; it != transforms_.end(); ++it)
	{
		delete (*it);
	}*/
}

void CompositeTransform::add(CoordinateTransformPtr t)
{
	// Let this transform know we are listening
	t->setListener(this);
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

	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);

	// Reset the transform matrix
	cachedTransform_ = MatrixFactory::Matrix4x4( IDENTITY );

	CoordinateTransformList::iterator it = transforms_.begin();
	for (; it != transforms_.end(); ++it)
	{
		cachedTransform_ *= (*it)->getTransform();
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

	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);

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
	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);

	if (listener_) {
		listener_->_notifyUpdate();
		LOG4CXX_DEBUG(logger, "Notified listener that I need an update.");
	}
	angle_ = ang;
	cachedTransformOutOfDate_ = true;
}

void SimpleRotation::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);

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
	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);
	if (listener_) {
		listener_->_notifyUpdate();
		LOG4CXX_DEBUG(logger, "Notified listener that I need an update.");
	}
	x_ = x; y_ = y; z_ = z;
	cachedTransformOutOfDate_ = true;
}

void SimpleTranslation::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);

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
	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);
	if (listener_) {
		listener_->_notifyUpdate();
		LOG4CXX_DEBUG(logger, "Notified listener that I need an update.");
	}
	tx_ = tx; ty_ = ty; tz_ = tz;
	cachedTransformOutOfDate_ = true;
}

void EulerRotation::_updateTransform()
{
	// Parent implementation
	CoordinateTransform::_updateTransform();

	LOG4CXX_TRACE(logger, "Running " << __FUNCTION__);

	cachedTransform_ = RotFromEulerSequence(seqType_, tx_, ty_, tz_);
}

} // namespace TinySG
