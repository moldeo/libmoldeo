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
  Andrs Colubri

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
typedef void moGstElement;
typedef void moGstPad;
typedef void moGstBus;
typedef void moGstBuffer;
typedef void moGstCaps;
typedef void moGstMessage;
typedef void *moGPointer;
typedef bool moGBoolean;
typedef int moGstStateChangeReturn;
typedef enum
{
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
class LIBMOLDEO_API moGsFramework:public moVideoFramework
{
	public:moGsFramework ();
	virtual ~ moGsFramework ();
	virtual moCaptureDevices * LoadCaptureDevices ();
	virtual moCaptureDevices * UpdateCaptureDevices ();
	virtual bool CheckCaptureDevice (int i);
	virtual bool AddCaptureDevice (moCaptureDevice & p_capdev);
	private:
	#ifdef MO_WIN32
		//ICreateDevEnum *m_pDevEnum;
		//IEnumMoniker *m_pEnum;
	#endif						 /*  */
};

/// GStreamer Graph Class
/**
 *  GStreamer Graph Class for video filters and video reproduction: you don't have to link to gstreamer
 * This is C++ wrapper to some GStreamer basic functions...
 *
 */
class LIBMOLDEO_API moGsGraph:public moVideoGraph
{
	public:
		/// Constructor
		moGsGraph ();

		/// Destructor
		virtual ~ moGsGraph ();

		//================================================
		//      INITIALIZATION AND FINALIZATION METHODS
		//================================================
		virtual bool InitGraph ();
		virtual bool FinishGraph ();
		bool BuildLiveGraph (moBucketsPool * pBucketsPool, moCaptureDevice p_capdev);

		//================================================
		//      FILTER METHODS CONSTRUCTION
		//================================================
		bool SetCaptureDevice (moText deviceport, MOint idevice /**/ = 0);
		bool BuildLiveDVGraph (moBucketsPool * pBucketsPool,
			moCaptureDevice & p_capdev);
		bool BuildLiveWebcamGraph (moBucketsPool * pBucketsPool,
			moCaptureDevice & p_capdev);
		bool BuildLiveVideoGraph (moText filename, moBucketsPool * pBucketsPool);
		bool BuildLiveQTVideoGraph (moText filename, moBucketsPool * pBucketsPool);
		bool BuildLiveSound (moText filename);
		bool BuildLiveStreamingGraph (moBucketsPool * pBucketsPool,
			moText p_location);
		bool BuildRecordGraph (moText filename, moBucketsPool * pBucketsPool);
		virtual moStreamState GetState ();

		//================================================
		//      CONTROL METHODS
		//================================================
		void Play ();
		void Stop ();
		void Pause ();
		void Seek (MOuint frame);
		bool IsRunning ();
								 /** cantidad de cuadros */
		MOulong GetFramesLength ();
		MObyte * GetFrameBuffer (MOlong * size);
		MOulong GetDuration ();	 /** en nanosegundos para gstreamer!!! 1E-9  1 s = 1E9 ns = 1 billion ns */
		MOulong GetPosition ();	 /** en cuadros */

		//================================================
		//      MISC METHODS
		//================================================
		void SetVideoFormat (moGstCaps * caps, moGstBuffer * buffer = NULL);

		/*
		   bool ShowError( HRESULT hr );
		   void SetVideoFormat( AM_MEDIA_TYPE* mt );
		   IPin * GetInPin( IBaseFilter * pFilter, int nPin );
		   IPin * GetOutPin( IBaseFilter * pFilter, int nPin );
		   HRESULT GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
		   void CheckMediaType( IPin* p_Pin );
		   void ShowConfigureDialog(IBaseFilter *pFilter);
		 */
		static moGBoolean bus_call (moGstBus * bus, moGstMessage * msg,
			moGPointer user_data);
		static moGBoolean cb_have_data (moGstPad * pad, moGstBuffer * buffer,
			moGPointer u_data);
		long cb_have_data_handler_id;
		static void cb_newpad (moGstElement * decodebin, moGstPad * pad,
			moGBoolean last, moGPointer u_data);
		long signal_newpad_id;
		static void cb_handoff (moGstElement * fakesrc, moGstBuffer * buffer,
			moGstPad * pad, moGPointer user_data);
		long signal_handoff_id;
		bool CheckState (moGstStateChangeReturn state_change_result,
			bool waitforsync = false);
		void RetreivePads (moGstElement * FilterElement);
		void WaitForFormatDefinition (MOulong timeout);
		void CopyVideoFrame (void *bufferdst, int size);
	private:
		moBucketsPool * m_pBucketsPool;
		moGsFramework * m_pGsFramework;
		moCaptureDevice m_CaptureDevice;

		/** Bin's o Pipeline's (Filtergraph...)*/
								 /**anlogo a ifiltergraph, oh si*/
		moGstElement *m_pGstPipeline;

		/** Elements (filters) */
								 /** "filesrc" */
		moGstElement *m_pFileSource;
								 /** "ffmpegcolorspace" */
		moGstElement *m_pColorSpace;
								 /** "capsfilter" */
		moGstElement *m_pCapsFilter;
								 /** "typefind" */
		moGstElement *m_pTypeFind;
								 /** "identity" */
		moGstElement *m_pIdentity;
		moGstElement * m_pVideoScale;
		moGstElement * m_pVideoDeinterlace;
								 /** "ffmpegcolorspace for deinterlace" */
		moGstElement * m_pColorSpaceInterlace;
								 /** "decodebin" */
		moGstElement * m_pDecoderBin;
								 /** "fakesink" */
		moGstElement *m_pFakeSink;
								 /** "videotestsrc" */
		moGstElement *m_pVideoTestSrc;
								 /** "fakesrc" */
		moGstElement * m_pFakeSource;
								 /** "encoder" */
		moGstElement * m_pEncoder;
								 /** "recorder" */
		moGstElement * m_pMultiplexer;
								 /** "filesink" */
		moGstElement * m_pFileSink;
								 /** "filesink" */
		moGstElement * m_pAudioSink;

		/**audio elements*/
		moGstElement * m_pAudioConverter;

		//GstElement  *m_pAudioConverter;
		/**Pad's o Pines */
		moGstPad *m_pVideoPad;	 /** audio pad last out */
		moGstPad *m_pAudioPad;	 /** video pad last out*/

		/** Control Bus */
		moGstBus * m_pGstBus;	 /** bus, analogo a IMediaControl...*/

		MOulong m_Duration;
		MOulong m_FramesLength;
};
#endif							 /*  */
#endif							 /*  */
