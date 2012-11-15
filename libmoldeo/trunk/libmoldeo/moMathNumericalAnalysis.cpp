/*******************************************************************************

                          moMathNumericalAnalysis.h

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

#include "moMathNumericalAnalysis.h"

// moIntegrate1 class -----------------------------------------------------------
/*
template <class Real>
Real moIntegrate1<Real>::RombergIntegral (int iOrder, Real fA, Real fB,
    Function oF, void* pvUserData)
{
    //assert(iOrder > 0);
    Real** aafRom;

    aafRom = new Real*[2];
	for (int i = 0; i < 2; i++) aafRom[i] = new Real[iOrder];

    Real fH = fB - fA;

    aafRom[0][0] = ((Real)0.5)*fH*(oF(fA,pvUserData)+oF(fB,pvUserData));
    for (int i0=2, iP0=1; i0 <= iOrder; i0++, iP0 *= 2, fH *= (Real)0.5)
    {
        // approximations via the trapezoid rule
        Real fSum = (Real)0.0;
        int i1;
        for (i1 = 1; i1 <= iP0; i1++)
        {
            fSum += oF(fA + fH*(i1-((Real)0.5)),pvUserData);
        }

        // Richardson extrapolation
        aafRom[1][0] = ((Real)0.5)*(aafRom[0][0] + fH*fSum);
        for (int i2 = 1, iP2 = 4; i2 < i0; i2++, iP2 *= 4)
        {
            aafRom[1][i2] = (iP2*aafRom[1][i2-1] - aafRom[0][i2-1])/(iP2-1);
        }

        for (i1 = 0; i1 < i0; i1++)
        {
            aafRom[0][i1] = aafRom[1][i1];
        }
    }

    Real fResult = aafRom[0][iOrder-1];

	for (int i = 0; i < 2; i++) delete[] aafRom[i];
	delete[] aafRom;

	return fResult;
}
*/
template <class Real>
Real moIntegrate1<Real>::GaussianQuadrature (Real fA, Real fB, Function oF,
    void* pvUserData)
{
    // Legendre polynomials
    // P_0(x) = 1
    // P_1(x) = x
    // P_2(x) = (3x^2-1)/2
    // P_3(x) = x(5x^2-3)/2
    // P_4(x) = (35x^4-30x^2+3)/8
    // P_5(x) = x(63x^4-70x^2+15)/8

    // generation of polynomials
    //   d/dx[ (1-x^2) dP_n(x)/dx ] + n(n+1) P_n(x) = 0
    //   P_n(x) = sum_{k=0}^{floor(n/2)} c_k x^{n-2k}
    //     c_k = (-1)^k (2n-2k)! / [ 2^n k! (n-k)! (n-2k)! ]
    //   P_n(x) = ((-1)^n/(2^n n!)) d^n/dx^n[ (1-x^2)^n ]
    //   (n+1)P_{n+1}(x) = (2n+1) x P_n(x) - n P_{n-1}(x)
    //   (1-x^2) dP_n(x)/dx = -n x P_n(x) + n P_{n-1}(x)

    // roots of the Legendre polynomial of specified degree
    const int iDegree = 5;
    const Real afRoot[iDegree] =
    {
        (Real)-0.9061798459,
        (Real)-0.5384693101,
        (Real) 0.0,
        (Real)+0.5384693101,
        (Real)+0.9061798459
    };
    const Real afCoeff[iDegree] =
    {
        (Real)0.2369268850,
        (Real)0.4786286705,
        (Real)0.5688888889,
        (Real)0.4786286705,
        (Real)0.2369268850
    };

    // Need to transform domain [a,b] to [-1,1].  If a <= x <= b and
    // -1 <= t <= 1, then x = ((b-a)*t+(b+a))/2.
    Real fRadius = ((Real)0.5)*(fB - fA);
    Real fCenter = ((Real)0.5)*(fB + fA);

    Real fResult = (Real)0.0;
    for (int i = 0; i < iDegree; i++)
    {
        fResult += afCoeff[i]*oF(fRadius*afRoot[i]+fCenter,pvUserData);
    }
    fResult *= fRadius;

    return fResult;
}

template <class Real>
Real moIntegrate1<Real>::TrapezoidRule (int iNumSamples, Real fA, Real fB,
    Function oF, void* pvUserData)
{
    //assert(iNumSamples >= 2);
    Real fH = (fB - fA)/(Real)(iNumSamples - 1);
    Real fResult = ((Real)0.5)*(oF(fA,pvUserData) + oF(fB,pvUserData));
    for (int i = 1; i <= iNumSamples - 2; i++)
    {
        fResult += oF(fA+i*fH,pvUserData);
    }
    fResult *= fH;
    return fResult;
}

// moMinimize1 class -----------------------------------------------------------

template <class Real>
moMinimize1<Real>::moMinimize1 (Function oFunction, int iMaxLevel,
    int iMaxBracket, void* pvData)
{
    //assert(oFunction);
    m_oFunction = oFunction;
    m_iMaxLevel = iMaxLevel;
    m_iMaxBracket = iMaxBracket;
    m_pvData = pvData;
}

template <class Real>
int& moMinimize1<Real>::MaxLevel ()
{
    return m_iMaxLevel;
}

template <class Real>
int& moMinimize1<Real>::MaxBracket ()
{
    return m_iMaxBracket;
}

template <class Real>
void*& moMinimize1<Real>::UserData ()
{
    return m_pvData;
}

template <class Real>
void moMinimize1<Real>::GetMinimum (Real fT0, Real fT1, Real fTInitial,
    Real& rfTMin, Real& rfFMin)
{
    //assert(fT0 <= fTInitial && fTInitial <= fT1);

    m_fTMin = moMath<Real>::MAX_REAL;
    m_fFMin = moMath<Real>::MAX_REAL;

    Real fF0 = m_oFunction(fT0,m_pvData);
    Real fFInitial = m_oFunction(fTInitial,m_pvData);
    Real fF1 = m_oFunction(fT1,m_pvData);

    GetMinimum(fT0,fF0,fTInitial,fFInitial,fT1,fF1,m_iMaxLevel);

    rfTMin = m_fTMin;
    rfFMin = m_fFMin;
}

template <class Real>
void moMinimize1<Real>::GetMinimum (Real fT0, Real fF0, Real fTm, Real fFm,
    Real fT1, Real fF1, int iLevel)
{
    if (fF0 < m_fFMin)
    {
        m_fTMin = fT0;
        m_fFMin = fF0;
    }

    if (fFm < m_fFMin)
    {
        m_fTMin = fTm;
        m_fFMin = fFm;
    }

    if (fF1 < m_fFMin)
    {
        m_fTMin = fT1;
        m_fFMin = fF1;
    }

    if (iLevel-- == 0)
    {
        return;
    }

    if ((fT1 - fTm)*(fF0 - fFm) > (fTm - fT0)*(fFm - fF1))
    {
        // quadratic fit has positive second derivative at midpoint

        if (fF1 > fF0)
        {
            if (fFm >= fF0)
            {
                // increasing, repeat on [t0,tm]
                GetMinimum(fT0,fF0,fTm,fFm,iLevel);
            }
            else
            {
                // not monotonic, have a bracket
                GetBracketedMinimum(fT0,fF0,fTm,fFm,fT1,fF1,iLevel);
            }
        }
        else if (fF1 < fF0)
        {
            if (fFm >= fF1)
            {
                // decreasing, repeat on [tm,t1]
                GetMinimum(fTm,fFm,fT1,fF1,iLevel);
            }
            else
            {
                // not monotonic, have a bracket
                GetBracketedMinimum(fT0,fF0,fTm,fFm,fT1,fF1,iLevel);
            }
        }
        else
        {
            // constant, repeat on [t0,tm] and [tm,t1]
            GetMinimum(fT0,fF0,fTm,fFm,iLevel);
            GetMinimum(fTm,fFm,fT1,fF1,iLevel);
        }
    }
    else
    {
        // quadratic fit has nonpositive second derivative at midpoint

        if (fF1 > fF0)
        {
            // repeat on [t0,tm]
            GetMinimum(fT0,fF0,fTm,fFm,iLevel);
        }
        else if ( fF1 < fF0 )
        {
            // repeat on [tm,t1]
            GetMinimum(fTm,fFm,fT1,fF1,iLevel);
        }
        else
        {
            // repeat on [t0,tm] and [tm,t1]
            GetMinimum(fT0,fF0,fTm,fFm,iLevel);
            GetMinimum(fTm,fFm,fT1,fF1,iLevel);
        }
    }
}

template <class Real>
void moMinimize1<Real>::GetMinimum (Real fT0, Real fF0, Real fT1, Real fF1,
    int iLevel)
{
    if (fF0 < m_fFMin)
    {
        m_fTMin = fT0;
        m_fFMin = fF0;
    }

    if (fF1 < m_fFMin)
    {
        m_fTMin = fT1;
        m_fFMin = fF1;
    }

    if (iLevel-- == 0)
    {
        return;
    }

    Real fTm = ((Real)0.5)*(fT0+fT1);
    Real fFm = m_oFunction(fTm,m_pvData);

    if (fF0 - ((Real)2.0)*fFm + fF1 > (Real)0.0)
    {
        // quadratic fit has positive second derivative at midpoint

        if (fF1 > fF0)
        {
            if (fFm >= fF0)
            {
                // increasing, repeat on [t0,tm]
                GetMinimum(fT0,fF0,fTm,fFm,iLevel);
            }
            else
            {
                // not monotonic, have a bracket
                GetBracketedMinimum(fT0,fF0,fTm,fFm,fT1,fF1,iLevel);
            }
        }
        else if (fF1 < fF0)
        {
            if (fFm >= fF1)
            {
                // decreasing, repeat on [tm,t1]
                GetMinimum(fTm,fFm,fT1,fF1,iLevel);
            }
            else
            {
                // not monotonic, have a bracket
                GetBracketedMinimum(fT0,fF0,fTm,fFm,fT1,fF1,iLevel);
            }
        }
        else
        {
            // constant, repeat on [t0,tm] and [tm,t1]
            GetMinimum(fT0,fF0,fTm,fFm,iLevel);
            GetMinimum(fTm,fFm,fT1,fF1,iLevel);
        }
    }
    else
    {
        // quadratic fit has nonpositive second derivative at midpoint

        if (fF1 > fF0)
        {
            // repeat on [t0,tm]
            GetMinimum(fT0,fF0,fTm,fFm,iLevel);
        }
        else if (fF1 < fF0)
        {
            // repeat on [tm,t1]
            GetMinimum(fTm,fFm,fT1,fF1,iLevel);
        }
        else
        {
            // repeat on [t0,tm] and [tm,t1]
            GetMinimum(fT0,fF0,fTm,fFm,iLevel);
            GetMinimum(fTm,fFm,fT1,fF1,iLevel);
        }
    }
}

template <class Real>
void moMinimize1<Real>::GetBracketedMinimum (Real fT0, Real fF0, Real fTm,
    Real fFm, Real fT1, Real fF1, int iLevel)
{
    for (int i = 0; i < m_iMaxBracket; i++)
    {
        // update minimum value
        if (fFm < m_fFMin)
        {
            m_fTMin = fTm;
            m_fFMin = fFm;
        }

        // test for convergence
        const Real fEps = (Real)1e-08, fTol = (Real)1e-04;
        if (moMath<Real>::FAbs(fT1-fT0) <= ((Real)2.0)*fTol*
            moMath<Real>::FAbs(fTm) + fEps )
        {
            break;
        }

        // compute vertex of interpolating parabola
        Real fDT0 = fT0 - fTm, fDT1 = fT1 - fTm;
        Real fDF0 = fF0 - fFm, fDF1 = fF1 - fFm;
        Real fTmp0 = fDT0*fDF1, fTmp1 = fDT1*fDF0;
        Real fDenom = fTmp1 - fTmp0;
        if (moMath<Real>::FAbs(fDenom) < fEps)
        {
           return;
        }

        Real fTv = fTm + ((Real)0.5)*(fDT1*fTmp1-fDT0*fTmp0)/fDenom;
        //assert(fT0 <= fTv && fTv <= fT1);
        Real fFv = m_oFunction(fTv,m_pvData);

        if (fTv < fTm)
        {
            if (fFv < fFm)
            {
                fT1 = fTm;
                fF1 = fFm;
                fTm = fTv;
                fFm = fFv;
            }
            else
            {
                fT0 = fTv;
                fF0 = fFv;
            }
        }
        else if (fTv > fTm)
        {
            if (fFv < fFm)
            {
                fT0 = fTm;
                fF0 = fFm;
                fTm = fTv;
                fFm = fFv;
            }
            else
            {
                fT1 = fTv;
                fF1 = fFv;
            }
        }
        else
        {
            // vertex of parabola is already at middle sample point
            GetMinimum(fT0,fF0,fTm,fFm,iLevel);
            GetMinimum(fTm,fFm,fT1,fF1,iLevel);
        }
    }
}

