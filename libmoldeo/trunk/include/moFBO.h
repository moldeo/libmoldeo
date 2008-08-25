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
  Andr�s Colubri

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
 * las operaciones de render a textura. Varias texturas pueden ser asociadas (attached) como b�fers de color
 * a un FBO, de esta manera se puede hacer un render a varias texturas simult�neamente. A un FBO tambi�n pueden
 * ser asociados b�fers de stencil y depth. De esta manera un FBO puede representar un contexto de render completo
 * donde se pueden llevar a cabo todas las operaciones de dibujo que se efect�an normalmente en el framebuffer de la
 * pantalla. Nota: los t�rminos punto de asociaci�n y b�fer de color son utilizados como sin�nimos en los comentarios
 * que siguen.
 */
class LIBMOLDEO_API moFBO : public moAbstract
{
    public:
        /**
         * Constructor por defecto.
         */
		moFBO();
        /**
         * Destructor por defecto.
         */
		virtual ~moFBO();

        /**
         * Initicializa el FBO utilizando p_gl como adminstrador de OpenGL.
         * @param p_gl puntero al administrador de OpenGL.
         * @return true si la operaci�n fue exitosa, false en caso contrario.
         */
		virtual MOboolean Init(moGLManager* p_gl);
        /**
         * Finaliza el FBO.
         * @return true si la operaci�n fue exitosa, false en caso contrario.
         */
		virtual MOboolean Finish();

        /**
         * Devuelve true o false dependiendo si el FBO tiene un depth b�fer asociado.
         * @return true o false de acuerdo a la presencia de un depth b�fer.
         */
		MOboolean HasDepthBuffer() { return m_has_depth_buffer; }
        /**
         * Devuelve true o false dependiendo si el FBO tiene un stencil b�fer asociado.
         * @return true o false de acuerdo a la presencia de un stencil b�fer.
         */
		MOboolean HasStencilBuffer() { return m_has_stencil_buffer; }
        /**
         * Crea y asocia depth y stencil b�fers al FBO.
         */
		MOboolean AddDepthStencilBuffer();

        /**
         * Activa este FBO. Todos los renders se realizaran a las texturas asociadas a
         * los b�fers de color de este FBO. Guarda el framebuffer actual.
         */
		void Bind();
        /**
         * Desactiva este FBO y restaura el framebuffer activo antes de llamar Bind().
         */
		void Unbind();

        /**
         * Establece como textura de lectura aquella asociada en el b�fer de color con �ndice p_attach_point en este FBO.
         * @param p_attach_point �ndice del b�fer de color a utilizar.
         */
		void SetReadTexture(MOuint p_attach_point);
        /**
         * Establece como textura de escritura aquella asociada en el b�fer de color con �ndice p_attach_point en este FBO.
         * @param p_attach_point �ndice del b�fer de color a utilizar.
         */
		void SetDrawTexture(MOuint p_attach_point);
        /**
         * Establece como textura de escritura aquella con OpenGL ID p_glid. Si dicha textura ya esta asociada al FBO, el efecto
         * de esta funci�n es equivalente al de llamar SetReadTexture con el �ndice p_attach_point correspondiente a dicha textura.
         * En el caso de que la textura no este asociada, es asociada (attached) utilizando la funci�n AttachTexture con los par�metros
         * p_width, p_height, etc.
         * @param p_glid OpenGL ID de la textura a asociar.
         * @param p_width ancho de la textura a asociar.
         * @param p_height alto de la textura a asociar.
         * @param p_param par�metros de la textura a asociar.
         * @param p_attach_point �ndice del punto de asociaci�n (el b�fer de color del FBO) al cual vincular la textura.
         * @see AttachTexture
         */
		void SetReadTexture(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_attach_point = 0);
        /**
         * Establece como textura de lectura aquella con OpenGL ID p_glid. Si dicha textura ya esta asociada al FBO, el efecto
         * de esta funci�n es equivalente al de llamar SetDrawTexture con el �ndice p_attach_point correspondiente a dicha textura.
         * En el caso de que la textura no este asociada, es asociada (attached) utilizando la funci�n AttachTexture con los par�metros
         * p_width, p_height, etc.
         * @param p_glid OpenGL ID de la textura a asociar.
         * @param p_width ancho de la textura a asociar.
         * @param p_height alto de la textura a asociar.
         * @param p_param par�metros de la textura a asociar.
         * @param p_attach_point �ndice del punto de asociaci�n (el b�fer de color del FBO) al cual vincular la textura.
         * @see AttachTexture
         */
		void SetDrawTexture(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_attach_point = 0);

        /**
         * Devuelve el identificador de OpenGL para el punto de asociaci�n p_attach_point.
         * @param p_attach_point �ndice del punto de asociaci�n.
         * @return identificador de OpenGL.
         */
		MOint GetColorAttachPoint(MOuint p_attach_point);
        /**
         * Devuelve el OpenGL ID de la textura vinculada al punto de asociaci�n p_attach_point.
         * @param p_attach_point �ndice del punto de asociaci�n.
         * @return OpenGL ID de la textura.
         */
		MOint GetTextureGLId(MOuint p_attach_point);
        /**
         * Devuelve el �ndice del punto de asociaci�n al cual est� vinculada la textura con OpenGL ID p_glid.
         * @param p_glid OpenGL ID de la textura.
         * @return �ndice del punto de asociaci�n.
         */
		MOint GetColorAttachPointIndex(MOuint p_glid);

        /**
         * Devuelve el n�mero de puntos de asociaci�n b�fers de color utilizados en este momento.
         * @return n�mero de puntos de asociaci�n.
         */
		MOuint GetColorAttachPointsCount() { return m_num_color_attach_points; }

        /**
         * Agrega la textura con los par�metros especificados como nuevo b�fer de color.
         * @param p_width ancho de la textura a asociar.
         * @param p_height alto de la textura a asociar.
         * @param p_param par�metros de la textura a asociar.
         * @param p_glid OpenGL ID de la textura a asociar.
         * @param p_attach_point en esta variable pasada por referencia es devuelto el �ndice del punto de asociaci�n asignado a la textura.
         * @return resultado de la operaci�n de asociaci�n.
         */
		MOuint AddTexture(MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_glid, MOuint &p_attach_point);

        /**
         * Asocia la textura con los par�metros especificados al b�fer de color p_attach_point.
         * @param p_width ancho de la textura a asociar.
         * @param p_height alto de la textura a asociar.
         * @param p_param par�metros de la textura a asociar.
         * @param p_glid OpenGL ID de la textura a asociar.
         * @param p_attach_point �ndice del punto de asociaci�n al cual ser� asociada la textura.
         * @return resultado de la operaci�n de asociaci�n.
         */
        MOuint AttachTexture(MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_glid, MOuint p_attach_point);
        /**
         * Limpia todos los puntos de asociaci�n de este FBO.
         */
		void ClearAttachements();

        /**
         * Devuelve el estado del FBO.
         * @return estado del FBO.
         */
		MOuint CheckStatus();

        /**
         * Revisa que una textura de tama�o p_width x p_height y par�metros p_param sea compatible con
         * la actual configuraci�n de los puntos de asociaci�n. Una vez que una textura es asociada a un
         * b�fer de color
         * @param p_width ancho a revisar.
         * @param p_height alto a revisar.
         * @param p_param par�metros a revisar.
         * @return resultado de la revisi�n: true en caso de que los par�metros sean compatibles, false en caso contrario.
         */
		MOboolean IsValidTexture(MOuint p_width, MOuint p_height, const moTexParam& p_param);

        /**
         * Devuelve el identificador de OpenGL de este FBO.
         * @return identificador de OpenGL.
         */
		MOuint GetGLId() { return m_fbo; }
        /**
         * Devuelve el t�rget OpenGL (GL_TEXTURE_2D, GL_TEXTURE_RECT, etc.) de las texturas asociadas a los b�fers de color de este FBO.
         * @return OpenGL target de las texturas asociadas.
         */
		MOuint GetTarget() { return m_target; }
        /**
         * Devuelve el formato interno (GL_RGBA, GL_RGB, etc.) de las texturas asociadas a los b�fers de color de este FBO.
         * @return formato interno de las texturas asociadas.
         */
		MOuint GetInternalFormat() { return m_internal_format; }
        /**
         * Devuelve el ancho de las texturas asociadas a los b�fers de color de este FBO.
         * @return ancho de la texturas asociadas.
         */
		MOuint GetWidth() { return m_width; }
        /**
         * Devuelve el alto de las texturas asociadas a los b�fers de color de este FBO.
         * @return alto de la texturas asociadas.
         */
		MOuint GetHeight() { return m_height; }
    private:
		moGLManager* m_gl;

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

        void InitAttachPointsArray();
};

/*
template class LIBMOLDEO_API moDynamicArray<moFBO*>;
typedef moDynamicArray<moFBO*> moFBOArray;
*/
moDeclareExportedDynamicArray( moFBO*, moFBOArray)

#include "moGLManager.h"

#endif
