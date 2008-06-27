/*
Copyright (C) 2002-2004  Etienne Lachance

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


Report problems and direct all questions to:

email: etienne.lachance@polymtl.ca or richard.gourdeau@polymtl.ca

-------------------------------------------------------------------------------
Revision_history:

2004/01/19: Etienne Lachance
	-Removed function Exp and Ln.
	-Added function power and Log.
	-Fixed bugs in Slerp, Slerp_prime, Squad and Squad_prime.

2003/05/23: Etienne Lachance
	-Added the following member function -=, +=, *=, /=, Exp, Ln, dot, d_dt, E
	-Added functions Integ_Trap_quat, Slerp, Slerp_prime, Squad, Squad_prime,

2004/05/14: Etienne Lachance
	-Replaced vec_x_prod by CrossProduct.

2004/05/21: Etienne Lachance
-Added comments that can be used with Doxygen.

2004/07/01: Etienne Lachance
-Replaced vec_dot_prod by DotProdut of Newmat library.

2004/07/01: Ethan Tira-Thompson
	-Added support for newmat's use_namespace #define, using ROBOOP namespace
	-Fixed problem in constructor using float as Real type

2005/11/06: Etienne Lachance
	- No need to provide a copy constructor and the assignment operator 
	(operator=) for Quaternion class. Instead we use the one provide by the
	compiler.

2005/11/13: Etienne Lachance
	- operator* and operator/ are now non-member functions when one of the
	operand is a real. With these modifications we support q2 = c * q1 and
	q2 = q1 * c
-------------------------------------------------------------------------------
*/


/*!
@file quaternion.cpp
@brief Quaternion functions.
*/
//#ifndef use_namespace
//#define use_namespace
//#endif

#include <math/smlQuaternion.h>

namespace sml {
	namespace math {
		//using namespace NEWMAT;

		//const Quaternion Quaternions::zero(zero,zero,zero,zero);
		//const Quaternion Quaternions::identity(one,zero,zero,zero);
		
		/*
		Quaternion::Quaternion()
		{
			s_ = one;
			v_ = ColumnVector(3);
			v_ = zero;
		}

		Quaternion::Quaternion(const Real angle, const ColumnVector & axis)
		{
			if(axis.Nrows() != 3)
			{
				cerr << "Quaternion::Quaternion, size of axis != 3" << endl;
				exit(1);
			}

			// make sure axis is a unit vector
			Real norm_axis = sqrt(DotProduct(axis, axis));

			if(norm_axis != 1)
			{
				cerr << "Quaternion::Quaternion(angle, axis), axis is not unit" << endl;
				cerr << "Make the axis unit." << endl;
				v_ = sin(angle/2) * axis/norm_axis;
			}
			else
			v_ = sin(angle/2) * axis;

			s_ = cos(angle/2);
		}

		Quaternion::Quaternion(const Real s_in, const Real v1, const Real v2,
		const Real v3)
		{
			s_ = s_in;
			v_ = ColumnVector(3);
			v_(1) = v1;
			v_(2) = v2;
			v_(3) = v3;
		}

		Quaternion::Quaternion(const Matrix & R)
		{
			if( (R.Nrows() == 3) && (R.Ncols() == 3) ||
					(R.Nrows() == 4) && (R.Ncols() == 4) )
			{
				Real tmp = fabs(R(1,1) + R(2,2) + R(3,3) + 1);
				s_ = 0.5*sqrt(tmp);
				if(v_.Nrows() != 3)
				v_ = ColumnVector(3);

				if(s_ > EPSILON)
				{
					v_(1) = (R(3,2)-R(2,3))/(4*s_);
					v_(2) = (R(1,3)-R(3,1))/(4*s_);
					v_(3) = (R(2,1)-R(1,2))/(4*s_);
				}
				else
				{
					// |w| <= 1/2
					static int s_iNext[3] = { 2, 3, 1 };
					int i = 1;
					if ( R(2,2) > R(1,1) )
					i = 2;
					if ( R(3,3) > R(2,2) )
					i = 3;
					int j = s_iNext[i-1];
					int k = s_iNext[j-1];

					Real fRoot = sqrt(R(i,i)-R(j,j)-R(k,k) + one);

					Real *tmp[3] = { &v_(1), &v_(2), &v_(3) };
					*tmp[i-1] = 0.5*fRoot;
					fRoot = 0.5/fRoot;
					s_ = (R(k,j)-R(j,k))*fRoot;
					*tmp[j-1] = (R(j,i)+R(i,j))*fRoot;
					*tmp[k-1] = (R(k,i)+R(i,k))*fRoot;
				}

			}
			else
			cerr << "Quaternion::Quaternion: matrix input is not 3x3 or 4x4" << endl;
		}

		Quaternion Quaternion::operator+(const Quaternion & rhs)const
		{
			Quaternion q;
			q.s_ = s_ + rhs.s_;
			q.v_ = v_ + rhs.v_;

			return q;
		}

		Quaternion Quaternion::operator-(const Quaternion & rhs)const
		{
			Quaternion q;
			q.s_ = s_ - rhs.s_;
			q.v_ = v_ - rhs.v_;

			return q;
		}

		Quaternion Quaternion::operator*(const Quaternion & rhs)const
		{
			Quaternion q;
			q.s_ = s_ * rhs.s_ - DotProduct(v_, rhs.v_);
			q.v_ = s_ * rhs.v_ + rhs.s_ * v_ + CrossProduct(v_, rhs.v_);

			return q;
		}


		Quaternion Quaternion::operator/(const Quaternion & rhs)const
		{
			return *this*rhs.i();
		}


		void Quaternion::set_v(const ColumnVector & v)
		{
			if(v.Nrows() == 3)
			v_ = v;
			else
			cerr << "Quaternion::set_v: input has a wrong size." << endl;
		}

		Quaternion Quaternion::conjugate()const
		{
			Quaternion q;
			q.s_ = s_;
			q.v_ = -1*v_;

			return q;
		}

		Real Quaternion::norm()const 
		{ 
			return( sqrt(s_*s_ + DotProduct(v_, v_)) );
		}

		Quaternion & Quaternion::unit()
		//! @brief Normalize a quaternion.
		{
			Real tmp = norm();
			if(tmp > EPSILON)
			{
				s_ = s_/tmp;
				v_ = v_/tmp;
			}
			return *this;
		}

		Quaternion Quaternion::i()const 
		{ 
			return conjugate()/norm();
		}

		Quaternion Quaternion::exp() const
		{
			Quaternion q;
			Real theta = sqrt(DotProduct(v_,v_)),
			sin_theta = sin(theta);

			q.s_ = cos(theta);
			if ( fabs(sin_theta) > EPSILON)
			q.v_ = v_*sin_theta/theta;
			else
			q.v_ = v_;

			return q;
		}

		Quaternion Quaternion::power(const Real t) const
		{
			Quaternion q = (Log()*t).exp();

			return q;
		}

		Quaternion Quaternion::Log()const
		{
			Quaternion q;
			q.s_ = zero;
			Real theta = acos(s_),
			sin_theta = sin(theta);

			if ( fabs(sin_theta) > EPSILON)
			q.v_ = v_/sin_theta*theta;
			else
			q.v_ = v_;

			return q;
		}

		Quaternion Quaternion::dot(const ColumnVector & w, const short sign)const
		{
			Quaternion q;
			Matrix tmp;

			tmp = -0.5*v_.t()*w;
			q.s_ = tmp(1,1);
			q.v_ = 0.5*E(sign)*w;

			return q;
		}

		ReturnMatrix Quaternion::E(const short sign)const
		{
			Matrix E(3,3), I(3,3);
			I << threebythreeident;

			if(sign == BODY_FRAME)
			E = s_*I + x_prod_matrix(v_);
			else
			E = s_*I - x_prod_matrix(v_);

			E.Release();
			return E;
		}

		Real Quaternion::dot_prod(const Quaternion & q)const
		{
			return (s_*q.s_ + v_(1)*q.v_(1) + v_(2)*q.v_(2) + v_(3)*q.v_(3));
		}

		ReturnMatrix Quaternion::R()const
		{
			Matrix R(3,3);
			R << threebythreeident;
			R = (1 - 2*DotProduct(v_, v_))*R + 2*v_*v_.t() + 2*s_*x_prod_matrix(v_);

			R.Release();
			return R;
		}

		ReturnMatrix Quaternion::T()const
		{
			Matrix T(4,4);
			T << fourbyfourident;
			T.SubMatrix(1,3,1,3) = (1 - 2*DotProduct(v_, v_))*T.SubMatrix(1,3,1,3)
			+ 2*v_*v_.t() + 2*s_*x_prod_matrix(v_);
			T.Release();
			return T;
		}

		// -------------------------------------------------------------------------------------

		Quaternion operator*(const Real c, const Quaternion & q)
		{
			Quaternion out;
			out.set_s(q.s() * c);
			out.set_v(q.v() * c);
			return out;
		}

		Quaternion operator*(const Quaternion & q, const Real c)
		{
			return operator*(c, q);
		}


		Quaternion operator/(const Real c, const Quaternion & q)
		{
			Quaternion out;
			out.set_s(q.s() / c);
			out.set_v(q.v() / c);
			return out;
		}

		Quaternion operator/(const Quaternion & q, const Real c)
		{
			return operator/(c, q);
		}

		ReturnMatrix Omega(const Quaternion & q, const Quaternion & q_dot)
		{
			Matrix A, B, M;
			UpperTriangularMatrix U;
			ColumnVector w(3);
			A = 0.5*q.E(BASE_FRAME);
			B = q_dot.v();
			if(A.Determinant())
			{
				QRZ(A,U);             //QR decomposition
				QRZ(A,B,M);
				w = U.i()*M;
			}
			else
			w = 0;

			w.Release();
			return w;
		}

		short Integ_quat(Quaternion & dquat_present, Quaternion & dquat_past,
		Quaternion & quat, const Real dt)
		{
			if (dt < 0)
			{
				cerr << "Integ_Trap(quat1, quat2, dt): dt < 0. dt is set to 0." << endl;
				return -1;
			}

			// Quaternion algebraic constraint
			//  Real Klambda = 0.5*(1 - quat.norm_sqr());

			dquat_present.set_s(dquat_present.s() );//+ Klambda*quat.s());
			dquat_present.set_v(dquat_present.v() ); //+ Klambda*quat.v());

			quat.set_s(quat.s() + Integ_Trap_quat_s(dquat_present, dquat_past, dt));
			quat.set_v(quat.v() + Integ_Trap_quat_v(dquat_present, dquat_past, dt));

			dquat_past.set_s(dquat_present.s());
			dquat_past.set_v(dquat_present.v());

			quat.unit();

			return 0;
		}

		Real Integ_Trap_quat_s(const Quaternion & present, Quaternion & past,
		const Real dt)
		{
			Real integ = 0.5*(present.s()+past.s())*dt;
			past.set_s(present.s());
			return integ;
		}

		ReturnMatrix Integ_Trap_quat_v(const Quaternion & present, Quaternion & past,
		const Real dt)
		{
			ColumnVector integ = 0.5*(present.v()+past.v())*dt;
			past.set_v(present.v());
			integ.Release();
			return integ;
		}

		Quaternion Slerp(const Quaternion & q0, const Quaternion & q1, const Real t)
		{
			if( (t < 0) || (t > 1) )
			cerr << "Slerp(q0, q1, t): t < 0 or t > 1. t is set to 0." << endl;

			if(q0.dot_prod(q1) >= 0)
			return q0*((q0.i()*q1).power(t));
			else
			return  q0*((q0.i()*-1*q1).power(t));
		}

		Quaternion Slerp_prime(const Quaternion & q0, const Quaternion & q1,
		const Real t)
		{
			if( (t < 0) || (t > 1) )
			cerr << "Slerp_prime(q0, q1, t): t < 0 or t > 1. t is set to 0." << endl;

			if(q0.dot_prod(q1) >= 0)
			return Slerp(q0, q1, t)*(q0.i()*q1).Log();
			else
			return Slerp(q0, q1, t)*(q0.i()*-1*q1).Log();
		}

		Quaternion Squad(const Quaternion & p, const Quaternion & a, const Quaternion & b,
		const Quaternion & q, const Real t)
		{
			if( (t < 0) || (t > 1) )
			cerr << "Squad(p,a,b,q, t): t < 0 or t > 1. t is set to 0." << endl;

			return Slerp(Slerp(p,q,t),Slerp(a,b,t),2*t*(1-t));
		}

		Quaternion Squad_prime(const Quaternion & p, const Quaternion & a, const Quaternion & b,
		const Quaternion & q, const Real t)
		{
			if( (t < 0) || (t > 1) )
			cerr << "Squad_prime(p,a,b,q, t): t < 0 or t > 1. t is set to 0." << endl;

			Quaternion q_squad,
			U = Slerp(p, q, t),
			V = Slerp(a, b, t),
			W = U.i()*V,
			U_prime = U*(p.i()*q).Log(),
			V_prime = V*(a.i()*b).Log(),
			W_prime = U.i()*V_prime - U.power(-2)*U_prime*V;

			q_squad = U*( W.power(2*t*(1-t))*W.Log()*(2-4*t) + W.power(2*t*(1-t)-1)*W_prime*2*t*(1-t) )
			+ U_prime*( W.power(2*t*(1-t)) );

			return q_squad;
		}

		ostream& operator<<(ostream& out, const Quaternion& q)
		{
			out << q.s_ << " " << q.v_(1) << "i " << q.v_(2) << "j " << q.v_(3) << "k";
		}
		*/
	} // sml::math
} // sml






