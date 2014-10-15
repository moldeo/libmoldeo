/*******************************************************************************

                              moMathQuaternion.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
  *                                                                          *
  *   This code is distributed in the hope that it will be useful, but       *
  *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
  *   General Public License for more details.                               *
  *                                                                          *
  *   A copy of the GNU General Public License is available on the World     *
  *   Wide Web at <http://www.gnu.org/copyleft/gpl.html>. You can also       *
  *   obtain it by writing to the Free Software Foundation,                  *
  *   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.         *
  *                                                                          *
  ****************************************************************************

  Copyright(C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andrés Colubri

  Portions taken from
  Wild Magic Source Code
  David Eberly
  http://www.geometrictools.com
  Copyright (c) 1998-2007

*******************************************************************************/

#include "moMathMatrix.h"

#ifndef __MO_MATH_QUATERNION_H__
#define __MO_MATH_QUATERNION_H__

template <class Real>
class LIBMOLDEO_API moQuaternion : public moAbstract
{
public:
    // A quaternion is q = w + x*i + y*j + z*k where (w,x,y,z) is not
    // necessarily a unit length vector in 4D.

    // construction
    moQuaternion ();  // uninitialized
    moQuaternion (Real fW, Real fX, Real fY, Real fZ);
    moQuaternion (const moQuaternion& rkQ);

    // quaternion for the input rotation matrix
    moQuaternion (const moMatrix3<Real>& rkRot);

    // quaternion for the rotation of the axis-angle pair
    moQuaternion (const moVector3<Real>& rkAxis, Real fAngle);

    // quaternion for the rotation matrix with specified columns
    moQuaternion (const moVector3<Real> akRotColumn[3]);

    // member access:  0 = w, 1 = x, 2 = y, 3 = z
    inline operator const Real* () const { return m_afTuple; }
    inline operator Real* () { return m_afTuple; }
    inline Real operator[] (int i) const { return m_afTuple[i]; }
    inline Real& operator[] (int i) { return m_afTuple[i]; }
    inline Real W () const { return m_afTuple[0]; }
    inline Real& W () { return m_afTuple[0]; }
    inline Real X () const { return m_afTuple[1]; }
    inline Real& X () { return m_afTuple[1]; }
    inline Real Y () const { return m_afTuple[2]; }
    inline Real& Y () { return m_afTuple[2]; }
    inline Real Z () const { return m_afTuple[3]; }
    inline Real& Z () { return m_afTuple[3]; }

    // assignment
    inline moQuaternion& operator= (const moQuaternion& rkQ)
    {
        m_afTuple[0] = rkQ.m_afTuple[0];
        m_afTuple[1] = rkQ.m_afTuple[1];
        m_afTuple[2] = rkQ.m_afTuple[2];
        m_afTuple[3] = rkQ.m_afTuple[3];
        return *this;
    }

    // comparison
    bool operator== (const moQuaternion& rkQ) const;
    bool operator!= (const moQuaternion& rkQ) const;
    bool operator<  (const moQuaternion& rkQ) const;
    bool operator<= (const moQuaternion& rkQ) const;
    bool operator>  (const moQuaternion& rkQ) const;
    bool operator>= (const moQuaternion& rkQ) const;

    // arithmetic operations
    inline moQuaternion operator+ (const moQuaternion& rkQ) const
    {
        moQuaternion kSum;
        for (int i = 0; i < 4; i++) kSum.m_afTuple[i] = m_afTuple[i] + rkQ.m_afTuple[i];
        return kSum;
    }
    inline moQuaternion operator- (const moQuaternion& rkQ) const
    {
        moQuaternion kDiff;
        for (int i = 0; i < 4; i++) kDiff.m_afTuple[i] = m_afTuple[i] - rkQ.m_afTuple[i];
        return kDiff;
    }
    inline moQuaternion operator* (const moQuaternion& rkQ) const
    {
        // NOTE:  Multiplication is not generally commutative, so in most
        // cases p*q != q*p.

        moQuaternion kProd;

        kProd.m_afTuple[0] =
            m_afTuple[0]*rkQ.m_afTuple[0] -
            m_afTuple[1]*rkQ.m_afTuple[1] -
            m_afTuple[2]*rkQ.m_afTuple[2] -
            m_afTuple[3]*rkQ.m_afTuple[3];

        kProd.m_afTuple[1] =
            m_afTuple[0]*rkQ.m_afTuple[1] +
            m_afTuple[1]*rkQ.m_afTuple[0] +
            m_afTuple[2]*rkQ.m_afTuple[3] -
            m_afTuple[3]*rkQ.m_afTuple[2];

        kProd.m_afTuple[2] =
            m_afTuple[0]*rkQ.m_afTuple[2] +
            m_afTuple[2]*rkQ.m_afTuple[0] +
            m_afTuple[3]*rkQ.m_afTuple[1] -
            m_afTuple[1]*rkQ.m_afTuple[3];

        kProd.m_afTuple[3] =
            m_afTuple[0]*rkQ.m_afTuple[3] +
            m_afTuple[3]*rkQ.m_afTuple[0] +
            m_afTuple[1]*rkQ.m_afTuple[2] -
            m_afTuple[2]*rkQ.m_afTuple[1];

        return kProd;
    }
    inline moQuaternion operator* (Real fScalar) const
    {
        moQuaternion kProd;
        for (int i = 0; i < 4; i++) kProd.m_afTuple[i] = fScalar*m_afTuple[i];
        return kProd;
    }
    inline moQuaternion operator/ (Real fScalar) const
    {
        moQuaternion kQuot;
        int i;

        if (fScalar != (Real)0.0)
        {
            Real fInvScalar = ((Real)1.0)/fScalar;
            for (i = 0; i < 4; i++) kQuot.m_afTuple[i] = fInvScalar*m_afTuple[i];
        }
        else
        {
            for (i = 0; i < 4; i++) kQuot.m_afTuple[i] = moMath<Real>::MAX_REAL;
        }

        return kQuot;
    }
    inline moQuaternion operator- () const
    {
        moQuaternion kNeg;
        for (int i = 0; i < 4; i++) kNeg.m_afTuple[i] = -m_afTuple[i];
        return kNeg;
    }

    // arithmetic updates
    inline moQuaternion& operator+= (const moQuaternion& rkQ)
    {
        for (int i = 0; i < 4; i++) m_afTuple[i] += rkQ.m_afTuple[i];
        return *this;
    }
    inline moQuaternion& operator-= (const moQuaternion& rkQ)
    {
        for (int i = 0; i < 4; i++) m_afTuple[i] -= rkQ.m_afTuple[i];
        return *this;
    }
    inline moQuaternion& operator*= (Real fScalar)
    {
        for (int i = 0; i < 4; i++) m_afTuple[i] *= fScalar;
        return *this;
    }
    inline moQuaternion& operator/= (Real fScalar)
    {
        int i;

        if (fScalar != (Real)0.0)
        {
            Real fInvScalar = ((Real)1.0)/fScalar;
            for (i = 0; i < 4; i++) m_afTuple[i] *= fInvScalar;
        }
        else
        {
            for (i = 0; i < 4; i++) m_afTuple[i] = moMath<Real>::MAX_REAL;
        }

        return *this;
    }

    // conversion between quaternions, matrices, and axis-angle
    moQuaternion& FromRotationMatrix (const moMatrix3<Real>& rkRot);
    void ToRotationMatrix (moMatrix3<Real>& rkRot) const;
    moQuaternion& FromRotationMatrix (const moVector3<Real> akRotColumn[3]);
    void ToRotationMatrix (moVector3<Real> akRotColumn[3]) const;
    moQuaternion& FromAxisAngle (const moVector3<Real>& rkAxis, Real fAngle);
    void ToAxisAngle (moVector3<Real>& rkAxis, Real& rfAngle) const;

    // functions of a quaternion
    inline Real Length () const  // length of 4-tuple
	{
		return moMath<Real>::Sqrt(
	        m_afTuple[0]*m_afTuple[0] +
		    m_afTuple[1]*m_afTuple[1] +
			m_afTuple[2]*m_afTuple[2] +
			m_afTuple[3]*m_afTuple[3]);
	}
    inline Real SquaredLength () const  // squared length of 4-tuple
	{
		return
			m_afTuple[0]*m_afTuple[0] +
			m_afTuple[1]*m_afTuple[1] +
			m_afTuple[2]*m_afTuple[2] +
			m_afTuple[3]*m_afTuple[3];
	}
    inline Real Dot (const moQuaternion& rkQ) const  // dot product of 4-tuples
	{
		Real fDot = (Real)0.0;
		for (int i = 0; i < 4; i++) fDot += m_afTuple[i]*rkQ.m_afTuple[i];
	    return fDot;
	}
    inline Real Normalize ()  // make the 4-tuple unit length
	{
		Real fLength = Length();

		if (fLength > moMath<Real>::ZERO_TOLERANCE)
		{
			Real fInvLength = ((Real)1.0)/fLength;
			m_afTuple[0] *= fInvLength;
			m_afTuple[1] *= fInvLength;
			m_afTuple[2] *= fInvLength;
			m_afTuple[3] *= fInvLength;
		}
		else
		{
	        fLength = (Real)0.0;
			m_afTuple[0] = (Real)0.0;
			m_afTuple[1] = (Real)0.0;
			m_afTuple[2] = (Real)0.0;
			m_afTuple[3] = (Real)0.0;
		}

		return fLength;
	}

    moQuaternion Inverse () const;  // apply to non-zero quaternion
    moQuaternion Conjugate () const;
    moQuaternion Exp () const;  // apply to quaternion with w = 0
    moQuaternion Log () const;  // apply to unit-length quaternion

    // rotation of a vector by a quaternion
    moVector3<Real> Rotate (const moVector3<Real>& rkVector) const;

    // spherical linear interpolation
    moQuaternion& Slerp (Real fT, const moQuaternion& rkP, const moQuaternion& rkQ);

    moQuaternion& SlerpExtraSpins (Real fT, const moQuaternion& rkP,
        const moQuaternion& rkQ, int iExtraSpins);

    // intermediate terms for spherical quadratic interpolation
    moQuaternion& Intermediate (const moQuaternion& rkQ0,
        const moQuaternion& rkQ1, const moQuaternion& rkQ2);

    // spherical quadratic interpolation
    moQuaternion& Squad (Real fT, const moQuaternion& rkQ0,
        const moQuaternion& rkA0, const moQuaternion& rkA1,
        const moQuaternion& rkQ1);

    // Compute a quaternion that rotates unit-length vector V1 to unit-length
    // vector V2.  The rotation is about the axis perpendicular to both V1 and
    // V2, with angle of that between V1 and V2.  If V1 and V2 are parallel,
    // any axis of rotation will do, such as the permutation (z2,x2,y2), where
    // V2 = (x2,y2,z2).
    moQuaternion& Align (const moVector3<Real>& rkV1, const moVector3<Real>& rkV2);

    // Decompose a quaternion into q = q_twist * q_swing, where q is 'this'
    // quaternion.  If V1 is the input axis and V2 is the rotation of V1 by
    // q, q_swing represents the rotation about the axis perpendicular to
    // V1 and V2 (see moQuaternion::Align), and q_twist is a rotation about V1.
    void DecomposeTwistTimesSwing (const moVector3<Real>& rkV1,
        moQuaternion& rkTwist, moQuaternion& rkSwing);

    // Decompose a quaternion into q = q_swing * q_twist, where q is 'this'
    // quaternion.  If V1 is the input axis and V2 is the rotation of V1 by
    // q, q_swing represents the rotation about the axis perpendicular to
    // V1 and V2 (see moQuaternion::Align), and q_twist is a rotation about V1.
    void DecomposeSwingTimesTwist (const moVector3<Real>& rkV1,
        moQuaternion& rkSwing, moQuaternion& rkTwist);

    // special values
    static const moQuaternion IDENTITY;
    static const moQuaternion ZERO;

private:
    // support for comparisons
    int CompareArrays (const moQuaternion& rkQ) const;

    // support for FromRotationMatrix
    static int ms_iNext[3];

    Real m_afTuple[4];
};

template <class Real>
inline moQuaternion<Real> operator* (Real fScalar, const moQuaternion<Real>& rkQ)
{
    moQuaternion<Real> kProd;
    for (int i = 0; i < 4; i++)
    {
        kProd[i] = fScalar*rkQ[i];
    }
    return kProd;
}

#ifndef MO_MACOSX
#ifndef MO_WIN32
template class LIBMOLDEO_API moQuaternion<MOfloat>;
template class LIBMOLDEO_API moQuaternion<MOdouble>;
#endif
#endif

typedef moQuaternion<MOfloat> moQuaternionf;
typedef moQuaternion<MOdouble> moQuaterniond;

#endif

