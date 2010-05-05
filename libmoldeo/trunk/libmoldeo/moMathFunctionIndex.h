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
  Andrs Colubri

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
 * permite definir un subconjunto de funciones matemticas definidas en el MathManager y accederlas de
 *  una manera ms directa.
 * @see moMathManager
 */
class LIBMOLDEO_API moMathFunctionIndex:public moAbstract
{
	public:
		/**
		 * Constructor por defecto.
		 */
		moMathFunctionIndex ()
		{
			m_functions_array.Init (0, NULL);
		}
		/**
		 * Destructor por defecto.
		 */
		virtual ~ moMathFunctionIndex ()
		{
			Finish ();
		}
		/**
		 * Mtodo de inicializacin.
		 * @param p_cfg puntero al objeto de configuracin de Moldeo donde se especifica la lista de funciones matemticas.
		 * @param p_math_man adminstrador matemtico donde las nuevas funciones son agregadas.
		 * @param p_param_idx
		 * @param p_subvalue_idx
		 * @return resultado de la operacin: true si fue exitosa, false en caso contrario.
		 */
		virtual MOboolean Init (moConfig * p_cfg, moMathManager * p_math_man,
			MOint p_param_idx, MOuint p_subvalue_idx = 0)
		{
			LoadFunctions (p_cfg, p_math_man, p_param_idx, p_subvalue_idx);
			return true;
		}

		/**
		 * Mtodo de inicializacin.
		 * @param p_cfg puntero al objeto de configuracin de Moldeo donde se especifica la lista de funciones matemticas.
		 * @param p_math_man adminstrador matemtico donde las nuevas funciones son agregadas.
		 * @param p_param_idx ndice del parmetro que contiene a las funciones.
		 * @param p_subvalue_idx
		 * @return resultado de la operacin: true si fue exitosa, false en caso contrario.
		 */
		virtual MOboolean Init (moConfig * p_cfg, moMathManager * p_math_man,
			moParamIndex p_param_idx, MOuint p_subvalue_idx = 0)
		{
			LoadFunctions (p_cfg, p_math_man, p_param_idx.index, p_subvalue_idx);
			return true;
		}

		/**
		 * Mtodo de inicializacin.
		 * @param p_cfg puntero al objeto de configuracin de Moldeo donde se especifica la lista de funciones matemticas.
		 * @param p_math_man adminstrador matemtico donde las nuevas funciones son agregadas.
		 * @param p_param_ref
		 * @param p_subvalue_idx
		 * @return resultado de la operacin: true si fue exitosa, false en caso contrario.
		 */
		virtual MOboolean Init (moConfig * p_cfg, moMathManager * p_math_man,
			moParamReference p_param_ref,
			MOuint p_subvalue_idx = 0)
		{
			LoadFunctions (p_cfg, p_math_man,
				p_cfg->GetConfigDefinition ()->
				ParamIndexes ().Get (p_param_ref.reference).index,
				p_subvalue_idx);
			return true;
		}

		/**
		 * Mtodo de finalizacin.
		 * @return resultado de la operacin: true si fue exitosa, false en caso contrario.
		 */
		virtual MOboolean Finish ()
		{
			m_functions_array.Finish ();
			return true;
		}

		/**
		 * Agrega una nueva funcin a la lista y al administrador matemtico, en el caso de no existir.
		 * @param p_name expresin que define a la nueva funcin.
		 * @param p_math_man puntero al administrador matemtico.
		 * @return ndice de la nueva funcin (-1 si la operacin no tiene xito).
		 */
		MOint Add (moText p_name, moMathManager * p_math_man);

		/**
		 * Agrega una a la lista el puntero a un objeto moMathFunction provisto.
		 * @param p_func puntero a la funcin matemtica.
		 * @return ndice de la nueva funcin (-1 si la operacin no tiene xito).
		 */
		MOint Add (moMathFunction * p_func);

		/**
		 * Devuelve la funcin con ndice p_idx.
		 * @param p_idx ndice de la funcin.
		 * @return puntero a la funcin matemtica.
		 */
		moMathFunction * GetFunction (MOuint p_idx);

		/**
		 * Verifica la validez del ndice especificado.
		 * @param p_idx ndice de la funcin.
		 * @return true si el ndice es vlido, false en caso contrario.
		 */
		MOboolean ValidIndex (MOuint p_idx);

		/**
		 * Devuelve el nmero de funciones matemticas almacenadas en la lista.
		 * @return nmero de funciones.
		 */
		MOuint Count ()
		{
			return m_functions_array.Count ();
		}

		/**
		 * Devuelve la funcin con ndice p_idx.
		 * @param p_idx ndice de la funcin.
		 * @return puntero a la funcin matemtica.
		 */
		moMathFunction *operator [] (MOuint p_idx)
		{
			return GetFunction (p_idx);
		}

		/**
		 * Devuelve la funcin con ndice p_value_index. Si p_value_index tiene el valor MO_SELECTED
		 * entonces la funcin que corresponde al parmetro seleccionado actualemente es devuelta.
		 * @param p_idx ndice de la funcin.
		 * @return puntero a la funcin matemtica.
		 */
		moMathFunction * Fun (MOint p_value_index = MO_SELECTED);
	private:moMathFunctionArray m_functions_array;
	moConfig * m_pConfig;
	moParamIndex m_ParamIndex;
	MOuint m_SubValueIndex;
	MOuint LoadFunctions (moConfig * p_cfg, moMathManager * p_math_man,
		MOuint p_param_idx, MOuint p_subvalue_idx = 0);
};
#endif							 /*  */
