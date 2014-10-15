/*******************************************************************************

                                moMathDMatrix.cpp

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

#include "moMathDMatrix.h"

// moDMatrix class ------------------------------------------------

template <class Real>
moDMatrix<Real>::moDMatrix (int iRows, int iCols)
{
    m_afData = 0;
    m_aafEntry = 0;
    SetSize(iRows,iCols);
}

template <class Real>
moDMatrix<Real>::moDMatrix (int iRows, int iCols, const Real* afEntry)
{
    m_afData = 0;
    m_aafEntry = 0;
    SetMatrix(iRows,iCols,afEntry);
}

template <class Real>
moDMatrix<Real>::moDMatrix (int iRows, int iCols, const Real** aafMatrix)
{
    m_afData = 0;
    m_aafEntry = 0;
    SetMatrix(iRows,iCols,aafMatrix);
}

template <class Real>
moDMatrix<Real>::moDMatrix (const moDMatrix& rkM) : moAbstract(rkM)
{
    m_iRows = 0;
    m_iCols = 0;
    m_iQuantity = 0;
    m_afData = 0;
    m_aafEntry = 0;
    *this = rkM;
}

template <class Real>
moDMatrix<Real>::~moDMatrix ()
{
    Deallocate();
}

template <class Real>
void moDMatrix<Real>::Allocate (bool bSetToZero)
{
    // //assert:  m_iRows, m_iCols, and m_iQuantity already initialized

    m_afData = new Real[m_iQuantity];
    if (bSetToZero)
    {
        memset(m_afData,0,m_iQuantity*sizeof(Real));
    }

    m_aafEntry = new Real*[m_iRows];
    for (int iRow = 0; iRow < m_iRows; iRow++)
    {
        m_aafEntry[iRow] = &m_afData[iRow*m_iCols];
    }
}

template <class Real>
void moDMatrix<Real>::Deallocate ()
{
    delete[] m_afData;
    delete[] m_aafEntry;
}

template <class Real>
void moDMatrix<Real>::SetSize (int iRows, int iCols)
{
    Deallocate();
    if (iRows > 0 && iCols > 0)
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iQuantity = m_iRows*m_iCols;
        Allocate(true);
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
}

template <class Real>
void moDMatrix<Real>::GetSize (int& riRows, int& riCols) const
{
    riRows = m_iRows;
    riCols = m_iCols;
}

template <class Real>
int moDMatrix<Real>::GetRows () const
{
    return m_iRows;
}

template <class Real>
int moDMatrix<Real>::GetColumns () const
{
    return m_iCols;
}

template <class Real>
int moDMatrix<Real>::GetQuantity () const
{
    return m_iQuantity;
}

template <class Real>
moDMatrix<Real>::operator const Real* () const
{
    return m_afData;
}

template <class Real>
moDMatrix<Real>::operator Real* ()
{
    return m_afData;
}

template <class Real>
const Real* moDMatrix<Real>::operator[] (int iRow) const
{
    //assert(0 <= iRow && iRow < m_iRows);
    return m_aafEntry[iRow];
}

template <class Real>
Real* moDMatrix<Real>::operator[] (int iRow)
{
    //assert(0 <= iRow && iRow < m_iRows);
    return m_aafEntry[iRow];
}

template <class Real>
Real moDMatrix<Real>::operator() (int iRow, int iCol) const
{
    return m_aafEntry[iRow][iCol];
}

template <class Real>
Real& moDMatrix<Real>::operator() (int iRow, int iCol)
{
    //assert(0 <= iRow && iRow < m_iRows && 0 <= iCol && iCol <= m_iCols);
    return m_aafEntry[iRow][iCol];
}

template <class Real>
void moDMatrix<Real>::SwapRows (int iRow0, int iRow1)
{
    //assert(0 <= iRow0 && iRow0 < m_iRows && 0 <= iRow1 && iRow1 < m_iRows);
    Real* afSave = m_aafEntry[iRow0];
    m_aafEntry[iRow0] = m_aafEntry[iRow1];
    m_aafEntry[iRow1] = afSave;
}

template <class Real>
void moDMatrix<Real>::SetRow (int iRow, const moDVector<Real>& rkV)
{
    //assert((0 <= iRow && iRow < m_iRows) && (rkV.GetSize() == m_iCols));
    for (int iCol = 0; iCol < m_iCols; iCol++)
    {
        m_aafEntry[iRow][iCol] = rkV[iCol];
    }
}

template <class Real>
moDVector<Real> moDMatrix<Real>::GetRow (int iRow) const
{
    //assert(0 <= iRow && iRow < m_iRows);
    moDVector<Real> kV(m_iCols);
    for (int iCol = 0; iCol < m_iCols; iCol++)
    {
        kV[iCol] = m_aafEntry[iRow][iCol];
    }
    return kV;
}

template <class Real>
void moDMatrix<Real>::SetColumn (int iCol, const moDVector<Real>& rkV)
{
    //assert((0 <= iCol && iCol < m_iCols) && (rkV.GetSize() == m_iRows));
    for (int iRow = 0; iRow < m_iRows; iRow++)
    {
        m_aafEntry[iRow][iCol] = rkV[iRow];
    }
}

template <class Real>
moDVector<Real> moDMatrix<Real>::GetColumn (int iCol) const
{
    //assert(0 <= iCol && iCol < m_iCols);
    moDVector<Real> kV(m_iRows);
    for (int iRow = 0; iRow < m_iRows; iRow++)
    {
        kV[iRow] = m_aafEntry[iRow][iCol];
    }
    return kV;
}

template <class Real>
void moDMatrix<Real>::SetMatrix (int iRows, int iCols, const Real* afData)
{
    Deallocate();
    if (iRows > 0 && iCols > 0)
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iQuantity = m_iRows*m_iCols;
        Allocate(false);
        size_t uiSize = m_iQuantity*sizeof(Real);
        memcpy(m_afData,afData,uiSize);
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
}

template <class Real>
void moDMatrix<Real>::SetMatrix (int iRows, int iCols, const Real** aafEntry)
{
    Deallocate();
    if (iRows > 0 && iCols > 0)
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iQuantity = m_iRows*m_iCols;
        Allocate(false);
        for (int iRow = 0; iRow < m_iRows; iRow++)
        {
            for (int iCol = 0; iCol < m_iCols; iCol++)
            {
                m_aafEntry[iRow][iCol] = aafEntry[iRow][iCol];
            }
        }
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
}

template <class Real>
void moDMatrix<Real>::GetColumnMajor (Real* afCMajor) const
{
    for (int iRow = 0, i = 0; iRow < m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < m_iCols; iCol++)
        {
            afCMajor[i++] = m_aafEntry[iCol][iRow];
        }
    }
}

template <class Real>
moDMatrix<Real>& moDMatrix<Real>::operator= (const moDMatrix& rkM)
{
    if (rkM.m_iQuantity > 0)
    {
        if (m_iRows != rkM.m_iRows || m_iCols != rkM.m_iCols)
        {
            Deallocate();
            m_iRows = rkM.m_iRows;
            m_iCols = rkM.m_iCols;
            m_iQuantity = rkM.m_iQuantity;
            Allocate(false);
        }
        for (int iRow = 0; iRow < m_iRows; iRow++)
        {
            for (int iCol = 0; iCol < m_iCols; iCol++)
            {
                m_aafEntry[iRow][iCol] = rkM.m_aafEntry[iRow][iCol];
            }
        }
    }
    else
    {
        Deallocate();
        m_iRows = 0;
        m_iCols = 0;
        m_iQuantity = 0;
        m_afData = 0;
        m_aafEntry = 0;
    }
    return *this;
}

template <class Real>
int moDMatrix<Real>::CompareArrays (const moDMatrix& rkM) const
{
    return memcmp(m_afData,rkM.m_afData,m_iQuantity*sizeof(Real));
}

template <class Real>
bool moDMatrix<Real>::operator== (const moDMatrix& rkM) const
{
    return CompareArrays(rkM) == 0;
}

template <class Real>
bool moDMatrix<Real>::operator!= (const moDMatrix& rkM) const
{
    return CompareArrays(rkM) != 0;
}

template <class Real>
bool moDMatrix<Real>::operator<  (const moDMatrix& rkM) const
{
    return CompareArrays(rkM) < 0;
}

template <class Real>
bool moDMatrix<Real>::operator<= (const moDMatrix& rkM) const
{
    return CompareArrays(rkM) <= 0;
}

template <class Real>
bool moDMatrix<Real>::operator>  (const moDMatrix& rkM) const
{
    return CompareArrays(rkM) > 0;
}

template <class Real>
bool moDMatrix<Real>::operator>= (const moDMatrix& rkM) const
{
    return CompareArrays(rkM) >= 0;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::operator+ (const moDMatrix& rkM) const
{
    moDMatrix<Real> kSum(rkM.m_iRows,rkM.m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
    {
        kSum.m_afData[i] = m_afData[i] + rkM.m_afData[i];
    }
    return kSum;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::operator- (const moDMatrix& rkM) const
{
    moDMatrix<Real> kDiff(rkM.m_iRows,rkM.m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
    {
        kDiff.m_afData[i] = m_afData[i] - rkM.m_afData[i];
    }
    return kDiff;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::operator* (const moDMatrix& rkM) const
{
    // 'this' is RxN, 'M' is NxC, 'product = this*M' is RxC
    //assert(m_iCols == rkM.m_iRows);
    moDMatrix<Real> kProd(m_iRows,rkM.m_iCols);
    for (int iRow = 0; iRow < kProd.m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < kProd.m_iCols; iCol++)
        {
            for (int iMid = 0; iMid < m_iCols; iMid++)
            {
                kProd.m_aafEntry[iRow][iCol] += m_aafEntry[iRow][iMid] *
                    rkM.m_aafEntry[iMid][iCol];
            }
        }
    }
    return kProd;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::operator* (Real fScalar) const
{
    moDMatrix<Real> kProd(m_iRows,m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
    {
        kProd.m_afData[i] = fScalar*m_afData[i];
    }
    return kProd;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::operator/ (Real fScalar) const
{
    moDMatrix<Real> kQuot(m_iRows,m_iCols);
    int i;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iQuantity; i++)
        {
            kQuot.m_afData[i] = fInvScalar*m_afData[i];
        }
    }
    else
    {
        for (i = 0; i < m_iQuantity; i++)
        {
            kQuot.m_afData[i] = moMath<Real>::MAX_REAL;
        }
    }

    return kQuot;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::operator- () const
{
    moDMatrix<Real> kNeg(m_iRows,m_iCols);
    for (int i = 0; i < m_iQuantity; i++)
    {
        kNeg.m_afData[i] = -m_afData[i];
    }
    return kNeg;
}

template <class Real>
moDMatrix<Real> operator* (Real fScalar, const moDMatrix<Real>& rkM)
{
    moDMatrix<Real> kProd(rkM.GetRows(),rkM.GetColumns());
    const Real* afMEntry = rkM;
    Real* afPEntry = kProd;
    for (int i = 0; i < rkM.GetQuantity(); i++)
    {
        afPEntry[i] = fScalar*afMEntry[i];
    }
    return kProd;
}

template <class Real>
moDMatrix<Real>& moDMatrix<Real>::operator+= (const moDMatrix& rkM)
{
    for (int i = 0; i < m_iQuantity; i++)
    {
        m_afData[i] += rkM.m_afData[i];
    }
    return *this;
}

template <class Real>
moDMatrix<Real>& moDMatrix<Real>::operator-= (const moDMatrix& rkM)
{
    for (int i = 0; i < m_iQuantity; i++)
    {
        m_afData[i] -= rkM.m_afData[i];
    }
    return *this;
}

template <class Real>
moDMatrix<Real>& moDMatrix<Real>::operator*= (Real fScalar)
{
    for (int i = 0; i < m_iQuantity; i++)
    {
        m_afData[i] *= fScalar;
    }
    return *this;
}

template <class Real>
moDMatrix<Real>& moDMatrix<Real>::operator/= (Real fScalar)
{
    int i;

    if (fScalar != (Real)0.0)
    {
        Real fInvScalar = ((Real)1.0)/fScalar;
        for (i = 0; i < m_iQuantity; i++)
        {
            m_afData[i] *= fInvScalar;
        }
    }
    else
    {
        for (i = 0; i < m_iQuantity; i++)
        {
            m_afData[i] = moMath<Real>::MAX_REAL;
        }
    }

    return *this;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::Transpose () const
{
    moDMatrix<Real> kTranspose(m_iCols,m_iRows);
    for (int iRow = 0; iRow < m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < m_iCols; iCol++)
        {
            kTranspose.m_aafEntry[iCol][iRow] = m_aafEntry[iRow][iCol];
        }
    }
    return kTranspose;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::TransposeTimes (const moDMatrix& rkM) const
{
    // P = A^T*B, P[r][c] = sum_m A[m][r]*B[m][c]
    //assert(m_iRows == rkM.m_iRows);
    moDMatrix<Real> kProd(m_iCols,rkM.m_iCols);
    for (int iRow = 0; iRow < kProd.m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < kProd.m_iCols; iCol++)
        {
            for (int iMid = 0; iMid < m_iRows; iMid++)
            {
                kProd.m_aafEntry[iRow][iCol] += m_aafEntry[iMid][iRow] *
                    rkM.m_aafEntry[iMid][iCol];
            }
        }
    }
    return kProd;
}

template <class Real>
moDMatrix<Real> moDMatrix<Real>::TimesTranspose (const moDMatrix& rkM) const
{
    // P = A*B^T, P[r][c] = sum_m A[r][m]*B[c][m]
    //assert(m_iCols == rkM.m_iCols);
    moDMatrix<Real> kProd(m_iRows,rkM.m_iRows);
    for (int iRow = 0; iRow < kProd.m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < kProd.m_iCols; iCol++)
        {
            for (int iMid = 0; iMid < m_iCols; iMid++)
            {
                kProd.m_aafEntry[iRow][iCol] +=  m_aafEntry[iRow][iMid] *
                    rkM.m_aafEntry[iCol][iMid];
            }
        }
    }
    return kProd;
}

template <class Real>
moDVector<Real> moDMatrix<Real>::operator* (const moDVector<Real>& rkV) const
{
    //assert(rkV.GetSize() == m_iCols);
    moDVector<Real> kProd(m_iRows);
    for (int iRow = 0; iRow < m_iRows; iRow++)
    {
        for (int iCol = 0; iCol < m_iCols; iCol++)
        {
            kProd[iRow] += m_aafEntry[iRow][iCol]*rkV[iCol];
        }

    }
    return kProd;
}

template <class Real>
moDVector<Real> operator* (const moDVector<Real>& rkV, const moDMatrix<Real>& rkM)
{
    //assert(rkV.GetSize() == rkM.GetRows());
    moDVector<Real> kProd(rkM.GetColumns());
    Real* afPEntry = kProd;
    for (int iCol = 0; iCol < rkM.GetColumns(); iCol++)
    {
        for (int iRow = 0; iRow < rkM.GetRows(); iRow++)
        {
            afPEntry[iCol] += rkV[iRow]*rkM[iRow][iCol];
        }
    }
    return kProd;
}

template <class Real>
Real moDMatrix<Real>::QForm (const moDVector<Real>& rkU, const moDVector<Real>& rkV)
    const
{
    //assert(rkU.GetSize() == m_iRows && rkV.GetSize() == m_iCols);
    return rkU.Dot((*this)*rkV);
}

template <class Real>
bool moDMatrix<Real>::GetInverse (moDMatrix<Real>& rkInverse) const
{
    // computations are performed in-place
    if (GetRows() > 0 && GetRows() != GetColumns())
    {
        return false;
    }

    int iSize = GetRows();
    rkInverse = *this;

    int* aiColIndex = new int[iSize];
    int* aiRowIndex = new int[iSize];
    bool* abPivoted = new bool[iSize];
    memset(abPivoted,0,iSize*sizeof(bool));

    int i1, i2, iRow = 0, iCol = 0;
    Real fSave;

    // elimination by full pivoting
    for (int i0 = 0; i0 < iSize; i0++)
    {
        // search matrix (excluding pivoted rows) for maximum absolute entry
        Real fMax = (Real)0.0;
        for (i1 = 0; i1 < iSize; i1++)
        {
            if (!abPivoted[i1])
            {
                for (i2 = 0; i2 < iSize; i2++)
                {
                    if (!abPivoted[i2])
                    {
                        Real fAbs = moMath<Real>::FAbs(rkInverse[i1][i2]);
                        if (fAbs > fMax)
                        {
                            fMax = fAbs;
                            iRow = i1;
                            iCol = i2;
                        }
                    }
                }
            }
        }

        if (fMax == (Real)0.0)
        {
            // matrix is not invertible
            delete[] aiColIndex;
            delete[] aiRowIndex;
            delete[] abPivoted;
            return false;
        }

        abPivoted[iCol] = true;

        // swap rows so that A[iCol][iCol] contains the pivot entry
        if (iRow != iCol)
        {
            rkInverse.SwapRows(iRow,iCol);
        }

        // keep track of the permutations of the rows
        aiRowIndex[i0] = iRow;
        aiColIndex[i0] = iCol;

        // scale the row so that the pivot entry is 1
        Real fInv = ((Real)1.0)/rkInverse[iCol][iCol];
        rkInverse[iCol][iCol] = (Real)1.0;
        for (i2 = 0; i2 < iSize; i2++)
        {
            rkInverse[iCol][i2] *= fInv;
        }

        // zero out the pivot column locations in the other rows
        for (i1 = 0; i1 < iSize; i1++)
        {
            if (i1 != iCol)
            {
                fSave = rkInverse[i1][iCol];
                rkInverse[i1][iCol] = (Real)0.0;
                for (i2 = 0; i2 < iSize; i2++)
                {
                    rkInverse[i1][i2] -= rkInverse[iCol][i2]*fSave;
                }
            }
        }
    }

    // reorder rows so that A[][] stores the inverse of the original matrix
    for (i1 = iSize-1; i1 >= 0; i1--)
    {
        if (aiRowIndex[i1] != aiColIndex[i1])
        {
            for (i2 = 0; i2 < iSize; i2++)
            {
                fSave = rkInverse[i2][aiRowIndex[i1]];
                rkInverse[i2][aiRowIndex[i1]] =
                    rkInverse[i2][aiColIndex[i1]];
                rkInverse[i2][aiColIndex[i1]] = fSave;
            }
        }
    }

    delete[] aiColIndex;
    delete[] aiRowIndex;
    delete[] abPivoted;
    return true;
}

// moDBandedMatrix class ------------------------------------------------

template <class Real>
moDBandedMatrix<Real>::moDBandedMatrix (int iSize, int iLBands, int iUBands)
{
    //assert(iSize > 0 && iLBands >= 0 && iUBands >= 0);
    //assert(iLBands < iSize && iUBands < iSize);

    m_iSize = iSize;
    m_iLBands = iLBands;
    m_iUBands = iUBands;
    Allocate();
}

template <class Real>
moDBandedMatrix<Real>::moDBandedMatrix (const moDBandedMatrix& rkM) : moAbstract(rkM)
{
    m_afDBand = 0;
    m_aafLBand = 0;
    m_aafUBand = 0;
    *this = rkM;
}

template <class Real>
moDBandedMatrix<Real>::~moDBandedMatrix ()
{
    Deallocate();
}

template <class Real>
moDBandedMatrix<Real>& moDBandedMatrix<Real>::operator= (const moDBandedMatrix& rkM)
{
    Deallocate();
    m_iSize = rkM.m_iSize;
    m_iLBands = rkM.m_iLBands;
    m_iUBands = rkM.m_iUBands;
    Allocate();

    size_t uiSize = m_iSize*sizeof(Real);
    memcpy(m_afDBand,rkM.m_afDBand,uiSize);

    int i;
    for (i = 0; i < m_iLBands; i++)
    {
        uiSize = (m_iSize-1-i)*sizeof(Real);
        memcpy(m_aafLBand[i],rkM.m_aafLBand[i],uiSize);
    }

    for (i = 0; i < m_iUBands; i++)
    {
        uiSize = (m_iSize-1-i)*sizeof(Real);
        memcpy(m_aafUBand[i],rkM.m_aafUBand[i],uiSize);
    }

    return *this;
}

template <class Real>
int moDBandedMatrix<Real>::GetSize () const
{
    return m_iSize;
}

template <class Real>
int moDBandedMatrix<Real>::GetLBands () const
{
    return m_iLBands;
}

template <class Real>
int moDBandedMatrix<Real>::GetUBands () const
{
    return m_iUBands;
}

template <class Real>
Real* moDBandedMatrix<Real>::GetDBand ()
{
    return m_afDBand;
}

template <class Real>
const Real* moDBandedMatrix<Real>::GetDBand () const
{
    return m_afDBand;
}

template <class Real>
int moDBandedMatrix<Real>::GetLBandMax (int i) const
{
    //assert(0 <= i && i < m_iLBands);
    return m_iSize-1-i;
}

template <class Real>
Real* moDBandedMatrix<Real>::GetLBand (int i)
{
    if ( m_aafLBand )
    {
        //assert(0 <= i && i < m_iLBands);
        return m_aafLBand[i];
    }
    return 0;
}

template <class Real>
const Real* moDBandedMatrix<Real>::GetLBand (int i) const
{
    if (m_aafLBand)
    {
        //assert(0 <= i && i < m_iLBands);
        return m_aafLBand[i];
    }
    return 0;
}

template <class Real>
int moDBandedMatrix<Real>::GetUBandMax (int i) const
{
    //assert(0 <= i && i < m_iUBands);
    return m_iSize-1-i;
}

template <class Real>
Real* moDBandedMatrix<Real>::GetUBand (int i)
{
    if (m_aafUBand)
    {
        //assert(0 <= i && i < m_iUBands);
        return m_aafUBand[i];
    }
    return 0;
}

template <class Real>
const Real* moDBandedMatrix<Real>::GetUBand (int i) const
{
    if (m_aafUBand)
    {
        //assert(0 <= i && i < m_iUBands);
        return m_aafUBand[i];
    }
    return 0;
}

template <class Real>
Real& moDBandedMatrix<Real>::operator() (int iRow, int iCol)
{
    //assert(0 <= iRow && iRow < m_iSize && 0 <= iCol && iCol < m_iSize);

    int iBand = iCol - iRow;
    if (iBand > 0)
    {
        if (--iBand < m_iUBands && iRow < m_iSize-1-iBand)
        {
            return m_aafUBand[iBand][iRow];
        }
    }
    else if ( iBand < 0 )
    {
        iBand = -iBand;
        if (--iBand < m_iLBands && iCol < m_iSize-1-iBand)
        {
            return m_aafLBand[iBand][iCol];
        }
    }
    else
    {
        return m_afDBand[iRow];
    }

    static Real s_fDummy = (Real)0.0;
    return s_fDummy;
}

template <class Real>
Real moDBandedMatrix<Real>::operator() (int iRow, int iCol) const
{
    //assert(0 <= iRow && iRow < m_iSize && 0 <= iCol && iCol < m_iSize);

    int iBand = iCol - iRow;
    if (iBand > 0)
    {
        if (--iBand < m_iUBands && iRow < m_iSize-1-iBand)
        {
            return m_aafUBand[iBand][iRow];
        }
    }
    else if ( iBand < 0 )
    {
        iBand = -iBand;
        if (--iBand < m_iLBands && iCol < m_iSize-1-iBand)
        {
            return m_aafLBand[iBand][iCol];
        }
    }
    else
    {
        return m_afDBand[iRow];
    }

    return (Real)0.0;
}

template <class Real>
void moDBandedMatrix<Real>::SetZero ()
{
    //assert(m_iSize > 0);

    memset(m_afDBand,0,m_iSize*sizeof(Real));

    int i;
    for (i = 0; i < m_iLBands; i++)
    {
        memset(m_aafLBand[i],0,(m_iSize-1-i)*sizeof(Real));
    }

    for (i = 0; i < m_iUBands; i++)
    {
        memset(m_aafUBand[i],0,(m_iSize-1-i)*sizeof(Real));
    }
}

template <class Real>
void moDBandedMatrix<Real>::SetIdentity ()
{
    //assert(m_iSize > 0);

    int i;
    for (i = 0; i < m_iSize; i++)
    {
        m_afDBand[i] = (Real)1.0;
    }

    for (i = 0; i < m_iLBands; i++)
    {
        memset(m_aafLBand[i],0,(m_iSize-1-i)*sizeof(Real));
    }

    for (i = 0; i < m_iUBands; i++)
    {
        memset(m_aafUBand[i],0,(m_iSize-1-i)*sizeof(Real));
    }
}

template <class Real>
void moDBandedMatrix<Real>::Allocate ()
{
    // //assert:  m_iSize, m_iLBands, m_iRBandQuantity already set
    // //assert:  m_afDBand, m_aafLBand, m_aafUBand all null

    m_afDBand = new Real[m_iSize];
    memset(m_afDBand,0,m_iSize*sizeof(Real));

    if (m_iLBands > 0)
    {
        m_aafLBand = new Real*[m_iLBands];
    }
    else
    {
        m_aafLBand = 0;
    }

    if (m_iUBands > 0)
    {
        m_aafUBand = new Real*[m_iUBands];
    }
    else
    {
        m_aafUBand = 0;
    }

    int i;
    for (i = 0; i < m_iLBands; i++)
    {
        m_aafLBand[i] = new Real[m_iSize-1-i];
        memset(m_aafLBand[i],0,(m_iSize-1-i)*sizeof(Real));
    }

    for (i = 0; i < m_iUBands; i++)
    {
        m_aafUBand[i] = new Real[m_iSize-1-i];
        memset(m_aafUBand[i],0,(m_iSize-1-i)*sizeof(Real));
    }
}

template <class Real>
void moDBandedMatrix<Real>::Deallocate ()
{
    delete[] m_afDBand;

    int i;

    if (m_aafLBand)
    {
        for (i = 0; i < m_iLBands; i++)
        {
            delete[] m_aafLBand[i];
        }

        delete[] m_aafLBand;
        m_aafLBand = 0;
    }

    if (m_aafUBand)
    {
        for (i = 0; i < m_iUBands; i++)
        {
            delete[] m_aafUBand[i];
        }

        delete[] m_aafUBand;
        m_aafUBand = 0;
    }
}


