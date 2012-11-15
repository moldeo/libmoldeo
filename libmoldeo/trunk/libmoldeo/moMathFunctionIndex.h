/*******************************************************************************

                                moMathFunctionIndex.h

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

#include "moTypes.h"
#include "moAbstract.h"
#include "moConfig.h"
#include "moParam.h"
#include "moMathManager.h"

#ifndef __MO_MATHFUNCTION_INDEX_H
#define __MO_MATHFUNCTION_INDEX_H

/**
 * Clase que encapsula una lista de punteros a objetos moMathFunction. La utilidad de esta clase es que
 * permite definir un subconjunto de funciones matemáticas definidas en el MathManager y accederlas de
 *  una manera más directa.
 * @see moMathManager
 */
class LIBMOLDEO_API moMathFunctionIndex : public moAbstract
{
public:
    /**
     * Constructor por defecto.
     */
	moMathFunctionIndex() { m_functions_array.Init(0, NULL); }
    /**
     * Destructor por defecto.
     */
	virtual ~moMathFunctionIndex() { Finish(); }

    /**
     * Método de inicialización.
     * @param p_cfg puntero al objeto de configuración de Moldeo donde se especifica la lista de funciones matemáticas.
     * @param p_math_man adminstrador matemático donde las nuevas funciones son agregadas.
     * @param p_param_idx
     * @param p_subvalue_idx
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg,  moMathManager* p_math_man, MOint p_param_idx, MOuint p_subvalue_idx = 0)
	{ LoadFunctions( p_cfg, p_math_man, p_param_idx, p_subvalue_idx); return true; }
    /**
     * Método de inicialización.
     * @param p_cfg puntero al objeto de configuración de Moldeo donde se especifica la lista de funciones matemáticas.
     * @param p_math_man adminstrador matemático donde las nuevas funciones son agregadas.
     * @param p_param_idx índice del parámetro que contiene a las funciones.
     * @param p_subvalue_idx
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg,  moMathManager* p_math_man, moParamIndex p_param_idx, MOuint p_subvalue_idx = 0)
	{ LoadFunctions( p_cfg, p_math_man, p_param_idx.index, p_subvalue_idx); return true; }
    /**
     * Método de inicialización.
     * @param p_cfg puntero al objeto de configuración de Moldeo donde se especifica la lista de funciones matemáticas.
     * @param p_math_man adminstrador matemático donde las nuevas funciones son agregadas.
     * @param p_param_ref
     * @param p_subvalue_idx
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg,  moMathManager* p_math_man, moParamReference p_param_ref, MOuint p_subvalue_idx = 0)
	{ LoadFunctions( p_cfg, p_math_man, p_cfg->GetConfigDefinition()->ParamIndexes().Get(p_param_ref.reference).index, p_subvalue_idx); return true; }
    /**
     * Método de finalización.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish() { m_functions_array.Finish(); return true; }

    /**
     * Agrega una nueva función a la lista y al administrador matemático, en el caso de no existir.
     * @param p_name expresión que define a la nueva función.
     * @param p_math_man puntero al administrador matemático.
     * @return índice de la nueva función (-1 si la operación no tiene éxito).
     */
	MOint Add(moText p_name, moMathManager* p_math_man);

    /**
     * Agrega una a la lista el puntero a un objeto moMathFunction provisto.
     * @param p_func puntero a la función matemática.
     * @return índice de la nueva función (-1 si la operación no tiene éxito).
     */
	MOint Add( moMathFunction* p_func);

    /**
     * Devuelve la función con índice p_idx.
     * @param p_idx índice de la función.
     * @return puntero a la función matemática.
     */
	moMathFunction* GetFunction(MOuint p_idx);

    /**
     * Verifica la validez del índice especificado.
     * @param p_idx índice de la función.
     * @return true si el índice es válido, false en caso contrario.
     */
	MOboolean ValidIndex(MOuint p_idx);
    /**
     * Devuelve el número de funciones matemáticas almacenadas en la lista.
     * @return número de funciones.
     */
	MOuint Count() { return m_functions_array.Count(); }
    /**
     * Devuelve la función con índice p_idx.
     * @param p_idx índice de la función.
     * @return puntero a la función matemática.
     */
	moMathFunction* operator [](MOuint p_idx) { return GetFunction(p_idx); }

    /**
     * Devuelve la función con índice p_value_index. Si p_value_index tiene el valor MO_SELECTED
     * entonces la función que corresponde al parámetro seleccionado actualemente es devuelta.
     * @param p_idx índice de la función.
     * @return puntero a la función matemática.
     */
	moMathFunction* Fun( MOint p_value_index = MO_SELECTED );
private:
	moMathFunctionArray m_functions_array;

	moConfig*	m_pConfig;
	moParamIndex m_ParamIndex;
	MOuint		m_SubValueIndex;

	MOuint LoadFunctions( moConfig* p_cfg,  moMathManager* p_math_man, MOuint p_param_idx, MOuint p_subvalue_idx = 0);
};

#endif

