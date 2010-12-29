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
  Andr�s Colubri

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
 * Clase que implementa un administrador de funciones matem�ticas.
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
     * M�todo de inicializaci�n.
     * @return resultado de la operaci�n: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init();
    /**
     * M�todo de finalizaci�n.
     * @return resultado de la operaci�n: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish();

    /**
     * Devuelve la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n.
     * @return puntero a la funci�n matem�tica.
     */
	moMathFunction* GetFunction(MOuint p_idx);
    /**
     * Devuelve el �ndice de la funci�n cuya expresi�n es p_expr.
     * @param p_expr expresi�n de la funci�n a buscar.
     * @return �ndice a la funci�n matem�tica.
     */
	MOint GetFunctionIdx(const moText& p_expr);
    /**
     * Agrega al administrador una nueva funci�n matem�tica definida por la expresi�n p_expr.
     * Si la funci�n ya existe, no es agregada a menos de que p_force_new sea true.
     * @param p_expr expresi�n que define a la funci�n.
     * @param p_force_new true para forzar la creaci�n de la funci�n, a pesar de que ya exista una
     * instancia de la misma en el administrador.
     * @return �ndice de la funci�n agregada.
     */
	MOint AddFunction(const moText& p_expr, MOboolean p_force_new = false, moMoldeoObject* p_pMOB = NULL );

    /**
     * Evalua la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param x0 primer argumento.
     * @return resultado de la evaluaci�n de la funci�n.
     */
	double EvalFunction(MOuint p_idx, double x0);
    /**
     * Evalua la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @return resultado de la evaluaci�n de la funci�n.
     */
	double EvalFunction(MOuint p_idx, double x0, double x1);
    /**
     * Evalua la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @param x2 segundo argumento.
     * @return resultado de la evaluaci�n de la funci�n.
     */
	double EvalFunction(MOuint p_idx, double x0, double x1, double x2);

    /**
     * Fija el el valor del primer par�metro de la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param s0 valor del primer par�metro.
     */
	void SetFunctionParam(MOuint p_idx, double s0);
    /**
     * Fija el el valor de los dos primeros par�metros de la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param s0 valor del primer par�metro.
     * @param s1 valor del segundo par�metro.
     */
	void SetFunctionParam(MOuint p_idx, double s0, double s1);
    /**
     * Fija el el valor de los tres primeros par�metros de la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param s0 valor del primer par�metro.
     * @param s1 valor del segundo par�metro.
     * @param s2 valor del tercer par�metro.
     */
	void SetFunctionParam(MOuint p_idx, double s0, double s1, double s2);

    /**
     * Verifica que el �ndice p_idx represente una funci�n v�lida.
     * @param p_idx �ndice de la funci�n
     * @return true si el �ndice es v�lido, false en caso contrario.
     */
	MOboolean ValidFunction(MOuint p_idx) {
		return m_functions[p_idx] != NULL;
	}

    /**
     * Evalua la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param x0 primer argumento.
     * @return resultado de la evaluaci�n de la funci�n.
     */
	double operator () (MOuint p_idx, double x0) { return EvalFunction(p_idx, x0); }
    /**
     * Evalua la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @return resultado de la evaluaci�n de la funci�n.
     */
	double operator () (MOuint p_idx, double x0, double x1) { return EvalFunction(p_idx, x0, x1); }
    /**
     * Evalua la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n
     * @param x0 primer argumento.
     * @param x1 segundo argumento.
     * @param x2 segundo argumento.
     * @return resultado de la evaluaci�n de la funci�n.
     */
	double operator () (MOuint p_idx, double x0, double x1, double x2) { return EvalFunction(p_idx, x0, x1, x2); }

    /**
     * Devuelve la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n.
     * @return puntero a la funci�n matem�tica.
     */
	moMathFunction*  operator [] (MOuint p_idx) { return GetFunction(p_idx); }

	static moMathd*  mathfd;
protected:
	moMathFunctionArray m_functions;
};

#endif
