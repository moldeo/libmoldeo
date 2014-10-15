/*******************************************************************************

                                moDsGraph.h

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


Class:   moDsGraph

Description: DirectShow Device.

Todo:

Example:


==============================================*/

#ifndef __MO_DSGRAPH_H
#define __MO_DSGRAPH_H

#include "moTypes.h"

#ifdef MO_DIRECTSHOW

#ifndef __STREAMS__
#include "streams.h"
#include "qedit.h"
#include "dvdmedia.h"
#endif

#include "moVideoGraph.h"

class LIBMOLDEO_API moDsFramework : public moVideoFramework {

	public:

		moDsFramework();
		virtual ~moDsFramework();

		virtual moCaptureDevices* LoadCaptureDevices();
		virtual moCaptureDevices* UpdateCaptureDevices();

		virtual bool	CheckCaptureDevice( int i );

		//DirectX
		bool ShowError( HRESULT hr );

	private:
		ICreateDevEnum *m_pDevEnum;
		IEnumMoniker *m_pEnum;

};

class LIBMOLDEO_API moDsSampleGrabberCB : public ISampleGrabberCB
{
private:
	moBucketsPool *m_pBucketsPool;

public:

    // These will get set by the main thread below. We need to
    // know this in order to write out the bmp
    long Width;
    long Height;

    // Fake out any COM ref counting
    //
    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

	STDMETHODIMP SetBucketsPool( moBucketsPool *pBucketsPool ) {

		m_pBucketsPool = pBucketsPool;

		return 0;
	}

    // Fake out any COM QI'ing
    //
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        CheckPointer(ppv,E_POINTER);

        if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
        {
            *ppv =(void *) static_cast<ISampleGrabberCB*>( this );
            return NOERROR;
        }

        return E_NOINTERFACE;
    }


    // We don't implement this one
    //
    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
    {
        return 0;
    }


    // The sample grabber is calling us back on its deliver thread.
    // This is NOT the main app thread!
    //
    STDMETHODIMP BufferCB( double SampleTime, BYTE * pBuffer, long BufferSize )
    {
        //
        // Convert the buffer into a bitmap
        //

		//Add el bucket!!!

		moBucket *pbucket=NULL;

		if(!m_pBucketsPool->IsFull()) {
			pbucket = new moBucket();
			if(pbucket!=NULL) {

				pbucket->SetBuffer( BufferSize,(MOubyte*)pBuffer );

				if(m_pBucketsPool->AddBucket( pbucket )) {

					//Width = 720;
					//Height = 480;
				}
			}
		}

		return 0;
    }
};


class LIBMOLDEO_API moDsGraph : public moVideoGraph {

public:

	moDsGraph();
	virtual ~moDsGraph();

//================================================
//	INITIALIZATION AND FINALIZATION METHODS
//================================================
	virtual bool InitGraph();
	virtual bool FinishGraph();
	bool BuildLiveGraph( moBucketsPool *pBucketsPool, moCaptureDevice p_capdev);

//================================================
//	FILTER METHODS CONSTRUCTION
//================================================
	bool CreateFilterByName( moText p_name, IBaseFilter **ppF);
	bool CreateCaptureDeviceByPath( moText p_Path, IBaseFilter **ppF );
	bool SetCaptureDevice( moText deviceport , MOint idevice /**/= 0);
	bool BuildLiveDVGraph( moBucketsPool *pBucketsPool, MOint idevice = 0);
	bool BuildLiveWebcamGraph( moBucketsPool *pBucketsPool, MOint idevice = 0);
	bool BuildLiveVideoGraph( moText filename , moBucketsPool *pBucketsPool );
	bool BuildLiveQTVideoGraph( moText filename , moBucketsPool *pBucketsPool );



//================================================
//	CONTROL METHODS
//================================================
	void Play();
	void Stop();
	void Pause();
	void Seek( MOuint frame );
	bool IsRunning();
	MOulong	GetFramesLength();
	MObyte* GetFrameBuffer(MOlong *size);

//================================================
//	MISC METHODS
//================================================
	bool ShowError( HRESULT hr );
	void SetVideoFormat( AM_MEDIA_TYPE* mt );
	IPin * GetInPin( IBaseFilter * pFilter, int nPin );
	IPin * GetOutPin( IBaseFilter * pFilter, int nPin );
	HRESULT GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
	void CheckMediaType( IPin* p_Pin );
	void ShowConfigureDialog(IBaseFilter *pFilter);


private:

	IGraphBuilder *m_pFilterGraph;
	IMediaControl *m_pMediaControl;
	IMediaSeeking *m_pMediaSeeking;

//========================
//	PREFERRED  FILTERS:
//========================
	IBaseFilter	*m_pQuicktimeFilter;
	IBaseFilter	*m_pQuicktimeDecoder;
	IBaseFilter	*m_pColorSpaceConverter;
	IBaseFilter	*m_pCaptureFilter;
	IBaseFilter	*m_pSourceFilter;
	IBaseFilter	*m_pDVDecoderFilter;
	IBaseFilter	*m_pAVIDecompressorFilter;
	IBaseFilter	*m_pSampleGrabberBase;
	IBaseFilter	*m_pNullRenderer;
	IBaseFilter	*m_pAviSplitter;
	IBaseFilter	*m_pSplitter;
	IBaseFilter	*m_pFfdshowFilter;

//========================
//	INTERFACES TO FILTERS:
//========================
	ISampleGrabber  *m_pSampleGrabber;
	moDsSampleGrabberCB	m_CB;
	IFileSourceFilter	*m_pFileSourceFilter;

	moDsFramework*		m_pDsFramework;
	moCaptureDevice m_CaptureDevice;

};

#endif

#endif
