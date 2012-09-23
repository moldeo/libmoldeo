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

  referencias codigo:  http://dev.alt.textdrive.com/browser/lu/
  benchmarks: http://shootout.alioth.debian.org/gp4/benchmark.php?test=all&lang=luajit&lang2=lua


*******************************************************************************/

#include "moLunaClasses.h"

#define lua_pindexes( L ) ( lua_gettop(L) - 1 )
#define lua_pindex( index ) (index + 1)

/**


        moLuaSoundManager


*/

IMPLEMENT_SCRIPT_CLASS(moLuaSoundManager)


DEFINE_SCRIPT_CLASS_FUNCTIONS(moLuaSoundManager)
END_SCRIPT_CLASS_FUNCTIONS

DEFINE_SCRIPT_CLASS_PROPERTIES(moLuaSoundManager)
END_SCRIPT_CLASS_PROPERTIES


SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaSoundManager)
{
  m_pSoundMan = NULL;
}

void moLuaSoundManager::Set( moSoundManager* p_pSoundManager) {
  m_pSoundMan = p_pSoundManager;
}


SCRIPT_FUNCTION_IMPLEMENTATION(moLuaSoundManager, GetSoundCount )
{
  //MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
  //MOdouble res = moMathd::ACos(fValue);
  //lua_pushnumber(L, (lua_Number)res);
  int count = 0;
  if (m_pSoundMan)
    count = m_pSoundMan->GetSoundCount();
  lua_pushnumber(L, (lua_Number)count);
  return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaSoundManager, GetSound )
{
  int id = (int) lua_tonumber (L, lua_pindex(1) );
  moSound* pSound = NULL;
  if (m_pSoundMan)
     pSound = m_pSoundMan->GetSound(id);
  //if (pCirc)
  return 1;
}



/**


        moLuaCircularVideoBuffer


*/

IMPLEMENT_SCRIPT_CLASS(moLuaCircularVideoBuffer)

DEFINE_SCRIPT_CLASS_FUNCTIONS(moLuaCircularVideoBuffer)
      SCRIPT_FUNCTION( moLuaCircularVideoBuffer, StartRecording),
      SCRIPT_FUNCTION( moLuaCircularVideoBuffer, PauseRecording),
      SCRIPT_FUNCTION( moLuaCircularVideoBuffer, ContinueRecording),
      SCRIPT_FUNCTION( moLuaCircularVideoBuffer, StopRecording),
      SCRIPT_FUNCTION( moLuaCircularVideoBuffer, GetRecordPosition),
      SCRIPT_FUNCTION( moLuaCircularVideoBuffer, GetFrameCount),
      SCRIPT_FUNCTION( moLuaCircularVideoBuffer, IsRecording)
END_SCRIPT_CLASS_FUNCTIONS


DEFINE_SCRIPT_CLASS_PROPERTIES(moLuaCircularVideoBuffer)
END_SCRIPT_CLASS_PROPERTIES


SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaCircularVideoBuffer)
{
  m_pCircularVideoBuffer = NULL;
}

void moLuaCircularVideoBuffer::Set( moCircularVideoBuffer* p_pCircularVideoBuffer) {
  m_pCircularVideoBuffer = p_pCircularVideoBuffer;
}


SCRIPT_FUNCTION_IMPLEMENTATION(moLuaCircularVideoBuffer, StartRecording )
{
  int at_position = (int) lua_tonumber (L, lua_pindex(1) );
  MODebug2->Push("at_position:"+IntToStr(at_position));
  if (m_pCircularVideoBuffer)
    m_pCircularVideoBuffer->StartRecording(at_position);
  return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaCircularVideoBuffer, PauseRecording )
{
  if (m_pCircularVideoBuffer)
    m_pCircularVideoBuffer->PauseRecording();
  return 0;
}


SCRIPT_FUNCTION_IMPLEMENTATION(moLuaCircularVideoBuffer, ContinueRecording )
{
  if (m_pCircularVideoBuffer)
    m_pCircularVideoBuffer->ContinueRecording();
  return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaCircularVideoBuffer, StopRecording )
{
  if (m_pCircularVideoBuffer)
    m_pCircularVideoBuffer->StopRecording();
  return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaCircularVideoBuffer, GetRecordPosition )
{
  int recp = -1;
  if (m_pCircularVideoBuffer)
    recp = m_pCircularVideoBuffer->GetRecordPosition();
  lua_pushnumber(L, (lua_Number)recp);
  return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaCircularVideoBuffer, GetFrameCount )
{
  int fcount = 0;
  if (m_pCircularVideoBuffer)
    fcount = m_pCircularVideoBuffer->GetFrameCount();
  lua_pushnumber(L, (lua_Number)fcount);
  return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaCircularVideoBuffer, IsRecording )
{
  bool res = false;
  if (m_pCircularVideoBuffer)
    res = m_pCircularVideoBuffer->IsRecording();
  lua_pushboolean(L, res);
  return 1;
}
/**


        moLuaVideoManager


*/

IMPLEMENT_SCRIPT_CLASS(moLuaVideoManager)

DEFINE_SCRIPT_CLASS_FUNCTIONS(moLuaVideoManager)
    SCRIPT_FUNCTION( moLuaVideoManager, GetCircularVideoBufferCount ),
    SCRIPT_FUNCTION( moLuaVideoManager, GetCircularVideoBuffer )
END_SCRIPT_CLASS_FUNCTIONS

DEFINE_SCRIPT_CLASS_PROPERTIES(moLuaVideoManager)
END_SCRIPT_CLASS_PROPERTIES

SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaVideoManager)
{
  m_pVideoMan = NULL;
}

void moLuaVideoManager::Set( moVideoManager* p_pVideoManager) {
  m_pVideoMan = p_pVideoManager;
}


SCRIPT_FUNCTION_IMPLEMENTATION(moLuaVideoManager, GetCircularVideoBufferCount )
{
  //MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
  //MOdouble res = moMathd::ACos(fValue);
  //lua_pushnumber(L, (lua_Number)res);
  int count = m_pVideoMan->GetCircularVideoBufferCount();
  lua_pushnumber(L, (lua_Number)count);
  return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaVideoManager, GetCircularVideoBuffer )
{
  int id = (int) lua_tonumber (L, 1);

  moCircularVideoBuffer* pCirc = NULL;
  moLuaCircularVideoBuffer* pLuaCirc = NULL;

  if (m_pVideoMan) {
     pCirc = m_pVideoMan->GetCircularVideoBuffer(id);
     if (pCirc) {
        pLuaCirc = new moLuaCircularVideoBuffer(L);
        pLuaCirc->Set( pCirc );
        moLuna <moLuaCircularVideoBuffer>::createFromExisting( L, pLuaCirc);
     }
  }
  return 1;
}


/**


        moLuaResourceManager


*/

IMPLEMENT_SCRIPT_CLASS(moLuaResourceManager)

DEFINE_SCRIPT_CLASS_FUNCTIONS(moLuaResourceManager)
    SCRIPT_FUNCTION( moLuaResourceManager, GetResourceCount),
    SCRIPT_FUNCTION( moLuaResourceManager, GetResource),
    SCRIPT_FUNCTION( moLuaResourceManager, GetResourceIndex),
    SCRIPT_FUNCTION( moLuaResourceManager, GetResourceByType),
    SCRIPT_FUNCTION( moLuaResourceManager, GetResourceName),
    SCRIPT_FUNCTION( moLuaResourceManager, GetResourceLabelName),
    SCRIPT_FUNCTION( moLuaResourceManager, GetResourceType),

    SCRIPT_FUNCTION( moLuaResourceManager, GetTextureMan),
    SCRIPT_FUNCTION( moLuaResourceManager, GetVideoMan),
    SCRIPT_FUNCTION( moLuaResourceManager, GetSoundMan)
END_SCRIPT_CLASS_FUNCTIONS


DEFINE_SCRIPT_CLASS_PROPERTIES(moLuaResourceManager)
END_SCRIPT_CLASS_PROPERTIES


SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaResourceManager)
{
  m_pResourceManager = NULL;
}

void moLuaResourceManager::Set( moResourceManager* p_pResourceManager) {
  m_pResourceManager = p_pResourceManager;
}


SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetResourceCount)
{
  //MOdouble fValue = (MOdouble) lua_tonumber (L, 1);
  //MOdouble res = moMathd::ACos(fValue);
  //lua_pushnumber(L, (lua_Number)res);
  int count = m_pResourceManager->Resources().Count();
  lua_pushnumber(L, (lua_Number)count);
  return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetResource)
{
  int id = (int) lua_tonumber (L, 1);
  moResource* pResource = m_pResourceManager->GetResource(id);
  if (pResource)
    id = pResource->GetId();
  else
    id = -1;
  lua_pushnumber(L, (lua_Number)id);
  return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetResourceIndex)
{
   	moText labelname = (moText) lua_tostring (L, 1);
   	int id = -1;

    if (m_pResourceManager) {
      id = m_pResourceManager->GetResourceIndex(labelname);
    }

    lua_pushnumber(L, (lua_Number)id);
    return 1;
}


SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetResourceByType)
{
    int id;

    id = -1;

    if (lua_isnumber(L,1)) {
      int typei = (int) lua_tonumber(L, 1);
      ///chequear validez del indice...
    } else if (lua_isstring(L,1)) {
      moText typestr = (moText) lua_tostring (L, 1);
      ///transformar de str a moResourceType
    }

/*
    if (m_pResourceManager) {
      moResourceType  rtype;
      moResource* pResource;
      rtype = MO_RESOURCETYPE_DATA;

      if (pResource)
        id = pResource->GetId();
    }

     (m_pResourceManager && typei<MO_RESOURCETYPE_) ?
      pResource = m_pResourceManager->GetResourceByType((moResourceType)typei) : pResource=NULL;
*/

    lua_pushnumber(L, (lua_Number)id);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetResourceName)
{
    moText rname;
    int id = (int) lua_tonumber (L, 1);

    moResource* pResource = m_pResourceManager->GetResource(id);

    if (pResource)
      rname = pResource->GetName();
    else
      rname = "";

    lua_pushstring(L, rname);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetResourceLabelName)
{
    moText rlname;
    int id = (int) lua_tonumber (L, 1);

    moResource* pResource = m_pResourceManager->GetResource(id);

    if (pResource)
      rlname = pResource->GetLabelName();
    else
      rlname = "";

    lua_pushstring(L, rlname);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetResourceType)
{
    int id = (int) lua_tonumber (L, 1);
    moResourceType  rtype;

    moResource* pResource = m_pResourceManager->GetResource(id);

    if (pResource)
      rtype = pResource->GetResourceType();
    else
      rtype = MO_RESOURCETYPE_UNDEFINED;

    lua_pushnumber(L, (lua_Number)(int)rtype);
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetTextureMan)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetVideoMan)
{
    moLuaVideoManager* pLuaVideoMan = new moLuaVideoManager( L );
    if (pLuaVideoMan && m_pResourceManager) {
      pLuaVideoMan->Set( m_pResourceManager->GetVideoMan());
      moLuna<moLuaVideoManager>::createFromExisting( L, pLuaVideoMan );
    }
    return 1;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaResourceManager, GetSoundMan)
{
    moLuaSoundManager* pLuaSoundMan = new moLuaSoundManager( L );
    if (pLuaSoundMan && m_pResourceManager) {
      pLuaSoundMan->Set( m_pResourceManager->GetSoundMan());
      moLuna<moLuaSoundManager>::createFromExisting( L, pLuaSoundMan );
    }
    return 1;
}




/** moLuaMath implementation ***************************************************
*/
/*
IMPLEMENT_SCRIPT_CLASS(moLuaMath)

// Bind member functions to LUA
DEFINE_SCRIPT_CLASS_FUNCTIONS(moLuaMath)
    SCRIPT_FUNCTION(moLuaMath, ACos),
    SCRIPT_FUNCTION(moLuaMath, ASin),
    SCRIPT_FUNCTION(moLuaMath, ATan),
    SCRIPT_FUNCTION(moLuaMath, ATan2),
    SCRIPT_FUNCTION(moLuaMath, Ceil),
    SCRIPT_FUNCTION(moLuaMath, Cos),
    SCRIPT_FUNCTION(moLuaMath, Exp),
    SCRIPT_FUNCTION(moLuaMath, FAbs),
    SCRIPT_FUNCTION(moLuaMath, Floor),
    SCRIPT_FUNCTION(moLuaMath, FMod),
    SCRIPT_FUNCTION(moLuaMath, InvSqrt),
    SCRIPT_FUNCTION(moLuaMath, Log),
    SCRIPT_FUNCTION(moLuaMath, Log2),
    SCRIPT_FUNCTION(moLuaMath, Log10),
    SCRIPT_FUNCTION(moLuaMath, Pow),
    SCRIPT_FUNCTION(moLuaMath, Sin),
    SCRIPT_FUNCTION(moLuaMath, Sqr),
    SCRIPT_FUNCTION(moLuaMath, Sqrt),
    SCRIPT_FUNCTION(moLuaMath, Tan),
    SCRIPT_FUNCTION(moLuaMath, UnitRandom),
    SCRIPT_FUNCTION(moLuaMath, SymmetricRandom),
    SCRIPT_FUNCTION(moLuaMath, IntervalRandom)
END_SCRIPT_CLASS_FUNCTIONS


DEFINE_SCRIPT_CLASS_PROPERTIES(moLuaMath)
{0}
END_SCRIPT_CLASS_PROPERTIES


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
*/

/** moLuaParserFunction implementation **********************************************/
/*
IMPLEMENT_SCRIPT_CLASS(moLuaParserFunction)

// Bind member functions to LUA
DEFINE_SCRIPT_CLASS_FUNCTIONS(moLuaParserFunction)
SCRIPT_FUNCTION(moLuaParserFunction, SetExpression),
SCRIPT_FUNCTION(moLuaParserFunction, SetParameters1),
SCRIPT_FUNCTION(moLuaParserFunction, SetParameters2),
SCRIPT_FUNCTION(moLuaParserFunction, SetParameters3),
SCRIPT_FUNCTION(moLuaParserFunction, Eval1),
SCRIPT_FUNCTION(moLuaParserFunction, Eval2),
SCRIPT_FUNCTION(moLuaParserFunction, Eval3),
SCRIPT_FUNCTION(moLuaParserFunction, GetParameterCount),
SCRIPT_FUNCTION(moLuaParserFunction, GetVariableCount),
{ 0, 0 }
END_SCRIPT_CLASS_FUNCTIONS


DEFINE_SCRIPT_CLASS_PROPERTIES(moLuaParserFunction)
{0}
END_SCRIPT_CLASS_PROPERTIES


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
*/

/** moLuaP5 implementation ****************************************************/

IMPLEMENT_SCRIPT_CLASS(moLuaP5)

// Bind member functions to LUA
DEFINE_SCRIPT_CLASS_FUNCTIONS(moLuaP5)

SCRIPT_FUNCTION(moLuaP5, triangle),
SCRIPT_FUNCTION(moLuaP5, line),
SCRIPT_FUNCTION(moLuaP5, arc),
SCRIPT_FUNCTION(moLuaP5, point),
SCRIPT_FUNCTION(moLuaP5, quad),
SCRIPT_FUNCTION(moLuaP5, ellipse),
SCRIPT_FUNCTION(moLuaP5, rect),

SCRIPT_FUNCTION(moLuaP5, strokeWeight),
SCRIPT_FUNCTION(moLuaP5, background),
SCRIPT_FUNCTION(moLuaP5, colorMode),

SCRIPT_FUNCTION(moLuaP5, stroke),
SCRIPT_FUNCTION(moLuaP5, noFill),
SCRIPT_FUNCTION(moLuaP5, noStroke),
SCRIPT_FUNCTION(moLuaP5, fill),

SCRIPT_FUNCTION(moLuaP5, pushMatrix),
SCRIPT_FUNCTION(moLuaP5, popMatrix),
SCRIPT_FUNCTION(moLuaP5, resetMatrix),

SCRIPT_FUNCTION(moLuaP5, scale),
SCRIPT_FUNCTION(moLuaP5, translate),
SCRIPT_FUNCTION(moLuaP5, rotate),

SCRIPT_FUNCTION(moLuaP5, PRGB),
SCRIPT_FUNCTION(moLuaP5, PHSB),

SCRIPT_FUNCTION(moLuaP5, PHALF_PI),
SCRIPT_FUNCTION(moLuaP5, PTWO_PI),
SCRIPT_FUNCTION(moLuaP5, PPI),

{ 0, 0 }
END_SCRIPT_CLASS_FUNCTIONS

DEFINE_SCRIPT_CLASS_PROPERTIES(moLuaP5)
END_SCRIPT_CLASS_PROPERTIES

SCRIPT_CONSTRUCTOR_IMPLEMENTATION(moLuaP5) : moP5()
{
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, triangle)
{
	return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, line)
{
  int n = lua_pindexes(L);

  MOfloat x1,x2,y1,y2,z1,z2;

  if (n==4) {
    x1 = (MOfloat) lua_tonumber (L, lua_pindex(1));
    y1 = (MOfloat) lua_tonumber (L, lua_pindex(2));
    x2 = (MOfloat) lua_tonumber (L, lua_pindex(3));
    y2 = (MOfloat) lua_tonumber (L, lua_pindex(4));
    moP5::line(x1, y1, x2, y2);
  } else if (n==6) {
    x1 = (MOfloat) lua_tonumber (L, lua_pindex(1));
    y1 = (MOfloat) lua_tonumber (L, lua_pindex(2));
    z1 = (MOfloat) lua_tonumber (L, lua_pindex(3));

    x2 = (MOfloat) lua_tonumber (L, lua_pindex(4));
    y2 = (MOfloat) lua_tonumber (L, lua_pindex(5));
    z2 = (MOfloat) lua_tonumber (L, lua_pindex(6));
    moP5::line(x1, y1, x2, y2);
  }
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
  int n = lua_pindexes(L);

  MOfloat x1,x2,x3,x4,y1,y2,y3,y4;

  if (n==4) {
    x1 = (MOfloat) lua_tonumber (L, lua_pindex(1));
    y1 = (MOfloat) lua_tonumber (L, lua_pindex(2));
    x2 = (MOfloat) lua_tonumber (L, lua_pindex(3));
    y2 = (MOfloat) lua_tonumber (L, lua_pindex(4));
    x3 = (MOfloat) lua_tonumber (L, lua_pindex(5));
    y3 = (MOfloat) lua_tonumber (L, lua_pindex(6));
    x4 = (MOfloat) lua_tonumber (L, lua_pindex(7));
    y4 = (MOfloat) lua_tonumber (L, lua_pindex(8));
    moP5::quad(x1, y1, x2, y2, x3, y3, x4, y4);
  }

    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, ellipse)
{
    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION( moLuaP5, rect )
{

  int n = lua_pindexes(L);

  MOfloat x1,y1,width,height;

  if (n==4) {
    x1 = (MOfloat) lua_tonumber (L, lua_pindex(1));
    y1 = (MOfloat) lua_tonumber (L, lua_pindex(2));
    width = (MOfloat) lua_tonumber (L, lua_pindex(3));
    height = (MOfloat) lua_tonumber (L, lua_pindex(4));
    moP5::rect(x1, y1, width, height);
  }

    return 0;
}


SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, strokeWeight)
{
    int n = lua_pindexes(L);
    MOfloat width = (MOfloat) lua_tonumber (L, lua_pindex(1) );

    //MODebug2->Push( "strokeWeight > width:" + FloatToStr(width) );

    moP5::strokeWeight( width );

    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, background)
{
  int n = lua_pindexes(L);
  MOfloat r,g,b,grey;
  MOfloat alpha = 1.0;

  switch(n) {
    case 1:
      grey = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      alpha = (MOfloat) lua_tonumber (L, lua_pindex(2));
      moP5::background(grey);
      break;
    case 2:
      grey = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      moP5::background(grey, alpha);
      break;
    case 3:
      r = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      g = (MOfloat) lua_tonumber (L, lua_pindex(2) );
      b = (MOfloat) lua_tonumber (L, lua_pindex(3) );
      moP5::background( r, g, b );
      break;
    case 4:
      r = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      g = (MOfloat) lua_tonumber (L, lua_pindex(2) );
      b = (MOfloat) lua_tonumber (L, lua_pindex(3) );
      alpha = (MOfloat) lua_tonumber (L, lua_pindex(4));
      moP5::background( r, g, b, alpha );
      break;
    default:
      moP5::background( 0.2, 0.2, 0.2, 1.0 );
      break;
  }
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, colorMode)
{
	MOint mode = (MOint) lua_tonumber (L, 2);
	moP5::colorMode(mode);

	return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, stroke)
{
  int n = lua_pindexes(L);
  MOfloat r,g,b,grey;
  MOfloat alpha = 1.0;

  switch(n) {
    case 1:
      grey = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      alpha = (MOfloat) lua_tonumber (L, lua_pindex(2));
      moP5::stroke(grey);
      break;
    case 2:
      grey = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      moP5::stroke(grey, alpha);
      break;
    case 3:
      r = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      g = (MOfloat) lua_tonumber (L, lua_pindex(2) );
      b = (MOfloat) lua_tonumber (L, lua_pindex(3) );
      moP5::stroke( r, g, b );
      break;
    case 4:
      r = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      g = (MOfloat) lua_tonumber (L, lua_pindex(2) );
      b = (MOfloat) lua_tonumber (L, lua_pindex(3) );
      alpha = (MOfloat) lua_tonumber (L, lua_pindex(4));
      moP5::stroke( r, g, b, alpha );
      break;
    default:
      moP5::stroke( 0.8, 0.8, 0.8, 1.0 );
      break;
  }

    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, noFill)
{

    moP5::noFill();

    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, noStroke)
{

    moP5::noStroke();

    return 0;
}

SCRIPT_FUNCTION_IMPLEMENTATION(moLuaP5, fill)
{

  int n = lua_pindexes(L);
  MOfloat r,g,b,grey;
  MOfloat alpha = 1.0;

  switch(n) {
    case 1:
      grey = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      alpha = (MOfloat) lua_tonumber (L, lua_pindex(2));
      moP5::fill(grey);
      break;
    case 2:
      grey = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      moP5::fill(grey, alpha);
      break;
    case 3:
      r = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      g = (MOfloat) lua_tonumber (L, lua_pindex(2) );
      b = (MOfloat) lua_tonumber (L, lua_pindex(3) );
      moP5::fill( r, g, b );
      break;
    case 4:
      r = (MOfloat) lua_tonumber (L, lua_pindex(1) );
      g = (MOfloat) lua_tonumber (L, lua_pindex(2) );
      b = (MOfloat) lua_tonumber (L, lua_pindex(3) );
      alpha = (MOfloat) lua_tonumber (L, lua_pindex(4));
      moP5::fill( r, g, b, alpha );
      break;
    default:
      moP5::fill( 0.8, 0.8, 0.8, 1.0 );
      break;
  }

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
