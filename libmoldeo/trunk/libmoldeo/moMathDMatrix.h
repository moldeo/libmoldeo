/*******************************************************************************

                                moMathDMatrix.h

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

#ifndef __MO_MATH_DMATRIX_H__
#define __MO_MATH_DMATRIX_H__

// moDMatrix class ------------------------------------------------

template <class Real>
class LIBMOLDEO_API moDMatrix : public moAbstract
{
public:
    // construction and destruction
    moDMatrix (int iRows = 0, int iCols = 0);
    moDMatrix (int iRows, int iCols, const Real* afData);
    moDMatrix (int iRows, int iCols, const Real** aafEntry);
    moDMatrix (const moDMatrix& rkM);
    ~moDMatrix ();

    // member access
    void SetSize (int iRows, int iCols);
    void GetSize (int& riRows, int& riCols) const;
    int GetRows () const;
    int GetColumns () const;
    int GetQuantity () const;
    operator const Real* () const;
    operator Real* ();
    const Real* operator[] (int iRow) const;
    Real* operator[] (int iRow);
    void SwapRows (int iRow0, int iRow1);
    Real operator() (int iRow, int iCol) const;
    Real& operator() (int iRow, int iCol);
    void SetRow (int iRow, const moDVector<Real>& rkV);
    moDVector<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const moDVector<Real>& rkV);
    moDVector<Real> GetColumn (int iCol) const;
    void SetMatrix (int iRows, int iCols, const Real* afEntry);
    void SetMatrix (int iRows, int iCols, const Real** aafMatrix);
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    moDMatrix& operator= (const moDMatrix& rkM);

    // comparison
    bool operator== (const moDMatrix& rkM) const;
    bool operator!= (const moDMatrix& rkM) const;
    bool operator<  (const moDMatrix& rkM) const;
    bool operator<= (const moDMatrix& rkM) const;
    bool operator>  (const moDMatrix& rkM) const;
    bool operator>= (const moDMatrix& rkM) const;

    // arithmetic operations
    moDMatrix operator+ (const moDMatrix& rkM) const;
    moDMatrix operator- (const moDMatrix& rkM) const;
    moDMatrix operator* (const moDMatrix& rkM) const;
    moDMatrix operator* (Real fScalar) const;
    moDMatrix operator/ (Real fScalar) const;
    moDMatrix operator- () const;

    // arithmetic updates
    moDMatrix& operator+= (const moDMatrix& rkM);
    moDMatrix& operator-= (const moDMatrix& rkM);
    moDMatrix& operator*= (Real fScalar);
    moDMatrix& operator/= (Real fScalar);

    // matrix products
    moDMatrix Transpose () const;  // M^T
    moDMatrix TransposeTimes (const moDMatrix& rkM) const;  // this^T * M
    moDMatrix TimesTranspose (const moDMatrix& rkM) const;  // this * M^T

    // matrix-vector operations
    moDVector<Real> operator* (const moDVector<Real>& rkV) const;  // M * v
    Real QForm (const moDVector<Real>& rkU, const moDVector<Real>& rkV)
        const;  // u^T*M*v

    // Inversion.  The matrix must be square.  The function returns true
    // whenever the matrix is square and invertible.
    bool GetInverse (moDMatrix<Real>& rkInverse) const;

protected:
    // Support for allocation and deallocation.  The allocation call requires
    // m_iRows, m_iCols, and m_iQuantity to have already been correctly
    // initialized.
    void Allocate (bool bSetToZero);
    void Deallocate ();

    // support for comparisons
    int CompareArrays (const moDMatrix& rkM) const;

    int m_iRows, m_iCols, m_iQuantity;

    // the matrix is stored in row-major form as a 1-dimensional array
    Real* m_afData;

    // An array of pointers to the rows of the matrix.  The separation of
    // row pointers and actual data supports swapping of rows in linear
    // algebraic algorithms such as solving linear systems of equations.
    Real** m_aafEntry;
};

// c * M
template <class Real>
moDMatrix<Real> operator* (Real fScalar, const moDMatrix<Real>& rkM);

// v^T * M
template <class Real>
moDVector<Real> operator* (const moDVector<Real>& rkV, const moDMatrix<Real>& rkM);

#ifndef MO_MACOSX
template class LIBMOLDEO_API moDMatrix<MOfloat>;
#endif
typedef moDMatrix<MOfloat> moDMatrixf;

#ifndef MO_MACOSX
template class LIBMOLDEO_API moDMatrix<MOdouble>;
#endif
typedef moDMatrix<MOdouble> moDMatrixd;

// moDBandedMatrix class ------------------------------------------------

template <class Real>
class moDBandedMatrix : public moAbstract
{
public:
    moDBandedMatrix (int iSize, int iLBands, int iUBands);
    moDBandedMatrix (const moDBandedMatrix& rkM);
    ~moDBandedMatrix ();

    moDBandedMatrix& operator= (const moDBandedMatrix& rkM);

    int GetSize () const;
    int GetLBands () const;
    int GetUBands () const;

    Real* GetDBand ();
    const Real* GetDBand () const;

    int GetLBandMax (int i) const;  // LBand(i):  0 <= index < LBandMax
    Real* GetLBand (int i);
    const Real* GetLBand (int i) const;

    int GetUBandMax (int i) const;  // UBand(i):  0 <= index < UBandMax
    Real* GetUBand (int i);
    const Real* GetUBand (int i) const;

    Real& operator() (int iRow, int iCol);
    Real operator() (int iRow, int iCol) const;

    void SetZero ();
    void SetIdentity ();

private:
    void Allocate ();
    void Deallocate ();

    int m_iSize, m_iLBands, m_iUBands;
    Real* m_afDBand;
    Real** m_aafLBand;
    Real** m_aafUBand;
};

#ifndef MO_MACOSX
template class LIBMOLDEO_API moDBandedMatrix<MOfloat>;
#endif
typedef moDBandedMatrix<MOfloat> moDBandedMatrixf;

#ifndef MO_MACOSX
template class LIBMOLDEO_API moDBandedMatrix<MOdouble>;
#endif
typedef moDBandedMatrix<MOdouble> moDBandedMatrixd;



#endif

