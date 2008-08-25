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
  Andr�s Colubri

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
 * permite definir un subconjunto de funciones matem�ticas definidas en el MathManager y accederlas de
 *  una manera m�s directa.
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
     * M�todo de inicializaci�n.
     * @param p_cfg puntero al objeto de configuraci�n de Moldeo donde se especifica la lista de funciones matem�ticas.
     * @param p_math_man adminstrador matem�tico donde las nuevas funciones son agregadas.
     * @param p_param_idx
     * @param p_subvalue_idx
     * @return resultado de la operaci�n: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg,  moMathManager* p_math_man, MOint p_param_idx, MOuint p_subvalue_idx = 0)
	{ LoadFunctions( p_cfg, p_math_man, p_param_idx, p_subvalue_idx); return true; }
    /**
     * M�todo de inicializaci�n.
     * @param p_cfg puntero al objeto de configuraci�n de Moldeo donde se especifica la lista de funciones matem�ticas.
     * @param p_math_man adminstrador matem�tico donde las nuevas funciones son agregadas.
     * @param p_param_idx �ndice del par�metro que contiene a las funciones.
     * @param p_subvalue_idx
     * @return resultado de la operaci�n: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg,  moMathManager* p_math_man, moParamIndex p_param_idx, MOuint p_subvalue_idx = 0)
	{ LoadFunctions( p_cfg, p_math_man, p_param_idx.index, p_subvalue_idx); return true; }
    /**
     * M�todo de inicializaci�n.
     * @param p_cfg puntero al objeto de configuraci�n de Moldeo donde se especifica la lista de funciones matem�ticas.
     * @param p_math_man adminstrador matem�tico donde las nuevas funciones son agregadas.
     * @param p_param_ref
     * @param p_subvalue_idx
     * @return resultado de la operaci�n: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg,  moMathManager* p_math_man, moParamReference p_param_ref, MOuint p_subvalue_idx = 0)
	{ LoadFunctions( p_cfg, p_math_man, p_cfg->GetConfigDefinition()->ParamIndexes().Get(p_param_ref.reference).index, p_subvalue_idx); return true; }
    /**
     * M�todo de finalizaci�n.
     * @return resultado de la operaci�n: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish() { m_functions_array.Finish(); return true; }

    /**
     * Agrega una nueva funci�n a la lista y al administrador matem�tico, en el caso de no existir.
     * @param p_name expresi�n que define a la nueva funci�n.
     * @param p_math_man puntero al administrador matem�tico.
     * @return �ndice de la nueva funci�n (-1 si la operaci�n no tiene �xito).
     */
	MOint Add(moText p_name, moMathManager* p_math_man);

    /**
     * Agrega una a la lista el puntero a un objeto moMathFunction provisto.
     * @param p_func puntero a la funci�n matem�tica.
     * @return �ndice de la nueva funci�n (-1 si la operaci�n no tiene �xito).
     */
	MOint Add( moMathFunction* p_func);

    /**
     * Devuelve la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n.
     * @return puntero a la funci�n matem�tica.
     */
	moMathFunction* GetFunction(MOuint p_idx);

    /**
     * Verifica la validez del �ndice especificado.
     * @param p_idx �ndice de la funci�n.
     * @return true si el �ndice es v�lido, false en caso contrario.
     */
	MOboolean ValidIndex(MOuint p_idx);
    /**
     * Devuelve el n�mero de funciones matem�ticas almacenadas en la lista.
     * @return n�mero de funciones.
     */
	MOuint Count() { return m_functions_array.Count(); }
    /**
     * Devuelve la funci�n con �ndice p_idx.
     * @param p_idx �ndice de la funci�n.
     * @return puntero a la funci�n matem�tica.
     */
	moMathFunction* operator [](MOuint p_idx) { return GetFunction(p_idx); }

    /**
     * Devuelve la funci�n con �ndice p_value_index. Si p_value_index tiene el valor MO_SELECTED
     * entonces la funci�n que corresponde al par�metro seleccionado actualemente es devuelta.
     * @param p_idx �ndice de la funci�n.
     * @return puntero a la funci�n matem�tica.
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
