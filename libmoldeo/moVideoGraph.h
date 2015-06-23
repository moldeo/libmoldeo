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

/// Resoluciones de audio
/**
*   enumeración de resoluciones de video
*/
enum moAudioMode {
  MONO_44100_8 = 10844100,
  MONO_44100_16 = 11644100,
  MONO_44100_24 = 12444100,
  MONO_44100_32 = 13244100,
  MONO_48000_8 = 10848000,
  MONO_48000_16 = 11648000,
  MONO_48000_24 = 12448000,
  MONO_48000_32 = 13248000,
  ESTEREO_44100_8 = 20844100,
  ESTEREO_44100_16 = 21644100,
  ESTEREO_44100_24 = 22444100,
  ESTEREO_44100_32 = 23244100,
  ESTEREO_48000_8 = 2848000,
  ESTEREO_48000_16 = 21648000,
  ESTEREO_48000_24 = 22448000,
  ESTEREO_48000_32 = 23248000,
	MOAUDIOMODE_UNKNOWN
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

/// Formato de audio
/**
*   estructura que define la definición y la frecuencia de muestreo del sonido
*   tamaño de buffer, etc... de 1 paquete de audio
*/
class LIBMOLDEO_API moAudioFormat {

	public:
		moAudioFormat() {
			m_AudioMode = MOAUDIOMODE_UNKNOWN;
			m_Width = 0;
      m_Depth = 0;
			m_BitRate = 0;
			m_TimePerSample = 0;
			m_BitCount = 0;
			m_Channels = 0;
			m_BufferSize = 0;
			m_SampleRate = 0;
			m_WaitForFormat = true;
		}

		void SetAudioMode() {
			switch(m_BufferSize) {
				case MONO_44100_8:
				case MONO_44100_16:
				case MONO_44100_24:
				case MONO_44100_32:
				case MONO_48000_8:
				case MONO_48000_16:
				case MONO_48000_24:
				case MONO_48000_32:
				case ESTEREO_44100_8:
				case ESTEREO_44100_16:
				case ESTEREO_44100_24:
				case ESTEREO_44100_32:
				case ESTEREO_48000_8:
				case ESTEREO_48000_16:
				case ESTEREO_48000_24:
				case ESTEREO_48000_32:
					m_AudioMode = (moAudioMode)m_BufferSize;
					break;

				default:
					m_AudioMode = MOAUDIOMODE_UNKNOWN;
					break;

			}
		}

		moAudioMode	m_AudioMode;/** modo de audio, ver: moVideoMode */
		//moColorMode m_ColorMode;
		MOuint m_Width;/** ancho de la muestra */
		MOuint m_Depth;/** profundidad de la muestra */
		MOuint m_Channels;/** cantidad canales*/

    MOuint m_BitRate; /** tasa en bits por segundos */
    MOuint m_SampleSize; /** 8,16,24,32*/
		MOuint m_SampleRate; /** cantidad de muestras por segundo : 41000 Hz, 48000 Hz*/
		MOuint m_TimePerSample; /** duración de la muestra en unidades de 100 nanosegundos (DirectShow) o 1ns (GStreamer), 1 ns = 1e-9 100ns = 1e-7 */

		MOuint m_BitCount;/** bits por cada pixel, 32, 24, 16, 8 */

		MOuint m_BufferSize;/** tamaño en bytes del buffer de la muestra*/

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
			m_LabelName = moText("");
			m_DevicePort = 0;
			m_bPresent = false;
			m_bPreferred = false;
      m_SourceWidth = m_SourceHeight = m_SourceBpp = m_SourceFlipH = m_SourceFlipV = 0;
		}

        /// contructor
		moCaptureDevice( const moText &p_name, const moText &p_description, const moText &p_path, MOint p_deviceport = 0, MOint p_sourcewidth = 0, MOint p_sourceheight = 0, MOint p_bpp = 0, MOint p_fliph=0, MOint p_flipv=0 ) {
			m_Name = p_name;
			m_LabelName = p_name;
			m_Description = p_description;
			m_Path  = p_path;
			m_DevicePort = p_deviceport;
			m_bPresent = true;
			m_bPreferred = false;
			m_SourceWidth = p_sourcewidth;
			m_SourceHeight = p_sourceheight;
			m_SourceBpp = p_bpp;
      m_SourceFlipH = p_fliph;
      m_SourceFlipV = p_flipv;
		}

		///Copy constructor
		moCaptureDevice( const moCaptureDevice &src ) {
			(*this) = src;
		}

    /// Operador de copia
		moCaptureDevice &operator = (const moCaptureDevice &src)
		{
			m_bPresent = src.m_bPresent;
			m_bPreferred = src.m_bPreferred;
			m_Name = src.m_Name;
			m_Description = src.m_Description;
			m_Path = src.m_Path ;
			m_VideoFormat = src.m_VideoFormat;
			m_LabelName = src.m_LabelName;
			m_DevicePort = src.m_DevicePort;
			m_SourceWidth = src.m_SourceWidth;
			m_SourceHeight = src.m_SourceHeight;
			m_SourceBpp = src.m_SourceBpp;
      m_SourceFlipH = src.m_SourceFlipH;
      m_SourceFlipV = src.m_SourceFlipV;

			return *this;
		}

    virtual ~moCaptureDevice() {
    }



		void SetName( const moText &p_name ) {
			m_Name = p_name;
		}

    /// Devuelve el nombre del dispositivo
		const moText& GetName() const {
			return m_Name;
		}

        /// Devuelve la descripción del dispositivo
		const moText& GetDescription() const {
			return m_Description;
		}

        /// Devuelve el camino al dispositivo
		const moText& GetPath() const {
			return m_Path;
		}

        /// Devuelve el puerto del dispositivo
		MOint GetPort() const {
			return m_DevicePort;
		}

        /// Devuelve el formato de video del dispositivo
		moVideoFormat& GetVideoFormat() {

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
		bool IsPresent() const {
			return m_bPresent;
		}

        /// Fija el nombre de código del dispositivo
		void SetLabelName( const moText &p_labelname ) {
			m_LabelName = p_labelname;
		}

        /// Devuelve el nombre de código del dispositivo
		const moText& GetLabelName() const {
			return m_LabelName;
		}

    /// Devuelve el ancho de la imagen de origen
    int GetSourceWidth() const {
        return m_SourceWidth;
    }

    /// Devuelve el alto de la imagen de origen
    int GetSourceHeight() const {
        return m_SourceHeight;
    }

    /// Devuelve los bits por pixel de la imagen de origen
    int GetSourceBpp() const {
        return m_SourceBpp;
    }

    /// Devuelve el valor de inversión de imagen horizontal
    int GetSourceFlipH() const {
        return m_SourceFlipH;
    }

    /// Devuelve el valor de inversión de imagen vertical
    int GetSourceFlipV() const {
        return m_SourceFlipV;
    }

    void SetPreffered( bool setpreferred = true) {
      m_bPreferred = setpreferred;
    }

    bool IsPreferred() {
      return m_bPreferred;
    }


	private:
		bool			m_bPresent;///Presencia del dispositivo
		bool      m_bPreferred;///Dispositivo preferido siempre se dan de alta cuando están presentes.
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

		moText			m_LabelName;///Etiqueta asignada por el usuario al dispositivo: si no se define es el nombre del dispositivo

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
        virtual void SetPreferredFormat( const moCaptureDevice &p_CaptureDevice );

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
	virtual void Seek( MOuint frame, float rate = 1.0 ) = 0;

    /// Está corriendo
    /**
    *   indicador que señala si el grafo se está reproduciendo
    *   @return verdadero o falso
    */
	virtual bool IsRunning() = 0;

    /// La cantidad de frames, el largo del stream
    /**
    *   devuelve el largo del stream convertido a frames
    *   @return largo en frames
    */
	virtual MOulong	GetFramesLength() = 0;

    /// La posición del stream en cuadros
    /**
    *   devuelve la posicion o cuadro que se encuentra reproduciendo
    *   @return posicion en cuadros
    */
  virtual MOulong GetPosition() = 0;

    /// La duración total del stream en nanosegundos
    /**
    *   devuelve la duración total del stream que se encuentra reproduciendo en nanosegundos
    *   @return duración total del stream en nanosegundos
    */
  virtual MOulong GetDuration() = 0;

    /// Puntero al frame buffer
    /**
    *   devuelve el puntero al frame buffer
    *   @return un puntero
    */
	virtual MObyte* GetFrameBuffer(MOlong *size) = 0;

    /// Devuelve el modo de video
    /**
    *   indicador que señala el modo de video del video que se reproduce
    *   @return moVideoMode el modo de video
    */
    moVideoMode		GetVideoMode();

    /// Fija el nivel de volumen
    /**
    *   permite fijar el volumen con un valor entre 0 y 10,  1.0 = 100%
    *   @param volume nivel de volumen entre 0 y 10, 1.0 = 100%
    */
    virtual void SetVolume( float volume ) = 0;

    /// Fija el balance entre canal izquierdo y derecho en caso de haberlos
    /**
    *   Permite modificar el balance estereo entre -1 y 1, siendo
    *   -1 todo a la izquierda y 1 todo a la derecha, 0 corresponde
    *   al equilibrio entre los dos canales.
    *   @param balance nivel balance entre los dos canales, entre -1 y 1
    */
    virtual void SetBalance( float balance ) = 0;

    virtual void SetBrightness( float brightness ) = 0;
    virtual void SetContrast( float contrast ) = 0;
    virtual void SetSaturation( float saturation ) = 0;
    virtual void SetHue( float hue ) = 0;
    virtual bool IsEOS() = 0;
    virtual void SetEOS(bool iseos) = 0;


    /// Devuelve el formato de video
    /**
    *   indicador que señala el formato de video utilizado por esta entrada de video
    *   @return moVideoFormat el formato de video
    */
    moVideoFormat	GetVideoFormat();

    /// Devuelve el formato de audio
    /**
    *   indicador que señala el formato de audio utilizado por esta entrada de video
    *   @return moAudioFormat el formato de video
    */
    moAudioFormat	GetAudioFormat();

    /// Estado de la reproducción
    /**
    *   Devuelve el estado en que se encuentra el grafo, un valor entre
    *     MO_STREAMSTATE_UNKNOWN=-1,
    *     MO_STREAMSTATE_PAUSED,
    *     MO_STREAMSTATE_PLAYING,
    *     MO_STREAMSTATE_WAITING,
    *     MO_STREAMSTATE_READY,
    *     MO_STREAMSTATE_STOPPED
    *   @return moStreamState estado del stream del grafo
    */
    virtual moStreamState    GetState();

    /// Estado de la reproducción (version texto)
    /**
    *   Esta función indica lo mismo que la moVideoGraph::GetState()
    *   pero convertida a texto.
    *   @return moText estado del stream del grafo en formato texto
    */
    static moText          StateToText( moStreamState state );

	protected:

	moVideoFormat		m_VideoFormat;///Formato de video
  moAudioFormat		m_AudioFormat;

};









#endif

