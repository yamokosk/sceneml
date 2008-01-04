#include "pose_estimation.h"
#include "matrix.h"

using namespace sceneml;

PoseEstimatorBase::PoseEstimatorBase()
{
	dTSetIdentity(tmatrix_);
}

void PoseEstimatorBase::getPose(dMatrix4 T)
{
	memcpy(T, tmatrix_, sizeof(dMatrix4));
	/*for (int r=0; r < 4; ++r) {
		for (int c=0; c < 4; ++c) {
			T[c + r*4] = tmatrix_(r+1,c+1);
		}
	}*/
}

/*ReturnMatrix PoseEstimatorBase::getPose()
{
	SquareMatrix T(tmatrix_);
	T.release();
	return T;
}*/

// From: http://www.kwon3d.com/theory/jkinem/rotmat.html	
void SVDEstimator::estimate(const dReal *X1, const dReal *X2, int nMarkers)
{
	// Normally to compute the rotation matrix from a set of marker data, you
	// would have 12 unknowns (9 for the rotation matrix and 3 for the location
	// of the center of rotation. 12 unknowns would then necessitate 4 markers.
	// To reduce this to three markers, we will calculate the rotational 
	// transformation about the mean-coordinate point within the body. See kwon
	// for more details
	dVector3 P1_1o_mo, P2_2o_mo;
	memset(P1_1o_mo, 0, sizeof(dVector3)); memset(P2_2o_mo, 0, sizeof(dVector3));
	for (int dim=0; dim < 3; ++dim) {
		for (int n=0; n < nMarkers; ++n) {
			P1_1o_mo[dim] += X1[n*3 + dim];
			P2_2o_mo[dim] += X2[n*3 + dim];
		}
		P1_1o_mo[dim] /= (dReal)nMarkers;
		P2_2o_mo[dim] /= (dReal)nMarkers;
	}
	
	dReal *Y1 = new dReal[nMarkers*3], *Y2 = new dReal[nMarkers*3];
	memset(Y1, 0, sizeof(Y1)); memset(Y2, 0, sizeof(Y2));
	for (int n=0; n < nMarkers; ++n) {
		for (int dim=0; dim < 3; ++dim) {
			Y1[n*3 + dim] = X1[n*3 + dim] - P1_1o_mo[dim];
			Y2[n*3 + dim] = X2[n*3 + dim] - P2_2o_mo[dim];
		}
	}
	
	// Compute the cross-dispersion matrix or the correlation matrix, C
	// 	C  = (1/N) * sum_1_N (y2i * y1i.transpose)
	dMatrix3 C; memset(C, 0, sizeof(dMatrix3));
	for (int n=0; n < nMarkers; ++n) dMULTIPLYOPV_333(C, +=, (Y2 + n*3), (Y1 + n*3));
	for (int n=0; n < 12; ++n) C[n] *= (REAL(1.0) / (float)nMarkers);
	
	// Singular value decomposition of the correlation matrix
	//	(U,D,V) = svd(C);
	dMatrix3 U, V; dVector3 d;
	memset(U, 0, sizeof(dMatrix3)); memset(V, 0, sizeof(dMatrix3));
	memset(d, 0, sizeof(dVector3));
	dSVD(U, V, d, C);

	// One problem that may arise is that the rotation matrix can have a 
	// determinant of -1. In other words, one can obtain a reflection matrix of 
	// the desired rotation matrix. To solve this problem define W,	
	dMatrix3 W, temp; dRSetIdentity(W);
	dMULTIPLY2_333(temp,U,V); // temp = U * V'
	W[10] = dDeterminant3(temp);
		
	// Finally the rotation matrix R = U * W * V.t'
	dMULTIPLY2_333(temp, W, V);
	dMatrix3 R_1_2;
	dMULTIPLY0_333(R_1_2, U, temp);
	
	// And translation
	dVector3 P1_2o_mo;
	dMULTIPLY0_331(P1_2o_mo, R_1_2, P2_2o_mo);
	dVector3 P1_1o_2o;
	for (int n=0; n < 3; ++n) P1_1o_2o[n] = P1_1o_mo[n] - P1_2o_mo[n];
	
	// Copy estimate into class variable
	dTFromRAndPos(tmatrix_, R_1_2, P1_1o_2o);
	
	return;
}

// From: http://www.kwon3d.com/theory/jkinem/rotmat.html
/*void SVDEstimator::estimate(const Matrix& X1, const Matrix& X2)
{
	if ( X1.ncols() != X2.ncols() ) throw std::runtime_error("");
	if ( (X1.nrows() != 3) || (X2.nrows() !=3) ) throw std::runtime_error("");
	
	int nMarkers = X1.ncols();
	
	// Normally to compute the rotation matrix from a set of marker data, you
	// would have 12 unknowns (9 for the rotation matrix and 3 for the location
	// of the center of rotation. 12 unknowns would then necessitate 4 markers.
	// To reduce this to three markers, we will calculate the rotational 
	// transformation about the mean-coordinate point within the body. See kwon
	// for more details
	Matrix Y1(3, nMarkers), Y2(3, nMarkers);
	ColumnVector X1_mean(3), X2_mean(3);
	for (int dim=1; dim <= 3; ++dim) {
		X1_mean(dim) = X1.row(dim).Sum() / (float)nMarkers;
		X2_mean(dim) = X2.row(dim).Sum() / (float)nMarkers;
		
		for (int n=1; n <= nMarkers; ++n) {
			Y1(dim, n) = X1(dim, n) - X1_mean(dim);
			Y2(dim, n) = X2(dim, n) - X2_mean(dim);
		}
	}
	
	// Compute the cross-dispersion matrix or the correlation matrix, C
	// 	C  = (1/N) * sum_1_N (y2i * y1i.transpose)
	Matrix C(3,3); C = 0;
	for (int n=1; n <= nMarkers; ++n) {
		C += ( Y2.column(n) * Y1.column(n).t() );
	}
	C *= (1.0 / (float)nMarkers);
	
	// Singular value decomposition of the correlation matrix
	//	(U,D,V) = svd(C);
	Matrix U(3,3), V(3,3), W(3,3); DiagonalMatrix D(3);
	SVD(C, D, U, V);

	// One problem that may arise is that the rotation matrix can have a 
	// determinant of -1. In other words, one can obtain a reflection matrix of 
	// the desired rotation matrix. To solve this problem define W,	
	W = IdentityMatrix(3);
	W(3,3) = (U * V.t()).determinant();
	
	// Finally the rotation matrix R is,
	tmatrix_.submatrix(1,3,1,3) = U * W * V.t();
	
	// And translation
	tmatrix_.submatrix(1,3,4,4) = X2_mean - X1_mean;
	
	return;
};*/
