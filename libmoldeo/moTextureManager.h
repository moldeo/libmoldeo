/*******************************************************************************

                                moTextureManager.h

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

  Copyright(C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andres Colubri

*******************************************************************************/

#ifndef __MO_TEXTURE_MANAGER_H
#define __MO_TEXTURE_MANAGER_H

#include "moTexture.h"
#include "moResourceManager.h"
#include <moFileManager.h>

typedef moTextureMemory* moTextureFramePtr;

moDeclareExportedDynamicArray(moTextureFramePtr,moTextureFrames);

/// Buffer de imágenes
/**
    Clase que permite cargar una sucesión de imágenes de mismo formato con compresión incluída dentro
    de un buffer de memoria y a la vez permite reproducir cada cuadro de forma independiente, descomprimiendo de memoria
    y asignando una textura
*/
enum moTextureBufferFormat {
    MO_TEXTURE_BUFFER_FORMAT_LEVELS = 0,
    MO_TEXTURE_BUFFER_FORMAT_PATTERNS = 1
};

class LIBMOLDEO_API moTextureBuffer : public moAbstract {

	public:
		moTextureBuffer();
		virtual ~moTextureBuffer();

		virtual MOboolean  Init();
		virtual MOboolean  Init( moText p_foldername, moText p_bufferformat, moResourceManager* p_pResourceManager );
		virtual MOboolean  Init( moText p_foldername, moTextureBufferFormat p_bufferformat, int p_pattern_width, int p_pattern_height, moResourceManager* p_pResourceManager );
		virtual MOboolean  Finish();

		virtual int GetFrame( MOuint p_i );

		virtual void ReleaseFrame( MOuint p_i );

		virtual moTextureMemory* GetTexture( MOuint p_i );

		MOboolean UpdateImages( int maxfiles = -1 );

        MOboolean	LoadCompleted();
        int GetImagesProcessed() { return  m_ImagesProcessed; }

		MOboolean LoadImage( moText p_ImageName, moBitmap* pImage, int indeximage );

		moText GetBufferPath() { return m_BufferPath; }
		moText	GetBufferFormat() { return m_BufferFormat; }

		moText GetName() { return m_FolderName; }

        moTextureFrames& GetBufferLevels( int L, int C );
        moTextureFrames& GetBufferPatterns( moTexture* p_ImageReference, int x, int y, int width=0, int height=0 );
        moTextureFrames& GetBufferPatterns( const moTexture& p_ImageReference, int x, int y, int width=0, int height=0 );
        MOuint GetTextureArray( int width, int height, int levels, bool force_load = false  );

        MOuint m_texture_array;

        int max_luminance;
        int min_luminance;
        int max_contrast;
        int min_contrast;

        //size of max_luminance
        MObyte*   LevelDiagram;

	private:

        int pattern_width;
        int pattern_height;

		MOint m_ImagesProcessed;
		MOboolean	m_bLoadCompleted;
		MOint m_ActualImage;


        moText  m_FolderName;
		moText	m_BufferPath;
		moText  m_BufferFormat;

		moResourceManager*	m_pResourceManager;

		moTextureFrames     m_Frames;
		moDirectory*	m_pDirectory;

		///max level contrast = 10
		///max level luminance = 10
		moTextureFrames**  m_pBufferLevels;
		moTextureFrames*  m_pBufferPatterns;

};

typedef moTextureBuffer* moTextureBufferPtr;

moDeclareExportedDynamicArray(moTextureBufferPtr,moTextureBuffers);



/**
*   Administrador de texturas
*   permite crear, modificar y eliminar recursos de texturas
*   Esta clase utiliza internamente la librería freeimage (http://www.freeimage.org)
*   para cargar archivos de imágenes desde el disco o desde la memoria por lo tanto los formatos soportados son aquellos
*   soportados por la librería en cuestión, hasta ahora: JPG,PNG,TGA,TIFF,BMP,
*
*/
class LIBMOLDEO_API moTextureManager : public moResource
{
	public:

        /**
         * Constructor genérico de la clase.
         */
		moTextureManager();

        /**
         * Destructor genérico de la clase.
         */
		virtual ~moTextureManager();

        /**
         * Inicializador.
         */
		virtual MOboolean Init();

        /**
         * Finalizador.
         */
		virtual MOboolean Finish();

        /**
         * Devuelve el Moldeo ID interno de la textura con parámetros param. Si dicha textura no existiese y
         * p_create_tex fuese true, entonces a nueva textura vacía es creada utilizando los parámetros provistos.
         * @param param parámetros que especifican la textura a localizar.
         * @param p_create_tex si es true y la textura no es encontrada, entonces una nueva textura es creada.
         * @return Moldeo ID.
         */
		MOint GetTextureMOId(moParam *param, MOboolean p_create_tex);

        /**
         * devuelve el id interno de la textura
         */
		MOint GetTextureMOId(moValue *value, MOboolean p_create_tex);


        /**
         * Devuelve el Moldeo ID interno de la textura con nombre p_name. Si dicha textura no existiese y
         * p_create_tex fuese true, entonces a nueva textura vacía es creada utilizando parámetros por defecto
         * y el nombre provisto.
         * @param p_name nombre utilizado para buscar la textura.
         * @param p_create_tex si es true y la textura no es encontrada, entonces una nueva textura es creada.
         * @return Moldeo ID.
         */
		MOint GetTextureMOId(moText p_name, MOboolean p_create_tex, bool p_refresh=false );

        /**
         * Devuelve el Moldeo ID interno de la textura con OpenGL ID p_glid.
         * @param p_glid OpenGL ID de la textura a buscar.
         * @return Moldeo ID.
         */
		MOint GetTextureMOId(MOuint p_glid);

        /**
         * Devuelve el Moldeo ID interno del texturebuffer con nombre de carpeta p_name. Si dicho objeto no existiese y
         * p_create_tex fuese true, entonces a nueva textura vacía es creada utilizando parámetros por defecto
         * y el nombre provisto.
         * @param p_name nombre utilizado para buscar el objeto.
         * @param p_create_tex si es true y el objeto no es encontrado, entonces un nuevo objeto es creado.
         * @return Moldeo ID.
         */
        MOint GetTextureBuffer( moText p_foldername, MOboolean p_create_tex = true, moText p_bufferformat = "JPGAVERAGE" );

        moTextureBuffer* GetTextureBuffer( int idx );

        /**
         * Agrega una textura de tipo p_type, nombre p_name y parámetros p_tex_param.
         * @param p_type tipo de la textura a crear (MO_TYPE_TEXTURE, MO_TYPE_TEXTURE_MULTIPLE, MO_TYPE_MOVIE, etc.).
         * @param p_name nombre de la textura a crear.
         * @param p_tex_param parámetros de la textura a crear.
         * @return Moldeo ID de la nueva textura (-1 si la textura no puede ser agregada).
         */
		MOint AddTexture(MOuint p_type, moText p_name, moTexParam p_tex_param = MODefTex2DParams);

        /**
         * Agrega la textura especificada en p_param.
         * @param p_param parámetros de la textura a agregar.
         * @return Moldeo ID de la nueva textura (-1 si la textura no puede ser agregada).
         */
		MOint AddTexture(moParam* p_param);

        /**
         * Agrega una textura de nombre p_name, ancho p_width, alto p_height y parámetros p_tex_param.
         * @param p_name nombre de la textura a agregar.
         * @param p_width ancho de la textura a agregar.
         * @param p_height alto de la textura a agregar.
         * @param p_tex_param parámetros de la textura a agregar.
         * @return Moldeo ID de la nueva textura (-1 si la textura no puede ser agregada).
         */
		MOint AddTexture(moValue* p_value);

        /**
         * agrega una textura
         */
		MOint AddTexture(moText p_name, MOuint p_width, MOuint p_height, moTexParam p_tex_param = MODefTex2DParams);

        /**
         * Agrega una textura de nombre p_name, ancho p_width, alto p_height y parámetros p_tex_param, y es inicializada
         * con el contenido del búfer apuntado por p_buffer.
         * @param p_name nombre de la textura a agregar.
         * @param p_width ancho de la textura a agregar.
         * @param p_height alto de la textura a agregar.
         * @param p_buffer búfer con el contenido a copiar en la nueva textura.
         * @param p_pixel_format formato del píxel en el búfer.
         * @param p_pixel_type píxel de píxel en el búfer.
         * @param p_tex_param parámetros de la textura a agregar.
         * @return Moldeo ID de la nueva textura (-1 si la textura no puede ser agregada).
         */
		MOint AddTexture(moText p_name, MOuint p_width, MOuint p_height, GLvoid* p_buffer, GLenum p_pixel_format = GL_RGBA, GLenum p_pixel_type = GL_UNSIGNED_BYTE, moTexParam p_tex_param = MODefTex2DParams);

        /**
         * Agrega una textura, leyéndola del archivo p_filename.
         * @param p_filename nombre de archivo con la imágen o película a cargar.
         * @return Moldeo ID de la nueva textura (-1 si la textura no puede ser agregada).
         */
		MOint AddTexture(moText p_filename);

        /**
         * Agrega un texturebuffer (carpeta de texturas), leyéndolo de la carpeta p_foldername.
         * @param p_foldername nombre de carpeta con las imágenes a cargar.
         * @return Moldeo ID de la nueva textura (-1 si el objeto no puede ser agregado).
         */
        MOint AddTextureBuffer( moText p_foldername, moText p_bufferformat = "JPGAVERAGE" );


        /**
         * Elimina la textura p_moid.
         * @param p_moid Moldeo ID de la textura a eliminar.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean DeleteTexture(MOuint p_moid);

        /**
         * Devuelve el número de texturas en el administrador.
         * @return número de texturas.
         */
		MOuint GetTextureCount() { return m_textures_array.Count(); }

        /**
         * Devuelve el puntero a la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return puntero a la textura obtenida.
         */
        moTexture* GetTexture(MOuint p_moid) { return m_textures_array[p_moid]; }

        /**
         * Devuelve el tipo de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return tipo de textura (MO_TYPE_TEXTURE, MO_TYPE_TEXTURE_MULTIPLE, etc.).
         */
		MOint GetType(MOuint p_moid);

        /**
         * Devuelve el OpenGL ID de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return OpenGL ID de la textura.
         */
		MOuint GetGLId(MOuint p_moid);

        /**
         * Devuelve el nombre de la textura con Moldeo ID p_moid.
         * El nombre de la textura contiene siempre el directorio relativo al directorio de datos y el nombre del archivo.
         * @param p_moid Moldeo ID de la textura.
         * @return nombre de la textura.
         * @see moDataManager
         */
        moText GetName(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetName() : moText(""); }

        /**
         * Devuelve el ancho de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return ancho de la textura.
         */
		MOint GetWidth(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetWidth() : MO_UNDEFINED; }

        /**
         * Devuelve el alto de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return alto de la textura.
         */
		MOint GetHeight(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetHeight() : MO_UNDEFINED; }

        /**
         * Devuelve el ancho de datos de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return ancho de datos de la textura.
         */
		MOint GetDataWidth(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetDataWidth() : MO_UNDEFINED; }

        /**
         * Devuelve el alto de datos de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return alto de datos de la textura.
         */
        MOint GetDataHeight(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetDataHeight() : MO_UNDEFINED; }

        /**
         * Devuelve el número de componentes por píxel de la textura con Moldeo ID p_moid:
         * 3 = RGB 24 bits
         * 4 = RGBA 32 bits
         * @param p_moid Moldeo ID de la textura.
         * @return número de componentes.
         */
		MOint GetComponents(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetComponents() : MO_UNDEFINED; }

        /**
         * Devuelve el número total de téxels (alto x ancho x número de componentes) de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return número de téxels.
         */
		MOlong GetTexelCount(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetTexelCount() : MO_UNDEFINED; }

        /**
         * Devuelve el número total de téxels de datos (alto de datos x ancho de datos x número de componentes)
         * de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return número de téxels de datos.
         */
		MOlong GetDataTexelCount(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetDataTexelCount() : MO_UNDEFINED; }

        /**
         * Devuelve el valor máximo de la coordenada de textura S de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return valor máximo de S.
         */
        MOfloat GetMaxCoordS(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetMaxCoordS() : MO_UNDEFINED; }

        /**
         * Devuelve el valor máximo de la coordenada de textura T de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return valor máximo de T.
         */
        MOfloat GetMaxCoordT(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetMaxCoordT() : MO_UNDEFINED; }

        /**
         * Devuelve los parámetros de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return parámetros de textura.
         */
   		moTexParam GetTexParam(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetTexParam() : MOUndefinedTex; }

        /**
         * Devuelve el tárget OpenGL (GL_TEXTURE_2D, GL_TEXTURE_RECT, etc.) de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return target de la textura.
         */
        GLenum GetTexTarget(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetTexTarget() : MO_UNDEFINED; }

        /**
         * Devuelve el formato interno de la textura con Moldeo ID p_moid, es decir, el número de componentes de color de la misma
         * (GL_RGBA, GL_RGB, etc.).
         * @param p_moid Moldeo ID de la textura.
         * @return formato interno de la textura.
         */
   		GLenum GetTexInternalFormat(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetTexInternalFormat() : MO_UNDEFINED; }

        /**
         * Devuelve el método utilizado en el filtro de minificación (GL_NEAREST, GL_LINEAR, GL_NEAREST,
         * GL_NEAREST_MIPMAP_NEAREST, etc.) en la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return método de filtrado de minificación.
         */
        GLint GetMinFilter(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetMinFilter() : MO_UNDEFINED; }

        /**
         * Devuelve el método utilizado en el filtro de magnificación (GL_NEAREST o GL_LINEAR) en la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return método de filtrado de magnificación.
         */
        GLint GetMagFilter(MOuint p_moid)  { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetMagFilter() : MO_UNDEFINED; }

        /**
         * Devuelve el método de envolvimiento utilizado en la coordenada de textura S (GL_CLAMP, GL_REPEAT)
         * en la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return método de envolvimiento en coordenada S.
         */
		GLint GetWrapS(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetWrapS() : MO_UNDEFINED; }

        /**
         * Devuelve el método de envolvimiento utilizado en la coordenada de textura T (GL_CLAMP, GL_REPEAT)
         * en la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return método de envolvimiento en coordenada T.
         */
        GLint GetWrapT(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetWrapT() : MO_UNDEFINED; }

        /**
         * Devuelve el FBO actualmente asociado a la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return puntero al FBO.
         */
        moFBO* GetFBO(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetFBO() : NULL; }

        /**
         * Devuelve el punto de attachement de la textura con Moldeo ID p_moid.
         * @param p_moid Moldeo ID de la textura.
         * @return índice del punto de attachement (0 para GL_COLOR_ATTACHMENT0_EXT, 1 para GL_COLOR_ATTACHMENT1_EXT, etc.).
         */
        MOint GetFBOAttachPoint(MOuint p_moid) { return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetFBOAttachPoint() : MO_UNDEFINED; }

        /**
         * Valida el Moldeo ID provisto.
         * @param p_moid Moldeo ID a validar.
         * @return true si la textura existe, false en caso contrario.
         */
		MOboolean ValidTexture(MOuint p_moid) {	return m_textures_array[p_moid] != NULL; }

        /**
         * Devuelve el tipo de archivo que corresponde a p_filename (MO_TYPE_TEXTURE, MO_TYPE_TEXTURE_MULTIPLE, etc.).
         * @return el tipo de textura.
         */
		MOuint GetTypeForFile(moText p_filename);

    void RefreshAll();

		virtual void Update(moEventList *Events);


		int         m_preview_texture_idx;


	protected:
		moGLManager* m_glmanager;
		moFBManager* m_fbmanager;
		moTextureArray m_textures_array;

		moTextureBuffers    m_textures_buffers;


        moTexture* CreateTexture(MOuint p_type, moText p_name, moTexParam p_param = MODefTex2DParams);
		MOboolean CreateMultipleTex(moTextureMultiple* p_tex_mult, moParam* p_param);
		MOboolean CreateMultipleTex(moTextureMultiple* p_tex_mult, moValue* p_value);
};


#endif

