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
  Andrés Colubri

  Description:
  En este archivo se deben crear las clases de interface de LUA que encapsulan
  las clases de Moldeo para que sean accesibles desde los scripts.

*******************************************************************************/

#include "moLuna.h"
#include "moMath.h"
#include "moMathFunction.h"
#include "moP5.h"
#include "moSoundManager.h"
#include "moVideoManager.h"
#include "moResourceManager.h"

#ifndef __MO_LUNA_CLASSES_H__
#define __MO_LUNA_CLASSES_H__

class LIBMOLDEO_API moLuaSoundManager : public moAbstract
{
	DECLARE_SCRIPT_CLASS(moLuaSoundManager)

	SCRIPT_CONSTRUCTOR_DECLARATION(moLuaSoundManager)
    ///TODO: Creacion dinamica de grafos de audio y video..a revisar
    ///SCRIPT_FUNCTION_DECLARATION(GetSoundGraphCount);
    ///SCRIPT_FUNCTION_DECLARATION(GetSoundGraph);
    ///SCRIPT_FUNCTION_DECLARATION(GetSoundFramework);
    ///SCRIPT_FUNCTION_DECLARATION(GetMicCount);
    ///SCRIPT_FUNCTION_DECLARATION(GetMic);
    SCRIPT_FUNCTION_DECLARATION(GetSoundCount);
    SCRIPT_FUNCTION_DECLARATION(GetSound);
    ///SCRIPT_FUNCTION_DECLARATION(GetSoundBufferCount);
    ///SCRIPT_FUNCTION_DECLARATION(GetSoundBuffer);
    ///SCRIPT_FUNCTION_DECLARATION(GetSoundSamplePathCount);
    ///SCRIPT_FUNCTION_DECLARATION(GetSoundSanmplePathBuffer);

    void Set( moSoundManager* p_pSoundMan);
  private:
    moSoundManager*  m_pSoundMan;

};

class LIBMOLDEO_API moLuaTextureManager : public moAbstract
{
	DECLARE_SCRIPT_CLASS(moLuaTextureManager)

	SCRIPT_CONSTRUCTOR_DECLARATION(moLuaTextureManager)

    SCRIPT_FUNCTION_DECLARATION(GetTextureCount);
    SCRIPT_FUNCTION_DECLARATION(GetTextureMOId);
    SCRIPT_FUNCTION_DECLARATION(GetTextureBuffer);
    SCRIPT_FUNCTION_DECLARATION(AddTexture);
    SCRIPT_FUNCTION_DECLARATION(DeleteTexture);
    SCRIPT_FUNCTION_DECLARATION(AddTextureBuffer);
    SCRIPT_FUNCTION_DECLARATION(DeleteTextureBuffer);
    SCRIPT_FUNCTION_DECLARATION(GetGLId);
    SCRIPT_FUNCTION_DECLARATION(ValidTexture);

    void Set( moTextureManager* p_pTextureMan);
  private:
    moTextureManager*  m_pTextureMan;

};

/**
  Instancia de la clase moCircularVideoBuffer para Lua
*/

class moLuaCircularVideoBuffer : public moCircularVideoBuffer
{
  DECLARE_SCRIPT_CLASS(moLuaCircularVideoBuffer)

    SCRIPT_CONSTRUCTOR_DECLARATION(moLuaCircularVideoBuffer)
      SCRIPT_FUNCTION_DECLARATION(StartRecording);
      SCRIPT_FUNCTION_DECLARATION(PauseRecording);
      SCRIPT_FUNCTION_DECLARATION(ContinueRecording);
      SCRIPT_FUNCTION_DECLARATION(StopRecording);
      SCRIPT_FUNCTION_DECLARATION(GetRecordPosition);
      SCRIPT_FUNCTION_DECLARATION(GetFrameCount);
      SCRIPT_FUNCTION_DECLARATION(IsRecording);

    void Set( moCircularVideoBuffer* p_pCircularVideoBuffer );

    private:
      moCircularVideoBuffer* m_pCircularVideoBuffer;
};

/**
  Instancia de la clase moVideoBuffer para Lua
*/

class moLuaVideoBuffer : public moVideoBuffer
{
  DECLARE_SCRIPT_CLASS(moLuaVideoBuffer)

    SCRIPT_CONSTRUCTOR_DECLARATION(moLuaVideoBuffer)
      SCRIPT_FUNCTION_DECLARATION(GetFrameCount);

    void Set( moVideoBuffer* p_pVideoBuffer );

    private:
      moVideoBuffer* m_pVideoBuffer;
};

/**
  Instancia de la clase moVideoBufferPath para Lua
*/
class moLuaVideoBufferPath : public moVideoBufferPath
{
  DECLARE_SCRIPT_CLASS(moLuaVideoBufferPath)

    SCRIPT_CONSTRUCTOR_DECLARATION(moLuaVideoBufferPath)
      SCRIPT_FUNCTION_DECLARATION(GetPath);
      SCRIPT_FUNCTION_DECLARATION(GetCompletePath);
      SCRIPT_FUNCTION_DECLARATION(GetTotalFiles);
      SCRIPT_FUNCTION_DECLARATION(GetImagesProcessed);
      SCRIPT_FUNCTION_DECLARATION(LoadCompleted);

    void Set( moVideoBufferPath* p_pVideoBufferPath );

    private:
      moVideoBufferPath* m_pVideoBufferPath;
};

class LIBMOLDEO_API moLuaVideoManager : public moAbstract
{
	DECLARE_SCRIPT_CLASS(moLuaVideoManager)

	SCRIPT_CONSTRUCTOR_DECLARATION(moLuaVideoManager)
    ///TODO: Creacion dinamica de grafos de audio y video..a revisar
    ///SCRIPT_FUNCTION_DECLARATION(GetVideoGraphCount);
    ///SCRIPT_FUNCTION_DECLARATION(GetVideoGraph);
    //SCRIPT_FUNCTION_DECLARATION(GetVideoFramework);
    //SCRIPT_FUNCTION_DECLARATION(GetCameraCount);
    //SCRIPT_FUNCTION_DECLARATION(GetCamera);
    SCRIPT_FUNCTION_DECLARATION(GetVideoBufferCount);
    SCRIPT_FUNCTION_DECLARATION(GetVideoBuffer);
    SCRIPT_FUNCTION_DECLARATION(GetCircularVideoBufferCount);
    SCRIPT_FUNCTION_DECLARATION(GetCircularVideoBuffer);
    SCRIPT_FUNCTION_DECLARATION(GetVideoBufferPathCount);
    SCRIPT_FUNCTION_DECLARATION(GetVideoBufferPath);

    void Set( moVideoManager* p_pVideoMan);
  private:
    moVideoManager*  m_pVideoMan;

};



class LIBMOLDEO_API moLuaResourceManager : public moAbstract
{
	DECLARE_SCRIPT_CLASS(moLuaResourceManager)

	SCRIPT_CONSTRUCTOR_DECLARATION(moLuaResourceManager)

    SCRIPT_FUNCTION_DECLARATION(GetResourceCount);
    SCRIPT_FUNCTION_DECLARATION(GetResource);
    SCRIPT_FUNCTION_DECLARATION(GetResourceIndex);
    SCRIPT_FUNCTION_DECLARATION(GetResourceByType);
    SCRIPT_FUNCTION_DECLARATION(GetResourceName);
    SCRIPT_FUNCTION_DECLARATION(GetResourceLabelName);
    SCRIPT_FUNCTION_DECLARATION(GetResourceType);

    SCRIPT_FUNCTION_DECLARATION(GetTextureMan);
    SCRIPT_FUNCTION_DECLARATION(GetVideoMan);
    SCRIPT_FUNCTION_DECLARATION(GetSoundMan);

    void Set( moResourceManager* p_pResourceManager);
  private:
    moResourceManager*  m_pResourceManager;

};


/**
 * Esta clase encapsula moMathd.
 * @see moMath
 */
 /*
class LIBMOLDEO_API moLuaMath : public moMathd
{
	DECLARE_SCRIPT_CLASS(moLuaMath)

	SCRIPT_CONSTRUCTOR_DECLARATION(moLuaMath)

    SCRIPT_FUNCTION_DECLARATION(ACos);
    SCRIPT_FUNCTION_DECLARATION(ASin);
    SCRIPT_FUNCTION_DECLARATION(ATan);
    SCRIPT_FUNCTION_DECLARATION(ATan2);
    SCRIPT_FUNCTION_DECLARATION(Ceil);
    SCRIPT_FUNCTION_DECLARATION(Cos);
    SCRIPT_FUNCTION_DECLARATION(Exp);
    SCRIPT_FUNCTION_DECLARATION(FAbs);
    SCRIPT_FUNCTION_DECLARATION(Floor);
    SCRIPT_FUNCTION_DECLARATION(FMod);
    SCRIPT_FUNCTION_DECLARATION(InvSqrt);
    SCRIPT_FUNCTION_DECLARATION(Log);
    SCRIPT_FUNCTION_DECLARATION(Log2);
    SCRIPT_FUNCTION_DECLARATION(Log10);
    SCRIPT_FUNCTION_DECLARATION(Pow);
    SCRIPT_FUNCTION_DECLARATION(Sin);
    SCRIPT_FUNCTION_DECLARATION(Sqr);
    SCRIPT_FUNCTION_DECLARATION(Sqrt);
    SCRIPT_FUNCTION_DECLARATION(Tan);

	SCRIPT_FUNCTION_DECLARATION(UnitRandom)
	SCRIPT_FUNCTION_DECLARATION(SymmetricRandom)
	SCRIPT_FUNCTION_DECLARATION(IntervalRandom)
};
*/

/**
 * Esta clase encapsula moParserFunction.
 * @see moParserFunction
 */
/*
class LIBMOLDEO_API moLuaParserFunction : public moParserFunction
{
	DECLARE_SCRIPT_CLASS(moLuaParserFunction)

	SCRIPT_CONSTRUCTOR_DECLARATION(moLuaParserFunction)

	SCRIPT_FUNCTION_DECLARATION(SetExpression)

	SCRIPT_FUNCTION_DECLARATION(SetParameters1)
	SCRIPT_FUNCTION_DECLARATION(SetParameters2)
	SCRIPT_FUNCTION_DECLARATION(SetParameters3)

	SCRIPT_FUNCTION_DECLARATION(Eval1)
	SCRIPT_FUNCTION_DECLARATION(Eval2)
	SCRIPT_FUNCTION_DECLARATION(Eval3)

	SCRIPT_FUNCTION_DECLARATION(GetParameterCount)
    SCRIPT_FUNCTION_DECLARATION(GetVariableCount)
};
*/

/**
 * Esta clase encapsula moP5.
 * @see moP5
 */
class LIBMOLDEO_API moLuaP5 : public moP5
{
	DECLARE_SCRIPT_CLASS(moLuaP5)

	SCRIPT_CONSTRUCTOR_DECLARATION(moLuaP5)

    SCRIPT_FUNCTION_DECLARATION(triangle)
    SCRIPT_FUNCTION_DECLARATION(line)
    SCRIPT_FUNCTION_DECLARATION(arc)
	SCRIPT_FUNCTION_DECLARATION(point)
	SCRIPT_FUNCTION_DECLARATION(quad)
	SCRIPT_FUNCTION_DECLARATION(ellipse)
	SCRIPT_FUNCTION_DECLARATION(rect)

	SCRIPT_FUNCTION_DECLARATION(strokeWeight)
	SCRIPT_FUNCTION_DECLARATION(background)
	SCRIPT_FUNCTION_DECLARATION(colorMode)

	SCRIPT_FUNCTION_DECLARATION(stroke)
	SCRIPT_FUNCTION_DECLARATION(noFill)
	SCRIPT_FUNCTION_DECLARATION(noStroke)
	SCRIPT_FUNCTION_DECLARATION(fill)

	SCRIPT_FUNCTION_DECLARATION(pushMatrix)
	SCRIPT_FUNCTION_DECLARATION(popMatrix)
	SCRIPT_FUNCTION_DECLARATION(resetMatrix)

	SCRIPT_FUNCTION_DECLARATION(scale)
	SCRIPT_FUNCTION_DECLARATION(translate)
	SCRIPT_FUNCTION_DECLARATION(rotate)

	SCRIPT_FUNCTION_DECLARATION(PRGB)
	SCRIPT_FUNCTION_DECLARATION(PHSB)

	SCRIPT_FUNCTION_DECLARATION(PHALF_PI)
	SCRIPT_FUNCTION_DECLARATION(PTWO_PI)
	SCRIPT_FUNCTION_DECLARATION(PPI)
};

#endif

