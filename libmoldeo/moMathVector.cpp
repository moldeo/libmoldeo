/*******************************************************************************

                                moMathVector.cpp

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

#include "moMathVector.h"

#include "moArray.h"
moDefineDynamicArray(moVector2iArray)
moDefineDynamicArray(moVector2fArray)
moDefineDynamicArray(moVector2fpArray)
moDefineDynamicArray(moVector2dArray)

// moVector2 class ------------------------------------------------------------
/*
template <class Real>
moVector2<Real>::moVector2 ()
{
    // uninitialized for performance in array construction
}


template <class Real>
moVector2<Real>::moVector2 (Real fX, Real fY)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
}

template <class Real>
moVector2<Real>::moVector2 (const Real* afTuple)
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
}

template <class Real>
moVector2<Real>::moVector2 (const moVector2& rkV) : moAbstract(rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
}

template <class Real>
int moVector2<Real>::CompareArrays (const moVector2& rkV) const
{
    return memcmp(m_afTuple,rkV.m_afTuple,2*sizeof(Real));
}

template <class Real>
bool moVector2<Real>::operator== (const moVector2& rkV) const
{
    return CompareArrays(rkV) == 0;
}

template <class Real>
bool moVector2<Real>::operator!= (const moVector2& rkV) const
{
    return CompareArrays(rkV) != 0;
}

template <class Real>
bool moVector2<Real>::operator< (const moVector2& rkV) const
{
    return CompareArrays(rkV) < 0;
}

template <class Real>
bool moVector2<Real>::operator<= (const moVector2& rkV) const
{
    return CompareArrays(rkV) <= 0;
}

template <class Real>
bool moVector2<Real>::operator> (const moVector2& rkV) const
{
    return CompareArrays(rkV) > 0;
}

template <class Real>
bool moVector2<Real>::operator>= (const moVector2& rkV) const
{
    return CompareArrays(rkV) >= 0;
}

template <class Real>
moVector2<Real> moVector2<Real>::operator+ (const moVector2& rkV) const
{
    moVector2<Real> ret(m_afTuple[0]+rkV.m_afTuple[0], m_afTuple[1]+rkV.m_afTuple[1]);
    return ret;
}

template <class Real>
moVector2<Real> moVector2<Real>::operator- (const moVector2& rkV) const
{
    moVector2<Real> ret(m_afTuple[0]-rkV.m_afTuple[0], m_afTuple[1]-rkV.m_afTuple[1]);
    return ret;
}
*/
/*
template <class Real>
moVector2<Real> moVector2<Real>::operator* (Real fScalar) const
{
  moVector2<Real> ret(fScalar*m_afTuple[0], fScalar*m_afTuple[1]);
  return ret;
}

template <class Real>
moVector2<Real> moVector2<Real>::operator/ (Real fScalar) const
{
    moVector2 kQuot;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        kQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
        kQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
    }
    else
    {
        kQuot.m_afTuple[0] = moMath<Real>::MAX_REAL;
        kQuot.m_afTuple[1] = moMath<Real>::MAX_REAL;
    }

    return kQuot;
}

template <class Real>
moVector2<Real> moVector2<Real>::operator- () const
{
  moVector2<Real> ret(-m_afTuple[0], -m_afTuple[1]);
  return ret;
}

template <class Real>
moVector2<Real>& moVector2<Real>::operator/= (Real fScalar)
{
    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        m_afTuple[0] *= fInvScalar;
        m_afTuple[1] *= fInvScalar;
    }
    else
    {
        m_afTuple[0] = moMath<Real>::MAX_REAL;
        m_afTuple[1] = moMath<Real>::MAX_REAL;
    }

    return *this;
}
*/
/*
// vector operations
template <class Real>
Real moVector2<Real>::Length () const
{
    return moMath<Real>::Sqrt(m_afTuple[0]*m_afTuple[0] + m_afTuple[1]*m_afTuple[1]);
}

template <class Real>
Real moVector2<Real>::SquaredLength () const
{
    return m_afTuple[0]*m_afTuple[0] + m_afTuple[1]*m_afTuple[1];
}

template <class Real>
Real moVector2<Real>::Dot (const moVector2& rkV) const
{
    return m_afTuple[0]*rkV.m_afTuple[0] + m_afTuple[1]*rkV.m_afTuple[1];
}
*/
/*
template <class Real>
Real moVector2<Real>::Normalize ()
{
    Real fLength = Length();

    if (fLength > moMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvLength = ((Real)1.0)/fLength;
        m_afTuple[0] *= fInvLength;
        m_afTuple[1] *= fInvLength;
    }
    else
    {
        fLength = (Real)0.0;
        m_afTuple[0] = (Real)0.0;
        m_afTuple[1] = (Real)0.0;
    }

    return fLength;
}

  // returns (y,-x)
template <class Real>
moVector2<Real> moVector2<Real>::Perp () const
{
  moVector2<Real> ret(m_afTuple[1],-m_afTuple[0]);
  return ret;
}

// returns (y,-x)/sqrt(x*x+y*y)
template <class Real>
moVector2<Real> moVector2<Real>::UnitPerp () const
{
    moVector2 kPerp(m_afTuple[1],-m_afTuple[0]);
    kPerp.Normalize();
    return kPerp;
}

// returns DotPerp((x,y),(V.x,V.y)) = x*V.y - y*V.x
template <class Real>
Real moVector2<Real>::DotPerp (const moVector2& rkV) const
{
    return m_afTuple[0]*rkV.m_afTuple[1] - m_afTuple[1]*rkV.m_afTuple[0];
}

template <class Real>
void moVector2<Real>::GetBarycentrics (const moVector2& rkV0, const moVector2& rkV1,
    const moVector2& rkV2, Real afBary[3]) const
{
    // compute the vectors relative to V2 of the triangle
    moVector2 akDiff[3] =
    {
        rkV0 - rkV2,
        rkV1 - rkV2,
        *this - rkV2
    };

    // If the vertices have large magnitude, the linear system of equations
    // for computing barycentric coordinates can be ill-conditioned.  To avoid
    // this, uniformly scale the triangle edges to be of order 1.  The scaling
    // of all differences does not change the barycentric coordinates.
    Real fMax = (Real)0.0;
    int i;
    for (i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
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
        for (i = 0; i < 3; i++)
        {
            akDiff[i] *= fInvMax;
        }
    }

    Real fDet = akDiff[0].DotPerp(akDiff[1]);
    if (moMath<Real>::FAbs(fDet) > moMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvDet = ((Real)1.0)/fDet;
        afBary[0] = akDiff[2].DotPerp(akDiff[1])*fInvDet;
        afBary[1] = akDiff[0].DotPerp(akDiff[2])*fInvDet;
        afBary[2] = (Real)1.0 - afBary[0] - afBary[1];
    }
    else
    {
        // The triangle is a sliver.  Determine the longest edge and
        // compute barycentric coordinates with respect to that edge.
        moVector2 kE2 = rkV0 - rkV1;
        Real fMaxSqrLength = kE2.SquaredLength();
        int iMaxIndex = 2;
        Real fSqrLength = akDiff[1].SquaredLength();
        if (fSqrLength > fMaxSqrLength)
        {
            iMaxIndex = 1;
            fMaxSqrLength = fSqrLength;
        }
        fSqrLength = akDiff[0].SquaredLength();
        if (fSqrLength > fMaxSqrLength)
        {
            iMaxIndex = 0;
            fMaxSqrLength = fSqrLength;
        }

        if (fMaxSqrLength > moMath<Real>::ZERO_TOLERANCE)
        {
            Real fInvSqrLength = ((Real)1.0)/fMaxSqrLength;
            if (iMaxIndex == 0)
            {
                // P-V2 = t(V0-V2)
                afBary[0] = akDiff[2].Dot(akDiff[0])*fInvSqrLength;
                afBary[1] = (Real)0.0;
                afBary[2] = (Real)1.0 - afBary[0];
            }
            else if (iMaxIndex == 1)
            {
                // P-V2 = t(V1-V2)
                afBary[0] = (Real)0.0;
                afBary[1] = akDiff[2].Dot(akDiff[1])*fInvSqrLength;
                afBary[2] = (Real)1.0 - afBary[1];
            }
            else
            {
                // P-V1 = t(V0-V1)
                akDiff[2] = *this - rkV1;
                afBary[0] = akDiff[2].Dot(kE2)*fInvSqrLength;
                afBary[1] = (Real)1.0 - afBary[0];
                afBary[2] = (Real)0.0;
            }
        }
        else
        {
            // triangle is a nearly a point, just return equal weights
            afBary[0] = ((Real)1.0)/(Real)3.0;
            afBary[1] = afBary[0];
            afBary[2] = afBary[0];
        }
    }
}

template <class Real>
void moVector2<Real>::Orthonormalize (moVector2& rkU, moVector2& rkV)
{
    // If the input vectors are v0 and v1, then the Gram-Schmidt
    // orthonormalization produces vectors u0 and u1 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //
    // where |A| indicates length of vector A and A*B indicates dot
    // product of vectors A and B.

    // compute u0
    rkU.Normalize();

    // compute u1
    Real fDot0 = rkU.Dot(rkV);
    rkV -= rkU*fDot0;
    rkV.Normalize();
}

template <class Real>
void moVector2<Real>::GenerateOrthonormalBasis (moVector2& rkU, moVector2& rkV)
{
    rkV.Normalize();
    rkU = rkV.Perp();
}

template <class Real>
void moVector2<Real>::ComputeExtremes (int iVQuantity, const moVector2* akPoint,
    moVector2& rkMin, moVector2& rkMax)
{
    if (!(iVQuantity > 0 && akPoint)) return;

    rkMin = akPoint[0];
    rkMax = rkMin;
    for (int i = 1; i < iVQuantity; i++)
    {
        const moVector2<Real>& rkPoint = akPoint[i];
        for (int j = 0; j < 2; j++)
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

// Cosine between 'this' vector and rkV.
template <class Real>
Real moVector2<Real>::Cosine (const moVector2<Real>& rkV)
{
  Real l = Length();
  Real lv = rkV.Length();
  if ((0 < l) && (0 < lv)) return Dot(rkV) / (l * lv);
  else return 0;
}
// Angle between 'this' vector and rkV.
template <class Real>
Real moVector2<Real>::Angle (const moVector2<Real>& rkV) {
  return moMath<Real>::ACos(Cosine(rkV));
}
*/
template<> const moVector2<MOlong> moVector2<MOlong>::ZERO(0,0);
template<> const moVector2<MOlong> moVector2<MOlong>::UNIT_X(1,0);
template<> const moVector2<MOlong> moVector2<MOlong>::UNIT_Y(0,1);
template<> const moVector2<MOlong> moVector2<MOlong>::ONE(1,1);

template<> const moVector2<MOfloat> moVector2<MOfloat>::ZERO(0.0f,0.0f);
template<> const moVector2<MOfloat> moVector2<MOfloat>::UNIT_X(1.0f,0.0f);
template<> const moVector2<MOfloat> moVector2<MOfloat>::UNIT_Y(0.0f,1.0f);
template<> const moVector2<MOfloat> moVector2<MOfloat>::ONE(1.0f,1.0f);

template<> const moVector2<MOdouble> moVector2<MOdouble>::ZERO(0.0,0.0);
template<> const moVector2<MOdouble> moVector2<MOdouble>::UNIT_X(1.0,0.0);
template<> const moVector2<MOdouble> moVector2<MOdouble>::UNIT_Y(0.0,1.0);
template<> const moVector2<MOdouble> moVector2<MOdouble>::ONE(1.0,1.0);


