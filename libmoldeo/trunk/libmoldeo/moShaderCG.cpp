/*******************************************************************************

                             moShaderCG.cpp

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *   (at your option) any later version.                                    *
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

  Copyright (C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andr�s Colubri

  Description:
  Class for Cg shaders.

*******************************************************************************/

#include "moShaderCG.h"

moShaderCG::moShaderCG() : moShader()
{
	SetType(MO_SHADER_CG);

    m_cgContext = NULL;
    m_cgFragmentShader = NULL;
    m_cgVertexShader = NULL;
}

moShaderCG::~moShaderCG()
{
	Finish();
}

MOboolean moShaderCG::Init()
{
	return moShader::Init();
}

MOboolean moShaderCG::Finish()
{
    if (m_cgVertexShader != NULL)
    {
        cgDestroyProgram(m_cgVertexShader);
        m_cgVertexShader = NULL;
    }
    if (m_cgFragmentShader != NULL)
    {
        cgDestroyProgram(m_cgFragmentShader);
        m_cgFragmentShader = NULL;
    }

    if(m_cgContext != NULL)
    {
        cgDestroyContext(m_cgContext);
        m_cgContext = NULL;
    }

	return moShader::Finish();
}

void moShaderCG::CreateVertShader(moText vert_source)
{
    m_cgContext = cgCreateContext();
    compileVertShader(vert_source);
}

void moShaderCG::CreateFragShader(moText frag_source)
{
    m_cgContext = cgCreateContext();
    compileFragShader(frag_source);
}

void moShaderCG::CreateShader(moText vert_source, moText frag_source)
{
    m_cgContext = cgCreateContext();
    compileVertShader(vert_source);
    compileFragShader(frag_source);
}

void moShaderCG::LoadVertShader(moText vert_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    CreateVertShader(vert_source);
}

void moShaderCG::LoadFragShader(moText frag_filename)
{
    moText frag_source = LoadShaderSource(frag_filename);
    CreateFragShader(frag_source);
}

void moShaderCG::LoadShader(moText vert_filename, moText frag_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    moText frag_source = LoadShaderSource(frag_filename);
    CreateShader(vert_source, frag_source);
}

void moShaderCG::StartShader()
{
    moShader::StartShader();

    if (m_cgVertexShader != NULL)
    {
        cgGLBindProgram(m_cgVertexShader);
        cgGLEnableProfile(m_cgVertexProfile);
    }
    if (m_cgFragmentShader != NULL)
    {
        cgGLBindProgram(m_cgFragmentShader);
        cgGLEnableProfile(m_cgFragmentProfile);
    }
}

void moShaderCG::StopShader()
{
    if (m_cgVertexShader != NULL) cgGLDisableProfile(m_cgVertexProfile);
    if (m_cgFragmentShader != NULL) cgGLDisableProfile(m_cgFragmentProfile);

    moShader::StopShader();
}

CGparameter moShaderCG::GetVertParameter(moText pName)
{
    return cgGetNamedParameter(m_cgVertexShader, pName);
}

CGparameter moShaderCG::GetFragParameter(moText pName)
{
    return cgGetNamedParameter(m_cgFragmentShader, pName);
}

void moShaderCG::compileVertShader(moText vert_source)
{
    m_cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
    if (m_cgVertexProfile == CG_PROFILE_UNKNOWN)
    {
		if (MODebug != NULL) MODebug->Push(moText("Vertex profile not found"));
        m_VertErrorCode = -1;
        return;
    }
    cgGLSetOptimalOptions(m_cgVertexProfile);
    m_cgVertexShader = cgCreateProgram(m_cgContext, CG_SOURCE, vert_source, m_cgVertexProfile, NULL, NULL);
    if (m_cgVertexShader == NULL)
    {
        CGerror Error = cgGetError();
		if (MODebug != NULL)
		{
			MODebug->Push(moText("Cg error!"));
			MODebug->Push(moText("Vertex shader could not be created")+Error);
//			MODebug->Push(moText(cgGetErrorString(Error)));
//			MODebug->Push(moText(cgGetLastListing(m_cgContext)));
		}
        m_VertErrorCode = 1;
    }
    else
    {
        cgGLLoadProgram(m_cgVertexShader);
        m_VertErrorCode = 0;
    }
}

void moShaderCG::compileFragShader(moText frag_source)
{
    m_cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    if (m_cgFragmentProfile == CG_PROFILE_UNKNOWN)
    {
		MODebug->Push(moText("Fragment profile not found"));
        m_FragErrorCode = -1;
        return;
    }
    cgGLSetOptimalOptions(m_cgFragmentProfile);
    m_cgFragmentShader = cgCreateProgram(m_cgContext, CG_SOURCE, frag_source, m_cgFragmentProfile, NULL, NULL);
    if (m_cgFragmentShader == NULL)
    {
        CGerror Error = cgGetError();
		if (MODebug != NULL)
		{
			MODebug->Push(moText("Cg error!"));
			MODebug->Push(moText("Fragmemt shader could not be created")+Error);
//			MODebug->Push(moText(cgGetErrorString(Error)));
//			MODebug->Push(moText(cgGetLastListing(m_cgContext)));

		}
        m_FragErrorCode = 1;
    }
    else
    {
        cgGLLoadProgram(m_cgFragmentShader);
        m_FragErrorCode = 0;
    }
}
