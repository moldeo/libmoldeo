/*******************************************************************************

                             moTextureFilterIndex.h

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

#ifndef __MO_TEXTURE_FILTER_INDEX_H
#define __MO_TEXTURE_FILTER_INDEX_H

#include "moTypes.h"
#include "moAbstract.h"
#include "moConfig.h"

#include "moTextureFilter.h"

/**
 * Clase que encapsula una lista de punteros a objetos moTextureFilter. La utilidad de esta clase es que
 * permite definir un subconjunto de filtros de textura y accederlos de manera directa.
 */
class LIBMOLDEO_API moTextureFilterIndex : public moAbstract
{
public:
    /**
     * Constructor por defecto.
     */
	moTextureFilterIndex();
    /**
     * Destructor por defecto.
     */
	virtual ~moTextureFilterIndex();

/**
     * Método de inicialización.
     * @param p_param_idx índice en el objeto de configuración donde se encuentran los filtros.
     * @param moGLManager puntero al administrador OpenGL.
     * @param moFBManager puntero al administrador de Framebuffers.
     * @param moShaderManager puntero al administrador de shaders.
     * @param moTextureManager puntero al administrador de texturas.
     * @param moRenderManager puntero al administrador de render.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
    virtual MOboolean Init( moParam* p_param, moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman);

/**
     * Método de inicialización.
     * @param moGLManager puntero al administrador OpenGL.
     * @param moFBManager puntero al administrador de Framebuffers.
     * @param moShaderManager puntero al administrador de shaders.
     * @param moTextureManager puntero al administrador de texturas.
     * @param moRenderManager puntero al administrador de render.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman);

    /**
     * Método de inicialización.
     * @param p_cfg puntero al objeto de configuración de Moldeo donde se especifica la lista de filtros.
     * @param p_param_idx índice en el objeto de configuración donde se encuentran los filtros.
     * @param moGLManager puntero al administrador OpenGL.
     * @param moFBManager puntero al administrador de Framebuffers.
     * @param moShaderManager puntero al administrador de shaders.
     * @param moTextureManager puntero al administrador de texturas.
     * @param moRenderManager puntero al administrador de render.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */

	virtual MOboolean Init(moConfig* p_cfg, MOuint p_param_idx, moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman);
    /**
     * Método de inicialización.
     * @param p_filters_str lista de expresiones que definen los filtros a agregar.
     * @param moGLManager puntero al administrador OpenGL.
     * @param moFBManager puntero al administrador de Framebuffers.
     * @param moShaderManager puntero al administrador de shaders.
     * @param moTextureManager puntero al administrador de texturas.
     * @param moRenderManager puntero al administrador de render.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moTextArray* p_filters_str, moGLManager* p_glman, moFBManager* p_fbman, moShaderManager* p_shaman, moTextureManager* p_texman, moRenderManager* p_renderman);
    /**
     * Método de finalización.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish();

    /**
     * Agrega el filtro apuntado por p_filter.
     * @return índice del nuevo filtro.
     */
	MOint Add(moTextureFilter* p_filter);

    /**
     * Aplica el filtro p_idx sobre las texturas de orígen y escribiendo el resutlado en las texturas de destino.
     * @param p_idx índice del filtro a aplicar.
     * @param p_i cuadro a utilizar en las texturas de orígen (en el caso de que sean animadas).
     * @param p_fade constante de "fading" utilizada para mezclar el resultado del filtro.
     * @param p_params parámetros del filtro.
     */
	void Apply(MOuint p_idx, MOuint p_i, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL);
    /**
     * Aplica el filtro p_idx sobre las texturas de orígen y escribiendo el resutlado en las texturas de destino.
     * @param p_idx índice del filtro a aplicar.
     * @param p_cycle fracción del ciclo entre 0.0 y 1.0 a utilizar en las texturas de orígen (en el caso de que sean animadas).
     * @param p_fade constante de "fading" utilizada para mezclar el resultado del filtro.
     * @param p_params parámetros del filtro.
     */
	void Apply(MOuint p_idx, MOfloat p_cycle, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL);
    /**
     * Aplica el filtro p_idx sobre las texturas de orígen y escribiendo el resutlado en las texturas de destino.
     * @param p_idx índice del filtro a aplicar.
     * @param p_tempo tempo a utilizar en las texturas de orígen (en el caso de que sean animadas).
     * @param p_fade constante de "fading" utilizada para mezclar el resultado del filtro.
     * @param p_params parámetros del filtro.
     */
	void Apply(MOuint p_idx, moTempo *p_tempo, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL);

    /**
     * Aplica todos los filtros sobre las texturas de orígen y escribiendo el resutlado en las texturas de destino.
     * @param p_tempo tempo a utilizar en las texturas de orígen (en el caso de que sean animadas).
     * @param p_fade constante de "fading" utilizada para mezclar el resultado del filtro.
     * @param p_params parámetros del filtro.
     */
	void Apply(moTempo *p_tempo, MOfloat p_fade = 1.0, moTextFilterParam *p_params = NULL);

    /**
     * Verifica el índice p_idx.
     * @param p_idx índice de filtro.
     * @return true si el índice es válido, false en caso contrario.
     */
	MOboolean ValidIndex(MOuint p_idx);

    /**
     * Devuelve el número de filtros.
     * @return número de filtros.
     */
	MOint Count() { return m_filters_array.Count(); }
    /**
     * Devuelve un puntero al filtro con índice p_idx.
     * @param p_idx índice de filtro.
     * @return puntero al filtro requerido.
     */
	moTextureFilter* operator [](MOuint p_idx) { return m_filters_array[p_idx]; }
	moTextureFilter* Get(MOuint p_idx) { return m_filters_array[p_idx]; }

	MOuint LoadFilter(moValue* p_value);
	MOuint LoadFilters(moParam* p_param);
	MOuint LoadFilters(moConfig* p_cfg, MOuint p_param_idx);
	MOuint LoadFilters(moTextArray* p_filters_str);

    MOint TextureFilterExists( moValue* p_value );


    moText MakeTextureFilterLabelName( moValue* p_value );

protected:
	moGLManager* m_glman;
	moFBManager* m_fbman;
	moShaderManager* m_shaman;
	moTextureManager* m_texman;
	moRenderManager* m_renderman;

	moTextureFilterArray m_filters_array;

	MOint LoadShader(moText& name, moShader **pshader);
	MOint LoadSourceTexture(moText& name, moTextureArray& src_tex, MOboolean first_tex, MOuint& dest_width, MOuint& dest_height);
	MOint LoadDestTexResolution(const moText& name, MOuint& dest_width, MOuint& dest_height);
	MOint LoadDestTexture(const moText& name, moTextureArray& dest_tex, MOuint dest_width, MOuint dest_height);
	void SetupDestTexFBO();
};


#endif

