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

#ifdef OPENGLESV2

#endif

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
     * M�todo de inicializaci�n.
     * @return true si la operaci�n fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init();
    /**
     * M�todo de finalizaci�n.
     * @return true si la operaci�n fue exitosa, false en caso contrario.
     */
    virtual MOboolean Finish();

    /**
     * Crea el shader de v�rtices a partir del c�digo fuente en vert_source.
     * @param vert_source c�digo fuente del shader de v�rtices.
     */
    virtual void CreateVertShader(const moText& vert_source);
    /**
     * Crea el shader de fragmentos a partir del c�digo fuente en frag_source.
     * @param frag_source c�digo fuente del shader de v�rtices.
     */
    virtual void CreateFragShader(const moText& frag_source);
     /**
     * Crea el shader de geometr�a a partir del c�digo fuente en geom_source.
     * @param geom_source c�digo fuente del shader de v�rtices.
     */
    virtual void CreateGeomShader(const moText& geom_source);
    /**
     * Crea el shader de teselaci�n a partir del c�digo fuente en tess_source.
     * @param geom_source c�digo fuente del shader de v�rtices.
     */
    virtual void CreateTessShader(const moText& tess_source);
    /**
     * Crea los shaders de v�rtices y fragmentos a partir del c�digo fuente en vert_source y frag_source.
     * @param vert_source c�digo fuente del shader de v�rtices.
     * @param frag_source c�digo fuente del shader de v�rtices.
     */
    virtual void CreateShader( const moText& vert_source, const moText& frag_source);
    virtual void CreateShader( const moText& vert_source, const moText& frag_source, const moText& geom_source);
    virtual void CreateShader( const moText& vert_source, const moText& frag_source, const moText& geom_source, const moText& tess_source);
    /**
     * Crea el shader de v�rtices a partir del c�digo fuente en el archivo vert_filename.
     * @param vert_filename nombre del archivo con el c�digo fuente del shader de v�rtices.
     */
    virtual void LoadVertShader( const moText& vert_filename);
    /**
     * Crea el shader de fragmentos a partir del c�digo fuente en el archivo frag_filename.
     * @param frag_filename nombre del archivo con el c�digo fuente del shader de fragmentos.
     */
    virtual void LoadFragShader(const moText& frag_filename);
    /**
     * Crea el shader de geometr�a a partir del c�digo fuente en el archivo geom_filename.
     * @param geom_filename nombre del archivo con el c�digo fuente del shader de geometr�a.
     */
    virtual void LoadGeomShader(const moText& geom_filename);
    /**
     * Crea el shader de teselaci�n a partir del c�digo fuente en el archivo tess_filename.
     * @param tess_filename nombre del archivo con el c�digo fuente del shader de teselaci�n.
     */
    virtual void LoadTessShader(const moText& tess_filename);
    /**
     * Crea los shaders de v�rtices y fragmentos a partir del c�digo fuente en los archivos vert_filename y
     * frag_filename.
     * @param vert_filename nombre del archivo con el c�digo fuente del shader de v�rtices.
     * @param frag_filename nombre del archivo con el c�digo fuente del shader de fragmentos.
     * @param geom_filename nombre del archivo con el c�digo fuente del shader de geometr�a.
     * @param tess_filename nombre del archivo con el c�digo fuente del shader de teselaci�n.
     */
    virtual void LoadShader( const moText& vert_filename, const moText& frag_filename);
    virtual void LoadShader(const moText& vert_filename, const moText& frag_filename, const moText& geom_filename);
    virtual void LoadShader(const moText& vert_filename, const moText& frag_filename, const moText& geom_filename, const moText& tess_filename);


    /**
     * Activa el shader.
     */
    virtual void StartShader();
    /**
     * Desactiva el shader.
     */
    virtual void StopShader();

    /**
     * Imprime los mensajes generados al compilar el shader de v�rtices.
     */
    void PrintVertShaderLog();
    /**
     * Imprime los mensajes generados al compilar el shader de fragmentos.
     */
    void PrintFragShaderLog();

    /**
     * Imprime los mensajes generados al compilar el shader de geometr�a.
     */
    void PrintGeomShaderLog();
    /**
     * Imprime los mensajes generados al compilar el shader de teselaci�n.
     */
    void PrintTessShaderLog();

    /**
     * Devuelve el identificador OpenGL del par�metro uniforme en el shader (puede ocurrir en el shader de
     * v�rtices o fragmentos
     * @param pName nombre del par�metro uniforme.
     * @return identificador OpenGL del par�metro.
     */
    GLint GetUniformID(const moText& uName);
    /**
     * Devuelve el identificador OpenGL del par�metro por atributo en el shader (puede ocurrir en el shader de
     * v�rtices o fragmentos
     * @param pName nombre del par�metro por atributo.
     * @return identificador OpenGL del par�metro.
     */
    GLint GetAttribID(const moText& aName);



protected:
    GLhandleARB m_ProgramObject;
    GLhandleARB m_FragmentShader;
    GLhandleARB m_VertexShader;
    GLhandleARB m_GeometryShader;
    GLhandleARB m_TesselationShader;

    void compileVertShader(const moText& vert_source);
    void compileFragShader(const moText& frag_source);
    void compileGeomShader(const moText& geom_source);
    void compileTessShader(const moText& tess_source);
    void linkProgram();

    void printInfoLog(GLhandleARB obj);
};

#endif

