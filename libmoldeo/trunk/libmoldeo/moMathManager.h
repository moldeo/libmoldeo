/*******************************************************************************

                                moMathManager.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                    *
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

*******************************************************************************/

#ifndef __MO_MATHMANAGER_H__
#define __MO_MATHMANAGER_H__

#include "moTypes.h"
#include "moText.h"
#include "moConfig.h"
#include "moLock.h"
#include "moResourceManager.h"
#include "moMath.h"
#include "moMathFunction.h"

/**
 * Clase que implementa un administrador de funciones matemáticas.
 */
class LIBMOLDEO_API moMathManager : public moResource
{
public:
    /**
     * Constructor por defecto.
     */
	moMathManager();
    /**
     * Destructor por defecto.
     */
	virtual ~moMathManager();

    /**
     * Método de inicialización.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init();
    /**
     * Método de finalización.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish();

    /**
     * Devuelve la función con índice p_idx.
     * @param p_idx índice de la función.
     * @return puntero a la función matemática.
     */
	moMathFunction* GetFunction(MOuint p_idx);
    /**
     * Devuelve el índice de la función cuya expresión es p_expr.
     * @param p_expr expresión de la función a buscar.
     * @return índice a la función matemática.
     */
	MOint GetFunctionIdx(const moText& p_expr);
    /**
     * Agrega al administrador una nueva función matemática definida por la expresión p_expr.
     * Si la función ya existe, no es agregada a menos de que p_force_new sea true.
     * @param p_expr expresión que define a la función.
     * @param p_force_new true para forzar la creación de la función, a pesar de que ya exista una
     * instancia de la misma en el administrador.
     * @return índice de la función agregada.
     */
	MOint AddFunction(const moText& p_expr, MOboolean p_force_new = false, moMoldeoObject* p_pMOB = NULL );

    /**
     * Evalua la función con índice p_idx.
     * @param p_idx índice de la función
     * @param x0 primer argumento.
     * @return resultado de la evaluación de la función.
     */
	double EvalFunction(MOuint p_idx, double x0);
    /**
     * Evalua la función con índice p_idx.
     * @param p_idx índice de la función
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @return resultado de la evaluación de la función.
     */
	double EvalFunction(MOuint p_idx, double x0, double x1);
    /**
     * Evalua la función con índice p_idx.
     * @param p_idx índice de la función
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @param x2 segundo argumento.
     * @return resultado de la evaluación de la función.
     */
	double EvalFunction(MOuint p_idx, double x0, double x1, double x2);

    /**
     * Fija el el valor del primer parámetro de la función con índice p_idx.
     * @param p_idx índice de la función
     * @param s0 valor del primer parámetro.
     */
	void SetFunctionParam(MOuint p_idx, double s0);
    /**
     * Fija el el valor de los dos primeros parámetros de la función con índice p_idx.
     * @param p_idx índice de la función
     * @param s0 valor del primer parámetro.
     * @param s1 valor del segundo parámetro.
     */
	void SetFunctionParam(MOuint p_idx, double s0, double s1);
    /**
     * Fija el el valor de los tres primeros parámetros de la función con índice p_idx.
     * @param p_idx índice de la función
     * @param s0 valor del primer parámetro.
     * @param s1 valor del segundo parámetro.
     * @param s2 valor del tercer parámetro.
     */
	void SetFunctionParam(MOuint p_idx, double s0, double s1, double s2);

    /**
     * Verifica que el índice p_idx represente una función válida.
     * @param p_idx índice de la función
     * @return true si el índice es válido, false en caso contrario.
     */
	MOboolean ValidFunction(MOuint p_idx) {
		return m_functions[p_idx] != NULL;
	}

    /**
     * Evalua la función con índice p_idx.
     * @param p_idx índice de la función
     * @param x0 primer argumento.
     * @return resultado de la evaluación de la función.
     */
	double operator () (MOuint p_idx, double x0) { return EvalFunction(p_idx, x0); }
    /**
     * Evalua la función con índice p_idx.
     * @param p_idx índice de la función
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @return resultado de la evaluación de la función.
     */
	double operator () (MOuint p_idx, double x0, double x1) { return EvalFunction(p_idx, x0, x1); }
    /**
     * Evalua la función con índice p_idx.
     * @param p_idx índice de la función
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @param x2 segundo argumento.
     * @return resultado de la evaluación de la función.
     */
	double operator () (MOuint p_idx, double x0, double x1, double x2) { return EvalFunction(p_idx, x0, x1, x2); }

    /**
     * Devuelve la función con índice p_idx.
     * @param p_idx índice de la función.
     * @return puntero a la función matemática.
     */
	moMathFunction*  operator [] (MOuint p_idx) { return GetFunction(p_idx); }

	static moMathd*  mathfd;
protected:
	moMathFunctionArray m_functions;
};

#endif

