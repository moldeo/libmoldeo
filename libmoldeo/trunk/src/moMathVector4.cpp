/*******************************************************************************

                                moMathVector4.cpp

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

#include "moMathVector4.h"

#include "moArray.cpp"
moDefineDynamicArray(moVector4fArray)
moDefineDynamicArray(moVector4dArray)

// moVector4 class ------------------------------------------------------------
/*
template <class Real>
moVector4<Real>::moVector4 ()
{
    // uninitialized for performance in array construction
}

template <class Real>
moVector4<Real>::moVector4 (Real fX, Real fY, Real fZ, Real fW)
{
    m_afTuple[0] = fX;
    m_afTuple[1] = fY;
    m_afTuple[2] = fZ;
    m_afTuple[3] = fW;
}

template <class Real>
moVector4<Real>::moVector4 (const Real* afTuple)
{
    m_afTuple[0] = afTuple[0];
    m_afTuple[1] = afTuple[1];
    m_afTuple[2] = afTuple[2];
    m_afTuple[3] = afTuple[3];
}

template <class Real>
moVector4<Real>::moVector4 (const moVector4& rkV) : moAbstract(rkV)
{
    m_afTuple[0] = rkV.m_afTuple[0];
    m_afTuple[1] = rkV.m_afTuple[1];
    m_afTuple[2] = rkV.m_afTuple[2];
    m_afTuple[3] = rkV.m_afTuple[3];
}

template <class Real>
int moVector4<Real>::CompareArrays (const moVector4& rkV) const
{
    return memcmp(m_afTuple,rkV.m_afTuple,4*sizeof(Real));
}

template <class Real>
bool moVector4<Real>::operator== (const moVector4& rkV) const
{
    return CompareArrays(rkV) == 0;
}

template <class Real>
bool moVector4<Real>::operator!= (const moVector4& rkV) const
{
    return CompareArrays(rkV) != 0;
}

template <class Real>
bool moVector4<Real>::operator< (const moVector4& rkV) const
{
    return CompareArrays(rkV) < 0;
}

template <class Real>
bool moVector4<Real>::operator<= (const moVector4& rkV) const
{
    return CompareArrays(rkV) <= 0;
}

template <class Real>
bool moVector4<Real>::operator> (const moVector4& rkV) const
{
    return CompareArrays(rkV) > 0;
}

template <class Real>
bool moVector4<Real>::operator>= (const moVector4& rkV) const
{
    return CompareArrays(rkV) >= 0;
}
*/
template<> const moVector4<MOfloat> moVector4<MOfloat>::ZERO(0.0f,0.0f,0.0f,0.0f);
template<> const moVector4<MOfloat> moVector4<MOfloat>::UNIT_X(1.0f,0.0f,0.0f,0.0f);
template<> const moVector4<MOfloat> moVector4<MOfloat>::UNIT_Y(0.0f,1.0f,0.0f,0.0f);
template<> const moVector4<MOfloat> moVector4<MOfloat>::UNIT_Z(0.0f,0.0f,1.0f,0.0f);
template<> const moVector4<MOfloat> moVector4<MOfloat>::UNIT_W(0.0f,0.0f,0.0f,1.0f);
template<> const moVector4<MOfloat> moVector4<MOfloat>::ONE(1.0f,1.0f,1.0f,1.0f);

template<> const moVector4<MOdouble> moVector4<MOdouble>::ZERO(0.0,0.0,0.0,0.0);
template<> const moVector4<MOdouble> moVector4<MOdouble>::UNIT_X(1.0,0.0,0.0,0.0);
template<> const moVector4<MOdouble> moVector4<MOdouble>::UNIT_Y(0.0,1.0,0.0,0.0);
template<> const moVector4<MOdouble> moVector4<MOdouble>::UNIT_Z(0.0,0.0,1.0,0.0);
template<> const moVector4<MOdouble> moVector4<MOdouble>::UNIT_W(0.0,0.0,0.0,1.0);
template<> const moVector4<MOdouble> moVector4<MOdouble>::ONE(1.0,1.0,1.0,1.0);
