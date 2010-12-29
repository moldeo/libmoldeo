/*******************************************************************************

                                moVideoGraph.h

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

#ifndef __MO_VIDEOGRAPH_H__
#define __MO_VIDEOGRAPH_H__

#include "moTypes.h"
#include "moText.h"
#include "moBuckets.h"
#include "moArray.h"
#include "moAbstract.h"

/*
	RESOLUCIONES

  http://en.wikipedia.org/wiki/Display_resolution


Digital TV standard 	Resolution 	Display aspect ratio 	Pixels
Video CD 	352 �240 (NTSC) 	4:3 (non-square pixels) 	84,480
			352 �288 (PAL) 	101,376
China Video Disc 	352 �480 (NTSC) 	4:3 (non-square pixels) 	168,960
					352 �576 (PAL) 	202,725
SVCD 	480 �480 (NTSC) 	4:3 (non-square pixels) 	230,400
		480 �576 (PAL) 	276,480
EDTV 480p 	640 �480 	4:3 or 16:9 	307,200
			704 �480 	337,920
			852 �480 	408,960
DVD 	704 �480 (NTSC) 	4:3 or 16:9 (non-square pixels) 	337,920
		704 �576 (PAL) 	405,504
D1 	720 �480 (NTSC) 	4:3 or 16:9 (non-square pixels) 	345,600
	720 �576 (PAL) 	414,720
D1 (NTSC with square pixels) 	720 �540 	4:3 	388,800
HDTV 720p 	1280 �720 	16:9 	921,600
HDTV 1080p, 1080i 	1920 �1080 	16:9 	2,073,600
*/
/// Resoluciones de video
/**
*   enumeración de resoluciones de video
*/
enum moVideoMode {
	PAL_D1 = 414720, //720 x 576
	NTSC_D1 = 345600, //720 x 480
	NTSC_SQUARE = 388800, //720x540

	NTSC_VDISC = 168960, //352x480
	PAL_VDISC = 202725, //352x576

	NTSC_SVCD = 230400, //480x480
	PAL_SVCD = 276480, //480x576

	NTSC_VCD = 84480, //352x240
	PAL_VCD = 101376, //352x288

	PAL_DVD = 337920, //704 x 576
	NTSC_DVD = 405504,//704 x 480

	HDTV_720p = 921600, //1280x720
	HDTV_1080p = 2073600, //1920x1080

	VGA = 307200, //640 x 480
	HVGA = 76800, //320 x 240
	CGA = 64000, //320x200
	MOVIDEOMODE_UNKNOWN
};


/// Formatos de codificacion de color
/**
*   enumeración de formatos de color
*/
enum moColorMode {
    YUV = 0,
    RGB = 1
};

/**
typedef enum {
  GST_STATE_VOID_PENDING        = 0,
  GST_STATE_NULL                = 1,
  GST_STATE_READY               = 2,
  GST_STATE_PAUSED              = 3,
  GST_STATE_PLAYING             = 4
} GstState;
*/
enum moStreamState {

  MO_STREAMSTATE_UNKNOWN=-1,
  MO_STREAMSTATE_PAUSED,
  MO_STREAMSTATE_PLAYING,
  MO_STREAMSTATE_WAITING,
  MO_STREAMSTATE_READY,
  MO_STREAMSTATE_STOPPED
};

/// Formato de video
/**
*   estructura que define el ancho, alto, velocidad de reproducción
*   tamaño de buffer, etc... de 1 cuadro de video
*/
class LIBMOLDEO_API moVideoFormat {

	public:
		moVideoFormat() {
			m_VideoMode = MOVIDEOMODE_UNKNOWN;
			m_Height = 0;
			m_Width = 0;
			m_BitRate = 0;
			m_TimePerFrame = 0;
			m_BitCount = 0;
			m_BufferSize = 0;
			m_FrameRate = 0;
			m_WaitForFormat = true;
		}

		void SetVideoMode() {
			switch(m_BufferSize) {
				case PAL_D1:
				case NTSC_D1:
				case NTSC_SQUARE:
				case NTSC_VDISC:
				case PAL_VDISC:
				case NTSC_SVCD:
				case PAL_SVCD:
				case NTSC_VCD:
				case PAL_VCD:
				case PAL_DVD:
				case NTSC_DVD:
				case HDTV_720p:
				case HDTV_1080p:
				case VGA:
				case HVGA:
				case CGA:
					m_VideoMode = (moVideoMode)m_BufferSize;
					break;

				default:
					m_VideoMode = MOVIDEOMODE_UNKNOWN;
					break;

			}
		}

		moVideoMode	m_VideoMode;/** modo de video, ver: moVideoMode */
		moColorMode m_ColorMode;
		MOuint m_Height;/** alto en pixels */
		MOuint m_Width;/** ancho en pixels */
		MOuint m_BitRate; /** tasa en bits por segundos*/
		MOuint m_TimePerFrame; /** duración del frame en unidades de 100 nanosegundos (DirectShow) o 1ns (GStreamer), 1 ns = 1e-9 100ns = 1e-7 */
		MOuint m_BitCount;/** bits por cada pixel, 32, 24, 16, 15, 8, 1*/
		MOuint m_BufferSize;/** tamaño en bytes del buffer de imagen*/
        MOuint m_FrameRate;/** cantidad de frames por cada 100 segundos, PAL: 2500 NTSC: 2997*/
        MOboolean m_WaitForFormat;/** bandera que marca la indefinición del formato, default: true*/

};

/// Muestra de video o cuadro
/**
*   Es literalmente el cuadro de video con un formato de video y sus datos.
*/
class LIBMOLDEO_API moVideoSample {

	public:

        /// Constructor
		moVideoSample() {
			m_pSampleBuffer = NULL;
		}

        /// Constructor de copia
		moVideoSample& operator = ( moVideoSample& p_vs) {
			m_VideoFormat = p_vs.m_VideoFormat;
			m_pSampleBuffer = p_vs.m_pSampleBuffer;
			return (*this);
		}

        /// Constructor con formato de video y el puntero al buffer de datos
		moVideoSample( moVideoFormat p_videoformat, MOpointer p_buffer ) {

			m_VideoFormat = p_videoformat;
			m_pSampleBuffer = p_buffer;

		}

        /// Fija el formato de video y puntero al buffer de datos
        /**
        *   Atención: esta función no copia el buffer sino solamente
        *   la referencia como puntero a un buffer ya existente.
        *   @param p_videoformat    el formato de video
        *   @param p_buffer    el puntero a los datos
        */
		void SetVideoSample( moVideoFormat p_videoformat, MOpointer p_buffer ) {
			m_VideoFormat = p_videoformat;
			m_pSampleBuffer = p_buffer;
		}

		moVideoFormat	m_VideoFormat;///Formato de video de la muestra
		void*			m_pSampleBuffer;///Puntero a los datos del cuadro

};

/// Definición de un dispositivo de video, generalmente uno de captura de video, o camara
/**
*   Un dispositivo de captura de video tiene 3 parametros, un nombre, una descricpión y una
*   cadena  o código de caracteres que lo representa.
*/
class LIBMOLDEO_API moCaptureDevice {
	public:

		/// contructor
		moCaptureDevice() {
			m_Name = moText("");
			m_Description = moText("");
			m_Path  = moText("");
			m_DevicePort = 0;
			m_bPresent = false;
		    m_SourceWidth = m_SourceHeight = m_SourceBpp = m_SourceFlipH = m_SourceFlipV = 0;
		}

        /// contructor
		moCaptureDevice( const moText &p_name, const moText &p_description, const moText &p_path, MOint p_deviceport = 0, MOint p_sourcewidth = 0, MOint p_sourceheight = 0, MOint p_bpp = 0, MOint p_fliph=0, MOint p_flipv=0 ) {
			m_Name = p_name;
			m_Description = p_description;
			m_Path  = p_path;
			m_DevicePort = p_deviceport;
			m_bPresent = true;
			m_SourceWidth = p_sourcewidth;
			m_SourceHeight = p_sourceheight;
			m_SourceBpp = p_bpp;
            m_SourceFlipH = p_fliph;
            m_SourceFlipV = p_flipv;
		}

		///Copy constructor
		moCaptureDevice( const moCaptureDevice &src ) {
			*this = src;
		}

        /// Devuelve el nombre del dispositivo
		moText GetName() {
			return m_Name;
		}

        /// Devuelve la descripción del dispositivo
		moText GetDescription() {
			return m_Description;
		}

        /// Devuelve el camino al dispositivo
		moText GetPath() {
			return m_Path;
		}

        /// Devuelve el puerto del dispositivo
		MOint GetPort() {
			return m_DevicePort;
		}

        /// Devuelve el formato de video del dispositivo
		moVideoFormat GetVideoFormat() {

			return m_VideoFormat;
		}

        /// Devuelve el formato de video del dispositivo
		void SetVideoFormat( const moVideoFormat &p_videoformat ) {

			m_VideoFormat = p_videoformat;

		}

        /// Fija la presencia del dispositivo
		void Present( bool p = true ) {
			m_bPresent = p;
		}

        /// Señala y verifica si está presente el dispositivo
		bool IsPresent() {
			return m_bPresent;
		}

        /// Fija el nombre de código del dispositivo
		void SetCodeName( const moText &p_codename ) {
			m_CodeName = p_codename;
		}

        /// Devuelve el nombre de código del dispositivo
		moText GetCodeName() {
			return m_CodeName;
		}

        /// Devuelve el ancho de la imagen de origen
        int GetSourceWidth() {
            return m_SourceWidth;
        }

        /// Devuelve el alto de la imagen de origen
        int GetSourceHeight() {
            return m_SourceHeight;
        }

        /// Devuelve los bits por pixel de la imagen de origen
        int GetSourceBpp() {
            return m_SourceBpp;
        }

        /// Devuelve el valor de inversión de imagen horizontal
        int GetSourceFlipH() {
            return m_SourceFlipH;
        }

        /// Devuelve el valor de inversión de imagen vertical
        int GetSourceFlipV() {
            return m_SourceFlipV;
        }


        /// Operador de copia
		moCaptureDevice &operator = (const moCaptureDevice &src)
		{
			m_bPresent = src.m_bPresent;
			m_Name = src.m_Name;
			m_Description = src.m_Description;
			m_Path = src.m_Path ;
			m_VideoFormat = src.m_VideoFormat;
			m_CodeName = src.m_CodeName;
			m_DevicePort = src.m_DevicePort;
			m_SourceWidth = src.m_SourceWidth;
			m_SourceHeight = src.m_SourceHeight;
			m_SourceBpp = src.m_SourceBpp;
            m_SourceFlipH = src.m_SourceFlipH;
            m_SourceFlipV = src.m_SourceFlipV;

			return *this;
		}

	private:
		bool			m_bPresent;///Presencia del dispositivo
		moText			m_Name;///Nombre del dispositivo
		moText			m_Description;///Descripción del dispositivo
		moText			m_Path;///Camino o clave del dispositivo
		moVideoFormat	m_VideoFormat;///Formato video del dispositivo
		MOint           m_DevicePort;///PUERTO DEL DISPOSITIVO

		MOint           m_SourceWidth;
		MOint           m_SourceHeight;
		MOint           m_SourceBpp;
		MOint           m_SourceFlipH;
		MOint           m_SourceFlipV;

		moText			m_CodeName;///Código del dispositivo

};


moDeclareExportedDynamicArray(moCaptureDevice, moCaptureDevices);

/// Plataforma de reproducción de video, actualmente GStreamer para todas las plataformas
/**
*   clase que define la interfaz virtual a la plataforma de manejo de video del sistema
*   de esta clase derivan moDsFramework(obsoleta) y moGsFramework que son implementaciones
*   de DirectShow y GStreamer respectivamente.
*   @see moDsFramework (obsoleta)
*   @see moGsFramework
*/
class LIBMOLDEO_API moVideoFramework : public moAbstract {

	public:

		moVideoFramework();
		virtual ~moVideoFramework();

		/// Fija los dispositivos predeterminados
		/**
		*   Guarda en m_PreferredDevices los nombres y formatos del dispositivo
		*/
		virtual void SetPreferredDevices( moCaptureDevices* p_pPreferredDevices );

		/// Fija el formato de un dispositivo
		/**
		*   fija el formato con el formato de la referencia a un moCaptureDevice
		*/
        virtual void SetPreferredFormat( moCaptureDevice &p_CaptureDevice );

		/// Acceso a los dispositivos de video preferidos
		/**
		*   devuelve una referencia a los dispositivos de video preferidos
		*/
		virtual moCaptureDevices*	GetPreferredDevices() {
			return &m_PreferredDevices;
		}

		/// Acceso a los dispositivos de video disponibles
		/**
		*   devuelve una referencia a los dispositivos de video disponibles
		*/
		virtual moCaptureDevices*	GetCaptureDevices() {
			return &m_CaptureDevices;
		}

		/// Carga los dispositivos de video disponibles
		/**
		*   devuelve una referencia a los dispositivos de video disponibles
		*   esta función debe ser implementada en cada plataforma
		*/
		virtual moCaptureDevices* LoadCaptureDevices() = 0;

		/// Actualiza los dispositivos de video disponibles
		/**
		*   devuelve una referencia a los dispositivos de video disponibles
		*   esta función debe ser implementada en cada plataforma
		*/
		virtual moCaptureDevices* UpdateCaptureDevices() = 0;

		/// Chequea si el dispositivos de video disponible está aún disponible
		/**
		*   En el indice i del arreglo de los dispositivos de video disponibles
		*   confirma la disponibilidad de este.
		*   Al implementar esta función se puede conectar y reconectar cámaras.
		*   @return verdadero si está disponible, falso si no
		*/
		virtual bool	CheckCaptureDevice( int i ) = 0;

		/// Agrega un dispositivo de video
		/**
        *   Si el dispositivo existe entonces esta función devuelve verdadero.
		*   @return verdadero si existe y pudo ser agregado, falso si no
		*/
		virtual bool    AddCaptureDevice(  moCaptureDevice& p_capdev ) = 0;


        /// Limpia el arreglo de dispositivos de video
		/**
		*   Elimina del arreglo todos los dispositivos de video
		*/
		virtual void CleanCaptureDevices() {
			m_CaptureDevices.Empty();
		}

	protected:
		moCaptureDevices	m_CaptureDevices;///Dispositivos de video disponibles
		moCaptureDevices	m_PreferredDevices;///Dispositivos de video preferidos

};

/// Grafo de reproducción de video
/**
*   clase que define la interfaz virtual con un recurso de video
*   de esta clase derivan moDsGraph(obsoleta) y moGsGraph que son implementaciones
*   de DirectShow y GStreamer respectivamente.
*   @see moDsGraph
*   @see moGsGraph
*/
class LIBMOLDEO_API moVideoGraph : public moAbstract {

	public:

	moVideoGraph();
	virtual ~moVideoGraph();

//================================================
//	INITIALIZATION AND FINALIZATION METHODS
//================================================
    /// Inicialización del grafo
    /**
    *   Inicializa los punteros y estructuras de la clase
    *   @return verdadero si fue exitoso, falso sino
    */
	virtual bool InitGraph() = 0;

    /// Finalización del grafo
    /**
    *   Destruye los punteros y estructuras de la clase creadas dinámicamente en la función InitGraph
    *   @return verdadero si fue exitoso, falso sino
    */
    virtual bool FinishGraph() = 0;
    /// Grafo de captura de video
    /**
    *   genera un grafo para capturar una cámara, especificada por moCaptureDevice
    *   @return verdadero si fue exitoso, falso sino
    */
	virtual bool BuildLiveGraph( moBucketsPool *pBucketsPool, moCaptureDevice p_capdev) = 0;

    /// Grafo de reproducción de video en modo vivo, asyncronicamente reproducido en función del clock
    /**
    *   genera un grafo para reproducir un video en modo normal
    *   @return verdadero si fue exitoso, falso sino
    */
	virtual bool BuildLiveVideoGraph( moText filename, moBucketsPool* pBucketsPool) = 0;

	virtual bool BuildLiveQTVideoGraph( moText filename, moBucketsPool* pBucketsPool) = 0;

    /// Grafo de grabación
    /**
    *   genera un grafo para grabar a un archivo una serie de imagenes
    *   @return verdadero si fue exitoso, falso sino
    */
	virtual bool BuildRecordGraph( moText filename, moBucketsPool* pBucketsPool) = 0;

//================================================
//	CONTROL METHODS
//================================================
    /// Reproducir el video
    /**
    *   comando de play, reproduce el video o continua la reproducción
    */
	virtual void Play() = 0;

    /// Detener la reproducción del video
    /**
    *   comando de stop, detiene la reproducción del video
    */
	virtual void Stop() = 0;

    /// Pausa la reproducción del video
    /**
    *   comando de pause, interrumpe la reproducción del video
    */
	virtual void Pause() = 0;

    /// Busca y posiciona
    /**
    *   comando de Seek, salta a la posición deseada y luego queda en pausa
    *   @param  frame   esto es un frame...
    */
	virtual void Seek( MOuint frame ) = 0;

    /// Está corriendo
    /**
    *   indicador que señala si el grafo se está reproduciendo
    *   @return verdadero o falso
    */
	virtual bool IsRunning() = 0;

    /// Está corriendo
    /**
    *   indicador que señala si el grafo se está reproduciendo
    *   @return verdadero o falso
    */
	virtual MOulong	GetFramesLength() = 0;

    /// Está corriendo
    /**
    *   indicador que señala si el grafo se está reproduciendo
    *   @return verdadero o falso
    */
	virtual MObyte* GetFrameBuffer(MOlong *size) = 0;

    /// Devuelve el modo de video
    /**
    *   indicador que señala el modo de video del video que se reproduce
    *   @return moVideoMode el modo de video
    */
    moVideoMode		GetVideoMode();

    /// Devuelve el formato de video
    /**
    *   indicador que señala el formato de video utilizado por esta entrada de video
    *   @return moVideoFormat el formato de video
    */
    moVideoFormat	GetVideoFormat();

    virtual moStreamState    GetState();

    static moText          StateToText( moStreamState state );

	protected:

	moVideoFormat		m_VideoFormat;///Formato de video


};









#endif
