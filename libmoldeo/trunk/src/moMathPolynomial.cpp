/*******************************************************************************

                             moMathPolynomial.cpp

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

#include "moMathPolynomial.h"

template <class Real>
moPolynomial1<Real>::moPolynomial1 (int iDegree)
{
    if (iDegree >= 0)
    {
        m_iDegree = iDegree;
        m_afCoeff = new Real[m_iDegree+1];
    }
    else
    {
        // default creation
        m_iDegree = -1;
        m_afCoeff = 0;
    }
}

template <class Real>
moPolynomial1<Real>::moPolynomial1 (const moPolynomial1& rkPoly) : moAbstract(rkPoly)
{
    m_iDegree = rkPoly.m_iDegree;
    m_afCoeff = new Real[m_iDegree+1];
    for (int i = 0; i <= m_iDegree; i++)
    {
        m_afCoeff[i] = rkPoly.m_afCoeff[i];
    }
}

template <class Real>
moPolynomial1<Real>::~moPolynomial1 ()
{
    delete[] m_afCoeff;
}

template <class Real>
void moPolynomial1<Real>::SetDegree (int iDegree)
{
    m_iDegree = iDegree;
    delete[] m_afCoeff;

    if (m_iDegree >= 0)
    {
        m_afCoeff = new Real[m_iDegree+1];
    }
    else
    {
        m_afCoeff = 0;
    }
}

template <class Real>
int moPolynomial1<Real>::GetDegree () const
{
    return m_iDegree;
}

template <class Real>
moPolynomial1<Real>::operator const Real* () const
{
    return m_afCoeff;
}

template <class Real>
moPolynomial1<Real>::operator Real* ()
{
    return m_afCoeff;
}

template <class Real>
Real moPolynomial1<Real>::operator[] (int i) const
{
    return m_afCoeff[i];
}

template <class Real>
Real& moPolynomial1<Real>::operator[] (int i)
{
    return m_afCoeff[i];
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator= (const moPolynomial1& rkPoly)
{
    delete[] m_afCoeff;
    m_iDegree = rkPoly.m_iDegree;

    if (m_iDegree >= 0)
    {
        m_afCoeff = new Real[m_iDegree+1];
        for (int i = 0; i <= m_iDegree; i++)
        {
            m_afCoeff[i] = rkPoly.m_afCoeff[i];
        }
    }

    return *this;
}

template <class Real>
Real moPolynomial1<Real>::operator() (Real fT) const
{
    if (m_iDegree < 0) return 0;

    Real fResult = m_afCoeff[m_iDegree];
    for (int i = m_iDegree-1; i >= 0; i--)
    {
        fResult *= fT;
        fResult += m_afCoeff[i];
    }
    return fResult;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator+ (const moPolynomial1& rkPoly)
    const
{
    moPolynomial1 kSum;
    int i;

    if (m_iDegree > rkPoly.m_iDegree)
    {
        kSum.SetDegree(m_iDegree);
        for (i = 0; i <= rkPoly.m_iDegree; i++)
        {
            kSum.m_afCoeff[i] = m_afCoeff[i] + rkPoly.m_afCoeff[i];
        }
        for (i = rkPoly.m_iDegree+1; i <= m_iDegree; i++)
        {
            kSum.m_afCoeff[i] = m_afCoeff[i];
        }

    }
    else
    {
        kSum.SetDegree(rkPoly.m_iDegree);
        for (i = 0; i <= m_iDegree; i++)
        {
            kSum.m_afCoeff[i] = m_afCoeff[i] + rkPoly.m_afCoeff[i];
        }
        for (i = m_iDegree+1; i <= rkPoly.m_iDegree; i++)
        {
            kSum.m_afCoeff[i] = rkPoly.m_afCoeff[i];
        }
    }

    return kSum;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator- (const moPolynomial1& rkPoly)
    const
{
    moPolynomial1 kDiff;
    int i;

    if (m_iDegree > rkPoly.m_iDegree)
    {
        kDiff.SetDegree(m_iDegree);
        for (i = 0; i <= rkPoly.m_iDegree; i++)
        {
            kDiff.m_afCoeff[i] = m_afCoeff[i] - rkPoly.m_afCoeff[i];
        }
        for (i = rkPoly.m_iDegree+1; i <= m_iDegree; i++)
        {
            kDiff.m_afCoeff[i] = m_afCoeff[i];
        }

    }
    else
    {
        kDiff.SetDegree(rkPoly.m_iDegree);
        for (i = 0; i <= m_iDegree; i++)
        {
            kDiff.m_afCoeff[i] = m_afCoeff[i] - rkPoly.m_afCoeff[i];
        }
        for (i = m_iDegree+1; i <= rkPoly.m_iDegree; i++)
        {
            kDiff.m_afCoeff[i] = -rkPoly.m_afCoeff[i];
        }
    }

    return kDiff;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator* (const moPolynomial1& rkPoly)
    const
{
    moPolynomial1 kProd(m_iDegree + rkPoly.m_iDegree);

    memset(kProd.m_afCoeff,0,(kProd.m_iDegree+1)*sizeof(Real));
    for (int i0 = 0; i0 <= m_iDegree; i0++)
    {
        for (int i1 = 0; i1 <= rkPoly.m_iDegree; i1++)
        {
            int i2 = i0 + i1;
            kProd.m_afCoeff[i2] += m_afCoeff[i0]*rkPoly.m_afCoeff[i1];
        }
    }

    return kProd;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator+ (Real fScalar) const
{
    moPolynomial1 kSum(m_iDegree);
    kSum.m_afCoeff[0] += fScalar;
    return kSum;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator- (Real fScalar) const
{
    moPolynomial1 kDiff(m_iDegree);
    kDiff.m_afCoeff[0] -= fScalar;
    return kDiff;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator* (Real fScalar) const
{
    moPolynomial1 kProd(m_iDegree);
    for (int i = 0; i <= m_iDegree; i++)
    {
        kProd.m_afCoeff[i] = fScalar*m_afCoeff[i];
    }
    return kProd;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator/ (Real fScalar) const
{
    moPolynomial1 kProd(m_iDegree);
    int i;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i <= m_iDegree; i++)
        {
            kProd.m_afCoeff[i] = fInvScalar*m_afCoeff[i];
        }
    }
    else
    {
        for (i = 0; i <= m_iDegree; i++)
        {
            kProd.m_afCoeff[i] = moMath<Real>::MAX_REAL;
        }
    }

    return kProd;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::operator- () const
{
    moPolynomial1 kNeg(m_iDegree);
    for (int i = 0; i <= m_iDegree; i++)
    {
        kNeg.m_afCoeff[i] = -m_afCoeff[i];
    }

    return kNeg;
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator += (const moPolynomial1& rkPoly)
{
    *this = *this + rkPoly;
    return *this;
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator -= (const moPolynomial1& rkPoly)
{
    *this = *this - rkPoly;
    return *this;
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator *= (const moPolynomial1& rkPoly)
{
    *this = (*this)*rkPoly;
    return *this;
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator += (Real fScalar)
{
    m_afCoeff[0] += fScalar;
    return *this;
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator -= (Real fScalar)
{
    m_afCoeff[0] -= fScalar;
    return *this;
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator *= (Real fScalar)
{
    *this = (*this)*fScalar;
    return *this;
}

template <class Real>
moPolynomial1<Real>& moPolynomial1<Real>::operator /= (Real fScalar)
{
    *this = (*this)/fScalar;
    return *this;
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::GetDerivative () const
{
    if (m_iDegree > 0)
    {
        moPolynomial1 kDeriv(m_iDegree-1);
        for (int i0 = 0, i1 = 1; i0 < m_iDegree; i0++, i1++)
        {
            kDeriv.m_afCoeff[i0] = i1*m_afCoeff[i1];
        }
        return kDeriv;
    }
    else if (m_iDegree == 0)
    {
        moPolynomial1 kConst(0);
        kConst.m_afCoeff[0] = (Real)0.0;
        return kConst;
    }
    return moPolynomial1<Real>();  // invalid in, invalid out
}

template <class Real>
moPolynomial1<Real> moPolynomial1<Real>::GetInversion () const
{
    moPolynomial1 kInvPoly(m_iDegree);
    for (int i = 0; i <= m_iDegree; i++)
    {
        kInvPoly.m_afCoeff[i] = m_afCoeff[m_iDegree-i];
    }
    return kInvPoly;
}

template <class Real>
void moPolynomial1<Real>::Compress (Real fEpsilon)
{
    int i;
    for (i = m_iDegree; i >= 0; i--)
    {
        if (moMath<Real>::FAbs(m_afCoeff[i]) <= fEpsilon)
        {
            m_iDegree--;
        }
        else
        {
            break;
        }
    }

    if (m_iDegree >= 0)
    {
        Real fInvLeading = ((Real)1.0)/m_afCoeff[m_iDegree];
        m_afCoeff[m_iDegree] = (Real)1.0;
        for (i = 0; i < m_iDegree; i++)
        {
            m_afCoeff[i] *= fInvLeading;
        }
    }
}

template <class Real>
void moPolynomial1<Real>::Divide (const moPolynomial1& rkDiv, moPolynomial1& rkQuot,
    moPolynomial1& rkRem, Real fEpsilon) const
{
    int iQuotDegree = m_iDegree - rkDiv.m_iDegree;
    if (iQuotDegree >= 0)
    {
        rkQuot.SetDegree(iQuotDegree);

        // temporary storage for the remainder
        moPolynomial1 kTmp = *this;

        // do the division (Euclidean algorithm)
        Real fInv = ((Real)1.0)/rkDiv[rkDiv.m_iDegree];
        for (int iQ = iQuotDegree; iQ >= 0; iQ--)
        {
            int iR = rkDiv.m_iDegree + iQ;
            rkQuot[iQ] = fInv*kTmp[iR];
            for (iR--; iR >= iQ; iR--)
            {
                kTmp[iR] -= rkQuot[iQ]*rkDiv[iR-iQ];
            }
        }

        // calculate the correct degree for the remainder
        int iRemDeg = rkDiv.m_iDegree - 1;
        while (iRemDeg > 0 && moMath<Real>::FAbs(kTmp[iRemDeg]) < fEpsilon)
        {
            iRemDeg--;
        }

        if (iRemDeg == 0 && moMath<Real>::FAbs(kTmp[0]) < fEpsilon)
        {
            kTmp[0] = (Real)0.0;
        }

        rkRem.SetDegree(iRemDeg);
        size_t uiSize = (iRemDeg+1)*sizeof(Real);
		memcpy(rkRem.m_afCoeff,kTmp.m_afCoeff,uiSize);
    }
    else
    {
        rkQuot.SetDegree(0);
        rkQuot[0] = (Real)0.0;
        rkRem = *this;
    }
}
