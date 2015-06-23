/*******************************************************************************

                                moVideoManager.h

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

#ifndef __MOVIDEOMANAGER_H
#define __MOVIDEOMANAGER_H
#include <moGsGraph.h>
#include <moDsGraph.h>
#include <moFBO.h>
#include <moTexture.h>
#include <moVideoGraph.h>
#include <moDebugManager.h>
#include <moDataManager.h>
#include <moTimeManager.h>

//RESOLUTION
//NTSC - PAL
#define LIVE_SAMPLE_DV_0_720X480_3		0
#define LIVE_SAMPLE_DV_1_720X480_3		1
#define LIVE_SAMPLE_DV_2_720X480_3		2
#define LIVE_SAMPLE_DV_3_720X480_3		3
#define LIVE_SAMPLE_DV_0_720X576_3		4
#define LIVE_SAMPLE_DV_1_720X576_3		5
#define LIVE_SAMPLE_DV_2_720X576_3		6
#define LIVE_SAMPLE_DV_3_720X576_3		7
//WEBCAM - 320x240
#define LIVE_SAMPLE_WEBCAM_0_320X240_3	8
#define LIVE_SAMPLE_WEBCAM_1_320X240_3	9
#define LIVE_SAMPLE_WEBCAM_2_320X240_3	10
#define LIVE_SAMPLE_WEBCAM_3_320X240_3	11
//VIDEO PAL-NTSC
#define LIVE_SAMPLE_VIDEO_0_720X480_3	12
#define LIVE_SAMPLE_VIDEO_1_720X480_3	13
#define LIVE_SAMPLE_VIDEO_2_720X480_3	14
#define LIVE_SAMPLE_VIDEO_3_720X480_3	15
#define LIVE_SAMPLE_VIDEO_0_720X576_3	16
#define LIVE_SAMPLE_VIDEO_1_720X576_3	17
#define LIVE_SAMPLE_VIDEO_2_720X576_3	18
#define LIVE_SAMPLE_VIDEO_3_720X576_3	19

//TAKE A SNAPSHOT OF A SAMPLE
#define LIVE_SNAPSHOT_DV_0_720X480_3			20
#define LIVE_SNAPSHOT_DV_0_720X576_3			21
#define LIVE_SNAPSHOT_WEBCAM_0_320X240_3		22

//DEVICE PORTS
#define IEEE1394_0						0
#define IEEE1394_1						1
#define IEEE1394_2						2
#define IEEE1394_3						3
#define WEBCAM_0						4
#define WEBCAM_1						5
#define WEBCAM_2						6
#define WEBCAM_3						7
#define VIDEO_0							8
#define VIDEO_1							9
#define VIDEO_2							10
#define VIDEO_3							11

#define MO_LIVE_LABELNAME	0
#define MO_LIVE_DEVICENAME	1
#define MO_LIVE_COLORMODE  	2
#define MO_LIVE_WIDTH		3
#define MO_LIVE_HEIGHT		4
#define MO_LIVE_BITCOUNT	5
#define MO_LIVE_SCALEWIDTH	6
#define MO_LIVE_SCALEHEIGHT	7
#define MO_LIVE_FLIPH   	8
#define MO_LIVE_FLIPV   	9

#define MO_VIDEO_BUFFERNAME		1
#define MO_VIDEO_BUFFERFORMAT	2
#define MO_VIDEO_WIDTH			3
#define MO_VIDEO_HEIGHT			4
#define MO_VIDEO_SOURCEXOFFSET	5
#define MO_VIDEO_SOURCEYOFFSET	6
#define MO_VIDEO_SOURCEWIDTH	7
#define MO_VIDEO_SOURCEHEIGHT	8
#define MO_VIDEO_SOURCEINTERPOLATION	9
#define MO_VIDEO_SOURCEINTERPOLATIONJUMP	10
#define MO_VIDEO_SOURCEINTERPOLATIONTIME	11



#define MO_VIDEO_CIRCULARBUFFERINPUT	0
#define MO_VIDEO_CIRCULARBUFFERNAME		1
#define MO_VIDEO_CIRCULARBUFFERFORMAT	2
#define	MO_VIDEO_CIRCULARBUFFERFRAMES	3
#define MO_VIDEO_CIRCULARWIDTH			4
#define MO_VIDEO_CIRCULARHEIGHT			5
#define MO_VIDEO_CIRCULARSOURCEXOFFSET	6
#define MO_VIDEO_CIRCULARSOURCEYOFFSET	7
#define MO_VIDEO_CIRCULARSOURCEWIDTH	8
#define MO_VIDEO_CIRCULARSOURCEHEIGHT	9

enum moLiveSystemType {
	LST_VIDEOCAMERA=0,
	LST_UNKNOWN
};


/// Sistema de tratamiento de una señal en vivo de video
/**
* Sistema de tratamiento de una señal en vivo de video
*   Por cada cámara se crea una instancia de esta clase,
*   a la cual se define el dispositivo moCaptureDevice a asociar y capturar.
*
* @see moVideoSample
* @see moVideoFormat
* @see moVideoGraph
* @see moGsGraph
* @see moVideoFramework
* @see moBucketsPool
*/
class LIBMOLDEO_API moLiveSystem {

	public:

		moLiveSystem();
		moLiveSystem( moCaptureDevice p_capdev );

		//Copy constructor
		moLiveSystem( const moLiveSystem &p_ls)
		{
      (*this)=p_ls;
		}

		moLiveSystem& operator = ( const moLiveSystem& p_ls) {
			m_Type = p_ls.m_Type;
			m_CaptureDevice = p_ls.m_CaptureDevice;
			m_pBucketsPool = p_ls.m_pBucketsPool;
			m_pVideoGraph = p_ls.m_pVideoGraph;
			m_pVideoSample = p_ls.m_pVideoSample;
			m_pTexture = p_ls.m_pTexture;
			return (*this);
		}

		virtual ~moLiveSystem();

		bool Init();
		void Finish();

		void SetType( moLiveSystemType p_Type );
		void SetCaptureDevice( moCaptureDevice p_capdev );

		moLiveSystemType	GetType();
		moText				GetLabelName();
		moText				GetDeviceName();
		moCaptureDevice		GetCaptureDevice();
		moBucketsPool*		GetBucketsPool();
		moVideoGraph*		GetVideoGraph();
		moVideoSample*		GetVideoSample();
		moTexture*      GetTexture();
		void            SetTexture( moTexture* p_Texture );

	private:

		moLiveSystemType	m_Type;
		moCaptureDevice		m_CaptureDevice;

		moBucketsPool*		m_pBucketsPool;
		moVideoGraph*		m_pVideoGraph;

		moVideoSample*		m_pVideoSample;
		moTexture*        m_pTexture;

};

typedef moLiveSystem* moLiveSystemPtr;

moDeclareExportedDynamicArray(moLiveSystemPtr,moLiveSystemPtrs);


/// Sistema de tratamiento de señales en vivo de video
/**
* Sistema de tratamiento de señales en vivo de video
*   Administra varias instancias de señales en vivo
*/
class LIBMOLDEO_API moLiveSystems : public moLiveSystemPtrs {

	public:

		moLiveSystems();
		virtual ~moLiveSystems();
		bool LoadLiveSystems( moCaptureDevices* p_pPreferredDevices );
		bool UpdateLiveSystems();
		bool UnloadLiveSystems();
		bool GetStatus( MOdevcode p_devcode );
		moVideoFramework* GetVideoFramework();

	protected:

        moVideoFramework*    m_pVideoFramework;

};

/// Clase de manipulación para el cuadro de un video en memoria
/**
* Clase de manipulación para el cuadro de un video en memoria
*
*/
class LIBMOLDEO_API moVideoFrame : public moAbstract {

	public:

		moVideoFrame();
		virtual ~moVideoFrame();

		virtual MOboolean Init();
		virtual MOboolean Init( moText bufferformat, moBitmap* pImageResult );
		virtual MOboolean Finish();


		MOint		options;

        moMemory	*hmem;
		moBitmapFormat fif;
		//FIBITMAP *m_pImage;


		MOlong m_FrameSize;

		MOuint	m_width;
		MOuint	m_height;

};

typedef moVideoFrame* moVideoFramePtr;

moDeclareExportedDynamicArray(moVideoFramePtr,moVideoFrames);

/// Buffer de imágenes para video
/**
    Clase que permite cargar una sucesión de imágenes de mismo formato con compresión incluída dentro
    de un buffer de memoria y a la vez permite reproducir cada cuadro de forma independiente, descomprimiendo de memoria
    y asignando una textura
*/

class LIBMOLDEO_API moVideoBuffer : public moTextureAnimated {

	public:
		moVideoBuffer();
		virtual ~moVideoBuffer();

		virtual MOboolean  Init();
		virtual MOboolean  Init( moText bufferpath, moText bufferformat, moResourceManager* p_pResourceManager,  MOuint width, MOuint height, MOuint xsource, MOuint ysource, MOuint sourcewidth, MOuint sourceheight, MOint interpolation=0, MOint interpolationjump=0, MOint interpolationtime=0 );
		virtual MOboolean  Finish();

		virtual void GetFrame( MOuint p_i );

		MOboolean LoadImage( moBitmap* pImage , int indeximage=-1 );

		MOint GetXSource() { return m_XSource; }
		MOint GetYSource() { return m_YSource; }
		MOint GetSourceWidth() { return m_SourceWidth; }
		MOint GetSourceHeight() { return m_SourceHeight; }
		moText GetBufferPath() { return m_BufferPath; }
		moText	GetBufferFormat() { return m_BufferFormat; }

	private:

		moText	m_BufferPath;
		moText  m_BufferFormat;

		moResourceManager*	m_pResourceManager;

		moVideoFrames	m_Frames;

		MOuint m_XSource;
		MOuint m_YSource;
		MOuint m_SourceWidth;
		MOuint m_SourceHeight;

};

typedef moVideoBuffer* moVideoBufferPtr;

moDeclareExportedDynamicArray(moVideoBufferPtr,moVideoBuffers);


/// Buffer Circular de imágenes para video
/**
    Este buffer permite cargar de manera gradual una sucesión de imágenes
    manteniendo siempre una cantidad fija histórica de cuadros para ser reproducidos desde la memoria.
    El circular buffer comprime en el formato seleccionado cada cuadro.

    Un uso común es el de aplicar un circularbuffer a una cámara para grabar en memoria una cantidad de cuadros
    para ser referenciados de manera desfazada, para crear el efecto de blur por ejemplo de movimiento.
*/

class LIBMOLDEO_API moCircularVideoBuffer : public moTextureAnimated {

	public:
		moCircularVideoBuffer();
		virtual ~moCircularVideoBuffer();

		virtual MOboolean  Init();
		virtual MOboolean  Init( moText videoinput, moText bufferformat, moResourceManager* p_pResourceManager, MOint frames,  MOint width, MOint height, MOint xsource, MOint ysource, MOint sourcewidth, MOint sourceheight );
		virtual MOboolean  Finish();

		virtual void  StartRecording( long at_position = -1 );
		virtual void  PauseRecording();
		virtual void  ContinueRecording();
		virtual void  JumpRecording( long at_position );
		virtual void  StopRecording();
		virtual long  GetRecordPosition();
		virtual bool  IsRecording();

		virtual void GetFrame( MOuint p_i );

		virtual MOboolean LoadSample( moVideoSample* pvideosample );
		//MOboolean LoadImage( FIBITMAP* pImage , MOuint indeximage );

		virtual MOint GetXSource() { return m_XSource; }
		virtual MOint GetYSource() { return m_YSource; }
		virtual MOint GetSourceWidth() { return m_SourceWidth; }
		virtual MOint GetSourceHeight() { return m_SourceHeight; }
		virtual moText GetVideoInput() { return m_VideoInput; }
		virtual moText	GetBufferFormat() { return m_BufferFormat; }

	private:

    moTimer m_RecTimer;

		MOint	m_WriteIndex;
		MOint	m_ReadIndex;

		moLock  m_Lock;

		moText	m_VideoInput;
		moText  m_BufferFormat;

		moResourceManager*	m_pResourceManager;

		moVideoFrames	m_Frames;

		MOuint m_XSource;
		MOuint m_YSource;
		MOuint m_SourceWidth;
		MOuint m_SourceHeight;
};

typedef moCircularVideoBuffer* moCircularVideoBufferPtr;

moDeclareExportedDynamicArray(moCircularVideoBufferPtr,moCircularVideoBuffers);

class moDirectory;

class LIBMOLDEO_API moVideoBufferPath : public moAbstract {

	public:
		moVideoBufferPath();
		virtual ~moVideoBufferPath();

		virtual MOboolean  Init();
		virtual MOboolean  Init( moResourceManager* pResources, moText videobufferpath );
		virtual MOboolean  Finish();

		MOboolean	LoadCompleted();
		moText      GetPath();
		moText      GetCompletePath();
		int         GetTotalFiles();
		int         GetImagesProcessed();
		int         GetActualImage();

		//MOboolean LoadFromVideo(  moText p_moviefile );
		MOboolean UpdateImages( MOint maxfiles = -1 );
        moVideoBuffers	m_VideoBuffers;

    private:
		moResourceManager*	m_pResourceManager;

		moText m_VideoBufferPath;
		moText m_CompletePath;

		moDirectory*	m_pDirectory;

		MOint m_ImagesProcessed;
		MOint m_TotalFiles;
		MOboolean	m_bLoadCompleted;
		MOint m_ActualImage;
};

typedef moVideoBufferPath* moVideoBufferPathPtr;

moDeclareExportedDynamicArray(moVideoBufferPathPtr,moVideoBufferPaths);

//could be: Video, Camera, or VideoLoop...
class LIBMOLDEO_API moVideoIn : public moAbstract {

	public:
		moVideoIn();
		virtual ~moVideoIn();

		virtual MOboolean Init();
		virtual MOboolean Finish();

	private:

};

//could be: Video, Camera, or VideoLoop...
class LIBMOLDEO_API moVideoOut : public moAbstract {

	public:
		moVideoOut();
		virtual ~moVideoOut();

		virtual MOboolean Init();
		virtual MOboolean Finish();


};

typedef moLiveSystem moCamera;

class LIBMOLDEO_API moVideoManager : public moResource
{
	public:

		moVideoManager();
		virtual ~moVideoManager();

		virtual MOboolean Init();
		virtual MOboolean Finish();

    virtual void Update(moEventList* p_EventList);

		MOswitch GetStatus(MOdevcode);
		MOswitch SetStatus( MOdevcode,MOswitch);
		MOint GetValue(MOdevcode);
		MOpointer GetPointer( MOdevcode devcode );
		MOdevcode GetCode( moText);

    /**
    * Devuelve el objeto moCamera de cb_idx
    * Se consigue el cam_idx con la función GetCameraByName, y el nombre del dispositivo de captura
    * con las funciones GetCaptureDevices
    */
		moCamera* GetCamera( int cam_idx );

    /**
    * Devuelve el objeto moCamera cuyo Device Name sea camera
    * en caso de no encontrarlo y el load es true intenta inicializar el dispositivo/camara
    * @param camera es el nombre del dispositivo de captura o cámara asignado por el sistema operativo (v4l2src|dshowvideosrc|osxvideosrc)
    * @param load si es true inicializa el dispositivo
    */
		moCamera* GetCameraByName( const moText& camera, bool load=false, moCaptureDevice customCD = moCaptureDevice() );

		moCamera* CreateCamera( const moCaptureDevice& p_CapDev );

    /**
    * Devuelve una lista de nombres de dispositivos disponibles
    */
    const moTextArray& GetCameraNames();

    /**
    * Devuelve el arreglo de dispostivos disponibles (moCapureDevice)
    * @param reload si es true vuelve a consultar al sistema el listado
    * de dispositivos disponibles
    */
    const moCaptureDevices& GetCaptureDevices( bool reload = true );

    /**
    * Devuelve la cantidad de objetos de moCamera disponibles
    */
		int GetCameraCount();

    /**
    * Devuelve el objeto moCircularVideoBuffer de cb_idx
    */
		moCircularVideoBuffer* GetCircularVideoBuffer( int cb_idx );

    /**
    * Devuelve la cantidad de objetos de moCircularVideoBuffer disponibles
    */
		int GetCircularVideoBufferCount();

    /**
    * Devuelve el objeto moVideoBufferPaths del indice vb_idx
    */
		moVideoBufferPath* GetVideoBufferPath( int vb_idx );

    /**
    * Devuelve la cantidad de objetos de moVideoBufferPath disponibles
    */
		int GetVideoBufferPathCount();


    /**
    * Devuelve el objeto moVideoBuffer del indice vb_idx
    */
		moVideoBuffer* GetVideoBuffer( int vb_idx );

    /**
    * Devuelve la cantidad de objetos de moVideoBuffer disponibles
    */
		int GetVideoBufferCount();

    static moText NanosecondsToTimecode( MOulonglong duration );
    static moText FramesToTimecode( MOulonglong duration, double framespersecond );

    moTextArray     m_CameraDevices;

	protected:

		moLiveSystems*		m_pLiveSystems;
		moCaptureDevices  m_CaptureDevices;
		moTextureIndex		Images;

		moCircularVideoBuffers	m_CircularVideoBuffers;
		moVideoBufferPaths	m_VideoBufferPaths;
		MOboolean			m_bLoading;

		bool m_bAutoReconnect;

};

#include "moIODeviceManager.h"
#include "moResourceManager.h"

#endif

