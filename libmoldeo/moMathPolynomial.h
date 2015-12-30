/*******************************************************************************

                             moMathPolynomial.h

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

#ifndef __MO_MATH_POLYNOMIAL_H__
#define __MO_MATH_POLYNOMIAL_H__

template <class Real>
class LIBMOLDEO_API moPolynomial1 : public moAbstract
{
public:
    // construction and destruction
    moPolynomial1 (int iDegree = -1);
    moPolynomial1 (const moPolynomial1& rkPoly);
    ~moPolynomial1 ();

    // member access
    void SetDegree (int iDegree);
    int GetDegree () const;
    operator const Real* () const;
    operator Real* ();
    Real operator[] (int i) const;
    Real& operator[] (int i);

    // assignment
    moPolynomial1& operator= (const moPolynomial1& rkPoly);

    // evaluation
    Real operator() (Real fT) const;

    // arithmetic operations
    moPolynomial1 operator+ (const moPolynomial1& rkPoly) const;
    moPolynomial1 operator- (const moPolynomial1& rkPoly) const;
    moPolynomial1 operator* (const moPolynomial1& rkPoly) const;
    moPolynomial1 operator+ (Real fScalar) const;  // input is degree 0 poly
    moPolynomial1 operator- (Real fScalar) const;  // input is degree 0 poly
    moPolynomial1 operator* (Real fScalar) const;
    moPolynomial1 operator/ (Real fScalar) const;
    moPolynomial1 operator- () const;

    // arithmetic updates
    moPolynomial1& operator += (const moPolynomial1& rkPoly);
    moPolynomial1& operator -= (const moPolynomial1& rkPoly);
    moPolynomial1& operator *= (const moPolynomial1& rkPoly);
    moPolynomial1& operator += (Real fScalar);  // input is degree 0 poly
    moPolynomial1& operator -= (Real fScalar);  // input is degree 0 poly
    moPolynomial1& operator *= (Real fScalar);
    moPolynomial1& operator /= (Real fScalar);

    // derivation
    moPolynomial1 GetDerivative () const;

    // inversion ( invpoly[i] = poly[degree-i] for 0 <= i <= degree )
    moPolynomial1 GetInversion () const;

    // Reduce degree by eliminating all (nearly) zero leading coefficients
    // and by making the leading coefficient one.  The input parameter is
    // the threshold for specifying that a coefficient is effectively zero.
    void Compress (Real fEpsilon);

    // If 'this' is P(t) and the divisor is D(t) with degree(P) >= degree(D),
    // then P(t) = Q(t)*D(t)+R(t) where Q(t) is the quotient with
    // degree(Q) = degree(P) - degree(D) and R(t) is the remainder with
    // degree(R) < degree(D).  If this routine is called with
    // degree(P) < degree(D), then Q = 0 and R = P are returned.  The value
    // of epsilon is used as a threshold on the coefficients of the remainder
    // polynomial.  If smaller, the coefficient is assumed to be zero.
    void Divide (const moPolynomial1& rkDiv, moPolynomial1& rkQuot,
        moPolynomial1& rkRem, Real fEpsilon) const;

protected:
    int m_iDegree;
    Real* m_afCoeff;
};

template <class Real>
moPolynomial1<Real> operator* (Real fScalar, const moPolynomial1<Real>& rkPoly)
{
    moPolynomial1<Real> kProd(rkPoly.GetDegree());
    for (int i = 0; i <= rkPoly.GetDegree(); i++)
    {
        kProd[i] = fScalar*rkPoly[i];
    }

    return kProd;
}

#ifndef MO_MACOSX
#ifndef MO_RASPBIAN
#ifndef MO_WIN32
template class LIBMOLDEO_API moPolynomial1<MOfloat>;
template class LIBMOLDEO_API moPolynomial1<MOdouble>;
#endif
#endif
#endif

typedef moPolynomial1<MOfloat> moPolynomial1f;
typedef moPolynomial1<MOdouble> moPolynomial1d;

#endif

