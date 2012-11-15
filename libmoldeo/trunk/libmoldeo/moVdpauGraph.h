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


Class:   moVdpauGraph

Description: DirectShow Device.

Todo:

Example:


==============================================*/

#include "moConfig.h"
#include "moTypes.h"

#include "moThread.h"
#include "moBuckets.h"

#include "moVideoGraph.h"

#ifndef __MO_VDPAUGRAPH_H
#define __MO_VDPAUGRAPH_H

#ifdef MO_VDPAU

typedef void moVdpDevice;
typedef void moVdpDecoder;
typedef void moVdpVideoMixer;
typedef void moVdpPresentationQueueDisplay;
typedef void moVdpPresentationQueue;

typedef void moVdpDecoderProfile;

typedef void moVdpVideoSurface;
typedef void moVdpOutputSurface;
typedef void moVdpBitmapSurface;

typedef void moVdpTime;
typedef void moVdpPictureInfo;
typedef void moVdpBitstreamBuffer;

class LIBMOLDEO_API moVdpauFramework : public moVideoFramework {

	public:

		moVdpauFramework();
		virtual ~moVdpauFramework();

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

/***
  VDPAU Graph Class for video filters and video reproduction: you don't have to link to vdpau to use
  This is C++ wrapper to some VDPAU basic functions...

*/

class LIBMOLDEO_API moVdpauGraph : public moVideoGraph {

public:

	moVdpauGraph();
	virtual ~moVdpauGraph();

//================================================
//	INITIALIZATION AND FINALIZATION METHODS
//================================================
	virtual bool InitGraph();
	virtual bool FinishGraph();

  virtual bool BuildLiveGraph( moBucketsPool* bpool, moCaptureDevice cdev );

  virtual bool BuildLiveVideoGraph( moText livevid, moBucketsPool* bpool );

  virtual bool BuildLiveQTVideoGraph( moText tx, moBucketsPool* bpool );

  virtual bool BuildRecordGraph( moText tx, moBucketsPool* bpool );

//================================================
//	FILTER METHODS CONSTRUCTION
//================================================
	bool SetCaptureDevice( moText deviceport , MOint idevice /**/= 0);


//================================================
//	CONTROL METHODS
//================================================

  virtual moStreamState GetState();

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

private:

    moBucketsPool       *m_pBucketsPool;

    moVdpauFramework*		m_pVdpauFramework;

    moCaptureDevice     m_CaptureDevice;

    /** Bin's o Pipeline's (Filtergraph...)*/
    moVdpDevice          *m_pVdpauDevice; /**análogo a ifiltergraph, oh si*/

    /** Elements (filters) */

    /**audio elements*/

    /**Pad's o Pines */



    /** Control Bus */

    MOulong             m_Duration;
    MOulong             m_FramesLength;


};

#endif

#endif


