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
  Andr�s Colubri

*******************************************************************************/

#ifndef __MOVIDEOMANAGER_H
#define __MOVIDEOMANAGER_H

#include "moGsGraph.h"
#include "moDsGraph.h"

#include "FreeImage.h"

#include "moFBO.h"
#include "moTexture.h"
#include "moVideoGraph.h"


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
#define MO_LIVE_WIDTH		2
#define MO_LIVE_HEIGHT		3
#define MO_LIVE_BITCOUNT	4

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
	LST_VIDEOCAMERA,
	LST_UNKNOWN
};

//custom
class LIBMOLDEO_API moLiveSystem {

	public:

		moLiveSystem();
		moLiveSystem( moText p_CodeName, moLiveSystemType p_Type );
		moLiveSystem( moCaptureDevice p_capdev );

		//Copy constructor
		moLiveSystem( moLiveSystem &p_ls)
		{
			m_Type = p_ls.GetType();
			m_CodeName = p_ls.GetCodeName();
			m_DeviceName = p_ls.GetDeviceName();
			m_CaptureDevice = p_ls.GetCaptureDevice();
			m_pBucketsPool = p_ls.GetBucketsPool();
			m_pVideoGraph = p_ls.GetVideoGraph();
			m_pVideoSample = p_ls.GetVideoSample();
		}

		moLiveSystem& operator = ( moLiveSystem& p_ls) {
			m_Type = p_ls.GetType();
			m_CodeName = p_ls.GetCodeName();
			m_DeviceName = p_ls.GetDeviceName();
			m_CaptureDevice = p_ls.GetCaptureDevice();
			m_pBucketsPool = p_ls.GetBucketsPool();
			m_pVideoGraph = p_ls.GetVideoGraph();
			m_pVideoSample = p_ls.GetVideoSample();
			return (*this);
		}

		virtual ~moLiveSystem();

		bool Init();
		void Finish();

		void SetCodeName( moText p_CodeName );
		void SetDeviceName( moText p_DeviceName );
		void SetType( moLiveSystemType p_Type );
		void SetCaptureDevice( moCaptureDevice p_capdev );
		moLiveSystemType	GetType();
		moText				GetCodeName();
		moText				GetDeviceName();
		moCaptureDevice		GetCaptureDevice();
		moBucketsPool*		GetBucketsPool();
		moVideoGraph*		GetVideoGraph();
		moVideoSample*		GetVideoSample();

	private:

		moLiveSystemType	m_Type;
		moText				m_CodeName;
		moText				m_DeviceName;
		moCaptureDevice		m_CaptureDevice;

		moBucketsPool*		m_pBucketsPool;
		moVideoGraph*		m_pVideoGraph;

		moVideoSample*		m_pVideoSample;

};

typedef moLiveSystem* moLiveSystemPtr;
/*
template class LIBMOLDEO_API moDynamicArray<moLiveSystemPtr>;
typedef  moDynamicArray<moLiveSystemPtr> moLiveSystemPtrs;
*/
moDeclareExportedDynamicArray(moLiveSystemPtr,moLiveSystemPtrs)

class LIBMOLDEO_API moLiveSystems : public moLiveSystemPtrs {

	public:

		moLiveSystems();
		~moLiveSystems();
		bool LoadLiveSystems( moCaptureDevices* p_pPreferredDevices );
		bool UpdateLiveSystems();
		bool UnloadLiveSystems();
		bool GetStatus( MOdevcode p_devcode );
		moVideoFramework* GetVideoFramework();

	protected:

        moVideoFramework*    m_pVideoFramework;

};

class LIBMOLDEO_API moVideoFrame : public moAbstract {

	public:

		moVideoFrame();
		virtual ~moVideoFrame();

		virtual MOboolean Init();
		virtual MOboolean Init( moText bufferformat, FIBITMAP* pImageResult );
		virtual MOboolean Finish();

		MOint		options;
		FIMEMORY	*hmem;
		FREE_IMAGE_FORMAT fif;
		FIBITMAP *m_pImage;

		MOlong m_FrameSize;

		MOuint	m_width;
		MOuint	m_height;

};

typedef moVideoFrame* moVideoFramePtr;
/*
template class LIBMOLDEO_API moDynamicArray<moVideoFramePtr>;
typedef  moDynamicArray<moVideoFramePtr> moVideoFrames;
*/
moDeclareExportedDynamicArray(moVideoFramePtr,moVideoFrames)

class LIBMOLDEO_API moVideoBuffer : public moTextureAnimated {

	public:
		moVideoBuffer();
		virtual ~moVideoBuffer();

		virtual MOboolean  Init();
		virtual MOboolean  Init( moText bufferpath, moText bufferformat, moResourceManager* p_pResourceManager,  MOuint width, MOuint height, MOuint xsource, MOuint ysource, MOuint sourcewidth, MOuint sourceheight, MOint interpolation=0, MOint interpolationjump=0, MOint interpolationtime=0 );
		virtual MOboolean  Finish();

		virtual void GetFrame( MOuint p_i );

		MOboolean LoadImage( FIBITMAP* pImage , MOuint indeximage );

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
/*
template class LIBMOLDEO_API moDynamicArray<moVideoBufferPtr>;
typedef  moDynamicArray<moVideoBufferPtr> moVideoBuffers;
*/
moDeclareExportedDynamicArray(moVideoBufferPtr,moVideoBuffers)

class LIBMOLDEO_API moCircularVideoBuffer : public moTextureAnimated {

	public:
		moCircularVideoBuffer();
		virtual ~moCircularVideoBuffer();

		virtual MOboolean  Init();
		virtual MOboolean  Init( moText videoinput, moText bufferformat, moResourceManager* p_pResourceManager, MOint frames,  MOint width, MOint height, MOint xsource, MOint ysource, MOint sourcewidth, MOint sourceheight );
		virtual MOboolean  Finish();

		virtual void GetFrame( MOuint p_i );

		MOboolean LoadSample( moVideoSample* pvideosample );
		//MOboolean LoadImage( FIBITMAP* pImage , MOuint indeximage );

		MOint GetXSource() { return m_XSource; }
		MOint GetYSource() { return m_YSource; }
		MOint GetSourceWidth() { return m_SourceWidth; }
		MOint GetSourceHeight() { return m_SourceHeight; }
		moText GetVideoInput() { return m_VideoInput; }
		moText	GetBufferFormat() { return m_BufferFormat; }

	private:

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
/*
template class LIBMOLDEO_API moDynamicArray<moCircularVideoBufferPtr>;
typedef  moDynamicArray<moCircularVideoBufferPtr> moCircularVideoBuffers;
*/
moDeclareExportedDynamicArray(moCircularVideoBufferPtr,moCircularVideoBuffers)

class moDirectory;

class LIBMOLDEO_API moVideoBufferPath : public moAbstract {

	public:
		moVideoBufferPath();
		virtual ~moVideoBufferPath();

		virtual MOboolean  Init();
		virtual MOboolean  Init( moResourceManager* pResources, moText videobufferpath );
		virtual MOboolean  Finish();

		MOboolean	LoadCompleted();

		//MOboolean LoadFromVideo(  moText p_moviefile );
		MOboolean UpdateImages( MOint maxfiles = -1 );

		moResourceManager*	m_pResourceManager;

		moText m_VideoBufferPath;
		moText m_CompletePath;

		moDirectory*	m_pDirectory;
		moVideoBuffers	m_VideoBuffers;

		MOint m_ImagesProcessed;
		MOboolean	m_bLoadCompleted;
		MOint m_ActualImage;
};

typedef moVideoBufferPath* moVideoBufferPathPtr;
/*
template class LIBMOLDEO_API moDynamicArray<moVideoBufferPathPtr>;
typedef  moDynamicArray<moVideoBufferPathPtr> moVideoBufferPaths;
*/
moDeclareExportedDynamicArray(moVideoBufferPathPtr,moVideoBufferPaths)

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

	private:

};


class LIBMOLDEO_API moVideoManager : public moResource
{
	public:

		moVideoManager();
		virtual ~moVideoManager();

		virtual MOboolean Init();
		virtual MOboolean Finish();

	    void Update(moEventList*);

		MOswitch GetStatus(MOdevcode);
		MOswitch SetStatus( MOdevcode,MOswitch);
		MOint GetValue(MOdevcode);
		MOpointer GetPointer( MOdevcode devcode );
		MOdevcode GetCode( moText);

	protected:

		moLiveSystems*		m_pLiveSystems;
		moTextureIndex		Images;

		moCircularVideoBuffers	m_CircularVideoBuffers;
		moVideoBufferPaths	m_VideoBufferPaths;
		MOboolean			m_bLoading;

		bool m_bAutoReconnect;

};

#include "moIODeviceManager.h"
#include "moResourceManager.h"

#endif
