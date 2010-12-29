/*******************************************************************************

                              moShaderCG.h

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

#ifndef __MO_SHADER_CG_H__
#define __MO_SHADER_CG_H__

#include "moShader.h"
#ifdef MO_MACOSX
    #include <cg.h>
    #include <cgGL.h>
#else
    #include <Cg/cg.h>
    #include <Cg/cgGL.h>
#endif
/**
 * Clase que encapsula un shader escrito en el lenguaje CG de NVidia.
 */
class LIBMOLDEO_API moShaderCG : public moShader
{
public:
    /**
     * El constructor por defecto de la clase.
     */
    moShaderCG();
    /**
     * El destructor por defecto de la clase.
     */
    ~moShaderCG();

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
    void CreateVertShader(moText vert_source);
    /**
     * Crea el shader de fragmentos a partir del c�digo fuente en frag_source.
     * @param frag_source c�digo fuente del shader de v�rtices.
     */
    void CreateFragShader(moText frag_source);
    /**
     * Crea los shaders de v�rtices y fragmentos a partir del c�digo fuente en vert_source y frag_source.
     * @param vert_source c�digo fuente del shader de v�rtices.
     * @param frag_source c�digo fuente del shader de v�rtices.
     */
    void CreateShader(moText vert_source, moText frag_source);
    /**
     * Crea el shader de v�rtices a partir del c�digo fuente en el archivo vert_filename.
     * @param vert_filename nombre del archivo con el c�digo fuente del shader de v�rtices.
     */
    void LoadVertShader(moText vert_filename);
    /**
     * Crea el shader de fragmentos a partir del c�digo fuente en el archivo frag_filename.
     * @param frag_filename nombre del archivo con el c�digo fuente del shader de fragmentos.
     */
    void LoadFragShader(moText frag_filename);
    /**
     * Crea los shaders de v�rtices y fragmentos a partir del c�digo fuente en los archivos vert_filename y
     * frag_filename.
     * @param vert_filename nombre del archivo con el c�digo fuente del shader de v�rtices.
     * @param frag_filename nombre del archivo con el c�digo fuente del shader de fragmentos.
     */
    void LoadShader(moText vert_filename, moText frag_filename);

    /**
     * Activa el shader.
     */
    void StartShader();
    /**
     * Desactiva el shader.
     */
    void StopShader();

    /**
     * Devuelve el par�metro CG con nombre pName en el shader de v�rtices.
     * @param pName nombre del par�metro.
     * @return par�metro CG.
     */
    CGparameter GetVertParameter(moText pName);
    /**
     * Devuelve el par�metro CG con nombre pName en el shader de fragmentos.
     * @param pName nombre del par�metro.
     * @return par�metro CG.
     */
    CGparameter GetFragParameter(moText pName);
protected:
    CGcontext m_cgContext;
    CGprogram m_cgFragmentShader;
    CGprogram m_cgVertexShader;
    CGprofile m_cgFragmentProfile;
    CGprofile m_cgVertexProfile;

    void compileVertShader(moText vert_source);
    void compileFragShader(moText frag_source);
};

#endif
