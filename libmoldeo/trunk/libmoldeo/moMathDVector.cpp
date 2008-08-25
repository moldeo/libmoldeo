/*******************************************************************************

                                moMathDVector.cpp

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

#include "moMathDVector.h"


template <class Real>
moDVector<Real>::moDVector (int iSize)
{
    if (iSize > 0)
    {
        m_iSize = iSize;
        m_afTuple = new Real[m_iSize];
        memset(m_afTuple,0,m_iSize*sizeof(Real));
    }
    else
    {
        m_iSize = 0;
        m_afTuple = 0;
    }
}

template <class Real>
moDVector<Real>::moDVector (int iSize, const Real* afTuple)
{
    if (iSize > 0)
    {
        m_iSize = iSize;
        m_afTuple = new Real[m_iSize];
        size_t uiSize = m_iSize*sizeof(Real);
        memcpy(m_afTuple,afTuple,uiSize);
    }
    else
    {
        m_iSize = 0;
        m_afTuple = 0;
    }
}

template <class Real>
moDVector<Real>::moDVector (const moDVector& rkV) : moAbstract(rkV)
{
    m_iSize = rkV.m_iSize;
    if (m_iSize > 0)
    {
        m_afTuple = new Real[m_iSize];
        size_t uiSize = m_iSize*sizeof(Real);
        memcpy(m_afTuple,rkV.m_afTuple,uiSize);
    }
    else
    {
        m_afTuple = 0;
    }
}

template <class Real>
moDVector<Real>::~moDVector ()
{
    delete[] m_afTuple;
}

template <class Real>
void moDVector<Real>::SetSize (int iSize)
{
    delete[] m_afTuple;
    if (iSize > 0)
    {
        m_iSize = iSize;
        m_afTuple = new Real[m_iSize];
        memset(m_afTuple,0,m_iSize*sizeof(Real));
    }
    else
    {
        m_iSize = 0;
        m_afTuple = 0;
    }
}

template <class Real>
int moDVector<Real>::GetSize () const
{
    return m_iSize;
}

template <class Real>
moDVector<Real>::operator const Real* () const
{
    return m_afTuple;
}

template <class Real>
moDVector<Real>::operator Real* ()
{
    return m_afTuple;
}

template <class Real>
Real moDVector<Real>::operator[] (int i) const
{
    //assert(0 <= i && i < m_iSize);
    return m_afTuple[i];
}

template <class Real>
Real& moDVector<Real>::operator[] (int i)
{
    //assert(0 <= i && i < m_iSize);
    return m_afTuple[i];
}

template <class Real>
moDVector<Real>& moDVector<Real>::operator= (const moDVector& rkV)
{
    if (rkV.m_iSize > 0)
    {
        if (m_iSize != rkV.m_iSize)
        {
            delete[] m_afTuple;
            m_iSize = rkV.m_iSize;
            m_afTuple = new Real[m_iSize];
        }
        size_t uiSize = m_iSize*sizeof(Real);
        memcpy(m_afTuple,rkV.m_afTuple,uiSize);
    }
    else
    {
        delete[] m_afTuple;
        m_iSize = 0;
        m_afTuple = 0;
    }
    return *this;
}

template <class Real>
int moDVector<Real>::CompareArrays (const moDVector& rkV) const
{
    return memcmp(m_afTuple,rkV.m_afTuple,m_iSize*sizeof(Real));
}

template <class Real>
bool moDVector<Real>::operator== (const moDVector& rkV) const
{
    return CompareArrays(rkV) == 0;
}

template <class Real>
bool moDVector<Real>::operator!= (const moDVector& rkV) const
{
    return CompareArrays(rkV) != 0;
}

template <class Real>
bool moDVector<Real>::operator< (const moDVector& rkV) const
{
    return CompareArrays(rkV) < 0;
}

template <class Real>
bool moDVector<Real>::operator<= (const moDVector& rkV) const
{
    return CompareArrays(rkV) <= 0;
}

template <class Real>
bool moDVector<Real>::operator> (const moDVector& rkV) const
{
    return CompareArrays(rkV) > 0;
}

template <class Real>
bool moDVector<Real>::operator>= (const moDVector& rkV) const
{
    return CompareArrays(rkV) >= 0;
}

template <class Real>
moDVector<Real> moDVector<Real>::operator+ (const moDVector& rkV) const
{
    moDVector<Real> kSum(m_iSize);
    for (int i = 0; i < m_iSize; i++)
    {
        kSum.m_afTuple[i] = m_afTuple[i] + rkV.m_afTuple[i];
    }
    return kSum;
}

template <class Real>
moDVector<Real> moDVector<Real>::operator- (const moDVector& rkV) const
{
    moDVector<Real> kDiff(m_iSize);
    for (int i = 0; i < m_iSize; i++)
    {
        kDiff.m_afTuple[i] = m_afTuple[i] - rkV.m_afTuple[i];
    }
    return kDiff;
}

template <class Real>
moDVector<Real> moDVector<Real>::operator* (Real fScalar) const
{
    moDVector<Real> kProd(m_iSize);
    for (int i = 0; i < m_iSize; i++)
    {
        kProd.m_afTuple[i] = fScalar*m_afTuple[i];
    }
    return kProd;
}

template <class Real>
moDVector<Real> moDVector<Real>::operator/ (Real fScalar) const
{
    moDVector<Real> kQuot(m_iSize);
    int i;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iSize; i++)
        {
            kQuot.m_afTuple[i] = fInvScalar*m_afTuple[i];
        }
    }
    else
    {
        for (i = 0; i < m_iSize; i++)
        {
            kQuot.m_afTuple[i] = moMath<Real>::MAX_REAL;
        }
    }

    return kQuot;
}

template <class Real>
moDVector<Real> moDVector<Real>::operator- () const
{
    moDVector<Real> kNeg(m_iSize);
    for (int i = 0; i < m_iSize; i++)
    {
        kNeg.m_afTuple[i] = -m_afTuple[i];
    }
    return kNeg;
}

template <class Real>
moDVector<Real> operator* (Real fScalar, const moDVector<Real>& rkV)
{
    moDVector<Real> kProd(rkV.GetSize());
    for (int i = 0; i < rkV.GetSize(); i++)
    {
        kProd[i] = fScalar*rkV[i];
    }
    return kProd;
}

template <class Real>
moDVector<Real>& moDVector<Real>::operator+= (const moDVector& rkV)
{
    for (int i = 0; i < m_iSize; i++)
    {
        m_afTuple[i] += rkV.m_afTuple[i];
    }
    return *this;
}

template <class Real>
moDVector<Real>& moDVector<Real>::operator-= (const moDVector& rkV)
{
    for (int i = 0; i < m_iSize; i++)
    {
        m_afTuple[i] -= rkV.m_afTuple[i];
    }
    return *this;
}

template <class Real>
moDVector<Real>& moDVector<Real>::operator*= (Real fScalar)
{
    for (int i = 0; i < m_iSize; i++)
    {
        m_afTuple[i] *= fScalar;
    }
    return *this;
}

template <class Real>
moDVector<Real>& moDVector<Real>::operator/= (Real fScalar)
{
    int i;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iSize; i++)
        {
            m_afTuple[i] *= fInvScalar;
        }
    }
    else
    {
        for (i = 0; i < m_iSize; i++)
        {
            m_afTuple[i] = moMath<Real>::MAX_REAL;
        }
    }

    return *this;
}

template <class Real>
Real moDVector<Real>::Length () const
{
    Real fSqrLen = (Real)0.0;
    for (int i = 0; i < m_iSize; i++)
    {
        fSqrLen += m_afTuple[i]*m_afTuple[i];
    }
    return moMath<Real>::Sqrt(fSqrLen);
}

template <class Real>
Real moDVector<Real>::SquaredLength () const
{
    Real fSqrLen = (Real)0.0;
    for (int i = 0; i < m_iSize; i++)
    {
        fSqrLen += m_afTuple[i]*m_afTuple[i];
    }
    return fSqrLen;
}

template <class Real>
Real moDVector<Real>::Dot (const moDVector& rkV) const
{
    Real fDot = (Real)0.0;
    for (int i = 0; i < m_iSize; i++)
    {
        fDot += m_afTuple[i]*rkV.m_afTuple[i];
    }
    return fDot;
}

template <class Real>
Real moDVector<Real>::Normalize ()
{
    Real fLength = Length();
    int i;

    if (fLength > moMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvLength = ((Real)1.0)/fLength;
        for (i = 0; i < m_iSize; i++)
        {
            m_afTuple[i] *= fInvLength;
        }
    }
    else
    {
        fLength = (Real)0.0;
        for (i = 0; i < m_iSize; i++)
        {
            m_afTuple[i] = (Real)0.0;
        }
    }

    return fLength;
}
