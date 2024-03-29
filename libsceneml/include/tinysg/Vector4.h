/*************************************************************************
* TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
* All rights reserved.
* Email: yamokosk at gmail dot com
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation; either version 2.1 of the License,
* or (at your option) any later version. The text of the GNU Lesser General
* Public License is included with this library in the file LICENSE.TXT.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for
* more details.
*
*************************************************************************/

/*
 * Code taken from the OGRE (Object-oriented Graphics Rendering Engine) Project
 * http://www.ogre3d.org/
 */

#ifndef __Vector4_H__
#define __Vector4_H__

#include <tinysg/MathPrerequisites.h>
#include <tinysg/Vector3.h>

#include <string>
#include <sstream>

namespace TinySG
{

/** 4-dimensional homogeneous vector.
*/
class SOEXPORT Vector4
{
public:
	Real x, y, z, w;

public:
	inline Vector4()
	{
	}

	inline Vector4( const Real fX, const Real fY, const Real fZ, const Real fW )
		: x( fX ), y( fY ), z( fZ ), w( fW)
	{
	}

	inline explicit Vector4( const Real afCoordinate[4] )
		: x( afCoordinate[0] ),
		  y( afCoordinate[1] ),
		  z( afCoordinate[2] ),
		  w( afCoordinate[3] )
	{
	}

	inline explicit Vector4( const int afCoordinate[4] )
	{
		x = (Real)afCoordinate[0];
		y = (Real)afCoordinate[1];
		z = (Real)afCoordinate[2];
		w = (Real)afCoordinate[3];
	}

	inline explicit Vector4( Real* const r )
		: x( r[0] ), y( r[1] ), z( r[2] ), w( r[3] )
	{
	}

	inline explicit Vector4( const Real scaler )
		: x( scaler )
		, y( scaler )
		, z( scaler )
		, w( scaler )
	{
	}

	inline explicit Vector4(const Vector3& rhs)
		: x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
	{
	}

	inline Real operator [] ( const size_t i ) const
	{
		assert( i < 4 );

		return *(&x+i);
	}

	inline Real& operator [] ( const size_t i )
	{
		assert( i < 4 );

		return *(&x+i);
	}

	/// Pointer accessor for direct copying
	inline Real* ptr()
	{
		return &x;
	}
	/// Pointer accessor for direct copying
	inline const Real* ptr() const
	{
		return &x;
	}

	/** Assigns the value of the other vector.
		@param
			rkVector The other vector
	*/
	inline Vector4& operator = ( const Vector4& rkVector )
	{
		x = rkVector.x;
		y = rkVector.y;
		z = rkVector.z;
		w = rkVector.w;

		return *this;
	}

	inline Vector4& operator = ( const Real fScalar)
	{
		x = fScalar;
		y = fScalar;
		z = fScalar;
		w = fScalar;
		return *this;
	}

	inline bool operator == ( const Vector4& rkVector ) const
	{
		return ( x == rkVector.x &&
			y == rkVector.y &&
			z == rkVector.z &&
			w == rkVector.w );
	}

	inline bool operator != ( const Vector4& rkVector ) const
	{
		return ( x != rkVector.x ||
			y != rkVector.y ||
			z != rkVector.z ||
			w != rkVector.w );
	}

	inline Vector4& operator = (const Vector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 1.0f;
		return *this;
	}

	// arithmetic operations
	inline Vector4 operator + ( const Vector4& rkVector ) const
	{
		return Vector4(
			x + rkVector.x,
			y + rkVector.y,
			z + rkVector.z,
			w + rkVector.w);
	}

	inline Vector4 operator - ( const Vector4& rkVector ) const
	{
		return Vector4(
			x - rkVector.x,
			y - rkVector.y,
			z - rkVector.z,
			w - rkVector.w);
	}

	inline Vector4 operator * ( const Real fScalar ) const
	{
		return Vector4(
			x * fScalar,
			y * fScalar,
			z * fScalar,
			w * fScalar);
	}

	inline Vector4 operator * ( const Vector4& rhs) const
	{
		return Vector4(
			rhs.x * x,
			rhs.y * y,
			rhs.z * z,
			rhs.w * w);
	}

	inline Vector4 operator / ( const Real fScalar ) const
	{
		assert( fScalar != 0.0 );

		Real fInv = 1.0 / fScalar;

		return Vector4(
			x * fInv,
			y * fInv,
			z * fInv,
			w * fInv);
	}

	inline Vector4 operator / ( const Vector4& rhs) const
	{
		return Vector4(
			x / rhs.x,
			y / rhs.y,
			z / rhs.z,
			w / rhs.w);
	}

	inline const Vector4& operator + () const
	{
		return *this;
	}

	inline Vector4 operator - () const
	{
		return Vector4(-x, -y, -z, -w);
	}

	inline friend Vector4 operator * ( const Real fScalar, const Vector4& rkVector )
	{
		return Vector4(
			fScalar * rkVector.x,
			fScalar * rkVector.y,
			fScalar * rkVector.z,
			fScalar * rkVector.w);
	}

	inline friend Vector4 operator / ( const Real fScalar, const Vector4& rkVector )
	{
		return Vector4(
			fScalar / rkVector.x,
			fScalar / rkVector.y,
			fScalar / rkVector.z,
			fScalar / rkVector.w);
	}

	inline friend Vector4 operator + (const Vector4& lhs, const Real rhs)
	{
		return Vector4(
			lhs.x + rhs,
			lhs.y + rhs,
			lhs.z + rhs,
			lhs.w + rhs);
	}

	inline friend Vector4 operator + (const Real lhs, const Vector4& rhs)
	{
		return Vector4(
			lhs + rhs.x,
			lhs + rhs.y,
			lhs + rhs.z,
			lhs + rhs.w);
	}

	inline friend Vector4 operator - (const Vector4& lhs, Real rhs)
	{
		return Vector4(
			lhs.x - rhs,
			lhs.y - rhs,
			lhs.z - rhs,
			lhs.w - rhs);
	}

	inline friend Vector4 operator - (const Real lhs, const Vector4& rhs)
	{
		return Vector4(
			lhs - rhs.x,
			lhs - rhs.y,
			lhs - rhs.z,
			lhs - rhs.w);
	}

	// arithmetic updates
	inline Vector4& operator += ( const Vector4& rkVector )
	{
		x += rkVector.x;
		y += rkVector.y;
		z += rkVector.z;
		w += rkVector.w;

		return *this;
	}

	inline Vector4& operator -= ( const Vector4& rkVector )
	{
		x -= rkVector.x;
		y -= rkVector.y;
		z -= rkVector.z;
		w -= rkVector.w;

		return *this;
	}

	inline Vector4& operator *= ( const Real fScalar )
	{
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		w *= fScalar;
		return *this;
	}

	inline Vector4& operator += ( const Real fScalar )
	{
		x += fScalar;
		y += fScalar;
		z += fScalar;
		w += fScalar;
		return *this;
	}

	inline Vector4& operator -= ( const Real fScalar )
	{
		x -= fScalar;
		y -= fScalar;
		z -= fScalar;
		w -= fScalar;
		return *this;
	}

	inline Vector4& operator *= ( const Vector4& rkVector )
	{
		x *= rkVector.x;
		y *= rkVector.y;
		z *= rkVector.z;
		w *= rkVector.w;

		return *this;
	}

	inline Vector4& operator /= ( const Real fScalar )
	{
		assert( fScalar != 0.0 );

		Real fInv = 1.0 / fScalar;

		x *= fInv;
		y *= fInv;
		z *= fInv;
		w *= fInv;

		return *this;
	}

	inline Vector4& operator /= ( const Vector4& rkVector )
	{
		x /= rkVector.x;
		y /= rkVector.y;
		z /= rkVector.z;
		w /= rkVector.w;

		return *this;
	}

	/** Calculates the dot (scalar) product of this vector with another.
		@param
			vec Vector with which to calculate the dot product (together
			with this one).
		@returns
			A float representing the dot product value.
	*/
	inline Real dotProduct(const Vector4& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}

	std::string toString() const
	{
		std::stringstream ss;
		ss << x << " " << y << " " << z << " " << w;
		return ss.str();
	}

	/** Function for writing to a stream.
	*/
	inline SOEXPORT friend std::ostream& operator <<
		( std::ostream& o, const Vector4& v )
	{
		o << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return o;
	}
	// special
	static const Vector4 ZERO;
};

}
#endif
