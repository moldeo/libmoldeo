/*******************************************************************************

                              moLunaClasses.cpp

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

#include "moLunaClasses.h"

// moLuaMath implementation ***************************************************

IMPLEMENT_SCRIPT_CLASS(moLuaMath)

// Bind member functions to LUA
DEFINE_SCRIPT_CLASS(moLuaMath)
SCRIPT_METHOD(moLuaMath, ACos),
SCRIPT_METHOD(moLuaMath, ASin),
SCRIPT_METHOD(moLuaMath, ATan),
SCRIPT_METHOD(moLuaMath, ATan2),
SCRIPT_METHOD(moLuaMath, Ceil),
SCRIPT_METHOD(moLuaMath, Cos),
SCRIPT_METHOD(moLuaMath, Exp),
SCRIPT_METHOD(moLuaMath, FAbs),
SCRIPT_METHOD(moLuaMath, Floor),
SCRIPT_METHOD(moLuaMath, FMod),
SCRIPT_METHOD(moLuaMath, InvSqrt),
SCRIPT_METHOD(moLuaMath, Log),
SCRIPT_METHOD(moLuaMath, Log2),
SCRIPT_METHOD(moLuaMath, Log10),
SCRIPT_METHOD(moLuaMath, Pow),
SCRIPT_METHOD(moLuaMath, Sin),
SCRIPT_METHOD(moLuaMath, Sqr),
SCRIPT_METHOD(moLuaMath, Sqrt),
SCRIPT_METHOD(moLuaMath, Tan),
SCRIPT_METHOD(moLuaMath, UnitRandom),
SCRIPT_METHOD(moLuaMath, SymmetricRandom),
SCRIPT_METHOD(moLuaMath, IntervalRandom),
{ 0, 0 }
END_SCRIPT_CLASS

SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaMath)
{
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, ACos)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::ACos(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, ASin)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::ASin(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, ATan)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::ATan(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, ATan2)
{
	MOdouble fY = (MOdouble) lua_tonumber (L, 1);
	MOdouble fX = (MOdouble) lua_tonumber (L, 2);
	MOdouble res = moMathd::ATan2(fY, fX);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Ceil)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Ceil(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Cos)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Cos(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Exp)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Exp(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, FAbs)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::FAbs(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Floor)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Floor(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, FMod)
{
	MOdouble fX = (MOdouble) lua_tonumber (L, 1);
	MOdouble fY = (MOdouble) lua_tonumber (L, 2);
	MOdouble res = moMathd::FMod(fX, fY);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, InvSqrt)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::InvSqrt(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Log)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Log(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Log2)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Log2(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Log10)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Log10(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Pow)
{
	MOdouble fBase = (MOdouble) lua_tonumber (L, 1);
	MOdouble fExponent = (MOdouble) lua_tonumber (L, 2);
	MOdouble res = moMathd::Pow(fBase, fExponent);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Sin)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Sin(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Sqr)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Sqr(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Sqrt)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Sqrt(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, Tan)
{
	MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moMathd::Tan(fValue);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, UnitRandom)
{
	MOuint uiSeed = (MOuint) lua_tonumber (L, 1);
	MOdouble rand = moMathd::UnitRandom((unsigned int)uiSeed);
	lua_pushnumber(L, (lua_Number)rand);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, SymmetricRandom)
{
	MOuint uiSeed = (MOuint) lua_tonumber (L, 1);
	MOdouble rand = moMathd::SymmetricRandom((unsigned int)uiSeed);
	lua_pushnumber(L, (lua_Number)rand);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaMath, IntervalRandom)
{
	MOdouble fMin = (MOdouble) lua_tonumber (L, 1);
	MOdouble fMax = (MOdouble) lua_tonumber (L, 2);
	MOuint uiSeed = (MOuint) lua_tonumber (L, 3);
	MOdouble rand = moMathd::IntervalRandom(fMin, fMax, (unsigned int)uiSeed);
	lua_pushnumber(L, (lua_Number)rand);
    return 1;
}

// moLuaParserFunction implementation *********************************************

IMPLEMENT_SCRIPT_CLASS(moLuaParserFunction)

// Bind member functions to LUA
DEFINE_SCRIPT_CLASS(moLuaParserFunction)
SCRIPT_METHOD(moLuaParserFunction, SetExpression),
SCRIPT_METHOD(moLuaParserFunction, SetParameters1),
SCRIPT_METHOD(moLuaParserFunction, SetParameters2),
SCRIPT_METHOD(moLuaParserFunction, SetParameters3),
SCRIPT_METHOD(moLuaParserFunction, Eval1),
SCRIPT_METHOD(moLuaParserFunction, Eval2),
SCRIPT_METHOD(moLuaParserFunction, Eval3),
SCRIPT_METHOD(moLuaParserFunction, GetParameterCount),
SCRIPT_METHOD(moLuaParserFunction, GetVariableCount),
{ 0, 0 }
END_SCRIPT_CLASS

SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaParserFunction) : moParserFunction()
{
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, SetExpression)
{
    char *expr = (char *) lua_tostring (L, 1);
	int res = moParserFunction::Init(moText(expr));
	lua_pushboolean(L, res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, SetParameters1)
{
	MOdouble par1 = (MOdouble) lua_tonumber (L, 1);
	moParserFunction::SetParameters(par1);
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, SetParameters2)
{
	MOdouble par1 = (MOdouble) lua_tonumber (L, 1);
	MOdouble par2 = (MOdouble) lua_tonumber (L, 2);
	moParserFunction::SetParameters(par1, par2);
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, SetParameters3)
{
	MOdouble par1 = (MOdouble) lua_tonumber (L, 1);
	MOdouble par2 = (MOdouble) lua_tonumber (L, 2);
	MOdouble par3 = (MOdouble) lua_tonumber (L, 3);
	moParserFunction::SetParameters(par1, par2, par3);
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, Eval1)
{
	MOdouble var1 = (MOdouble) lua_tonumber (L, 1);
	MOdouble res = moParserFunction::Eval(var1);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, Eval2)
{
	MOdouble var1 = (MOdouble) lua_tonumber (L, 1);
	MOdouble var2 = (MOdouble) lua_tonumber (L, 2);
	MOdouble res = moParserFunction::Eval(var1, var2);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, Eval3)
{
	MOdouble var1 = (MOdouble) lua_tonumber (L, 1);
	MOdouble var2 = (MOdouble) lua_tonumber (L, 2);
	MOdouble var3 = (MOdouble) lua_tonumber (L, 3);
	MOdouble res = moParserFunction::Eval(var1, var2, var3);
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, GetParameterCount)
{
	MOint res = moParserFunction::GetParameterCount();
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaParserFunction, GetVariableCount)
{
	MOint res = moParserFunction::GetVariableCount();
	lua_pushnumber(L, (lua_Number)res);
    return 1;
}

// moLuaMath implementation ***************************************************

IMPLEMENT_SCRIPT_CLASS(moLuaP5)

// Bind member functions to LUA
DEFINE_SCRIPT_CLASS(moLuaP5)

SCRIPT_METHOD(moLuaP5, triangle),
SCRIPT_METHOD(moLuaP5, line),
SCRIPT_METHOD(moLuaP5, arc),
SCRIPT_METHOD(moLuaP5, point),
SCRIPT_METHOD(moLuaP5, quad),
SCRIPT_METHOD(moLuaP5, ellipse),
SCRIPT_METHOD(moLuaP5, rect),

SCRIPT_METHOD(moLuaP5, strokeWeight),
SCRIPT_METHOD(moLuaP5, background),
SCRIPT_METHOD(moLuaP5, colorMode),

SCRIPT_METHOD(moLuaP5, stroke),
SCRIPT_METHOD(moLuaP5, noFill),
SCRIPT_METHOD(moLuaP5, noStroke),
SCRIPT_METHOD(moLuaP5, fill),

SCRIPT_METHOD(moLuaP5, pushMatrix),
SCRIPT_METHOD(moLuaP5, popMatrix),
SCRIPT_METHOD(moLuaP5, resetMatrix),

SCRIPT_METHOD(moLuaP5, scale),
SCRIPT_METHOD(moLuaP5, translate),
SCRIPT_METHOD(moLuaP5, rotate),

SCRIPT_METHOD(moLuaP5, PRGB),
SCRIPT_METHOD(moLuaP5, PHSB),

SCRIPT_METHOD(moLuaP5, PHALF_PI),
SCRIPT_METHOD(moLuaP5, PTWO_PI),
SCRIPT_METHOD(moLuaP5, PPI),

{ 0, 0 }
END_SCRIPT_CLASS

SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaP5) : moP5()
{
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, triangle)
{
	return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, line)
{
	//int res;

	MOfloat x1 = (MOfloat) lua_tonumber (L, 1);
	MOfloat y1 = (MOfloat) lua_tonumber (L, 2);

	MOfloat x2 = (MOfloat) lua_tonumber (L, 3);
	MOfloat y2 = (MOfloat) lua_tonumber (L, 4);

	moP5::line(x1, y1, x2, y2);

    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, arc)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, point)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, quad)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, ellipse)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, rect)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, strokeWeight)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, background)
{
	MOfloat grey = (MOfloat) lua_tonumber (L, 1);

	int type2 = (MOint) lua_type (L, 2);
	if (type2 == 3)
	{
		MOfloat alpha = (MOfloat) lua_tonumber (L, 2);
		moP5::background(grey, alpha);
	}
	else
		moP5::background(grey);

	return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, colorMode)
{
	MOint mode = (MOint) lua_tonumber (L, 1);
	moP5::colorMode(mode);

	return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, stroke)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, noFill)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, noStroke)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, fill)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, pushMatrix)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, popMatrix)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, resetMatrix)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, scale)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, translate)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, rotate)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, PRGB)
{
	lua_pushnumber(L, (lua_Number)MO_P5_RGB);
	return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, PHSB)
{
	lua_pushnumber(L, (lua_Number)MO_P5_HSB);
	return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, PHALF_PI)
{
	lua_pushnumber(L, (lua_Number)MO_P5_HALF_PI);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, PTWO_PI)
{
	lua_pushnumber(L, (lua_Number)MO_P5_TWO_PI);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, PPI)
{
	lua_pushnumber(L, (lua_Number)MO_P5_PI);
    return 1;
}
