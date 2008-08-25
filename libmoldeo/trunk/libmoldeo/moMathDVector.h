/*******************************************************************************

                                moMathDVector.h

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

#ifndef __MO_MATH_DVECTOR_H__
#define __MO_MATH_DVECTOR_H__

template <class Real>
class LIBMOLDEO_API moDVector : public moAbstract
{
public:
    // construction
    moDVector (int iSize = 0);
    moDVector (int iSize, const Real* afTuple);
    moDVector (const moDVector& rkV);
    ~moDVector ();

    // coordinate access
    void SetSize (int iSize);
    int GetSize () const;
    operator const Real* () const;
    operator Real* ();
    Real operator[] (int i) const;
    Real& operator[] (int i);

    // assignment
    moDVector& operator= (const moDVector& rkV);

    // comparison
    bool operator== (const moDVector& rkV) const;
    bool operator!= (const moDVector& rkV) const;
    bool operator<  (const moDVector& rkV) const;
    bool operator<= (const moDVector& rkV) const;
    bool operator>  (const moDVector& rkV) const;
    bool operator>= (const moDVector& rkV) const;

    // arithmetic operations
    moDVector operator+ (const moDVector& rkV) const;
    moDVector operator- (const moDVector& rkV) const;
    moDVector operator* (Real fScalar) const;
    moDVector operator/ (Real fScalar) const;
    moDVector operator- () const;

    // arithmetic updates
    moDVector& operator+= (const moDVector& rkV);
    moDVector& operator-= (const moDVector& rkV);
    moDVector& operator*= (Real fScalar);
    moDVector& operator/= (Real fScalar);

    // vector operations
    Real Length () const;
    Real SquaredLength () const;
    Real Dot (const moDVector& rkV) const;
    Real Normalize ();

protected:
    // support for comparisons
    int CompareArrays (const moDVector& rkV) const;

    int m_iSize;
    Real* m_afTuple;
};

template <class Real>
moDVector<Real> operator* (Real fScalar, const moDVector<Real>& rkV);

template class LIBMOLDEO_API moDVector<MOfloat>;
typedef moDVector<MOfloat> moDVectorf;

template class LIBMOLDEO_API moDVector<MOdouble>;
typedef moDVector<MOdouble> moDVectord;


#endif
