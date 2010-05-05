/*******************************************************************************

							   moLunaClasses.h

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
  Andrs Colubri

  Description:
  En este archivo se deben crear las clases de interface de LUA que encapsulan
  las clases de Moldeo para que sean accesibles desde los scripts.

*******************************************************************************/

#include "moLuna.h"
#include "moMath.h"
#include "moMathFunction.h"
#include "moP5.h"

#ifndef __MO_LUNA_CLASSES_H__
#define __MO_LUNA_CLASSES_H__

/**
 * Esta clase encapsula moMathd.
 * @see moMath
 */
class LIBMOLDEO_API moLuaMath:public moMathd
{
	DECLARE_SCRIPT_CLASS (moLuaMath)
		SCRIPT_CONSTRUCTOR_DECLARATION (moLuaMath)
		SCRIPT_FUNCTION_DECLARATION (ACos);
	SCRIPT_FUNCTION_DECLARATION (ASin);
	SCRIPT_FUNCTION_DECLARATION (ATan);
	SCRIPT_FUNCTION_DECLARATION (ATan2);
	SCRIPT_FUNCTION_DECLARATION (Ceil);
	SCRIPT_FUNCTION_DECLARATION (Cos);
	SCRIPT_FUNCTION_DECLARATION (Exp);
	SCRIPT_FUNCTION_DECLARATION (FAbs);
	SCRIPT_FUNCTION_DECLARATION (Floor);
	SCRIPT_FUNCTION_DECLARATION (FMod);
	SCRIPT_FUNCTION_DECLARATION (InvSqrt);
	SCRIPT_FUNCTION_DECLARATION (Log);
	SCRIPT_FUNCTION_DECLARATION (Log2);
	SCRIPT_FUNCTION_DECLARATION (Log10);
	SCRIPT_FUNCTION_DECLARATION (Pow);
	SCRIPT_FUNCTION_DECLARATION (Sin);
	SCRIPT_FUNCTION_DECLARATION (Sqr);
	SCRIPT_FUNCTION_DECLARATION (Sqrt);
	SCRIPT_FUNCTION_DECLARATION (Tan);
	SCRIPT_FUNCTION_DECLARATION (UnitRandom)
		SCRIPT_FUNCTION_DECLARATION (SymmetricRandom)
		SCRIPT_FUNCTION_DECLARATION (IntervalRandom)
};

/**
 * Esta clase encapsula moParserFunction.
 * @see moParserFunction
 */
class LIBMOLDEO_API moLuaParserFunction:public moParserFunction
{
	DECLARE_SCRIPT_CLASS (moLuaParserFunction)
		SCRIPT_CONSTRUCTOR_DECLARATION (moLuaParserFunction)
		SCRIPT_FUNCTION_DECLARATION (SetExpression)
		SCRIPT_FUNCTION_DECLARATION (SetParameters1)
		SCRIPT_FUNCTION_DECLARATION (SetParameters2)
		SCRIPT_FUNCTION_DECLARATION (SetParameters3)
		SCRIPT_FUNCTION_DECLARATION (Eval1)
		SCRIPT_FUNCTION_DECLARATION (Eval2)
		SCRIPT_FUNCTION_DECLARATION (Eval3)
		SCRIPT_FUNCTION_DECLARATION (GetParameterCount)
		SCRIPT_FUNCTION_DECLARATION (GetVariableCount)
};

/**
 * Esta clase encapsula moP5.
 * @see moP5
 */
class LIBMOLDEO_API moLuaP5:public moP5
{
	DECLARE_SCRIPT_CLASS (moLuaP5)
		SCRIPT_CONSTRUCTOR_DECLARATION (moLuaP5)
		SCRIPT_FUNCTION_DECLARATION (triangle)
		SCRIPT_FUNCTION_DECLARATION (line)
		SCRIPT_FUNCTION_DECLARATION (arc)
		SCRIPT_FUNCTION_DECLARATION (point)
		SCRIPT_FUNCTION_DECLARATION (quad)
		SCRIPT_FUNCTION_DECLARATION (ellipse)
		SCRIPT_FUNCTION_DECLARATION (rect)
		SCRIPT_FUNCTION_DECLARATION (strokeWeight)
		SCRIPT_FUNCTION_DECLARATION (background)
		SCRIPT_FUNCTION_DECLARATION (colorMode)
		SCRIPT_FUNCTION_DECLARATION (stroke)
		SCRIPT_FUNCTION_DECLARATION (noFill)
		SCRIPT_FUNCTION_DECLARATION (noStroke)
		SCRIPT_FUNCTION_DECLARATION (fill)
		SCRIPT_FUNCTION_DECLARATION (pushMatrix)
		SCRIPT_FUNCTION_DECLARATION (popMatrix)
		SCRIPT_FUNCTION_DECLARATION (resetMatrix)
		SCRIPT_FUNCTION_DECLARATION (scale)
		SCRIPT_FUNCTION_DECLARATION (translate)
		SCRIPT_FUNCTION_DECLARATION (rotate)
		SCRIPT_FUNCTION_DECLARATION (PRGB)
		SCRIPT_FUNCTION_DECLARATION (PHSB)
		SCRIPT_FUNCTION_DECLARATION (PHALF_PI)
		SCRIPT_FUNCTION_DECLARATION (PTWO_PI) SCRIPT_FUNCTION_DECLARATION (PPI)
};
#endif							 /*  */
