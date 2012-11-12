/*******************************************************************************

                                moTextureIndex.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                    *
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

#ifndef __MO_TEXTURE_INDEX_H
#define __MO_TEXTURE_INDEX_H

#include "moTypes.h"
#include "moAbstract.h"
#include "moConfig.h"
#include "moParam.h"

class moTextureManager;

/**
 * Clase que encapsula una lista de punteros a objetos moTexture. La utilidad de esta clase es que
 * permite definir un subconjunto de texturas definidas en el administrador de texturas y accederlas de
 *  una manera más directa.
 * @see moTextureManager
 */
class LIBMOLDEO_API moTextureIndex : public moAbstract
{
public:
    /**
     * Constructor por defecto.
     */
	moTextureIndex() { m_textures_array.Init(0, NULL); }
    /**
     * Destructor por defecto.
     */
	virtual ~moTextureIndex() { Finish(); }

    /**
     * Método de inicialización.
     * @param p_cfg puntero al objeto de configuración de Moldeo donde se especifica la lista de texturas.
     * @param p_param_idx índice en el objeto de configuración donde se encuentran los filtros.
     * @param p_tex_man puntero al administrador de texturas.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg, MOuint p_param_idx, moTextureManager* p_tex_man)
	{ LoadTextures(p_cfg, p_param_idx, p_tex_man); return true; }
    /**
     * Método de inicialización.
     * @param p_cfg puntero al objeto de configuración de Moldeo donde se especifica la lista de texturas.
     * @param p_param_ref
     * @param p_tex_man puntero al administrador de texturas.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Init(moConfig* p_cfg, moParamReference p_param_ref, moTextureManager* p_tex_man)
	{ LoadTextures(p_cfg, p_cfg->GetConfigDefinition()->ParamIndexes().GetRef(p_param_ref.reference).index, p_tex_man); return true; }
    /**
     * Método de finalización.
     * @return resultado de la operación: true si fue exitosa, false en caso contrario.
     */
	virtual MOboolean Finish() { m_textures_array.Finish(); return true; }

    /**
     * Agrega la textura con nombre p_name.
     * @param p_name nombre de la textura a agregar.
     * @param p_tex_man puntero al administrador de texturas.
     * @return índice de la nueva textura.
     */
	MOint Add(moText p_name, moTextureManager* p_tex_man);
    /**
     * Agrega la textura apuntada por p_tex.
     * @param p_tex puntero de la textura a agregar.
     * @return índice de la nueva textura.
     */
	MOint Add(moTexture* p_tex);

    /**
     * Devuelve el OpenGL ID de la textura con índice p_idx.
     * @param p_idx índice de la textura.
     * @return OpenGL ID de la textura.
     */
	MOuint GetGLId(MOuint p_idx);
    /**
     * Devuelve el OpenGL ID de la sub-textura p_cuad de la textura con índice p_idx.
     * @param p_idx índice de la textura.
     * @param p_cuad índice del cuadro.
     * @return OpenGL ID de la textura.
     */
	MOuint GetGLId(MOuint p_idx, MOint p_cuad);
    /**
     * Devuelve el OpenGL ID de la sub-textura que corresponde a la fracción de ciclo p_cycle para
     * la textura con índice p_idx.
     * @param p_idx índice de la textura.
     * @param p_cycle fracción de ciclo entre 0.0 y 1.0.
     * @return OpenGL ID de la textura.
     */
	MOuint GetGLId(MOuint p_idx, MOfloat p_cycle);
    /**
     * Devuelve el OpenGL ID de textura que corresponde al tempo p_tempo para la textura con índice p_idx.
     * @param p_idx índice de la textura.
     * @param p_tempo puntero al objeto de tempo.
     * @return OpenGL ID de la textura.
     */
	MOuint GetGLId(MOuint p_idx, moTempo *p_tempo);
    /**
     * Devuelve el tipo de la textura con índice p_idx.
     * @param p_idx índice de la textura.
     * @return tipo de textura (MO_TYPE_TEXTURE, MO_TYPE_TEXTURE_MULTIPLE, MO_TYPE_MOVIE, etc.).
     */
	MOint GetType(MOuint p_idx);
    /**
     * Devuelve el puntero a la textura con índice p_idx.
     * @param p_idx índice de la textura.
     * @return puntero a la textura requerida.
     */
	moTexture* GetTexture(MOuint p_idx);

    /**
     * Valida el índice de textura p_idx.
     * @param p_idx índice de textura a validar.
     * @return true si el índice es válido, false en caso contrario.
     */
	MOboolean ValidIndex(MOuint p_idx);
    /**
     * Devuelve la lista de texturas.
     * @return lista de texturas.
     */
	moTextureArray GetTextureArray() { return m_textures_array; } // Deberia devolver referencia constante!
    /**
     * Devuelve el número de texturas.
     * @return número de texturas.
     */
	MOuint Count() { return m_textures_array.Count(); }
    /**
     * Devuelve el puntero a la textura con índice p_idx.
     * @param p_idx índice de la textura.
     * @return puntero a la textura requerida.
     */
	moTexture* operator [](MOuint p_idx) { return GetTexture(p_idx); }
private:
	moTextureArray m_textures_array;

	MOuint LoadTextures(moConfig* p_cfg, MOuint p_param_idx, moTextureManager* p_tex_man);
};

#endif

