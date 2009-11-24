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
  Andrés Colubri

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

#ifndef __MO_MATH_MATRIX_H_
#define __MO_MATH_MATRIX_H__

#include "moMathVector.h"
#include "moMathVector3.h"
#include "moMathVector4.h"

// moMatrix2 class ------------------------------------------------------------

template <class Real>
class LIBMOLDEO_API moMatrix2 : public moAbstract
{
public:
    // If bZero is true, create the zero matrix.  Otherwise, create the
    // identity matrix.
    moMatrix2 (bool bZero = true);

    // copy constructor
    moMatrix2 (const moMatrix2& rkM);

    // input Mrc is in row r, column c.
    moMatrix2 (Real fM00, Real fM01, Real fM10, Real fM11);

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..3] = {m00,m01,m10,m11}  [row major]
    //   false: entry[0..3] = {m00,m10,m01,m11}  [column major]
    moMatrix2 (const Real afEntry[4], bool bRowMajor);

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    moMatrix2 (const moVector2<Real>& rkU, const moVector2<Real>& rkV,
        bool bColumns);
    moMatrix2 (const moVector2<Real>* akV, bool bColumns);

    // create a diagonal matrix
    moMatrix2 (Real fM00, Real fM11);

    // create a rotation matrix (positive angle - counterclockwise)
    moMatrix2 (Real fAngle);

    // create a tensor product U*V^T
    moMatrix2 (const moVector2<Real>& rkU, const moVector2<Real>& rkV);

    // create various matrices
    void MakeZero ();
    void MakeIdentity ();
    void MakeDiagonal (Real fM00, Real fM11);
    void FromAngle (Real fAngle);
    void MakeTensorProduct (const moVector2<Real>& rkU,
        const moVector2<Real>& rkV);

    // member access
    inline operator const Real* () const { return m_afEntry; }
    inline operator Real* () { return m_afEntry; }
    inline const Real* operator[] (int iRow) const { return &m_afEntry[2*iRow]; }
    inline Real* operator[] (int iRow) { return &m_afEntry[2*iRow]; }
    inline Real operator() (int iRow, int iCol) const { return m_afEntry[iCol + 2*iRow]; }
    inline Real& operator() (int iRow, int iCol) { return m_afEntry[iCol + 2*iRow]; }
    void SetRow (int iRow, const moVector2<Real>& rkV);
    moVector2<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const moVector2<Real>& rkV);
    moVector2<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

    // assignment
    inline moMatrix2& operator= (const moMatrix2& rkM)
    {
        m_afEntry[0] = rkM.m_afEntry[0];
        m_afEntry[1] = rkM.m_afEntry[1];
        m_afEntry[2] = rkM.m_afEntry[2];
        m_afEntry[3] = rkM.m_afEntry[3];
        return *this;
    }

    // comparison
    bool operator== (const moMatrix2& rkM) const;
    bool operator!= (const moMatrix2& rkM) const;
    bool operator<  (const moMatrix2& rkM) const;
    bool operator<= (const moMatrix2& rkM) const;
    bool operator>  (const moMatrix2& rkM) const;
    bool operator>= (const moMatrix2& rkM) const;

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

    // other operations
    moMatrix2 Transpose () const;  // M^T
    moMatrix2 TransposeTimes (const moMatrix2& rkM) const;  // this^T * M
    moMatrix2 TimesTranspose (const moMatrix2& rkM) const;  // this * M^T
    moMatrix2 Inverse () const;
    moMatrix2 Adjoint () const;
    Real Determinant () const;
    Real QForm (const moVector2<Real>& rkU,
        const moVector2<Real>& rkV) const;  // u^T*M*v

    // The matrix must be a rotation for these functions to be valid.  The
    // last function uses Gram-Schmidt orthonormalization applied to the
    // columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAngle (Real& rfAngle) const;
    void Orthonormalize ();

    // The matrix must be symmetric.  Factor M = R * D * R^T where
    // R = [u0|u1] is a rotation matrix with columns u0 and u1 and
    // D = diag(d0,d1) is a diagonal matrix whose diagonal entries are d0 and
    // d1.  The eigenvector u[i] corresponds to eigenvector d[i].  The
    // eigenvalues are ordered as d0 <= d1.
    void EigenDecomposition (moMatrix2& rkRot, moMatrix2& rkDiag) const;

    static const moMatrix2 ZERO;
    static const moMatrix2 IDENTITY;

private:
    // support for comparisons
    int CompareArrays (const moMatrix2& rkM) const;

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

template class LIBMOLDEO_API moMatrix2<MOfloat>;
typedef moMatrix2<MOfloat> moMatrix2f;

template class LIBMOLDEO_API moMatrix2<MOdouble>;
typedef moMatrix2<MOdouble> moMatrix2d;

// moMatrix3 class ------------------------------------------------------------

template <class Real>
class moMatrix3 : public moAbstract
{
public:
    // If bZero is true, create the zero matrix.  Otherwise, create the
    // identity matrix.
    moMatrix3 (bool bZero = true);

    // copy constructor
    moMatrix3 (const moMatrix3& rkM);

    // input Mrc is in row r, column c.
    moMatrix3 (Real fM00, Real fM01, Real fM02,
             Real fM10, Real fM11, Real fM12,
             Real fM20, Real fM21, Real fM22);

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..8]={m00,m01,m02,m10,m11,m12,m20,m21,m22} [row major]
    //   false: entry[0..8]={m00,m10,m20,m01,m11,m21,m02,m12,m22} [col major]
    moMatrix3 (const Real afEntry[9], bool bRowMajor);

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    moMatrix3 (const moVector3<Real>& rkU, const moVector3<Real>& rkV,
        const moVector3<Real>& rkW, bool bColumns);
    moMatrix3 (const moVector3<Real>* akV, bool bColumns);

    // create a diagonal matrix
    moMatrix3 (Real fM00, Real fM11, Real fM22);

    // Create rotation matrices (positive angle - counterclockwise).  The
    // angle must be in radians, not degrees.
    moMatrix3 (const moVector3<Real>& rkAxis, Real fAngle);

    // create a tensor product U*V^T
    moMatrix3 (const moVector3<Real>& rkU, const moVector3<Real>& rkV);

    // create various matrices
    moMatrix3& MakeZero ();
    moMatrix3& MakeIdentity ();
    moMatrix3& MakeDiagonal (Real fM00, Real fM11, Real fM22);
    moMatrix3& FromAxisAngle (const moVector3<Real>& rkAxis, Real fAngle);
    moMatrix3& MakeTensorProduct (const moVector3<Real>& rkU,
        const moVector3<Real>& rkV);

    // member access
    inline operator const Real* () const { return m_afEntry; }
    inline operator Real* () { return m_afEntry; }
    inline const Real* operator[] (int iRow) const { return &m_afEntry[3*iRow]; }
    inline Real* operator[] (int iRow) { return &m_afEntry[3*iRow]; }
    inline Real operator() (int iRow, int iCol) const { return m_afEntry[iCol+3*iRow]; }
    inline Real& operator() (int iRow, int iCol) { return m_afEntry[iCol+3*iRow]; }
	void SetRow (int iRow, const moVector3<Real>& rkV);
    moVector3<Real> GetRow (int iRow) const;
    void SetColumn (int iCol, const moVector3<Real>& rkV);
    moVector3<Real> GetColumn (int iCol) const;
    void GetColumnMajor (Real* afCMajor) const;

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
    bool operator== (const moMatrix3& rkM) const;
    bool operator!= (const moMatrix3& rkM) const;
    bool operator<  (const moMatrix3& rkM) const;
    bool operator<= (const moMatrix3& rkM) const;
    bool operator>  (const moMatrix3& rkM) const;
    bool operator>= (const moMatrix3& rkM) const;

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

    // other operations
    moMatrix3 Transpose () const;  // M^T
    moMatrix3 TransposeTimes (const moMatrix3& rkM) const;  // this^T * M
    moMatrix3 TimesTranspose (const moMatrix3& rkM) const;  // this * M^T
    moMatrix3 Inverse () const;
    moMatrix3 Adjoint () const;
    Real Determinant () const;
    Real QForm (const moVector3<Real>& rkU,
        const moVector3<Real>& rkV) const;  // u^T*M*v
    moMatrix3 TimesDiagonal (const moVector3<Real>& rkDiag) const;  // M*D
    moMatrix3 DiagonalTimes (const moVector3<Real>& rkDiag) const;  // D*M

    // The matrix must be a rotation for these functions to be valid.  The
    // last function uses Gram-Schmidt orthonormalization applied to the
    // columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAxisAngle (moVector3<Real>& rkAxis, Real& rfAngle) const;
    void Orthonormalize ();

    // The matrix must be symmetric.  Factor M = R * D * R^T where
    // R = [u0|u1|u2] is a rotation matrix with columns u0, u1, and u2 and
    // D = diag(d0,d1,d2) is a diagonal matrix whose diagonal entries are d0,
    // d1, and d2.  The eigenvector u[i] corresponds to eigenvector d[i].
    // The eigenvalues are ordered as d0 <= d1 <= d2.
    void EigenDecomposition (moMatrix3& rkRot, moMatrix3& rkDiag) const;

    // Create rotation matrices from Euler angles.
    moMatrix3& FromEulerAnglesXYZ (Real fXAngle, Real fYAngle, Real fZAngle);
    moMatrix3& FromEulerAnglesXZY (Real fXAngle, Real fZAngle, Real fYAngle);
    moMatrix3& FromEulerAnglesYXZ (Real fYAngle, Real fXAngle, Real fZAngle);
    moMatrix3& FromEulerAnglesYZX (Real fYAngle, Real fZAngle, Real fXAngle);
    moMatrix3& FromEulerAnglesZXY (Real fZAngle, Real fXAngle, Real fYAngle);
    moMatrix3& FromEulerAnglesZYX (Real fZAngle, Real fYAngle, Real fXAngle);

    // Extract Euler angles from rotation matrices.  The return value is
    // 'true' iff the factorization is unique relative to certain angle
    // ranges.  That is, if (U,V,W) is some permutation of (X,Y,Z), the angle
    // ranges for the outputs from ToEulerAnglesUVW(uAngle,vAngle,wAngle) are
    // uAngle in [-pi,pi], vAngle in [-pi/2,pi/2], and wAngle in [-pi,pi].  If
    // the function returns 'false', wAngle is 0 and vAngle is either pi/2 or
    // -pi/2.
    bool ToEulerAnglesXYZ (Real& rfXAngle, Real& rfYAngle, Real& rfZAngle)
        const;
    bool ToEulerAnglesXZY (Real& rfXAngle, Real& rfZAngle, Real& rfYAngle)
        const;
    bool ToEulerAnglesYXZ (Real& rfYAngle, Real& rfXAngle, Real& rfZAngle)
        const;
    bool ToEulerAnglesYZX (Real& rfYAngle, Real& rfZAngle, Real& rfXAngle)
        const;
    bool ToEulerAnglesZXY (Real& rfZAngle, Real& rfXAngle, Real& rfYAngle)
        const;
    bool ToEulerAnglesZYX (Real& rfZAngle, Real& rfYAngle, Real& rfXAngle)
        const;

    // SLERP (spherical linear interpolation) without quaternions.  Computes
    // R(t) = R0*(Transpose(R0)*R1)^t.  If Q is a rotation matrix with
    // unit-length axis U and angle A, then Q^t is a rotation matrix with
    // unit-length axis U and rotation angle t*A.
    moMatrix3& Slerp (Real fT, const moMatrix3& rkR0, const moMatrix3& rkR1);

    // Singular value decomposition, M = L*D*Transpose(R), where L and R are
    // orthogonal and D is a diagonal matrix whose diagonal entries are
    // nonnegative.
    void SingularValueDecomposition (moMatrix3& rkL, moMatrix3& rkD,
        moMatrix3& rkRTranspose) const;

    // For debugging purposes.  Take the output of SingularValueDecomposition
    // and multiply to see if you get M.
    void SingularValueComposition (const moMatrix3& rkL, const moMatrix3& rkD,
        const moMatrix3& rkRTranspose);

    // Polar decomposition, M = Q*S, where Q is orthogonal and S is symmetric.
    // This uses the singular value decomposition:
    //   M = L*D*Transpose(R) = (L*Transpose(R))*(R*D*Transpose(R)) = Q*S
    // where Q = L*Transpose(R) and S = R*D*Transpose(R).
    void PolarDecomposition (moMatrix3& rkQ, moMatrix3& rkS);

    // Factor M = Q*D*U with orthogonal Q, diagonal D, upper triangular U.
    void QDUDecomposition (moMatrix3& rkQ, moMatrix3& rkD, moMatrix3& rkU) const;

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
    bool Tridiagonalize (Real afDiag[3], Real afSubd[2]);
    bool QLAlgorithm (Real afDiag[3], Real afSubd[2]);

    // support for singular value decomposition
    static void Bidiagonalize (moMatrix3& rkA, moMatrix3& rkL, moMatrix3& rkR);
    static void GolubKahanStep (moMatrix3& rkA, moMatrix3& rkL, moMatrix3& rkR);

    // support for comparisons
    int CompareArrays (const moMatrix3& rkM) const;

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

template class LIBMOLDEO_API moMatrix3<MOfloat>;
typedef moMatrix3<MOfloat> moMatrix3f;

template class LIBMOLDEO_API moMatrix3<MOdouble>;
typedef moMatrix3<MOdouble> moMatrix3d;

// moMatrix4 class ------------------------------------------------------------

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

template class LIBMOLDEO_API moMatrix4<MOfloat>;
typedef moMatrix4<MOfloat> moMatrix4f;

/*
template class LIBMOLDEO_API moMatrix4<MOdouble>;
typedef moMatrix4<MOdouble> moMatrix4d;
*/
#endif
