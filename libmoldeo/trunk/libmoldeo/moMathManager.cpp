/*******************************************************************************

                              moMathManager.cpp

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

*******************************************************************************/

#include "moMathManager.h"

#include "moMathVector3.h"

moMathd *moMathManager::mathfd = new moMathd();


moMathManager::moMathManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_MATH );

	SetName("Math Manager");
	SetLabelName("Math Manager");
}

moMathManager::~moMathManager()
{
	Finish();
}

MOboolean moMathManager::Init()
{
    m_functions.Init(0, NULL);

	return true;
}

MOboolean moMathManager::Finish()
{
	for(MOuint i =0; i<m_functions.Count(); i++) {
		moMathFunction* pfunc = m_functions[i];
		if (pfunc) {
			pfunc->Finish();
			delete pfunc;
			m_functions.Set(i,NULL);
		}
	}
	m_functions.Finish();

	return true;
}

moMathFunction* moMathManager::GetFunction(MOuint p_idx)
{
	return m_functions[p_idx];
}


MOint moMathManager::GetFunctionIdx(const moText& p_expr)
{
	moMathFunction* pfun;
	for (MOuint i = 0; i < m_functions.Count(); i++)
	{
		pfun = m_functions[i];
		if ((pfun != NULL) && (!stricmp(pfun->GetExpression(), moText(p_expr)))) return i;
	}
	return -1;
}

MOint moMathManager::AddFunction(const moText& p_expr, MOboolean p_force_new, moMoldeoObject* p_pMOB )
{

	MOint idx = -1;

	if ( p_expr.Length() == 0 )
        return idx;

	if (!p_force_new)
	{
        idx = GetFunctionIdx(p_expr);
		if (-1 < idx) return idx; // La función ya existe.
	}

	moMathFunction* p_math_fun;
	moParserFunction* p_parser_fun; // Función de parseado.

    p_math_fun = BuiltInMathFunctionFactory(p_expr);
	if (p_math_fun == NULL)
	{
		p_parser_fun = new moParserFunction();

		p_math_fun = (moMathFunction*)p_parser_fun;
	}

	if (p_math_fun != NULL)
	{
	    bool res = p_math_fun->Init( p_expr, p_pMOB );
		if (res)
		{
			m_functions.Add(p_math_fun);
			//double check = p_math_fun->Eval(0);
			return m_functions.Count() - 1;
		}
		else
		{
			delete p_math_fun;
			return -1;
		}
	}
	else return -1;
}


double moMathManager::EvalFunction(MOuint p_idx, double x0)
{
    if (ValidFunction(p_idx))
			return m_functions[p_idx]->Eval(x0);
	else return 0;
}

double moMathManager::EvalFunction(MOuint p_idx, double x0, double x1)
{
    if (ValidFunction(p_idx)) return m_functions[p_idx]->Eval(x0, x1);
	else return 0;
}

double moMathManager::EvalFunction(MOuint p_idx, double x0, double x1, double x2)
{
    if (ValidFunction(p_idx)) return m_functions[p_idx]->Eval(x0, x1, x2);
	else return 0;
}

void moMathManager::SetFunctionParam(MOuint p_idx, double s0)
{
    if (ValidFunction(p_idx)) m_functions[p_idx]->SetParameters(s0);
}

void moMathManager::SetFunctionParam(MOuint p_idx, double s0, double s1)
{
    if (ValidFunction(p_idx)) m_functions[p_idx]->SetParameters(s0, s1);
}

void moMathManager::SetFunctionParam(MOuint p_idx, double s0, double s1, double s2)
{
    if (ValidFunction(p_idx)) m_functions[p_idx]->SetParameters(s0, s1, s2);
}

