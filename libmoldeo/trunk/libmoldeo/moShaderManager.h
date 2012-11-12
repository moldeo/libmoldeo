/*******************************************************************************

                                moShaderManager.h

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


*******************************************************************************/

#ifndef __MO_SHADER_MANAGER_H
#define __MO_SHADER_MANAGER_H

#include "moTypes.h"
#include "moAbstract.h"
#include "moText.h"
#include "moParam.h"
#include "moConfig.h"
#include "moShader.h"
#include "moShaderGLSL.h"
#include "moShaderCG.h"

#include "moResourceManager.h"
#include "moGLManager.h"
#include "moFBManager.h"


class moTextureFilterIndex;

/// Clase que implementa un administrador de shaders.
/**
 * Clase que implementa un administrador de shaders.
 */
class LIBMOLDEO_API moShaderManager : public moResource
{
public:
    /**
     * El constructor por defecto de la clase.
     */
	moShaderManager();

	/**
     * El destructor por defecto de la clase.
     */
	virtual ~moShaderManager() {}

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
     * Elimina todos los shaders cargados hasta el momento.
     */
	void Delete() { m_shaders_array.Empty(); }

    /**
     * Devuelve el índice del shader con nombre p_name. Si el shader no existe y p_create_shader es true,
     * entonces intenta crear un nuevo shader interpretando a p_name como el nombre del archivo de configuración
     * del mismo.
     * @param p_name nombre del shader a buscar.
     * @param p_create_shader si es true intenta crear el shader si el mismo no es encontrado.
     * @return índice del shader.
     */
	MOint GetShaderIndex(moText p_name, MOboolean p_create_shader);
    /**
     * Agrega un shader de tipo p_type (MO_SHADER_GLSL o MO_SHADER_CG) en blanco, es decir, sin vértices
     * de fragmentos o vértices asociados al mismo.
     * @param p_type especifica si el shader es CG o GLSL.
     * @param p_name nombre del shader a crear.
     * @return índice del nuevo shader.
     */
	MOint AddShader(MOuint p_type, moText p_name);
    /**
     * Agrega un shader especificado en el archivo de configuración p_filename. Este archivo contiene la siguiente
     * información: el tipo de shader (CG o GLSL), los archivos contieniendo el código de los shaders de vértices
     * y fragmentos, y la configuración de la grilla 2D utilizada para implementar deformaciones de texturas.
     * @return índice del nuevo shader.
     */
	MOint AddShader(moText p_filename);
    /**
     * Agrega un shader de tipo p_type (MO_SHADER_GLSL o MO_SHADER_CG), carga en el mismo los shaders de
     * vértices y fragmentos cuyo código está contenido en los archivos especificados, y asocia la grilla
     * pasada como parámetro.
     * @param p_type especifica si el shader es CG o GLSL.
     * @param p_name nombre del shader a crear.
     * @param p_vert_fn archivo con el código del shader de vértices.
     * @param p_vert_fn archivo con el código del shader de fragmentos.
     * @param p_tex_grid objeto de grilla 2D para deformación de texturas.
     * @return índice del nuevo shader.
     */
	MOint AddShader(MOuint p_type, moText p_name, moText p_vert_fn, moText p_frag_fn, moTexturedGrid p_tex_grid);

    /**
     * Elimina el shader con índice p_idx.
     * @param p_idx índice del shader a eliminar.
     * @return true si la operación fue exitosa, false en caso contrario.
     */
	MOboolean DeleteShader(MOint p_idx);

    /**
     * Devuelve el número de shaders.
     * @return número de shaders.
     */
	MOuint GetShaderCount() { return m_shaders_array.Count(); }
    /**
     * Devuelve un puntero al shader con índice p_idx.
     * @param p_idx índice del shader .
     * @return puntero al shader solicitado.
     */
	moShader* GetShader(MOuint p_idx) { return m_shaders_array[p_idx]; }

	moTextureFilterIndex*   GetTextureFilterIndex() {
	    return m_pTextureFilterIndex;
    }

private:
	moGLManager* m_glmanager;
	moFBManager* m_fbmanager;

    /**
    *  Objeto que referencia cada sombreador aplicadp
    *  Cada sombreador aplicado a una textura tiene diferentes
    *   texturas de entrada y
    *   texturas de salida
    *   parametros de aplicacion (!!!)
    *   parametro de alpha (mezcla de este sombreador)
    */
	moTextureFilterIndex*    m_pTextureFilterIndex;

	moShaderArray m_shaders_array;
};


#endif

