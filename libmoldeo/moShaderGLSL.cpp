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
    m_GeometryShader = 0;
    m_TesselationShader = 0;
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
    if (m_GeometryShader != 0)
    {
        glDetachObjectARB(m_ProgramObject, m_GeometryShader);
        m_GeometryShader = 0;
    }

    if (m_TesselationShader != 0)
    {
        glDetachObjectARB(m_ProgramObject, m_TesselationShader);
        m_TesselationShader = 0;
    }

    if(m_ProgramObject != 0)
    {
        glDeleteObjectARB(m_ProgramObject);
        m_ProgramObject = 0;
    }

    return moShader::Finish();
}

void moShaderGLSL::CreateVertShader(const moText& vert_source)
{
    m_ProgramObject = glCreateProgramObjectARB();
    compileVertShader(vert_source);
    linkProgram();
}

void moShaderGLSL::CreateFragShader(const moText& frag_source)
{
    m_ProgramObject = glCreateProgramObjectARB();
    compileFragShader(frag_source);
    linkProgram();
}

void moShaderGLSL::CreateGeomShader(const moText& geom_source)
{
    m_ProgramObject = glCreateProgramObjectARB();
    compileGeomShader(geom_source);
    linkProgram();
}

void moShaderGLSL::CreateTessShader(const moText& tess_source)
{
    m_ProgramObject = glCreateProgramObjectARB();
    compileTessShader(tess_source);
    linkProgram();
}

void moShaderGLSL::CreateShader( const moText& vert_source, const moText& frag_source)
{
    try {
        m_ProgramObject = glCreateProgramObjectARB();
        compileVertShader(vert_source);
        compileFragShader(frag_source);
        linkProgram();
    } catch(...) {
        cout << "error shader" << endl;
    }
}

void moShaderGLSL::CreateShader( const moText& vert_source, const moText& frag_source, const moText& geom_source) {
    try {
        m_ProgramObject = glCreateProgramObjectARB();
        compileVertShader(vert_source);
        compileFragShader(frag_source);
        compileGeomShader(geom_source);
        linkProgram();
    } catch(...) {
        cout << "error shader" << endl;
    }

}

void moShaderGLSL::CreateShader( const moText& vert_source, const moText& frag_source, const moText& geom_source,  const moText& tess_source) {
    try {
        m_ProgramObject = glCreateProgramObjectARB();
        compileVertShader(vert_source);
        compileFragShader(frag_source);
        compileGeomShader(geom_source);
        compileTessShader(tess_source);
        linkProgram();
    } catch(...) {
        cout << "error shader" << endl;
    }

}


void moShaderGLSL::LoadVertShader(const moText& vert_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    CreateVertShader(vert_source);
}

void moShaderGLSL::LoadFragShader(const moText& frag_filename)
{
    moText frag_source = LoadShaderSource(frag_filename);
    CreateFragShader(frag_source);
}

void moShaderGLSL::LoadGeomShader(const moText& geom_filename)
{
    moText geom_source = LoadShaderSource(geom_filename);
    CreateGeomShader(geom_source);
}

void moShaderGLSL::LoadTessShader(const moText& tess_filename)
{
    moText tess_source = LoadShaderSource(tess_filename);
    CreateTessShader(tess_source);
}


void moShaderGLSL::LoadShader(const moText& vert_filename, const moText& frag_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    moText frag_source = LoadShaderSource(frag_filename);
    CreateShader(vert_source, frag_source);
}


void moShaderGLSL::LoadShader(const moText& vert_filename, const moText& frag_filename, const moText& geom_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    moText frag_source = LoadShaderSource(frag_filename);
    moText geom_source = LoadShaderSource(geom_filename);
    CreateShader(vert_source, frag_source, geom_source);
}


void moShaderGLSL::LoadShader(const moText& vert_filename, const moText& frag_filename, const moText& geom_filename, const moText& tess_filename)
{
    moText vert_source = LoadShaderSource(vert_filename);
    moText frag_source = LoadShaderSource(frag_filename);
    moText geom_source = LoadShaderSource(geom_filename);
    moText tess_source = LoadShaderSource(tess_filename);
    CreateShader(vert_source, frag_source, geom_source, tess_source);
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

void moShaderGLSL::PrintGeomShaderLog()
{
    if (m_GeometryShader != 0) printInfoLog(m_GeometryShader);
}

void moShaderGLSL::PrintTessShaderLog()
{
    if (m_TesselationShader != 0) printInfoLog(m_TesselationShader);
}

GLint moShaderGLSL::GetUniformID(const moText& uName)
{
    //MODebug2->Message("uName:"+uName);
    return glGetUniformLocationARB(m_ProgramObject, uName);
}

GLint moShaderGLSL::GetAttribID(const moText& aName)
{
    return glGetAttribLocationARB(m_ProgramObject, aName);
}

void moShaderGLSL::compileVertShader(const moText& vert_source)
{
    m_VertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	const char *source = (const char *)vert_source;
	const char **psource = &source;
	cout << "Source:" << source << endl;
    glShaderSourceARB(m_VertexShader, 1, psource, NULL);
    glCompileShaderARB(m_VertexShader);
		cout << "m_VertexShader:" << m_VertexShader << endl;
    int IsCompiled_FS;
    int maxLength;
    GLchar *vertexInfoLog;

    glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &IsCompiled_FS);
	cout << "Compiled VS:" << IsCompiled_FS << endl;
	glGetShaderiv(m_VertexShader, GL_INFO_LOG_LENGTH, &maxLength);
	cout << " maxLength: " << maxLength << endl;

	/* The maxLength includes the NULL character */
	vertexInfoLog = new GLchar[maxLength];
	//vertexInfoLog = {'\0'};
	//GLchar infoLog[512] = {'\0'};

	//glGetShaderInfoLog(m_VertexShader, maxLength, &maxLength, vertexInfoLog);
	if(IsCompiled_FS == MO_FALSE && maxLength)
	{
		glGetShaderInfoLog(m_VertexShader, 512, &maxLength, vertexInfoLog);
		cout << " maxLength: " << maxLength << " infoLog: " << vertexInfoLog << endl;
    if (MODebug2 != NULL)
        MODebug2->Error(moText("Vertex Shader compile error:")
                      + "("+GetName()+")"
                      + moText(vertexInfoLog) );

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */

		//return;
	} else {
        MODebug2->Message(moText("Vertex Shader compile log:")
                      + "("+GetName()+")"
                      + moText(vertexInfoLog) );
	}
delete [] vertexInfoLog;

    glAttachObjectARB(m_ProgramObject, m_VertexShader);
}

void moShaderGLSL::compileFragShader(const moText& frag_source)
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

    if (MODebug2 != NULL)
      MODebug2->Error(moText("Fragment Shader compile error:")
                      + "("+GetName()+")"
                      + moText(fragmentInfoLog) );

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete [] fragmentInfoLog;
		//return;
	}

    glAttachObjectARB(m_ProgramObject, m_FragmentShader);



}

void moShaderGLSL::compileGeomShader(const moText& geom_source)
{
    m_GeometryShader = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
	const char *source = (const char *)geom_source;
	const char **psource = &source;
    glShaderSourceARB(m_GeometryShader, 1, psource, NULL);
    glCompileShaderARB(m_GeometryShader);

    int IsCompiled_GS;
    int maxLength;
    char *geometryInfoLog;

    glGetShaderiv(m_GeometryShader, GL_COMPILE_STATUS, &IsCompiled_GS);
	if(IsCompiled_GS == MO_FALSE)
	{
		glGetShaderiv(m_GeometryShader, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		geometryInfoLog = new char[maxLength];

		glGetShaderInfoLog(m_GeometryShader, maxLength, &maxLength, geometryInfoLog);

    if (MODebug2 != NULL)
      MODebug2->Error(moText("Geometry Shader compile error:")
                      + "("+GetName()+")"
                      + moText(geometryInfoLog) );

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete [] geometryInfoLog;
		//return;
	}

    glAttachObjectARB(m_ProgramObject, m_GeometryShader);



}

void moShaderGLSL::compileTessShader(const moText& tess_source)
{/*
  m_TesselationShader = glCreateShaderObjectARB(GL_TESSELATION_SHADER_ARB);
	const char *source = (const char *)tess_source;
	const char **psource = &source;
    glShaderSourceARB(m_TesselationShader, 1, psource, NULL);
    glCompileShaderARB(m_TesselationShader);

    int IsCompiled_TS;
    int maxLength;
    char *tesselationInfoLog;

    glGetShaderiv(m_TesselationShader, GL_COMPILE_STATUS, &IsCompiled_TS);
	if(IsCompiled_TS == MO_FALSE)
	{
		glGetShaderiv(m_TesselationShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		tesselationInfoLog = new char[maxLength];

		glGetShaderInfoLog(m_TesselationShader, maxLength, &maxLength, tesselationInfoLog);

    if (MODebug2 != NULL)
      MODebug2->Error(moText("Tesselation Shader compile error:")
                      + "("+GetName()+")"
                      + moText(tesselationInfoLog) );

		// Handle the error in an appropriate way such as displaying a message or writing to a log file.
		// In this simple program, we'll just leave
		delete [] tesselationInfoLog;
		//return;
	}

    glAttachObjectARB(m_ProgramObject, m_TesselationShader);
*/
}

void moShaderGLSL::linkProgram()
{
    glLinkProgramARB(m_ProgramObject);
    GLint progLinkSuccess;
    glGetObjectParameterivARB(m_ProgramObject, GL_OBJECT_LINK_STATUS_ARB, &progLinkSuccess);
    m_VertErrorCode = progLinkSuccess;
    m_FragErrorCode = progLinkSuccess;
    m_GeomErrorCode = progLinkSuccess;
    m_TessErrorCode = progLinkSuccess;
    if (!progLinkSuccess) {
      if (MODebug2 != NULL) {
        MODebug2->Error(moText("Shader program could not be linked: ") + this->GetName()
                                        + " Type:" + IntToStr(GetType()) );
        printInfoLog( m_ProgramObject );
      }
    }
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
        if (MODebug2 != NULL) MODebug2->Message(msg);
        free(infoLog);
    }
}
