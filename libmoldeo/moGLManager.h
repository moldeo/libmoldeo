/*******************************************************************************

                                moGLManager.h

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

#ifndef __MO_GLMANAGER_H__
#define __MO_GLMANAGER_H__

#define MO_GPU_NV 0
#define MO_GPU_ATI 1
#define MO_GPU_INTEL 2
#define MO_GPU_OTHER 3

#include "moTypes.h"
#include "moText.h"
#include "moFBO.h"

#include "moResourceManager.h"


typedef void* moGLContext;
typedef void* moDisplayServer;
typedef void* moDisplayScreen;
typedef void* moDisplayWindow;

/*
class moGLContext {

    public:
        moGLContext();
        virtual ~moGLContext();

    void *m_Context;
};
*/


/// manejador de operaciones comunes de Open GL
/**
 * Contiene funciones básicas de manejo de estado de OpenGL, manejo de errors y consulta de características
 * la placa gráfica.
 */
class LIBMOLDEO_API moGLManager : public moResource
{
    public:
        /**
         * El constructor por defecto.
         */
		moGLManager();
        /**
         * El destructor por defecto.
         */
		virtual ~moGLManager();

        /**
         * Inicializador.
         * @ true o false de acuerdo con el resultado de la operación.
         */
		virtual MOboolean Init();
        /**
         * Finalizador.
         * @return true o false de acuerdo con el resultado de la operación.
         */
		virtual MOboolean Finish();

        /**
         * Imprime los errores que se acumularon en la pila de errores de OpenGL desde la última llamada.
         * @param p_location cadena de texto donde se puede pasar la posición en el programa desde donde se llama a esta función.
         * @return true si se encontraron errores, false en caso contrario.
         */
        MOboolean CheckErrors(moText p_location);

        /**
         * Devuelve el código que identifica al fabricante del GPU.
         * @return código del fabricante (MO_GPU_NV, MO_GPU_ATI, etc.).
         */
		MOuint GetGPUVendorCode() { return m_gpu_vendor_code; }
        /**
         * Devuelve la cadena de texto completa que identifica al fabricante del GPU.
         * @return texto identificando el fabricante del GPU.
         */
		moText GetGPUVendorString() { return m_gpu_ventor_string; }

        /**
         * Configura la matriz de proyección y el viewport a fin de generar una visualización en perspectiva
         * con el tamaño de ventana indicado.
         * @param p_width ancho de la ventana.
         * @param p_height alto de la ventana.
         */
		void SetPerspectiveView(MOint p_width, MOint p_height);
        /**
         * Configura la matriz de proyección y el viewport a fin de generar una visualización ortográfica (2D)
         * con el tamaño de ventana indicado.
         * @param p_width ancho de la ventana.
         * @param p_height alto de la ventana.
         */
		void SetOrthographicView(MOint p_width=0, MOint p_height=0);

        /**
         * Devuelve el modo de render actual (GL_RENDER, GL_SELECT o GL_FEEDBACK).
         * @return p_width modo de render.
         */
		MOint GetRenderMode();
        /**
         * Establece el modo de render (GL_RENDER, GL_SELECT o GL_FEEDBACK).
         * @param p_width modo de render.
         */
		void SetRenderMode(MOint p_mode);

        /**
         * Fija el estado de OpenGL de acuerdo con la configuración por defecto de Moldeo:
         * test de profundidad, blending y texturas 2D activados.
         */
		void SetMoldeoGLState();
        /**
         * Revierte los cambios introducidos por SetMoldeoGLState. Es decir,
         * test de profundidad, blending y texturas 2D desactivados.
         */
		void SetDefaultGLState();
        /**
         * Establece los modos de almacenamiento de píxels (pack y unpack) por defecto.
         */
		void SetDefaultPixelStorageModes();
        /**
         * Guarda el estado actual de OpenGL (estado del framebuffer, todos los bits de atributos y
         * las matrices).
         */
		void SaveGLState();
        /**
         * Guarda las matrices de OpenGL (proyección, vista de modelo y textura) en su estado actual.
         */
		void SaveGLMatrices();
        /**
         * Guarda la configuración de la ventana de visualización o viewport.
         */
		void SaveView();
        /**
         * Guarda el framebuffer actual y los búfers de escritura y lectura.
         */
		void SaveFramebuffer();
        /**
         * Restaura el estado de OpenGL que fue guardado anteriormente con SaveGLState.
         */
		void RestoreGLState();
        /**
         * Restaura el estado de las matrices de OpenGL que fue guardado anteriormente con SaveGLMatrices.
         */
		void RestoreGLMatrices();
        /**
         * Restaura la configuración de la ventana de visualización o viewport, guardada anteriormente con
         * SaveView.
         */
		void RestoreView();
        /**
         * Restaura el framebuffer actual y los búfers de escritura y lectura, guardada anteriormente con
         * SaveFramebuffer.
         */
		void RestoreFramebuffer();

        /**
         * Construye los parámetros de una textura de punto flotante de acuerdo con las opciones pasadas a la
         * función.
         * @param p_16bits si es true, entonces se genera los parámetros para una textura de 16 bits, en caso contrario, de 32 bits.
         * @param p_num_components número de componentes por píxel.
         * @return parámetros de textura de punto flotante.
         */
		moTexParam BuildFPTexParam(MOboolean p_16bits = true, MOushort p_num_components = 4);
        /**
         * Revisa que p_target represente una textura rectangular.
         * @param p_target target de textura.
         * @return true o false, de acuerdo si el tárget corresponde una textura rectangular o no.
         */
		MOboolean RectTexture(GLenum p_target)  const;
        /**
         * Revisa que p_internal_format represente una textura de punto flotante.
         * @param p_internal_format formato interno de textura.
         * @return true o false, de acuerdo si el formato interno corresponde una textura de punto flotante o no.
         */
		MOboolean FPTexture(GLint p_internal_format);
        /**
         * Revisa que p_min_filter represente un filtro de minimización válido.
         * @param p_min_filter filtro de minimización.
         * @return true o false, de acuerdo si el filtro de minimización es válido.
         */
		MOboolean MipMapTexture(GLint p_min_filter);

        /**
         * Establece m_fbo como nuevo FBO.
         * @param m_fbo identificador OpenGL del FBO.
         */
		void SetCurrentFBO(MOuint m_fbo);
        /**
         * Devuelve el FBO activo en este momento.
         * @return identificador OpenGL del FBO.
         */
		MOuint GetCurrentFBO() { return m_current_fbo; }

        /**
         * Establece p_buffer como nuevo búfer de lectura.
         * @param p_buffer búfer de lectura.
         */
		void SetCurrentReadBuffer(MOint p_buffer);
        /**
         * Devuelve el búfer de lectura activo en este momento.
         * @return búfer de lectura.
         */
		MOint GetCurrentReadBuffer() { return m_current_read_buffer; }
        /**
         * Establece p_buffer como nuevo búfer de escritura.
         * @param p_buffer búfer de escritura.
         */
		void SetCurrentDrawBuffer(MOint p_buffer);
        /**
         * Devuelve el búfer de escritura activo en este momento.
         * @return búfer de escritura.
         */
		MOint GetCurrentDrawBuffer() { return m_current_draw_buffer; }
        /**
         * Guarda el FBO y los búfers de escritura y lectura activos en este momento.
         */
		void SaveFBOState();
        /**
         * Restaura el FBO y los búfers de escritura y lectura guardados con SaveFBOState.
         */
		void RestoreFBOState();


		int CreateContext( int p_width, int p_height );
    moGLContext GetContext();
    void SetContext(moGLContext p_Context);
    moDisplayServer GetDisplayServer();
    moDisplayScreen  GetDisplayScreen();
    moDisplayWindow  GetDisplayWindow();

		void SetFrameBufferObjectActive( bool active = true );

    private:

    moGLContext m_Context;
    moDisplayServer m_DisplayServer;
    moDisplayScreen  m_DisplayScreen;
    moDisplayWindow  m_DisplayWindow;

		MOuint m_gpu_vendor_code;
		moText m_gpu_ventor_string;

		MOuint m_current_fbo;
		MOint m_current_read_buffer;
		MOint m_current_draw_buffer;
		MOuint m_previous_fbo;
		MOint m_previous_read_buffer;
		MOint m_previous_draw_buffer;
		MOuint m_saved_fbo;
		MOint m_saved_read_buffer;
		MOint m_saved_draw_buffer;

		bool    m_bFrameBufferObjectActive;

		void QueryGPUVendorString();
};


#endif
