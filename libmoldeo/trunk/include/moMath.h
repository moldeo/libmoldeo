/*******************************************************************************

                                moMath.h

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

#ifndef __MO_MATH_H__
#define __MO_MATH_H__

#include "moTypes.h"
#include "moAbstract.h"

#ifndef NOMINMAX
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#endif

/**
 * Clase template que implementa funciones matemáticas básicas. Si bien muchas de ellas
 * ya están disponibles en las librerías estándard de C, las provistas aquí ofrecen
 * algoritmos aproximados más rápidos, control de bordes, etc.
 * Las funciones aceleradas (Fast*) implementan evaluaciones rápidas de funciones
 * trigonométricas y sus inversas, utilizando aproximaciones polinomiales.
 * Las mejoras en velocidad fueron medidas con un CPU AMD 2800 (2.08 GHz) utilizando
 * Visual Studio .NET 2003 con un release build.
 */

 template <class Real>
class LIBMOLDEO_API moMath
{
public:
    /**
     * Constructor por defecto.
     */
    moMath() {}
    /**
     * Destructor por defecto.
     */
    ~moMath() {}

	/**
     * Función arco coseno, el argumento es restringido al intervalo [-1,1].
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real ACos (Real fValue)
    {
        if (-(Real)1.0 < fValue)
        {
            if (fValue < (Real)1.0)
            {
                return (Real)acos((double)fValue);
            }
            else
            {
                return (Real)0.0;
            }
        }
        else
        {
            return moMath::PI;
        }
    }

	/**
     * Función arco seno, el argumento es restringido al intervalo [-1,1].
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real ASin (Real fValue)
    {
        if (-(Real)1.0 < fValue)
        {
            if (fValue < (Real)1.0)
            {
                return (Real)asin((double)fValue);
            }
            else
            {
                return HALF_PI;
            }
        }
        else
        {
            return -HALF_PI;
        }
    }

	/**
     * Función arco tangente.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real ATan (Real fValue)
    {
        return (Real)atan((double)fValue);
    }

	/**
     * Función arco tangente 2. Calcula el arco tangente de fY/fY, pero devolviendo un resultado
     * en el rango (-PI, PI].
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real ATan2 (Real fY, Real fX)
    {
        return (Real)atan2((double)fY,(double)fX);
    }

	/**
     * Función techo.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Ceil (Real fValue)
    {
        return (Real)ceil((double)fValue);
    }

	/**
     * Función coseno.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Cos (Real fValue)
    {
        return (Real)cos((double)fValue);
    }

	/**
     * Función exponencial.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Exp (Real fValue)
    {
        return (Real)exp((double)fValue);
    }

	/**
     * Función valor absoluto.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real FAbs (Real fValue)
    {
        return (Real)fabs((double)fValue);
    }

	/**
     * Función piso.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Floor (Real fValue)
    {
        return (Real)floor((double)fValue);
    }

	/**
     * Función módulo.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real FMod (Real fX, Real fY)
    {
        return (Real)fmod((double)fX,(double)fY);
    }

	/**
     * Función 1 / raiz cuadrada.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real InvSqrt (Real fValue)
    {
        return (Real)(1.0/sqrt((double)fValue));
    }

	/**
     * Función logaritmo natural.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Log (Real fValue)
    {
        return (Real)log((double)fValue);
    }

	/**
     * Función logaritmo (en base 2).
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Log2 (Real fValue)
    {
        return moMath<Real>::INV_LN_2 * (Real)log((double)fValue);
    }

	/**
     * Función logaritmo (en base 10).
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Log10 (Real fValue)
    {
        return moMath<Real>::INV_LN_10 * (Real)log((double)fValue);
    }

	/**
     * Función potencia.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Pow (Real fBase, Real fExponent)
    {
        return (Real)pow((double)fBase,(double)fExponent);
    }

	/**
     * Función seno.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Sin (Real fValue)
    {
        return (Real)sin((double)fValue);
    }

	/**
     * Función potencia de dos.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Sqr (Real fValue) {
        return fValue*fValue;
    }

	/**
     * Función raiz cuadrada.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Sqrt (Real fValue) {
        return (Real)sqrt((double)fValue);
    }

	/**
     * Función tangente.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Tan (Real fValue) {
         return (Real)tan((double)fValue);
    }

	/**
     * Verifica que el argumento sea una potencia de dos.
	 * @param iValue argumento.
	 * @return resultado.
	 */
	inline bool IsPowerOfTwo (int iValue) {
	  return (iValue > 0) && ((iValue & (iValue - 1)) == 0);
    }

	/**
     * Calcula el logaritmo en base 2 de un número que es una potencia de 2.
	 * @param uiPowerOfTwo argumento.
	 * @return resultado.
	 */
	inline unsigned int Log2OfPowerOfTwo (unsigned int uiPowerOfTwo) {
	  unsigned int uiLog2 = (uiPowerOfTwo & 0xAAAAAAAA) != 0;
        uiLog2 |= ((uiPowerOfTwo & 0xFFFF0000) != 0) << 4;
        uiLog2 |= ((uiPowerOfTwo & 0xFF00FF00) != 0) << 3;
        uiLog2 |= ((uiPowerOfTwo & 0xF0F0F0F0) != 0) << 2;
        uiLog2 |= ((uiPowerOfTwo & 0xCCCCCCCC) != 0) << 1;
        return uiLog2;
    }

	/**
     * Conversión rápida de un número fFloat de punto flotante de 32 bits en el intervalo [0,1]
     *a un entero de 32 bits en [0,2^iLog-1].
	 * @param fFloat número a convertir.
	 * @param iLog exponende del límite derecho del intervalo.
	 * @return resultado de la conversión.
	 */
    inline static int ScaledFloatToInt(float fFloat, int iLog)
    {
		int iInt;
        int iShift = 150 - iLog - ((*(int*)(&fFloat) >> 23) & 0xFF);
        if ( iShift < 24 )
        {
            iInt = ((*(int*)(&fFloat) & 0x007FFFFF) | 0x00800000) >> iShift;
            if ( iInt == (1 << iLog) ) iInt--;
        }
        else iInt = 0;
		return iInt;
    }

	/**
     * Función signo (devuelve -1 si el argumento es negtivo, 0 si es cero, y +1 si es positivo).
	 * @param iValue argumento.
	 * @return resultado.
	 */
    inline static int Sign (int iValue) {
        if (iValue > 0)
        {
            return 1;
        }

        if (iValue < 0)
        {
            return -1;
        }

        return 0;
    }

	/**
     * Función signo (devuelve -1 si el argumento es negtivo, 0 si es cero, y +1 si es positivo).
	 * @param fValue argumento.
	 * @return resultado.
	 */
    inline static Real Sign (Real fValue) {
      if (fValue > (Real)0.0)
      {
          return (Real)1.0;
      }

      if (fValue < (Real)0.0)
      {
          return -(Real)1.0;
      }

      return (Real)0.0;
    }

	/**
     * Genera un número aleatorio en [0,1). El generador puede ser inicializado con una primera llamada
     * pasando como parámetro una semilla positiva.
	 * @param uiSeed semilla para el generador aleatorio.
	 * @return número aleatorio en en [0,1).
	 */
    inline static Real UnitRandom (unsigned int uiSeed = 0) {
      if (uiSeed > 0)
      {
          srand(uiSeed);
      }

      double dRatio = ((double)rand())/((double)(RAND_MAX));
      return (Real)dRatio;
    }

	/**
     * Genera un número aleatorio en [-1,1). El generador puede ser inicializado con una primera llamada
     * pasando como parámetro una semilla positiva.
	 * @param uiSeed semilla para el generador aleatorio.
	 * @return número aleatorio en [-1,1).
	 */
    inline static Real SymmetricRandom (unsigned int uiSeed = 0) {
      if (uiSeed > 0.0)
      {
          srand(uiSeed);
      }

      double dRatio = ((double)rand())/((double)(RAND_MAX));
      return (Real)(2.0*dRatio - 1.0);
    }

	/**
     * Genera un número aleatorio en [fMin,fMax). El generador puede ser inicializado con una primera llamada
     * pasando como parámetro uiSeed una semilla positiva.
	 * @param fMin extremo izquierdo del intervalo.
	 * @param fMax extremo derecho del intervalo.
	 * @param uiSeed semilla para el generador aleatorio.
	 * @return resultado.
	 */
    inline static Real IntervalRandom (Real fMin, Real fMax,
        unsigned int uiSeed = 0) {
        if (uiSeed > 0)
        {
            srand(uiSeed);
        }

        double dRatio = ((double)rand())/((double)(RAND_MAX));
        return fMin+(fMax-fMin)*((Real)dRatio);
    }

	/**
     * Genera un número aleatorio en [0,1). El generador puede ser inicializado con una primera llamada
     * pasando como parámetro una semilla positiva.
	 * @param rSeed semilla para el generador aleatorio.
	 * @return número aleatorio en en [0,1).
	 */
	inline static Real UnitRandom (Real rSeed) {
	  return UnitRandom ((unsigned int)rSeed);
  }
	/**
     * Genera un número aleatorio en [-1,1). El generador puede ser inicializado con una primera llamada
     * pasando como parámetro una semilla positiva.
	 * @param rSeed semilla para el generador aleatorio.
	 * @return número aleatorio en [-1,1).
	 */
	inline static Real SymmetricRandom (Real rSeed) {
    return SymmetricRandom  ((unsigned int)rSeed);
  }
	/**
     * Genera un número aleatorio en [fMin,fMax). El generador puede ser inicializado con una primera llamada
     * pasando como parámetro rSeed una semilla positiva.
	 * @param fMin extremo izquierdo del intervalo.
	 * @param fMax extremo derecho del intervalo.
	 * @param rSeed semilla para el generador aleatorio.
	 * @return resultado.
	 */
	inline static Real IntervalRandom (Real fMin, Real fMax, Real rSeed) {
	  return IntervalRandom (fMin, fMax, (unsigned int)rSeed);
	}

	/**
     * Función seno acelerada, version 0. Max error sin0 = 1.7e-04, aceleración = 4.0
	 * @param fValue argumento en [0,pi/2].
	 * @return resultado.
	 */
    inline static Real FastSin0 (Real fAngle) {
      Real fASqr = fAngle*fAngle;
      Real fResult = (Real)7.61e-03;
      fResult *= fASqr;
      fResult -= (Real)1.6605e-01;
      fResult *= fASqr;
      fResult += (Real)1.0;
      fResult *= fAngle;
      return fResult;
    }

	/**
     * Función seno acelerada, version 1. Max error sin0 = 1.9e-08, aceleración = 2.8
	 * @param fValue argumento en [0,pi/2].
	 * @return resultado.
	 */
    inline static Real FastSin1 (Real fAngle) {
      Real fASqr = fAngle*fAngle;
      Real fResult = -(Real)2.39e-08;
      fResult *= fASqr;
      fResult += (Real)2.7526e-06;
      fResult *= fASqr;
      fResult -= (Real)1.98409e-04;
      fResult *= fASqr;
      fResult += (Real)8.3333315e-03;
      fResult *= fASqr;
      fResult -= (Real)1.666666664e-01;
      fResult *= fASqr;
      fResult += (Real)1.0;
      fResult *= fAngle;
      return fResult;
    }

	/**
     * Función coseno acelerada, version 0. Max error sin0 = 1.2e-03, aceleración = 4.5
	 * @param fValue argumento en [0,pi/2].
	 * @return resultado.
	 */
    inline static Real FastCos0 (Real fAngle) {
      Real fASqr = fAngle*fAngle;
      Real fResult = (Real)3.705e-02;
      fResult *= fASqr;
      fResult -= (Real)4.967e-01;
      fResult *= fASqr;
      fResult += (Real)1.0;
      return fResult;
    }

	/**
     * Función coseno acelerada, version 1. Max error sin0 = 6.5e-09, aceleración = 2.8
	 * @param fValue argumento en [0,pi/2].
	 * @return resultado.
	 */
    inline static Real FastCos1 (Real fAngle) {
      Real fASqr = fAngle*fAngle;
      Real fResult = -(Real)2.605e-07;
      fResult *= fASqr;
      fResult += (Real)2.47609e-05;
      fResult *= fASqr;
      fResult -= (Real)1.3888397e-03;
      fResult *= fASqr;
      fResult += (Real)4.16666418e-02;
      fResult *= fASqr;
      fResult -= (Real)4.999999963e-01;
      fResult *= fASqr;
      fResult += (Real)1.0;
      return fResult;
    }

	/**
     * Función tangente acelerada, version 0. Max error sin0 = 8.1e-04, aceleración = 5.6
	 * @param fValue argumento en [0,pi/4].
	 * @return resultado.
	 */
    inline static Real FastTan0 (Real fAngle) {
      Real fASqr = fAngle*fAngle;
      Real fResult = (Real)2.033e-01;
      fResult *= fASqr;
      fResult += (Real)3.1755e-01;
      fResult *= fASqr;
      fResult += (Real)1.0;
      fResult *= fAngle;
      return fResult;
    }

	/**
     * Función tangente acelerada, version 1. Max error sin0 = 1.9e-08, aceleración = 3.4
	 * @param fValue argumento en [0,pi/4].
	 * @return resultado.
	 */
    inline static Real FastTan1 (Real fAngle) {
      Real fASqr = fAngle*fAngle;
      Real fResult = (Real)9.5168091e-03;
      fResult *= fASqr;
      fResult += (Real)2.900525e-03;
      fResult *= fASqr;
      fResult += (Real)2.45650893e-02;
      fResult *= fASqr;
      fResult += (Real)5.33740603e-02;
      fResult *= fASqr;
      fResult += (Real)1.333923995e-01;
      fResult *= fASqr;
      fResult += (Real)3.333314036e-01;
      fResult *= fASqr;
      fResult += (Real)1.0;
      fResult *= fAngle;
      return fResult;
    }

	/**
     * Función arco seno acelerada, version 0. Max error sin0 = 6.8e-05, aceleración = 7.5
	 * @param fValue argumento en [0,1].
	 * @return resultado.
	 */
    inline static Real FastInvSin0 (Real fValue) {
      Real fRoot = moMath<Real>::Sqrt(((Real)1.0)-fValue);
      Real fResult = -(Real)0.0187293;
      fResult *= fValue;
      fResult += (Real)0.0742610;
      fResult *= fValue;
      fResult -= (Real)0.2121144;
      fResult *= fValue;
      fResult += (Real)1.5707288;
      fResult = HALF_PI - fRoot*fResult;
      return fResult;
    }

	/**
     * Función arco seno acelerada, version 1. Max error sin0 = 1.4e-07, aceleración = 5.5
	 * @param fValue argumento en [0,1].
	 * @return resultado.
	 */
    inline static Real FastInvSin1 (Real fValue) {
      Real fRoot = moMath<Real>::Sqrt(FAbs(((Real)1.0)-fValue));
      Real fResult = -(Real)0.0012624911;
      fResult *= fValue;
      fResult += (Real)0.0066700901;
      fResult *= fValue;
      fResult -= (Real)0.0170881256;
      fResult *= fValue;
      fResult += (Real)0.0308918810;
      fResult *= fValue;
      fResult -= (Real)0.0501743046;
      fResult *= fValue;
      fResult += (Real)0.0889789874;
      fResult *= fValue;
      fResult -= (Real)0.2145988016;
      fResult *= fValue;
      fResult += (Real)1.5707963050;
      fResult = HALF_PI - fRoot*fResult;
      return fResult;
    }

	/**
     * Función arco coseno acelerada, version 0. Max error sin0 = 6.8e-05, aceleración = 7.5
	 * @param fValue argumento en [0,1].
	 * @return resultado.
	 */
    inline static Real FastInvCos0 (Real fValue) {
      Real fRoot = moMath<Real>::Sqrt(((Real)1.0)-fValue);
      Real fResult = -(Real)0.0187293;
      fResult *= fValue;
      fResult += (Real)0.0742610;
      fResult *= fValue;
      fResult -= (Real)0.2121144;
      fResult *= fValue;
      fResult += (Real)1.5707288;
      fResult *= fRoot;
      return fResult;
    }

	/**
     * Función arco coseno acelerada, version 1. Max error sin0 = 1.4e-07, aceleración = 5.7
	 * @param fValue argumento en [0,1].
	 * @return resultado.
	 */
    inline static Real FastInvCos1 (Real fValue) {
      Real fRoot = moMath<Real>::Sqrt(FAbs(((Real)1.0)-fValue));
      Real fResult = -(Real)0.0012624911;
      fResult *= fValue;
      fResult += (Real)0.0066700901;
      fResult *= fValue;
      fResult -= (Real)0.0170881256;
      fResult *= fValue;
      fResult += (Real)0.0308918810;
      fResult *= fValue;
      fResult -= (Real)0.0501743046;
      fResult *= fValue;
      fResult += (Real)0.0889789874;
      fResult *= fValue;
      fResult -= (Real)0.2145988016;
      fResult *= fValue;
      fResult += (Real)1.5707963050;
      fResult *= fRoot;
      return fResult;
    }

	/**
     * Función arco tangente acelerada, version 0. Max error sin0 = 1.2e-05, aceleración = 2.8
	 * @param fValue argumento en [-1,1].
	 * @return resultado.
	 */
    inline static Real FastInvTan0 (Real fValue) {
      Real fVSqr = fValue*fValue;
      Real fResult = (Real)0.0208351;
      fResult *= fVSqr;
      fResult -= (Real)0.085133;
      fResult *= fVSqr;
      fResult += (Real)0.180141;
      fResult *= fVSqr;
      fResult -= (Real)0.3302995;
      fResult *= fVSqr;
      fResult += (Real)0.999866;
      fResult *= fValue;
      return fResult;
    }

	/**
     * Función arco tangente acelerada, version 1. Max error sin0 = 2.3e-08, aceleración = 1.8
	 * @param fValue argumento en [-1,1].
	 * @return resultado.
	 */
    inline static Real FastInvTan1 (Real fValue) {
      Real fVSqr = fValue*fValue;
      Real fResult = (Real)0.0028662257;
      fResult *= fVSqr;
      fResult -= (Real)0.0161657367;
      fResult *= fVSqr;
      fResult += (Real)0.0429096138;
      fResult *= fVSqr;
      fResult -= (Real)0.0752896400;
      fResult *= fVSqr;
      fResult += (Real)0.1065626393;
      fResult *= fVSqr;
      fResult -= (Real)0.1420889944;
      fResult *= fVSqr;
      fResult += (Real)0.1999355085;
      fResult *= fVSqr;
      fResult -= (Real)0.3333314528;
      fResult *= fVSqr;
      fResult += (Real)1.0;
      fResult *= fValue;
      return fResult;
    }

	/**
     * Aproximación rápida de 1/sqrt.
	 * @param fValue argumento.
	 * @return resultado.
	 */
    static Real FastInvSqrt (Real fValue);

	/**
     * Función exponencial negativa (exp(-x)) acelerada, version 0. Max error sin0 = 0.00024, aceleración = 25.4
	 * @param fValue argumento en [0,infinito).
	 * @return resultado.
	 */
    inline static Real FastNegExp0 (Real fValue) {
      Real fResult = (Real)0.0038278;
      fResult *= fValue;
      fResult += (Real)0.0292732;
      fResult *= fValue;
      fResult += (Real)0.2507213;
      fResult *= fValue;
      fResult += (Real)1.0;
      fResult *= fResult;
      fResult *= fResult;
      fResult = ((Real)1.0)/fResult;
      return fResult;
    }

	/**
     * Función exponencial negativa (exp(-x)) acelerada, version 1. Max error sin0 = 0.000024, aceleración = 25.4
	 * @param fValue argumento en [0,infinito).
	 * @return resultado.
	 */
    inline static Real FastNegExp1 (Real fValue) {
      Real fResult = (Real)0.00026695;
      fResult *= fValue;
      fResult += (Real)0.00227723;
      fResult *= fValue;
      fResult += (Real)0.03158565;
      fResult *= fValue;
      fResult += (Real)0.24991035;
      fResult *= fValue;
      fResult += (Real)1.0;
      fResult *= fResult;
      fResult *= fResult;
      fResult = ((Real)1.0)/fResult;
      return fResult;
    }

	/**
     * Función exponencial negativa (exp(-x)) acelerada, version 2. Max error sin0 = 0.0000024, aceleración = 20.5
	 * @param fValue argumento en [0,infinito).
	 * @return resultado.
	 */
    inline static Real FastNegExp2 (Real fValue) {
      Real fResult = (Real)0.000014876;
      fResult *= fValue;
      fResult += (Real)0.000127992;
      fResult *= fValue;
      fResult += (Real)0.002673255;
      fResult *= fValue;
      fResult += (Real)0.031198056;
      fResult *= fValue;
      fResult += (Real)0.250010936;
      fResult *= fValue;
      fResult += (Real)1.0;
      fResult *= fResult;
      fResult *= fResult;
      fResult = ((Real)1.0)/fResult;
      return fResult;
    }

	/**
     * Función exponencial negativa (exp(-x)) acelerada, version 2. Max error sin0 = 0.00000025, aceleración = 17.3
	 * @param fValue argumento en [0,infinito).
	 * @return resultado.
	 */
    inline static Real FastNegExp3 (Real fValue) {
      Real fResult = (Real)0.0000006906;
      fResult *= fValue;
      fResult += (Real)0.0000054302;
      fResult *= fValue;
      fResult += (Real)0.0001715620;
      fResult *= fValue;
      fResult += (Real)0.0025913712;
      fResult *= fValue;
      fResult += (Real)0.0312575832;
      fResult *= fValue;
      fResult += (Real)0.2499986842;
      fResult *= fValue;
      fResult += (Real)1.0;
      fResult *= fResult;
      fResult *= fResult;
      fResult = ((Real)1.0)/fResult;
      return fResult;
    }

	/**
     * Conversión de grados a radianes.
	 * @param iDeg argumento en grados.
	 * @return resultado en radianes.
	 */
    inline static Real DegToRad(Real iDeg)
    {
      return DEG_TO_RAD * iDeg;
    }

	/**
     * Conversión de radianes a grados.
	 * @param iDeg argumento en radianes.
	 * @return resultado en grados.
	 */
    inline static Real RadToDeg(Real iRad)
    {
      return RAD_TO_DEG * iRad;
    }

	/**
     * Constantes matemáticas útiles.
	 */
    static const Real EPSILON;
    static const Real ZERO_TOLERANCE;
    static const Real MAX_REAL;
    static const Real PI;
    static const Real TWO_PI;
    static const Real HALF_PI;
    static const Real INV_PI;
    static const Real INV_TWO_PI;
    static const Real DEG_TO_RAD;
    static const Real RAD_TO_DEG;
    static const Real LN_2;
    static const Real LN_10;
    static const Real INV_LN_2;
    static const Real INV_LN_10;
};

template class LIBMOLDEO_API moMath<double>;
typedef moMath<double> moMathd;

template class LIBMOLDEO_API moMath<float>;
typedef moMath<float> moMathf;

#ifndef MO_USING_VC
	template<> const MOfloat moMath<MOfloat>::EPSILON = FLT_EPSILON;
	template<> const MOfloat moMath<MOfloat>::ZERO_TOLERANCE = 1e-06f;
	template<> const MOfloat moMath<MOfloat>::MAX_REAL = FLT_MAX;
	template<> const MOfloat moMath<MOfloat>::PI = (MOfloat)(4.0*atan(1.0));
	template<> const MOfloat moMath<MOfloat>::TWO_PI = 2.0f*moMath<MOfloat>::PI;
	template<> const MOfloat moMath<MOfloat>::HALF_PI = 0.5f*moMath<MOfloat>::PI;
	template<> const MOfloat moMath<MOfloat>::INV_PI = 1.0f/moMath<MOfloat>::PI;
	template<> const MOfloat moMath<MOfloat>::INV_TWO_PI = 1.0f/moMath<MOfloat>::TWO_PI;
	template<> const MOfloat moMath<MOfloat>::DEG_TO_RAD = moMath<MOfloat>::PI/180.0f;
	template<> const MOfloat moMath<MOfloat>::RAD_TO_DEG = 180.0f/moMath<MOfloat>::PI;
	template<> const MOfloat moMath<MOfloat>::LN_2 = moMath<MOfloat>::Log(2.0f);
	template<> const MOfloat moMath<MOfloat>::LN_10 = moMath<MOfloat>::Log(10.0f);
	template<> const MOfloat moMath<MOfloat>::INV_LN_2 = 1.0f/moMath<MOfloat>::LN_2;
	template<> const MOfloat moMath<MOfloat>::INV_LN_10 = 1.0f/moMath<MOfloat>::LN_10;

	template<> const MOdouble moMath<MOdouble>::EPSILON = DBL_EPSILON;
	template<> const MOdouble moMath<MOdouble>::ZERO_TOLERANCE = 1e-08;
	template<> const MOdouble moMath<MOdouble>::MAX_REAL = DBL_MAX;
	template<> const MOdouble moMath<MOdouble>::PI = 4.0*atan(1.0);
	template<> const MOdouble moMath<MOdouble>::TWO_PI = 2.0*moMath<MOdouble>::PI;
	template<> const MOdouble moMath<MOdouble>::HALF_PI = 0.5*moMath<MOdouble>::PI;
	template<> const MOdouble moMath<MOdouble>::INV_PI = 1.0/moMath<MOdouble>::PI;
	template<> const MOdouble moMath<MOdouble>::INV_TWO_PI = 1.0/moMath<MOdouble>::TWO_PI;
	template<> const MOdouble moMath<MOdouble>::DEG_TO_RAD = moMath<MOdouble>::PI/180.0;
	template<> const MOdouble moMath<MOdouble>::RAD_TO_DEG = 180.0/moMath<MOdouble>::PI;
	template<> const MOdouble moMath<MOdouble>::LN_2 = moMath<MOdouble>::Log(2.0);
	template<> const MOdouble moMath<MOdouble>::LN_10 = moMath<MOdouble>::Log(10.0);
	template<> const MOdouble moMath<MOdouble>::INV_LN_2 = 1.0/moMath<MOdouble>::LN_2;
	template<> const MOdouble moMath<MOdouble>::INV_LN_10 = 1.0/moMath<MOdouble>::LN_10;

	template <>
	float moMath<MOfloat>::FastInvSqrt (MOfloat fValue)
	{
		MOfloat fHalf = 0.5f*fValue;
		int i  = *(int*)&fValue;
		i = 0x5f3759df - (i >> 1);
		fValue = *(MOfloat*)&i;
		fValue = fValue*(1.5f - fHalf*fValue*fValue);
		return fValue;
	}

	template <>
	MOdouble moMath<MOdouble>::FastInvSqrt (MOdouble dValue)
	{
		MOdouble dHalf = 0.5*dValue;
		MOlonglong i  = *(MOlonglong*)&dValue;
	#if defined(WM4_USING_VC70) || defined(WM4_USING_VC6)
		i = 0x5fe6ec85e7de30da - (i >> 1);
	#else
		i = 0x5fe6ec85e7de30daLL - (i >> 1);
	#endif
		dValue = *(MOdouble*)&i;
		dValue = dValue*(1.5 - dHalf*dValue*dValue);
		return dValue;
	}
#endif

#endif /*__MO_MATH_H__*/
