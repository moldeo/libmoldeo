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

#include "moMath.h"

#ifndef __MO_MATH_NUMERICAL_ANALYSIS_H__
#define __MO_MATH_NUMERICAL_ANALYSIS_H__

// moIntegrate1 class -----------------------------------------------------------

template <class Real>
class LIBMOLDEO_API moIntegrate1 : public moAbstract
{
public:
    // last parameter is for user-defined data
    typedef Real (*Function)(Real,void*);

    static Real RombergIntegral (int iOrder, Real fA, Real fB, Function oF,
        void* pvUserData = 0) {

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

    static Real GaussianQuadrature (Real fA, Real fB, Function oF,
        void* pvUserData = 0);

    static Real TrapezoidRule (int iNumSamples, Real fA, Real fB,
        Function oF, void* pvUserData = 0);
};

template class LIBMOLDEO_API moIntegrate1<MOfloat>;
typedef moIntegrate1<MOfloat> moIntegrate1f;

template class LIBMOLDEO_API moIntegrate1<MOdouble>;
typedef moIntegrate1<MOdouble> moIntegrate1d;

// moMinimize1 class -----------------------------------------------------------

template <class Real>
class LIBMOLDEO_API moMinimize1 : public moAbstract
{
public:
    typedef Real (*Function)(Real,void*);

    moMinimize1 (Function oFunction, int iMaxLevel, int iMaxBracket,
        void* pvData = 0);

    int& MaxLevel ();
    int& MaxBracket ();
    void*& UserData ();

    void GetMinimum (Real fT0, Real fT1, Real fTInitial, Real& rfTMin,
        Real& rfFMin);

private:
    Function m_oFunction;
    int m_iMaxLevel, m_iMaxBracket;
    Real m_fTMin, m_fFMin;
    void* m_pvData;

    void GetMinimum (Real fT0, Real fF0, Real fT1, Real fF1, int iLevel);

    void GetMinimum (Real fT0, Real fF0, Real fTm, Real fFm, Real fT1,
        Real fF1, int iLevel);

    void GetBracketedMinimum (Real fT0, Real fF0, Real fTm,
        Real fFm, Real fT1, Real fF1, int iLevel);
};

template class LIBMOLDEO_API moMinimize1<MOfloat>;
typedef moMinimize1<MOfloat> moMinimize1f;

template class LIBMOLDEO_API moMinimize1<MOdouble>;
typedef moMinimize1<MOdouble> moMinimize1d;

#endif
