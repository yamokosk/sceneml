#ifndef POSE_ESTIMATION_H
#define POSE_ESTIMATION_H

#include <ode/ode.h>

#include "config.h"

namespace sceneml {

class PoseEstimatorBase
{
public:
	virtual void estimate(const dReal *X1, const dReal *X2, int nMarkers) = 0;
	//virtual void estimate(const Matrix& lCoord, const Matrix& gCoord) = 0;
	void getPose(dMatrix4 T);
	//ReturnMatrix getPose();

protected:
	PoseEstimatorBase();
	virtual ~PoseEstimatorBase() {};

	dMatrix4 tmatrix_;
};

class SVDEstimator : public PoseEstimatorBase
{
public:
	SVDEstimator() : PoseEstimatorBase() {};
	virtual ~SVDEstimator() {};

	void estimate(const dReal *X1, const dReal *X2, int nMarkers);
	//void estimate(const Matrix& lCoord, const Matrix& gCoord);
protected:
};

};

#endif
