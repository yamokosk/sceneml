/*
 * CoordinateTransform.h
 *
 *  Created on: Aug 12, 2008
 *      Author: yamokosk
 */

#ifndef COORDINATE_TRANSFORM_H_
#define COORDINATE_TRANSFORM_H_

// External
#include <log4cxx/logger.h>

// Internal
#include "math/Math.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"

namespace TinySG
{

/*!
@ingroup TinySG
@brief Encapsulates knowledge of a single coordinate transformation.

Stores data about and provides public methods to modify the orientation, translation, and scale
of a coordinate transformation.

@par Dependencies.
- newmat (for Matrix/Vector math)
- Boost::Quaternion
 */
class CoordinateTransform
{
	static log4cxx::LoggerPtr logger;

public:
	CoordinateTransform();
	virtual ~CoordinateTransform();

	//! Sets the orientation of this node via a Quaternion.
	void setOrientation (const Quaternion &q);
	//! Sets the orientation of this node via Quaternion parameters.
	void setOrientation (Real w, Real x, Real y, Real z);
	//! Sets the position of the node relative to it's parent.
	void setPosition(const ColumnVector &pos);
	//! Sets the position of the node relative to it's parent.
	void setPosition(Real x, Real y, Real z);
	//! Sets the scale of the node relative to it's parent.
	void setScale(const ColumnVector &s);
	//! Sets the scale of the node relative to it's parent.
	void setScale(Real x, Real y, Real z);
	//! Returns a Quaternion representing the transform's orientation.
	const Quaternion& getOrientation(void) const {return orientation_;}
	//! Gets the position of the node relative to it's parent.
	const ColumnVector& getPosition(void) const {return position_;}
	//! Returns the scale associated with this transform
	const ColumnVector& getScale(void) const {return scale_;}
	//! Gets a matrix whose columns are the local axes based on the nodes orientation relative to it's parent.
	//ReturnMatrix getLocalAxes (void) const;
	//! Returns full 4x4 matrix representation of the transform
	const Matrix& getFullTransform (void) const;

private:
	//! Stores the orientation of the node relative to it's parent.
	Quaternion orientation_;
	//! Stores the position/translation of the node relative to its parent.
	ColumnVector position_;
	//! Stores the scaling factor applied to this node.
	ColumnVector scale_;

	//! Internal method called to update the cached transform
	virtual void updateCachedTransform() const;
	//! Cached transform
	mutable Matrix cachedTransform_;
	//! Flag to indicate cached transform needs to be recalculated
	mutable bool cachedTransformOutOfDate_;
};

} // namespace TinySG

#endif /* COORDINATETRANSFORM_H_ */
