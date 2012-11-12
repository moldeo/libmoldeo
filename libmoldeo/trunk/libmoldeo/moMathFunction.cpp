/*******************************************************************************

                                moMathFunction.cpp

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


#include "muParser.h"

#include <cstdarg>

#include "moConfig.h"
#include "moMoldeoObject.h"
#include "moMathFunction.h"

#include "moArray.h"
moDefineDynamicArray( moIntArray )
moDefineDynamicArray( moBoolArray )
moDefineDynamicArray( moFloatArray )
moDefineDynamicArray( moMathVariableArray )
moDefineDynamicArray( moMathFunctionArray )


moMathFunction* BuiltInMathFunctionFactory(const moText& p_expr)
{
	if (moText(p_expr) == moText("CubicInterpolant(x)")) return new moCubicInterpolant();
	if (moText(p_expr) == moText("TautInterpolant(x)")) return new moTautInterpolant();
	return NULL;
}

double* AddParserVariableFunction(const char *p_pVarName, void *p_pUserData)
{
    moMathVariableFactory *pFactory = (moMathVariableFactory*)p_pUserData;
    return pFactory->CreateNewVariable(p_pVarName);
}


moMathVariable::moMathVariable() {

    m_pParam = NULL;
    m_pInlet = NULL;
}


moMathVariable::moMathVariable( const char* p_name, double p_value0 ) {

    m_name = (char*)p_name;
    m_value = p_value0;
    m_pParam = NULL;
    m_pInlet = NULL;

}


moMathVariable::moMathVariable( moParam* p_Param  ) {

    SetParam( p_Param );

}

moMathVariable::moMathVariable( moInlet* p_Inlet  ) {

    SetInlet( p_Inlet );

}

void
moMathVariable::SetParam( moParam* p_Param ) {


    if (p_Param!=NULL) {

        m_pParam = p_Param;
        m_name = m_pParam->GetParamDefinition().GetName();
        GetValue();

    }
}

void
moMathVariable::SetInlet( moInlet* p_Inlet ) {


    if (p_Inlet!=NULL) {

        m_pInlet = p_Inlet;
        m_name = m_pInlet->GetConnectorLabelName();
        GetValue();

    }
}


double moMathVariable::GetValue()
{
    if (m_pParam) {
        moData* pData = m_pParam->GetData();
        if (pData)
            m_value = pData->Double();
    } else if (m_pInlet) {
        moData* pData = m_pInlet->GetData();
        if (pData)
            m_value = pData->Double();
    }

    return m_value;
}

double* moMathVariable::GetValuePointer() {

    GetValue();

    return &m_value;
}


/*******************************************************************************
                      Implementación de moMathFunction
*******************************************************************************/

moMathFunction::moMathFunction()
{
    m_Expression = moText("");
    m_EmptyName = moText("");
    m_pConfig = NULL;
    m_pMOB = NULL;
    m_LastEval = 0.0;
}

moMathFunction::~moMathFunction()
{
    Finish();
}

MOboolean moMathFunction::Init( const moText& p_Expression, moMoldeoObject* p_pMOB )
{
	SetExpression(p_Expression);
	BuildParamList();
	BuildVarList();

///ASSOCIATE VARIABLES WITH PARAMETERS....
    if (p_pMOB) {
        m_pMOB = p_pMOB;
        m_pConfig = m_pMOB->GetConfig();
    }

    if ( m_pConfig ) {
        moParams& Params( m_pConfig->GetParams() );
        for( MOuint i=0; i < m_Variables.Count(); i++ ) {

            moMathVariable* pVariable = m_Variables[i];

            for( MOuint p=0; p<Params.Count(); p++  ) {

                moParam& param( Params[p] );
                ///check for every parameters in config...
                if ( param.GetParamDefinition().GetName() == pVariable->GetName() ) {

                    ///assign pointer to variable!!!!
                    pVariable->SetParam( param.GetPtr() );

                }

            }

            for( MOuint m=0; m<m_pMOB->GetInlets()->Count(); m++ ) {

              moInlet* pInlet = m_pMOB->GetInlets()->Get(m);

              if ( pInlet->GetConnectorLabelName() == pVariable->GetName() ) {
                  pVariable->SetInlet( pInlet );
              }


            }

        }

    }

	return true;
}

MOboolean moMathFunction::Finish()
{
	return true;
}

void moMathFunction::SetParameters(double s, ...)
{
	int num = m_Parameters.Count();        // Number of parameters. Must be equal to the number of arguments.

	if (num>0) {
		va_list arguments;                     // A place to store the list of arguments

    if (m_Parameters[0])
      m_Parameters[0]->SetValue(s);

		va_start(arguments, s);           // Initializing arguments to store all values after s
		for (int i = 1; i < num; i++)
			if (m_Parameters[i] != NULL)
				m_Parameters[i]->SetValue(va_arg(arguments, double));
		va_end (arguments);               // Cleans up the list
	}
	OnParamUpdate();
}

double moMathFunction::Eval() {

    int num = m_Variables.Count();

    if (num>0) {
		for (int i = 0; i < num; i++) {
            if (m_Variables[i] != NULL) {
                /// Values are updated from params....
                m_Variables[i]->GetValue();
            }
        }
    }


    return OnFuncEval();
}


double moMathFunction::LastEval() {
    return m_LastEval;
}

double moMathFunction::Eval(double x, ...)
{
	int num = m_Variables.Count();    // Number of variables. Must be equal to the number of arguments.

	if (num>0) {
	  if (m_Variables[0])
      m_Variables[0]->SetValue(x);

		va_list arguments;                // A place to store the list of arguments
		va_start(arguments, x);           // Initializing arguments to store all values after x.
		for (int i = 1; i < num; i++)
			if (m_Variables[i] != NULL)
				m_Variables[i]->SetValue(va_arg(arguments, double));
		va_end (arguments);               // Cleans up the list
	}
    return OnFuncEval();
}

double moMathFunction::DEval(int n, ...)
{
	int num = m_Variables.Count();    // Number of variables. Must be equal to the number of arguments.

	if (num>0) {
		va_list arguments;                // A place to store the list of arguments

		va_start(arguments, n);           // Initializing arguments to store all values after n.
		for (int i = 0; i < num; i++)
			if (m_Variables[i] != NULL)
				m_Variables[i]->SetValue(va_arg(arguments, double));
		va_end (arguments);               // Cleans up the list
	}
	return OnDerivEval(n);
}

MOuint moMathFunction::GetParameterCount()
{
    return m_Parameters.Count();
}

moText& moMathFunction::GetParameterName(int i)
{
	if (m_Parameters[i] != NULL) return m_Parameters[i]->GetName();
	else return m_EmptyName;
}

double moMathFunction::GetParameterValue(int i)
{
	if (m_Parameters[i] != NULL) return m_Parameters[i]->GetValue();
	else return 0;
}

MOuint moMathFunction::GetVariableCount()
{
    return m_Variables.Count();
}

moText& moMathFunction::GetVariableName(int i)
{
	if (m_Variables[i] != NULL) return m_Variables[i]->GetName();
	else return m_EmptyName;
}

double moMathFunction::GetVariableValue(int i)
{
	if (m_Variables[i] != NULL) return m_Variables[i]->GetValue();
	else return 0;
}

double moMathFunction::operator () (double x, ...)
{
	int num = m_Variables.Count();    // Number of variables. Must be equal to the number of arguments.
	if (num>0) {
		va_list arguments;                // A place to store the list of arguments

		m_Variables[0]->SetValue(x);

		va_start(arguments, x);           // Initializing arguments to store all values after x.
		for (int i = 1; i < num; i++)
			if (m_Variables[i] != NULL)
				m_Variables[i]->SetValue(va_arg(arguments, double));
		va_end (arguments);               // Cleans up the list
	}
    return OnFuncEval();
}

/*******************************************************************************
                       Implementación de moCubicInterpolant
*******************************************************************************/

MOboolean moCubicInterpolant::Init(moText& p_Expression)
{
    if (p_Expression == moText("CubicInterpolant(x)"))
	{
		moMathFunction::Init(p_Expression);

		// Asignando parámetros por defecto:
		// x0 = 0.0, y0 = 1.0, d0 = 0.0
		// x1 = 1.0, y1 = 1.0, d1 = 1.0
        SetParameters(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);

		return true;
	}
	else return false;
}

void moCubicInterpolant::OnParamUpdate()
{
    double x0 = m_Parameters[0]->GetValue();
    double y0 = m_Parameters[1]->GetValue();
    double d0 = m_Parameters[2]->GetValue();

    double x1 = m_Parameters[3]->GetValue();
    double y1 = m_Parameters[4]->GetValue();
    double d1 = m_Parameters[5]->GetValue();

    double delta =  x1 - x0;
    double deriv = (y0 - y1) /(x0 - x1);

    C4 = (d0 + d1 - 2 * deriv) /(delta * delta);
    C3 = (deriv - d0) / delta - C4 * delta;
    C2 = d0;
    C1 = y0;
}

double moCubicInterpolant::OnFuncEval()
{
	// Aqui calculamos delta = x - x0:
	double delta = m_Variables[0]->GetValue() - m_Parameters[0]->GetValue();
    double sqr_delta = delta * delta;

    return C1 + C2 * delta + C3 * sqr_delta + C4 * sqr_delta * delta;
}

double moCubicInterpolant::OnDerivEval(int n)
{
    if (n == 0) return OnFuncEval();
	else if (n < 4)
	{
	    double delta;

		// Aqui calculamos delta = x - x0:
		if (n < 3) delta = m_Variables[0]->GetValue() - m_Parameters[0]->GetValue();
		else delta = 0; // La derivada de orden 3 es constante, por lo que no necesitamos 'delta'.

        if (n == 1) return C2 + 2 * C3 * delta + 3 * C4 * delta * delta;
	    else if (n == 2) return 2 * C3 + 6 * C4 * delta;
	    else return 6 * C4;
	}
	else return 0;
}

void moCubicInterpolant::BuildParamList()
{
    m_Parameters.Add(new moMathVariable("_x0"));
    m_Parameters.Add(new moMathVariable("_y0"));
    m_Parameters.Add(new moMathVariable("_d0"));
    m_Parameters.Add(new moMathVariable("_x1"));
    m_Parameters.Add(new moMathVariable("_y1"));
    m_Parameters.Add(new moMathVariable("_d1"));
}

void moCubicInterpolant::BuildVarList()
{
    m_Variables.Add(new moMathVariable("x"));
}

/*******************************************************************************
                       Implementación de moTautInterpolant
*******************************************************************************/

MOboolean moTautInterpolant::Init(moText& p_Expression)
{
    if (p_Expression == moText("TautInterpolant(x)"))
	{
		moMathFunction::Init(p_Expression);

		// Asignando parámetros por defecto:
		// x0 = 0.0, y0 = 1.0, d0 = 0.0
		// x1 = 1.0, y1 = 1.0, d1 = 1.0
		// gamma = 1.5
        SetParameters(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.5);

		return true;
	}
	else return false;
}

double moTautInterpolant::PhiFunc1(double u)
{
    double v = max(one_minus_knot_inv *(u - knot), 0.0);
    return alpha * u * u * u + (1 - alpha) * v * v * v;
}

double moTautInterpolant::DPhiFunc1(double u)
{
    double s, v, dv;
    s = one_minus_knot_inv *(u - knot);
    v = max(s, 0.0);
    if (0 <= s) dv = one_minus_knot_inv; else dv = 0;
    return 3 * (alpha * u * u + (1 - alpha) * v * v * dv);
}

double moTautInterpolant::PhiFunc2(double u)
{
    double v = max(one_minus_knot_inv2 *(u - knot2), 0.0);
    return alpha2 * u * u * u + (1 - alpha2) * v * v * v;
}

double moTautInterpolant::DPhiFunc2(double u)
{
    double s, v, dv;
    s = one_minus_knot_inv2 *(u - knot2);
    v = max(s, 0.0);
    if (0 <= s) dv = one_minus_knot_inv2; else dv = 0;
    return 3 * (alpha2 * u * u + (1 - alpha2) * v * v * dv);
}

void moTautInterpolant::OnParamUpdate()
{
    double x0 = m_Parameters[0]->GetValue();
    double y0 = m_Parameters[1]->GetValue();
    double Dy0 = m_Parameters[2]->GetValue();

    double x1 = m_Parameters[3]->GetValue();
    double y1 = m_Parameters[4]->GetValue();
    double Dy1 = m_Parameters[5]->GetValue();

	double gamma = m_Parameters[6]->GetValue();

    double diff, delta, s0, s1;
    double zeta2;
    double d1, d2, a, b;

    delta = x1 - x0; delta_inv = 1.0 / delta;

    diff = y1 - y0;

    s0 = Dy0 * delta - diff;
    s1 = Dy1 * delta - diff;

    // Parameters of Phi 1 function.
    zeta = s1 / (s1 - s0);
    knot = 1.0 - gamma * min(1.0 - zeta, 1.0 / 3.0);
    knot = max(1e-10, knot); knot = min(1.0 - 1e-10, knot); // Keeping the knot in(0, 1).
    one_minus_knot_inv = 1.0 /(1.0 - knot);
    alpha = (1.0 - gamma / 3.0) / knot;
    d1 = DPhiFunc1(1);

    // Parameters of Phi 2 function.
    zeta2 = 1.0 - zeta;
    knot2 = 1.0 - gamma * min(1.0 - zeta2, 1.0 / 3.0);
    knot2 = max(1e-10, knot2); knot2 = min(1.0 - 1e-10, knot2); // Keeping the knot in(0, 1).
    one_minus_knot_inv2 = 1.0 /(1.0 - knot2);
    alpha2 = (1.0 - gamma / 3.0) / knot2;
    d2 = DPhiFunc2(1);

    a = d1 - 1.0;
    b = 1.0 - d2;

    // Coefficients of the taut spline.
    if (a * b == -1)
    {
        // The coefficient D is indetermined. A interpolating spline exists only
        // if -a * s0 == s1.
        D = 0.0;
    }
    else D = (s1 + a * s0) / (a * b + 1);
    C =  b * D - s0;
    B = diff + D - C;
    A = y0 - D;
}

double moTautInterpolant::OnFuncEval()
{
    double u = delta_inv *(m_Variables[0]->GetValue() - m_Parameters[0]->GetValue());
    return A + B * u + C * PhiFunc1(u) + D * PhiFunc2(1 - u);
}

double moTautInterpolant::OnDerivEval(int n)
{
    if (n == 0) return OnFuncEval();
	else if (n < 2)
	{
        double u = delta_inv *(m_Variables[0]->GetValue() - m_Parameters[0]->GetValue());
        return (B + C * DPhiFunc1(u) - D * DPhiFunc2(1 - u)) * delta_inv;
	}
	else
		// Calculation of derivatives of order 2 and 3 is not implemented.
		return 0;
}

void moTautInterpolant::BuildParamList()
{
    m_Parameters.Add(new moMathVariable("_x0"));
    m_Parameters.Add(new moMathVariable("_y0"));
    m_Parameters.Add(new moMathVariable("_d0"));
    m_Parameters.Add(new moMathVariable("_x1"));
    m_Parameters.Add(new moMathVariable("_y1"));
    m_Parameters.Add(new moMathVariable("_d1"));
    m_Parameters.Add(new moMathVariable("_gamma"));
}

void moTautInterpolant::BuildVarList()
{
    m_Variables.Add(new moMathVariable("x"));
}

/*******************************************************************************
                       Implementación de moParserFunction
*******************************************************************************/

moParserFunction::moParserFunction() {

    m_pConfig = NULL;
    m_pMOB = NULL;

}


MOboolean moParserFunction::Init(const moText& p_Expression, moMoldeoObject* p_pMOB )
{
    mu::Parser* pParser = new mu::Parser();
    m_pParser = (moParser*) pParser;
    m_pMOB = p_pMOB;
    if (m_pMOB) m_pConfig = m_pMOB->GetConfig();

    moMathVariableFactory* pVarFactory = new moMathVariableFactory(&m_Parameters, &m_Variables);

    if (pParser) {
        pParser->SetVarFactory(AddParserVariableFunction, pVarFactory);

        AddMathFunctions();
        AddMathConstants();

        m_Expression = p_Expression;
        char* expr = (char*)(m_Expression);

        try {
            pParser->SetExpr(expr);
            pParser->Eval();
        }
        catch ( mu::ParserError Exc) {
            moText msgexpr = (char*)Exc.GetExpr().c_str();
            moText msgerror = (char*)Exc.GetMsg().c_str();
            MODebug2->Error( (moText)msgexpr + moText(":") + (moText)msgerror );
            delete pVarFactory;
            return false;
        }

        delete pVarFactory;

    }

    ///ASSOCIATE VARIABLES WITH PARAMETERS....

    if ( m_pConfig ) {
        moParams& Params( m_pConfig->GetParams() );
        for( MOuint i=0; i < m_Variables.Count(); i++ ) {

            moMathVariable* pVariable = m_Variables[i];

            for( MOuint p=0; p<Params.Count(); p++  ) {

                moParam& param( Params[p] );
                ///check for every parameters in config...
                if ( param.GetParamDefinition().GetName() == pVariable->GetName() ) {

                    ///assign pointer to variable!!!!
                    pVariable->SetParam( param.GetPtr() );

                }

            }

            for( MOuint m=0; m<m_pMOB->GetInlets()->Count(); m++ ) {

              moInlet* pInlet = m_pMOB->GetInlets()->Get(m);

              if ( pInlet->GetConnectorLabelName() == pVariable->GetName() ) {
                  pVariable->SetInlet( pInlet );
              }


            }

        }

    }

	return CheckVariables();
}

MOboolean moParserFunction::Finish()
{
    if (m_pParser) {
        mu::Parser* pParser = (mu::Parser*) m_pParser;
        delete pParser;
        m_pParser = NULL;
    }
	return true;
}

void moParserFunction::AddMathFunctions()
{
    mu::Parser* pParser = (mu::Parser*) m_pParser;
    if (pParser) {
        pParser->DefineFun("UnitRandom", moMathd::UnitRandom, false);
        pParser->DefineFun("SymmetricRandom", moMathd::SymmetricRandom, false);
        pParser->DefineFun("IntervalRandom", moMathd::IntervalRandom, false);

        pParser->DefineFun("FastSin0", moMathd::FastSin0, false);
        pParser->DefineFun("FastSin1", moMathd::FastSin1, false);

        pParser->DefineFun("FastCos0", moMathd::FastCos0, false);
        pParser->DefineFun("FastCos1", moMathd::FastCos1, false);

        pParser->DefineFun("FastTan0", moMathd::FastTan0, false);
        pParser->DefineFun("FastTan1", moMathd::FastTan1, false);

        pParser->DefineFun("FastInvSin0", moMathd::FastInvSin0, false);
        pParser->DefineFun("FastInvSin1", moMathd::FastInvSin1, false);

        pParser->DefineFun("FastInvCos0", moMathd::FastInvCos0, false);
        pParser->DefineFun("FastInvCos1", moMathd::FastInvCos1, false);

        pParser->DefineFun("FastInvTan0", moMathd::FastInvTan0, false);
        pParser->DefineFun("FastInvTan1", moMathd::FastInvTan1, false);

        pParser->DefineFun("FastInvSqrt", moMathd::FastInvSqrt, false);

        pParser->DefineFun("FastNegExp0", moMathd::FastNegExp0, false);
        pParser->DefineFun("FastNegExp1", moMathd::FastNegExp1, false);
        pParser->DefineFun("FastNegExp2", moMathd::FastNegExp2, false);
        pParser->DefineFun("FastNegExp3", moMathd::FastNegExp3, false);

        pParser->DefineFun("DegToRad", moMathd::DegToRad, false);
        pParser->DefineFun("RadToDeg", moMathd::RadToDeg, false);
    }
}

void moParserFunction::AddMathConstants()
{
    mu::Parser* pParser = (mu::Parser*) m_pParser;
    if (pParser) {
        pParser->DefineConst("EPSILON", moMathd::EPSILON);
        pParser->DefineConst("ZERO_TOLERANCE", moMathd::ZERO_TOLERANCE);
        pParser->DefineConst("MAX_REAL", moMathd::MAX_REAL);
        pParser->DefineConst("PI", moMathd::PI);
        pParser->DefineConst("TWO_PI", moMathd::TWO_PI);
        pParser->DefineConst("HALF_PI", moMathd::HALF_PI);
        pParser->DefineConst("INV_PI", moMathd::INV_PI);
        pParser->DefineConst("INV_TWO_PI", moMathd::INV_TWO_PI);
        pParser->DefineConst("DEG_TO_RAD", moMathd::DEG_TO_RAD);
        pParser->DefineConst("RAD_TO_DEG", moMathd::RAD_TO_DEG);
        pParser->DefineConst("LN_2", moMathd::LN_2);
        pParser->DefineConst("LN_10", moMathd::LN_10);
        pParser->DefineConst("INV_LN_2", moMathd::INV_LN_2);
        pParser->DefineConst("INV_LN_10", moMathd::INV_LN_10);
    }
}

MOboolean moParserFunction::CheckVariables()
{
    int iNumVar;
    int nvar, npar;
	nvar = npar = 0;

    mu::Parser* pParser = (mu::Parser*) m_pParser;

    if (pParser) {

        mu::varmap_type variables = pParser->GetVar();

        iNumVar = (int)variables.size();

        mu::varmap_type::const_iterator item = variables.begin();

        for (; item!=variables.end(); ++item)
        {
            if (item->first[0] == '_')
            {
                npar++;
            }
            else
            {
                nvar++;
            }
        }

        if (nvar != (MOint)m_Variables.Count())
        {
            MODebug2->Push("Error in number of parser variables.");
            return false;
        }

        if (npar != (MOint)m_Parameters.Count())
        {
            MODebug2->Push("Error in number of parser parameters.");
            return false;
        }
        return true;
    }
	return false;
}


double moParserFunction::OnFuncEval() {

    mu::Parser* pParser = (mu::Parser*) m_pParser;
    if (pParser) {

        try {
            m_LastEval = pParser->Eval();
        }
        catch ( mu::ParserError Exc) {
            moText msgexpr = (char*)Exc.GetExpr().c_str();
            moText msgerror = (char*)Exc.GetMsg().c_str();
            MODebug2->Error( (moText)msgexpr + moText(":") + (moText)msgerror );
            m_LastEval = 0.0;
        }

    }

    return m_LastEval;

}

