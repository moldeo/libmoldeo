/*******************************************************************************

							   moMathCurve.h

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
  Andrs Colubri

  Portions taken from
  Wild Magic Source Code
  David Eberly
  http://www.geometrictools.com
  Copyright (c) 1998-2007

*******************************************************************************/

#ifndef __MO_MATH_CURVE_H__
#define __MO_MATH_CURVE_H__

#include "moMathVector.h"
#include "moMathVector3.h"
#include "moMathVector4.h"
#include "moMathNumericalAnalysis.h"
// moCurve2 class ------------------------------------------------------------
template < class Real > class LIBMOLDEO_API moCurve2:public moAbstract
{
	public:
		// abstract base class
		moCurve2 (Real fTMin, Real fTMax)
		{
			m_fTMin = fTMin;
			m_fTMax = fTMax;
		}
		virtual ~ moCurve2 ()
		{
		}

		// Interval on which curve parameter is defined.  If you are interested
		// in only a subinterval of the actual domain of the curve, you may set
		// that subinterval with SetTimeInterval.  This function requires that
		// fTMin < fTMax.
		Real GetMinTime ()const
		{
			return m_fTMin;
		}
		Real GetMaxTime () const
		{
			return m_fTMax;
		}
		void SetTimeInterval (Real fTMin, Real fTMax)
		{
			m_fTMin = fTMin;
			m_fTMax = fTMax;
		}
		// position and derivatives
		virtual moVector2 < Real > GetPosition (Real fTime) const = 0;
		virtual moVector2 < Real > GetFirstDerivative (Real fTime) const = 0;
		virtual moVector2 < Real > GetSecondDerivative (Real fTime) const = 0;
		virtual moVector2 < Real > GetThirdDerivative (Real fTime) const = 0;

		// differential geometric quantities
		virtual Real GetLength (Real fT0, Real fT1) const = 0;
		Real GetSpeed (Real fTime) const
		{
			moVector2 < Real > kVelocity = GetFirstDerivative (fTime);
			Real fSpeed = kVelocity.Length ();
			return fSpeed;
		}
		Real GetTotalLength () const
		{
			return GetLength (m_fTMin, m_fTMax);
		}
		moVector2 < Real > GetTangent (Real fTime) const
		{
			moVector2 < Real > kVelocity = GetFirstDerivative (fTime);
			kVelocity.Normalize ();
			return kVelocity;
		}
		moVector2 < Real > GetNormal (Real fTime) const
		{
			moVector2 < Real > kTangent = GetFirstDerivative (fTime);
			kTangent.Normalize ();
			moVector2 < Real > kNormal = kTangent.Perp ();
			return kNormal;
		}
		void GetFrame (Real fTime, moVector2 < Real > &rkPosition,
			moVector2 < Real > &rkTangent,
			moVector2 < Real > &rkNormal) const
		{
			rkPosition = GetPosition (fTime);
			rkTangent = GetFirstDerivative (fTime);
			rkTangent.Normalize ();
			rkNormal = rkTangent.Perp ();
		} Real GetCurvature (Real fTime) const
		{
			moVector2 < Real > kDer1 = GetFirstDerivative (fTime);
			moVector2 < Real > kDer2 = GetSecondDerivative (fTime);
			Real fSpeedSqr = kDer1.SquaredLength ();
			if (fSpeedSqr >= moMath < Real >::ZERO_TOLERANCE)

			{
				Real fNumer = kDer1.DotPerp (kDer2);
				Real fDenom = moMath < Real >::Pow (fSpeedSqr, (Real) 1.5);
				return fNumer / fDenom;
			}

			else

			{

				// curvature is indeterminate, just return 0
				return (Real) 0.0;
			}
		}
		void SubdivideByTime (int iNumPoints, moVector2 < Real > *&rakPoint) const
		{
			rakPoint = new moVector2 < Real >[iNumPoints];
			Real fDelta = (m_fTMax - m_fTMin) / (iNumPoints - 1);
			for (int i = 0; i < iNumPoints; i++)

			{
				Real fTime = m_fTMin + fDelta * i;
				rakPoint[i] = GetPosition (fTime);
			}
		}
		// inverse mapping of s = Length(t) given by t = Length^{-1}(s)

		virtual Real GetTime (Real fLength, int iIterations =
			32, Real fTolerance = (Real) 1e-06) const = 0;

		// subdivision
		void SubdivideByLength (int iNumPoints, moVector2 < Real > *&rakPoint) const
		{
			rakPoint = new moVector2 < Real >[iNumPoints];
			Real fDelta = GetTotalLength () / (iNumPoints - 1);
			for (int i = 0; i < iNumPoints; i++)

			{
				Real fLength = fDelta * i;
				Real fTime = GetTime (fLength);
				rakPoint[i] = GetPosition (fTime);
			}
		} void SubdivideByVariation (Real fMinVariation, int iMaxLevel,
			int &riNumPoints,
			moVector2 < Real > *&rakPoint) const
		{

			// compute end points of curve
			moVector2 < Real > kPMin = GetPosition (m_fTMin);
			moVector2 < Real > kPMax = GetPosition (m_fTMax);

			// add left end point to list
			PointList * pkList = new PointList (kPMin, 0);
			riNumPoints = 1;

			// binary subdivision, leaf nodes add right end point of subinterval
			SubdivideByVariation (m_fTMin, kPMin, m_fTMax, kPMax, fMinVariation,
				iMaxLevel, riNumPoints, pkList->m_kNext);

			// repackage points in an array
			rakPoint = new moVector2 < Real >[riNumPoints];
			for (int i = 0; i < riNumPoints; i++)

			{
				rakPoint[i] = pkList->m_kPoint;
				PointList * pkSave = pkList;
				pkList = pkList->m_kNext;
				delete pkSave;
			}
		}
		// Subdivision by variation. The pointers pkP0 and pkP1 correspond to the
		// curve points at fT0 and fT1.  If the pointer values are not null, the
		// assumption is that the caller has passed in the curve points.
		// Otherwise, the function computes the curve points.

		virtual Real GetVariation (Real fT0, Real fT1,
			const moVector2 < Real > *pkP0 =
			0, const moVector2 < Real > *pkP1 =
			0)  const = 0;
	protected:
		// curve parameter is t where tmin <= t <= tmax
		Real m_fTMin, m_fTMax;

		// subdivision
		class LIBMOLDEO_API PointList
		{
			public:PointList (const moVector2 < Real > &rkPoint,
					PointList * pkNext)
				{
					m_kPoint = rkPoint;
					m_kNext = pkNext;
				} moVector2 < Real > m_kPoint;
				PointList * m_kNext;
		};
		void SubdivideByVariation (Real fT0, const moVector2 < Real > &rkP0,
			Real fT1, const moVector2 < Real > &rkP1,
			Real fMinVariation, int iLevel,
			int &riNumPoints, PointList * &rpkList) const
		{
			if (iLevel > 0 && GetVariation (fT0, fT1, &rkP0, &rkP1) > fMinVariation)

			{

				// too much variation, subdivide interval
				iLevel--;
				Real fTMid = ((Real) 0.5) * (fT0 + fT1);
				moVector2 < Real > kPMid = GetPosition (fTMid);
				SubdivideByVariation (fT0, rkP0, fTMid, kPMid, fMinVariation,
					iLevel, riNumPoints, rpkList);
				SubdivideByVariation (fTMid, kPMid, fT1, rkP1, fMinVariation,
					iLevel, riNumPoints, rpkList);
			}

			else

			{

				// add right end point, left end point was added by neighbor
				rpkList = new PointList (rkP1, rpkList);
				riNumPoints++;
			}
		}
};

template class LIBMOLDEO_API moCurve2 < MOfloat >;
typedef moCurve2 < MOfloat > moCurve2f;
template class LIBMOLDEO_API moCurve2 < MOdouble >;
typedef moCurve2 < MOdouble > moCurve2d;

// moSingleCurve2 class ------------------------------------------------------------
template < class Real > class LIBMOLDEO_API moSingleCurve2:public moCurve2 <
Real >
{
	public:
		// abstract base class
		moSingleCurve2 (Real fTMin, Real fTMax):moCurve2 < Real > (fTMin, fTMax)
		{
		}
		// length-from-time and time-from-length
		virtual Real GetLength (Real fT0, Real fT1) const
		{

			////assert(m_fTMin <= fT0 && fT0 <= m_fTMax);
			////assert(m_fTMin <= fT1 && fT1 <= m_fTMax);
			////assert(fT0 <= fT1);
			return moIntegrate1 < Real >::RombergIntegral (8, fT0, fT1,
				GetSpeedWithData,
				(void *) this);
		} virtual Real GetTime (Real fLength, int iIterations =
			32, Real fTolerance = (Real) 1e-06) const
		{
			if (fLength <= (Real) 0.0)

			{
				return m_fTMin;
			}
			if (fLength >= GetTotalLength ())

			{
				return m_fTMax;
			}

			// initial guess for Newton's method
			Real fRatio = fLength / GetTotalLength ();
			Real fOmRatio = ((Real) 1.0) - fRatio;
			Real fTime = fOmRatio * m_fTMin + fRatio * m_fTMax;
			for (int i = 0; i < iIterations; i++)

			{
				Real fDifference = GetLength (m_fTMin, fTime) - fLength;
				if (moMath < Real >::FAbs (fDifference) < fTolerance)

				{
					return fTime;
				}
				fTime -= fDifference / GetSpeed (fTime);
			}

			// Newton's method failed.  You might want to increase iterations or
			// tolerance or integration accuracy.  However, in this application it
			// is likely that the time values are oscillating, due to the limited
			// numerical precision of 32-bit floats.  It is safe to use the last
			// computed time.
			return fTime;
		}
	protected:using moCurve2 < Real >::m_fTMin;
	using moCurve2 < Real >::m_fTMax;
	using moCurve2 < Real >::GetTotalLength;
	static Real GetSpeedWithData (Real fTime, void *pvData)
	{
		return ((moCurve2 < Real > *)pvData)->GetSpeed (fTime);
	}
};

template class LIBMOLDEO_API moSingleCurve2 < MOfloat >;
typedef moSingleCurve2 < MOfloat > moSingleCurve2f;
template class LIBMOLDEO_API moSingleCurve2 < MOdouble >;
typedef moSingleCurve2 < MOdouble > moSingleCurve2d;

// moMultipleCurve2 class ------------------------------------------------------------
template < class Real > class LIBMOLDEO_API moMultipleCurve2:public moCurve2 <
Real >
{
	public:
		// Construction and destruction for abstract base class.  moMultipleCurve2
		// accepts responsibility for deleting the input array.
		moMultipleCurve2 (int iSegments,
			Real * afTime):moCurve2 < Real > (afTime[0],
			afTime[iSegments])
		{
			m_iSegments = iSegments;
			m_afTime = afTime;
			m_afLength = 0;
			m_afAccumLength = 0;
		} virtual ~ moMultipleCurve2 ()
		{
			delete[]m_afTime;
			delete[]m_afLength;
			delete[]m_afAccumLength;
		}
		// member access
		int GetSegments () const
		{
			return m_iSegments;
		}
		const Real *GetTimes () const
		{
			return m_afTime;
		}

		// length-from-time and time-from-length
		virtual Real GetLength (Real fT0, Real fT1) const
		{

			//assert(m_fTMin <= fT0 && fT0 <= m_fTMax);
			//assert(m_fTMin <= fT1 && fT1 <= m_fTMax);
			//assert(fT0 <= fT1);
			if (!m_afLength)

			{
				InitializeLength ();
			}
			int iKey0, iKey1;
			Real fDt0, fDt1;
			GetKeyInfo (fT0, iKey0, fDt0);
			GetKeyInfo (fT1, iKey1, fDt1);
			Real fLength;
			if (iKey0 < iKey1)

			{

				// accumulate full-segment lengths
				fLength = (Real) 0.0;
				for (int i = iKey0 + 1; i < iKey1; i++)

				{
					fLength += m_afLength[i];
				}
				// add on partial first segment
				fLength +=
					GetLengthKey (iKey0, fDt0, m_afTime[iKey0 + 1] - m_afTime[iKey0]);

				// add on partial last segment
				fLength += GetLengthKey (iKey1, (Real) 0.0, fDt1);
			}

			else

			{
				fLength = GetLengthKey (iKey0, fDt0, fDt1);
			}
			return fLength;
		}
		virtual Real GetTime (Real fLength, int iIterations =
			32, Real fTolerance = (Real) 1e-06) const
		{
			if (!m_afLength)

			{
				InitializeLength ();
			}
			if (fLength <= (Real) 0.0)

			{
				return m_fTMin;
			}
			if (fLength >= m_afAccumLength[m_iSegments - 1])

			{
				return m_fTMax;
			}
			int iKey;
			for (iKey = 0; iKey < m_iSegments; iKey++)

			{
				if (fLength < m_afAccumLength[iKey])

				{
					break;
				}
			}
			if (iKey >= m_iSegments)

			{
				return m_afTime[m_iSegments];
			}

			// try Newton's method first for rapid convergence
			Real fL0, fL1;
			if (iKey == 0)

			{
				fL0 = fLength;
				fL1 = m_afAccumLength[0];
			}

			else

			{
				fL0 = fLength - m_afAccumLength[iKey - 1];
				fL1 = m_afAccumLength[iKey] - m_afAccumLength[iKey - 1];
			}

			// use Newton's method to invert the arc length integral
			Real fDt1 = m_afTime[iKey + 1] - m_afTime[iKey];
			Real fDt0 = fDt1 * fL0 / fL1;
			for (int i = 0; i < iIterations; i++)

			{
				Real fDifference = GetLengthKey (iKey, (Real) 0.0, fDt0) - fL0;
				if (moMath < Real >::FAbs (fDifference) <= fTolerance)

				{
					return m_afTime[iKey] + fDt0;
				}
				fDt0 -= fDifference / GetSpeedKey (iKey, fDt0);
			}

			// Newton's method failed.  You might want to increase iterations or
			// tolerance or integration accuracy.  However, in this application it
			// is likely that the time values are oscillating, due to the limited
			// numerical precision of 32-bit floats.  It is safe to use the last
			// computed time.
			return m_afTime[iKey] + fDt0;
		}

		// support for subdivision
		virtual Real GetVariation (Real fT0, Real fT1,
			const moVector2 < Real > *pkP0 =
			0, const moVector2 < Real > *pkP1 = 0) const
		{

			//assert(m_fTMin <= fT0 && fT0 <= m_fTMax);
			//assert(m_fTMin <= fT1 && fT1 <= m_fTMax);
			//assert(fT0 <= fT1);

			// construct line segment, A + (t-t0)*B
			moVector2 < Real > kP0, kP1;
			if (!pkP0)

			{
				kP0 = GetPosition (fT0);
				pkP0 = &kP0;
			}
			if (!pkP1)

			{
				kP1 = GetPosition (fT1);
				pkP1 = &kP1;
			}
			Real fInvDT = ((Real) 1.0) / (fT1 - fT0);
			moVector2 < Real > kA, kB = fInvDT * (*pkP1 - *pkP0);
			int iKey0, iKey1;
			Real fDt0, fDt1;
			GetKeyInfo (fT0, iKey0, fDt0);
			GetKeyInfo (fT1, iKey1, fDt1);
			Real fVariation;
			if (iKey0 < iKey1)

			{

				// accumulate full-segment variations
				fVariation = (Real) 0.0;
				for (int i = iKey0 + 1; i < iKey1; i++)

				{
					kA = *pkP0 + (m_afTime[i] - fT0) * kB;
					fVariation +=
						GetVariationKey (i, (Real) 0.0, m_afTime[i + 1] - m_afTime[i],
						kA, kB);
				}
				// add on partial first segment
				kA = *pkP0 + (m_afTime[iKey0] - fT0) * kB;
				fVariation +=
					GetVariationKey (iKey0, fDt0,
					m_afTime[iKey0 + 1] - m_afTime[iKey0], kA, kB);

				// add on partial last segment
				kA = *pkP0 + (m_afTime[iKey1] - fT0) * kB;
				fVariation += GetVariationKey (iKey1, (Real) 0.0, fDt1, kA, kB);
			}

			else

			{
				kA = *pkP0 + (m_afTime[iKey0] - fT0) * kB;
				fVariation = GetVariationKey (iKey0, fDt0, fDt1, kA, kB);
			}
			return fVariation;
		}
	protected:using moCurve2 < Real >::m_fTMin;
	using moCurve2 < Real >::m_fTMax;
	int m_iSegments;
	Real * m_afTime;

	// These quantities are allocated by GetLength when they are needed the
	// first time.  The allocations occur in InitializeLength (called by
	// GetLength), so this member function must be 'const'. In order to
	// allocate the arrays in a 'const' function, they must be declared as
	// 'mutable'.
	mutable Real * m_afLength;
	mutable Real * m_afAccumLength;
	void GetKeyInfo (Real fTime, int &riKey, Real & rfDt) const
	{
		if (fTime <= m_afTime[0])

		{
			riKey = 0;
			rfDt = (Real) 0.0;
		}

		else if (fTime >= m_afTime[m_iSegments])

		{
			riKey = m_iSegments - 1;
			rfDt = m_afTime[m_iSegments] - m_afTime[m_iSegments - 1];
		}

		else

		{
			for (int i = 0; i < m_iSegments; i++)

			{
				if (fTime < m_afTime[i + 1])

				{
					riKey = i;
					rfDt = fTime - m_afTime[i];
					break;
				}
			}
		}
	}
	void InitializeLength () const
	{
		m_afLength = new Real[m_iSegments];
		m_afAccumLength = new Real[m_iSegments];

		// arc lengths of the segments
		int iKey;
		for (iKey = 0; iKey < m_iSegments; iKey++)

		{
			m_afLength[iKey] =
				GetLengthKey (iKey, (Real) 0.0, m_afTime[iKey + 1] - m_afTime[iKey]);
		}

		// accumulative arc length
		m_afAccumLength[0] = m_afLength[0];
		for (iKey = 1; iKey < m_iSegments; iKey++)

		{
			m_afAccumLength[iKey] = m_afAccumLength[iKey - 1] + m_afLength[iKey];
		}
	}
	virtual Real GetSpeedKey (int iKey, Real fTime) const = 0;
	virtual Real GetLengthKey (int iKey, Real fT0, Real fT1) const = 0;
	virtual Real GetVariationKey (int iKey, Real fT0, Real fT1,
		const moVector2 < Real > &rkA,
		const moVector2 < Real > &rkB) const = 0;
	static Real GetSpeedWithData (Real fTime, void *pvData)
	{
		moMultipleCurve2 * pvThis = *(moMultipleCurve2 **) pvData;
		int iKey = *(int *) ((char *) pvData + sizeof (pvThis));
		return pvThis->GetSpeedKey (iKey, fTime);
	}
};

template class LIBMOLDEO_API moMultipleCurve2 < MOfloat >;
typedef moMultipleCurve2 < MOfloat > moMultipleCurve2f;
template class LIBMOLDEO_API moMultipleCurve2 < MOdouble >;
typedef moMultipleCurve2 < MOdouble > moMultipleCurve2d;

// moCurve3 class ------------------------------------------------------------
template < class Real > class LIBMOLDEO_API moCurve3:public moAbstract
{
	public:
		// abstract base class
		moCurve3 (Real fTMin, Real fTMax)
		{
			m_fTMin = fTMin;
			m_fTMax = fTMax;
		} virtual ~ moCurve3 ()
		{
		}
		// Interval on which curve parameter is defined.  If you are interested
		// in only a subinterval of the actual domain of the curve, you may set
		// that subinterval with SetTimeInterval.  This function requires that
		// fTMin < fTMax.
		Real GetMinTime ()const
		{
			return m_fTMin;
		}
		Real GetMaxTime () const
		{
			return m_fTMax;
		}
		void SetTimeInterval (Real fTMin, Real fTMax)
		{

			//assert(fTMin < fTMax);
			m_fTMin = fTMin;
			m_fTMax = fTMax;
		}
		// position and derivatives
		virtual moVector3 < Real > GetPosition (Real fTime) const = 0;
		virtual moVector3 < Real > GetFirstDerivative (Real fTime) const = 0;
		virtual moVector3 < Real > GetSecondDerivative (Real fTime) const = 0;
		virtual moVector3 < Real > GetThirdDerivative (Real fTime) const = 0;
		virtual Real GetTime (Real fLength, int iIterations =
			32, Real fTolerance = (Real) 1e-06) const = 0;
		virtual Real GetVariation (Real fT0, Real fT1,
			const moVector3 < Real > *pkP0,
			const moVector3 < Real > *pkP1) const = 0;

		// differential geometric quantities
		virtual Real GetLength (Real fT0, Real fT1) const = 0;
		Real GetSpeed (Real fTime) const
		{
			moVector3 < Real > kVelocity = GetFirstDerivative (fTime);
			Real fSpeed = kVelocity.Length ();
			return fSpeed;
		}
		Real GetTotalLength () const
		{
			return GetLength (m_fTMin, m_fTMax);
		}
		moVector3 < Real > GetTangent (Real fTime) const
		{
			moVector3 < Real > kVelocity = GetFirstDerivative (fTime);
			kVelocity.Normalize ();
			return kVelocity;
		}
		moVector3 < Real > GetNormal (Real fTime) const
		{
			moVector3 < Real > kVelocity = GetFirstDerivative (fTime);
			moVector3 < Real > kAcceleration = GetSecondDerivative (fTime);
			Real fVDotV = kVelocity.Dot (kVelocity);
			Real fVDotA = kVelocity.Dot (kAcceleration);
			moVector3 < Real > kNormal = fVDotV * kAcceleration - fVDotA * kVelocity;
			kNormal.Normalize ();
			return kNormal;
		}
		moVector3 < Real > GetBinormal (Real fTime) const
		{
			moVector3 < Real > kVelocity = GetFirstDerivative (fTime);
			moVector3 < Real > kAcceleration = GetSecondDerivative (fTime);
			Real fVDotV = kVelocity.Dot (kVelocity);
			Real fVDotA = kVelocity.Dot (kAcceleration);
			moVector3 < Real > kNormal = fVDotV * kAcceleration - fVDotA * kVelocity;
			kNormal.Normalize ();
			kVelocity.Normalize ();
			moVector3 < Real > kBinormal = kVelocity.Cross (kNormal);
			return kBinormal;
		}
		void GetFrame (Real fTime, moVector3 < Real > &rkPosition,
			moVector3 < Real > &rkTangent,
			moVector3 < Real > &rkNormal,
			moVector3 < Real > &rkBinormal) const
		{
			rkPosition = GetPosition (fTime);
			moVector3 < Real > kVelocity = GetFirstDerivative (fTime);
			moVector3 < Real > kAcceleration = GetSecondDerivative (fTime);
			Real fVDotV = kVelocity.Dot (kVelocity);
			Real fVDotA = kVelocity.Dot (kAcceleration);
			rkNormal = fVDotV * kAcceleration - fVDotA * kVelocity;
			rkNormal.Normalize ();
			rkTangent = kVelocity;
			rkTangent.Normalize ();
			rkBinormal = rkTangent.Cross (rkNormal);
		} Real GetCurvature (Real fTime) const
		{
			moVector3 < Real > kVelocity = GetFirstDerivative (fTime);
			Real fSpeedSqr = kVelocity.SquaredLength ();
			if (fSpeedSqr >= moMath < Real >::ZERO_TOLERANCE)

			{
				moVector3 < Real > kAcceleration = GetSecondDerivative (fTime);
				moVector3 < Real > kCross = kVelocity.Cross (kAcceleration);
				Real fNumer = kCross.Length ();
				Real fDenom = moMath < Real >::Pow (fSpeedSqr, (Real) 1.5);
				return fNumer / fDenom;
			}

			else

			{

				// curvature is indeterminate, just return 0
				return (Real) 0.0;
			}
		}
		Real GetTorsion (Real fTime) const
		{
			moVector3 < Real > kVelocity = GetFirstDerivative (fTime);
			moVector3 < Real > kAcceleration = GetSecondDerivative (fTime);
			moVector3 < Real > kCross = kVelocity.Cross (kAcceleration);
			Real fDenom = kCross.SquaredLength ();
			if (fDenom >= moMath < Real >::ZERO_TOLERANCE)

			{
				moVector3 < Real > kJerk = GetThirdDerivative (fTime);
				Real fNumer = kCross.Dot (kJerk);
				return fNumer / fDenom;
			}

			else

			{

				// torsion is indeterminate, just return 0
				return (Real) 0.0;
			}
		}
		void SubdivideByTime (int iNumPoints, moVector3 < Real > *&rakPoint) const
		{

			//assert( iNumPoints >= 2 );
			rakPoint = new moVector3 < Real >[iNumPoints];
			Real fDelta = (m_fTMax - m_fTMin) / (iNumPoints - 1);
			for (int i = 0; i < iNumPoints; i++)

			{
				Real fTime = m_fTMin + fDelta * i;
				rakPoint[i] = GetPosition (fTime);
			}
		} void SubdivideByLength (int iNumPoints,
			moVector3 < Real > *&rakPoint) const
		{

			//assert(iNumPoints >= 2);
			rakPoint = new moVector3 < Real >[iNumPoints];
			Real fDelta = GetTotalLength () / (iNumPoints - 1);
			for (int i = 0; i < iNumPoints; i++)

			{
				Real fLength = fDelta * i;
				Real fTime = GetTime (fLength);
				rakPoint[i] = GetPosition (fTime);
			}
		} protected:
		// curve parameter is t where tmin <= t <= tmax
		Real m_fTMin, m_fTMax;

		// subdivision
		class LIBMOLDEO_API PointList
		{
			public:PointList (const moVector3 < Real > &rkPoint,
					PointList * pkNext)
				{
					m_kPoint = rkPoint;
					m_kNext = pkNext;
				} moVector3 < Real > m_kPoint;
				PointList * m_kNext;
		};
		void SubdivideByVariation (Real fT0, const moVector3 < Real > &rkP0,
			Real fT1, const moVector3 < Real > &rkP1,
			Real fMinVariation, int iLevel,
			int &riNumPoints, PointList * &rpkList) const
		{
			if (iLevel > 0 && GetVariation (fT0, fT1, &rkP0, &rkP1) > fMinVariation)

			{

				// too much variation, subdivide interval
				iLevel--;
				Real fTMid = ((Real) 0.5) * (fT0 + fT1);
				moVector3 < Real > kPMid = GetPosition (fTMid);
				SubdivideByVariation (fT0, rkP0, fTMid, kPMid, fMinVariation,
					iLevel, riNumPoints, rpkList);
				SubdivideByVariation (fTMid, kPMid, fT1, rkP1, fMinVariation,
					iLevel, riNumPoints, rpkList);
			}

			else

			{

				// add right end point, left end point was added by neighbor
				rpkList = new PointList (rkP1, rpkList);
				riNumPoints++;
			}
		}
};

template class LIBMOLDEO_API moCurve3 < MOfloat >;
typedef moCurve3 < MOfloat > moCurve3f;
template class LIBMOLDEO_API moCurve3 < MOdouble >;
typedef moCurve3 < MOdouble > moCurve3d;

// moSingleCurve3 ------------------------------------------------------------
template < class Real > class LIBMOLDEO_API moSingleCurve3:public moCurve3 <
Real >
{
	public:
		// abstract base class
		moSingleCurve3 (Real fTMin, Real fTMax):moCurve3 < Real > (fTMin, fTMax)
		{
		}
		// length-from-time and time-from-length
		virtual Real GetLength (Real fT0, Real fT1) const
		{
			return moIntegrate1 < Real >::RombergIntegral (8, fT0, fT1,
				GetSpeedWithData,
				(void *) this);
		} Real GetTime (Real fLength, int iIterations, Real fTolerance) const
		{
			if (fLength <= (Real) 0.0)

			{
				return m_fTMin;
			}
			if (fLength >= GetTotalLength ())

			{
				return m_fTMax;
			}

			// initial guess for Newton's method
			Real fRatio = fLength / GetTotalLength ();
			Real fOmRatio = (Real) 1.0 - fRatio;
			Real fTime = fOmRatio * m_fTMin + fRatio * m_fTMax;
			for (int i = 0; i < iIterations; i++)

			{
				Real fDifference = GetLength (m_fTMin, fTime) - fLength;
				if (moMath < Real >::FAbs (fDifference) < fTolerance)

				{
					return fTime;
				}
				fTime -= fDifference / GetSpeed (fTime);
			}

			// Newton's method failed.  You might want to increase iterations or
			// tolerance or integration accuracy.  However, in this application it
			// is likely that the time values are oscillating, due to the limited
			// numerical precision of 32-bit floats.  It is safe to use the last
			// computed time.
			return fTime;
		}
	protected:using moCurve3 < Real >::m_fTMin;
	using moCurve3 < Real >::m_fTMax;
	using moCurve3 < Real >::GetTotalLength;
	static Real GetSpeedWithData (Real fTime, void *pvData)
	{
		return ((moCurve3 < Real > *)pvData)->GetSpeed (fTime);
	}
};

template class LIBMOLDEO_API moSingleCurve3 < MOfloat >;
typedef moSingleCurve3 < MOfloat > moSingleCurve3f;
template class LIBMOLDEO_API moSingleCurve3 < MOdouble >;
typedef moSingleCurve3 < MOdouble > moSingleCurve3d;

// moMultipleCurve3 ------------------------------------------------------------
template < class Real > class LIBMOLDEO_API moMultipleCurve3:public moCurve3 <
Real >
{
	public:
		// Construction and destruction for abstract base class.  moMultipleCurve3
		// accepts responsibility for deleting the input array.
		moMultipleCurve3 (int iSegments,
			Real * afTime) :moCurve3 < Real > (afTime[0],
			afTime[iSegments])
		{
			m_iSegments = iSegments;
			m_afTime = afTime;
			m_afLength = 0;
			m_afAccumLength = 0;
		} virtual ~ moMultipleCurve3 ()
		{
			delete[]m_afTime;
			delete[]m_afLength;
			delete[]m_afAccumLength;
		}
		// member access
		int GetSegments () const
		{
			return m_iSegments;
		}
		const Real *GetTimes () const
		{
			return m_afTime;
		}

		// length-from-time and time-from-length
		virtual Real GetLength (Real fT0, Real fT1) const
		{
			if (!m_afLength)

			{
				InitializeLength ();
			}
			int iKey0, iKey1;
			Real fDt0, fDt1;
			GetKeyInfo (fT0, iKey0, fDt0);
			GetKeyInfo (fT1, iKey1, fDt1);
			Real fLength;
			if (iKey0 < iKey1)

			{

				// accumulate full-segment lengths
				fLength = (Real) 0.0;
				for (int i = iKey0 + 1; i < iKey1; i++)
					fLength += m_afLength[i];

				// add on partial first segment
				fLength +=
					GetLengthKey (iKey0, fDt0, m_afTime[iKey0 + 1] - m_afTime[iKey0]);

				// add on partial last segment
				fLength += GetLengthKey (iKey1, (Real) 0.0, fDt1);
			}

			else

			{
				fLength = GetLengthKey (iKey0, fDt0, fDt1);
			}
			return fLength;
		}
		virtual Real GetTime (Real fLength, int iIterations =
			32, Real fTolerance = (Real) 1e-06) const
		{
			if (!m_afLength)

			{
				InitializeLength ();
			}
			if (fLength <= (Real) 0.0)

			{
				return m_fTMin;
			}
			if (fLength >= m_afAccumLength[m_iSegments - 1])

			{
				return m_fTMax;
			}
			int iKey;
			for (iKey = 0; iKey < m_iSegments; iKey++)

			{
				if (fLength < m_afAccumLength[iKey])

				{
					break;
				}
			}
			if (iKey >= m_iSegments)

			{
				return m_afTime[m_iSegments];
			}

			// try Newton's method first for rapid convergence
			Real fL0, fL1;
			if (iKey == 0)

			{
				fL0 = fLength;
				fL1 = m_afAccumLength[0];
			}

			else

			{
				fL0 = fLength - m_afAccumLength[iKey - 1];
				fL1 = m_afAccumLength[iKey] - m_afAccumLength[iKey - 1];
			}

			// use Newton's method to invert the arc length integral
			Real fDt1 = m_afTime[iKey + 1] - m_afTime[iKey];
			Real fDt0 = fDt1 * fL0 / fL1;
			for (int i = 0; i < iIterations; i++)

			{
				Real fDifference = GetLengthKey (iKey, (Real) 0.0, fDt0) - fL0;
				if (moMath < Real >::FAbs (fDifference) <= fTolerance)

				{
					return m_afTime[iKey] + fDt0;
				}
				fDt0 -= fDifference / GetSpeedKey (iKey, fDt0);
			}

			// Newton's method failed.  You might want to increase iterations or
			// tolerance or integration accuracy.  However, in this application it
			// is likely that the time values are oscillating, due to the limited
			// numerical precision of 32-bit floats.  It is safe to use the last
			// computed time.
			return m_afTime[iKey] + fDt0;
		}

		// support for subdivision
		virtual Real GetVariation (Real fT0, Real fT1,
			const moVector3 < Real > *pkP0 =
			0, const moVector3 < Real > *pkP1 = 0) const
		{

			//assert(m_fTMin <= fT0 && fT0 <= m_fTMax);
			//assert(m_fTMin <= fT1 && fT1 <= m_fTMax);
			//assert(fT0 <= fT1);

			// construct line segment, A + (t-t0)*B
			moVector3 < Real > kP0, kP1;
			if (!pkP0)

			{
				kP0 = GetPosition (fT0);
				pkP0 = &kP0;
			}
			if (!pkP1)

			{
				kP1 = GetPosition (fT1);
				pkP1 = &kP1;
			}
			Real fInvDT = ((Real) 1.0) / (fT1 - fT0);
			moVector3 < Real > kA, kB = fInvDT * (*pkP1 - *pkP0);
			int iKey0, iKey1;
			Real fDt0, fDt1;
			GetKeyInfo (fT0, iKey0, fDt0);
			GetKeyInfo (fT1, iKey1, fDt1);
			Real fVariation;
			if (iKey0 < iKey1)

			{

				// accumulate full-segment variations
				fVariation = (Real) 0.0;
				for (int i = iKey0 + 1; i < iKey1; i++)

				{
					kA = *pkP0 + (m_afTime[i] - fT0) * kB;
					fVariation +=
						GetVariationKey (i, (Real) 0.0, m_afTime[i + 1] - m_afTime[i],
						kA, kB);
				}
				// add on partial first segment
				kA = *pkP0 + (m_afTime[iKey0] - fT0) * kB;
				fVariation +=
					GetVariationKey (iKey0, fDt0,
					m_afTime[iKey0 + 1] - m_afTime[iKey0], kA, kB);

				// add on partial last segment
				kA = *pkP0 + (m_afTime[iKey1] - fT0) * kB;
				fVariation += GetVariationKey (iKey1, 0.0f, fDt1, kA, kB);
			}

			else

			{
				kA = *pkP0 + (m_afTime[iKey0] - fT0) * kB;
				fVariation = GetVariationKey (iKey0, fDt0, fDt1, kA, kB);
			}
			return fVariation;
		}
	protected:using moCurve3 < Real >::m_fTMin;
	using moCurve3 < Real >::m_fTMax;
	int m_iSegments;
	Real * m_afTime;

	// These quantities are allocated by GetLength when they are needed the
	// first time.  The allocations occur in InitializeLength (called by
	// GetLength), so this member function must be 'const'. In order to
	// allocate the arrays in a 'const' function, they must be declared as
	// 'mutable'.
	mutable Real * m_afLength;
	mutable Real * m_afAccumLength;
	void GetKeyInfo (Real fTime, int &riKey, Real & rfDt) const
	{
		if (fTime <= m_afTime[0])

		{
			riKey = 0;
			rfDt = (Real) 0.0;
		}

		else if (fTime >= m_afTime[m_iSegments])

		{
			riKey = m_iSegments - 1;
			rfDt = m_afTime[m_iSegments] - m_afTime[m_iSegments - 1];
		}

		else

		{
			for (int i = 0; i < m_iSegments; i++)

			{
				if (fTime < m_afTime[i + 1])

				{
					riKey = i;
					rfDt = fTime - m_afTime[i];
					break;
				}
			}
		}
	}
	void InitializeLength () const
	{
		m_afLength = new Real[m_iSegments];
		m_afAccumLength = new Real[m_iSegments];

		// arc lengths of the segments
		int iKey;
		for (iKey = 0; iKey < m_iSegments; iKey++)

		{
			m_afLength[iKey] =
				GetLengthKey (iKey, (Real) 0.0, m_afTime[iKey + 1] - m_afTime[iKey]);
		}

		// accumulative arc length
		m_afAccumLength[0] = m_afLength[0];
		for (iKey = 1; iKey < m_iSegments; iKey++)

		{
			m_afAccumLength[iKey] = m_afAccumLength[iKey - 1] + m_afLength[iKey];
		}
	}
	virtual Real GetSpeedKey (int iKey, Real fTime) const = 0;
	virtual Real GetLengthKey (int iKey, Real fT0, Real fT1) const = 0;
	virtual Real GetVariationKey (int iKey, Real fT0, Real fT1,
		const moVector3 < Real > &rkA,
		const moVector3 < Real > &rkB) const = 0;
	static Real GetSpeedWithData (Real fTime, void *pvData)
	{
		moMultipleCurve3 * pvThis = *(moMultipleCurve3 **) pvData;
		int iKey = *(int *) ((char *) pvData + sizeof (pvThis));
		return pvThis->GetSpeedKey (iKey, fTime);
	}
};

template class LIBMOLDEO_API moMultipleCurve3 < MOfloat >;
typedef moMultipleCurve3 < MOfloat > moMultipleCurve3f;
template class LIBMOLDEO_API moMultipleCurve3 < MOdouble >;
typedef moMultipleCurve3 < MOdouble > moMultipleCurve3d;
#endif
