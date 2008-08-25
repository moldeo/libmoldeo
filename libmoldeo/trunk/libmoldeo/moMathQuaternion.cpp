/*******************************************************************************

                              moMathQuaternion.cpp

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

#include "moMathQuaternion.h"

template <class Real>
moQuaternion<Real>::moQuaternion ()
{
    // uninitialized for performance in array construction
}

template <class Real>
moQuaternion<Real>::moQuaternion (Real fW, Real fX, Real fY, Real fZ)
{
    m_afTuple[0] = fW;
    m_afTuple[1] = fX;
    m_afTuple[2] = fY;
    m_afTuple[3] = fZ;
}

template <class Real>
moQuaternion<Real>::moQuaternion (const moQuaternion& rkQ) : moAbstract(rkQ)
{
    m_afTuple[0] = rkQ.m_afTuple[0];
    m_afTuple[1] = rkQ.m_afTuple[1];
    m_afTuple[2] = rkQ.m_afTuple[2];
    m_afTuple[3] = rkQ.m_afTuple[3];
}

template <class Real>
moQuaternion<Real>::moQuaternion (const moMatrix3<Real>& rkRot)
{
    FromRotationMatrix(rkRot);
}

template <class Real>
moQuaternion<Real>::moQuaternion (const moVector3<Real>& rkAxis, Real fAngle)
{
    FromAxisAngle(rkAxis,fAngle);
}

template <class Real>
moQuaternion<Real>::moQuaternion (const moVector3<Real> akRotColumn[3])
{
    FromRotationMatrix(akRotColumn);
}

template <class Real>
int moQuaternion<Real>::CompareArrays (const moQuaternion& rkQ) const
{
    return memcmp(m_afTuple,rkQ.m_afTuple,4*sizeof(Real));
}

template <class Real>
bool moQuaternion<Real>::operator== (const moQuaternion& rkQ) const
{
    return CompareArrays(rkQ) == 0;
}

template <class Real>
bool moQuaternion<Real>::operator!= (const moQuaternion& rkQ) const
{
    return CompareArrays(rkQ) != 0;
}

template <class Real>
bool moQuaternion<Real>::operator< (const moQuaternion& rkQ) const
{
    return CompareArrays(rkQ) < 0;
}

template <class Real>
bool moQuaternion<Real>::operator<= (const moQuaternion& rkQ) const
{
    return CompareArrays(rkQ) <= 0;
}

template <class Real>
bool moQuaternion<Real>::operator> (const moQuaternion& rkQ) const
{
    return CompareArrays(rkQ) > 0;
}

template <class Real>
bool moQuaternion<Real>::operator>= (const moQuaternion& rkQ) const
{
    return CompareArrays(rkQ) >= 0;
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::FromRotationMatrix (
    const moMatrix3<Real>& rkRot)
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    Real fTrace = rkRot(0,0) + rkRot(1,1) + rkRot(2,2);
    Real fRoot;

    if (fTrace > (Real)0.0)
    {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = moMath<Real>::Sqrt(fTrace + (Real)1.0);  // 2w
        m_afTuple[0] = ((Real)0.5)*fRoot;
        fRoot = ((Real)0.5)/fRoot;  // 1/(4w)
        m_afTuple[1] = (rkRot(2,1)-rkRot(1,2))*fRoot;
        m_afTuple[2] = (rkRot(0,2)-rkRot(2,0))*fRoot;
        m_afTuple[3] = (rkRot(1,0)-rkRot(0,1))*fRoot;
    }
    else
    {
        // |w| <= 1/2
        int i = 0;
        if ( rkRot(1,1) > rkRot(0,0) )
        {
            i = 1;
        }
        if ( rkRot(2,2) > rkRot(i,i) )
        {
            i = 2;
        }
        int j = ms_iNext[i];
        int k = ms_iNext[j];

        fRoot = moMath<Real>::Sqrt(rkRot(i,i)-rkRot(j,j)-rkRot(k,k)+(Real)1.0);
        Real* apfQuat[3] = { &m_afTuple[1], &m_afTuple[2], &m_afTuple[3] };
        *apfQuat[i] = ((Real)0.5)*fRoot;
        fRoot = ((Real)0.5)/fRoot;
        m_afTuple[0] = (rkRot(k,j)-rkRot(j,k))*fRoot;
        *apfQuat[j] = (rkRot(j,i)+rkRot(i,j))*fRoot;
        *apfQuat[k] = (rkRot(k,i)+rkRot(i,k))*fRoot;
    }

    return *this;
}

template <class Real>
void moQuaternion<Real>::ToRotationMatrix (moMatrix3<Real>& rkRot) const
{
    Real fTx  = ((Real)2.0)*m_afTuple[1];
    Real fTy  = ((Real)2.0)*m_afTuple[2];
    Real fTz  = ((Real)2.0)*m_afTuple[3];
    Real fTwx = fTx*m_afTuple[0];
    Real fTwy = fTy*m_afTuple[0];
    Real fTwz = fTz*m_afTuple[0];
    Real fTxx = fTx*m_afTuple[1];
    Real fTxy = fTy*m_afTuple[1];
    Real fTxz = fTz*m_afTuple[1];
    Real fTyy = fTy*m_afTuple[2];
    Real fTyz = fTz*m_afTuple[2];
    Real fTzz = fTz*m_afTuple[3];

    rkRot(0,0) = (Real)1.0-(fTyy+fTzz);
    rkRot(0,1) = fTxy-fTwz;
    rkRot(0,2) = fTxz+fTwy;
    rkRot(1,0) = fTxy+fTwz;
    rkRot(1,1) = (Real)1.0-(fTxx+fTzz);
    rkRot(1,2) = fTyz-fTwx;
    rkRot(2,0) = fTxz-fTwy;
    rkRot(2,1) = fTyz+fTwx;
    rkRot(2,2) = (Real)1.0-(fTxx+fTyy);
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::FromRotationMatrix (
    const moVector3<Real> akRotColumn[3])
{
    moMatrix3<Real> kRot;
    for (int iCol = 0; iCol < 3; iCol++)
    {
        kRot(0,iCol) = akRotColumn[iCol][0];
        kRot(1,iCol) = akRotColumn[iCol][1];
        kRot(2,iCol) = akRotColumn[iCol][2];
    }
    return FromRotationMatrix(kRot);
}

template <class Real>
void moQuaternion<Real>::ToRotationMatrix (moVector3<Real> akRotColumn[3]) const
{
    moMatrix3<Real> kRot;
    ToRotationMatrix(kRot);
    for (int iCol = 0; iCol < 3; iCol++)
    {
        akRotColumn[iCol][0] = kRot(0,iCol);
        akRotColumn[iCol][1] = kRot(1,iCol);
        akRotColumn[iCol][2] = kRot(2,iCol);
    }
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::FromAxisAngle (
    const moVector3<Real>& rkAxis, Real fAngle)
{
    // assert:  axis[] is unit length
    //
    // The quaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    Real fHalfAngle = ((Real)0.5)*fAngle;
    Real fSin = moMath<Real>::Sin(fHalfAngle);
    m_afTuple[0] = moMath<Real>::Cos(fHalfAngle);
    m_afTuple[1] = fSin*rkAxis[0];
    m_afTuple[2] = fSin*rkAxis[1];
    m_afTuple[3] = fSin*rkAxis[2];

    return *this;
}

template <class Real>
void moQuaternion<Real>::ToAxisAngle (moVector3<Real>& rkAxis, Real& rfAngle)
    const
{
    // The quaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    Real fSqrLength = m_afTuple[1]*m_afTuple[1] + m_afTuple[2]*m_afTuple[2]
        + m_afTuple[3]*m_afTuple[3];

    if (fSqrLength > moMath<Real>::ZERO_TOLERANCE)
    {
        rfAngle = ((Real)2.0)*moMath<Real>::ACos(m_afTuple[0]);
        Real fInvLength = moMath<Real>::InvSqrt(fSqrLength);
        rkAxis[0] = m_afTuple[1]*fInvLength;
        rkAxis[1] = m_afTuple[2]*fInvLength;
        rkAxis[2] = m_afTuple[3]*fInvLength;
    }
    else
    {
        // angle is 0 (mod 2*pi), so any axis will do
        rfAngle = (Real)0.0;
        rkAxis[0] = (Real)1.0;
        rkAxis[1] = (Real)0.0;
        rkAxis[2] = (Real)0.0;
    }
}

template <class Real>
moQuaternion<Real> moQuaternion<Real>::Inverse () const
{
    moQuaternion kInverse;

    Real fNorm = (Real)0.0;
    int i;
    for (i = 0; i < 4; i++)
    {
        fNorm += m_afTuple[i]*m_afTuple[i];
    }

    if (fNorm > (Real)0.0)
    {
        Real fInvNorm = ((Real)1.0)/fNorm;
        kInverse.m_afTuple[0] = m_afTuple[0]*fInvNorm;
        kInverse.m_afTuple[1] = -m_afTuple[1]*fInvNorm;
        kInverse.m_afTuple[2] = -m_afTuple[2]*fInvNorm;
        kInverse.m_afTuple[3] = -m_afTuple[3]*fInvNorm;
    }
    else
    {
        // return an invalid result to flag the error
        for (i = 0; i < 4; i++)
        {
            kInverse.m_afTuple[i] = (Real)0.0;
        }
    }

    return kInverse;
}

template <class Real>
moQuaternion<Real> moQuaternion<Real>::Conjugate () const
{
    return moQuaternion(m_afTuple[0],-m_afTuple[1],-m_afTuple[2],
        -m_afTuple[3]);
}

template <class Real>
moQuaternion<Real> moQuaternion<Real>::Exp () const
{
    // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
    // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
    // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

    moQuaternion kResult;

    Real fAngle = moMath<Real>::Sqrt(m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2] + m_afTuple[3]*m_afTuple[3]);

    Real fSin = moMath<Real>::Sin(fAngle);
    kResult.m_afTuple[0] = moMath<Real>::Cos(fAngle);

    int i;

    if (moMath<Real>::FAbs(fSin) >= moMath<Real>::ZERO_TOLERANCE)
    {
        Real fCoeff = fSin/fAngle;
        for (i = 1; i <= 3; i++)
        {
            kResult.m_afTuple[i] = fCoeff*m_afTuple[i];
        }
    }
    else
    {
        for (i = 1; i <= 3; i++)
        {
            kResult.m_afTuple[i] = m_afTuple[i];
        }
    }

    return kResult;
}

template <class Real>
moQuaternion<Real> moQuaternion<Real>::Log () const
{
    // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
    // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
    // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

    moQuaternion kResult;
    kResult.m_afTuple[0] = (Real)0.0;

    int i;

    if (moMath<Real>::FAbs(m_afTuple[0]) < (Real)1.0)
    {
        Real fAngle = moMath<Real>::ACos(m_afTuple[0]);
        Real fSin = moMath<Real>::Sin(fAngle);
        if (moMath<Real>::FAbs(fSin) >= moMath<Real>::ZERO_TOLERANCE)
        {
            Real fCoeff = fAngle/fSin;
            for (i = 1; i <= 3; i++)
            {
                kResult.m_afTuple[i] = fCoeff*m_afTuple[i];
            }
            return kResult;
        }
    }

    for (i = 1; i <= 3; i++)
    {
        kResult.m_afTuple[i] = m_afTuple[i];
    }
    return kResult;
}

template <class Real>
moVector3<Real> moQuaternion<Real>::Rotate (const moVector3<Real>& rkVector)
    const
{
    // Given a vector u = (x0,y0,z0) and a unit length quaternion
    // q = <w,x,y,z>, the vector v = (x1,y1,z1) which represents the
    // rotation of u by q is v = q*u*q^{-1} where * indicates quaternion
    // multiplication and where u is treated as the quaternion <0,x0,y0,z0>.
    // Note that q^{-1} = <w,-x,-y,-z>, so no real work is required to
    // invert q.  Now
    //
    //   q*u*q^{-1} = q*<0,x0,y0,z0>*q^{-1}
    //     = q*(x0*i+y0*j+z0*k)*q^{-1}
    //     = x0*(q*i*q^{-1})+y0*(q*j*q^{-1})+z0*(q*k*q^{-1})
    //
    // As 3-vectors, q*i*q^{-1}, q*j*q^{-1}, and 2*k*q^{-1} are the columns
    // of the rotation matrix computed in moQuaternion<Real>::ToRotationMatrix.
    // The vector v is obtained as the product of that rotation matrix with
    // vector u.  As such, the quaternion representation of a rotation
    // matrix requires less space than the matrix and more time to compute
    // the rotated vector.  Typical space-time tradeoff...

    moMatrix3<Real> kRot;
    ToRotationMatrix(kRot);
    return kRot*rkVector;
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::Slerp (Real fT, const moQuaternion& rkP,
    const moQuaternion& rkQ)
{
    Real fCos = rkP.Dot(rkQ);
    Real fAngle = moMath<Real>::ACos(fCos);

    if (moMath<Real>::FAbs(fAngle) >= moMath<Real>::ZERO_TOLERANCE)
    {
        Real fSin = moMath<Real>::Sin(fAngle);
        Real fInvSin = ((Real)1.0)/fSin;
        Real fCoeff0 = moMath<Real>::Sin(((Real)1.0-fT)*fAngle)*fInvSin;
        Real fCoeff1 = moMath<Real>::Sin(fT*fAngle)*fInvSin;
        *this = fCoeff0*rkP + fCoeff1*rkQ;
    }
    else
    {
        *this = rkP;
    }

    return *this;
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::SlerpExtraSpins (Real fT,
    const moQuaternion& rkP, const moQuaternion& rkQ, int iExtraSpins)
{
    Real fCos = rkP.Dot(rkQ);
    Real fAngle = moMath<Real>::ACos(fCos);

    if (moMath<Real>::FAbs(fAngle) >= moMath<Real>::ZERO_TOLERANCE)
    {
        Real fSin = moMath<Real>::Sin(fAngle);
        Real fPhase = moMath<Real>::PI*iExtraSpins*fT;
        Real fInvSin = ((Real)1.0)/fSin;
        Real fCoeff0 = moMath<Real>::Sin(((Real)1.0-fT)*fAngle-fPhase)*fInvSin;
        Real fCoeff1 = moMath<Real>::Sin(fT*fAngle + fPhase)*fInvSin;
        *this = fCoeff0*rkP + fCoeff1*rkQ;
    }
    else
    {
        *this = rkP;
    }

    return *this;
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::Intermediate (const moQuaternion& rkQ0,
    const moQuaternion& rkQ1, const moQuaternion& rkQ2)
{
    // assert:  Q0, Q1, Q2 all unit-length
    moQuaternion kQ1Inv = rkQ1.Conjugate();
    moQuaternion kP0 = kQ1Inv*rkQ0;
    moQuaternion kP2 = kQ1Inv*rkQ2;
    moQuaternion kArg = -((Real)0.25)*(kP0.Log()+kP2.Log());
    moQuaternion kA = rkQ1*kArg.Exp();
    *this = kA;

    return *this;
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::Squad (Real fT, const moQuaternion& rkQ0,
    const moQuaternion& rkA0, const moQuaternion& rkA1, const moQuaternion& rkQ1)
{
    Real fSlerpT = ((Real)2.0)*fT*((Real)1.0-fT);
    moQuaternion kSlerpP = Slerp(fT,rkQ0,rkQ1);
    moQuaternion kSlerpQ = Slerp(fT,rkA0,rkA1);
    return Slerp(fSlerpT,kSlerpP,kSlerpQ);
}

template <class Real>
moQuaternion<Real>& moQuaternion<Real>::Align (const moVector3<Real>& rkV1,
    const moVector3<Real>& rkV2)
{
    // If V1 and V2 are not parallel, the axis of rotation is the unit-length
    // vector U = Cross(V1,V2)/Length(Cross(V1,V2)).  The angle of rotation,
    // A, is the angle between V1 and V2.  The quaternion for the rotation is
    // q = cos(A/2) + sin(A/2)*(ux*i+uy*j+uz*k) where U = (ux,uy,uz).
    //
    // (1) Rather than extract A = acos(Dot(V1,V2)), multiply by 1/2, then
    //     compute sin(A/2) and cos(A/2), we reduce the computational costs by
    //     computing the bisector B = (V1+V2)/Length(V1+V2), so cos(A/2) =
    //     Dot(V1,B).
    //
    // (2) The rotation axis is U = Cross(V1,B)/Length(Cross(V1,B)), but
    //     Length(Cross(V1,B)) = Length(V1)*Length(B)*sin(A/2) = sin(A/2), in
    //     which case sin(A/2)*(ux*i+uy*j+uz*k) = (cx*i+cy*j+cz*k) where
    //     C = Cross(V1,B).
    //
    // If V1 = V2, then B = V1, cos(A/2) = 1, and U = (0,0,0).  If V1 = -V2,
    // then B = 0.  This can happen even if V1 is approximately -V2 using
    // floating point arithmetic, since moVector3::Normalize checks for
    // closeness to zero and returns the zero vector accordingly.  The test
    // for exactly zero is usually not recommend for floating point
    // arithmetic, but the implementation of moVector3::Normalize guarantees
    // the comparison is robust.  In this case, the A = pi and any axis
    // perpendicular to V1 may be used as the rotation axis.

    moVector3<Real> kBisector = rkV1 + rkV2;
    kBisector.Normalize();

    Real fCosHalfAngle = rkV1.Dot(kBisector);
    moVector3<Real> kCross;

    m_afTuple[0] = fCosHalfAngle;

    if (fCosHalfAngle != (Real)0.0)
    {
        kCross = rkV1.Cross(kBisector);
        m_afTuple[1] = kCross.X();
        m_afTuple[2] = kCross.Y();
        m_afTuple[3] = kCross.Z();
    }
    else
    {
        Real fInvLength;
        if (moMath<Real>::FAbs(rkV1[0]) >= moMath<Real>::FAbs(rkV1[1]))
        {
            // V1.x or V1.z is the largest magnitude component
            fInvLength = moMath<Real>::InvSqrt(rkV1[0]*rkV1[0] +
                rkV1[2]*rkV1[2]);

            m_afTuple[1] = -rkV1[2]*fInvLength;
            m_afTuple[2] = (Real)0.0;
            m_afTuple[3] = +rkV1[0]*fInvLength;
        }
        else
        {
            // V1.y or V1.z is the largest magnitude component
            fInvLength = moMath<Real>::InvSqrt(rkV1[1]*rkV1[1] +
                rkV1[2]*rkV1[2]);

            m_afTuple[1] = (Real)0.0;
            m_afTuple[2] = +rkV1[2]*fInvLength;
            m_afTuple[3] = -rkV1[1]*fInvLength;
        }
    }

    return *this;
}

template <class Real>
void moQuaternion<Real>::DecomposeTwistTimesSwing (
    const moVector3<Real>& rkV1, moQuaternion& rkTwist, moQuaternion& rkSwing)
{
    moVector3<Real> kV2 = Rotate(rkV1);
    rkSwing = Align(rkV1,kV2);
    rkTwist = (*this)*rkSwing.Conjugate();
}

template <class Real>
void moQuaternion<Real>::DecomposeSwingTimesTwist (
    const moVector3<Real>& rkV1, moQuaternion& rkSwing, moQuaternion& rkTwist)
{
    moVector3<Real> kV2 = Rotate(rkV1);
    rkSwing = Align(rkV1,kV2);
    rkTwist = rkSwing.Conjugate()*(*this);
}

template<> const moQuaternion<MOfloat>
    moQuaternion<MOfloat>::IDENTITY(1.0f,0.0f,0.0f,0.0f);
template<> const moQuaternion<MOfloat>
    moQuaternion<MOfloat>::ZERO(0.0f,0.0f,0.0f,0.0f);
template<> int moQuaternion<MOfloat>::ms_iNext[3] = { 1, 2, 0 };

template<> const moQuaternion<MOdouble>
    moQuaternion<MOdouble>::IDENTITY(1.0,0.0,0.0,0.0);
template<> const moQuaternion<MOdouble>
    moQuaternion<MOdouble>::ZERO(0.0,0.0,0.0,0.0);
template<> int moQuaternion<MOdouble>::ms_iNext[3] = { 1, 2, 0 };
