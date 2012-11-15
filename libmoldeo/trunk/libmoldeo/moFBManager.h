/*******************************************************************************

                                moFBManager.h

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
  Andrés Colubri

*******************************************************************************/

#ifndef __MO_FBMANAGER_H__
#define __MO_FBMANAGER_H__

#include "moTypes.h"
#include "moText.h"
#include "moLock.h"
#include "moFBO.h"
#include "moTexture.h"
#include "moResourceManager.h"

/**
 * Esta clase implementa un administrador de framebuffers. Existen dos tipos de framebuffers:
 * el framebuffer de la pantalla, que es el utilizado por defecto por OpenGL para todas las operaciones
 * de render que terminan siendo visualizadas en la pantalla, y los FBO (FrameBuffer Objects) que permiten
 * contextos de render fuera de pantalla.
 * Cada vez que un framebuffer es activado (bind), el que estaba activo hasta ese momento es guardado de manera
 * tal que pueda ser restaurado cuando al llamar a la función de desactivación (unbind).

 */
class LIBMOLDEO_API moFBManager : public moResource
{
    public:
        /**
         * Constructor por defecto.
         */
		moFBManager();
        /**
         * Destructor por defecto.
         */
		virtual ~moFBManager();

        /**
         * Inicializador del administrador.
         */
		virtual MOboolean Init();
        /**
         * Finalizador del administrador.
         */
		virtual MOboolean Finish();

        /**
         * Agrega un nuevo FBO.
         * @return índice del nuevo FBO.
         */
        MOuint CreateFBO();
        /**
         * Elimina el FBO con índice p_fbo.
         * @param p_fbo índice del FBO a eliminar.
         * @return true en caso de que el FBO se haya podido eliminar, false en caso contrario.
         */
		MOboolean DeleteFBO(MOuint p_fbo);
        /**
         * Devuelve el número de FBOs.
         * @return número de FBOs.
         */
		MOuint GetFBOCount() { return m_fbo_array.Count(); }
        /**
         * Devuelve un puntero al FBO con índice p_fbo.
         * @param p_fbo índice del FBO.
         * @return puntero al FBO.
         */
		moFBO* GetFBO(MOuint p_fbo) { return m_fbo_array.Get(p_fbo); }

        /**
         * Devuelve un puntero al primer FBO cuyos parámetros coincidan con p_target, p_internal_format, p_width
         * y p_height.
         * @param p_target tárget OpenGL de las texturas asociadas al FBO (GL_TEXTURE_2D, GL_TEXTURE_RECT, etc.).
         * @param p_internal_format formato interno de las texturas asociadas al FBO (GL_RGBA, GL_RGB, etc.).
         * @param p_width ancho de las texturas asociadas al FBO.
         * @param p_height alto de las texturas asociadas al FBO.
         * @return puntero al FBO.
         */
		MOint GetFBO(MOuint p_target, MOuint p_internal_format, MOuint p_width, MOuint p_height);

        /**
         * Activa el frambuffer de la pantalla.
         */
		void BindScreenFB();
        /**
         * Desactiva el frambuffer de la pantalla.
         */
		void UnbindScreenFB();

        /**
         * Activa al FBO con índice p_fbo.
         * @param p_fbo índice del FBO.
         * @return true si la operación tiene éxito, false en caso contrario.
         */
		MOboolean BindFBO(MOuint p_fbo);
        /**
         * Activa al FBO con índice p_fbo y a la textura de escritura con índice p_attach_point en dicho FBO.
         * @param p_fbo índice del FBO.
         * @param p_attach_point índice del búfer de color a utilizar.
         * @return true si la operación tiene éxito, false en caso contrario.
         */
		MOboolean BindFBO(MOuint p_fbo, MOuint p_attach_point);
        /**
         * Activa al FBO con índice p_fbo y a la texturas de escritura con índices p_attach_point_list en dicho FBO.
         * @param p_fbo índice del FBO.
         * @param p_num_attach_points número de texturas de escritura a utilizar.
         * @param p_attach_point_list índice de los búfers de color a utilizar (a los cuales están asociados las texturas).
         * @return true si la operación tiene éxito, false en caso contrario.
         */
		MOboolean BindFBO(MOuint p_fbo, MOuint p_num_attach_points, MOuint *p_attach_point_list);

        /**
         * Agrega la textura apuntada por p_pTex como nuevo búfer de color del FBO con índice p_fbo.
         * @param p_fbo índice del FBO.
         * @param p_pTex puntero a la textura a agregar.
         * @param p_attach_point en esta variable pasada por referencia es devuelto el índice del punto de asociación asignado a la textura.
         * @return resultado de la operación de asociación.
         */
		MOboolean AddTexture(MOuint p_fbo, moTexture* p_pTex, MOuint &p_attach_point);
        /**
         * Asocia la textura apuntada por p_pTex al búfer de color p_attach_point del FBO con índice p_fbo.
         * @param p_fbo índice del FBO.
         * @param p_pTex puntero a la textura a agregar.
         * @param p_attach_point índice del punto de asociación al cual será asociada la textura.
         * @return resultado de la operación de asociación.
         */
        MOboolean AttachTexture(MOuint p_fbo, moTexture* p_pTex, MOuint p_attach_point);

        /**
         * Establece como textura de escritura aquella asociada en el búfer de color con índice p_attach_point
         * en el FBO actualmente activo.
         * @param p_attach_point índice del búfer de color a utilizar.
         */
	    void SetDrawTexture(MOuint p_attach_point);
        /**
         * Establece como texturas de escritura aquellas asociada con los búfers de color con índice p_attach_point_list
         * en el FBO actualmente activo.
         * @param p_num_attach_points número de texturas de escritura a utilizar.
         * @param p_attach_point_list índice de los búfers de color a utilizar (a los cuales están asociados las texturas).
         */
		void SetDrawTextures(MOuint p_num_attach_points, MOuint *p_attach_point_list);
        /**
         * Establece como textura de lectura aquella asociada en el búfer de color con índice p_attach_point
         * en el FBO actualmente activo.
         * @param p_attach_point índice del búfer de color a utilizar.
         */
	    void SetReadTexture(MOuint p_attach_point);

        /**
         * Establece como textura de escritura aquella con OpenGL ID p_glid en el FBO actualmente activo.
         * @param p_glid OpenGL ID de la textura a utilizar.
         * @param p_width ancho de la textura.
         * @param p_height alto de la textura.
         * @param p_param parámetros de la textura.
         * @param p_def_attach_point índice del punto de asociación por defecto.
         */
		void SetDrawTextureByGLId(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_def_attach_point = 0);
        /**
         * Establece como texturas de escritura aquella con OpenGL IDs en p_glid_list en el FBO actualmente activo.
         * @param p_num_glids número de texturas a utilizar.
         * @param p_glid_list lista de los OpenGL IDs de la texturas a utilizar.
         * @param p_width ancho de la texturas.
         * @param p_height alto de la texturas.
         * @param p_param parámetros de las texturas.
         */
		void SetDrawTexturesByGLId(MOuint p_num_glids, MOuint *p_glid_list, MOuint p_width, MOuint p_height, const moTexParam& p_param);
        /**
         * Establece como textura de lectura aquella con OpenGL ID p_glid en el FBO actualmente activo.
         * @param p_glid OpenGL ID de la textura a utilizar.
         * @param p_width ancho de la textura.
         * @param p_height alto de la textura.
         * @param p_param parámetros de la textura.
         * @param p_def_attach_point índice del punto de asociación por defecto.
         */
        void SetReadTextureByGLId(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_def_attach_point = 0);

        /**
         * Establece como textura de escritura aquella apuntada por p_pTex. Si dicha textura no está asociada
         * al FBO actualmente activo, la función devuelve false.
         * @param p_pTex puntero a la textura a utilizar.
         * @param p_def_attach_point índice del punto de asociación por defecto.
         * @return true si la operación es exitosa, false en caso contrario.
         */
		MOint SetDrawTexture(moTexture* p_pTex, MOuint p_def_attach_point = 0);
        /**
         * Establece como texturas de escritura aquella apuntada por p_tex_array.
         * @param p_tex_array lista de texturas a utilizar.
         * @return true si la operación es exitosa, false en caso contrario.
         */
		MOint SetDrawTextures(moTextureArray &p_tex_array);
        /**
         * Establece como textura de lectura aquella apuntada por p_pTex. Si dicha textura no está asociada
         * al FBO actualmente activo, la función devuelve false.
         * @param p_pTex puntero a la textura a utilizar.
         * @param p_def_attach_point índice del punto de asociación por defecto.
         * @return true si la operación es exitosa, false en caso contrario.
         */
		MOint SetReadTexture(moTexture* p_pTex, MOuint p_def_attach_point = 0);

        /**
         * Desactiva el FBO actualmente en uso, y activa el que estaba activo al momento de llamar BindFBO.
         * @see BindFBO
         */
		void UnbindFBO();
        /**
         * Establece el framebuffer de la pantalla como framebuffer activo.
         */
		void UnbindAllFBO();
    private:
		moGLManager* m_glmanager;

		moFBOArray m_fbo_array;

		// Index of the currently selected fbo, -1 if the screen framebuffer is binded.
		MOint m_current_fbo;

        MOint m_fbo_stack_index;
		MOuint m_fbo_stack[MO_MAX_FBO_STACK_SIZE];
		MOuint m_draw_buffer_stack[MO_MAX_FBO_STACK_SIZE];
		MOuint m_read_buffer_stack[MO_MAX_FBO_STACK_SIZE];

		GLint m_screen_read_buffer;
        GLint m_screen_draw_buffer;

		GLenum m_color_buffers[MO_MAX_COLOR_ATTACHMENTS_EXT];

		void GetScreenBuffers();
};

#include "moGLManager.h"

#endif

