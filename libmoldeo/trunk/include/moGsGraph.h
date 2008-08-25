/*******************************************************************************

                                moGsGraph.h

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

/*==============================================


Class:   moGsGraph

Description: DirectShow Device.

Todo:

Example:


==============================================*/

#include "moConfig.h"
#include "moTypes.h"

#include "moThread.h"
#include "moBuckets.h"

#include "moVideoGraph.h"

#ifndef __MO_GSGRAPH_H
#define __MO_GSGRAPH_H

#ifdef MO_GSTREAMER

#include <gst/gst.h>
//#include <gst/controller/gstcontroller.h>
#ifdef MO_WIN32
    //#include "objbase.h"
    //#include "dshow.h"
#endif

typedef enum {
  GST_VIDEO_TEST_SRC_SMPTE,
  GST_VIDEO_TEST_SRC_SNOW,
  GST_VIDEO_TEST_SRC_BLACK,
  GST_VIDEO_TEST_SRC_WHITE,
  GST_VIDEO_TEST_SRC_RED,
  GST_VIDEO_TEST_SRC_GREEN,
  GST_VIDEO_TEST_SRC_BLUE,
  GST_VIDEO_TEST_SRC_CHECKERS1,
  GST_VIDEO_TEST_SRC_CHECKERS2,
  GST_VIDEO_TEST_SRC_CHECKERS4,
  GST_VIDEO_TEST_SRC_CHECKERS8,
  GST_VIDEO_TEST_SRC_CIRCULAR,
  GST_VIDEO_TEST_SRC_BLINK
} GstVideoTestSrcPattern;


class LIBMOLDEO_API moGsFramework : public moVideoFramework {

	public:

		moGsFramework();
		virtual ~moGsFramework();

		virtual moCaptureDevices* LoadCaptureDevices();
		virtual moCaptureDevices* UpdateCaptureDevices();

		virtual bool	CheckCaptureDevice( int i );


	private:

	#ifdef MO_WIN32
        //ICreateDevEnum *m_pDevEnum;
		//IEnumMoniker *m_pEnum;
	#endif

};


class LIBMOLDEO_API moGsGraph : public moVideoGraph {

public:

	moGsGraph();
	virtual ~moGsGraph();

//================================================
//	INITIALIZATION AND FINALIZATION METHODS
//================================================
	virtual bool InitGraph();
	virtual bool FinishGraph();
	bool BuildLiveGraph( moBucketsPool *pBucketsPool, moCaptureDevice p_capdev);

//================================================
//	FILTER METHODS CONSTRUCTION
//================================================
	bool SetCaptureDevice( moText deviceport , MOint idevice /**/= 0);
	bool BuildLiveDVGraph( moBucketsPool *pBucketsPool, MOint idevice = 0);
	bool BuildLiveWebcamGraph( moBucketsPool *pBucketsPool, moText devicename = moText("") );
	bool BuildLiveVideoGraph( moText filename , moBucketsPool *pBucketsPool );
	bool BuildLiveQTVideoGraph( moText filename , moBucketsPool *pBucketsPool );

    bool BuildLiveSound( moText filename  );

//================================================
//	CONTROL METHODS
//================================================
	void Play();
	void Stop();
	void Pause();
	void Seek( MOuint frame );
	bool IsRunning();
	MOulong	GetFramesLength(); /** cantidad de cuadros */
	MObyte* GetFrameBuffer(MOlong *size);
    MOulong GetDuration();  /** en nanosegundos para gstreamer!!! 1E-9  1 s = 1E9 ns = 1 billion ns */
    MOulong GetPosition();  /** en cuadros */

//================================================
//	MISC METHODS
//================================================
    void SetVideoFormat( GstCaps* caps, GstBuffer* buffer = NULL );
	/*
	bool ShowError( HRESULT hr );
	void SetVideoFormat( AM_MEDIA_TYPE* mt );
	IPin * GetInPin( IBaseFilter * pFilter, int nPin );
	IPin * GetOutPin( IBaseFilter * pFilter, int nPin );
	HRESULT GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
	void CheckMediaType( IPin* p_Pin );
	void ShowConfigureDialog(IBaseFilter *pFilter);
	*/
    static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer user_data);
    static gboolean cb_have_data (GstPad    *pad, GstBuffer *buffer, gpointer   u_data);
    static void cb_newpad (GstElement *decodebin, GstPad     *pad, gboolean    last, gpointer    u_data);
    bool CheckState( GstStateChangeReturn state_change_result, bool waitforsync = false);
    void RetreivePads( GstElement* FilterElement);
    void WaitForFormatDefinition( MOulong timeout );
private:

    moBucketsPool       *m_pBucketsPool;

	moGsFramework*		m_pGsFramework;

	moCaptureDevice     m_CaptureDevice;

    /** Bin's o Pipeline's (Filtergraph...)*/
    GstElement          *m_pGstPipeline; /**análogo a ifiltergraph, oh si*/

    /** Elements (filters) */
    GstElement          *m_pFileSource;/** "filesrc" */
    GstElement          *m_pColorSpace;/** "ffmpegcolorspace" */
    GstElement          *m_pCapsFilter; /** "capsfilter" */
    GstElement          *m_pTypeFind; /** "typefind" */
    GstElement          *m_pIdentity; /** "identity" */

    GstElement          *m_pDecoderBin;/** "decodebin" */
    GstElement          *m_pFakeSink; /** "fakesink" */
    GstElement          *m_pVideoTestSrc;/** "videotestsrc" */

    /**audio elements*/
    GstElement   *m_pAudioConverter;
    //GstElement  *m_pAudioConverter;

    /**Pad's o Pines */
    GstPad              *m_pVideoPad;/** audio pad last out */
    GstPad              *m_pAudioPad;/** video pad last out*/



    /** Control Bus */
    GstBus              *m_pGstBus;/** bus, analogo a IMediaControl...*/
    GError              m_GError;

    MOulong             m_Duration;
    MOulong             m_FramesLength;
    static GMainLoop    *loop;


};

#endif

#endif


