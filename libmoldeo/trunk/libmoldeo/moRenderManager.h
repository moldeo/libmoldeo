/*******************************************************************************

								moRenderManager.h

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

#ifndef __MO_RENDERMANAGER_H__
#define __MO_RENDERMANAGER_H__

//#include "moTypes.h"
//#include "moText.h"
//#include "moLock.h"
//#include "moResourceManager.h"
#include <moTextureManager.h>

//#include "moMathVector.h"
#include <moBuckets.h>

class moBucketPool;

#define MO_RENDER_RESOLUTION 0
#define MO_SCREEN_RESOLUTION 1

#define MO_RENDER_TO_TEXTURE_FBSCREEN 0
#define MO_RENDER_TO_TEXTURE_FBOBJECT 1
enum moRenderManagerMode
{

	RENDERMANAGER_MODE_NORMAL = 0,
	RENDERMANAGER_MODE_FRAMEBUFFER = 1,
	RENDERMANAGER_MODE_VDPAU = 2
};
enum moRenderOutputMode
{								 ///rsolution ouput to display output
	MO_RENDER_OUTPUT_MODE_NORMAL,
								 ///clip based.... each clip is a subsection of render resolution to a display resolution
	MO_RENDER_OUTPUT_MODE_ADVANCED
};
class LIBMOLDEO_API moResolution
{
	public:int width;
	int height;
	int aspect;
};

class LIBMOLDEO_API moRenderClip
{
	public:moVector2f points[4];
};

moDeclareExportedDynamicArray (moRenderClip *, moRenderClips);
class LIBMOLDEO_API moDisplayOutput
{
	public:moResolution m_DisplayResolution;
	int m_renderclip;			 /// index to corresponding render clip
};

moDeclareExportedDynamicArray (moDisplayOutput *, moDisplayOutputs);
class LIBMOLDEO_API moRenderOutputConfiguration
{
	public:moRenderOutputMode m_OutputMode;
	moResolution m_RenderResolution;

	///normal mode need this
	moResolution m_OutputResolution;

	///normal output need this
	moRenderClips Clips;		 ///clip or section of render resolution
	moDisplayOutputs Output1;	 ///each with diff resolution or not
};

/**
 * Clase que implementa el administrador de render. Este administrador crea 4 texturas que son utilizadas para
 * guardar estadios sucesivos en cada iteracin de dibujo:
 * 1) textura de render, identificada con MO_RENDER_TEX, donde se almacena el resultado del render luego de aplicar
 * cada efecto. Es decir, luego de aplicar el efecto i, esta textura contiene el render incluyendo la textura i
 * inclusive. Sin embargo, esta textura es slo utilizada cuando se est usando el mode de render a FBO fuera de pantalla.
 * 2) textura de pantalla, identificada con MO_SCREEN_TEX, donde se almacena el estado actual del render cuando se
 * ejecuta la funcin SaveScreen.
 * 3) textura de efectos, identificada con MO_EFFECTS_TEX, donde se almancena el resultado del render luego
 * de aplicar todos los pre-efectos y efectos (pero no los post-efectos).
 * 4) textura final, identificada con MO_FINAL_TEX, donde se almancena el resultado del render luego
 * de aplicar todos los efectos y post-efectos.
 */
class LIBMOLDEO_API moRenderManager:public moResource
{
	public:
		/**
		 * Constructor por defecto.
		 */
		moRenderManager ();

		/**
		 * Destructor por defecto.
		 */
		virtual ~ moRenderManager ();

		/**
		 * Crea un lock en el thread de render para evitar conflictos de recursos.
		 * @return el resultado de la operacin: true o false.
		 */
		bool Lock ();

		/**
		 * Destraba el lock creado con la funcin Lock.
		 * @return el resultado de la operacin: true o false.
		 */
		bool Unlock ();

		/**
		 * Mtodo de inicializacin. Las dimensiones de render son distintas de las dimensiones de la pantalla,
		 * esto permite renderear en baja resolucin, por ejemplo 320x200, y lueg re-escalar el resultado del render
		 * a una resolucin ms alta.
		 * @param p_render_to_texture_mode si es true, entonces el render se hace a un FBO, si es false, se utiliza el framebuffer de la pantalla.
		 * @param p_screen_width ancho de la pantalla.
		 * @param p_screen_height alto de la pantalla.
		 * @param p_render_width ancho de render.
		 * @param p_render_height alto de render.
		 * @return el resultado de la operacin: true o false.
		 */
		virtual MOboolean Init (moRenderManagerMode p_render_to_texture_mode,
			MOint p_screen_width, MOint p_screen_height,
			MOint p_render_width, MOint p_render_height);

		/**
		 * Mtodo de finalizacin.
		 * @return el resultado de la operacin: true o false.
		 */
		virtual MOboolean Finish ();
		void SetView (int p_width, int p_height);

		/**
		 * Devuelve true o false dependiendo si el render a FBO est habilitado.
		 * @return true si el render se est haciendo fuera de pantalla (a un FBO) o false si se est haciendo al framebuffer de la pantalla.
		 */
		MOboolean IsRenderToFBOEnabled ();

		/**
		 * Compara al resolucin de render con la resolucin de la pantalla y devuelve el resultado de la comparacin.
		 * @return true si las resoluciones de render y de pantalla son iguales, false en caso contrario.
		 */
		MOboolean RenderResEqualScreenRes ();

		/**
		 * Establece el modo de render (MO_RENDER_TO_TEXTURE_FBOBJECT o MO_RENDER_TO_TEXTURE_FBSCREEN).
		 * @param p_render_to_texture_mode nuevo modo de render.
		 */
		void SetRenderToTexMode (moRenderManagerMode p_render_to_texture_mode);

		/**
		 * Devuelve el modo de render utilizado en este momento.
		 * @return el modo de render actual.
		 */
		MOint GetRenderToTexMode ()
		{
			return m_render_to_texture_mode;
		}

		/**
		 * Esta funcin debe ser llamada antes del bucle de ejecucin de la funcin Update de los objetos y dispositivos
		 * de Moldeo en la consola.
		 */
		void BeginUpdate ();

		/**
		 * Esta funcin debe ser llamada antes de la ejecucin de la funcin Update de los dispositivos
		 * de Moldeo en la consola. Crea un lock en el thread de render y guarda la configuracin de OpenGL.
		 */
		void BeginUpdateDevice ();

		/**
		 * Esta funcin debe ser llamada luego de la ejecucin de la funcin Update de los dispositivos
		 * de Moldeo en la consola. Destraba el lock en el thread de render y restaura la configuracin de OpenGL.
		 */
		void EndUpdateDevice ();

		/**
		 * Esta funcin debe ser llamada luego del bucle de ejecucin de la funcin Update de los objetos y dispositivos
		 * de Moldeo en la consola.
		 */
		void EndUpdate ();

		/**
		 * Esta funcin debe ser llamada antes de la ejecucin de la funcin Update de los objetos
		 * de Moldeo en la consola. Crea un lock en el thread de render y guarda la configuracin de OpenGL.
		 */
		void BeginUpdateObject ();

		/**
		 * Esta funcin debe ser llamada luego de la ejecucin de la funcin Update de los objetos
		 * de Moldeo en la consola. Destraba el lock en el thread de render y restaura la configuracin de OpenGL.
		 */
		void EndUpdateObject ();

		/**
		 * Esta funcin debe ser llamada antes del bucle de dibujo de los efectos de Moldeo en la consola.
		 * Si se est usando el render a FBO, se activa el FBO correspondiente junto con los bfers de color.
		 */
		void BeginDraw ();

		/**
		 * Esta funcin debe ser llamada antes de la ejecucin de la funcin Draw de los efectos
		 * de Moldeo en la consola. Crea un lock en el thread de render, guarda la configuracin de OpenGL y
		 * configura un viewport en perspectiva con las dimensiones de render de pantalla.
		 */
		void BeginDrawEffect ();

		/**
		 * Esta funcin debe ser llamada luego de la ejecucin de la funcin Draw de los efectos
		 * de Moldeo en la consola. Destraba el lock en el thread de render y restaura la configuracin de OpenGL.
		 */
		void EndDrawEffect ();

		/**
		 * Esta funcin debe ser llamada luego del bucle de dibujo de los efectos de Moldeo en la consola.
		 * Si se est usando el render a FBO, se desactiba el FBO correspondiente.
		 */
		void EndDraw ();

		/**
		 * Dibuja el contenido de la textura de render con la resolucin requerida (MO_SCREEN_RESOLUTION o.
		 * MO_RENDER_RESOLUTION).
		 * @param p_resolution resolucin con que es dibujada la textura.
		 */
		void DrawRenderTexure (MOint p_resolution)
		{
			DrawTexture (p_resolution, 0);
		}
		/**
		 * Dibuja el contenido de la textura especificada (MO_RENDER_TEX, MO_SCREEN_TEX, MO_EFFECTS_TEX o
		 * MO_FINAL_TEX) con la resolucin requerida (MO_SCREEN_RESOLUTION o MO_RENDER_RESOLUTION).
		 * @param p_resolution resolucin con que es dibujada la textura.
		 * @param p_tex_num textura a dibujar.
		 */
		void DrawTexture (MOint p_resolution, MOint p_tex_num);

		/**
		 * Dibuja el contenido de la textura especificada (MO_RENDER_TEX, MO_SCREEN_TEX, MO_EFFECTS_TEX o
		 * MO_FINAL_TEX) con la resolucin requerida en alto x ancho.
		 * @param p_width alto con el que es dibujada la textura especificada.
		 * @param p_height ancho con el que es dibujada la textura especificada.
		 * @param p_tex_num textura a dibujar.
		 */
		void DrawTexture (MOint p_width, MOint p_height, MOint p_tex_num);

		/**
		 * Copia el contenido de la pantalla (o del bfer de dibujo si est haciendo off-screen rendering a un FBO)
		 * a la textura MO_SCREEN_TEX.
		 */
		void SaveScreen ();

		/**
		 * Copia el contenido de la pantalla (o del bfer de dibujo si est haciendo off-screen rendering a un FBO)
		 * a la textura especificada (MO_RENDER_TEX, MO_SCREEN_TEX, MO_EFFECTS_TEX o MO_FINAL_TEX).
		 * @param p_tex_num textura de destino.
		 */
		void CopyRenderToTexture (MOint p_tex_num);

		/**
		 * Devuelve el OpenGL ID de la textura especificada (MO_RENDER_TEX, MO_SCREEN_TEX, MO_EFFECTS_TEX o MO_FINAL_TEX).
		 * @return p_tex_num textura de la cual se pude el OpenGL ID.
		 */
		MOint RenderTexGLId (MOint p_tex_num);

		/**
		 * Devuelve ancho de la pantalla.
		 * @return ancho de la pantalla.
		 */
		MOint ScreenWidth ();

		/**
		 * Devuelve alto de la pantalla.
		 * @return alto de la pantalla.
		 */
		MOint ScreenHeight ();

		/**
		 * Devuelve ancho de render.
		 * @return ancho de render.
		 */
		MOint RenderWidth ()
		{
			return m_render_width;
		}

		/**
		 * Devuelve alto de render.
		 * @return alto de render.
		 */
		MOint RenderHeight ()
		{
			return m_render_height;
		}

		/**
		 * Verifica que la placa grfica soporte texturas mltiples.
		 * @return true si la capacidad de texturas mltiples est soportada, false en caso contrario.
		 */
		MOboolean MultiTextureSupported ();

		/**
		 * Verifica que la placa grfica soporte FrameBuffer Objects (FBO).
		 * @return true si la capacidad de FBOs est soportada, false en caso contrario.
		 */
		MOboolean FramebufferObjectSupported ();

		/**
		 * Verifica que la placa grfica soporte shaders.
		 * @return true si la capacidad de shaders est soportada, false en caso contrario.
		 */
		MOboolean ShadersSupported ();
		void SetOutputConfiguration (moRenderOutputConfiguration
			p_output_configuration);
		moRenderOutputConfiguration GetOutputConfiguration ();
		moBucketsPool * GetFramesPool ()
		{
			return m_pFramesPool;
		}
	protected:
		moLock m_RenderLock;
		MOboolean m_saved_screen;
		moTextureManager * m_pTextureManager;
		moGLManager * m_pGLManager;
		moFBManager * m_pFBManager;

		moDecoderManager *m_pDecoderManager;
		moRenderOutputConfiguration m_OutputConfiguration;
		moBucketsPool * m_pFramesPool;
		moRenderManagerMode m_render_to_texture_mode;
		MOint m_screen_width, m_screen_height;
		MOint m_render_width, m_render_height;
		MOint m_render_tex_moid[6];
		MOuint m_render_attach_points[4];
		MOuint m_fbo_idx;
		MOboolean ValidSourceTexNum (MOint p_tex_num)
		{
			return (0 <= p_tex_num) && (p_tex_num < 4);
		}
		MOboolean ValidDestTexNum (MOint p_tex_num)
		{
			return (0 < p_tex_num) && (p_tex_num < 4);
		}
};
#endif							 /*  */
