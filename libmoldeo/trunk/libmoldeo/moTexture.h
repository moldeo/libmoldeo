/*******************************************************************************

                                 moTexture.h

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

#ifndef __MO_TEXTURE_H__
#define __MO_TEXTURE_H__

#include "moTypes.h"
#include "moText.h"
#include "moParam.h"
#include "moBuckets.h"
#include "moVideoGraph.h"
#include "moGsGraph.h"
#include "moDsGraph.h"
#include "moMathVector3.h"

typedef void moBitmap;
typedef int moBitmapFormat;
typedef void moMemory;

/// Tipos de texturas
/**
* Tipos de texturas
*/
typedef enum  {
        MO_TYPE_TEXTURE, /// TEXTURA BASE
        MO_TYPE_TEXTURE_MULTIPLE, /// textura múltiple
        MO_TYPE_MOVIE,/// película
        MO_TYPE_VIDEOBUFFER,/// buffer de video
        MO_TYPE_TEXTUREMEMORY,/// textura en memoria
        MO_TYPE_TEXTUREBUFFER,/// buffer de texturas
        MO_TYPE_CIRCULARVIDEOBUFFER /// buffer circular de video
} moTextureType;

class moFBO;
class moTexture;
class moGLManager;
class moDataManager;
class moFile;
class moFileManager;
class moResourceManager;


/// clase base para el manejo de una textura
/**
 * Clase que encapsula una textura openGL y provee funcionalidades básicas tales como copiar contenidos desde o hacia
 * búfer, leer desde un archivo, manejar el FBO al cual pueda estar asociada la textura, etc.
 *  @see moTextureManager
 *  @see moVideoManager
 */
class LIBMOLDEO_API moTexture : public moAbstract {
    public:
        /**
         * Constructor por defecto.
         */
		moTexture();
        /**
         * Destructor por defecto.
         */
		virtual ~moTexture();

        /**
         * Inicializa propiedades básicas de la textura tales como el nombre, id, administradores y parámetros.
         * @param p_name nombre de la textura.
         * @param p_moid Moldeo ID de la textura.
         * @param p_res puntero al administrador de recursos, del cual son extraidos todos los administradores que la textura necesita (GL, Data, File, etc).
         * @param p_param parámetros de textura.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		virtual MOboolean Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param = MODefTex2DParams);
        /**
         * Destruye la textura openGL.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		virtual MOboolean Finish();

        /**
         * Construye una textura vacía con el ancho y alto especificados.
         * @param p_width ancho de la textura.
         * @param p_heigh alto de la textura.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean BuildEmpty(MOuint p_width, MOuint p_height);
        /**
         * Construye una textura con el ancho, alto, formato y tipo especificados, copiando en la misma el contenido apuntado por p_buffer.
         * @param p_width ancho de la textura.
         * @param p_heigh alto de la textura.
         * @param p_buffer buffer utilizado para inicializar el contenido de la textura.
         * @param p_format formato de la textura, por ejemplo: GL_RGBA o GL_RGB.
         * @param p_type tipo de la textura, por ejemplo: GL_UNSIGNED_BYTE o GL_FLOAT.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean BuildFromBuffer(MOuint p_width, MOuint p_height, const GLvoid* p_buffer, GLenum p_format = GL_RGBA, GLenum p_type = GL_UNSIGNED_BYTE);
        /**
         * Construye una textura a partir de la imágen contenida en p_filename.
         * @param p_filename nombre del archivo que contiene la imágen a cargar en la textura.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean BuildFromFile(moText p_filename);

        /**
         * Revisa que el archivo especificado con p_filename sea un archivo de imágen soportado. En estos momentos, los
         * formatos soportados son tga jpg, png, gif, bmp, xpm, y ppm.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean SupportedFile(moText p_filename);

        /**
         * Construye una textura a partir de la imágen especificada en los parámetros p_param.
         * @param p_param puntero a un objeto de parámetros que contiene el nombre del archivo del cual la textura será leida.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean Load( moParam* p_param );
		MOboolean Load( moValue* p_value );

        /**
         * Copia a la textura el búfer pasado como parametro, asumiendo que el mismo tiene el alto y ancho correcto.
         * @param p_buffer puntero al búfer en memoria que será copiado a la textura.
         * @param p_format formato de los datos contenidos en el búfer (GL_RGBA, GL_RGB, etc.).
         * @param p_type tipo de los datos contenidos en el búfer (GL_UNSIGNED_BYTE, GL_FLOAT, etc.).
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean SetBuffer( const GLvoid* p_buffer, GLenum p_format = GL_RGBA, GLenum p_type = GL_UNSIGNED_BYTE);
        /**
         * Copia a la textura el búfer pasado como parametro, y usando los parametros de alto y ancho como las nuevas dimensiones de la textura.
         * @param p_width ancho de los datos contenidos en el búfer.
         * @param p_height alto de los datos contenidos en el búfer.
         * @param p_buffer puntero al búfer en memoria que será copiado a la textura.
         * @param p_format formato de los datos contenidos en el búfer (GL_RGBA, GL_RGB, etc.).
         * @param p_type tipo de los datos contenidos en el búfer (GL_UNSIGNED_BYTE, GL_FLOAT, etc.).
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean SetBuffer(MOuint p_width, MOuint p_height, const GLvoid* p_buffer, GLenum p_format = GL_RGBA, GLenum p_type = GL_UNSIGNED_BYTE);

        /**
         * Copia el contenido actual de la textura al búfer pasado como parametro.
         * @param p_buffer puntero al búfer en memoria donde será copiada la textura.
         * @param p_format especifica el formato de píxel de los datos copiados al búfer (GL_RGBA, GL_RGB, etc.).
         * @param p_type especifica el tipo de píxel de los datos copiados al búfer (GL_UNSIGNED_BYTE, GL_FLOAT, etc.).
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean GetBuffer(GLvoid* p_buffer, GLenum p_format = GL_RGBA, GLenum p_type = GL_UNSIGNED_BYTE);

        /**
         * Utiliza p_gl como nuevo administrador de OpenGL de la textura.
         * @param p_gl puntero al administrador de OpenGL.
         */
		void SetGLMan(moGLManager* p_gl) { m_gl = p_gl; }
        /**
         * Utiliza p_dataman como nuevo administrador de datos de la textura.
         * @param p_dataman puntero al administrador de datos.
         */
		void SetDataMan(moDataManager* p_dataman) { m_pDataMan = p_dataman; }
        /**
         * Utiliza p_fileman como nuevo administrador de archivos de la textura.
         * @param p_fileman puntero al administrador de archivos.
         */
		void SetFileMan(moFileManager* p_fileman) { m_pFileMan = p_fileman; }

        /**
         * Utiliza p_fbo como nuevo objeto de Framebuffer para la textura.
         * @param p_fbo puntero al FBO.
         */
		void SetFBO(moFBO* p_fbo) { m_fbo = p_fbo; }
        /**
         * Utiliza p_fbo como nuevo objeto de Framebuffer para la textura, además de generar un nuevo
         * punto de attachement de la textura en el FBO.
         * @param p_fbo puntero al FBO.
         * @return índice del punto de attachement (0 para GL_COLOR_ATTACHMENT0_EXT, 1 para GL_COLOR_ATTACHMENT1_EXT, etc.).
         */
		MOuint SetFBOandAttachPoint(moFBO* p_fbo);
        /**
         * Utiliza p_fbo_attach_point como nuevo punto de attachement para la textura.
         * @param p_fbo_attach_point índice del nuevo punto de attachement (0 para GL_COLOR_ATTACHMENT0_EXT, 1 para GL_COLOR_ATTACHMENT1_EXT, etc.).
         */
		void SetFBOAttachPoint(MOuint p_fbo_attach_point) { m_fbo_attach_point = p_fbo_attach_point; }
        /**
         * Devuelve el FBO actualmente asociado a la textura.
         * @return puntero al FBO.
         */
		moFBO* GetFBO() { return m_fbo; }
        /**
         * Devuelve el punto de attachement de la textura.
         * @return índice del punto de attachement (0 para GL_COLOR_ATTACHMENT0_EXT, 1 para GL_COLOR_ATTACHMENT1_EXT, etc.).
         */
		MOuint GetFBOAttachPoint() { return m_fbo_attach_point; }

        /**
         * Devuelve el tipo de píxel de la textura.
         * @return tipo de píxel (GL_UNSIGNED_BYTE, GL_FLOAT, etc.).
         */
		moTextureType GetType() { return m_type; }
        /**
         * Devuelve el identificador OpenGL de la textura.
         * @return OpenGL ID de la textura.
         */
		MOuint GetGLId() { return m_glid; }
        /**
         * Devuelve el identificador Moldeo de la textura.
         * @return Moldeo ID de la textura.
         */
	    MOuint GetMOId() { return m_moid; }
        /**
         * Utiliza p_moid como nuevo Moldeo ID de la textura.
         * @param p_moid nuevo Moldeo ID.
         */
		void SetMOId(MOuint p_moid) { m_moid = p_moid; }
        /**
         * Devuelve el nombre de la textura.
         * @return nombre de la textura.
         */
		moText GetName() { return m_name; }
        /**
         * Utiliza p_name como nuevo nombre de la textura.
         * @param p_name nuevo nombre de la textura.
         */
		void SetName(moText p_name) { m_name = p_name; }

        /**
         * Devuelve el ancho de la textura.
         * @return ancho de la textura.
         */
		MOuint GetWidth() { return m_width; }
        /**
         * Devuelve el alto de la textura.
         * @return alto de la textura.
         */
		MOuint GetHeight() { return m_height; }
        /**
         * Si la textura es rectangular, devuelve el mismo valor que GetWidth. En caso contrario,
         * devuelve el ancho origial escalado por el rango máximo de la coordenada S de texturas.
         * @return ancho de datos de la textura.
         * @see GetWidth
         */
		MOuint GetDataWidth();
        /**
         * Si la textura es rectangular, devuelve el mismo valor que GetHeight. En caso contrario,
         * devuelve la altura origial escalada por el rango máximo de la coordenada T de texturas.
         * @return alto de datos de la textura.
         * @see GetHeight
         */
		MOuint GetDataHeight();
        /**
         * Devuelve el número de componentes por píxel.
         * @return número de componentes.
         */
		MOuint GetComponents() { return m_components; }
        /**
         * Devuelve el número total de téxels (alto x ancho x número de componentes).
         * @return número de téxels.
         */
		MOlong GetTexelCount() { return m_width * m_height * m_components; }
        /**
         * Devuelve el número total de téxels de datos (alto de datos x ancho de datos x número de componentes).
         * @return número de téxels de datos.
         */
		MOlong GetDataTexelCount() { return GetDataWidth() * GetDataHeight() * m_components; }

        /**
         * Devuelve el valor máximo de la coordenada S de textura.
         * @return valor máximo de S.
         */
		MOfloat GetMaxCoordS() { return m_max_coord_s; }
        /**
         * Devuelve el valor máximo de la coordenada T de textura.
         * @return valor máximo de T.
         */
		MOfloat GetMaxCoordT() { return m_max_coord_t; }

        /**
         * Devuelve los parámetros de la textura.
         * @return parámetros de textura.
         */
		moTexParam GetTexParam() { return m_param; }
        /**
         * Devuelve el tárget OpenGL de la textura (GL_TEXTURE_2D, GL_TEXTURE_RECT, etc.).
         * @return target de la textura.
         */
		GLenum GetTexTarget() { return m_param.target; }
        /**
         * Devuelve el formato interno de la textura, es decir, el número de componentes de color de la misma
         * (GL_RGBA, GL_RGB, etc.).
         * @return formato interno de la textura.
         */
		GLenum GetTexInternalFormat() { return m_param.internal_format; }
        /**
         * Devuelve el método utilizado en el filtro de minificación (GL_NEAREST, GL_LINEAR, GL_NEAREST,
         * GL_NEAREST_MIPMAP_NEAREST, etc.).
         * @return método de filtrado de minificación.
         */
		GLint GetMinFilter() { return m_param.min_filter; }
        /**
         * Devuelve el método utilizado en el filtro de magnificación (GL_NEAREST o GL_LINEAR).
         * @return método de filtrado de magnificación.
         */
		GLint GetMagFilter() { return m_param.mag_filter; }
        /**
         * Devuelve el método de envolvimiento utilizado en la coordenada S de textura (GL_CLAMP, GL_REPEAT).
         * @return método de envolvimiento en coordenada S.
         */
		GLint GetWrapS() { return m_param.wrap_s; }
        /**
         * Devuelve el método de envolvimiento utilizado en la coordenada T de textura (GL_CLAMP, GL_REPEAT).
         * @return método de envolvimiento en coordenada T.
         */
		GLint GetWrapT() { return m_param.wrap_t; }

        /**
         * Invierte verticalmente el búfer de imágen apuntado por pBuffer.
         * @param pBuffer puntero al búffer que contiene la imágen a invertir.
         * @param p_depth número de componentes de color por píxel.
         */
		void FlipBufferVert(MOubyte *pBuffer, MOint p_depth);

        /**
         * Copia a esta textura la que esta siendo apuntada por p_src_tex.
         * @param p_src_tex puntero al objeto de textura desde el cual se está realizando la copia.
         * @param p_copy_glid si es true entonces el OpenGL ID es copiado.
         * @param p_copy_moid si es true entonces el Moldeo ID es copiado.
         * @param p_copy_type si es true entonces el tipo de textura es copiado.
         * @param p_copy_name si es true entonces el nombre de la textura es copiado.
         */
		void CopyFromTex(moTexture* p_src_tex, MOboolean p_copy_glid, MOboolean p_copy_moid, MOboolean p_copy_type, MOboolean p_copy_name);
		moTexture &operator = (const moTexture &p_src_tex);

        /**
        *   Devuelve verdadero si esta textura fue cargada desde un archivo
        *   @return verdadero si fue cargada, falso si es cualquier otro tipo de textura
        */
		MOboolean       IsBuildedFromFile() {
                return m_bBuildedFromFile;
        }

        /**
        *   Salva al disco la textura especificando el nuevo tamaño y el formato
        *   El nombre del archivo es opcional, se toma en general el nombre de la textura
        *   y se le agrega: .thm.jpg   o  .thm.tga
        *   @param  p_bufferformat  el formato: "JPG" (75:1), "JPGSUPERB" (100:1), "JPGBAD" (10:1), "JPEGNORMAL" (50:1), "JPGAVERAGE" (25:1), "JPGGOOD" (75:1), "TGA"
        *   @param  w   ancho de la nueva imagen
        *   @param  h   alto de la nueva imagen
        *   @param  newfilename el nombre del archivo, si no especifica se usa: GetName()  y se le agrega: .thm.jpg   o  .thm.tga
        *   @return el nombre del archivo si fue salvado exitosamente, sino una cadena vacía moText("")
        */
        moText  CreateThumbnail( moText p_bufferformat, int w, int h, moText newfilename = moText("") );

        bool  CalculateLuminanceAndConstrast( int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0 );

        int  GetLuminance() { return Luminance; }
        int  GetContrast() { return Contrast; }

protected:

        MOboolean       m_bBuildedFromFile;

		moFile*			m_pFile;
		moDataManager*	m_pDataMan;
		moFileManager*	m_pFileMan;
        moGLManager* m_gl;
		moResourceManager*	m_pResourceManager;

		moFBO* m_fbo;
		MOuint m_fbo_attach_point;

		moTextureType m_type;
    MOint m_moid;
    MOuint m_glid;
		moText m_name;

		moTexParam m_param;
		MOuint m_width;
		MOuint m_height;
		MOint m_components;
		MOfloat m_max_coord_s;
		MOfloat m_max_coord_t;

		void SetParam();
		void CalculateSize(MOuint p_width, MOuint p_height);
		MOuint NextPowerOf2(MOuint p_seed);
		MOboolean Build();

		int Luminance;
		int Contrast;
		moDWord Histogram[256];
};


moDeclareExportedDynamicArray( moTexture*, moTextureArray);



/// clase base para el manejo de una textura con su copia comprimida en memoria
/**
 *  Esta clase mantiene en memoria una copia comprimida de la imagen
 *  a medida que esta imagen es requerida se aumenta el numero de referencia
 *  cuando la cantidad de referencias llegan a cero se da de baja de la memoria de opengl
 *  @see moTexture
 *  @see moTextureBuffer
 *  @see moTextureManager
 *  @see moVideoManager
 */
class LIBMOLDEO_API moTextureMemory : public moTexture {
    public:
        /**
         * Constructor por defecto.
         */
		moTextureMemory();
        /**
         * Destructor por defecto.
         */
		virtual ~moTextureMemory();

        /**
         * Inicializa propiedades básicas de la textura tales como el nombre, id, administradores y parámetros.
         * @param p_name nombre de la textura.
         * @param p_moid Moldeo ID de la textura.
         * @param p_res puntero al administrador de recursos, del cual son extraidos todos los administradores que la textura necesita (GL, Data, File, etc).
         * @param p_param parámetros de textura.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		virtual MOboolean Init( moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param = MODefTex2DParams);

		virtual MOboolean Init( moText p_name, MOuint p_moid, moResourceManager* p_res, moText bufferformat, moBitmap* pImageResult, moTexParam p_param = MODefTex2DParams );

        //virtual MOboolean BuildFromFile( moText p_filename );

        //virtual MOboolean BuildFromFile( moText p_filename, moText p_bufferformat );

        ///Construye la textura opengl desde el bitmap en memoria
        virtual MOboolean BuildFromMemory();

        ///Guarda el bitmap con el formato elegido en memoria, luego construye la textura opengl
        virtual MOboolean BuildFromBitmap( moBitmap* p_bitmap, moText p_bufferformat = "JPG" );


        ///Guarda el bitmap con el formato elegido en memoria
        virtual MOboolean LoadFromBitmap( moBitmap* p_bitmap );


        /**
         * Destruye la textura openGL.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		virtual MOboolean Finish();

		/**
         * Devuelve una referencia (internamente marca la referencia y asigna la imagen a la textura)
         * Para liberar la memoria de opengl hay q desreferenciar...
         * @return la cantidad de referencias (si es 0 es que no pudo cargar nada, asi q la referencia no sirve)
         */
		int  GetReference();

		/**
         * Libera una referencia a esta textura pudiendo de esta manera, liberar la memoria de opengl...
         */
		void  ReleaseReference();

        int GetContrast() { return Contrast; }

        int GetLuminance() { return Luminance; }


        ///0 left up, 1 right up, 2, left down, 3, righ down
        moVector3f  GetColorMatrixCorner( int corner ) { return color_matrix[corner]; }
        moVector3f  GetColorAverage() { return color_average; }

    private:

        //four corners average color
        moVector3f      color_matrix[4];
        moVector3f      color_average;

        MOint       reference_counter;

        MOint		options;

        moMemory	*hmem;
		moBitmapFormat fif;
		//FIBITMAP *m_pImage;

		MOlong m_SizeInMemory;

		moText m_BufferFormat;
		MOboolean   m_bBitmapInMemory;

        ///average luminance
		MOint   Luminance;

		///average contrast
		MOint   Contrast;

		///Histogram
		moDWord Histogram[256];

};

#include "moTextureIndex.h"

class moShader;
class moTextureFilter;

/// una textura asociada a una animación de cuadros
/**
 * Clase de la que derivan las texturas animadas, y todas las que supongan tratar con
 * una sucesión de imágenes.
 * @see moMovie
 * @see moTextureMultiple
 */
class LIBMOLDEO_API moTextureAnimated : public  moTexture
{
	public:
        /**
         * Constructor por defecto.
         */
		moTextureAnimated();
        /**
         * Destructor por defecto.
         */
		virtual ~moTextureAnimated();

        /**
         * Inicializa propiedades básicas de la textura tales como el nombre, id, administradores y parámetros.
         * @param p_name nombre de la textura.
         * @param p_moid Moldeo ID de la textura.
         * @param p_res puntero al administrador de recursos, del cual son extraidos todos los administradores que la textura necesita (GL, Data, File, etc).
         * @param p_param parámetros de textura.
         */
		virtual MOboolean  Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param = MODefTex2DParams);
        /**
         * Destruye la textura openGL.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		virtual MOboolean  Finish();

        /**
         * Play de la animación, corre la animación según su velocidad propia
         */
    virtual void Play();

        /**
         * Para la animación, mostrando el cuadro del principio
         */
    virtual void Stop();

        /**
         * Pregunta si esta corriendo
         *
         */
    virtual bool IsPlaying();


        /**
         * Devuelve el OpenGL ID de textura que corresponde al tempo pasado como parámetro.
         * @param tempo puntero al objeto de tempo.
         * @return OpenGL ID.
         */
		virtual MOint		GetGLId( moTempo *tempo );
        /**
         * Devuelve el OpenGL ID de la sub-textura i-ésima.
         * @param p_i índice del cuadro.
         * @return OpenGL ID.
         */
		virtual MOint		GetGLId( MOuint p_i );
        /**
         * Devuelve el OpenGL ID de la textura, pasando como parámetro una fracción de ciclo 0 = cuadro 0, 1.0 = último cuadro.
         * @param p_cycle fracción de ciclo entre 0.0 y 1.0.
         * @return OpenGL ID.
         */
		virtual MOint		GetGLId( MOfloat p_cycle );

        /**
         * Devuelve el número de cuadros de la animación.
         * @return número de cuadros.
         */
		virtual MOuint		GetFrameCount() { return m_nFrames; }
        /**
         * Fija el número de cuadros de la animación
         * @param p_nframes Nuevo número de cuadros.
         */
		virtual void 		SetFrameCount( MOuint p_nframes ) { m_nFrames = p_nframes; } //

        /**
         * Devuelve los cuadros por segundo de la animación.
         * @return cuadros por segundo.
         */
		virtual MOfloat		GetFramesPerSecond() { return m_fFramesPerSecond; }

        /**
         * Fija el número de cuadros por segundo de la animación.
         * @param p_fps Nuevo número de cuadros por segundo de la animación.
         */
		virtual void 		SetFramesPerSecond( MOfloat p_fps) { m_fFramesPerSecond = p_fps; }


    enum moPlayMode {
      MO_PLAYMODE_TIMEBASE, ///time base relative
      MO_PLAYMODE_FRAMEBASE ///frame base relative
    };
        /**
         * Fija el modo de reproducción :
         * @see moPlayMode
         * MO_PLAYMODE_TIMEBASE : reproducción segun timer interno de GStreamer
         * MO_PLAYMODE_FRAMEBASE : reproducción arbitraria por manejo de frame
         * @param p_i índice de textura a validar.
         */
    virtual void SetPlayMode( moPlayMode playmode );

        /**
         * Devuelve el modo de reproducción :
         * @see moPlayMode
         * MO_PLAYMODE_TIMEBASE : reproducción segun timer interno de GStreamer
         * MO_PLAYMODE_FRAMEBASE : reproducción arbitraria por manejo de frame
         * @return moPlayMode modo de reproducción de este video
         */
    virtual moPlayMode GetPlayMode();

        /**
         * Calcula el cuadro i-ésimo de la animación.
         * @param p_i índice del cuadro a cualcular.
         */
		virtual void		GetFrame( MOuint p_i );

		virtual MOuint		GetActualFrame();

        /**
         * Configura el modo de interpolación de la textura. Este modo genera texturas intermedias que
         * entre dos cuadros de la animación.
         * @param p_FrameJump cuadro que define el "salto" de la interpolación: las texturas interpoladas son generadas
         * entre el cuadro p_FrameJump y p_FrameJump + 1.
         * @param p_InterpolationTime el tiempo que dura la interpolación.
         */
		virtual void		SetInterpolation( MOuint p_FrameJump, MOuint p_InterpolationTime );
        /**
         * Devuelve si el cuadro de la animación está en el rango de interpolación.
         * @return true o falsed dependiendo del cuadro actual.
         */
		virtual MOboolean	IsInterpolating();
        /**
         * Devuelve si el modo de interpolación está activo.
         * @return true o false dependiendo del estado del modo de interpolación.
         */
		virtual MOboolean	IsInterpolationActive();
        /**
         * Fija el estado del modo de interpolación.
         * @param activate el nuevo estado.
         * @return el estado del modo de interpolación. Aunque se intente activar la interplación, puede ocurrir
         * que los shaders requeridos para la operación de interpolación no estén disponibles, en cuyo caso se
         * devuelve false.
         */
		virtual MOboolean   ActivateInterpolation( MOboolean activate = true );

	protected:

		virtual MOboolean	NeedsInterpolation();
		virtual MOint		Interpolate();

    moPlayMode    m_PlayMode;
    MOboolean     m_bIsPlaying;

		MOuint				m_nFrames;
		MOfloat				m_fFramesPerSecond;

		MOuint				m_FrameNext;
		MOuint				m_FramePrevious;
		MOuint				m_Time;
		MOuint              m_ActualFrame;

		//interpolation flags
		MOboolean			m_bInterpolation;
		MOboolean			m_bInterpolating;

		//interpolation filters
		moTextureFilter*	m_pCopyStart;
		moTextureFilter*	m_pCopyEnd;
		moTextureFilter*	m_pInterpolator;

		//interpolation textures
		moTextureArray		m_srcstart,m_dststart;
		moTextureArray		m_srcend,m_dstend;
		moTextureArray		m_srcinterpol,m_dstinterpol;

		//interpolation shaders
		moShader*			m_pShaderCopy;
		moShader*			m_pShaderInterpolate;

		//interpolation parameters
		MOuint				m_FrameJump;/* salto entre el cuadro inicial y final mínimo, para resolver la interpolación*/
		MOuint				m_InterpolationTime;/*intervalo de resolución de la interpolación en milisegundos*/

		//interpolation variables
		MOuint				m_StartTime;
		MOuint				m_FrameStart;/*cuadro inicial de la interpolación*/
		MOuint				m_FrameEnd;/*cuadro final de la interpolación*/
		MOfloat				m_InterpolationPosition;/*valor entre 0 y 1 indicando el nivel de interpolación, 0 corresponde al cuadro inicial, 1 al cuadro final*/

};

/// Secuencia de texturas.
/**
 * Clase para manejar una secuencia de texturas.
 *
 */
class LIBMOLDEO_API moTextureMultiple : public moTextureAnimated
{
	public:

        /**
         * Constructor por defecto.
         */
		moTextureMultiple();
        /**
         * Destructor por defecto.
         */
		~moTextureMultiple();

        /**
         * Inicializa propiedades básicas de la textura tales como el nombre, id, administradores y parámetros.
         * @param p_name nombre de la textura.
         * @param p_moid Moldeo ID de la textura.
         * @param p_res puntero al administrador de recursos, del cual son extraidos todos los administradores que la textura necesita (GL, Data, File, etc).
         * @param p_param parámetros de textura.
         */
		virtual MOboolean  Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param = MODefTex2DParams);
        /**
         * Destruye la textura openGL.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		virtual MOboolean  Finish();

        /**
         * Revisa que el archivo especificado con p_filename sea un archivo de textura múltiple soportado.
         * @return true si el archivo está soportado, false en caso contrario.
         */
		MOboolean SupportedFile(moText p_filename);
        /**
         * Construye una textura múltiple a partir de la imágenes especificadas en los parámetros p_param.
         * @param p_param puntero a un objeto de parámetros que contiene el nombre del archivo del cual la textura será leida.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean Load(moParam* p_param);

        /**
         * Construye una textura múltiple a partir de la imágenes especificadas en los subvalores del valor p_value
         * @param p_value puntero a un objeto de subvalores que contiene el nombre de los archivos de los cuales las texturas serán leidas.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean Load(moValue* p_value);

        /**
         * Construye una textura múltiple a partir de las texturas entre los índices p_id0 y p_id1 de la lista
         * de texturas p_textures.
         * @param p_name Nombre que es asignado a la textura.
         * @param p_textures Referencia a la lista de texturas de donde son extraídas las texturas a copiar.
         * @param p_id0 índice inicial.
         * @param p_id1 índice final.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean Load(moText p_name, moTextureArray &p_textures, MOuint p_id0, MOuint p_id1);

        /**
         * Inicializa la textura múltiple con p_tex_count en blano.
         * @param p_tex_count número de texturas.
         */
		void SetTextureCount(MOuint p_tex_count);
        /**
         * Devuelve el número de texturas en la secuencia que define esta textura múltiple.
         * @return número de texturas.
         */
		MOuint GetTextureCount() { return m_textures_array.Count(); }
        /**
         * Fija como textura actual aquella que tiene la posición i-ésima en la secuencia.
         * @param p_i índice de la textura a fijar como actual.
         */
		void GetFrame(MOuint p_i);
        /**
         * Copia la textura apuntada por p_texture en la posición i-ésima de la secuencia.
         * @param p_i índice de la textura donde se copiara la que es pasada como parámetro.
         * @param p_texture puntero al objeto de textura a copiar en la posición i-ésima.
         */
		void SetFrame(MOuint p_i, moTexture* p_texture);
        /**
         * Agrega una nueva textura en la secuencia.
         * @param p_texture puntero al objeto de textura que será agregado a la secuencia.
         */
		void AddFrame(moTexture* p_texture);
        /**
         * Devuelve un puntero al objeto de textura en la posición i-ésima en la secuencia.
         * @param p_i índice de la textura requerida.
         * @return puntero a la textura devuelta.
         */
		moTexture* GetTexture( MOuint p_i ) { return m_textures_array[p_i];	}

        /**
         * Valida el índice de textura pasado como parámetro, es decir, revisa que esté entre los rángos
         * permitidos (0 y número total de texturas en esta textura múltiple - 1).
         * @param p_i índice de textura a validar.
         * @return true si el índice es válido, false en caso contrario.
         */
		MOboolean ValidTexture(MOuint p_i);


	protected:
		moTextureArray m_textures_array;

};

///  una textura animada basada en una película
/**
 * Clase para manejar una textura animada basada en una película.
 *
 */
class LIBMOLDEO_API moMovie : public moTextureAnimated
{
	public:
        /**
         * Constructor por defecto.
         */
		moMovie();
        /**
         * Destructor por defecto.
         */
		virtual ~moMovie();

        /**
         * Inicializa propiedades básicas de la textura tales como el nombre, id, administradores y parámetros.
         * @param p_name nombre de la textura.
         * @param p_moid Moldeo ID de la textura.
         * @param p_res puntero al administrador de recursos, del cual son extraidos todos los administradores que la textura necesita (GL, Data, File, etc).
         * @param p_param parámetros de textura.
         */
		virtual MOboolean  Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param = MODefTex2DParams);
        /**
         * Destruye la textura openGL.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		virtual MOboolean  Finish();

        /**
         * Funciones de control de la pelicula
         */
    virtual void Play();
    virtual void Pause();
    virtual void Continue();
    virtual void Stop();
    virtual void Seek( long frame, float rate = 1.0 );
    virtual bool IsPlaying();
    virtual MOulong GetPosition();
    virtual moStreamState State();

    virtual void SetBrightness( float brightness );
    virtual void SetContrast( float contrast );
    virtual void SetSaturation( float saturation );
    virtual void SetHue( float hue );

    virtual void SetVolume( float volume );
    virtual void SetBalance( float balance );

        /**
         * Revisa que el archivo especificado con p_filename sea un archivo de película soportado.
         * Tiene que ser de extensión avi, mpg o mov.
         * @return true si el archivo está soportado, false en caso contrario.
         */
		MOboolean SupportedFile(moText p_filename);
        /**
         * Carga en la textura la película p_filename.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean LoadMovieFile(moText p_filename);
        /**
         * Construye una película a partir de los parámetros p_param.
         * @param p_param puntero a un objeto de parámetros que contiene el nombre del archivo del cual la película será leida.
         * @return true si la operación fue exitosa, false en caso contrario.
         */
		MOboolean Load( moParam* p_param );
		MOboolean Load( moValue* p_value );

		void EnableVideo(int);
		bool HasVideo();

		void EnableAudio(int);
		bool HasAudio();

    bool  IsEOS();

	protected:

		void GetFrame( MOuint p_i );

		MOint frameprevious;
		MOint lastframe;
		moBucketsPool	m_BucketsPool;

    moVideoGraph*	m_pGraph;

};

#include "moFBO.h"
#include "moGLManager.h"
#include "moTextureFilter.h"

#endif

