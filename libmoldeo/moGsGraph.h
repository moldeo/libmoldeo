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

#ifndef MO_GSTREAMER
#define MO_GSTREAMER
#endif
#ifdef MO_GSTREAMER


typedef void moGstElement;
typedef void moGstPad;
typedef void moGstBus;
typedef void moGstBuffer;
typedef void moGstCaps;
typedef void moGstMessage;
typedef void* moGPointer;
typedef void* moGUserData;
typedef bool moGBoolean;
typedef int moGstStateChangeReturn;
typedef void moGMainLoop;
typedef void moGMainContext;
typedef int moGstCallbackReturn;
typedef void moGstPadProbeInfo;
typedef void moGstAppSink;
typedef int moGstFlowReturn;


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

/***
All functions related to GStreamer Framework

  in Linux:

      use the dv1394src for DV
      and the v4l2src for Webcams

      it's important to grant permission to device in linux running in terminal:
        sudo chmod 666 raw1394

  in Windows:
      dshowvideosrc is used from the dshowvideowrapper


  To test gstreamer in linux run:
    gst-launch -v -m v4l2src ! decodebin ! ffmpegcolorspace ! autovideosink

  To test gstreamer in windows run:
    gst-launch -v -m dv1394src ! decodebin ! ffmpegcolorspace ! autovideosink

  ( tested with last gstreamer in linux: 0.10.20 )
  ( tested with last gstreamer in windows: 0.10.10 from windows official vinaries and special wrapper dshowvideowrapper )

*/
class LIBMOLDEO_API moGsFramework : public moVideoFramework {

	public:

		moGsFramework();
		virtual ~moGsFramework();

		virtual moCaptureDevices* LoadCaptureDevices();
		virtual moCaptureDevices* UpdateCaptureDevices();

		virtual bool	CheckCaptureDevice( int i );
	    virtual bool    AddCaptureDevice(  moCaptureDevice& p_capdev );



	private:

	#ifdef MO_WIN32
        //ICreateDevEnum *m_pDevEnum;
		//IEnumMoniker *m_pEnum;
	#endif

};

/// GStreamer Graph Class
/**
*  GStreamer Graph Class for video filters and video reproduction: you don't have to link to gstreamer
* This is C++ wrapper to some GStreamer basic functions...
*
*/
class LIBMOLDEO_API moGsGraph : public moVideoGraph {

public:

    /// Constructor
	moGsGraph();

	/// Destructor
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
	bool BuildLiveDVGraph( moBucketsPool *pBucketsPool, moCaptureDevice &p_capdev );
	bool BuildLiveWebcamGraph( moBucketsPool *pBucketsPool, moCaptureDevice &p_capdev );
	bool BuildLiveVideoGraph( moText filename , moBucketsPool *pBucketsPool );
	bool BuildLiveQTVideoGraph( moText filename , moBucketsPool *pBucketsPool );

    bool BuildLiveSound( moText filename  );

	bool BuildLiveStreamingGraph( moBucketsPool *pBucketsPool, moText p_location );
    bool BuildRecordGraph( moText filename, moBucketsPool* pBucketsPool);

    virtual moStreamState GetState();
//================================================
//	CONTROL METHODS
//================================================
	void Play();
	void Stop();
	void Pause();
	void Seek( MOuint frame, float rate = 1.0 );
	bool IsRunning();
	MOulong	GetFramesLength(); /** cantidad de cuadros */
	MOulong	GetSamplesLength(); /** cantidad de muestras de sonido */

	MObyte* GetFrameBuffer(MOlong *size);
   virtual MOulong GetDuration();  /** en nanosegundos para gstreamer!!! 1E-9  1 s = 1E9 ns = 1 billion ns */
   virtual MOulong GetPosition();  /** en cuadros */
   virtual MOulong GetPositionMS(); /** en millisegundos */

//================================================
//	MISC METHODS
//================================================
    /**
      Funciones de Audio
    */
    void SetVolume( float volume );
    void SetBalance( float balance );

    void SetPitch( float pitch );
    void SetEchoDelay( float delay );
    void SetEchoIntensity( float intensity );
    void SetEchoFeedback( float feedback );

    /**funciones de video*/
    void SetBrightness( float brightness );
    void SetContrast( float contrast );
    void SetSaturation( float saturation );
    void SetHue( float hue );

    void BuildAudioFilters();

    void    SetVideoFormat( moGstCaps* caps, moGstBuffer* buffer = NULL );
    void    SetAudioFormat( moGstCaps* caps, moGstBuffer* buffer = NULL );
	/*
	bool ShowError( HRESULT hr );
	void SetVideoFormat( AM_MEDIA_TYPE* mt );
	IPin * GetInPin( IBaseFilter * pFilter, int nPin );
	IPin * GetOutPin( IBaseFilter * pFilter, int nPin );
	HRESULT GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
	void CheckMediaType( IPin* p_Pin );
	void ShowConfigureDialog(IBaseFilter *pFilter);
	*/
    /*static moGBoolean bus_call (moGstBus *bus,
                                moGstMessage *msg,
                                moGPointer user_data);
                                */
#ifndef GSTVERSION
    static moGBoolean cb_have_data (moGstPad    *pad,
                                    moGstBuffer *buffer,
                                    moGPointer   u_data);
#else
    static moGstCallbackReturn cb_have_data (moGstPad    *pad,
                                    moGstPadProbeInfo *info,
                                    moGPointer   u_data);
#endif

    static moGBoolean cb_buffer_disconnected (
                                    moGPointer   u_data
                                    );

    long cb_have_data_handler_id;

    static void on_rtsppadd_added(  moGstElement *rtspsrc,
                                    moGstPad *pad,
                                    moGPointer u_data );
    long signal_rtsppad_added_id;

#ifndef GSTVERSION
    static void cb_newpad ( moGstElement *decodebin,
                            moGstPad     *pad,
                            moGBoolean    last,
                            moGPointer    u_data);
#else
    static void cb_pad_added_new ( moGstElement *decodebin,
                            moGstPad     *pad,
                            moGPointer    u_data);

    static moGstFlowReturn appsink_new_sample( moGstAppSink* appsink, moGPointer user_data );
    static moGstFlowReturn  appsink_new_preroll( moGstAppSink* appsink, moGPointer user_data );
    static void appsink_eos( moGstAppSink* appsink, moGPointer user_data );

#endif

    static void cb_pad_added ( moGstElement *decodebin2,
                            moGstPad     *pad,
                            moGPointer    u_data);

    long signal_newpad_id;
    long m_BusWatchId;
#ifndef GSTVERSION
    static void cb_handoff ( moGstElement *fakesrc,
	    moGstBuffer  *buffer,
	    moGstPad     *pad,
	    moGPointer    user_data);
#endif
    long signal_handoff_id;

    bool CheckState( moGstStateChangeReturn state_change_result, bool waitforsync = false);
    void RetreivePads( moGstElement* FilterElement);
    void WaitForFormatDefinition( MOulong timeout );

    void CopyVideoFrame( void* bufferdst, int size );
    virtual bool IsEOS();
    virtual void SetEOS(bool iseos);

    moBucket           m_Buckets[100];

private:


    moBucketsPool       *m_pBucketsPool;


	moGsFramework*		m_pGsFramework;

	moCaptureDevice     m_CaptureDevice;

    /** Bin's o Pipeline's (Filtergraph...)*/
    moGstElement          *m_pGstPipeline; /**análogo a ifiltergraph, oh si*/

    /** Elements (filters) */
    moGstElement          *m_pFileSource;/** "filesrc" */
    moGstElement          *m_pRTSPSource;/** "rtsp depay" */
    moGstElement          *m_pRTSPDepay;/** "rtsp depay" */
    moGstElement          *m_pHTTPSource;/** "http source" */
    moGstElement          *m_pMultipartDemux;/** "multipardemux for souphttpsrc" */
    moGstElement          *m_pJpegDecode;/** "jpeg decode" */
    moGstElement          *m_pFinalSource;/** "finalsource" */
    moGstElement          *m_pColorSpace;/** "ffmpegcolorspace" */
    moGstElement          *m_pCapsFilter; /** "capsfilter" */
    moGstElement          *m_pTypeFind; /** "typefind" */
    moGstElement          *m_pIdentity; /** "identity" */

    moGstElement          *m_pVideoScale;
    moGstElement          *m_pVideoFlip;
    moGstElement          *m_pVideoDeinterlace;
    moGstElement          *m_pVideoBalance;
    moGstElement          *m_pColorSpaceInterlace;/** "ffmpegcolorspace for deinterlace" */

    moGstElement          *m_pDecoderBin;/** "decodebin" */
    moGstElement          *m_pFakeSink; /** "fakesink" */
    moGstElement          *m_pVideoTestSrc;/** "videotestsrc" */

    moGstElement          *m_pFakeSource;/** "fakesrc" */
    moGstElement          *m_pEncoder;/** "encoder" */
    moGstElement          *m_pMultiplexer;/** "recorder" */
    moGstElement          *m_pFileSink;/** "filesink" */

    moGstElement          *m_pAudioSink;/** "filesink" */

    /**audio elements*/
    moGstElement          *m_pAudioConverter;
    moGstElement          *m_pAudioConverter2;
    moGstElement          *m_pAudioConverter3;
    moGstElement          *m_pAudioConverter4;
    ///stereo balance-1 a 1: panorama
    moGstElement          *m_pAudioPanorama;
    ///echo effect : audioecho > delay [ nanoseconds 10E-9, intensity, feedback ]
    moGstElement          *m_pAudioEcho;
    ///speed:pitch speed 0.1 - 40.0
    moGstElement          *m_pAudioSpeed;
    ///volume: volume, 0 - 10 [1]
    moGstElement          *m_pAudioVolume;
    ///audioamplify: amplification: -inf +inf (dangerous)
    moGstElement          *m_pAudioAmplify;

    //GstElement  *m_pAudioConverter;

    /**Pad's o Pines para el DecoderBin*/
    moGstPad              *m_pVideoPad;/** audio pad last out */
    moGstPad              *m_pAudioPad;/** video pad last out*/

    /**Pad's o Pines para el DecoderBin*/
    moGstPad             *m_pRTSPDepaySink;
    moGstPad             *m_pRTSPSrcVideo;
    moGstPad             *m_pRTSPSrcAudio;

    /** Control Bus */
    moGstBus              *m_pGstBus;/** bus, analogo a IMediaControl...*/

    moGMainLoop         *m_pGMainLoop;
    moGMainContext         *m_pGMainContext;

    MOulong             m_Duration;
    MOulong             m_FramesLength;
    MOulong             m_SamplesLength;

    bool                m_bEOS;


};

#endif

#endif
