/*******************************************************************************

                             moShaderGLSL.cpp

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


  Description:
  Class for GLSL shaders.

*******************************************************************************/

#include "moShaderGLSL.h"

moShaderGLSL::moShaderGLSL() : moShader()
{
	SetType(MO_SHADER_GLSL);

    m_VertexShader = 0;
    m_FragmentShader = 0;
    m_ProgramObject = 0;
}

moShaderGLSL::~moShaderGLSL()
{
	Finish();
}

MOboolean moShaderGLSL::Init()
{
	return moShader::Init();
}

MOboolean moShaderGLSL::Finish()
{
    if (m_VertexShader != 0)
    {
        glDetachObjectARB(m_ProgramObject, m_VertexShader);
        m_VertexShader = 0;
    }
    if (m_FragmentShader != 0)
    {
        glDetachObjectARB(m_ProgramObject, m_FragmentShader);
        m_FragmentShader = 0;
    }

    if(m_ProgramObject != 0)
    {
        glDeleteObjectARB(m_ProgramObject);
        m_ProgramObject = 0;
    }

    return moShader::Finish();
}

void moShaderGLSL::CreateVertShader(moText vert_source)
{
    m_ProgramObject = glCreateProgramObjectARB();
    compileVertShader(vert_source);
    linkProgram();
}

void moShaderGLSL::CreateFragShader(moText frag_source)
{
    m_ProgramObject = glCreateProgramObjectARB();
    compileFragShader(frag_source);
    linkProgram();
}

void moShaderGLSL::CreateShader(moText vert_source, moText frag_source)
{
    m_ProgramObject = glCreateProgramObjectARB();
    compileVertShader(vert_source);
    compileFragShader(frag_source);
    linkProgram();
}

void moShaderGLSL::LoadVertShader(moText vert_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    CreateVertShader(vert_source);
}

void moShaderGLSL::LoadFragShader(moText frag_filename)
{
    moText frag_source = LoadShaderSource(frag_filename);
    CreateFragShader(frag_source);
}

void moShaderGLSL::LoadShader(moText vert_filename, moText frag_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    moText frag_source = LoadShaderSource(frag_filename);
    CreateShader(vert_source, frag_source);
}

void moShaderGLSL::StartShader()
{
    moShader::StartShader();
    glUseProgramObjectARB(m_ProgramObject);
}

void moShaderGLSL::StopShader()
{
    glUseProgramObjectARB(0);
    moShader::StopShader();
}

void moShaderGLSL::PrintVertShaderLog()
{
    if (m_VertexShader != 0) printInfoLog(m_VertexShader);
}

void moShaderGLSL::PrintFragShaderLog()
{
    if (m_FragmentShader != 0) printInfoLog(m_FragmentShader);
}

GLint moShaderGLSL::GetUniformID(moText uName)
{
    return glGetUniformLocationARB(m_ProgramObject, uName);
}

GLint moShaderGLSL::GetAttribID(moText aName)
{
    return glGetAttribLocationARB(m_ProgramObject, aName);
}

void moShaderGLSL::compileVertShader(moText vert_source)
{
    m_VertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	const char *source = (const char *)vert_source;
	const char **psource = &source;
    glShaderSourceARB(m_VertexShader, 1, psource, NULL);
    glCompileShaderARB(m_VertexShader);
    glAttachObjectARB(m_ProgramObject, m_VertexShader);
}

void moShaderGLSL::compileFragShader(moText frag_source)
{
    m_FragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	const char *source = (const char *)frag_source;
	const char **psource = &source;
    glShaderSourceARB(m_FragmentShader, 1, psource, NULL);
    glCompileShaderARB(m_FragmentShader);

    int IsCompiled_FS;
    int maxLength;
    char *fragmentInfoLog;

    glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if(IsCompiled_FS == MO_FALSE)
	{
		glGetShaderiv(m_FragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		fragmentInfoLog = new char[maxLength];

		glGetShaderInfoLog(m_FragmentShader, maxLength, &maxLength, fragmentInfoLog);

        if (MODebug2 != NULL) MODebug2->Error(moText("Shader compile error:") + moText(fragmentInfoLog) );

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete [] fragmentInfoLog;
		//return;
	}

    glAttachObjectARB(m_ProgramObject, m_FragmentShader);



}

void moShaderGLSL::linkProgram()
{
    glLinkProgramARB(m_ProgramObject);
    GLint progLinkSuccess;
    glGetObjectParameterivARB(m_ProgramObject, GL_OBJECT_LINK_STATUS_ARB, &progLinkSuccess);
    m_VertErrorCode = progLinkSuccess;
    m_FragErrorCode = progLinkSuccess;
    if (!progLinkSuccess)
		if (MODebug2 != NULL) MODebug2->Error(moText("Shader program could not be linked"));
}

void moShaderGLSL::printInfoLog(GLhandleARB obj)
{
    GLint infologLength = 0;
    GLsizei charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		moText msg = moText(infoLog);
        if (MODebug2 != NULL) MODebug2->Push(msg);
        free(infoLog);
    }
}

