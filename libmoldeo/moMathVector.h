/*******************************************************************************

                                moMathVector.h

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

#include "moMath.h"

#ifndef __MO_MATH_VECTOR_H__
#define __MO_MATH_VECTOR_H__

// moVector2 class ------------------------------------------------------------

template <class Real>
class LIBMOLDEO_API moVector2 : public moAbstract
{
public:
    // construction
    moVector2 () {}  // uninitialized

    moVector2 (Real fX, Real fY)
    {
        m_afTuple[0] = fX;
        m_afTuple[1] = fY;
    }


    moVector2 (const Real* afTuple)
    {
        m_afTuple[0] = afTuple[0];
        m_afTuple[1] = afTuple[1];
    }


    moVector2 (const moVector2& rkV) : moAbstract(rkV)
    {
        m_afTuple[0] = rkV.m_afTuple[0];
        m_afTuple[1] = rkV.m_afTuple[1];
    }

    // coordinate access
	inline operator const Real* () const { return m_afTuple; }
	inline operator Real* () { return m_afTuple; }
	inline Real operator[] (int i) const { return m_afTuple[i]; }
	inline Real& operator[] (int i) { return m_afTuple[i]; }
	inline Real X () const { return m_afTuple[0]; }
	inline Real& X () { return m_afTuple[0]; }
    inline Real Y () const { return m_afTuple[1]; }
    inline Real& Y () { return m_afTuple[1]; }

    // assignment
    inline moVector2& operator= (const moVector2& rkV)
    {
        m_afTuple[0] = rkV.m_afTuple[0];
        m_afTuple[1] = rkV.m_afTuple[1];
        return *this;
    }

    // comparison
    bool operator== (const moVector2& rkV) const { return CompareArrays(rkV) == 0; }
    bool operator!= (const moVector2& rkV) const { return CompareArrays(rkV) != 0; }
    bool operator<  (const moVector2& rkV) const { return CompareArrays(rkV) < 0; }
    bool operator<= (const moVector2& rkV) const { return CompareArrays(rkV) <= 0; }
    bool operator>  (const moVector2& rkV) const { return CompareArrays(rkV) > 0; }
    bool operator>= (const moVector2& rkV) const { return CompareArrays(rkV) >= 0; }

    // arithmetic operations
    moVector2 operator+ (const moVector2& rkV) const
    {
        return moVector2(m_afTuple[0]+rkV.m_afTuple[0], m_afTuple[1]+rkV.m_afTuple[1]);
    }

    moVector2 operator- (const moVector2& rkV) const
    {
        return moVector2(m_afTuple[0]-rkV.m_afTuple[0], m_afTuple[1]-rkV.m_afTuple[1]);
    }
    moVector2 operator* (Real fScalar) const {
      moVector2<Real> ret(fScalar*m_afTuple[0], fScalar*m_afTuple[1]);
      return ret;
    }
    moVector2 operator/ (Real fScalar) const {
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
    moVector2 operator- () const {
        moVector2<Real> ret(-m_afTuple[0], -m_afTuple[1]);
        return ret;
    }

    // arithmetic updates
    inline moVector2& operator+= (const moVector2& rkV)
    {
        m_afTuple[0] += rkV.m_afTuple[0];
        m_afTuple[1] += rkV.m_afTuple[1];
        return *this;
    }
    inline moVector2& operator-= (const moVector2& rkV)
    {
        m_afTuple[0] -= rkV.m_afTuple[0];
        m_afTuple[1] -= rkV.m_afTuple[1];
        return *this;
    }
    inline moVector2& operator*= (Real fScalar)
    {
        m_afTuple[0] *= fScalar;
        m_afTuple[1] *= fScalar;
        return *this;
    }
    moVector2& operator/= (Real fScalar) {
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

    // vector operations
    Real Length () const {
        return moMath<Real>::Sqrt(m_afTuple[0]*m_afTuple[0] + m_afTuple[1]*m_afTuple[1]);
    }
    Real SquaredLength () const {
        return m_afTuple[0]*m_afTuple[0] + m_afTuple[1]*m_afTuple[1];
    }
    Real Dot (const moVector2& rkV) const {
        return m_afTuple[0]*rkV.m_afTuple[0] + m_afTuple[1]*rkV.m_afTuple[1];
    }
    Real Normalize () {
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

    /// returns (y,-x)
    moVector2 Perp () const {
        moVector2<Real> ret(m_afTuple[1],-m_afTuple[0]);
        return ret;
    }

    /// returns (y,-x)/sqrt(x*x+y*y)
    moVector2 UnitPerp () const {
        moVector2 kPerp(m_afTuple[1],-m_afTuple[0]);
        kPerp.Normalize();
        return kPerp;
    }

    /// returns DotPerp((x,y),(V.x,V.y)) = x*V.y - y*V.x
    Real DotPerp (const moVector2& rkV) const {
        return m_afTuple[0]*rkV.m_afTuple[1] - m_afTuple[1]*rkV.m_afTuple[0];
    }

    // Compute the barycentric coordinates of the point with respect to the
    // triangle <V0,V1,V2>, P = b0*V0 + b1*V1 + b2*V2, where b0 + b1 + b2 = 1.
    void GetBarycentrics (const moVector2& rkV0, const moVector2& rkV1,
        const moVector2& rkV2, Real afBary[3]) const {
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

    // Gram-Schmidt orthonormalization.  Take linearly independent vectors U
    // and V and compute an orthonormal set (unit length, mutually
    // perpendicular).
    static void Orthonormalize (moVector2& rkU, moVector2& rkV) {
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

    // Input V must be a nonzero vector.  The output is an orthonormal basis
    // {U,V}.  The input V is normalized by this function.  If you know V is
    // already unit length, use U = V.Perp().
    static void GenerateOrthonormalBasis (moVector2& rkU, moVector2& rkV) {

        rkV.Normalize();
        rkU = rkV.Perp();

    }

    // Compute the extreme values.
    static void ComputeExtremes (int iVQuantity, const moVector2* akPoint,
        moVector2& rkMin, moVector2& rkMax) {
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
    Real Cosine (const moVector2<Real>& rkV) {
      Real l = Length();
      Real lv = rkV.Length();
      if ((0 < l) && (0 < lv)) return Dot(rkV) / (l * lv);
      else return 0;
    }
    // Angle between 'this' vector and rkV.
	  Real Angle (const moVector2<Real>& rkV) {
	      return moMath<Real>::ACos(Cosine(rkV));
	  }

    moText ToJSON() const {
      moText JSON = "[";
      moText comma="";
      for(int i=0;i<2; i++) {
        JSON+= comma + FloatToStr( (*this)[i] );
        comma=",";
      }
      JSON+= "]";
      return JSON;
    }

    // special vectors
    static const moVector2 ZERO;    // (0,0)
    static const moVector2 UNIT_X;  // (1,0)
    static const moVector2 UNIT_Y;  // (0,1)
    static const moVector2 ONE;     // (1,1)

private:
    // support for comparisons
    int CompareArrays (const moVector2& rkV) const
    {
        return memcmp(m_afTuple,rkV.m_afTuple,2*sizeof(Real));
    }

    Real m_afTuple[2];
};

// arithmetic operations

template <class Real>
inline moVector2<Real> operator* (Real fScalar, const moVector2<Real>& rkV)
{
	return moVector2<Real>(fScalar*rkV[0], fScalar*rkV[1]);
}

#ifndef MO_MACOSX
#ifndef MO_RASPBIAN
template class LIBMOLDEO_API moVector2<MOlong>;
template class LIBMOLDEO_API moVector2<MOfloat>;
template class LIBMOLDEO_API moVector2<MOdouble>;
#endif // MO_RASPBIAN
#endif // MO_MACOSX

typedef moVector2<MOlong> moVector2i;
typedef moVector2<MOfloat> moVector2f;
typedef moVector2<MOdouble> moVector2d;

moDeclareExportedDynamicArray( moVector2i, moVector2iArray );
moDeclareExportedDynamicArray( moVector2f, moVector2fArray );
moDeclareExportedDynamicArray( moVector2f*, moVector2fpArray );
moDeclareExportedDynamicArray( moVector2d, moVector2dArray );



#endif
