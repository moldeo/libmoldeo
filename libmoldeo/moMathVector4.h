/*******************************************************************************

                                moMathVector4.h

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

#ifndef __MO_MATH_VECTOR4_H__
#define __MO_MATH_VECTOR4_H__

// moVector4 class ------------------------------------------------------------

template <class Real>
class LIBMOLDEO_API moVector4 : public moAbstract
{
public:
    // construction
    moVector4 () {}  // uninitialized
    moVector4 (Real fX, Real fY, Real fZ, Real fW) {
        m_afTuple[0] = fX;
        m_afTuple[1] = fY;
        m_afTuple[2] = fZ;
        m_afTuple[3] = fW;
    }
    moVector4 (const Real* afTuple) {
        m_afTuple[0] = afTuple[0];
        m_afTuple[1] = afTuple[1];
        m_afTuple[2] = afTuple[2];
        m_afTuple[3] = afTuple[3];
    }
    moVector4 (const moVector4 & rkV) {
        m_afTuple[0] = rkV.m_afTuple[0];
        m_afTuple[1] = rkV.m_afTuple[1];
        m_afTuple[2] = rkV.m_afTuple[2];
        m_afTuple[3] = rkV.m_afTuple[3];
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
    inline Real Z () const { return m_afTuple[2]; }
    inline Real& Z () { return m_afTuple[2]; }
    inline Real W () const { return m_afTuple[3]; }
    inline Real& W () { return m_afTuple[3]; }

    // assignment
    inline moVector4 & operator= (const moVector4 & rkV)
    {
        m_afTuple[0] = rkV.m_afTuple[0];
        m_afTuple[1] = rkV.m_afTuple[1];
        m_afTuple[2] = rkV.m_afTuple[2];
        m_afTuple[3] = rkV.m_afTuple[3];
        return *this;
    }

    // comparison
    bool operator== (const moVector4 & rkV) const { return CompareArrays(rkV) == 0; }
    bool operator!= (const moVector4 & rkV) const { return CompareArrays(rkV) != 0; }
    bool operator<  (const moVector4 & rkV) const { return CompareArrays(rkV) < 0; }
    bool operator<= (const moVector4 & rkV) const { return CompareArrays(rkV) <= 0; }
    bool operator>  (const moVector4 & rkV) const { return CompareArrays(rkV) > 0; }
    bool operator>= (const moVector4 & rkV) const { return CompareArrays(rkV) >= 0; }

    // arithmetic operations
    inline moVector4 operator+ (const moVector4 & rkV) const
    {
        return moVector4 (
            m_afTuple[0]+rkV.m_afTuple[0],
            m_afTuple[1]+rkV.m_afTuple[1],
            m_afTuple[2]+rkV.m_afTuple[2],
            m_afTuple[3]+rkV.m_afTuple[3]);
    }
    inline moVector4 operator- (const moVector4 & rkV) const
    {
        return moVector4 (
            m_afTuple[0]-rkV.m_afTuple[0],
            m_afTuple[1]-rkV.m_afTuple[1],
            m_afTuple[2]-rkV.m_afTuple[2],
            m_afTuple[3]-rkV.m_afTuple[3]);
    }
    inline moVector4 operator* (Real fScalar) const
    {
        return moVector4 (
            fScalar*m_afTuple[0],
            fScalar*m_afTuple[1],
            fScalar*m_afTuple[2],
            fScalar*m_afTuple[3]);
    }
    inline moVector4 operator/ (Real fScalar) const
    {
        moVector4 kQuot;

        if (fScalar != (Real)0.0)
        {
            Real fInvScalar = ((Real)1.0)/fScalar;
            kQuot.m_afTuple[0] = fInvScalar*m_afTuple[0];
            kQuot.m_afTuple[1] = fInvScalar*m_afTuple[1];
            kQuot.m_afTuple[2] = fInvScalar*m_afTuple[2];
            kQuot.m_afTuple[3] = fInvScalar*m_afTuple[3];
        }
        else
        {
            kQuot.m_afTuple[0] = moMath<Real>::MAX_REAL;
            kQuot.m_afTuple[1] = moMath<Real>::MAX_REAL;
            kQuot.m_afTuple[2] = moMath<Real>::MAX_REAL;
            kQuot.m_afTuple[3] = moMath<Real>::MAX_REAL;
        }

        return kQuot;
    }
    inline moVector4 operator- () const
    {
        return moVector4(
            -m_afTuple[0],
            -m_afTuple[1],
            -m_afTuple[2],
            -m_afTuple[3]);
    }

    // arithmetic updates
    inline moVector4 & operator+= (const moVector4 & rkV)
    {
        m_afTuple[0] += rkV.m_afTuple[0];
        m_afTuple[1] += rkV.m_afTuple[1];
        m_afTuple[2] += rkV.m_afTuple[2];
        m_afTuple[3] += rkV.m_afTuple[3];
        return *this;
    }
    inline moVector4 & operator-= (const moVector4 & rkV)
    {
        m_afTuple[0] -= rkV.m_afTuple[0];
        m_afTuple[1] -= rkV.m_afTuple[1];
        m_afTuple[2] -= rkV.m_afTuple[2];
        m_afTuple[3] -= rkV.m_afTuple[3];
        return *this;
    }
    inline moVector4 & operator*= (Real fScalar)
    {
        m_afTuple[0] *= fScalar;
        m_afTuple[1] *= fScalar;
        m_afTuple[2] *= fScalar;
        m_afTuple[3] *= fScalar;
        return *this;
    }
    inline moVector4 & operator/= (Real fScalar)
    {
        if (fScalar != (Real)0.0)
        {
            Real fInvScalar = ((Real)1.0)/fScalar;
            m_afTuple[0] *= fInvScalar;
            m_afTuple[1] *= fInvScalar;
            m_afTuple[2] *= fInvScalar;
            m_afTuple[3] *= fInvScalar;
        }
        else
        {
            m_afTuple[0] = moMath<Real>::MAX_REAL;
            m_afTuple[1] = moMath<Real>::MAX_REAL;
            m_afTuple[2] = moMath<Real>::MAX_REAL;
            m_afTuple[3] = moMath<Real>::MAX_REAL;
        }

        return *this;
    }

    // vector operations
    inline Real Length () const
    {
        return moMath<Real>::Sqrt(
            m_afTuple[0]*m_afTuple[0] +
            m_afTuple[1]*m_afTuple[1] +
            m_afTuple[2]*m_afTuple[2] +
            m_afTuple[3]*m_afTuple[3]);
    }
    inline Real SquaredLength () const
    {
        return
            m_afTuple[0]*m_afTuple[0] +
            m_afTuple[1]*m_afTuple[1] +
            m_afTuple[2]*m_afTuple[2] +
            m_afTuple[3]*m_afTuple[3];
    }
    inline Real Dot (const moVector4 & rkV) const
    {
        return
            m_afTuple[0]*rkV.m_afTuple[0] +
            m_afTuple[1]*rkV.m_afTuple[1] +
            m_afTuple[2]*rkV.m_afTuple[2] +
            m_afTuple[3]*rkV.m_afTuple[3];
    }
    inline Real Normalize ()
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

    // Cosine between 'this' vector and rkV.
    inline Real Cosine (const moVector4<Real>& rkV)
    {
	    Real l = Length();
	    Real lv = rkV.Length();
	    if ((0 < l) && (0 < lv)) return Dot(rkV) / (l * lv);
	    else return 0;
    }
    // Angle between 'this' vector and rkV.
    inline Real Angle (const moVector4<Real>& rkV) {

      return moMath<Real>::ACos(Cosine(rkV));
    }

    moText ToJSON() const {
      moText JSON = "[";
      moText comma="";
      for(int i=0;i<4; i++) {
        JSON+= comma + FloatToStr( (*this)[i] );
        comma=",";
      }
      JSON+= "]";
      return JSON;
    }

    // special vectors
    static const moVector4 ZERO;
    static const moVector4 UNIT_X;  // (1,0,0,0)
    static const moVector4 UNIT_Y;  // (0,1,0,0)
    static const moVector4 UNIT_Z;  // (0,0,1,0)
    static const moVector4 UNIT_W;  // (0,0,0,1)
    static const moVector4 ONE;     // (1,1,1,1)

private:
    // support for comparisons
    int CompareArrays (const moVector4 & rkV) const {
        return memcmp(m_afTuple,rkV.m_afTuple,4*sizeof(Real));
    }

    Real m_afTuple[4];
};


// arithmetic operations
template <class Real>
inline moVector4<Real> operator* (Real fScalar, const moVector4<Real>& rkV)
{
    moVector4<Real> v4(fScalar*rkV[0], fScalar*rkV[1], fScalar*rkV[2], fScalar*rkV[3]);
    return v4;
}

#ifndef MO_MACOSX
#ifndef MO_WIN32
#ifndef MO_RASPBIAN
template class LIBMOLDEO_API moVector4<MOlong>;
template class LIBMOLDEO_API moVector4<MOfloat>;
template class LIBMOLDEO_API moVector4<MOdouble>;
#endif
#endif
#endif

typedef moVector4<MOlong> moVector4i;
typedef moVector4<MOfloat> moVector4f;
typedef moVector4<MOdouble> moVector4d;

moDeclareExportedDynamicArray( moVector4i, moVector4iArray );
moDeclareExportedDynamicArray( moVector4f, moVector4fArray );
moDeclareExportedDynamicArray( moVector4d, moVector4dArray );


#endif
