/*******************************************************************************

                                moMathMatrix.h

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
  Andr�s Colubri

  Portions taken from
  Wild Magic Source Code
  David Eberly
  http://www.geometrictools.com
  Copyright (c) 1998-2007

*******************************************************************************/

// Matrix operations are applied on the left.  For example, given a matrix M
// and a vector V, matrix-times-vector is M*V.  That is, V is treated as a
// column vector.  Some graphics APIs use V*M where V is treated as a row
// vector.  In this context the "M" matrix is really a transpose of the M as
// represented in Wild Magic.  Similarly, to apply two matrix operations M0
// and M1, in that order, you compute M1*M0 so that the transform of a vector
// is (M1*M0)*V = M1*(M0*V).  Some graphics APIs use M0*M1, but again these
// matrices are the transpose of those as represented in Wild Magic.  You
// must therefore be careful about how you interface the transformation code
// with graphics APIS.
//
// For memory organization it might seem natural to chose Real[N][N] for the
// matrix storage, but this can be a problem on a platform/console that
// chooses to store the data in column-major rather than row-major format.
// To avoid potential portability problems, the matrix is stored as Real[N*N]
// and organized in row-major order.  That is, the entry of the matrix in row
// r (0 <= r < N) and column c (0 <= c < N) is stored at index i = c+N*r
// (0 <= i < N*N).

// Rotation matrices are of the form
//   R = cos(t) -sin(t)
//       sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

// The (x,y,z) coordinate system is assumed to be right-handed.  Coordinate
// axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

#ifndef __MO_MATH_MATRIX_H__
#define __MO_MATH_MATRIX_H__

#include "moMathVector.h"
#include "moMathVector3.h"
#include "moMathVector4.h"

// moMatrix2 class ------------------------------------------------------------

template <class Real>
class LIBMOLDEO_API moMatrix2 : public moAbstract
{
public:

moMatrix2 (bool bZero = true )
{
    if (bZero)
    {
        MakeZero();
    }
    else
    {
        MakeIdentity();
    }
}

moMatrix2 (const moMatrix2& rkM) : moAbstract(rkM)
{
    m_bInitialized = rkM.m_bInitialized;
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
}

moMatrix2 (Real fM00, Real fM01, Real fM10, Real fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = fM01;
    m_afEntry[2] = fM10;
    m_afEntry[3] = fM11;
}

moMatrix2 (const Real afEntry[4], bool bRowMajor)
{
    if (bRowMajor)
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[1];
        m_afEntry[2] = afEntry[2];
        m_afEntry[3] = afEntry[3];
    }
    else
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[2];
        m_afEntry[2] = afEntry[1];
        m_afEntry[3] = afEntry[3];
    }
}

moMatrix2 (const moVector2<Real>& rkU, const moVector2<Real>& rkV,
    bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkV[0];
        m_afEntry[2] = rkU[1];
        m_afEntry[3] = rkV[1];
    }
    else
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkU[1];
        m_afEntry[2] = rkV[0];
        m_afEntry[3] = rkV[1];
    }
}

moMatrix2 (const moVector2<Real>* akV, bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[1][0];
        m_afEntry[2] = akV[0][1];
        m_afEntry[3] = akV[1][1];
    }
    else
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[0][1];
        m_afEntry[2] = akV[1][0];
        m_afEntry[3] = akV[1][1];
    }
}

moMatrix2 (Real fM00, Real fM11)
{
    MakeDiagonal(fM00,fM11);
}

moMatrix2 (Real fAngle)
{
    FromAngle(fAngle);
}


moMatrix2 (const moVector2<Real>& rkU, const moVector2<Real>& rkV)
{
    MakeTensorProduct(rkU,rkV);
}


void MakeZero ()
{
    m_afEntry[0] = (Real)0.0;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)0.0;
}


void MakeIdentity ()
{
    m_afEntry[0] = (Real)1.0;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)1.0;
}


void MakeDiagonal (Real fM00, Real fM11)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = fM11;
}


void FromAngle (Real fAngle)
{
    m_afEntry[0] = moMath<Real>::Cos(fAngle);
    m_afEntry[2] = moMath<Real>::Sin(fAngle);
    m_afEntry[1] = -m_afEntry[2];
    m_afEntry[3] =  m_afEntry[0];
}


void MakeTensorProduct (const moVector2<Real>& rkU,
    const moVector2<Real>& rkV)
{
    m_afEntry[0] = rkU[0]*rkV[0];
    m_afEntry[1] = rkU[0]*rkV[1];
    m_afEntry[2] = rkU[1]*rkV[0];
    m_afEntry[3] = rkU[1]*rkV[1];
}


void SetRow (int iRow, const moVector2<Real>& rkV)
{
    int i0 = 2*iRow ,i1 = i0+1;
    m_afEntry[i0] = rkV[0];
    m_afEntry[i1] = rkV[1];
}


moVector2<Real> GetRow (int iRow) const
{
    int i0 = 2*iRow ,i1 = i0+1;
    return moVector2<Real>(m_afEntry[i0],m_afEntry[i1]);
}


void SetColumn (int iCol, const moVector2<Real>& rkV)
{
    m_afEntry[iCol] = rkV[0];
    m_afEntry[iCol+2] = rkV[1];
}


moVector2<Real> GetColumn (int iCol) const
{
    return moVector2<Real>(m_afEntry[iCol],m_afEntry[iCol+2]);
}


void GetColumnMajor (Real* afCMajor) const
{
    afCMajor[0] = m_afEntry[0];
    afCMajor[1] = m_afEntry[2];
    afCMajor[2] = m_afEntry[1];
    afCMajor[3] = m_afEntry[3];
}




bool operator== (const moMatrix2& rkM) const
{
    return CompareArrays(rkM) == 0;
}


bool operator!= (const moMatrix2& rkM) const
{
    return CompareArrays(rkM) != 0;
}


bool operator<  (const moMatrix2& rkM) const
{
    return CompareArrays(rkM) < 0;
}


bool operator<= (const moMatrix2& rkM) const
{
    return CompareArrays(rkM) <= 0;
}


bool operator>  (const moMatrix2& rkM) const
{
    return CompareArrays(rkM) > 0;
}


bool operator>= (const moMatrix2& rkM) const
{
    return CompareArrays(rkM) >= 0;
}


moMatrix2<Real> Transpose () const
{
    return moMatrix2<Real>(
        m_afEntry[0],
        m_afEntry[2],
        m_afEntry[1],
        m_afEntry[3]);
}


moMatrix2<Real> TransposeTimes (const moMatrix2& rkM) const
{
    // P = A^T*B
    return moMatrix2<Real>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[2]*rkM.m_afEntry[2],
        m_afEntry[0]*rkM.m_afEntry[1] + m_afEntry[2]*rkM.m_afEntry[3],
        m_afEntry[1]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[2],
        m_afEntry[1]*rkM.m_afEntry[1] + m_afEntry[3]*rkM.m_afEntry[3]);
}

moMatrix2<Real> TimesTranspose (const moMatrix2& rkM) const
{
    // P = A*B^T
    return moMatrix2<Real>(
        m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[1]*rkM.m_afEntry[1],
        m_afEntry[0]*rkM.m_afEntry[2] + m_afEntry[1]*rkM.m_afEntry[3],
        m_afEntry[2]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[1],
        m_afEntry[2]*rkM.m_afEntry[2] + m_afEntry[3]*rkM.m_afEntry[3]);
}

moMatrix2<Real> Inverse () const
{

    moMatrix2 kInverse;

    Real fDet = m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
    if (moMath<Real>::FAbs(fDet) > moMath<Real>::ZERO_TOLERANCE)
    {
        Real fInvDet = ((Real)1.0)/fDet;
        kInverse.m_afEntry[0] =  m_afEntry[3]*fInvDet;
        kInverse.m_afEntry[1] = -m_afEntry[1]*fInvDet;
        kInverse.m_afEntry[2] = -m_afEntry[2]*fInvDet;
        kInverse.m_afEntry[3] =  m_afEntry[0]*fInvDet;
    }
    else
    {
        kInverse.m_afEntry[0] = (Real)0.0;
        kInverse.m_afEntry[1] = (Real)0.0;
        kInverse.m_afEntry[2] = (Real)0.0;
        kInverse.m_afEntry[3] = (Real)0.0;
    }

    return kInverse;
}


moMatrix2<Real> Adjoint () const
{
    return moMatrix2<Real>(
        m_afEntry[3],
        -m_afEntry[1],
        -m_afEntry[2],
        m_afEntry[0]);
}

Real Determinant () const
{
    return m_afEntry[0]*m_afEntry[3] - m_afEntry[1]*m_afEntry[2];
}


Real QForm (const moVector2<Real>& rkU,
    const moVector2<Real>& rkV) const
{
    return rkU.Dot((*this)*rkV);
}

void ToAngle (Real& rfAngle) const
{
    // assert:  matrix is a rotation
    rfAngle = moMath<Real>::ATan2(m_afEntry[2],m_afEntry[0]);
}


void Orthonormalize ()
{
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
    // M = [m0|m1], then orthonormal output matrix is Q = [q0|q1],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.

    // compute q0
    Real fInvLength = moMath<Real>::InvSqrt(m_afEntry[0]*m_afEntry[0] +
        m_afEntry[2]*m_afEntry[2]);

    m_afEntry[0] *= fInvLength;
    m_afEntry[2] *= fInvLength;

    // compute q1
    Real fDot0 = m_afEntry[0]*m_afEntry[1] + m_afEntry[2]*m_afEntry[3];
    m_afEntry[1] -= fDot0*m_afEntry[0];
    m_afEntry[3] -= fDot0*m_afEntry[2];

    fInvLength = moMath<Real>::InvSqrt(m_afEntry[1]*m_afEntry[1] +
        m_afEntry[3]*m_afEntry[3]);

    m_afEntry[1] *= fInvLength;
    m_afEntry[3] *= fInvLength;
}


void EigenDecomposition (moMatrix2& rkRot, moMatrix2& rkDiag) const
{
    Real fTrace = m_afEntry[0] + m_afEntry[3];
    Real fDiff = m_afEntry[0] - m_afEntry[3];
    Real fDiscr = moMath<Real>::Sqrt(fDiff*fDiff +
        ((Real)4.0)*m_afEntry[1]*m_afEntry[1]);
    Real fEVal0 = ((Real)0.5)*(fTrace-fDiscr);
    Real fEVal1 = ((Real)0.5)*(fTrace+fDiscr);
    rkDiag.MakeDiagonal(fEVal0,fEVal1);

    Real fCos, fSin;
    if (fDiff >= (Real)0.0)
    {
        fCos = m_afEntry[1];
        fSin = fEVal0 - m_afEntry[0];
    }
    else
    {
        fCos = fEVal0 - m_afEntry[3];
        fSin = m_afEntry[1];
    }
    Real fTmp = moMath<Real>::InvSqrt(fCos*fCos + fSin*fSin);
    fCos *= fTmp;
    fSin *= fTmp;

    rkRot.m_afEntry[0] = fCos;
    rkRot.m_afEntry[1] = -fSin;
    rkRot.m_afEntry[2] = fSin;
    rkRot.m_afEntry[3] = fCos;
}

    // assignment
    inline moMatrix2& operator= (const moMatrix2& rkM)
    {
        m_afEntry[0] = rkM.m_afEntry[0];
        m_afEntry[1] = rkM.m_afEntry[1];
        m_afEntry[2] = rkM.m_afEntry[2];
        m_afEntry[3] = rkM.m_afEntry[3];
        return *this;
    }

    // arithmetic operations
    inline moMatrix2 operator+ (const moMatrix2& rkM) const
    {
        return moMatrix2<Real>(
            m_afEntry[0] + rkM.m_afEntry[0],
            m_afEntry[1] + rkM.m_afEntry[1],
            m_afEntry[2] + rkM.m_afEntry[2],
            m_afEntry[3] + rkM.m_afEntry[3]);
    }
    inline moMatrix2 operator- (const moMatrix2& rkM) const
    {
        return moMatrix2<Real>(
            m_afEntry[0] - rkM.m_afEntry[0],
            m_afEntry[1] - rkM.m_afEntry[1],
            m_afEntry[2] - rkM.m_afEntry[2],
            m_afEntry[3] - rkM.m_afEntry[3]);
    }
    inline moMatrix2 operator* (const moMatrix2& rkM) const
    {
        return moMatrix2<Real>(
            m_afEntry[0]*rkM.m_afEntry[0] + m_afEntry[1]*rkM.m_afEntry[2],
            m_afEntry[0]*rkM.m_afEntry[1] + m_afEntry[1]*rkM.m_afEntry[3],
            m_afEntry[2]*rkM.m_afEntry[0] + m_afEntry[3]*rkM.m_afEntry[2],
            m_afEntry[2]*rkM.m_afEntry[1] + m_afEntry[3]*rkM.m_afEntry[3]);
    }
    inline moMatrix2 operator* (Real fScalar) const
    {
        return moMatrix2<Real>(
            fScalar*m_afEntry[0],
            fScalar*m_afEntry[1],
            fScalar*m_afEntry[2],
            fScalar*m_afEntry[3]);
    }
    inline moMatrix2 operator/ (Real fScalar) const
    {
        if (fScalar != (Real)0.0)
        {
            Real fInvScalar = ((Real)1.0)/fScalar;
            return moMatrix2<Real>(
                fInvScalar*m_afEntry[0],
                fInvScalar*m_afEntry[1],
                fInvScalar*m_afEntry[2],
                fInvScalar*m_afEntry[3]);
        }

        return moMatrix2<Real>(
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL);
    }
	inline moMatrix2 operator- () const
    {
        return moMatrix2<Real>(
            -m_afEntry[0],
            -m_afEntry[1],
            -m_afEntry[2],
            -m_afEntry[3]);
    }

    // arithmetic updates
    inline moMatrix2& operator+= (const moMatrix2& rkM)
    {
        m_afEntry[0] += rkM.m_afEntry[0];
        m_afEntry[1] += rkM.m_afEntry[1];
        m_afEntry[2] += rkM.m_afEntry[2];
        m_afEntry[3] += rkM.m_afEntry[3];
        return *this;
    }
    inline moMatrix2& operator-= (const moMatrix2& rkM)
    {
        m_afEntry[0] -= rkM.m_afEntry[0];
        m_afEntry[1] -= rkM.m_afEntry[1];
        m_afEntry[2] -= rkM.m_afEntry[2];
        m_afEntry[3] -= rkM.m_afEntry[3];
        return *this;
    }
    inline moMatrix2& operator*= (Real fScalar)
    {
        m_afEntry[0] *= fScalar;
        m_afEntry[1] *= fScalar;
        m_afEntry[2] *= fScalar;
        m_afEntry[3] *= fScalar;
        return *this;
    }
    inline moMatrix2& operator/= (Real fScalar)
    {
        if (fScalar != (Real)0.0)
        {
            Real fInvScalar = ((Real)1.0)/fScalar;
            m_afEntry[0] *= fInvScalar;
            m_afEntry[1] *= fInvScalar;
            m_afEntry[2] *= fInvScalar;
            m_afEntry[3] *= fInvScalar;
        }
        else
        {
            m_afEntry[0] = moMath<Real>::MAX_REAL;
            m_afEntry[1] = moMath<Real>::MAX_REAL;
            m_afEntry[2] = moMath<Real>::MAX_REAL;
            m_afEntry[3] = moMath<Real>::MAX_REAL;
        }

        return *this;
    }

    // matrix times vector
    inline moVector2<Real> operator* (const moVector2<Real>& rkV) const  // M * v
    {
        return moVector2<Real>(
            m_afEntry[0]*rkV[0] + m_afEntry[1]*rkV[1],
            m_afEntry[2]*rkV[0] + m_afEntry[3]*rkV[1]);
    }


    // The matrix must be a rotation for these functions to be valid.  The
    // last function uses Gram-Schmidt orthonormalization applied to the
    // columns of the rotation matrix.  The angle must be in radians, not
    // degrees.

    // The matrix must be symmetric.  Factor M = R * D * R^T where
    // R = [u0|u1] is a rotation matrix with columns u0 and u1 and
    // D = diag(d0,d1) is a diagonal matrix whose diagonal entries are d0 and
    // d1.  The eigenvector u[i] corresponds to eigenvector d[i].  The
    // eigenvalues are ordered as d0 <= d1.

    static const moMatrix2 ZERO;
    static const moMatrix2 IDENTITY;

private:
    // support for comparisons
int CompareArrays (const moMatrix2& rkM) const
{
    return memcmp(m_afEntry,rkM.m_afEntry,4*sizeof(Real));
}


    // matrix stored in row-major order
    Real m_afEntry[4];
};

// c * M
template <class Real>
inline LIBMOLDEO_API moMatrix2<Real> operator* (Real fScalar, const moMatrix2<Real>& rkM)
{
    return rkM*fScalar;
}

// v^T * M
template <class Real>
inline LIBMOLDEO_API moVector2<Real> operator* (const moVector2<Real>& rkV,
    const moMatrix2<Real>& rkM)
{
    return moVector2<Real>(
        rkV[0]*rkM[0][0] + rkV[1]*rkM[1][0],
        rkV[0]*rkM[0][1] + rkV[1]*rkM[1][1]);
}

#ifndef MO_MACOSX
template class LIBMOLDEO_API moMatrix2<MOfloat>;
#endif
typedef moMatrix2<MOfloat> moMatrix2f;

#ifndef MO_MACOSX
template class LIBMOLDEO_API moMatrix2<MOdouble>;
#endif
typedef moMatrix2<MOdouble> moMatrix2d;

// moMatrix3 class ------------------------------------------------------------



template <class Real>
class moMatrix3 : public moAbstract
{
public:



moMatrix3 (bool bZero = true)
{
    if (bZero)
    {
        MakeZero();
    }
    else
    {
        MakeIdentity();
    }
}


moMatrix3 (const moMatrix3& rkM) : moAbstract(rkM)
{
    m_afEntry[0] = rkM.m_afEntry[0];
    m_afEntry[1] = rkM.m_afEntry[1];
    m_afEntry[2] = rkM.m_afEntry[2];
    m_afEntry[3] = rkM.m_afEntry[3];
    m_afEntry[4] = rkM.m_afEntry[4];
    m_afEntry[5] = rkM.m_afEntry[5];
    m_afEntry[6] = rkM.m_afEntry[6];
    m_afEntry[7] = rkM.m_afEntry[7];
    m_afEntry[8] = rkM.m_afEntry[8];
}


moMatrix3 (Real fM00, Real fM01, Real fM02, Real fM10, Real fM11,
    Real fM12, Real fM20, Real fM21, Real fM22)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = fM01;
    m_afEntry[2] = fM02;
    m_afEntry[3] = fM10;
    m_afEntry[4] = fM11;
    m_afEntry[5] = fM12;
    m_afEntry[6] = fM20;
    m_afEntry[7] = fM21;
    m_afEntry[8] = fM22;
}


moMatrix3 (const Real afEntry[9], bool bRowMajor)
{
    if (bRowMajor)
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[1];
        m_afEntry[2] = afEntry[2];
        m_afEntry[3] = afEntry[3];
        m_afEntry[4] = afEntry[4];
        m_afEntry[5] = afEntry[5];
        m_afEntry[6] = afEntry[6];
        m_afEntry[7] = afEntry[7];
        m_afEntry[8] = afEntry[8];
    }
    else
    {
        m_afEntry[0] = afEntry[0];
        m_afEntry[1] = afEntry[3];
        m_afEntry[2] = afEntry[6];
        m_afEntry[3] = afEntry[1];
        m_afEntry[4] = afEntry[4];
        m_afEntry[5] = afEntry[7];
        m_afEntry[6] = afEntry[2];
        m_afEntry[7] = afEntry[5];
        m_afEntry[8] = afEntry[8];
    }
}


moMatrix3 (const moVector3<Real>& rkU, const moVector3<Real>& rkV,
    const moVector3<Real>& rkW, bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkV[0];
        m_afEntry[2] = rkW[0];
        m_afEntry[3] = rkU[1];
        m_afEntry[4] = rkV[1];
        m_afEntry[5] = rkW[1];
        m_afEntry[6] = rkU[2];
        m_afEntry[7] = rkV[2];
        m_afEntry[8] = rkW[2];
    }
    else
    {
        m_afEntry[0] = rkU[0];
        m_afEntry[1] = rkU[1];
        m_afEntry[2] = rkU[2];
        m_afEntry[3] = rkV[0];
        m_afEntry[4] = rkV[1];
        m_afEntry[5] = rkV[2];
        m_afEntry[6] = rkW[0];
        m_afEntry[7] = rkW[1];
        m_afEntry[8] = rkW[2];
    }
}


moMatrix3 (const moVector3<Real>* akV, bool bColumns)
{
    if (bColumns)
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[1][0];
        m_afEntry[2] = akV[2][0];
        m_afEntry[3] = akV[0][1];
        m_afEntry[4] = akV[1][1];
        m_afEntry[5] = akV[2][1];
        m_afEntry[6] = akV[0][2];
        m_afEntry[7] = akV[1][2];
        m_afEntry[8] = akV[2][2];
    }
    else
    {
        m_afEntry[0] = akV[0][0];
        m_afEntry[1] = akV[0][1];
        m_afEntry[2] = akV[0][2];
        m_afEntry[3] = akV[1][0];
        m_afEntry[4] = akV[1][1];
        m_afEntry[5] = akV[1][2];
        m_afEntry[6] = akV[2][0];
        m_afEntry[7] = akV[2][1];
        m_afEntry[8] = akV[2][2];
    }
}


moMatrix3 (Real fM00, Real fM11, Real fM22)
{
    MakeDiagonal(fM00,fM11,fM22);
}


moMatrix3 (const moVector3<Real>& rkAxis, Real fAngle)
{
    FromAxisAngle(rkAxis,fAngle);
}


moMatrix3 (const moVector3<Real>& rkU, const moVector3<Real>& rkV)
{
    MakeTensorProduct(rkU,rkV);
}


moMatrix3<Real>& MakeZero ()
{
    m_afEntry[0] = (Real)0.0;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)0.0;
    m_afEntry[4] = (Real)0.0;
    m_afEntry[5] = (Real)0.0;
    m_afEntry[6] = (Real)0.0;
    m_afEntry[7] = (Real)0.0;
    m_afEntry[8] = (Real)0.0;
    return *this;
}


moMatrix3<Real>& MakeIdentity ()
{
    m_afEntry[0] = (Real)1.0;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)0.0;
    m_afEntry[4] = (Real)1.0;
    m_afEntry[5] = (Real)0.0;
    m_afEntry[6] = (Real)0.0;
    m_afEntry[7] = (Real)0.0;
    m_afEntry[8] = (Real)1.0;
    return *this;
}


moMatrix3<Real>& MakeDiagonal (Real fM00, Real fM11, Real fM22)
{
    m_afEntry[0] = fM00;
    m_afEntry[1] = (Real)0.0;
    m_afEntry[2] = (Real)0.0;
    m_afEntry[3] = (Real)0.0;
    m_afEntry[4] = fM11;
    m_afEntry[5] = (Real)0.0;
    m_afEntry[6] = (Real)0.0;
    m_afEntry[7] = (Real)0.0;
    m_afEntry[8] = fM22;
    return *this;
}


moMatrix3<Real>& FromAxisAngle (const moVector3<Real>& rkAxis,
    Real fAngle)
{
    Real fCos = moMath<Real>::Cos(fAngle);
    Real fSin = moMath<Real>::Sin(fAngle);
    Real fOneMinusCos = ((Real)1.0)-fCos;
    Real fX2 = rkAxis[0]*rkAxis[0];
    Real fY2 = rkAxis[1]*rkAxis[1];
    Real fZ2 = rkAxis[2]*rkAxis[2];
    Real fXYM = rkAxis[0]*rkAxis[1]*fOneMinusCos;
    Real fXZM = rkAxis[0]*rkAxis[2]*fOneMinusCos;
    Real fYZM = rkAxis[1]*rkAxis[2]*fOneMinusCos;
    Real fXSin = rkAxis[0]*fSin;
    Real fYSin = rkAxis[1]*fSin;
    Real fZSin = rkAxis[2]*fSin;

    m_afEntry[0] = fX2*fOneMinusCos+fCos;
    m_afEntry[1] = fXYM-fZSin;
    m_afEntry[2] = fXZM+fYSin;
    m_afEntry[3] = fXYM+fZSin;
    m_afEntry[4] = fY2*fOneMinusCos+fCos;
    m_afEntry[5] = fYZM-fXSin;
    m_afEntry[6] = fXZM-fYSin;
    m_afEntry[7] = fYZM+fXSin;
    m_afEntry[8] = fZ2*fOneMinusCos+fCos;

    return *this;
}


moMatrix3<Real>& MakeTensorProduct (const moVector3<Real>& rkU,
    const moVector3<Real>& rkV)
{
    m_afEntry[0] = rkU[0]*rkV[0];
    m_afEntry[1] = rkU[0]*rkV[1];
    m_afEntry[2] = rkU[0]*rkV[2];
    m_afEntry[3] = rkU[1]*rkV[0];
    m_afEntry[4] = rkU[1]*rkV[1];
    m_afEntry[5] = rkU[1]*rkV[2];
    m_afEntry[6] = rkU[2]*rkV[0];
    m_afEntry[7] = rkU[2]*rkV[1];
    m_afEntry[8] = rkU[2]*rkV[2];
    return *this;
}


void SetRow (int iRow, const moVector3<Real>& rkV)
{
    int i0 = 3*iRow, i1 = i0+1, i2 = i1+1;
    m_afEntry[i0] = rkV[0];
    m_afEntry[i1] = rkV[1];
    m_afEntry[i2] = rkV[2];
}


moVector3<Real> GetRow (int iRow) const
{
    int i0 = 3*iRow, i1 = i0+1, i2 = i1+1;
    return moVector3<Real>(m_afEntry[i0],m_afEntry[i1],m_afEntry[i2]);
}


void SetColumn (int iCol, const moVector3<Real>& rkV)
{
    m_afEntry[iCol] = rkV[0];
    m_afEntry[iCol+3] = rkV[1];
    m_afEntry[iCol+6] = rkV[2];
}


moVector3<Real> GetColumn (int iCol) const
{
    return moVector3<Real>(m_afEntry[iCol],m_afEntry[iCol+3],m_afEntry[iCol+6]);
}


void GetColumnMajor (Real* afCMajor) const
{
    afCMajor[0] = m_afEntry[0];
    afCMajor[1] = m_afEntry[3];
    afCMajor[2] = m_afEntry[6];
    afCMajor[3] = m_afEntry[1];
    afCMajor[4] = m_afEntry[4];
    afCMajor[5] = m_afEntry[7];
    afCMajor[6] = m_afEntry[2];
    afCMajor[7] = m_afEntry[5];
    afCMajor[8] = m_afEntry[8];
}





bool operator== (const moMatrix3& rkM) const
{
    return CompareArrays(rkM) == 0;
}


bool operator!= (const moMatrix3& rkM) const
{
    return CompareArrays(rkM) != 0;
}


bool operator<  (const moMatrix3& rkM) const
{
    return CompareArrays(rkM) < 0;
}


bool operator<= (const moMatrix3& rkM) const
{
    return CompareArrays(rkM) <= 0;
}


bool operator>  (const moMatrix3& rkM) const
{
    return CompareArrays(rkM) > 0;
}


bool operator>= (const moMatrix3& rkM) const
{
    return CompareArrays(rkM) >= 0;
}


moMatrix3<Real> Transpose () const
{
    return moMatrix3<Real>(
        m_afEntry[0],
        m_afEntry[3],
        m_afEntry[6],
        m_afEntry[1],
        m_afEntry[4],
        m_afEntry[7],
        m_afEntry[2],
        m_afEntry[5],
        m_afEntry[8]);
}


moMatrix3<Real> TransposeTimes (const moMatrix3& rkM) const
{
    // P = A^T*B
    return moMatrix3<Real>(
        m_afEntry[0]*rkM.m_afEntry[0] +
        m_afEntry[3]*rkM.m_afEntry[3] +
        m_afEntry[6]*rkM.m_afEntry[6],

        m_afEntry[0]*rkM.m_afEntry[1] +
        m_afEntry[3]*rkM.m_afEntry[4] +
        m_afEntry[6]*rkM.m_afEntry[7],

        m_afEntry[0]*rkM.m_afEntry[2] +
        m_afEntry[3]*rkM.m_afEntry[5] +
        m_afEntry[6]*rkM.m_afEntry[8],

        m_afEntry[1]*rkM.m_afEntry[0] +
        m_afEntry[4]*rkM.m_afEntry[3] +
        m_afEntry[7]*rkM.m_afEntry[6],

        m_afEntry[1]*rkM.m_afEntry[1] +
        m_afEntry[4]*rkM.m_afEntry[4] +
        m_afEntry[7]*rkM.m_afEntry[7],

        m_afEntry[1]*rkM.m_afEntry[2] +
        m_afEntry[4]*rkM.m_afEntry[5] +
        m_afEntry[7]*rkM.m_afEntry[8],

        m_afEntry[2]*rkM.m_afEntry[0] +
        m_afEntry[5]*rkM.m_afEntry[3] +
        m_afEntry[8]*rkM.m_afEntry[6],

        m_afEntry[2]*rkM.m_afEntry[1] +
        m_afEntry[5]*rkM.m_afEntry[4] +
        m_afEntry[8]*rkM.m_afEntry[7],

        m_afEntry[2]*rkM.m_afEntry[2] +
        m_afEntry[5]*rkM.m_afEntry[5] +
        m_afEntry[8]*rkM.m_afEntry[8]);
}


moMatrix3<Real> TimesTranspose (const moMatrix3& rkM) const
{
    // P = A*B^T
    return moMatrix3<Real>(
        m_afEntry[0]*rkM.m_afEntry[0] +
        m_afEntry[1]*rkM.m_afEntry[1] +
        m_afEntry[2]*rkM.m_afEntry[2],

        m_afEntry[0]*rkM.m_afEntry[3] +
        m_afEntry[1]*rkM.m_afEntry[4] +
        m_afEntry[2]*rkM.m_afEntry[5],

        m_afEntry[0]*rkM.m_afEntry[6] +
        m_afEntry[1]*rkM.m_afEntry[7] +
        m_afEntry[2]*rkM.m_afEntry[8],

        m_afEntry[3]*rkM.m_afEntry[0] +
        m_afEntry[4]*rkM.m_afEntry[1] +
        m_afEntry[5]*rkM.m_afEntry[2],

        m_afEntry[3]*rkM.m_afEntry[3] +
        m_afEntry[4]*rkM.m_afEntry[4] +
        m_afEntry[5]*rkM.m_afEntry[5],

        m_afEntry[3]*rkM.m_afEntry[6] +
        m_afEntry[4]*rkM.m_afEntry[7] +
        m_afEntry[5]*rkM.m_afEntry[8],

        m_afEntry[6]*rkM.m_afEntry[0] +
        m_afEntry[7]*rkM.m_afEntry[1] +
        m_afEntry[8]*rkM.m_afEntry[2],

        m_afEntry[6]*rkM.m_afEntry[3] +
        m_afEntry[7]*rkM.m_afEntry[4] +
        m_afEntry[8]*rkM.m_afEntry[5],

        m_afEntry[6]*rkM.m_afEntry[6] +
        m_afEntry[7]*rkM.m_afEntry[7] +
        m_afEntry[8]*rkM.m_afEntry[8]);
}


moMatrix3<Real> Inverse () const
{
    // Invert a 3x3 using cofactors.  This is faster than using a generic
    // Gaussian elimination because of the loop overhead of such a method.


    moMatrix3 kInverse;

    kInverse.m_afEntry[0] =
        m_afEntry[4]*m_afEntry[8] - m_afEntry[5]*m_afEntry[7];
    kInverse.m_afEntry[1] =
        m_afEntry[2]*m_afEntry[7] - m_afEntry[1]*m_afEntry[8];
    kInverse.m_afEntry[2] =
        m_afEntry[1]*m_afEntry[5] - m_afEntry[2]*m_afEntry[4];
    kInverse.m_afEntry[3] =
        m_afEntry[5]*m_afEntry[6] - m_afEntry[3]*m_afEntry[8];
    kInverse.m_afEntry[4] =
        m_afEntry[0]*m_afEntry[8] - m_afEntry[2]*m_afEntry[6];
    kInverse.m_afEntry[5] =
        m_afEntry[2]*m_afEntry[3] - m_afEntry[0]*m_afEntry[5];
    kInverse.m_afEntry[6] =
        m_afEntry[3]*m_afEntry[7] - m_afEntry[4]*m_afEntry[6];
    kInverse.m_afEntry[7] =
        m_afEntry[1]*m_afEntry[6] - m_afEntry[0]*m_afEntry[7];
    kInverse.m_afEntry[8] =
        m_afEntry[0]*m_afEntry[4] - m_afEntry[1]*m_afEntry[3];

    Real fDet =
        m_afEntry[0]*kInverse.m_afEntry[0] +
        m_afEntry[1]*kInverse.m_afEntry[3] +
        m_afEntry[2]*kInverse.m_afEntry[6];

    if (moMath<Real>::FAbs(fDet) <= moMath<Real>::ZERO_TOLERANCE)
    {
        return ZERO;
    }

    Real fInvDet = ((Real)1.0)/fDet;
    kInverse.m_afEntry[0] *= fInvDet;
    kInverse.m_afEntry[1] *= fInvDet;
    kInverse.m_afEntry[2] *= fInvDet;
    kInverse.m_afEntry[3] *= fInvDet;
    kInverse.m_afEntry[4] *= fInvDet;
    kInverse.m_afEntry[5] *= fInvDet;
    kInverse.m_afEntry[6] *= fInvDet;
    kInverse.m_afEntry[7] *= fInvDet;
    kInverse.m_afEntry[8] *= fInvDet;
    return kInverse;
}


moMatrix3<Real> Adjoint () const
{
    return moMatrix3<Real>(
        m_afEntry[4]*m_afEntry[8] - m_afEntry[5]*m_afEntry[7],
        m_afEntry[2]*m_afEntry[7] - m_afEntry[1]*m_afEntry[8],
        m_afEntry[1]*m_afEntry[5] - m_afEntry[2]*m_afEntry[4],
        m_afEntry[5]*m_afEntry[6] - m_afEntry[3]*m_afEntry[8],
        m_afEntry[0]*m_afEntry[8] - m_afEntry[2]*m_afEntry[6],
        m_afEntry[2]*m_afEntry[3] - m_afEntry[0]*m_afEntry[5],
        m_afEntry[3]*m_afEntry[7] - m_afEntry[4]*m_afEntry[6],
        m_afEntry[1]*m_afEntry[6] - m_afEntry[0]*m_afEntry[7],
        m_afEntry[0]*m_afEntry[4] - m_afEntry[1]*m_afEntry[3]);
}


Real Determinant () const
{
    Real fCo00 = m_afEntry[4]*m_afEntry[8] - m_afEntry[5]*m_afEntry[7];
    Real fCo10 = m_afEntry[5]*m_afEntry[6] - m_afEntry[3]*m_afEntry[8];
    Real fCo20 = m_afEntry[3]*m_afEntry[7] - m_afEntry[4]*m_afEntry[6];
    Real fDet = m_afEntry[0]*fCo00 + m_afEntry[1]*fCo10 + m_afEntry[2]*fCo20;
    return fDet;
}


Real QForm (const moVector3<Real>& rkU, const moVector3<Real>& rkV)
    const
{
    return rkU.Dot((*this)*rkV);
}


moMatrix3<Real> TimesDiagonal (const moVector3<Real>& rkDiag) const
{
    return moMatrix3<Real>(
        m_afEntry[0]*rkDiag[0],m_afEntry[1]*rkDiag[1],m_afEntry[2]*rkDiag[2],
        m_afEntry[3]*rkDiag[0],m_afEntry[4]*rkDiag[1],m_afEntry[5]*rkDiag[2],
        m_afEntry[6]*rkDiag[0],m_afEntry[7]*rkDiag[1],m_afEntry[8]*rkDiag[2]);
}


moMatrix3<Real> DiagonalTimes (const moVector3<Real>& rkDiag) const
{
    return moMatrix3<Real>(
        rkDiag[0]*m_afEntry[0],rkDiag[0]*m_afEntry[1],rkDiag[0]*m_afEntry[2],
        rkDiag[1]*m_afEntry[3],rkDiag[1]*m_afEntry[4],rkDiag[1]*m_afEntry[5],
        rkDiag[2]*m_afEntry[6],rkDiag[2]*m_afEntry[7],rkDiag[2]*m_afEntry[8]);
}


void ToAxisAngle (moVector3<Real>& rkAxis, Real& rfAngle) const
{
    // Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
    // The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
    // I is the identity and
    //
    //       +-        -+
    //   P = |  0 -z +y |
    //       | +z  0 -x |
    //       | -y +x  0 |
    //       +-        -+
    //
    // If A > 0, R represents a counterclockwise rotation about the axis in
    // the sense of looking from the tip of the axis vector towards the
    // origin.  Some algebra will show that
    //
    //   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
    //
    // In the event that A = pi, R-R^t = 0 which prevents us from extracting
    // the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
    // P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
    // z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
    // it does not matter which sign you choose on the square roots.

    Real fTrace = m_afEntry[0] + m_afEntry[4] + m_afEntry[8];
    Real fCos = ((Real)0.5)*(fTrace-(Real)1.0);
    rfAngle = moMath<Real>::ACos(fCos);  // in [0,PI]

    if (rfAngle > (Real)0.0)
    {
        if (rfAngle < moMath<Real>::PI)
        {
            rkAxis[0] = m_afEntry[7]-m_afEntry[5];
            rkAxis[1] = m_afEntry[2]-m_afEntry[6];
            rkAxis[2] = m_afEntry[3]-m_afEntry[1];
            rkAxis.Normalize();
        }
        else
        {
            // angle is PI
            Real fHalfInverse;
            if (m_afEntry[0] >= m_afEntry[4])
            {
                // r00 >= r11
                if (m_afEntry[0] >= m_afEntry[8])
                {
                    // r00 is maximum diagonal term
                    rkAxis[0] = ((Real)0.5)*moMath<Real>::Sqrt(m_afEntry[0] -
                        m_afEntry[4] - m_afEntry[8] + (Real)1.0);
                    fHalfInverse = ((Real)0.5)/rkAxis[0];
                    rkAxis[1] = fHalfInverse*m_afEntry[1];
                    rkAxis[2] = fHalfInverse*m_afEntry[2];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rkAxis[2] = ((Real)0.5)*moMath<Real>::Sqrt(m_afEntry[8] -
                        m_afEntry[0] - m_afEntry[4] + (Real)1.0);
                    fHalfInverse = ((Real)0.5)/rkAxis[2];
                    rkAxis[0] = fHalfInverse*m_afEntry[2];
                    rkAxis[1] = fHalfInverse*m_afEntry[5];
                }
            }
            else
            {
                // r11 > r00
                if (m_afEntry[4] >= m_afEntry[8])
                {
                    // r11 is maximum diagonal term
                    rkAxis[1] = ((Real)0.5)*moMath<Real>::Sqrt(m_afEntry[4] -
                        m_afEntry[0] - m_afEntry[8] + (Real)1.0);
                    fHalfInverse  = ((Real)0.5)/rkAxis[1];
                    rkAxis[0] = fHalfInverse*m_afEntry[1];
                    rkAxis[2] = fHalfInverse*m_afEntry[5];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rkAxis[2] = ((Real)0.5)*moMath<Real>::Sqrt(m_afEntry[8] -
                        m_afEntry[0] - m_afEntry[4] + (Real)1.0);
                    fHalfInverse = ((Real)0.5)/rkAxis[2];
                    rkAxis[0] = fHalfInverse*m_afEntry[2];
                    rkAxis[1] = fHalfInverse*m_afEntry[5];
                }
            }
        }
    }
    else
    {
        // The angle is 0 and the matrix is the identity.  Any axis will
        // work, so just use the x-axis.
        rkAxis[0] = (Real)1.0;
        rkAxis[1] = (Real)0.0;
        rkAxis[2] = (Real)0.0;
    }
}


void Orthonormalize ()
{
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
    // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.

    // compute q0
    Real fInvLength = moMath<Real>::InvSqrt(m_afEntry[0]*m_afEntry[0] +
        m_afEntry[3]*m_afEntry[3] + m_afEntry[6]*m_afEntry[6]);

    m_afEntry[0] *= fInvLength;
    m_afEntry[3] *= fInvLength;
    m_afEntry[6] *= fInvLength;

    // compute q1
    Real fDot0 = m_afEntry[0]*m_afEntry[1] + m_afEntry[3]*m_afEntry[4] +
        m_afEntry[6]*m_afEntry[7];

    m_afEntry[1] -= fDot0*m_afEntry[0];
    m_afEntry[4] -= fDot0*m_afEntry[3];
    m_afEntry[7] -= fDot0*m_afEntry[6];

    fInvLength = moMath<Real>::InvSqrt(m_afEntry[1]*m_afEntry[1] +
        m_afEntry[4]*m_afEntry[4] + m_afEntry[7]*m_afEntry[7]);

    m_afEntry[1] *= fInvLength;
    m_afEntry[4] *= fInvLength;
    m_afEntry[7] *= fInvLength;

    // compute q2
    Real fDot1 = m_afEntry[1]*m_afEntry[2] + m_afEntry[4]*m_afEntry[5] +
        m_afEntry[7]*m_afEntry[8];

    fDot0 = m_afEntry[0]*m_afEntry[2] + m_afEntry[3]*m_afEntry[5] +
        m_afEntry[6]*m_afEntry[8];

    m_afEntry[2] -= fDot0*m_afEntry[0] + fDot1*m_afEntry[1];
    m_afEntry[5] -= fDot0*m_afEntry[3] + fDot1*m_afEntry[4];
    m_afEntry[8] -= fDot0*m_afEntry[6] + fDot1*m_afEntry[7];

    fInvLength = moMath<Real>::InvSqrt(m_afEntry[2]*m_afEntry[2] +
        m_afEntry[5]*m_afEntry[5] + m_afEntry[8]*m_afEntry[8]);

    m_afEntry[2] *= fInvLength;
    m_afEntry[5] *= fInvLength;
    m_afEntry[8] *= fInvLength;
}


void EigenDecomposition (moMatrix3& rkRot, moMatrix3& rkDiag) const
{
    // Factor M = R*D*R^T.  The columns of R are the eigenvectors.  The
    // diagonal entries of D are the corresponding eigenvalues.
    Real afDiag[3], afSubd[2];
    rkRot = *this;
    bool bReflection = rkRot.Tridiagonalize(afDiag,afSubd);
    bool bConverged = rkRot.QLAlgorithm(afDiag,afSubd);
    if (!bConverged) return;

    // (insertion) sort eigenvalues in increasing order, d0 <= d1 <= d2
    int i;
    Real fSave;

    if (afDiag[1] < afDiag[0])
    {
        // swap d0 and d1
        fSave = afDiag[0];
        afDiag[0] = afDiag[1];
        afDiag[1] = fSave;

        // swap V0 and V1
        for (i = 0; i < 3; i++)
        {
            fSave = rkRot[i][0];
            rkRot[i][0] = rkRot[i][1];
            rkRot[i][1] = fSave;
        }
        bReflection = !bReflection;
    }

    if (afDiag[2] < afDiag[1])
    {
        // swap d1 and d2
        fSave = afDiag[1];
        afDiag[1] = afDiag[2];
        afDiag[2] = fSave;

        // swap V1 and V2
        for (i = 0; i < 3; i++)
        {
            fSave = rkRot[i][1];
            rkRot[i][1] = rkRot[i][2];
            rkRot[i][2] = fSave;
        }
        bReflection = !bReflection;
    }

    if (afDiag[1] < afDiag[0])
    {
        // swap d0 and d1
        fSave = afDiag[0];
        afDiag[0] = afDiag[1];
        afDiag[1] = fSave;

        // swap V0 and V1
        for (i = 0; i < 3; i++)
        {
            fSave = rkRot[i][0];
            rkRot[i][0] = rkRot[i][1];
            rkRot[i][1] = fSave;
        }
        bReflection = !bReflection;
    }

    rkDiag.MakeDiagonal(afDiag[0],afDiag[1],afDiag[2]);

    if (bReflection)
    {
        // The orthogonal transformation that diagonalizes M is a reflection.
        // Make the eigenvectors a right--handed system by changing sign on
        // the last column.
        rkRot[0][2] = -rkRot[0][2];
        rkRot[1][2] = -rkRot[1][2];
        rkRot[2][2] = -rkRot[2][2];
    }
}


moMatrix3<Real>& FromEulerAnglesXYZ (Real fXAngle, Real fYAngle,
    Real fZAngle)
{
    Real fCos, fSin;

    fCos = moMath<Real>::Cos(fXAngle);
    fSin = moMath<Real>::Sin(fXAngle);
    moMatrix3 kXMat(
        (Real)1.0,(Real)0.0,(Real)0.0,
        (Real)0.0,fCos,-fSin,
        (Real)0.0,fSin,fCos);

    fCos = moMath<Real>::Cos(fYAngle);
    fSin = moMath<Real>::Sin(fYAngle);
    moMatrix3 kYMat(
        fCos,(Real)0.0,fSin,
        (Real)0.0,(Real)1.0,(Real)0.0,
        -fSin,(Real)0.0,fCos);

    fCos = moMath<Real>::Cos(fZAngle);
    fSin = moMath<Real>::Sin(fZAngle);
    moMatrix3 kZMat(
        fCos,-fSin,(Real)0.0,
        fSin,fCos,(Real)0.0,
        (Real)0.0,(Real)0.0,(Real)1.0);

    *this = kXMat*(kYMat*kZMat);
    return *this;
}


moMatrix3<Real>& FromEulerAnglesXZY (Real fXAngle, Real fZAngle,
    Real fYAngle)
{
    Real fCos, fSin;

    fCos = moMath<Real>::Cos(fXAngle);
    fSin = moMath<Real>::Sin(fXAngle);
    moMatrix3 kXMat(
        (Real)1.0,(Real)0.0,(Real)0.0,
        (Real)0.0,fCos,-fSin,
        (Real)0.0,fSin,fCos);

    fCos = moMath<Real>::Cos(fZAngle);
    fSin = moMath<Real>::Sin(fZAngle);
    moMatrix3 kZMat(
        fCos,-fSin,(Real)0.0,
        fSin,fCos,(Real)0.0,
        (Real)0.0,(Real)0.0,(Real)1.0);

    fCos = moMath<Real>::Cos(fYAngle);
    fSin = moMath<Real>::Sin(fYAngle);
    moMatrix3 kYMat(
        fCos,(Real)0.0,fSin,
        (Real)0.0,(Real)1.0,(Real)0.0,
        -fSin,(Real)0.0,fCos);

    *this = kXMat*(kZMat*kYMat);
    return *this;
}


moMatrix3<Real>& FromEulerAnglesYXZ (Real fYAngle, Real fXAngle,
    Real fZAngle)
{
    Real fCos, fSin;

    fCos = moMath<Real>::Cos(fYAngle);
    fSin = moMath<Real>::Sin(fYAngle);
    moMatrix3 kYMat(
        fCos,(Real)0.0,fSin,
        (Real)0.0,(Real)1.0,(Real)0.0,
        -fSin,(Real)0.0,fCos);

    fCos = moMath<Real>::Cos(fXAngle);
    fSin = moMath<Real>::Sin(fXAngle);
    moMatrix3 kXMat(
        (Real)1.0,(Real)0.0,(Real)0.0,
        (Real)0.0,fCos,-fSin,
        (Real)0.0,fSin,fCos);

    fCos = moMath<Real>::Cos(fZAngle);
    fSin = moMath<Real>::Sin(fZAngle);
    moMatrix3 kZMat(
        fCos,-fSin,(Real)0.0,
        fSin,fCos,(Real)0.0,
        (Real)0.0,(Real)0.0,(Real)1.0);

    *this = kYMat*(kXMat*kZMat);
    return *this;
}


moMatrix3<Real>& FromEulerAnglesYZX (Real fYAngle, Real fZAngle,
    Real fXAngle)
{
    Real fCos, fSin;

    fCos = moMath<Real>::Cos(fYAngle);
    fSin = moMath<Real>::Sin(fYAngle);
    moMatrix3 kYMat(
        fCos,(Real)0.0,fSin,
        (Real)0.0,(Real)1.0,(Real)0.0,
        -fSin,(Real)0.0,fCos);

    fCos = moMath<Real>::Cos(fZAngle);
    fSin = moMath<Real>::Sin(fZAngle);
    moMatrix3 kZMat(
        fCos,-fSin,(Real)0.0,
        fSin,fCos,(Real)0.0,
        (Real)0.0,(Real)0.0,(Real)1.0);

    fCos = moMath<Real>::Cos(fXAngle);
    fSin = moMath<Real>::Sin(fXAngle);
    moMatrix3 kXMat(
        (Real)1.0,(Real)0.0,(Real)0.0,
        (Real)0.0,fCos,-fSin,
        (Real)0.0,fSin,fCos);

    *this = kYMat*(kZMat*kXMat);
    return *this;
}


moMatrix3<Real>& FromEulerAnglesZXY (Real fZAngle, Real fXAngle,
    Real fYAngle)
{
    Real fCos, fSin;

    fCos = moMath<Real>::Cos(fZAngle);
    fSin = moMath<Real>::Sin(fZAngle);
    moMatrix3 kZMat(
        fCos,-fSin,(Real)0.0,
        fSin,fCos,(Real)0.0,
        (Real)0.0,(Real)0.0,(Real)1.0);

    fCos = moMath<Real>::Cos(fXAngle);
    fSin = moMath<Real>::Sin(fXAngle);
    moMatrix3 kXMat(
        (Real)1.0,(Real)0.0,(Real)0.0,
        (Real)0.0,fCos,-fSin,
        (Real)0.0,fSin,fCos);

    fCos = moMath<Real>::Cos(fYAngle);
    fSin = moMath<Real>::Sin(fYAngle);
    moMatrix3 kYMat(
        fCos,(Real)0.0,fSin,
        (Real)0.0,(Real)1.0,(Real)0.0,
        -fSin,(Real)0.0,fCos);

    *this = kZMat*(kXMat*kYMat);
    return *this;
}


moMatrix3<Real>& FromEulerAnglesZYX (Real fZAngle, Real fYAngle,
    Real fXAngle)
{
    Real fCos, fSin;

    fCos = moMath<Real>::Cos(fZAngle);
    fSin = moMath<Real>::Sin(fZAngle);
    moMatrix3 kZMat(
        fCos,-fSin,(Real)0.0,
        fSin,fCos,(Real)0.0,
        (Real)0.0,(Real)0.0,(Real)1.0);

    fCos = moMath<Real>::Cos(fYAngle);
    fSin = moMath<Real>::Sin(fYAngle);
    moMatrix3 kYMat(
        fCos,(Real)0.0,fSin,
        (Real)0.0,(Real)1.0,(Real)0.0,
        -fSin,(Real)0.0,fCos);

    fCos = moMath<Real>::Cos(fXAngle);
    fSin = moMath<Real>::Sin(fXAngle);
    moMatrix3 kXMat(
        (Real)1.0,(Real)0.0,(Real)0.0,
        (Real)0.0,fCos,-fSin,
        (Real)0.0,fSin,fCos);

    *this = kZMat*(kYMat*kXMat);
    return *this;
}


bool ToEulerAnglesXYZ (Real& rfXAngle, Real& rfYAngle,
    Real& rfZAngle) const
{
    // rot =  cy*cz          -cy*sz           sy
    //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
    //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

    if (m_afEntry[2] < (Real)1.0)
    {
        if (m_afEntry[2] > -(Real)1.0)
        {
            rfXAngle = moMath<Real>::ATan2(-m_afEntry[5],m_afEntry[8]);
            rfYAngle = (Real)asin((double)m_afEntry[2]);
            rfZAngle = moMath<Real>::ATan2(-m_afEntry[1],m_afEntry[0]);
            return true;
        }
        else
        {
            // WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
            rfXAngle = -moMath<Real>::ATan2(m_afEntry[3],m_afEntry[4]);
            rfYAngle = -moMath<Real>::HALF_PI;
            rfZAngle = (Real)0.0;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
        rfXAngle = moMath<Real>::ATan2(m_afEntry[3],m_afEntry[4]);
        rfYAngle = moMath<Real>::HALF_PI;
        rfZAngle = (Real)0.0;
        return false;
    }
}


bool ToEulerAnglesXZY (Real& rfXAngle, Real& rfZAngle,
    Real& rfYAngle) const
{
    // rot =  cy*cz          -sz              cz*sy
    //        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
    //       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

    if (m_afEntry[1] < (Real)1.0)
    {
        if (m_afEntry[1] > -(Real)1.0)
        {
            rfXAngle = moMath<Real>::ATan2(m_afEntry[7],m_afEntry[4]);
            rfZAngle = (Real)asin(-(double)m_afEntry[1]);
            rfYAngle = moMath<Real>::ATan2(m_afEntry[2],m_afEntry[0]);
            return true;
        }
        else
        {
            // WARNING.  Not unique.  XA - YA = atan2(r20,r22)
            rfXAngle = moMath<Real>::ATan2(m_afEntry[6],m_afEntry[8]);
            rfZAngle = moMath<Real>::HALF_PI;
            rfYAngle = (Real)0.0;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  XA + YA = atan2(-r20,r22)
        rfXAngle = moMath<Real>::ATan2(-m_afEntry[6],m_afEntry[8]);
        rfZAngle = -moMath<Real>::HALF_PI;
        rfYAngle = (Real)0.0;
        return false;
    }
}


bool ToEulerAnglesYXZ (Real& rfYAngle, Real& rfXAngle,
    Real& rfZAngle) const
{
    // rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
    //        cx*sz           cx*cz          -sx
    //       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

    if (m_afEntry[5] < (Real)1.0)
    {
        if (m_afEntry[5] > -(Real)1.0)
        {
            rfYAngle = moMath<Real>::ATan2(m_afEntry[2],m_afEntry[8]);
            rfXAngle = (Real)asin(-(double)m_afEntry[5]);
            rfZAngle = moMath<Real>::ATan2(m_afEntry[3],m_afEntry[4]);
            return true;
        }
        else
        {
            // WARNING.  Not unique.  YA - ZA = atan2(r01,r00)
            rfYAngle = moMath<Real>::ATan2(m_afEntry[1],m_afEntry[0]);
            rfXAngle = moMath<Real>::HALF_PI;
            rfZAngle = (Real)0.0;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  YA + ZA = atan2(-r01,r00)
        rfYAngle = moMath<Real>::ATan2(-m_afEntry[1],m_afEntry[0]);
        rfXAngle = -moMath<Real>::HALF_PI;
        rfZAngle = (Real)0.0;
        return false;
    }
}


bool ToEulerAnglesYZX (Real& rfYAngle, Real& rfZAngle,
    Real& rfXAngle) const
{
    // rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
    //        sz              cx*cz          -cz*sx
    //       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

    if (m_afEntry[3] < (Real)1.0)
    {
        if (m_afEntry[3] > -(Real)1.0)
        {
            rfYAngle = moMath<Real>::ATan2(-m_afEntry[6],m_afEntry[0]);
            rfZAngle = (Real)asin((double)m_afEntry[3]);
            rfXAngle = moMath<Real>::ATan2(-m_afEntry[5],m_afEntry[4]);
            return true;
        }
        else
        {
            // WARNING.  Not unique.  YA - XA = -atan2(r21,r22);
            rfYAngle = -moMath<Real>::ATan2(m_afEntry[7],m_afEntry[8]);
            rfZAngle = -moMath<Real>::HALF_PI;
            rfXAngle = (Real)0.0;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  YA + XA = atan2(r21,r22)
        rfYAngle = moMath<Real>::ATan2(m_afEntry[7],m_afEntry[8]);
        rfZAngle = moMath<Real>::HALF_PI;
        rfXAngle = (Real)0.0;
        return false;
    }
}


bool ToEulerAnglesZXY (Real& rfZAngle, Real& rfXAngle,
    Real& rfYAngle) const
{
    // rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
    //        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
    //       -cx*sy           sx              cx*cy

    if (m_afEntry[7] < (Real)1.0)
    {
        if (m_afEntry[7] > -(Real)1.0)
        {
            rfZAngle = moMath<Real>::ATan2(-m_afEntry[1],m_afEntry[4]);
            rfXAngle = (Real)asin((double)m_afEntry[7]);
            rfYAngle = moMath<Real>::ATan2(-m_afEntry[6],m_afEntry[8]);
            return true;
        }
        else
        {
            // WARNING.  Not unique.  ZA - YA = -atan(r02,r00)
            rfZAngle = -moMath<Real>::ATan2(m_afEntry[2],m_afEntry[0]);
            rfXAngle = -moMath<Real>::HALF_PI;
            rfYAngle = (Real)0.0;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  ZA + YA = atan2(r02,r00)
        rfZAngle = moMath<Real>::ATan2(m_afEntry[2],m_afEntry[0]);
        rfXAngle = moMath<Real>::HALF_PI;
        rfYAngle = (Real)0.0;
        return false;
    }
}


bool ToEulerAnglesZYX (Real& rfZAngle, Real& rfYAngle,
    Real& rfXAngle) const
{
    // rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
    //        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
    //       -sy              cy*sx           cx*cy

    if (m_afEntry[6] < (Real)1.0)
    {
        if (m_afEntry[6] > -(Real)1.0)
        {
            rfZAngle = moMath<Real>::ATan2(m_afEntry[3],m_afEntry[0]);
            rfYAngle = (Real)asin(-(double)m_afEntry[6]);
            rfXAngle = moMath<Real>::ATan2(m_afEntry[7],m_afEntry[8]);
            return true;
        }
        else
        {
            // WARNING.  Not unique.  ZA - XA = -atan2(r01,r02)
            rfZAngle = -moMath<Real>::ATan2(m_afEntry[1],m_afEntry[2]);
            rfYAngle = moMath<Real>::HALF_PI;
            rfXAngle = (Real)0.0;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  ZA + XA = atan2(-r01,-r02)
        rfZAngle = moMath<Real>::ATan2(-m_afEntry[1],-m_afEntry[2]);
        rfYAngle = -moMath<Real>::HALF_PI;
        rfXAngle = (Real)0.0;
        return false;
    }
}


moMatrix3<Real>& Slerp (Real fT, const moMatrix3& rkR0,
    const moMatrix3& rkR1)
{
    moVector3<Real> kAxis;
    Real fAngle;
    moMatrix3 kProd = rkR0.TransposeTimes(rkR1);
    kProd.ToAxisAngle(kAxis,fAngle);
    FromAxisAngle(kAxis,fT*fAngle);
    return *this;
}



void SingularValueDecomposition (moMatrix3& rkL, moMatrix3& rkD,
    moMatrix3& rkRTranspose) const
{
    int iRow, iCol;

    moMatrix3 kA = *this;
    Bidiagonalize(kA,rkL,rkRTranspose);
    rkD.MakeZero();

    const int iMax = 32;
    const Real fEpsilon = (Real)1e-04;
    for (int i = 0; i < iMax; i++)
    {
        Real fTmp, fTmp0, fTmp1;
        Real fSin0, fCos0, fTan0;
        Real fSin1, fCos1, fTan1;

        bool bTest1 = (moMath<Real>::FAbs(kA[0][1]) <=
            fEpsilon*(moMath<Real>::FAbs(kA[0][0]) +
            moMath<Real>::FAbs(kA[1][1])));
        bool bTest2 = (moMath<Real>::FAbs(kA[1][2]) <=
            fEpsilon*(moMath<Real>::FAbs(kA[1][1]) +
            moMath<Real>::FAbs(kA[2][2])));
        if (bTest1)
        {
            if (bTest2)
            {
                rkD[0][0] = kA[0][0];
                rkD[1][1] = kA[1][1];
                rkD[2][2] = kA[2][2];
                break;
            }
            else
            {
                // 2x2 closed form factorization
                fTmp = (kA[1][1]*kA[1][1] - kA[2][2]*kA[2][2] +
                    kA[1][2]*kA[1][2])/(kA[1][2]*kA[2][2]);
                fTan0 = ((Real)0.5)*(fTmp + moMath<Real>::Sqrt(fTmp*fTmp +
                    ((Real)4.0)));
                fCos0 = moMath<Real>::InvSqrt(((Real)1.0)+fTan0*fTan0);
                fSin0 = fTan0*fCos0;

                for (iCol = 0; iCol < 3; iCol++)
                {
                    fTmp0 = rkL[iCol][1];
                    fTmp1 = rkL[iCol][2];
                    rkL[iCol][1] = fCos0*fTmp0-fSin0*fTmp1;
                    rkL[iCol][2] = fSin0*fTmp0+fCos0*fTmp1;
                }

                fTan1 = (kA[1][2]-kA[2][2]*fTan0)/kA[1][1];
                fCos1 = moMath<Real>::InvSqrt(((Real)1.0)+fTan1*fTan1);
                fSin1 = -fTan1*fCos1;

                for (iRow = 0; iRow < 3; iRow++)
                {
                    fTmp0 = rkRTranspose[1][iRow];
                    fTmp1 = rkRTranspose[2][iRow];
                    rkRTranspose[1][iRow] = fCos1*fTmp0-fSin1*fTmp1;
                    rkRTranspose[2][iRow] = fSin1*fTmp0+fCos1*fTmp1;
                }

                rkD[0][0] = kA[0][0];
                rkD[1][1] = fCos0*fCos1*kA[1][1] -
                    fSin1*(fCos0*kA[1][2]-fSin0*kA[2][2]);
                rkD[2][2] = fSin0*fSin1*kA[1][1] +
                    fCos1*(fSin0*kA[1][2]+fCos0*kA[2][2]);
                break;
            }
        }
        else
        {
            if (bTest2)
            {
                // 2x2 closed form factorization
                fTmp = (kA[0][0]*kA[0][0] + kA[1][1]*kA[1][1] -
                    kA[0][1]*kA[0][1])/(kA[0][1]*kA[1][1]);
                fTan0 = ((Real)0.5)*(-fTmp + moMath<Real>::Sqrt(fTmp*fTmp +
                    ((Real)4.0)));
                fCos0 = moMath<Real>::InvSqrt(((Real)1.0)+fTan0*fTan0);
                fSin0 = fTan0*fCos0;

                for (iCol = 0; iCol < 3; iCol++)
                {
                    fTmp0 = rkL[iCol][0];
                    fTmp1 = rkL[iCol][1];
                    rkL[iCol][0] = fCos0*fTmp0-fSin0*fTmp1;
                    rkL[iCol][1] = fSin0*fTmp0+fCos0*fTmp1;
                }

                fTan1 = (kA[0][1]-kA[1][1]*fTan0)/kA[0][0];
                fCos1 = moMath<Real>::InvSqrt(((Real)1.0)+fTan1*fTan1);
                fSin1 = -fTan1*fCos1;

                for (iRow = 0; iRow < 3; iRow++)
                {
                    fTmp0 = rkRTranspose[0][iRow];
                    fTmp1 = rkRTranspose[1][iRow];
                    rkRTranspose[0][iRow] = fCos1*fTmp0-fSin1*fTmp1;
                    rkRTranspose[1][iRow] = fSin1*fTmp0+fCos1*fTmp1;
                }

                rkD[0][0] = fCos0*fCos1*kA[0][0] -
                    fSin1*(fCos0*kA[0][1]-fSin0*kA[1][1]);
                rkD[1][1] = fSin0*fSin1*kA[0][0] +
                    fCos1*(fSin0*kA[0][1]+fCos0*kA[1][1]);
                rkD[2][2] = kA[2][2];
                break;
            }
            else
            {
                GolubKahanStep(kA,rkL,rkRTranspose);
            }
        }
    }

    // Make the diagonal entries positive.
    for (iRow = 0; iRow < 3; iRow++)
    {
        if (rkD[iRow][iRow] < (Real)0.0)
        {
            rkD[iRow][iRow] = -rkD[iRow][iRow];
            for (iCol = 0; iCol < 3; iCol++)
            {
                rkRTranspose[iRow][iCol] = -rkRTranspose[iRow][iCol];
            }
        }
    }
}


void SingularValueComposition (const moMatrix3& rkL,
    const moMatrix3& rkD, const moMatrix3& rkRTranspose)
{
    *this = rkL*(rkD*rkRTranspose);
}


void PolarDecomposition (moMatrix3& rkQ, moMatrix3& rkS)
{
    // Decompose M = L*D*R^T.
    moMatrix3 kL, kD, kRTranspose;
    SingularValueDecomposition(kL,kD,kRTranspose);

    // Compute Q = L*R^T.
    rkQ = kL*kRTranspose;

    // Compute S = R*D*R^T.
    rkS = kRTranspose.TransposeTimes(kD*kRTranspose);

    // Numerical round-off errors can cause S not to be symmetric in the
    // sense that S[i][j] and S[j][i] are slightly different for i != j.
    // Correct this by averaging S and Transpose(S).
    rkS[0][1] = ((Real)0.5)*(rkS[0][1] + rkS[1][0]);
    rkS[1][0] = rkS[0][1];
    rkS[0][2] = ((Real)0.5)*(rkS[0][2] + rkS[2][0]);
    rkS[2][0] = rkS[0][2];
    rkS[1][2] = ((Real)0.5)*(rkS[1][2] + rkS[2][1]);
    rkS[2][1] = rkS[1][2];
}


void QDUDecomposition (moMatrix3& rkQ, moMatrix3& rkD,
    moMatrix3& rkU) const
{
    // Factor M = QR = QDU where Q is orthogonal (rotation), D is diagonal
    // (scaling),  and U is upper triangular with ones on its diagonal
    // (shear).  Algorithm uses Gram-Schmidt orthogonalization (the QR
    // algorithm).
    //
    // If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.  The matrix R has entries
    //
    //   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
    //   r10 = 0      r11 = q1*m1  r12 = q1*m2
    //   r20 = 0      r21 = 0      r22 = q2*m2
    //
    // so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
    // u02 = r02/r00, and u12 = r12/r11.

    // build orthogonal matrix Q
    Real fInvLength = moMath<Real>::InvSqrt(m_afEntry[0]*m_afEntry[0] +
        m_afEntry[3]*m_afEntry[3] + m_afEntry[6]*m_afEntry[6]);
    rkQ[0][0] = m_afEntry[0]*fInvLength;
    rkQ[1][0] = m_afEntry[3]*fInvLength;
    rkQ[2][0] = m_afEntry[6]*fInvLength;

    Real fDot = rkQ[0][0]*m_afEntry[1] + rkQ[1][0]*m_afEntry[4] +
        rkQ[2][0]*m_afEntry[7];
    rkQ[0][1] = m_afEntry[1]-fDot*rkQ[0][0];
    rkQ[1][1] = m_afEntry[4]-fDot*rkQ[1][0];
    rkQ[2][1] = m_afEntry[7]-fDot*rkQ[2][0];
    fInvLength = moMath<Real>::InvSqrt(rkQ[0][1]*rkQ[0][1] +
        rkQ[1][1]*rkQ[1][1] + rkQ[2][1]*rkQ[2][1]);
    rkQ[0][1] *= fInvLength;
    rkQ[1][1] *= fInvLength;
    rkQ[2][1] *= fInvLength;

    fDot = rkQ[0][0]*m_afEntry[2] + rkQ[1][0]*m_afEntry[5] +
        rkQ[2][0]*m_afEntry[8];
    rkQ[0][2] = m_afEntry[2]-fDot*rkQ[0][0];
    rkQ[1][2] = m_afEntry[5]-fDot*rkQ[1][0];
    rkQ[2][2] = m_afEntry[8]-fDot*rkQ[2][0];
    fDot = rkQ[0][1]*m_afEntry[2] + rkQ[1][1]*m_afEntry[5] +
        rkQ[2][1]*m_afEntry[8];
    rkQ[0][2] -= fDot*rkQ[0][1];
    rkQ[1][2] -= fDot*rkQ[1][1];
    rkQ[2][2] -= fDot*rkQ[2][1];
    fInvLength = moMath<Real>::InvSqrt(rkQ[0][2]*rkQ[0][2] +
        rkQ[1][2]*rkQ[1][2] + rkQ[2][2]*rkQ[2][2]);
    rkQ[0][2] *= fInvLength;
    rkQ[1][2] *= fInvLength;
    rkQ[2][2] *= fInvLength;

    // guarantee that orthogonal matrix has determinant 1 (no reflections)
    Real fDet = rkQ[0][0]*rkQ[1][1]*rkQ[2][2] + rkQ[0][1]*rkQ[1][2]*rkQ[2][0]
        +  rkQ[0][2]*rkQ[1][0]*rkQ[2][1] - rkQ[0][2]*rkQ[1][1]*rkQ[2][0]
        -  rkQ[0][1]*rkQ[1][0]*rkQ[2][2] - rkQ[0][0]*rkQ[1][2]*rkQ[2][1];

    if (fDet < (Real)0.0)
    {
        for (int iRow = 0; iRow < 3; iRow++)
        {
            for (int iCol = 0; iCol < 3; iCol++)
            {
                rkQ[iRow][iCol] = -rkQ[iRow][iCol];
            }
        }
    }

    // build "right" matrix R
    moMatrix3 kR;
    kR[0][0] = rkQ[0][0]*m_afEntry[0] + rkQ[1][0]*m_afEntry[3] +
        rkQ[2][0]*m_afEntry[6];
    kR[0][1] = rkQ[0][0]*m_afEntry[1] + rkQ[1][0]*m_afEntry[4] +
        rkQ[2][0]*m_afEntry[7];
    kR[1][1] = rkQ[0][1]*m_afEntry[1] + rkQ[1][1]*m_afEntry[4] +
        rkQ[2][1]*m_afEntry[7];
    kR[0][2] = rkQ[0][0]*m_afEntry[2] + rkQ[1][0]*m_afEntry[5] +
        rkQ[2][0]*m_afEntry[8];
    kR[1][2] = rkQ[0][1]*m_afEntry[2] + rkQ[1][1]*m_afEntry[5] +
        rkQ[2][1]*m_afEntry[8];
    kR[2][2] = rkQ[0][2]*m_afEntry[2] + rkQ[1][2]*m_afEntry[5] +
        rkQ[2][2]*m_afEntry[8];

    // the scaling component
    rkD.MakeDiagonal(kR[0][0],kR[1][1],kR[2][2]);

    // the shear component
    Real fInvD0 = ((Real)1.0)/rkD[0][0];
    rkU[0][0] = (Real)1.0;
    rkU[0][1] = kR[0][1]*fInvD0;
    rkU[0][2] = kR[0][2]*fInvD0;
    rkU[1][0] = (Real)0.0;
    rkU[1][1] = (Real)1.0;
    rkU[1][2] = kR[1][2]/rkD[1][1];
    rkU[2][0] = (Real)0.0;
    rkU[2][1] = (Real)0.0;
    rkU[2][2] = (Real)1.0;
}


    inline operator const Real* () const { return m_afEntry; }
    inline operator Real* () { return m_afEntry; }
    inline const Real* operator[] (int iRow) const { return &m_afEntry[3*iRow]; }
    inline Real* operator[] (int iRow) { return &m_afEntry[3*iRow]; }
    inline Real operator() (int iRow, int iCol) const { return m_afEntry[iCol+3*iRow]; }
    inline Real& operator() (int iRow, int iCol) { return m_afEntry[iCol+3*iRow]; }


    // assignment
    inline moMatrix3& operator= (const moMatrix3& rkM)
    {
        m_afEntry[0] = rkM.m_afEntry[0];
        m_afEntry[1] = rkM.m_afEntry[1];
        m_afEntry[2] = rkM.m_afEntry[2];
        m_afEntry[3] = rkM.m_afEntry[3];
        m_afEntry[4] = rkM.m_afEntry[4];
        m_afEntry[5] = rkM.m_afEntry[5];
        m_afEntry[6] = rkM.m_afEntry[6];
        m_afEntry[7] = rkM.m_afEntry[7];
        m_afEntry[8] = rkM.m_afEntry[8];
        return *this;
    }

    // comparison

    // arithmetic operations
    inline moMatrix3 operator+ (const moMatrix3& rkM) const
    {
        return moMatrix3<Real>(
            m_afEntry[0] + rkM.m_afEntry[0],
            m_afEntry[1] + rkM.m_afEntry[1],
            m_afEntry[2] + rkM.m_afEntry[2],
            m_afEntry[3] + rkM.m_afEntry[3],
            m_afEntry[4] + rkM.m_afEntry[4],
            m_afEntry[5] + rkM.m_afEntry[5],
            m_afEntry[6] + rkM.m_afEntry[6],
            m_afEntry[7] + rkM.m_afEntry[7],
            m_afEntry[8] + rkM.m_afEntry[8]);
    }
    inline moMatrix3 operator- (const moMatrix3& rkM) const
    {
        return moMatrix3<Real>(
            m_afEntry[0] - rkM.m_afEntry[0],
            m_afEntry[1] - rkM.m_afEntry[1],
            m_afEntry[2] - rkM.m_afEntry[2],
            m_afEntry[3] - rkM.m_afEntry[3],
            m_afEntry[4] - rkM.m_afEntry[4],
            m_afEntry[5] - rkM.m_afEntry[5],
            m_afEntry[6] - rkM.m_afEntry[6],
            m_afEntry[7] - rkM.m_afEntry[7],
            m_afEntry[8] - rkM.m_afEntry[8]);
    }
    inline moMatrix3 operator* (const moMatrix3& rkM) const
    {
        return moMatrix3<Real>(
            m_afEntry[0]*rkM.m_afEntry[0] +
            m_afEntry[1]*rkM.m_afEntry[3] +
            m_afEntry[2]*rkM.m_afEntry[6],

            m_afEntry[0]*rkM.m_afEntry[1] +
            m_afEntry[1]*rkM.m_afEntry[4] +
            m_afEntry[2]*rkM.m_afEntry[7],

            m_afEntry[0]*rkM.m_afEntry[2] +
            m_afEntry[1]*rkM.m_afEntry[5] +
            m_afEntry[2]*rkM.m_afEntry[8],

            m_afEntry[3]*rkM.m_afEntry[0] +
            m_afEntry[4]*rkM.m_afEntry[3] +
            m_afEntry[5]*rkM.m_afEntry[6],

            m_afEntry[3]*rkM.m_afEntry[1] +
            m_afEntry[4]*rkM.m_afEntry[4] +
            m_afEntry[5]*rkM.m_afEntry[7],

            m_afEntry[3]*rkM.m_afEntry[2] +
            m_afEntry[4]*rkM.m_afEntry[5] +
            m_afEntry[5]*rkM.m_afEntry[8],

            m_afEntry[6]*rkM.m_afEntry[0] +
            m_afEntry[7]*rkM.m_afEntry[3] +
            m_afEntry[8]*rkM.m_afEntry[6],

            m_afEntry[6]*rkM.m_afEntry[1] +
            m_afEntry[7]*rkM.m_afEntry[4] +
            m_afEntry[8]*rkM.m_afEntry[7],

            m_afEntry[6]*rkM.m_afEntry[2] +
            m_afEntry[7]*rkM.m_afEntry[5] +
            m_afEntry[8]*rkM.m_afEntry[8]);
    }
    inline moMatrix3 operator* (Real fScalar) const
    {
        return moMatrix3<Real>(
            fScalar*m_afEntry[0],
            fScalar*m_afEntry[1],
            fScalar*m_afEntry[2],
            fScalar*m_afEntry[3],
            fScalar*m_afEntry[4],
            fScalar*m_afEntry[5],
            fScalar*m_afEntry[6],
            fScalar*m_afEntry[7],
            fScalar*m_afEntry[8]);
    }
    inline moMatrix3 operator/ (Real fScalar) const
    {
        if (fScalar != (Real)0.0)
        {
            Real fInvScalar = ((Real)1.0)/fScalar;
            return moMatrix3<Real>(
                fInvScalar*m_afEntry[0],
                fInvScalar*m_afEntry[1],
                fInvScalar*m_afEntry[2],
                fInvScalar*m_afEntry[3],
                fInvScalar*m_afEntry[4],
                fInvScalar*m_afEntry[5],
                fInvScalar*m_afEntry[6],
                fInvScalar*m_afEntry[7],
                fInvScalar*m_afEntry[8]);
        }

        return moMatrix3<Real>(
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL,
            moMath<Real>::MAX_REAL);
    }
    inline moMatrix3 operator- () const
    {
        return moMatrix3<Real>(
            -m_afEntry[0],
            -m_afEntry[1],
            -m_afEntry[2],
            -m_afEntry[3],
            -m_afEntry[4],
            -m_afEntry[5],
            -m_afEntry[6],
            -m_afEntry[7],
            -m_afEntry[8]);
    }

    // arithmetic updates
    inline moMatrix3& operator+= (const moMatrix3& rkM)
    {
        m_afEntry[0] += rkM.m_afEntry[0];
        m_afEntry[1] += rkM.m_afEntry[1];
        m_afEntry[2] += rkM.m_afEntry[2];
		m_afEntry[3] += rkM.m_afEntry[3];
		m_afEntry[4] += rkM.m_afEntry[4];
		m_afEntry[5] += rkM.m_afEntry[5];
		m_afEntry[6] += rkM.m_afEntry[6];
		m_afEntry[7] += rkM.m_afEntry[7];
		m_afEntry[8] += rkM.m_afEntry[8];
		return *this;
	}
    inline moMatrix3& operator-= (const moMatrix3& rkM)
	{
		m_afEntry[0] -= rkM.m_afEntry[0];
		m_afEntry[1] -= rkM.m_afEntry[1];
		m_afEntry[2] -= rkM.m_afEntry[2];
		m_afEntry[3] -= rkM.m_afEntry[3];
		m_afEntry[4] -= rkM.m_afEntry[4];
		m_afEntry[5] -= rkM.m_afEntry[5];
		m_afEntry[6] -= rkM.m_afEntry[6];
		m_afEntry[7] -= rkM.m_afEntry[7];
		m_afEntry[8] -= rkM.m_afEntry[8];
		return *this;
	}
    inline moMatrix3& operator*= (Real fScalar)
	{
		m_afEntry[0] *= fScalar;
		m_afEntry[1] *= fScalar;
		m_afEntry[2] *= fScalar;
		m_afEntry[3] *= fScalar;
		m_afEntry[4] *= fScalar;
		m_afEntry[5] *= fScalar;
		m_afEntry[6] *= fScalar;
		m_afEntry[7] *= fScalar;
		m_afEntry[8] *= fScalar;
		return *this;
	}
    inline moMatrix3& operator/= (Real fScalar)
	{
		if (fScalar != (Real)0.0)
		{
			Real fInvScalar = ((Real)1.0)/fScalar;
			m_afEntry[0] *= fInvScalar;
			m_afEntry[1] *= fInvScalar;
			m_afEntry[2] *= fInvScalar;
			m_afEntry[3] *= fInvScalar;
			m_afEntry[4] *= fInvScalar;
			m_afEntry[5] *= fInvScalar;
			m_afEntry[6] *= fInvScalar;
			m_afEntry[7] *= fInvScalar;
			m_afEntry[8] *= fInvScalar;
		}
		else
		{
			m_afEntry[0] = moMath<Real>::MAX_REAL;
			m_afEntry[1] = moMath<Real>::MAX_REAL;
			m_afEntry[2] = moMath<Real>::MAX_REAL;
			m_afEntry[3] = moMath<Real>::MAX_REAL;
			m_afEntry[4] = moMath<Real>::MAX_REAL;
			m_afEntry[5] = moMath<Real>::MAX_REAL;
			m_afEntry[6] = moMath<Real>::MAX_REAL;
			m_afEntry[7] = moMath<Real>::MAX_REAL;
			m_afEntry[8] = moMath<Real>::MAX_REAL;
		}

		return *this;
	}

    // matrix times vector
    inline moVector3<Real> operator* (const moVector3<Real>& rkV) const  // M * v
    {
        return moVector3<Real>(
            m_afEntry[0]*rkV[0] + m_afEntry[1]*rkV[1] + m_afEntry[2]*rkV[2],
            m_afEntry[3]*rkV[0] + m_afEntry[4]*rkV[1] + m_afEntry[5]*rkV[2],
            m_afEntry[6]*rkV[0] + m_afEntry[7]*rkV[1] + m_afEntry[8]*rkV[2]);
    }



    // special matrices
    static const moMatrix3 ZERO;
    static const moMatrix3 IDENTITY;

private:
    // Support for eigendecomposition.  The Tridiagonalize function applies
    // a Householder transformation to the matrix.  If that transformation
    // is the identity (the matrix is already tridiagonal), then the return
    // value is 'false'.  Otherwise, the transformation is a reflection and
    // the return value is 'true'.  The QLAlgorithm returns 'true' iff the
    // QL iteration scheme converged.
bool Tridiagonalize (Real afDiag[3], Real afSubd[2])
{
    // Householder reduction T = Q^t M Q
    //   Input:
    //     mat, symmetric 3x3 matrix M
    //   Output:
    //     mat, orthogonal matrix Q (a reflection)
    //     diag, diagonal entries of T
    //     subd, subdiagonal entries of T (T is symmetric)

    Real fM00 = m_afEntry[0];
    Real fM01 = m_afEntry[1];
    Real fM02 = m_afEntry[2];
    Real fM11 = m_afEntry[4];
    Real fM12 = m_afEntry[5];
    Real fM22 = m_afEntry[8];

    afDiag[0] = fM00;
    if (moMath<Real>::FAbs(fM02) >= moMath<Real>::ZERO_TOLERANCE)
    {
        afSubd[0] = moMath<Real>::Sqrt(fM01*fM01+fM02*fM02);
        Real fInvLength = ((Real)1.0)/afSubd[0];
        fM01 *= fInvLength;
        fM02 *= fInvLength;
        Real fTmp = ((Real)2.0)*fM01*fM12+fM02*(fM22-fM11);
        afDiag[1] = fM11+fM02*fTmp;
        afDiag[2] = fM22-fM02*fTmp;
        afSubd[1] = fM12-fM01*fTmp;

        m_afEntry[0] = (Real)1.0;
        m_afEntry[1] = (Real)0.0;
        m_afEntry[2] = (Real)0.0;
        m_afEntry[3] = (Real)0.0;
        m_afEntry[4] = fM01;
        m_afEntry[5] = fM02;
        m_afEntry[6] = (Real)0.0;
        m_afEntry[7] = fM02;
        m_afEntry[8] = -fM01;
        return true;
    }
    else
    {
        afDiag[1] = fM11;
        afDiag[2] = fM22;
        afSubd[0] = fM01;
        afSubd[1] = fM12;

        m_afEntry[0] = (Real)1.0;
        m_afEntry[1] = (Real)0.0;
        m_afEntry[2] = (Real)0.0;
        m_afEntry[3] = (Real)0.0;
        m_afEntry[4] = (Real)1.0;
        m_afEntry[5] = (Real)0.0;
        m_afEntry[6] = (Real)0.0;
        m_afEntry[7] = (Real)0.0;
        m_afEntry[8] = (Real)1.0;
        return false;
    }
}


bool QLAlgorithm (Real afDiag[3], Real afSubd[2])
{
    // This is an implementation of the symmetric QR algorithm from the book
    // "Matrix Computations" by Gene H. Golub and Charles F. Van Loan, second
    // edition.  The algorithm is 8.2.3.  The implementation has a slight
    // variation to actually make it a QL algorithm, and it traps the case
    // when either of the subdiagonal terms s0 or s1 is zero and reduces the
    // 2-by-2 subblock directly.

    const int iMax = 32;
    for (int i = 0; i < iMax; i++)
    {
        Real fSum, fDiff, fDiscr, fEValue0, fEValue1, fCos, fSin, fTmp;
        int iRow;

        fSum = moMath<Real>::FAbs(afDiag[0]) + moMath<Real>::FAbs(afDiag[1]);
        if (moMath<Real>::FAbs(afSubd[0]) + fSum == fSum)
        {
            // The matrix is effectively
            //       +-        -+
            //   M = | d0  0  0 |
            //       | 0  d1 s1 |
            //       | 0  s1 d2 |
            //       +-        -+

            // Compute the eigenvalues as roots of a quadratic equation.
            fSum = afDiag[1] + afDiag[2];
            fDiff = afDiag[1] - afDiag[2];
            fDiscr = moMath<Real>::Sqrt(fDiff*fDiff +
                ((Real)4.0)*afSubd[1]*afSubd[1]);
            fEValue0 = ((Real)0.5)*(fSum - fDiscr);
            fEValue1 = ((Real)0.5)*(fSum + fDiscr);

            // Compute the Givens rotation.
            if (fDiff >= (Real)0.0)
            {
                fCos = afSubd[1];
                fSin = afDiag[1] - fEValue0;
            }
            else
            {
                fCos = afDiag[2] - fEValue0;
                fSin = afSubd[1];
            }
            fTmp = moMath<Real>::InvSqrt(fCos*fCos + fSin*fSin);
            fCos *= fTmp;
            fSin *= fTmp;

            // Postmultiply the current orthogonal matrix with the Givens
            // rotation.
            for (iRow = 0; iRow < 3; iRow++)
            {
                fTmp = m_afEntry[2+3*iRow];
                m_afEntry[2+3*iRow] = fSin*m_afEntry[1+3*iRow] + fCos*fTmp;
                m_afEntry[1+3*iRow] = fCos*m_afEntry[1+3*iRow] - fSin*fTmp;
            }

            // Update the tridiagonal matrix.
            afDiag[1] = fEValue0;
            afDiag[2] = fEValue1;
            afSubd[0] = (Real)0.0;
            afSubd[1] = (Real)0.0;
            return true;
        }

        fSum = moMath<Real>::FAbs(afDiag[1]) + moMath<Real>::FAbs(afDiag[2]);
        if (moMath<Real>::FAbs(afSubd[1]) + fSum == fSum)
        {
            // The matrix is effectively
            //       +-         -+
            //   M = | d0  s0  0 |
            //       | s0  d1  0 |
            //       | 0   0  d2 |
            //       +-         -+

            // Compute the eigenvalues as roots of a quadratic equation.
            fSum = afDiag[0] + afDiag[1];
            fDiff = afDiag[0] - afDiag[1];
            fDiscr = moMath<Real>::Sqrt(fDiff*fDiff +
                ((Real)4.0)*afSubd[0]*afSubd[0]);
            fEValue0 = ((Real)0.5)*(fSum - fDiscr);
            fEValue1 = ((Real)0.5)*(fSum + fDiscr);

            // Compute the Givens rotation.
            if (fDiff >= (Real)0.0)
            {
                fCos = afSubd[0];
                fSin = afDiag[0] - fEValue0;
            }
            else
            {
                fCos = afDiag[1] - fEValue0;
                fSin = afSubd[0];
            }
            fTmp = moMath<Real>::InvSqrt(fCos*fCos + fSin*fSin);
            fCos *= fTmp;
            fSin *= fTmp;

            // Postmultiply the current orthogonal matrix with the Givens
            // rotation.
            for (iRow = 0; iRow < 3; iRow++)
            {
                fTmp = m_afEntry[1+3*iRow];
                m_afEntry[1+3*iRow] = fSin*m_afEntry[0+3*iRow] + fCos*fTmp;
                m_afEntry[0+3*iRow] = fCos*m_afEntry[0+3*iRow] - fSin*fTmp;
            }

            // Update the tridiagonal matrix.
            afDiag[0] = fEValue0;
            afDiag[1] = fEValue1;
            afSubd[0] = (Real)0.0;
            afSubd[1] = (Real)0.0;
            return true;
        }

        // The matrix is
        //       +-        -+
        //   M = | d0 s0  0 |
        //       | s0 d1 s1 |
        //       | 0  s1 d2 |
        //       +-        -+

        // Set up the parameters for the first pass of the QL step.  The
        // value for A is the difference between diagonal term D[2] and the
        // implicit shift suggested by Wilkinson.
        Real fRatio = (afDiag[1]-afDiag[0])/(((Real)2.0f)*afSubd[0]);
        Real fRoot = moMath<Real>::Sqrt((Real)1.0 + fRatio*fRatio);
        Real fB = afSubd[1];
        Real fA = afDiag[2] - afDiag[0];
        if (fRatio >= (Real)0.0)
        {
            fA += afSubd[0]/(fRatio + fRoot);
        }
        else
        {
            fA += afSubd[0]/(fRatio - fRoot);
        }

        // Compute the Givens rotation for the first pass.
        if (moMath<Real>::FAbs(fB) >= moMath<Real>::FAbs(fA))
        {
            fRatio = fA/fB;
            fSin = moMath<Real>::InvSqrt((Real)1.0 + fRatio*fRatio);
            fCos = fRatio*fSin;
        }
        else
        {
            fRatio = fB/fA;
            fCos = moMath<Real>::InvSqrt((Real)1.0 + fRatio*fRatio);
            fSin = fRatio*fCos;
        }

        // Postmultiply the current orthogonal matrix with the Givens
        // rotation.
        for (iRow = 0; iRow < 3; iRow++)
        {
            fTmp = m_afEntry[2+3*iRow];
            m_afEntry[2+3*iRow] = fSin*m_afEntry[1+3*iRow]+fCos*fTmp;
            m_afEntry[1+3*iRow] = fCos*m_afEntry[1+3*iRow]-fSin*fTmp;
        }

        // Set up the parameters for the second pass of the QL step.  The
        // values tmp0 and tmp1 are required to fully update the tridiagonal
        // matrix at the end of the second pass.
        Real fTmp0 = (afDiag[1] - afDiag[2])*fSin +
            ((Real)2.0)*afSubd[1]*fCos;
        Real fTmp1 = fCos*afSubd[0];
        fB = fSin*afSubd[0];
        fA = fCos*fTmp0 - afSubd[1];
        fTmp0 *= fSin;

        // Compute the Givens rotation for the second pass.  The subdiagonal
        // term S[1] in the tridiagonal matrix is updated at this time.
        if (moMath<Real>::FAbs(fB) >= moMath<Real>::FAbs(fA))
        {
            fRatio = fA/fB;
            fRoot = moMath<Real>::Sqrt((Real)1.0 + fRatio*fRatio);
            afSubd[1] = fB*fRoot;
            fSin = ((Real)1.0)/fRoot;
            fCos = fRatio*fSin;
        }
        else
        {
            fRatio = fB/fA;
            fRoot = moMath<Real>::Sqrt((Real)1.0 + fRatio*fRatio);
            afSubd[1] = fA*fRoot;
            fCos = ((Real)1.0)/fRoot;
            fSin = fRatio*fCos;
        }

        // Postmultiply the current orthogonal matrix with the Givens
        // rotation.
        for (iRow = 0; iRow < 3; iRow++)
        {
            fTmp = m_afEntry[1+3*iRow];
            m_afEntry[1+3*iRow] = fSin*m_afEntry[0+3*iRow]+fCos*fTmp;
            m_afEntry[0+3*iRow] = fCos*m_afEntry[0+3*iRow]-fSin*fTmp;
        }

        // Complete the update of the tridiagonal matrix.
        Real fTmp2 = afDiag[1] - fTmp0;
        afDiag[2] += fTmp0;
        fTmp0 = (afDiag[0] - fTmp2)*fSin + ((Real)2.0)*fTmp1*fCos;
        afSubd[0] = fCos*fTmp0 - fTmp1;
        fTmp0 *= fSin;
        afDiag[1] = fTmp2 + fTmp0;
        afDiag[0] -= fTmp0;
    }
    return false;
}
/*
    static void Bidiagonalize (moMatrix3& rkA, moMatrix3& rkL, moMatrix3& rkR);
    static void GolubKahanStep (moMatrix3& rkA, moMatrix3& rkL, moMatrix3& rkR);
    */

static void Bidiagonalize (moMatrix3& rkA, moMatrix3& rkL, moMatrix3& rkR)
{
    Real afV[3], afW[3];
    Real fLength, fSign, fT1, fInvT1, fT2;
    bool bIdentity;

    // map first column to (*,0,0)
    fLength = moMath<Real>::Sqrt(rkA[0][0]*rkA[0][0] + rkA[1][0]*rkA[1][0] +
        rkA[2][0]*rkA[2][0]);
    if (fLength > (Real)0.0)
    {
        fSign = (rkA[0][0] > (Real)0.0 ? (Real)1.0 : -(Real)1.0);
        fT1 = rkA[0][0] + fSign*fLength;
        fInvT1 = ((Real)1.0)/fT1;
        afV[1] = rkA[1][0]*fInvT1;
        afV[2] = rkA[2][0]*fInvT1;

        fT2 = -((Real)2.0)/(((Real)1.0)+afV[1]*afV[1]+afV[2]*afV[2]);
        afW[0] = fT2*(rkA[0][0]+rkA[1][0]*afV[1]+rkA[2][0]*afV[2]);
        afW[1] = fT2*(rkA[0][1]+rkA[1][1]*afV[1]+rkA[2][1]*afV[2]);
        afW[2] = fT2*(rkA[0][2]+rkA[1][2]*afV[1]+rkA[2][2]*afV[2]);
        rkA[0][0] += afW[0];
        rkA[0][1] += afW[1];
        rkA[0][2] += afW[2];
        rkA[1][1] += afV[1]*afW[1];
        rkA[1][2] += afV[1]*afW[2];
        rkA[2][1] += afV[2]*afW[1];
        rkA[2][2] += afV[2]*afW[2];

        rkL[0][0] = ((Real)1.0)+fT2;
        rkL[0][1] = fT2*afV[1];
        rkL[1][0] = rkL[0][1];
        rkL[0][2] = fT2*afV[2];
        rkL[2][0] = rkL[0][2];
        rkL[1][1] = ((Real)1.0)+fT2*afV[1]*afV[1];
        rkL[1][2] = fT2*afV[1]*afV[2];
        rkL[2][1] = rkL[1][2];
        rkL[2][2] = ((Real)1.0)+fT2*afV[2]*afV[2];
        bIdentity = false;
    }
    else
    {
        rkL = moMatrix3::IDENTITY;
        bIdentity = true;
    }

    // map first row to (*,*,0)
    fLength = moMath<Real>::Sqrt(rkA[0][1]*rkA[0][1]+rkA[0][2]*rkA[0][2]);
    if (fLength > (Real)0.0)
    {
        fSign = (rkA[0][1] > (Real)0.0 ? (Real)1.0 : -(Real)1.0);
        fT1 = rkA[0][1] + fSign*fLength;
        afV[2] = rkA[0][2]/fT1;

        fT2 = -((Real)2.0)/(((Real)1.0)+afV[2]*afV[2]);
        afW[0] = fT2*(rkA[0][1]+rkA[0][2]*afV[2]);
        afW[1] = fT2*(rkA[1][1]+rkA[1][2]*afV[2]);
        afW[2] = fT2*(rkA[2][1]+rkA[2][2]*afV[2]);
        rkA[0][1] += afW[0];
        rkA[1][1] += afW[1];
        rkA[1][2] += afW[1]*afV[2];
        rkA[2][1] += afW[2];
        rkA[2][2] += afW[2]*afV[2];

        rkR[0][0] = (Real)1.0;
        rkR[0][1] = (Real)0.0;
        rkR[1][0] = (Real)0.0;
        rkR[0][2] = (Real)0.0;
        rkR[2][0] = (Real)0.0;
        rkR[1][1] = ((Real)1.0)+fT2;
        rkR[1][2] = fT2*afV[2];
        rkR[2][1] = rkR[1][2];
        rkR[2][2] = ((Real)1.0)+fT2*afV[2]*afV[2];
    }
    else
    {
        rkR = moMatrix3::IDENTITY;
    }

    // map second column to (*,*,0)
    fLength = moMath<Real>::Sqrt(rkA[1][1]*rkA[1][1]+rkA[2][1]*rkA[2][1]);
    if (fLength > (Real)0.0)
    {
        fSign = (rkA[1][1] > (Real)0.0 ? (Real)1.0 : -(Real)1.0);
        fT1 = rkA[1][1] + fSign*fLength;
        afV[2] = rkA[2][1]/fT1;

        fT2 = -((Real)2.0)/(((Real)1.0)+afV[2]*afV[2]);
        afW[1] = fT2*(rkA[1][1]+rkA[2][1]*afV[2]);
        afW[2] = fT2*(rkA[1][2]+rkA[2][2]*afV[2]);
        rkA[1][1] += afW[1];
        rkA[1][2] += afW[2];
        rkA[2][2] += afV[2]*afW[2];

        Real fA = ((Real)1.0)+fT2;
        Real fB = fT2*afV[2];
        Real fC = ((Real)1.0)+fB*afV[2];

        if (bIdentity)
        {
            rkL[0][0] = (Real)1.0;
            rkL[0][1] = (Real)0.0;
            rkL[1][0] = (Real)0.0;
            rkL[0][2] = (Real)0.0;
            rkL[2][0] = (Real)0.0;
            rkL[1][1] = fA;
            rkL[1][2] = fB;
            rkL[2][1] = fB;
            rkL[2][2] = fC;
        }
        else
        {
            for (int iRow = 0; iRow < 3; iRow++)
            {
                Real fTmp0 = rkL[iRow][1];
                Real fTmp1 = rkL[iRow][2];
                rkL[iRow][1] = fA*fTmp0+fB*fTmp1;
                rkL[iRow][2] = fB*fTmp0+fC*fTmp1;
            }
        }
    }
}



static void GolubKahanStep (moMatrix3& rkA, moMatrix3& rkL, moMatrix3& rkR)
{
    Real fT11 = rkA[0][1]*rkA[0][1]+rkA[1][1]*rkA[1][1];
    Real fT22 = rkA[1][2]*rkA[1][2]+rkA[2][2]*rkA[2][2];
    Real fT12 = rkA[1][1]*rkA[1][2];
    Real fTrace = fT11+fT22;
    Real fDiff = fT11-fT22;
    Real fDiscr = moMath<Real>::Sqrt(fDiff*fDiff+((Real)4.0)*fT12*fT12);
    Real fRoot1 = ((Real)0.5)*(fTrace+fDiscr);
    Real fRoot2 = ((Real)0.5)*(fTrace-fDiscr);

    // adjust right
    Real fY = rkA[0][0] - (moMath<Real>::FAbs(fRoot1-fT22) <=
        moMath<Real>::FAbs(fRoot2-fT22) ? fRoot1 : fRoot2);
    Real fZ = rkA[0][1];
    Real fInvLength = moMath<Real>::InvSqrt(fY*fY+fZ*fZ);
    Real fSin = fZ*fInvLength;
    Real fCos = -fY*fInvLength;

    Real fTmp0 = rkA[0][0];
    Real fTmp1 = rkA[0][1];
    rkA[0][0] = fCos*fTmp0-fSin*fTmp1;
    rkA[0][1] = fSin*fTmp0+fCos*fTmp1;
    rkA[1][0] = -fSin*rkA[1][1];
    rkA[1][1] *= fCos;

    int iRow;
    for (iRow = 0; iRow < 3; iRow++)
    {
        fTmp0 = rkR[0][iRow];
        fTmp1 = rkR[1][iRow];
        rkR[0][iRow] = fCos*fTmp0-fSin*fTmp1;
        rkR[1][iRow] = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust left
    fY = rkA[0][0];
    fZ = rkA[1][0];
    fInvLength = moMath<Real>::InvSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    rkA[0][0] = fCos*rkA[0][0]-fSin*rkA[1][0];
    fTmp0 = rkA[0][1];
    fTmp1 = rkA[1][1];
    rkA[0][1] = fCos*fTmp0-fSin*fTmp1;
    rkA[1][1] = fSin*fTmp0+fCos*fTmp1;
    rkA[0][2] = -fSin*rkA[1][2];
    rkA[1][2] *= fCos;

    int iCol;
    for (iCol = 0; iCol < 3; iCol++)
    {
        fTmp0 = rkL[iCol][0];
        fTmp1 = rkL[iCol][1];
        rkL[iCol][0] = fCos*fTmp0-fSin*fTmp1;
        rkL[iCol][1] = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust right
    fY = rkA[0][1];
    fZ = rkA[0][2];
    fInvLength = moMath<Real>::InvSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    rkA[0][1] = fCos*rkA[0][1]-fSin*rkA[0][2];
    fTmp0 = rkA[1][1];
    fTmp1 = rkA[1][2];
    rkA[1][1] = fCos*fTmp0-fSin*fTmp1;
    rkA[1][2] = fSin*fTmp0+fCos*fTmp1;
    rkA[2][1] = -fSin*rkA[2][2];
    rkA[2][2] *= fCos;

    for (iRow = 0; iRow < 3; iRow++)
    {
        fTmp0 = rkR[1][iRow];
        fTmp1 = rkR[2][iRow];
        rkR[1][iRow] = fCos*fTmp0-fSin*fTmp1;
        rkR[2][iRow] = fSin*fTmp0+fCos*fTmp1;
    }

    // adjust left
    fY = rkA[1][1];
    fZ = rkA[2][1];
    fInvLength = moMath<Real>::InvSqrt(fY*fY+fZ*fZ);
    fSin = fZ*fInvLength;
    fCos = -fY*fInvLength;

    rkA[1][1] = fCos*rkA[1][1]-fSin*rkA[2][1];
    fTmp0 = rkA[1][2];
    fTmp1 = rkA[2][2];
    rkA[1][2] = fCos*fTmp0-fSin*fTmp1;
    rkA[2][2] = fSin*fTmp0+fCos*fTmp1;

    for (iCol = 0; iCol < 3; iCol++)
    {
        fTmp0 = rkL[iCol][1];
        fTmp1 = rkL[iCol][2];
        rkL[iCol][1] = fCos*fTmp0-fSin*fTmp1;
        rkL[iCol][2] = fSin*fTmp0+fCos*fTmp1;
    }
}

    // support for comparisons
int CompareArrays (const moMatrix3& rkM) const
{
    return memcmp(m_afEntry,rkM.m_afEntry,9*sizeof(Real));
}

    Real m_afEntry[9];
};

// c * M
template <class Real>
inline moMatrix3<Real> operator* (Real fScalar, const moMatrix3<Real>& rkM)
{
    return rkM*fScalar;
}

// v^T * M
template <class Real>
inline moVector3<Real> operator* (const moVector3<Real>& rkV,
    const moMatrix3<Real>& rkM)
{
    return moVector3<Real>(
        rkV[0]*rkM[0][0] + rkV[1]*rkM[1][0] + rkV[2]*rkM[2][0],
        rkV[0]*rkM[0][1] + rkV[1]*rkM[1][1] + rkV[2]*rkM[2][1],
        rkV[0]*rkM[0][2] + rkV[1]*rkM[1][2] + rkV[2]*rkM[2][2]);
}

#ifndef MO_MACOSX
template class LIBMOLDEO_API moMatrix3<MOfloat>;
#endif
typedef moMatrix3<MOfloat> moMatrix3f;

#ifndef MO_MACOSX
template class LIBMOLDEO_API moMatrix3<MOdouble>;
#endif
typedef moMatrix3<MOdouble> moMatrix3d;





















// moMatrix4 class ------------------------------------------------------------
/*
template <class Real>
class moMatrix4 : public moAbstract
{
public:
    // If bZero is true, create the zero matrix.  Otherwise, create the
    // identity matrix.
    moMatrix4 (bool bZero = true);

    // copy constructor
    moMatrix4 (const moMatrix4& rkM);

    // input Mrc is in row r, column c.
    moMatrix4 (Real fM00, Real fM01, Real fM02, Real fM03,
             Real fM10, Real fM11, Real fM12, Real fM13,
             Real fM20, Real fM21, Real fM22, Real fM23,
             Real fM30, Real fM31, Real fM32, Real fM33);

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..15]={m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,
    //                        m23,m30,m31,m32,m33} [row major]
    //   false: entry[0..15]={m00,m10,m20,m30,m01,m11,m21,m31,m02,m12,m22,
    //                        m32,m03,m13,m23,m33} [col major]
    moMatrix4 (const Real afEntry[16], bool bRowMajor);

    void MakeZero ();
    void MakeIdentity ();

	// member access
    inline operator const Real* () const { return m_afEntry; }
    inline operator Real* () { return m_afEntry; }
    inline const Real* operator[] (int iRow) const { return &m_afEntry[4*iRow]; }
    inline Real* operator[] (int iRow) { return &m_afEntry[4*iRow]; }
    inline Real operator() (int iRow, int iCol) const { return m_afEntry[iCol+4*iRow]; }
    inline Real& operator() (int iRow, int iCol) { return m_afEntry[iCol+4*iRow]; }
    void SetRow (int iRow, const moVector4<Real>& rkV);
    moVector4<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const moVector4<Real>& rkV);
    moVector4<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    inline moMatrix4& operator= (const moMatrix4& rkM)
	{
		m_afEntry[ 0] = rkM.m_afEntry[ 0];
		m_afEntry[ 1] = rkM.m_afEntry[ 1];
		m_afEntry[ 2] = rkM.m_afEntry[ 2];
		m_afEntry[ 3] = rkM.m_afEntry[ 3];
		m_afEntry[ 4] = rkM.m_afEntry[ 4];
		m_afEntry[ 5] = rkM.m_afEntry[ 5];
		m_afEntry[ 6] = rkM.m_afEntry[ 6];
		m_afEntry[ 7] = rkM.m_afEntry[ 7];
		m_afEntry[ 8] = rkM.m_afEntry[ 8];
		m_afEntry[ 9] = rkM.m_afEntry[ 9];
		m_afEntry[10] = rkM.m_afEntry[10];
		m_afEntry[11] = rkM.m_afEntry[11];
		m_afEntry[12] = rkM.m_afEntry[12];
		m_afEntry[13] = rkM.m_afEntry[13];
		m_afEntry[14] = rkM.m_afEntry[14];
		m_afEntry[15] = rkM.m_afEntry[15];
		return *this;
	}

    // comparison
    bool operator== (const moMatrix4& rkM) const;
    bool operator!= (const moMatrix4& rkM) const;
    bool operator<  (const moMatrix4& rkM) const;
    bool operator<= (const moMatrix4& rkM) const;
    bool operator>  (const moMatrix4& rkM) const;
    bool operator>= (const moMatrix4& rkM) const;

    // arithmetic operations
    inline moMatrix4 operator+ (const moMatrix4& rkM) const
	{
		return moMatrix4<Real>(
			m_afEntry[ 0] + rkM.m_afEntry[ 0],
			m_afEntry[ 1] + rkM.m_afEntry[ 1],
			m_afEntry[ 2] + rkM.m_afEntry[ 2],
			m_afEntry[ 3] + rkM.m_afEntry[ 3],
			m_afEntry[ 4] + rkM.m_afEntry[ 4],
			m_afEntry[ 5] + rkM.m_afEntry[ 5],
			m_afEntry[ 6] + rkM.m_afEntry[ 6],
			m_afEntry[ 7] + rkM.m_afEntry[ 7],
			m_afEntry[ 8] + rkM.m_afEntry[ 8],
			m_afEntry[ 9] + rkM.m_afEntry[ 9],
			m_afEntry[10] + rkM.m_afEntry[10],
			m_afEntry[11] + rkM.m_afEntry[11],
			m_afEntry[12] + rkM.m_afEntry[12],
			m_afEntry[13] + rkM.m_afEntry[13],
			m_afEntry[14] + rkM.m_afEntry[14],
			m_afEntry[15] + rkM.m_afEntry[15]);
	}
    inline moMatrix4 operator- (const moMatrix4& rkM) const
	{
		return moMatrix4<Real>(
			m_afEntry[ 0] - rkM.m_afEntry[ 0],
			m_afEntry[ 1] - rkM.m_afEntry[ 1],
			m_afEntry[ 2] - rkM.m_afEntry[ 2],
			m_afEntry[ 3] - rkM.m_afEntry[ 3],
			m_afEntry[ 4] - rkM.m_afEntry[ 4],
			m_afEntry[ 5] - rkM.m_afEntry[ 5],
			m_afEntry[ 6] - rkM.m_afEntry[ 6],
			m_afEntry[ 7] - rkM.m_afEntry[ 7],
			m_afEntry[ 8] - rkM.m_afEntry[ 8],
			m_afEntry[ 9] - rkM.m_afEntry[ 9],
			m_afEntry[10] - rkM.m_afEntry[10],
			m_afEntry[11] - rkM.m_afEntry[11],
			m_afEntry[12] - rkM.m_afEntry[12],
			m_afEntry[13] - rkM.m_afEntry[13],
			m_afEntry[14] - rkM.m_afEntry[14],
			m_afEntry[15] - rkM.m_afEntry[15]);
	}
    inline moMatrix4 operator* (const moMatrix4& rkM) const
	{
		return moMatrix4<Real>(
			m_afEntry[ 0]*rkM.m_afEntry[ 0] +
			m_afEntry[ 1]*rkM.m_afEntry[ 4] +
			m_afEntry[ 2]*rkM.m_afEntry[ 8] +
			m_afEntry[ 3]*rkM.m_afEntry[12],

			m_afEntry[ 0]*rkM.m_afEntry[ 1] +
			m_afEntry[ 1]*rkM.m_afEntry[ 5] +
			m_afEntry[ 2]*rkM.m_afEntry[ 9] +
			m_afEntry[ 3]*rkM.m_afEntry[13],

			m_afEntry[ 0]*rkM.m_afEntry[ 2] +
			m_afEntry[ 1]*rkM.m_afEntry[ 6] +
			m_afEntry[ 2]*rkM.m_afEntry[10] +
			m_afEntry[ 3]*rkM.m_afEntry[14],

			m_afEntry[ 0]*rkM.m_afEntry[ 3] +
			m_afEntry[ 1]*rkM.m_afEntry[ 7] +
			m_afEntry[ 2]*rkM.m_afEntry[11] +
			m_afEntry[ 3]*rkM.m_afEntry[15],

			m_afEntry[ 4]*rkM.m_afEntry[ 0] +
			m_afEntry[ 5]*rkM.m_afEntry[ 4] +
			m_afEntry[ 6]*rkM.m_afEntry[ 8] +
			m_afEntry[ 7]*rkM.m_afEntry[12],

			m_afEntry[ 4]*rkM.m_afEntry[ 1] +
			m_afEntry[ 5]*rkM.m_afEntry[ 5] +
			m_afEntry[ 6]*rkM.m_afEntry[ 9] +
			m_afEntry[ 7]*rkM.m_afEntry[13],

			m_afEntry[ 4]*rkM.m_afEntry[ 2] +
			m_afEntry[ 5]*rkM.m_afEntry[ 6] +
			m_afEntry[ 6]*rkM.m_afEntry[10] +
			m_afEntry[ 7]*rkM.m_afEntry[14],

	        m_afEntry[ 4]*rkM.m_afEntry[ 3] +
			m_afEntry[ 5]*rkM.m_afEntry[ 7] +
			m_afEntry[ 6]*rkM.m_afEntry[11] +
			m_afEntry[ 7]*rkM.m_afEntry[15],

			m_afEntry[ 8]*rkM.m_afEntry[ 0] +
			m_afEntry[ 9]*rkM.m_afEntry[ 4] +
			m_afEntry[10]*rkM.m_afEntry[ 8] +
			m_afEntry[11]*rkM.m_afEntry[12],

			m_afEntry[ 8]*rkM.m_afEntry[ 1] +
			m_afEntry[ 9]*rkM.m_afEntry[ 5] +
			m_afEntry[10]*rkM.m_afEntry[ 9] +
			m_afEntry[11]*rkM.m_afEntry[13],

			m_afEntry[ 8]*rkM.m_afEntry[ 2] +
			m_afEntry[ 9]*rkM.m_afEntry[ 6] +
			m_afEntry[10]*rkM.m_afEntry[10] +
			m_afEntry[11]*rkM.m_afEntry[14],

			m_afEntry[ 8]*rkM.m_afEntry[ 3] +
			m_afEntry[ 9]*rkM.m_afEntry[ 7] +
			m_afEntry[10]*rkM.m_afEntry[11] +
			m_afEntry[11]*rkM.m_afEntry[15],

			m_afEntry[12]*rkM.m_afEntry[ 0] +
			m_afEntry[13]*rkM.m_afEntry[ 4] +
			m_afEntry[14]*rkM.m_afEntry[ 8] +
			m_afEntry[15]*rkM.m_afEntry[12],

			m_afEntry[12]*rkM.m_afEntry[ 1] +
			m_afEntry[13]*rkM.m_afEntry[ 5] +
			m_afEntry[14]*rkM.m_afEntry[ 9] +
			m_afEntry[15]*rkM.m_afEntry[13],

			m_afEntry[12]*rkM.m_afEntry[ 2] +
			m_afEntry[13]*rkM.m_afEntry[ 6] +
			m_afEntry[14]*rkM.m_afEntry[10] +
			m_afEntry[15]*rkM.m_afEntry[14],

			m_afEntry[12]*rkM.m_afEntry[ 3] +
	        m_afEntry[13]*rkM.m_afEntry[ 7] +
		    m_afEntry[14]*rkM.m_afEntry[11] +
			m_afEntry[15]*rkM.m_afEntry[15]);
	}
    inline moMatrix4 operator* (Real fScalar) const
	{
		return moMatrix4<Real>(
			fScalar*m_afEntry[ 0],
			fScalar*m_afEntry[ 1],
			fScalar*m_afEntry[ 2],
			fScalar*m_afEntry[ 3],
			fScalar*m_afEntry[ 4],
			fScalar*m_afEntry[ 5],
			fScalar*m_afEntry[ 6],
			fScalar*m_afEntry[ 7],
			fScalar*m_afEntry[ 8],
			fScalar*m_afEntry[ 9],
			fScalar*m_afEntry[10],
			fScalar*m_afEntry[11],
			fScalar*m_afEntry[12],
			fScalar*m_afEntry[13],
			fScalar*m_afEntry[14],
			fScalar*m_afEntry[15]);
	}
    inline moMatrix4 operator/ (Real fScalar) const
	{
		if (fScalar != (Real)0.0)
		{
			Real fInvScalar = ((Real)1.0)/fScalar;
			return moMatrix4<Real>(
				fInvScalar*m_afEntry[ 0],
				fInvScalar*m_afEntry[ 1],
				fInvScalar*m_afEntry[ 2],
				fInvScalar*m_afEntry[ 3],
				fInvScalar*m_afEntry[ 4],
				fInvScalar*m_afEntry[ 5],
				fInvScalar*m_afEntry[ 6],
				fInvScalar*m_afEntry[ 7],
				fInvScalar*m_afEntry[ 8],
				fInvScalar*m_afEntry[ 9],
				fInvScalar*m_afEntry[10],
				fInvScalar*m_afEntry[11],
				fInvScalar*m_afEntry[12],
				fInvScalar*m_afEntry[13],
				fInvScalar*m_afEntry[14],
				fInvScalar*m_afEntry[15]);
		}

		return moMatrix4<Real>(
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL,
	        moMath<Real>::MAX_REAL,
		    moMath<Real>::MAX_REAL,
			moMath<Real>::MAX_REAL);
	}
    inline moMatrix4 operator- () const
	{
		return moMatrix4<Real>(
			-m_afEntry[ 0],
			-m_afEntry[ 1],
			-m_afEntry[ 2],
			-m_afEntry[ 3],
			-m_afEntry[ 4],
			-m_afEntry[ 5],
	        -m_afEntry[ 6],
		    -m_afEntry[ 7],
			-m_afEntry[ 8],
			-m_afEntry[ 9],
			-m_afEntry[10],
			-m_afEntry[11],
			-m_afEntry[12],
			-m_afEntry[13],
			-m_afEntry[14],
			-m_afEntry[15]);
	}

    // arithmetic updates
    inline moMatrix4& operator+= (const moMatrix4& rkM)
	{
		m_afEntry[ 0] += rkM.m_afEntry[ 0];
		m_afEntry[ 1] += rkM.m_afEntry[ 1];
		m_afEntry[ 2] += rkM.m_afEntry[ 2];
		m_afEntry[ 3] += rkM.m_afEntry[ 3];
		m_afEntry[ 4] += rkM.m_afEntry[ 4];
		m_afEntry[ 5] += rkM.m_afEntry[ 5];
		m_afEntry[ 6] += rkM.m_afEntry[ 6];
		m_afEntry[ 7] += rkM.m_afEntry[ 7];
		m_afEntry[ 8] += rkM.m_afEntry[ 8];
		m_afEntry[ 9] += rkM.m_afEntry[ 9];
		m_afEntry[10] += rkM.m_afEntry[10];
		m_afEntry[11] += rkM.m_afEntry[11];
		m_afEntry[12] += rkM.m_afEntry[12];
		m_afEntry[13] += rkM.m_afEntry[13];
		m_afEntry[14] += rkM.m_afEntry[14];
		m_afEntry[15] += rkM.m_afEntry[15];
		return *this;
	}
    inline moMatrix4& operator-= (const moMatrix4& rkM)
	{
		m_afEntry[ 0] -= rkM.m_afEntry[ 0];
		m_afEntry[ 1] -= rkM.m_afEntry[ 1];
		m_afEntry[ 2] -= rkM.m_afEntry[ 2];
		m_afEntry[ 3] -= rkM.m_afEntry[ 3];
		m_afEntry[ 4] -= rkM.m_afEntry[ 4];
		m_afEntry[ 5] -= rkM.m_afEntry[ 5];
		m_afEntry[ 6] -= rkM.m_afEntry[ 6];
		m_afEntry[ 7] -= rkM.m_afEntry[ 7];
		m_afEntry[ 8] -= rkM.m_afEntry[ 8];
		m_afEntry[ 9] -= rkM.m_afEntry[ 9];
		m_afEntry[10] -= rkM.m_afEntry[10];
		m_afEntry[11] -= rkM.m_afEntry[11];
		m_afEntry[12] -= rkM.m_afEntry[12];
		m_afEntry[13] -= rkM.m_afEntry[13];
		m_afEntry[14] -= rkM.m_afEntry[14];
		m_afEntry[15] -= rkM.m_afEntry[15];
		return *this;
	}
    inline moMatrix4& operator*= (Real fScalar)
	{
		m_afEntry[ 0] *= fScalar;
		m_afEntry[ 1] *= fScalar;
		m_afEntry[ 2] *= fScalar;
		m_afEntry[ 3] *= fScalar;
		m_afEntry[ 4] *= fScalar;
		m_afEntry[ 5] *= fScalar;
		m_afEntry[ 6] *= fScalar;
		m_afEntry[ 7] *= fScalar;
		m_afEntry[ 8] *= fScalar;
		m_afEntry[ 9] *= fScalar;
		m_afEntry[10] *= fScalar;
		m_afEntry[11] *= fScalar;
		m_afEntry[12] *= fScalar;
		m_afEntry[13] *= fScalar;
		m_afEntry[14] *= fScalar;
		m_afEntry[15] *= fScalar;
		return *this;
	}
    inline moMatrix4& operator/= (Real fScalar)
	{
	    if (fScalar != (Real)0.0)
		{
			Real fInvScalar = ((Real)1.0)/fScalar;
			m_afEntry[ 0] *= fInvScalar;
			m_afEntry[ 1] *= fInvScalar;
			m_afEntry[ 2] *= fInvScalar;
			m_afEntry[ 3] *= fInvScalar;
			m_afEntry[ 4] *= fInvScalar;
			m_afEntry[ 5] *= fInvScalar;
			m_afEntry[ 6] *= fInvScalar;
			m_afEntry[ 7] *= fInvScalar;
			m_afEntry[ 8] *= fInvScalar;
			m_afEntry[ 9] *= fInvScalar;
			m_afEntry[10] *= fInvScalar;
			m_afEntry[11] *= fInvScalar;
			m_afEntry[12] *= fInvScalar;
			m_afEntry[13] *= fInvScalar;
			m_afEntry[14] *= fInvScalar;
			m_afEntry[15] *= fInvScalar;
		}
		else
		{
			m_afEntry[ 0] = moMath<Real>::MAX_REAL;
			m_afEntry[ 1] = moMath<Real>::MAX_REAL;
			m_afEntry[ 2] = moMath<Real>::MAX_REAL;
			m_afEntry[ 3] = moMath<Real>::MAX_REAL;
			m_afEntry[ 4] = moMath<Real>::MAX_REAL;
			m_afEntry[ 5] = moMath<Real>::MAX_REAL;
			m_afEntry[ 6] = moMath<Real>::MAX_REAL;
			m_afEntry[ 7] = moMath<Real>::MAX_REAL;
			m_afEntry[ 8] = moMath<Real>::MAX_REAL;
			m_afEntry[ 9] = moMath<Real>::MAX_REAL;
			m_afEntry[10] = moMath<Real>::MAX_REAL;
			m_afEntry[11] = moMath<Real>::MAX_REAL;
			m_afEntry[12] = moMath<Real>::MAX_REAL;
			m_afEntry[13] = moMath<Real>::MAX_REAL;
			m_afEntry[14] = moMath<Real>::MAX_REAL;
			m_afEntry[15] = moMath<Real>::MAX_REAL;
		}

		return *this;
	}

    // matrix times vector
    inline moVector4<Real> operator* (const moVector4<Real>& rkV) const  // M * v
	{
		return moVector4<Real>(
			m_afEntry[ 0]*rkV[0] +
			m_afEntry[ 1]*rkV[1] +
			m_afEntry[ 2]*rkV[2] +
			m_afEntry[ 3]*rkV[3],

			m_afEntry[ 4]*rkV[0] +
			m_afEntry[ 5]*rkV[1] +
			m_afEntry[ 6]*rkV[2] +
			m_afEntry[ 7]*rkV[3],

			m_afEntry[ 8]*rkV[0] +
			m_afEntry[ 9]*rkV[1] +
			m_afEntry[10]*rkV[2] +
			m_afEntry[11]*rkV[3],

			m_afEntry[12]*rkV[0] +
			m_afEntry[13]*rkV[1] +
			m_afEntry[14]*rkV[2] +
			m_afEntry[15]*rkV[3]);
	}

    // other operations
    moMatrix4 Transpose () const;  // M^T
    moMatrix4 TransposeTimes (const moMatrix4& rkM) const;  // this^T * M
    moMatrix4 TimesTranspose (const moMatrix4& rkM) const;  // this * M^T
    moMatrix4 Inverse () const;
    moMatrix4 Adjoint () const;
    Real Determinant () const;
    Real QForm (const moVector4<Real>& rkU,
        const moVector4<Real>& rkV) const;  // u^T*M*v

    // projection matrices onto a specified plane
    void MakeObliqueProjection (const moVector3<Real>& rkNormal,
        const moVector3<Real>& rkPoint, const moVector3<Real>& rkDirection);
    void MakePerspectiveProjection (const moVector3<Real>& rkNormal,
        const moVector3<Real>& rkPoint, const moVector3<Real>& rkEye);

    // reflection matrix through a specified plane
    void MakeReflection (const moVector3<Real>& rkNormal,
        const moVector3<Real>& rkPoint);

    // special matrices
    static const moMatrix4 ZERO;
    static const moMatrix4 IDENTITY;

private:
    // support for comparisons
    int CompareArrays (const moMatrix4& rkM) const;

    Real m_afEntry[16];
};

// c * M
template <class Real>
inline moMatrix4<Real> operator* (Real fScalar, const moMatrix4<Real>& rkM)
{
    return rkM*fScalar;
}

// v^T * M
template <class Real>
inline moVector4<Real> operator* (const moVector4<Real>& rkV,
    const moMatrix4<Real>& rkM)
{
    return moVector4<Real>(
        rkV[0]*rkM[0][0]+rkV[1]*rkM[1][0]+rkV[2]*rkM[2][0]+rkV[3]*rkM[3][0],
        rkV[0]*rkM[0][1]+rkV[1]*rkM[1][1]+rkV[2]*rkM[2][1]+rkV[3]*rkM[3][1],
        rkV[0]*rkM[0][2]+rkV[1]*rkM[1][2]+rkV[2]*rkM[2][2]+rkV[3]*rkM[3][2],
        rkV[0]*rkM[0][3]+rkV[1]*rkM[1][3]+rkV[2]*rkM[2][3]+rkV[3]*rkM[3][3]);
}

#ifndef MO_MACOSX
template class LIBMOLDEO_API moMatrix4<MOfloat>;
#endif

typedef moMatrix4<MOfloat> moMatrix4f;
*/
#endif
