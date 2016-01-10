/*******************************************************************************

                             moShaderGLSL.h

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

#ifndef __MO_SHADER_GLSL_H__
#define __MO_SHADER_GLSL_H__

#include "moShader.h"

/**
 * Clase que encapsula un shader escrito en el lenguaje GLSL.
 */
class LIBMOLDEO_API moShaderGLSL : public moShader
{
public:
    /**
     * El constructor por defecto de la clase.
     */
    moShaderGLSL();
    /**
     * El destructor por defecto de la clase.
     */
	virtual ~moShaderGLSL();

    /**
     * Método de inicialización.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init();
    /**
     * Método de finalización.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
    virtual MOboolean Finish();

    /**
     * Crea el shader de vértices a partir del código fuente en vert_source.
     * @param vert_source código fuente del shader de vértices.
     */
    virtual void CreateVertShader(moText vert_source);
    /**
     * Crea el shader de fragmentos a partir del código fuente en frag_source.
     * @param frag_source código fuente del shader de vértices.
     */
    virtual void CreateFragShader(moText frag_source);
    /**
     * Crea los shaders de vértices y fragmentos a partir del código fuente en vert_source y frag_source.
     * @param vert_source código fuente del shader de vértices.
     * @param frag_source código fuente del shader de vértices.
     */
    virtual void CreateShader(moText vert_source, moText frag_source);
    /**
     * Crea el shader de vértices a partir del código fuente en el archivo vert_filename.
     * @param vert_filename nombre del archivo con el código fuente del shader de vértices.
     */
    virtual void LoadVertShader(moText vert_filename);
    /**
     * Crea el shader de fragmentos a partir del código fuente en el archivo frag_filename.
     * @param frag_filename nombre del archivo con el código fuente del shader de fragmentos.
     */
    virtual void LoadFragShader(moText frag_filename);
    /**
     * Crea los shaders de vértices y fragmentos a partir del código fuente en los archivos vert_filename y
     * frag_filename.
     * @param vert_filename nombre del archivo con el código fuente del shader de vértices.
     * @param frag_filename nombre del archivo con el código fuente del shader de fragmentos.
     */
    virtual void LoadShader(moText vert_filename, moText frag_filename);

    /**
     * Activa el shader.
     */
    virtual void StartShader();
    /**
     * Desactiva el shader.
     */
    virtual void StopShader();

    /**
     * Imprime los mensajes generados al compilar el shader de vértices.
     */
    void PrintVertShaderLog();
    /**
     * Imprime los mensajes generados al compilar el shader de fragmentos.
     */
    void PrintFragShaderLog();

    /**
     * Devuelve el identificador OpenGL del parámetro uniforme en el shader (puede ocurrir en el shader de
     * vértices o fragmentos
     * @param pName nombre del parámetro uniforme.
     * @return identificador OpenGL del parámetro.
     */
    GLint GetUniformID(moText uName);
    /**
     * Devuelve el identificador OpenGL del parámetro por atributo en el shader (puede ocurrir en el shader de
     * vértices o fragmentos
     * @param pName nombre del parámetro por atributo.
     * @return identificador OpenGL del parámetro.
     */
    GLint GetAttribID(moText aName);
protected:
    GLhandleARB m_ProgramObject;
    GLhandleARB m_FragmentShader;
    GLhandleARB m_VertexShader;

    void compileVertShader(moText vert_source);
    void compileFragShader(moText frag_source);
    void linkProgram();

    void printInfoLog(GLhandleARB obj);
};

#endif

