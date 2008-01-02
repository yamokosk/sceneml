#include <xode/transform.h>
#include <xode/matrix.h>

#include <methods/pose_estimation.h>

using namespace xode;

SimpleTransform::~SimpleTransform()
{
	delete [] data_;
}

const dReal* SimpleTransform::compute()
{
	// Compute transform
	if ( !type_.compare("translation") ) {
		dTFromTrans(tmatrix_, data_[0], data_[1], data_[2]);
	} else if ( !type_.compare("rotation") ) {
		if ( !subtype_.compare("x") ) 		dTFromAxisAndAngle(tmatrix_, REAL(1.0), REAL(0.0), REAL(0.0), data_[0]); 
		else if ( !subtype_.compare("y") ) 	dTFromAxisAndAngle(tmatrix_, REAL(0.0), REAL(1.0), REAL(0.0), data_[0]); 
		else if ( !subtype_.compare("z") ) 	dTFromAxisAndAngle(tmatrix_, REAL(0.0), REAL(0.0), REAL(1.0), data_[0]);
		else if ( !subtype_.compare("e123") )	dTFromEuler123(tmatrix_, data_[0], data_[1], data_[2]);
		else if ( !subtype_.compare("t123") )	dTFromEuler123(tmatrix_, -data_[0], -data_[1], -data_[2]);
		else throw std::runtime_error("");
	} else  {
		throw std::runtime_error("");
	}
	
	return tmatrix_;
}


MarkerTransform::~MarkerTransform()
{
	for (int n=0; n < localCoords_.size(); ++n)
	{
		delete [] localCoords_[n];
		delete [] globalCoords_[n];
	}
}

const dReal* MarkerTransform::compute()
{
	// Number of local and global coords is ASSUMED to be the same.. no error 
	// checking done here
	int nNumCoords = localCoords_.size();
	
	// Allocate temp storage for markers and fill up with data
	dReal *lCoords = new dReal[3 * nNumCoords];
	dReal *gCoords = new dReal[3 * nNumCoords];

	for (int n=0; n < nNumCoords; ++n)
	{
		//dReal* lCoord = localCoords_[n], gCoord = globalCoords[n];
		
		memcpy( (lCoords+n*3), localCoords_[n], 3*sizeof(dReal) );
		memcpy( (gCoords+n*3), globalCoords_[n], 3*sizeof(dReal) );
	}
	
	// Do estimation and get answer
	SVDEstimator estimator;
	estimator.estimate(lCoords, gCoords, nNumCoords);
	estimator.getPose(tmatrix_);
	
	// Clean up
	delete [] lCoords;
	delete [] gCoords;
	
	return tmatrix_;
}


CompositeTransform::~CompositeTransform()
{
	CoordinateTransformList_t::iterator it = childTransforms_.begin();
	for (; it != childTransforms_.end(); ++it) delete (*it);
}


const dReal* CompositeTransform::compute() 
{
	dTSetIdentity(tmatrix_);
	CoordinateTransformList_t::iterator it = childTransforms_.begin();
	for (; it != childTransforms_.end(); ++it)
	{
		dMatrix4 Tr;
		dMultiply0(Tr, tmatrix_, (*it)->compute(), 4, 4, 4);
		memcpy(tmatrix_, Tr, sizeof(dMatrix4));
	}
	return tmatrix_;
};
