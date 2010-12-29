/*******************************************************************************

                                moMathVector3.cpp

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

#include "moMathVector3.h"

#include "moArray.cpp"
moDefineDynamicArray(moVector3iArray)
moDefineDynamicArray(moVector3fArray)
moDefineDynamicArray(moVector3dArray)

// moVector3 class ------------------------------------------------------------
/*
template <class Real>
moVector3<Real>::moVector3 ()
{
    // uninitialized for performance in array construction
}

template <class Real>
moVector3<Real>::moVector3 (Real fX, Real fY, Real fZ)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
    m_afTuple[2] = fZ;
}

template <class Real>
moVector3<Real>::moVector3 (const Real* afTuple)
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
    m_afTuple[2] = afTuple[2];
}
*/
/*
template <class Real>
moVector3<Real>::moVector3 (const moVector3& rkV) : moAbstract(rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
    m_afTuple[2] = rkV.m_afTuple[2];
}

template <class Real>
int moVector3<Real>::CompareArrays (const moVector3& rkV) const
{
    return memcmp(m_afTuple,rkV.m_afTuple,3*sizeof(Real));
}

template <class Real>
bool moVector3<Real>::operator== (const moVector3& rkV) const
{
    return CompareArrays(rkV) == 0;
}

template <class Real>
bool moVector3<Real>::operator!= (const moVector3& rkV) const
{
    return CompareArrays(rkV) != 0;
}

template <class Real>
bool moVector3<Real>::operator< (const moVector3& rkV) const
{
    return CompareArrays(rkV) < 0;
}

template <class Real>
bool moVector3<Real>::operator<= (const moVector3& rkV) const
{
    return CompareArrays(rkV) <= 0;
}

template <class Real>
bool moVector3<Real>::operator> (const moVector3& rkV) const
{
    return CompareArrays(rkV) > 0;
}

template <class Real>
bool moVector3<Real>::operator>= (const moVector3& rkV) const
{
    return CompareArrays(rkV) >= 0;
}

template <class Real>
void moVector3<Real>::GetBarycentrics (const moVector3<Real>& rkV0,
    const moVector3<Real>& rkV1, const moVector3<Real>& rkV2,
    const moVector3<Real>& rkV3, Real afBary[4]) const
{
    // compute the vectors relative to V3 of the tetrahedron
    moVector3<Real> akDiff[4] =
    {
        rkV0 - rkV3,
        rkV1 - rkV3,
        rkV2 - rkV3,
        *this - rkV3
    };

    // If the vertices have large magnitude, the linear system of
    // equations for computing barycentric coordinates can be
    // ill-conditioned.  To avoid this, uniformly scale the tetrahedron
    // edges to be of order 1.  The scaling of all differences does not
    // change the barycentric coordinates.
    Real fMax = (Real)0.0;
    int i;
    for (i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Real fValue = moMath<Real>::FAbs(akDiff[i][j]);
            if (fValue > fMax)
            {
                fMax = fValue;
            }
        }
    }

    // scale down only large data
    if (fMax > (Real)1.0)
    {
        Real fInvMax = ((Real)1.0)/fMax;
        for (i = 0; i < 4; i++)
        {
            akDiff[i] *= fInvMax;
        }
    }

    Real fDet = akDiff[0].Dot(akDiff[1].Cross(akDiff[2]));
    moVector3<Real> kE1cE2 = akDiff[1].Cross(akDiff[2]);
    moVector3<Real> kE2cE0 = akDiff[2].Cross(akDiff[0]);
    moVector3<Real> kE0cE1 = akDiff[0].Cross(akDiff[1]);
    if (moMath<Real>::FAbs(fDet) > moMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvDet = ((Real)1.0)/fDet;
        afBary[0] = akDiff[3].Dot(kE1cE2)*fInvDet;
        afBary[1] = akDiff[3].Dot(kE2cE0)*fInvDet;
        afBary[2] = akDiff[3].Dot(kE0cE1)*fInvDet;
        afBary[3] = (Real)1.0 - afBary[0] - afBary[1] - afBary[2];
    }
    else
    {
        // The tetrahedron is potentially flat.  Determine the face of
        // maximum area and compute barycentric coordinates with respect
        // to that face.
        moVector3<Real> kE02 = rkV0 - rkV2;
        moVector3<Real> kE12 = rkV1 - rkV2;
        moVector3<Real> kE02cE12 = kE02.Cross(kE12);
        Real fMaxSqrArea = kE02cE12.SquaredLength();
        int iMaxIndex = 3;
        Real fSqrArea = kE0cE1.SquaredLength();
        if (fSqrArea > fMaxSqrArea)
        {
            iMaxIndex = 0;
            fMaxSqrArea = fSqrArea;
        }
        fSqrArea = kE1cE2.SquaredLength();
        if (fSqrArea > fMaxSqrArea)
        {
            iMaxIndex = 1;
            fMaxSqrArea = fSqrArea;
        }
        fSqrArea = kE2cE0.SquaredLength();
        if (fSqrArea > fMaxSqrArea)
        {
            iMaxIndex = 2;
            fMaxSqrArea = fSqrArea;
        }

        if (fMaxSqrArea > moMath<Real>::ZERO_TOLERANCE)
        {
            Real fInvSqrArea = ((Real)1.0)/fMaxSqrArea;
            moVector3<Real> kTmp;
            if (iMaxIndex == 0)
            {
                kTmp = akDiff[3].Cross(akDiff[1]);
                afBary[0] = kE0cE1.Dot(kTmp)*fInvSqrArea;
                kTmp = akDiff[0].Cross(akDiff[3]);
                afBary[1] = kE0cE1.Dot(kTmp)*fInvSqrArea;
                afBary[2] = (Real)0.0;
                afBary[3] = (Real)1.0 - afBary[0] - afBary[1];
            }
            else if (iMaxIndex == 1)
            {
                afBary[0] = (Real)0.0;
                kTmp = akDiff[3].Cross(akDiff[2]);
                afBary[1] = kE1cE2.Dot(kTmp)*fInvSqrArea;
                kTmp = akDiff[1].Cross(akDiff[3]);
                afBary[2] = kE1cE2.Dot(kTmp)*fInvSqrArea;
                afBary[3] = (Real)1.0 - afBary[1] - afBary[2];
            }
            else if (iMaxIndex == 2)
            {
                kTmp = akDiff[2].Cross(akDiff[3]);
                afBary[0] = kE2cE0.Dot(kTmp)*fInvSqrArea;
                afBary[1] = (Real)0.0;
                kTmp = akDiff[3].Cross(akDiff[0]);
                afBary[2] = kE2cE0.Dot(kTmp)*fInvSqrArea;
                afBary[3] = (Real)1.0 - afBary[0] - afBary[2];
            }
            else
            {
                akDiff[3] = *this - rkV2;
                kTmp = akDiff[3].Cross(kE12);
                afBary[0] = kE02cE12.Dot(kTmp)*fInvSqrArea;
                kTmp = kE02.Cross(akDiff[3]);
                afBary[1] = kE02cE12.Dot(kTmp)*fInvSqrArea;
                afBary[2] = (Real)1.0 - afBary[0] - afBary[1];
                afBary[3] = (Real)0.0;
            }
        }
        else
        {
            // The tetrahedron is potentially a sliver.  Determine the edge of
            // maximum length and compute barycentric coordinates with respect
            // to that edge.
            Real fMaxSqrLength = akDiff[0].SquaredLength();
            iMaxIndex = 0;  // <V0,V3>
            Real fSqrLength = akDiff[1].SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 1;  // <V1,V3>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = akDiff[2].SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 2;  // <V2,V3>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = kE02.SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 3;  // <V0,V2>
                fMaxSqrLength = fSqrLength;
            }
            fSqrLength = kE12.SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 4;  // <V1,V2>
                fMaxSqrLength = fSqrLength;
            }
            moVector3<Real> kE01 = rkV0 - rkV1;
            fSqrLength = kE01.SquaredLength();
            if (fSqrLength > fMaxSqrLength)
            {
                iMaxIndex = 5;  // <V0,V1>
                fMaxSqrLength = fSqrLength;
            }

            if (fMaxSqrLength > moMath<Real>::ZERO_TOLERANCE)
            {
                Real fInvSqrLength = ((Real)1.0)/fMaxSqrLength;
                if (iMaxIndex == 0)
                {
                    // P-V3 = t*(V0-V3)
                    afBary[0] = akDiff[3].Dot(akDiff[0])*fInvSqrLength;
                    afBary[1] = (Real)0.0;
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)1.0 - afBary[0];
                }
                else if (iMaxIndex == 1)
                {
                    // P-V3 = t*(V1-V3)
                    afBary[0] = (Real)0.0;
                    afBary[1] = akDiff[3].Dot(akDiff[1])*fInvSqrLength;
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)1.0 - afBary[1];
                }
                else if (iMaxIndex == 2)
                {
                    // P-V3 = t*(V2-V3)
                    afBary[0] = (Real)0.0;
                    afBary[1] = (Real)0.0;
                    afBary[2] = akDiff[3].Dot(akDiff[2])*fInvSqrLength;
                    afBary[3] = (Real)1.0 - afBary[2];
                }
                else if (iMaxIndex == 3)
                {
                    // P-V2 = t*(V0-V2)
                    akDiff[3] = *this - rkV2;
                    afBary[0] = akDiff[3].Dot(kE02)*fInvSqrLength;
                    afBary[1] = (Real)0.0;
                    afBary[2] = (Real)1.0 - afBary[0];
                    afBary[3] = (Real)0.0;
                }
                else if (iMaxIndex == 4)
                {
                    // P-V2 = t*(V1-V2)
                    akDiff[3] = *this - rkV2;
                    afBary[0] = (Real)0.0;
                    afBary[1] = akDiff[3].Dot(kE12)*fInvSqrLength;
                    afBary[2] = (Real)1.0 - afBary[1];
                    afBary[3] = (Real)0.0;
                }
                else
                {
                    // P-V1 = t*(V0-V1)
                    akDiff[3] = *this - rkV1;
                    afBary[0] = akDiff[3].Dot(kE01)*fInvSqrLength;
                    afBary[1] = (Real)1.0 - afBary[0];
                    afBary[2] = (Real)0.0;
                    afBary[3] = (Real)0.0;
                }
            }
            else
            {
                // tetrahedron is a nearly a point, just return equal weights
                afBary[0] = (Real)0.25;
                afBary[1] = afBary[0];
                afBary[2] = afBary[0];
                afBary[3] = afBary[0];
            }
        }
    }
}

template <class Real>
void moVector3<Real>::Orthonormalize (moVector3& rkU, moVector3& rkV, moVector3& rkW)
{
    // If the input vectors are v0, v1, and v2, then the Gram-Schmidt
    // orthonormalization produces vectors u0, u1, and u2 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //   u2 = (v2-(u0*v2)u0-(u1*v2)u1)/|v2-(u0*v2)u0-(u1*v2)u1|
    //
    // where |A| indicates length of vector A and A*B indicates dot
    // product of vectors A and B.

    // compute u0
    rkU.Normalize();

    // compute u1
    Real fDot0 = rkU.Dot(rkV);
    rkV -= fDot0*rkU;
    rkV.Normalize();

    // compute u2
    Real fDot1 = rkV.Dot(rkW);
    fDot0 = rkU.Dot(rkW);
    rkW -= fDot0*rkU + fDot1*rkV;
    rkW.Normalize();
}

template <class Real>
void moVector3<Real>::Orthonormalize (moVector3* akV)
{
    Orthonormalize(akV[0],akV[1],akV[2]);
}

template <class Real>
void moVector3<Real>::GenerateOrthonormalBasis (moVector3& rkU, moVector3& rkV,
    moVector3& rkW)
{
    rkW.Normalize();
    GenerateComplementBasis(rkU,rkV,rkW);
}

template <class Real>
void moVector3<Real>::GenerateComplementBasis (moVector3& rkU, moVector3& rkV,
    const moVector3& rkW)
{
    Real fInvLength;

    if (moMath<Real>::FAbs(rkW.m_afTuple[0]) >=
        moMath<Real>::FAbs(rkW.m_afTuple[1]) )
    {
        // W.x or W.z is the largest magnitude component, swap them
        fInvLength = moMath<Real>::InvSqrt(rkW.m_afTuple[0]*rkW.m_afTuple[0] +
            rkW.m_afTuple[2]*rkW.m_afTuple[2]);
        rkU.m_afTuple[0] = -rkW.m_afTuple[2]*fInvLength;
        rkU.m_afTuple[1] = (Real)0.0;
        rkU.m_afTuple[2] = +rkW.m_afTuple[0]*fInvLength;
        rkV.m_afTuple[0] = rkW.m_afTuple[1]*rkU.m_afTuple[2];
        rkV.m_afTuple[1] = rkW.m_afTuple[2]*rkU.m_afTuple[0] -
            rkW.m_afTuple[0]*rkU.m_afTuple[2];
        rkV.m_afTuple[2] = -rkW.m_afTuple[1]*rkU.m_afTuple[0];
    }
    else
    {
        // W.y or W.z is the largest magnitude component, swap them
        fInvLength = moMath<Real>::InvSqrt(rkW.m_afTuple[1]*rkW.m_afTuple[1] +
            rkW.m_afTuple[2]*rkW.m_afTuple[2]);
        rkU.m_afTuple[0] = (Real)0.0;
        rkU.m_afTuple[1] = +rkW.m_afTuple[2]*fInvLength;
        rkU.m_afTuple[2] = -rkW.m_afTuple[1]*fInvLength;
        rkV.m_afTuple[0] = rkW.m_afTuple[1]*rkU.m_afTuple[2] -
            rkW.m_afTuple[2]*rkU.m_afTuple[1];
        rkV.m_afTuple[1] = -rkW.m_afTuple[0]*rkU.m_afTuple[2];
        rkV.m_afTuple[2] = rkW.m_afTuple[0]*rkU.m_afTuple[1];
    }
}

template <class Real>
void moVector3<Real>::ComputeExtremes (int iVQuantity, const moVector3* akPoint,
    moVector3& rkMin, moVector3& rkMax)
{
    if (!(iVQuantity > 0 && akPoint)) return;

    rkMin = akPoint[0];
    rkMax = rkMin;
    for (int i = 1; i < iVQuantity; i++)
    {
        const moVector3<Real>& rkPoint = akPoint[i];
        for (int j = 0; j < 3; j++)
        {
            if (rkPoint[j] < rkMin[j])
            {
                rkMin[j] = rkPoint[j];
            }
            else if (rkPoint[j] > rkMax[j])
            {
                rkMax[j] = rkPoint[j];
            }
        }
    }
}


// arithmetic operations
template <class Real>
moVector3<Real> moVector3<Real>::operator+ (const moVector3& rkV) const
{
    moVector3<Real> ret(m_afTuple[0]+rkV.m_afTuple[0], m_afTuple[1]+rkV.m_afTuple[1], m_afTuple[2]+rkV.m_afTuple[2]);
    return ret;
}


template <class Real>
moVector3<Real> moVector3<Real>::operator- (const moVector3& rkV) const
{
    return moVector3<Real>(m_afTuple[0]-rkV.m_afTuple[0],m_afTuple[1]-rkV.m_afTuple[1],m_afTuple[2]-rkV.m_afTuple[2]);
}


template <class Real>
moVector3<Real> moVector3<Real>::operator* (Real fScalar) const
{
    return moVector3<Real>(
        fScalar*m_afTuple[0],
        fScalar*m_afTuple[1],
        fScalar*m_afTuple[2]);
}


template <class Real>
moVector3<Real> moVector3<Real>::operator/ (Real fScalar) const
{
    moVector3<Real> kQuot;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        kQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        kQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
        kQuot.m_afTuple[2] = fInvScalar*m_afTuple[2];
    }
    else
    {
        kQuot.m_afTuple[0] = moMath<Real>::MAX_REAL;
        kQuot.m_afTuple[1] = moMath<Real>::MAX_REAL;
        kQuot.m_afTuple[2] = moMath<Real>::MAX_REAL;
    }

    return kQuot;
}
template <class Real>
moVector3<Real> moVector3<Real>::operator- () const
{
    return moVector3<Real>(-m_afTuple[0], -m_afTuple[1], -m_afTuple[2]);
}

// arithmetic updates
template <class Real>
moVector3<Real>& moVector3<Real>::operator+= (const moVector3& rkV)
{
    m_afTuple[0] += rkV.m_afTuple[0];
    m_afTuple[1] += rkV.m_afTuple[1];
    m_afTuple[2] += rkV.m_afTuple[2];
    return *this;
}
template <class Real>
moVector3<Real>& moVector3<Real>::operator-= (const moVector3& rkV)
{
    m_afTuple[0] -= rkV.m_afTuple[0];
    m_afTuple[1] -= rkV.m_afTuple[1];
    m_afTuple[2] -= rkV.m_afTuple[2];
    return *this;
}
template <class Real>
moVector3<Real>& moVector3<Real>::operator*= (const Real fScalar)
{
    m_afTuple[0] *= fScalar;
    m_afTuple[1] *= fScalar;
    m_afTuple[2] *= fScalar;
    return *this;
}
template <class Real>
moVector3<Real>& moVector3<Real>::operator/= (const Real fScalar)
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
        m_afTuple[2] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = moMath<Real>::MAX_REAL;
        m_afTuple[1] = moMath<Real>::MAX_REAL;
        m_afTuple[2] = moMath<Real>::MAX_REAL;
    }

    return *this;
}
*/
/*
// vector operations
template <class Real>
Real moVector3<Real>::Length () const
{
    return moMath<Real>::Sqrt(
  m_afTuple[0]*m_afTuple[0] +
  m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2]);
}


template <class Real>
Real moVector3<Real>::SquaredLength () const
{
    return
        m_afTuple[0]*m_afTuple[0] +
        m_afTuple[1]*m_afTuple[1] +
        m_afTuple[2]*m_afTuple[2];
}
template <class Real>
Real moVector3<Real>::Dot (const moVector3& rkV) const
{
    return
        m_afTuple[0]*rkV.m_afTuple[0] +
        m_afTuple[1]*rkV.m_afTuple[1] +
        m_afTuple[2]*rkV.m_afTuple[2];
}


template <class Real>
Real moVector3<Real>::Normalize ()
{
    Real fLength = Length();

    if (fLength > moMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
        m_afTuple[2] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
        m_afTuple[2] = (Real)0.0;
    }

    return fLength;
}

// The cross products are computed using the right-handed rule.  Be aware
// that some graphics APIs use a left-handed rule.  If you have to compute
// a cross product with these functions and send the result to the API
// that expects left-handed, you will need to change sign on the vector
// (replace each component value c by -c).

template <class Real>
moVector3<Real> moVector3<Real>::Cross (const moVector3& rkV) const
{
    moVector3<Real> kCross(
        m_afTuple[1]*rkV.m_afTuple[2] - m_afTuple[2]*rkV.m_afTuple[1],
        m_afTuple[2]*rkV.m_afTuple[0] - m_afTuple[0]*rkV.m_afTuple[2],
        m_afTuple[0]*rkV.m_afTuple[1] - m_afTuple[1]*rkV.m_afTuple[0]);
    return kCross;
}

template <class Real>
moVector3<Real> moVector3<Real>::UnitCross (const moVector3& rkV) const
{
    moVector3<Real> kCross(
        m_afTuple[1]*rkV.m_afTuple[2] - m_afTuple[2]*rkV.m_afTuple[1],
        m_afTuple[2]*rkV.m_afTuple[0] - m_afTuple[0]*rkV.m_afTuple[2],
        m_afTuple[0]*rkV.m_afTuple[1] - m_afTuple[1]*rkV.m_afTuple[0]);
     kCross.Normalize();
     return kCross;
}


// Cosine between 'this' vector and rkV.
template <class Real>
Real moVector3<Real>::Cosine (const moVector3<Real>& rkV)
{
  Real l = Length();
  Real lv = rkV.Length();
  if ((0 < l) && (0 < lv)) return Dot(rkV) / (l * lv);
  else return 0;
}
// Angle between 'this' vector and rkV.
template <class Real>
Real moVector3<Real>::Angle (const moVector3<Real>& rkV) {
  return moMath<Real>::ACos(Cosine(rkV));
}
*/
template<> const moVector3<MOlong> moVector3<MOlong>::ZERO(0,0,0);
template<> const moVector3<MOlong> moVector3<MOlong>::UNIT_X(1,0,0);
template<> const moVector3<MOlong> moVector3<MOlong>::UNIT_Y(0,1,0);
template<> const moVector3<MOlong> moVector3<MOlong>::UNIT_Z(0,0,1);
template<> const moVector3<MOlong> moVector3<MOlong>::ONE(1,1,1);

template<> const moVector3<MOfloat> moVector3<MOfloat>::ZERO(0.0f,0.0f,0.0f);
template<> const moVector3<MOfloat> moVector3<MOfloat>::UNIT_X(1.0f,0.0f,0.0f);
template<> const moVector3<MOfloat> moVector3<MOfloat>::UNIT_Y(0.0f,1.0f,0.0f);
template<> const moVector3<MOfloat> moVector3<MOfloat>::UNIT_Z(0.0f,0.0f,1.0f);
template<> const moVector3<MOfloat> moVector3<MOfloat>::ONE(1.0f,1.0f,1.0f);

template<> const moVector3<MOdouble> moVector3<MOdouble>::ZERO(0.0,0.0,0.0);
template<> const moVector3<MOdouble> moVector3<MOdouble>::UNIT_X(1.0,0.0,0.0);
template<> const moVector3<MOdouble> moVector3<MOdouble>::UNIT_Y(0.0,1.0,0.0);
template<> const moVector3<MOdouble> moVector3<MOdouble>::UNIT_Z(0.0,0.0,1.0);
template<> const moVector3<MOdouble> moVector3<MOdouble>::ONE(1.0,1.0,1.0);

