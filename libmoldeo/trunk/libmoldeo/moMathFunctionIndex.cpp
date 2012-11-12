/*******************************************************************************

                              moMathFunctionIndex.cpp

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

#include "moMathFunctionIndex.h"

MOint moMathFunctionIndex::Add(moText p_name, moMathManager* p_math_man) // Returns the index of the newly added texture.
{
	MOint moid = p_math_man->AddFunction(p_name, false);
	if (-1 < moid) return Add(p_math_man->GetFunction(moid));
	else return -1;
}

MOint moMathFunctionIndex::Add(moMathFunction* p_tex)
{
	MOint idx;
	m_functions_array.Add(p_tex);
	idx = m_functions_array.Count() - 1;
	moText msg;
	#ifdef _DEBUG
        msg = moText("Function loaded: ");
        msg += m_functions_array[idx]->GetExpression();
        MODebug2->Log(msg);
    #endif
	return idx;
}


moMathFunction* moMathFunctionIndex::GetFunction(MOuint p_idx)
{
	if (ValidIndex(p_idx)) return m_functions_array[p_idx];
	return NULL;
}

MOboolean moMathFunctionIndex::ValidIndex(MOuint p_idx)
{
	if ( p_idx < m_functions_array.Count()) return true;
	else
	{
		moText text;
		text =  moText("Error(moMathFunctionIndex): the index: ");
		text += IntToStr(p_idx);
		text +=  moText(" does not exists.");
		MODebug2->Error(text);
		return false;
	}
}

MOuint moMathFunctionIndex::LoadFunctions(moConfig* p_cfg,  moMathManager* p_math_man, MOuint p_param_idx, MOuint p_subvalue_idx)
{
	if (p_cfg!=NULL) m_pConfig = p_cfg; else return MO_ERROR;
	m_ParamIndex.index = p_param_idx;
	m_SubValueIndex = p_subvalue_idx;

	MOint moid;
	MOint idx = -1;
	MOuint nFunctions = m_pConfig->GetParam(p_param_idx).GetValuesCount();
	MOuint nSubValues;
	moMathFunction *pfunc;

	p_cfg->SetCurrentParamIndex(m_ParamIndex.index);
	p_cfg->FirstValue();
	for (MOuint i = 0; i < nFunctions; i++)
	{
		nSubValues = p_cfg->GetParam().GetValue(i).GetSubValueCount();
		moText txt = "";
		moValueType vt;

		vt = p_cfg->GetParam().GetValue(i).GetSubValue(p_subvalue_idx).GetType();
		if ( vt==MO_VALUE_FUNCTION ) {
			txt = p_cfg->GetParam().GetValue(i).GetSubValue(p_subvalue_idx).Text();
		} else {
			txt = FloatToStr(p_cfg->GetParam().GetValue(i).GetSubValue(p_subvalue_idx).Float());
		}
		moid = p_math_man->AddFunction( txt, false);
		if (-1 < moid)
		{
			idx++;
			pfunc = p_math_man->GetFunction(moid);
			m_functions_array.Add(pfunc);
			#ifdef _DEBUG
                moText msg;
                msg = moText("Function loaded: ");
                msg += pfunc->GetExpression();
                MODebug2->Log(msg);
            #endif
		}
		p_cfg->NextValue();
	}
	return m_functions_array.Count();
}


moMathFunction* moMathFunctionIndex::Fun( MOint p_value_index ) {
	if (p_value_index == MO_SELECTED)
		p_value_index = m_pConfig->GetParams().Get(m_ParamIndex.index).GetIndexValue();
	return (m_pConfig !=NULL ? GetFunction( p_value_index ) : NULL);

}

