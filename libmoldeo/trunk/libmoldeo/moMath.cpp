/*******************************************************************************

                                moMath.cpp

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

#ifndef MO_USING_VC
template<> const MOlong moMath<MOlong>::EPSILON = FLT_EPSILON;
template<> const MOlong moMath<MOlong>::ZERO_TOLERANCE = 1e-06f;
template<> const MOlong moMath<MOlong>::MAX_REAL = (MOlong)FLT_MAX;
template<> const MOlong moMath<MOlong>::PI = (MOlong)(4.0*atan(1.0));
template<> const MOlong moMath<MOlong>::TWO_PI = 2.0f*moMath<MOlong>::PI;
template<> const MOlong moMath<MOlong>::HALF_PI = 0.5f*moMath<MOlong>::PI;
template<> const MOlong moMath<MOlong>::INV_PI = 1.0f/moMath<MOlong>::PI;
template<> const MOlong moMath<MOlong>::INV_TWO_PI = 1.0f/moMath<MOlong>::TWO_PI;
template<> const MOlong moMath<MOlong>::DEG_TO_RAD = moMath<MOlong>::PI/180.0f;
template<> const MOlong moMath<MOlong>::RAD_TO_DEG = 180.0f/moMath<MOlong>::PI;
template<> const MOlong moMath<MOlong>::LN_2 = moMath<MOlong>::Log(2.0f);
template<> const MOlong moMath<MOlong>::LN_10 = moMath<MOlong>::Log(10.0f);
template<> const MOlong moMath<MOlong>::INV_LN_2 = 1.0f/moMath<MOlong>::LN_2;
template<> const MOlong moMath<MOlong>::INV_LN_10 = 1.0f/moMath<MOlong>::LN_10;

template<> const MOfloat moMath<MOfloat>::EPSILON = FLT_EPSILON;
template<> const MOfloat moMath<MOfloat>::ZERO_TOLERANCE = 1e-06f;
template<> const MOfloat moMath<MOfloat>::MAX_REAL = FLT_MAX;
template<> const MOfloat moMath<MOfloat>::PI = (MOfloat)(4.0*atan(1.0));
template<> const MOfloat moMath<MOfloat>::TWO_PI = 2.0f*moMath<MOfloat>::PI;
template<> const MOfloat moMath<MOfloat>::HALF_PI = 0.5f*moMath<MOfloat>::PI;
template<> const MOfloat moMath<MOfloat>::INV_PI = 1.0f/moMath<MOfloat>::PI;
template<> const MOfloat moMath<MOfloat>::INV_TWO_PI = 1.0f/moMath<MOfloat>::TWO_PI;
template<> const MOfloat moMath<MOfloat>::DEG_TO_RAD = moMath<MOfloat>::PI/180.0f;
template<> const MOfloat moMath<MOfloat>::RAD_TO_DEG = 180.0f/moMath<MOfloat>::PI;
template<> const MOfloat moMath<MOfloat>::LN_2 = moMath<MOfloat>::Log(2.0f);
template<> const MOfloat moMath<MOfloat>::LN_10 = moMath<MOfloat>::Log(10.0f);
template<> const MOfloat moMath<MOfloat>::INV_LN_2 = 1.0f/moMath<MOfloat>::LN_2;
template<> const MOfloat moMath<MOfloat>::INV_LN_10 = 1.0f/moMath<MOfloat>::LN_10;

template<> const MOdouble moMath<MOdouble>::EPSILON = DBL_EPSILON;
template<> const MOdouble moMath<MOdouble>::ZERO_TOLERANCE = 1e-08;
template<> const MOdouble moMath<MOdouble>::MAX_REAL = DBL_MAX;
template<> const MOdouble moMath<MOdouble>::PI = 4.0*atan(1.0);
template<> const MOdouble moMath<MOdouble>::TWO_PI = 2.0*moMath<MOdouble>::PI;
template<> const MOdouble moMath<MOdouble>::HALF_PI = 0.5*moMath<MOdouble>::PI;
template<> const MOdouble moMath<MOdouble>::INV_PI = 1.0/moMath<MOdouble>::PI;
template<> const MOdouble moMath<MOdouble>::INV_TWO_PI = 1.0/moMath<MOdouble>::TWO_PI;
template<> const MOdouble moMath<MOdouble>::DEG_TO_RAD = moMath<MOdouble>::PI/180.0;
template<> const MOdouble moMath<MOdouble>::RAD_TO_DEG = 180.0/moMath<MOdouble>::PI;
template<> const MOdouble moMath<MOdouble>::LN_2 = moMath<MOdouble>::Log(2.0);
template<> const MOdouble moMath<MOdouble>::LN_10 = moMath<MOdouble>::Log(10.0);
template<> const MOdouble moMath<MOdouble>::INV_LN_2 = 1.0/moMath<MOdouble>::LN_2;
template<> const MOdouble moMath<MOdouble>::INV_LN_10 = 1.0/moMath<MOdouble>::LN_10;

template <>
long moMath<MOlong>::FastInvSqrt (MOlong fValue)
{

    return fValue;
}

template <>
float moMath<MOfloat>::FastInvSqrt (MOfloat fValue)
{
    MOfloat fHalf = 0.5f*fValue;
    int i;
    i = *(int*)&fValue;
    i = 0x5f3759df - (i >> 1);
    fValue = *(MOfloat*)&i;
    fValue = fValue*(1.5f - fHalf*fValue*fValue);
    return fValue;
}

template <>
MOdouble moMath<MOdouble>::FastInvSqrt (MOdouble dValue)
{
    MOdouble dHalf = 0.5*dValue;
    MOlonglong i  = *(MOlonglong*)&dValue;
#if defined(WM4_USING_VC70) || defined(WM4_USING_VC6)
    i = 0x5fe6ec85e7de30da - (i >> 1);
#else
    i = 0x5fe6ec85e7de30daLL - (i >> 1);
#endif
    dValue = *(MOdouble*)&i;
    dValue = dValue*(1.5 - dHalf*dValue*dValue);
    return dValue;
}

#endif

