/*******************************************************************************

								moFBO.h

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
  Andrs Colubri

*******************************************************************************/

#ifndef __MO_FBO_H__
#define __MO_FBO_H__

#include "moTypes.h"

#define MO_FRAMEBUFFER_FULL 0
#define MO_FRAMEBUFFER_INVALID_TEX 1
#define MO_MAX_FBO_STACK_SIZE 10
#define MO_MAX_COLOR_ATTACHMENTS_EXT GL_MAX_COLOR_ATTACHMENTS_EXT
class moGLManager;

/**
 * Clase que encapsula un FrameBuffer Object (FBO) de OpenGL. Este tipo de objeto facilita enormemente
 * las operaciones de render a textura. Varias texturas pueden ser asociadas (attached) como bfers de color
 * a un FBO, de esta manera se puede hacer un render a varias texturas simultneamente. A un FBO tambin pueden
 * ser asociados bfers de stencil y depth. De esta manera un FBO puede representar un contexto de render completo
 * donde se pueden llevar a cabo todas las operaciones de dibujo que se efectan normalmente en el framebuffer de la
 * pantalla. Nota: los trminos punto de asociacin y bfer de color son utilizados como sinnimos en los comentarios
 * que siguen.
 */
class LIBMOLDEO_API moFBO:public moAbstract
{
	public:
		/**
		 * Constructor por defecto.
		 */
		moFBO ();

		/**
		 * Destructor por defecto.
		 */
		virtual ~ moFBO ();

		/**
		 * Initicializa el FBO utilizando p_gl como adminstrador de OpenGL.
		 * @param p_gl puntero al administrador de OpenGL.
		 * @return true si la operacin fue exitosa, false en caso contrario.
		 */
		virtual MOboolean Init (moGLManager * p_gl);

		/**
		 * Finaliza el FBO.
		 * @return true si la operacin fue exitosa, false en caso contrario.
		 */
		virtual MOboolean Finish ();

		/**
		 * Devuelve true o false dependiendo si el FBO tiene un depth bfer asociado.
		 * @return true o false de acuerdo a la presencia de un depth bfer.
		 */
		MOboolean HasDepthBuffer ()
		{
			return m_has_depth_buffer;
		}

		/**
		 * Devuelve true o false dependiendo si el FBO tiene un stencil bfer asociado.
		 * @return true o false de acuerdo a la presencia de un stencil bfer.
		 */
		MOboolean HasStencilBuffer ()
		{
			return m_has_stencil_buffer;
		}

		/**
		 * Crea y asocia depth y stencil bfers al FBO.
		 */
		MOboolean AddDepthStencilBuffer ();

		/**
		 * Activa este FBO. Todos los renders se realizaran a las texturas asociadas a
		 * los bfers de color de este FBO. Guarda el framebuffer actual.
		 */
		void Bind ();

		/**
		 * Desactiva este FBO y restaura el framebuffer activo antes de llamar Bind().
		 */
		void Unbind ();

		/**
		 * Establece como textura de lectura aquella asociada en el bfer de color con ndice p_attach_point en este FBO.
		 * @param p_attach_point ndice del bfer de color a utilizar.
		 */
		void SetReadTexture (MOuint p_attach_point);

		/**
		 * Establece como textura de escritura aquella asociada en el bfer de color con ndice p_attach_point en este FBO.
		 * @param p_attach_point ndice del bfer de color a utilizar.
		 */
		void SetDrawTexture (MOuint p_attach_point);

		/**
		 * Establece como textura de escritura aquella con OpenGL ID p_glid. Si dicha textura ya esta asociada al FBO, el efecto
		 * de esta funcin es equivalente al de llamar SetReadTexture con el ndice p_attach_point correspondiente a dicha textura.
		 * En el caso de que la textura no este asociada, es asociada (attached) utilizando la funcin AttachTexture con los parmetros
		 * p_width, p_height, etc.
		 * @param p_glid OpenGL ID de la textura a asociar.
		 * @param p_width ancho de la textura a asociar.
		 * @param p_height alto de la textura a asociar.
		 * @param p_param parmetros de la textura a asociar.
		 * @param p_attach_point ndice del punto de asociacin (el bfer de color del FBO) al cual vincular la textura.
		 * @see AttachTexture
		 */
		void SetReadTexture (MOuint p_glid, MOuint p_width, MOuint p_height,
			const moTexParam & p_param, MOuint p_attach_point = 0);

		/**
		 * Establece como textura de lectura aquella con OpenGL ID p_glid. Si dicha textura ya esta asociada al FBO, el efecto
		 * de esta funcin es equivalente al de llamar SetDrawTexture con el ndice p_attach_point correspondiente a dicha textura.
		 * En el caso de que la textura no este asociada, es asociada (attached) utilizando la funcin AttachTexture con los parmetros
		 * p_width, p_height, etc.
		 * @param p_glid OpenGL ID de la textura a asociar.
		 * @param p_width ancho de la textura a asociar.
		 * @param p_height alto de la textura a asociar.
		 * @param p_param parmetros de la textura a asociar.
		 * @param p_attach_point ndice del punto de asociacin (el bfer de color del FBO) al cual vincular la textura.
		 * @see AttachTexture
		 */
		void SetDrawTexture (MOuint p_glid, MOuint p_width, MOuint p_height,
			const moTexParam & p_param, MOuint p_attach_point = 0);

		/**
		 * Devuelve el identificador de OpenGL para el punto de asociacin p_attach_point.
		 * @param p_attach_point ndice del punto de asociacin.
		 * @return identificador de OpenGL.
		 */
		MOint GetColorAttachPoint (MOuint p_attach_point);

		/**
		 * Devuelve el OpenGL ID de la textura vinculada al punto de asociacin p_attach_point.
		 * @param p_attach_point ndice del punto de asociacin.
		 * @return OpenGL ID de la textura.
		 */
		MOint GetTextureGLId (MOuint p_attach_point);

		/**
		 * Devuelve el ndice del punto de asociacin al cual est vinculada la textura con OpenGL ID p_glid.
		 * @param p_glid OpenGL ID de la textura.
		 * @return ndice del punto de asociacin.
		 */
		MOint GetColorAttachPointIndex (MOuint p_glid);

		/**
		 * Devuelve el nmero de puntos de asociacin bfers de color utilizados en este momento.
		 * @return nmero de puntos de asociacin.
		 */
		MOuint GetColorAttachPointsCount ()
		{
			return m_num_color_attach_points;
		}

		/**
		 * Agrega la textura con los parmetros especificados como nuevo bfer de color.
		 * @param p_width ancho de la textura a asociar.
		 * @param p_height alto de la textura a asociar.
		 * @param p_param parmetros de la textura a asociar.
		 * @param p_glid OpenGL ID de la textura a asociar.
		 * @param p_attach_point en esta variable pasada por referencia es devuelto el ndice del punto de asociacin asignado a la textura.
		 * @return resultado de la operacin de asociacin.
		 */
		MOuint AddTexture (MOuint p_width, MOuint p_height,
			const moTexParam & p_param, MOuint p_glid,
			MOuint & p_attach_point);

		/**
		 * Asocia la textura con los parmetros especificados al bfer de color p_attach_point.
		 * @param p_width ancho de la textura a asociar.
		 * @param p_height alto de la textura a asociar.
		 * @param p_param parmetros de la textura a asociar.
		 * @param p_glid OpenGL ID de la textura a asociar.
		 * @param p_attach_point ndice del punto de asociacin al cual ser asociada la textura.
		 * @return resultado de la operacin de asociacin.
		 */
		MOuint AttachTexture (MOuint p_width, MOuint p_height,
			const moTexParam & p_param, MOuint p_glid,
			MOuint p_attach_point);

		/**
		 * Limpia todos los puntos de asociacin de este FBO.
		 */
		void ClearAttachements ();

		/**
		 * Devuelve el estado del FBO.
		 * @return estado del FBO.
		 */
		MOuint CheckStatus ();

		/**
		 * Revisa que una textura de tamao p_width x p_height y parmetros p_param sea compatible con
		 * la actual configuracin de los puntos de asociacin. Una vez que una textura es asociada a un
		 * bfer de color
		 * @param p_width ancho a revisar.
		 * @param p_height alto a revisar.
		 * @param p_param parmetros a revisar.
		 * @return resultado de la revisin: true en caso de que los parmetros sean compatibles, false en caso contrario.
		 */
		MOboolean IsValidTexture (MOuint p_width, MOuint p_height,
			const moTexParam & p_param);

		/**
		 * Devuelve el identificador de OpenGL de este FBO.
		 * @return identificador de OpenGL.
		 */
		MOuint GetGLId ()
		{
			return m_fbo;
		}

		/**
		 * Devuelve el trget OpenGL (GL_TEXTURE_2D, GL_TEXTURE_RECT, etc.) de las texturas asociadas a los bfers de color de este FBO.
		 * @return OpenGL target de las texturas asociadas.
		 */
		MOuint GetTarget ()
		{
			return m_target;
		}

		/**
		 * Devuelve el formato interno (GL_RGBA, GL_RGB, etc.) de las texturas asociadas a los bfers de color de este FBO.
		 * @return formato interno de las texturas asociadas.
		 */
		MOuint GetInternalFormat ()
		{
			return m_internal_format;
		}

		/**
		 * Devuelve el ancho de las texturas asociadas a los bfers de color de este FBO.
		 * @return ancho de la texturas asociadas.
		 */
		MOuint GetWidth ()
		{
			return m_width;
		}

		/**
		 * Devuelve el alto de las texturas asociadas a los bfers de color de este FBO.
		 * @return alto de la texturas asociadas.
		 */
		MOuint GetHeight ()
		{
			return m_height;
		}
	private:moGLManager * m_gl;
	MOuint m_fbo;
	GLenum m_target;
	GLint m_internal_format;
	MOuint m_width;
	MOuint m_height;
	MOboolean m_has_depth_buffer;
	MOboolean m_has_stencil_buffer;
	MOuint m_DepthStencilTex;
	MOuint m_num_color_attach_points;
	MOuint m_tex_glid_array[MO_MAX_COLOR_ATTACHMENTS_EXT];
	MOuint m_attach_points_array[MO_MAX_COLOR_ATTACHMENTS_EXT];
	void InitAttachPointsArray ();
};

moDeclareExportedDynamicArray (moFBO *, moFBOArray);

#include "moGLManager.h"
#endif							 /*  */
