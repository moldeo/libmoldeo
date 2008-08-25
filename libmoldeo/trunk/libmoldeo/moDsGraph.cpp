/*******************************************************************************

                              moDsGraph.cpp

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

#include "moDsGraph.h"


#ifdef MO_DIRECTSHOW


//===========================================
//
//     Class: moDsFramewwork
//
//===========================================

moDsFramework::moDsFramework() {
	m_pDevEnum = NULL;
	m_pEnum = NULL;
}


moDsFramework::~moDsFramework() {

	if (m_pDevEnum!=NULL) {
		m_pDevEnum->Release();
		m_pDevEnum = NULL;
	}

	if (m_pEnum!=NULL) {
		m_pEnum->Release();
		m_pEnum = NULL;
	}

}

bool 
moDsFramework::ShowError( HRESULT hr ) {
    if(FAILED(hr))
    {
        TCHAR szErr[MAX_ERROR_TEXT_LEN];
        DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);
        if(res == 0)
        {
            wsprintf(szErr, "Unknown Error: 0x%2x", hr);
        }
        MessageBox(0, szErr, TEXT("Error!"), MB_OK | MB_ICONERROR);
		return false;
    } else return true;
}


moCaptureDevices* moDsFramework::LoadCaptureDevices() {

	//DIRECT SHOW TEST//
	HRESULT hr;

	// Create the System Device Enumerator.
	if(m_pDevEnum==NULL) {
		HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
			CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
			reinterpret_cast<void**>(&m_pDevEnum));

		if(SUCCEEDED(hr) && m_pEnum==NULL)
		{
			// Create an enumerator for the video capture category.
			hr = m_pDevEnum->CreateClassEnumerator(
				CLSID_VideoInputDeviceCategory,
				&m_pEnum, 0);
		} else {
			ShowError(hr);
			return &m_CaptureDevices;
		}
	}
	
	if( m_pEnum )
	{
		m_pEnum->Reset();

		printf("Capture devices: \n");
		IMoniker *pMoniker = NULL;
		while(m_pEnum->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
				(void**)(&pPropBag));
			if(FAILED(hr)) {
				pMoniker->Release();
				continue;  // Skip this one, maybe the next one will work.
			} 
			// Find the description or friendly name.
			moText FriendlyName = "";
			moText Description = "";
			moText DevicePath = "";

			VARIANT varFriendlyName;
			VARIANT varDescription;
			VARIANT varDevicePath;

			VariantInit(&varFriendlyName);
			VariantInit(&varDescription);
			VariantInit(&varDevicePath);

			hr = pPropBag->Read(L"FriendlyName", &varFriendlyName, 0);			
			if(SUCCEEDED(hr)) FriendlyName =(short*)varFriendlyName.bstrVal;

			hr = pPropBag->Read(L"Description", &varDescription, 0);
			if(SUCCEEDED(hr)) Description =(short*)varDescription.bstrVal;

			hr = pPropBag->Read(L"DevicePath", &varDevicePath, 0);
			if(SUCCEEDED(hr)) DevicePath =(short*)varDevicePath.bstrVal;
			
			moCaptureDevice CapDev( FriendlyName, Description, DevicePath );
			CapDev.Present(true);

			m_CaptureDevices.Add( CapDev );			

			pPropBag->Release();
			pMoniker->Release();
		} 
	} else {
		printf("WARNING: No capture devices available...\n");
		return &m_CaptureDevices;
	}

	return &m_CaptureDevices;

}



moCaptureDevices* moDsFramework::UpdateCaptureDevices() {
	
	HRESULT hr;
	MOuint K;
	moCaptureDevice CapDev;

	for( K=0; K < m_CaptureDevices.Count(); K++) {
		CapDev = m_CaptureDevices.Get(K);
		CapDev.Present( false );
		m_CaptureDevices.Set( K , CapDev );		
	}

	if (m_pEnum)  {
		m_pEnum->Release();
		m_pEnum = NULL;
	}

	if (m_pDevEnum) {
		hr = m_pDevEnum->CreateClassEnumerator(
		CLSID_VideoInputDeviceCategory,
		&m_pEnum, 0);
	}

	if( m_pEnum )
	{
		m_pEnum->Reset();		

		IMoniker *pMoniker = NULL;
		while(m_pEnum->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
				(void**)(&pPropBag));
			if(FAILED(hr)) {
				pMoniker->Release();
				continue;  // Skip this one, maybe the next one will work.
			} 
			// Find the description or friendly name.
			moText FriendlyName = "";
			moText Description = "";
			moText DevicePath = "";

			VARIANT varFriendlyName;
			VARIANT varDescription;
			VARIANT varDevicePath;

			VariantInit(&varFriendlyName);
			VariantInit(&varDescription);
			VariantInit(&varDevicePath);

			hr = pPropBag->Read(L"FriendlyName", &varFriendlyName, 0);			
			if(SUCCEEDED(hr)) FriendlyName =(short*)varFriendlyName.bstrVal;

			hr = pPropBag->Read(L"Description", &varDescription, 0);
			if(SUCCEEDED(hr)) Description =(short*)varDescription.bstrVal;

			hr = pPropBag->Read(L"DevicePath", &varDevicePath, 0);
			if(SUCCEEDED(hr)) DevicePath =(short*)varDevicePath.bstrVal;

			bool CapDevFounded = false;
			for( K=0; K < m_CaptureDevices.Count(); K++) {
				if ( m_CaptureDevices.Get(K).GetPath() == DevicePath ) {
					//AN OLD ONE... :-D CONFIRMS HIS PRESENCE
					CapDev = m_CaptureDevices.Get(K);
					CapDev.Present( true );
					m_CaptureDevices.Set( K , CapDev );					
					CapDevFounded = true;
				}
			}

			if (!CapDevFounded) {				
				//A NEW ONE!!!!
				m_CaptureDevices.Add( moCaptureDevice( FriendlyName, Description, DevicePath ) );
			}

			pPropBag->Release();
			pMoniker->Release();
		} 
	} else {
		//printf("WARNING: No capture devices available...\n");
		return &m_CaptureDevices;
	}

	return &m_CaptureDevices;

}


bool
moDsFramework::CheckCaptureDevice( int i ) {

	HRESULT hr;

	if (m_pEnum)  {
		m_pEnum->Release();
		m_pEnum = NULL;
	}


	if (m_pDevEnum) {
		hr = m_pDevEnum->CreateClassEnumerator(
		CLSID_VideoInputDeviceCategory,
		&m_pEnum, 0);
	}

	if( m_pEnum )
	{	
		m_pEnum->Reset();

		IMoniker *pMoniker = NULL;
		while(m_pEnum->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
				(void**)(&pPropBag));
			if(FAILED(hr)) {
				pMoniker->Release();
				continue;  // Skip this one, maybe the next one will work.
			} 
			// Find the description or friendly name.
			moText FriendlyName = "";
			moText Description = "";
			moText DevicePath = "";

			VARIANT varFriendlyName;
			VARIANT varDescription;
			VARIANT varDevicePath;

			VariantInit(&varFriendlyName);
			VariantInit(&varDescription);
			VariantInit(&varDevicePath);

			hr = pPropBag->Read(L"FriendlyName", &varFriendlyName, 0);			
			if(SUCCEEDED(hr)) FriendlyName =(short*)varFriendlyName.bstrVal;

			hr = pPropBag->Read(L"Description", &varDescription, 0);
			if(SUCCEEDED(hr)) Description =(short*)varDescription.bstrVal;

			hr = pPropBag->Read(L"DevicePath", &varDevicePath, 0);
			if(SUCCEEDED(hr)) DevicePath =(short*)varDevicePath.bstrVal;			

			pPropBag->Release();
			pMoniker->Release();
			if ( DevicePath == m_CaptureDevices.Get(i).GetPath() ) 
				return true;
		} 
	} else {
		return false;
	}

	return false;

}

//===========================================
//
//     Class: moDsGraph
//
//===========================================

moDsGraph::moDsGraph() {
	
	m_pFilterGraph = NULL;
	m_pMediaControl = NULL;
	m_pMediaSeeking = NULL;

//PREFERRED  FILTERS:
	m_pQuicktimeFilter = NULL;
	m_pColorSpaceConverter = NULL;
	m_pCaptureFilter = NULL;
	m_pSourceFilter = NULL;
	m_pFileSourceFilter = NULL;
	m_pAviSplitter =  NULL;
	m_pSplitter = NULL;
	
	//QUICKTIME ALTERNATIVE
	m_pQuicktimeDecoder = NULL;

	//FFDSHOW
	m_pFfdshowFilter = NULL;

	//DV
	m_pDVDecoderFilter = NULL;
	//WEBCAM AVI
	m_pAVIDecompressorFilter = NULL;

	//COMMON
	m_pSampleGrabberBase = NULL;
	m_pSampleGrabber = NULL;
	m_pNullRenderer = NULL;

//

}

moDsGraph::~moDsGraph() {
	//last try to release objects
	FinishGraph();
}

	//INIT METHODS
bool
moDsGraph::InitGraph() {

	HRESULT hr = S_OK;

	if(!m_pFilterGraph) {

		hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER,
            IID_IGraphBuilder,(void**)&m_pFilterGraph);
		
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->QueryInterface(IID_IMediaControl,(void**)&m_pMediaControl);
			if(SUCCEEDED(hr)) {
				hr = m_pFilterGraph->QueryInterface(IID_IMediaSeeking,(void**)&m_pMediaSeeking);
				if(FAILED(hr))
					ShowError(hr);
			} else ShowError(hr);
		} else ShowError(hr);
	}

	return(m_pFilterGraph!=NULL);
}


bool 
moDsGraph::FinishGraph() {

	ULONG rc;

	if (IsRunning()) {
		Stop();
	}

	if(m_pSampleGrabberBase) {
		if(m_pSampleGrabber) {
			HRESULT hr;
			//Cancel Callback
			hr = m_pSampleGrabber->SetCallback( NULL , 1 );
				if(FAILED(hr)) 
					return ShowError(hr);
		}
		rc = m_pSampleGrabberBase->Release();
		m_pSampleGrabberBase = NULL;
		m_pSampleGrabber = NULL;
	}

	if(m_pCaptureFilter) {
		rc = 1;
		rc = m_pCaptureFilter->Release();
		m_pCaptureFilter = NULL;
	}

	if (m_pFileSourceFilter) {
		rc = m_pFileSourceFilter->Release();
		m_pFileSourceFilter = NULL;
	}

	if(m_pSourceFilter) {
		rc = m_pSourceFilter->Release();
		m_pSourceFilter = NULL;		
	}

	if(m_pQuicktimeDecoder) {
		rc = m_pQuicktimeDecoder->Release();
		m_pQuicktimeDecoder = NULL;
	}

	if(m_pFfdshowFilter) {
		rc = m_pFfdshowFilter->Release();
		m_pFfdshowFilter = NULL;
	}

	if(m_pDVDecoderFilter) {
		rc = m_pDVDecoderFilter->Release();
		m_pDVDecoderFilter = NULL;
	}

	if(m_pAVIDecompressorFilter) {
		rc = m_pAVIDecompressorFilter->Release();
		m_pAVIDecompressorFilter = NULL;
	}

	if(m_pNullRenderer) {
		rc = m_pNullRenderer->Release();
		m_pNullRenderer = NULL;
	}

	if(m_pAviSplitter) {
		rc = m_pAviSplitter->Release();
		m_pAviSplitter = NULL;
	}

	if(m_pSplitter) {
		rc = m_pSplitter->Release();
		m_pSplitter = NULL;
	}

	if(m_pMediaControl) {
		rc = m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}
	
	if(m_pMediaSeeking) {
		rc = m_pMediaSeeking->Release();
		m_pMediaSeeking = NULL;
	}

	if(m_pFilterGraph) {
		rc = m_pFilterGraph->Release();
		m_pFilterGraph = NULL;
	}

	return !(m_pFilterGraph && m_pCaptureFilter && m_pSampleGrabberBase);
}


bool
moDsGraph::CreateCaptureDeviceByPath( moText p_Path, IBaseFilter **ppF ) {

    if (! ppF) return false;
    *ppF = 0;
    IBaseFilter *pF = 0;

	ICreateDevEnum *pDevEnum = NULL;
	IEnumMoniker *pEnum = NULL;

	// Create the System Device Enumerator.
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));

	if(SUCCEEDED(hr))
	{
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,
			&pEnum, 0);
	} else return ShowError(hr);
	
	if( pEnum )
	{
		printf("DirectShow filters: \n");
		IMoniker *pMoniker = NULL;
		while(pEnum->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
				(void**)(&pPropBag));
			if(FAILED(hr)) {
				pMoniker->Release();
				continue;  // Skip this one, maybe the next one will work.
			} 
			// Find the description or friendly name.
			VARIANT varName;
			VariantInit(&varName);
			hr = pPropBag->Read(L"DevicePath", &varName, 0);					
			if(SUCCEEDED(hr)) {
				moText fname;
				fname =(short*)varName.bstrVal;
				// To create an instance of the filter, do the following:		
				if( fname==p_Path ) {
					hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)ppF);
					if(FAILED(hr)) {
						pPropBag->Release();
						pMoniker->Release();
						pEnum->Release();
						pDevEnum->Release();
						return ShowError(hr);
					} else {
						pPropBag->Release();
						pMoniker->Release();
						pEnum->Release();
						pDevEnum->Release();
						return true;
					}					
				}
			}
			pPropBag->Release();
			pMoniker->Release();
		} 
		pEnum->Release();
		pDevEnum->Release();
	} else {
		printf("WARNING: No filter found with that path...\n");		
		pDevEnum->Release();
		return false;
	}

	return false;

}

bool
moDsGraph::CreateFilterByName( moText p_Name, IBaseFilter **ppF )      // Receives a pointer to the filter.
{
    if (! ppF) return false;
    *ppF = 0;
    IBaseFilter *pF = 0;

	ICreateDevEnum *pDevEnum = NULL;
	IEnumMoniker *pEnum = NULL;

	// Create the System Device Enumerator.
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));

	if(SUCCEEDED(hr))
	{
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_LegacyAmFilterCategory,
			&pEnum, 0);
	} else return ShowError(hr);
	
	if( pEnum )
	{
		printf("DirectShow filters: \n");
		IMoniker *pMoniker = NULL;
		while(pEnum->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
				(void**)(&pPropBag));
			if(FAILED(hr)) {
				pMoniker->Release();
				continue;  // Skip this one, maybe the next one will work.
			} 
			// Find the description or friendly name.
			VARIANT varName;
			VariantInit(&varName);
			hr = pPropBag->Read(L"Description", &varName, 0);
			if(FAILED(hr)) {
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);				
				printf( "FriendlyName: " );
				//wprintf((const wchar_t *)varName.bstrVal);
				printf( "\n" );
			} else {
				printf( "Description: " );
				//wprintf((const wchar_t *)varName.bstrVal);
				printf( "\n" );
			}			
			if(SUCCEEDED(hr)) {
				moText fname;
				fname =(short*)varName.bstrVal;
				// To create an instance of the f.ilter, do the following:		
				if( fname==p_Name ) {
					hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)ppF);
					if(FAILED(hr)) {
						pPropBag->Release();
						pMoniker->Release();
						pEnum->Release();
						pDevEnum->Release();
						return ShowError(hr);
					} else {
						pPropBag->Release();
						pMoniker->Release();
						pEnum->Release();
						pDevEnum->Release();
						return true;
					}					
				}
			}
			pPropBag->Release();
			pMoniker->Release();
		} 
		pEnum->Release();
		pDevEnum->Release();
	} else {
		printf("WARNING: No filter found with that name...\n");
		pDevEnum->Release();
		return false;
	}

	return false;
}

//FILTER METHODS
bool
moDsGraph::SetCaptureDevice( moText deviceport, MOint idevice) {

	ICreateDevEnum *pDevEnum = NULL;
	IEnumMoniker *pEnum = NULL;
	int ndevice = 0;

	//DIRECT SHOW TEST//

	// Create the System Device Enumerator.
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));

	if(SUCCEEDED(hr))
	{
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,
			&pEnum, 0);
	} else return ShowError(hr);
	
	if( pEnum )
	{
		printf("Capture devices: \n");
		IMoniker *pMoniker = NULL;
		while(pEnum->Next(1, &pMoniker, NULL) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
				(void**)(&pPropBag));
			if(FAILED(hr)) {
				pMoniker->Release();
				continue;  // Skip this one, maybe the next one will work.
			} 
			// Find the description or friendly name.
			VARIANT varName;
			VariantInit(&varName);
			hr = pPropBag->Read(L"Description", &varName, 0);
			if(FAILED(hr)) {
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);				
				printf( "FriendlyName: " );
				//wprintf_s((const wchar_t *)varName.bstrVal);
				printf( "\n" );
			} else {
				printf( "Description: " );
				//wprintf_s((const wchar_t *)varName.bstrVal);
				printf( "\n" );
			}			
			if(SUCCEEDED(hr)) {
				moText fname;
				fname =(short*)varName.bstrVal;
				// To create an instance of the filter, do the following:		
				hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)&m_pCaptureFilter);
				if(FAILED(hr)) {
					pPropBag->Release();
					pMoniker->Release();
					pEnum->Release();
					pDevEnum->Release();
					return ShowError(hr);
				} else {
					pPropBag->Release();
					pMoniker->Release();
					pEnum->Release();
					pDevEnum->Release();
					return true;
				}
				/*
				if((fname=="Microsoft DV Camera and VCR" || fname=="Sony DV Device" || fname=="Canon DV Device") &&(deviceport=="IEEE1394") ) {
					if(ndevice==idevice) {
						hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)&m_pCaptureFilter);
						if(FAILED(hr)) {
							pPropBag->Release();
							pMoniker->Release();
							return ShowError(hr);
						} else {
							pPropBag->Release();
							pMoniker->Release();
							return true;
						}

					} else ndevice++;
				} else if((fname=="D-Link VGA Webcam" || fname=="VideoCAM Messenger" || fname=="Acer OrbiCam") &&(deviceport=="WEBCAM") ) {
					if(ndevice==idevice) {
					hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)&m_pCaptureFilter);
						if(FAILED(hr)) {
							pPropBag->Release();
							pMoniker->Release();
							return ShowError(hr);
						} else {
							pPropBag->Release();
							pMoniker->Release();
							return true;
						}
					} else ndevice++;
				}
				*/
			}
			pPropBag->Release();
			pMoniker->Release();
		} 
		pEnum->Release();
		pDevEnum->Release();
	} else {
		printf("WARNING: No capture devices available...\n");
		pDevEnum->Release();
		return false;
	}

	return false;
}

//Displaying a Filter's Property Pages
void
moDsGraph::ShowConfigureDialog(IBaseFilter *pFilter) {

	/* Obtain the filter's IBaseFilter interface. (Not shown) */
	ISpecifyPropertyPages *pProp;
	HRESULT hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pProp);
	if (SUCCEEDED(hr)) 
	{
		// Get the filter's name and IUnknown pointer.
		FILTER_INFO FilterInfo;
		hr = pFilter->QueryFilterInfo(&FilterInfo); 
		IUnknown *pFilterUnk;
		pFilter->QueryInterface(IID_IUnknown, (void **)&pFilterUnk);

		// Show the page. 
		CAUUID caGUID;
		pProp->GetPages(&caGUID);
		pProp->Release();
		OleCreatePropertyFrame(
			NULL,                   // Parent window
			0, 0,                   // Reserved
			FilterInfo.achName,     // Caption for the dialog box
			1,                      // Number of objects (just the filter)
			&pFilterUnk,            // Array of object pointers. 
			caGUID.cElems,          // Number of property pages
			caGUID.pElems,          // Array of property page CLSIDs
			0,                      // Locale identifier
			0, NULL                 // Reserved
		);

		// Clean up.
		pFilterUnk->Release();
		FilterInfo.pGraph->Release(); 
		CoTaskMemFree(caGUID.pElems);
	}




}

bool
moDsGraph::BuildLiveGraph( moBucketsPool *pBucketsPool, moCaptureDevice p_capdev) {

	HRESULT hr;

	IPin* pPinSource;
	IPin* pPinDecoderIn;
	IPin* pPinDecoderOut;
    IPin* pGrabPinIn;
	IPin* pGrabPinOut;
	IPin* pRendererPin;

	//media type enumaration
	IEnumMediaTypes* ppEnum;
	AM_MEDIA_TYPE* pMediaTypes;



	//SOURCE
	if ( p_capdev.GetPath() != "" ) {
		
		if(CreateCaptureDeviceByPath( p_capdev.GetPath(), &m_pCaptureFilter )) {
			hr = m_pFilterGraph->AddFilter(m_pCaptureFilter, L"Capture Device");

			//ShowConfigureDialog(m_pCaptureFilter);
			
			if(SUCCEEDED(hr)) {
			
				pPinSource = GetOutPin( m_pCaptureFilter ,0 );

			} else return ShowError(hr);
		} else return false;

	} else return false;

	//CHEQUEAMOS EL FORMATO DEL PIN
	bool isdv = false;	

	AM_MEDIA_TYPE MTPreferredResolution;	
	AM_MEDIA_TYPE MTDefaultResolution;

	MTDefaultResolution.majortype = GUID_NULL;
	MTPreferredResolution.majortype = GUID_NULL;

	moVideoFormat LastFormat;
	hr = pPinSource->EnumMediaTypes( &ppEnum);

	if(SUCCEEDED(hr)) {

		while( ppEnum->Next( 1, &pMediaTypes, NULL) == S_OK ) {

			if ( pMediaTypes->majortype == MEDIATYPE_Video ) { //VIDEO

				if ( pMediaTypes->subtype == MEDIASUBTYPE_dvsl ||
				     pMediaTypes->subtype == MEDIASUBTYPE_dvsd ||
					 pMediaTypes->subtype == MEDIASUBTYPE_dvhd ) {					

					isdv = true;
					
					SetVideoFormat( pMediaTypes );

					if (MTDefaultResolution.majortype == GUID_NULL)
						MTDefaultResolution = (*pMediaTypes);
					
					if (p_capdev.GetVideoFormat().m_Width > 0) {
						if ( p_capdev.GetVideoFormat().m_Width == m_VideoFormat.m_Width &&
							p_capdev.GetVideoFormat().m_Height == m_VideoFormat.m_Height &&
							p_capdev.GetVideoFormat().m_BitCount == m_VideoFormat.m_BitCount ) {

							MTPreferredResolution = (*pMediaTypes);
							
						}
					}

					LastFormat = m_VideoFormat;				
					//continuamos hasta terminar...
				} else {//EVERYTHING ELSE MAY BE UNCOMPRESSED
					if ( pMediaTypes->formattype == FORMAT_VideoInfo ) {					
						
						SetVideoFormat( pMediaTypes );
						
						if (MTDefaultResolution.majortype == GUID_NULL)
							MTDefaultResolution = (*pMediaTypes);

						if (p_capdev.GetVideoFormat().m_Width > 0) {
							if ( p_capdev.GetVideoFormat().m_Width == m_VideoFormat.m_Width &&
								p_capdev.GetVideoFormat().m_Height == m_VideoFormat.m_Height &&
								p_capdev.GetVideoFormat().m_BitCount == m_VideoFormat.m_BitCount ) {

								MTPreferredResolution = (*pMediaTypes);
								
							}
						}

						LastFormat = m_VideoFormat;
					}				
				}
			}
		}
	}
	ppEnum->Release();	

	//FORMATO DV
	if ( isdv ) {

		//DV DECODER
		if(!m_pDVDecoderFilter) {
			hr = CoCreateInstance(CLSID_DVVideoCodec, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter,(void**)&m_pDVDecoderFilter);
			if(SUCCEEDED(hr)) {
				hr = m_pFilterGraph->AddFilter(m_pDVDecoderFilter, L"DV Decoder");
				if(SUCCEEDED(hr)) {
					//mostrar la pagina de configuracion del fitro
					//sacar pines				
					pPinDecoderIn = GetInPin( m_pDVDecoderFilter ,0 );
					pPinDecoderOut = GetOutPin( m_pDVDecoderFilter ,0 );

					hr = m_pFilterGraph->Connect( pPinSource , pPinDecoderIn );

					if(FAILED(hr)) 
						return ShowError(hr);
				} else return ShowError(hr);			
			} else return ShowError(hr);
		} else return ShowError(hr);

	} else {
	
		//AVI DECOMPRESSOR
		if(!m_pAVIDecompressorFilter) {
			hr = CoCreateInstance(CLSID_AVIDec, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter,(void**)&m_pAVIDecompressorFilter);
			if(SUCCEEDED(hr)) {
				hr = m_pFilterGraph->AddFilter(m_pAVIDecompressorFilter, L"AVI Decompressor");
				if(SUCCEEDED(hr)) {
					//mostrar la pagina de configuracion del fitro
					//sacar pines				
					pPinDecoderIn = GetInPin( m_pAVIDecompressorFilter ,0 );
					pPinDecoderOut = GetOutPin( m_pAVIDecompressorFilter ,0 );

					//hr = m_pFilterGraph->Connect( pPinSource , pPinDecoderIn );

					if ( MTPreferredResolution.majortype != GUID_NULL ) {
						hr = pPinSource->Connect( pPinDecoderIn ,  &MTPreferredResolution );
						if (SUCCEEDED(hr)) {
							SetVideoFormat( &MTPreferredResolution );
						} else {
							hr = pPinSource->Connect( pPinDecoderIn ,  &MTDefaultResolution );
							if (SUCCEEDED(hr)) {
								SetVideoFormat( &MTDefaultResolution );
							} else {
								hr = m_pFilterGraph->Connect( pPinSource, pPinDecoderIn );
								if (SUCCEEDED(hr)) {
									AM_MEDIA_TYPE mt;
									if ( pPinDecoderIn->ConnectionMediaType(&mt) == S_OK ) {		
										SetVideoFormat( &mt );			
									}
								}
							}
						}
					} else {
						hr = m_pFilterGraph->Connect( pPinSource, pPinDecoderIn );
						//hr = pPinSource->Connect( pPinDecoderIn ,  &MTDefaultResolution );
						//SetVideoFormat( &MTDefaultResolution );
					}

					if(FAILED(hr))	
						return ShowError(hr);
				} else return ShowError(hr);			
			} else return ShowError(hr);
		} else return ShowError(hr);
	}
		
	//FRAME GRABBER
	if(!m_pSampleGrabberBase) {
		hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pSampleGrabberBase);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pSampleGrabberBase, L"Sample Grabber");
			if(SUCCEEDED(hr)) {

				pGrabPinIn = GetInPin( m_pSampleGrabberBase ,0 );
				pGrabPinOut = GetOutPin( m_pSampleGrabberBase ,0 );
				m_pSampleGrabberBase->QueryInterface(IID_ISampleGrabber,(void**)&m_pSampleGrabber);
				
				//SET RGB24 BEFORE CONNECTING SampleGrabber
				CMediaType GrabType;
				GrabType.SetType( &MEDIATYPE_Video );
				GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );
				hr = m_pSampleGrabber->SetMediaType( &GrabType );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pSampleGrabber->SetBufferSamples(FALSE);
				if(FAILED(hr)) 
					return ShowError(hr);	
				
				//set the pool receiving the samples
				m_CB.SetBucketsPool( pBucketsPool );
				
				hr = m_pSampleGrabber->SetCallback( &m_CB, 1 );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pFilterGraph->Connect( pPinDecoderOut , pGrabPinIn );
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	//NULL RENDERER
	if(!m_pNullRenderer) {
		hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pNullRenderer);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pNullRenderer, L"Null Renderer");
			if(SUCCEEDED(hr)) {
				pRendererPin = GetInPin(  m_pNullRenderer, 0);
				
				hr = m_pFilterGraph->Connect( pGrabPinOut, pRendererPin);
				if(FAILED(hr))	
					return ShowError(hr);
				else {
					AM_MEDIA_TYPE mt;
					if ( pRendererPin->ConnectionMediaType(&mt) == S_OK ) {		
						SetVideoFormat( &mt );			
					}
					return true;
				}
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	return false;
}


bool
moDsGraph::BuildLiveDVGraph( moBucketsPool *pBucketsPool, MOint idevice) {
	
	HRESULT hr;

	IPin* pPinSource;
	IPin* pPinDecoderIn;
	IPin* pPinDecoderOut;
    IPin* pGrabPinIn;
	IPin* pGrabPinOut;
	IPin* pRendererPin;

	//SOURCE
	if( SetCaptureDevice( "IEEE1394", idevice ) ) {
		hr = m_pFilterGraph->AddFilter(m_pCaptureFilter, L"DV Cam");
		if(SUCCEEDED(hr)) {
			pPinSource = GetOutPin( m_pCaptureFilter ,0 );
		} else return ShowError(hr);
	} else return false;


	//DV DECODER
	if(!m_pDVDecoderFilter) {
		hr = CoCreateInstance(CLSID_DVVideoCodec, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pDVDecoderFilter);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pDVDecoderFilter, L"DV Decoder");
			if(SUCCEEDED(hr)) {
				//mostrar la pagina de configuracion del fitro
				//sacar pines				
				pPinDecoderIn = GetInPin( m_pDVDecoderFilter ,0 );
				pPinDecoderOut = GetOutPin( m_pDVDecoderFilter ,0 );

				hr = m_pFilterGraph->Connect( pPinSource , pPinDecoderIn );

				if(FAILED(hr)) 
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);


	//FRAME GRABBER
	if(!m_pSampleGrabberBase) {
		hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pSampleGrabberBase);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pSampleGrabberBase, L"Sample Grabber");
			if(SUCCEEDED(hr)) {

				pGrabPinIn = GetInPin( m_pSampleGrabberBase ,0 );
				pGrabPinOut = GetOutPin( m_pSampleGrabberBase ,0 );
				m_pSampleGrabberBase->QueryInterface(IID_ISampleGrabber,(void**)&m_pSampleGrabber);
				
				//SET RGB24 BEFORE CONNECTING SampleGrabber
				CMediaType GrabType;
				GrabType.SetType( &MEDIATYPE_Video );
				GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );
				hr = m_pSampleGrabber->SetMediaType( &GrabType );
				if(FAILED(hr)) ShowError(hr);
				hr = m_pSampleGrabber->SetBufferSamples(FALSE);
				if(FAILED(hr)) ShowError(hr);	
				
				//set the pool receiving the samples
				m_CB.SetBucketsPool( pBucketsPool );
				
				hr = m_pSampleGrabber->SetCallback( &m_CB, 1 );
				if(FAILED(hr)) ShowError(hr);
				hr = m_pFilterGraph->Connect( pPinDecoderOut , pGrabPinIn );
				if(FAILED(hr)) 
					return ShowError(hr);			
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	//NULL RENDERER
	if(!m_pNullRenderer) {
		hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pNullRenderer);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pNullRenderer, L"Null Renderer");
			if(SUCCEEDED(hr)) {
				pRendererPin = GetInPin(  m_pNullRenderer, 0);
				
				hr = m_pFilterGraph->Connect( pGrabPinOut, pRendererPin);
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	AM_MEDIA_TYPE mt;
	if ( pRendererPin->ConnectionMediaType(&mt) == S_OK ) {		
			SetVideoFormat( &mt );			
	}

	return true;
}



bool
moDsGraph::BuildLiveWebcamGraph( moBucketsPool *pBucketsPool, MOint idevice) {
	
	HRESULT hr;

	IPin* pPinSource;
	IPin* pPinDecoderIn;
	IPin* pPinDecoderOut;
    IPin* pGrabPinIn;
	IPin* pGrabPinOut;
	IPin* pRendererPin;

	//SOURCE	
	if( SetCaptureDevice("WEBCAM", idevice) ) {
		hr = m_pFilterGraph->AddFilter(m_pCaptureFilter, L"Web Cam");
		if(SUCCEEDED(hr)) {
			pPinSource = GetOutPin( m_pCaptureFilter ,0 );
		} else return ShowError(hr);
	} else return false;


	//AVI DECOMPRESSOR
	if(!m_pAVIDecompressorFilter) {
		hr = CoCreateInstance(CLSID_AVIDec, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pAVIDecompressorFilter);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pAVIDecompressorFilter, L"AVI Decompressor");
			if(SUCCEEDED(hr)) {
				//mostrar la pagina de configuracion del fitro
				//sacar pines				
				pPinDecoderIn = GetInPin( m_pAVIDecompressorFilter ,0 );
				pPinDecoderOut = GetOutPin( m_pAVIDecompressorFilter ,0 );

				hr = m_pFilterGraph->Connect( pPinSource , pPinDecoderIn );

				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);


	//FRAME GRABBER
	if(!m_pSampleGrabberBase) {
		hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pSampleGrabberBase);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pSampleGrabberBase, L"Sample Grabber");
			if(SUCCEEDED(hr)) {

				pGrabPinIn = GetInPin( m_pSampleGrabberBase ,0 );
				pGrabPinOut = GetOutPin( m_pSampleGrabberBase ,0 );
				m_pSampleGrabberBase->QueryInterface(IID_ISampleGrabber,(void**)&m_pSampleGrabber);
				
				//SET RGB24 BEFORE CONNECTING SampleGrabber
				CMediaType GrabType;
				GrabType.SetType( &MEDIATYPE_Video );
				GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );
				hr = m_pSampleGrabber->SetMediaType( &GrabType );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pSampleGrabber->SetBufferSamples(FALSE);
				if(FAILED(hr)) 
					return ShowError(hr);	
				
				//set the pool receiving the samples
				m_CB.SetBucketsPool( pBucketsPool );
				
				hr = m_pSampleGrabber->SetCallback( &m_CB, 1 );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pFilterGraph->Connect( pPinDecoderOut , pGrabPinIn );
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	//NULL RENDERER
	if(!m_pNullRenderer) {
		hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pNullRenderer);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pNullRenderer, L"Null Renderer");
			if(SUCCEEDED(hr)) {
				pRendererPin = GetInPin(  m_pNullRenderer, 0);
				
				hr = m_pFilterGraph->Connect( pGrabPinOut, pRendererPin);
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);


	return true;
}

bool moDsGraph::BuildLiveQTVideoGraph( moText filename , moBucketsPool *pBucketsPool ) {

	HRESULT hr;

	IPin* pPinSource;
	IPin* pPinDecoderIn;
	IPin* pPinDecoderOut;
    IPin* pGrabPinIn;
	IPin* pGrabPinOut;
	IPin* pRendererPin;

	bool bQTAlternative;
	moText QTFilter = "";

	//QUICKTIME....CYBERLINK QT ALTERNATIVE
	if( !m_pQuicktimeFilter ) {
		//QT ALTERNATIVE 1.3 QT 6
		QTFilter = "CyberLink QuickTime Source Filter";
		bQTAlternative = CreateFilterByName( QTFilter, &m_pQuicktimeFilter );
		
		//QT ALTERNATIVE 1.7 QT 7
		/*
		if (!bQTAlternative) {
			QTFilter = "Nero Digital Parser";
			bQTAlternative = CreateFilterByName( QTFilter, &m_pQuicktimeFilter );
		}
		*/
		
		if ( bQTAlternative ) {
			hr = m_pQuicktimeFilter->QueryInterface(IID_IFileSourceFilter,(void**)&m_pFileSourceFilter);
			if(SUCCEEDED(hr)) {	
				WCHAR wFileName[MAX_PATH];
				MultiByteToWideChar(CP_ACP, 0, filename, -1, wFileName, MAX_PATH);
				//(LPCOLESTR)filename
				hr = m_pFileSourceFilter->Load( wFileName, NULL );
				if(SUCCEEDED(hr)) {
					hr = m_pFilterGraph->AddFilter(m_pQuicktimeFilter, L"File Source");
					if(SUCCEEDED(hr)) {
						pPinSource = GetOutPin( m_pQuicktimeFilter ,0 );
					} else return ShowError(hr);
				} else return ShowError(hr);			
			}
		} else return false;
	}

	//CHECK MEDIA TYPE:for debugging
	//CheckMediaType( pPinSource );

	if (QTFilter == "CyberLink QuickTime Source Filter") {
		//COLOR SPACE CONVERTER
		if(!m_pColorSpaceConverter) {
			hr = CoCreateInstance( CLSID_Colour, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter,(void**)&m_pColorSpaceConverter);
			if( SUCCEEDED(hr) ) {
				hr = m_pFilterGraph->AddFilter( m_pColorSpaceConverter, L"Color Space Converter");
				if( SUCCEEDED(hr) ) {
					//mostrar la pagina de configuracion del fitro
					//sacar pines				
					pPinDecoderIn = GetInPin( m_pColorSpaceConverter ,0 );
					pPinDecoderOut = GetOutPin( m_pColorSpaceConverter ,0 );

					hr = m_pFilterGraph->Connect( pPinSource , pPinDecoderIn );

					if( FAILED(hr) ) 
						return ShowError(hr);

				} else return ShowError(hr);			
			} else return ShowError(hr);
		} else return ShowError(hr);
	} else {		
		if (!m_pQuicktimeDecoder) {
			if ( CreateFilterByName( moText("Nero QuickTime(tm) Video Decoder"), &m_pQuicktimeDecoder ) ) {
				hr = m_pFilterGraph->AddFilter( m_pQuicktimeDecoder, L"Nero QuickTime(tm) Video Decoder");
				if( SUCCEEDED(hr) ) {
					//mostrar la pagina de configuracion del fitro
					//sacar pines				
					pPinDecoderIn = GetInPin( m_pQuicktimeDecoder ,0 );
					pPinDecoderOut = GetOutPin( m_pQuicktimeDecoder ,0 );

					hr = m_pFilterGraph->Connect( pPinSource , pPinDecoderIn );

					if( FAILED(hr) ) 
						return ShowError(hr);

				} else return ShowError(hr);			
			} else return false;		
		}
	
	}

	//FRAME GRABBER
	if(!m_pSampleGrabberBase) {
		hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pSampleGrabberBase);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pSampleGrabberBase, L"Sample Grabber");
			if(SUCCEEDED(hr)) {

				pGrabPinIn = GetInPin( m_pSampleGrabberBase ,0 );
				pGrabPinOut = GetOutPin( m_pSampleGrabberBase ,0 );
				m_pSampleGrabberBase->QueryInterface(IID_ISampleGrabber,(void**)&m_pSampleGrabber);
				
				//SET RGB24 BEFORE CONNECTING SampleGrabber
				CMediaType GrabType;
				GrabType.SetType( &MEDIATYPE_Video );
				GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );
				hr = m_pSampleGrabber->SetMediaType( &GrabType );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pSampleGrabber->SetBufferSamples(FALSE);
				if(FAILED(hr)) 
					return ShowError(hr);	
				
				//set the pool receiving the samples
				m_CB.SetBucketsPool( pBucketsPool );
				
				hr = m_pSampleGrabber->SetCallback( &m_CB, 1 );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pFilterGraph->Connect( pPinDecoderOut , pGrabPinIn );
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	//NULL RENDERER
	if(!m_pNullRenderer) {
		hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pNullRenderer);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pNullRenderer, L"Null Renderer");
			if(SUCCEEDED(hr)) {
				pRendererPin = GetInPin(  m_pNullRenderer, 0);				
				hr = m_pFilterGraph->Connect( pGrabPinOut, pRendererPin);
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	AM_MEDIA_TYPE mt;
	if ( pRendererPin->ConnectionMediaType(&mt) == S_OK ) {		
			SetVideoFormat( &mt );			
	}

	return true;


}

bool moDsGraph::BuildLiveVideoGraph( moText filename , moBucketsPool *pBucketsPool ) {

	HRESULT hr;

	IPin* pPinSource;
	IPin* pPinSplitterIn;
	IPin* pPinSplitterOut;
	IPin* pPinDecoderIn;
	IPin* pPinDecoderOut;
    IPin* pGrabPinIn;
	IPin* pGrabPinOut;
	IPin* pRendererPin;

	//media type enumaration
	IEnumMediaTypes* ppEnum;
	AM_MEDIA_TYPE* pMediaTypes;

	//AVI OR MPEG 
	if( !m_pSourceFilter ) {
		hr = CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pSourceFilter );
		if(SUCCEEDED(hr)) {
			hr = m_pSourceFilter->QueryInterface(IID_IFileSourceFilter,(void**)&m_pFileSourceFilter);
			if(SUCCEEDED(hr)) {								
				WCHAR wFileName[MAX_PATH];
				MultiByteToWideChar(CP_ACP, 0, filename, -1, wFileName, MAX_PATH);
				//(LPCOLESTR)filename
				hr = m_pFileSourceFilter->Load( wFileName, NULL );
				if(SUCCEEDED(hr)) {
					hr = m_pFilterGraph->AddFilter(m_pSourceFilter, L"File Source");
					if(SUCCEEDED(hr)) {
						pPinSource = GetOutPin( m_pSourceFilter ,0 );
					} else return ShowError(hr);
				} else return ShowError(hr);
			} else return ShowError(hr);
		} else return ShowError(hr);		
	} else return false;


	//CHEQUEAMOS EL FORMATO DEL PIN
	hr = pPinSource->EnumMediaTypes( &ppEnum);
	if(SUCCEEDED(hr)) {
		while( ppEnum->Next( 1, &pMediaTypes, NULL) == S_OK ) {
			if ( pMediaTypes->majortype == MEDIATYPE_Stream ) { //ALL STREAMS
				
				bool passed = false;

				if ( pMediaTypes->subtype == MEDIASUBTYPE_Avi ) {
					//put the AviSplitter
					if(!m_pSplitter) {
						hr = CoCreateInstance( CLSID_AviSplitter, NULL, CLSCTX_INPROC_SERVER,
						IID_IBaseFilter,(void**)&m_pSplitter);
						if (SUCCEEDED(hr)) passed = true;
					} else return ShowError(hr);							
				} else if ( pMediaTypes->subtype == MEDIASUBTYPE_MPEG1Video ||
							pMediaTypes->subtype == MEDIASUBTYPE_MPEG1System ||
							pMediaTypes->subtype == MEDIASUBTYPE_MPEG1VideoCD) {
					if(!m_pSplitter) {
						hr = CoCreateInstance( CLSID_MPEG1Splitter, NULL, CLSCTX_INPROC_SERVER,
						IID_IBaseFilter,(void**)&m_pSplitter);
						if (SUCCEEDED(hr)) passed = true;
					} else return ShowError(hr);
				} else if ( pMediaTypes->subtype == MEDIASUBTYPE_MPEG2_PROGRAM ) {
					return false;
				}

				if (passed) {
					if (m_pSplitter) {
						if( SUCCEEDED(hr) ) {
							hr = m_pFilterGraph->AddFilter( m_pSplitter, L"Splitter");
							if( SUCCEEDED(hr) ) {									
								//sacar pines				
								pPinSplitterIn = GetInPin( m_pSplitter ,0 );								

								hr = m_pFilterGraph->Connect( pPinSource , pPinSplitterIn );

								if( FAILED(hr) ) 
									return ShowError(hr);
								else
									pPinSplitterOut = GetOutPin( m_pSplitter ,0 );								

							} else return ShowError(hr);			
						} else return ShowError(hr);
					}
					break;
				}
			}
		}
	}
	ppEnum->Release();

	bool isdv = false;
	if (pPinSplitterOut) {
		hr = pPinSplitterOut->EnumMediaTypes( &ppEnum);
		if(SUCCEEDED(hr)) {
			while( ppEnum->Next( 1, &pMediaTypes, NULL) == S_OK ) {
				if (pMediaTypes->majortype == MEDIATYPE_Video) {
					if ( pMediaTypes->subtype == MEDIASUBTYPE_dvsd ||
						 pMediaTypes->subtype == MEDIASUBTYPE_dvsl ||
						 pMediaTypes->subtype == MEDIASUBTYPE_dvhd ) {
						isdv = true;
						SetVideoFormat( pMediaTypes );
						break;
					} else if ( pMediaTypes->formattype == FORMAT_MPEGVideo ) {
						SetVideoFormat( pMediaTypes );
						break;
					} else if ( pMediaTypes->formattype == FORMAT_MPEG2Video ) {
						SetVideoFormat( pMediaTypes );
						break;
					} else if ( pMediaTypes->formattype == FORMAT_VideoInfo ||
								pMediaTypes->formattype == FORMAT_VideoInfo2 ) {
						SetVideoFormat( pMediaTypes );
						break;
					}				
				}			
			}
		}
		ppEnum->Release();
	} else return false;

	//ELEGIMOS EL CODEC...
	//aqui debemos agregar segun  el nivel de calidad deseado
	//y el decoder de DV elegido o bien el AVI decompressor
	bool bHighQuality = true;

	if ( isdv ) {
		//DV DECODER
		if(!m_pDVDecoderFilter) {
			if (bHighQuality) {
				hr = CoCreateInstance(CLSID_AVIDec, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter,(void**)&m_pDVDecoderFilter);
			} else {
				hr = CoCreateInstance( CLSID_DVVideoCodec, NULL, CLSCTX_INPROC_SERVER,
				IID_IBaseFilter,(void**)&m_pDVDecoderFilter);
			}
			if( SUCCEEDED(hr) ) {
				hr = m_pFilterGraph->AddFilter( m_pDVDecoderFilter, L"DV Decoder");
				if( SUCCEEDED(hr) ) {
					//mostrar la pagina de configuracion del fitro
					//sacar pines				
					pPinDecoderIn = GetInPin( m_pDVDecoderFilter ,0 );
					pPinDecoderOut = GetOutPin( m_pDVDecoderFilter ,0 );

					hr = m_pFilterGraph->Connect( pPinSplitterOut , pPinDecoderIn );

					if( FAILED(hr) ) 
						return ShowError(hr);

				} else return ShowError(hr);			
			} else return ShowError(hr);
		} else return ShowError(hr);		

	} else {
		//ALL OTHERS TRY TO OPEN WITH FFDSHOW		
		//if ( CreateFilterByName(moText("ffdshow MPEG-4 Video Decoder"), &m_pFfdshowFilter) ) {
		hr = CoCreateInstance(CLSID_AVIDec, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter,(void**)&m_pFfdshowFilter);
		if (SUCCEEDED(hr)) {
			
			hr = m_pFilterGraph->AddFilter( m_pFfdshowFilter, L"ffdshow MPEG-4 Video Decoder");
			
			if( SUCCEEDED(hr) ) {
				//mostrar la pagina de configuracion del fitro
				//sacar pines				
				pPinDecoderIn = GetInPin( m_pFfdshowFilter ,0 );
				pPinDecoderOut = GetOutPin( m_pFfdshowFilter ,0 );

				hr = m_pFilterGraph->Connect( pPinSplitterOut , pPinDecoderIn );

				if( FAILED(hr) ) {

					hr = m_pFilterGraph->RemoveFilter( m_pFfdshowFilter );
					
					m_pFfdshowFilter->Release();
					m_pFfdshowFilter = NULL;

					pPinDecoderIn->Release();
					pPinDecoderIn = NULL;

					pPinDecoderOut->Release();
					pPinDecoderOut = NULL;					

					if (CreateFilterByName(moText("ffdshow MPEG-4 Video Decoder"), &m_pFfdshowFilter)) {
						hr = m_pFilterGraph->AddFilter( m_pFfdshowFilter, L"ffdshow MPEG-4 Video Decoder");
					} else {
						printf("ffdshow Filter not found\n");
						_flushall();
						return false;
					}	
					if( FAILED(hr) ) 
						return ShowError(hr);
				}

			} else return ShowError(hr);	

		} else {
			printf("Filter not found\n");
			return false;
		}		
	}

	//FRAME GRABBER
	if(!m_pSampleGrabberBase) {
		hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pSampleGrabberBase);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pSampleGrabberBase, L"Sample Grabber");
			if(SUCCEEDED(hr)) {

				pGrabPinIn = GetInPin( m_pSampleGrabberBase ,0 );
				pGrabPinOut = GetOutPin( m_pSampleGrabberBase ,0 );
				m_pSampleGrabberBase->QueryInterface(IID_ISampleGrabber,(void**)&m_pSampleGrabber);
				
				//SET RGB24 BEFORE CONNECTING SampleGrabber
				CMediaType GrabType;
				GrabType.SetType( &MEDIATYPE_Video );
				GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );
				hr = m_pSampleGrabber->SetMediaType( &GrabType );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pSampleGrabber->SetBufferSamples(FALSE);
				if(FAILED(hr)) 
					return ShowError(hr);	
				
				//set the pool receiving the samples
				m_CB.SetBucketsPool( pBucketsPool );
				
				hr = m_pSampleGrabber->SetCallback( &m_CB, 1 );
				if(FAILED(hr)) 
					return ShowError(hr);
				hr = m_pFilterGraph->Connect( pPinDecoderOut , pGrabPinIn );
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	//NULL RENDERER
	if(!m_pNullRenderer) {
		hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pNullRenderer);
		if(SUCCEEDED(hr)) {
			hr = m_pFilterGraph->AddFilter(m_pNullRenderer, L"Null Renderer");
			if(SUCCEEDED(hr)) {
				pRendererPin = GetInPin(  m_pNullRenderer, 0);				
				hr = m_pFilterGraph->Connect( pGrabPinOut, pRendererPin);
				if(FAILED(hr))	
					return ShowError(hr);
			} else return ShowError(hr);			
		} else return ShowError(hr);
	} else return ShowError(hr);

	AM_MEDIA_TYPE mt;
	if ( pRendererPin->ConnectionMediaType(&mt) == S_OK ) {		
			SetVideoFormat( &mt );			
	}

	return true;

}


//CONTROL METHODS
void
moDsGraph::Play() {
	HRESULT hr;
	
	if(m_pFilterGraph) {
		if(m_pMediaControl) {
			hr = m_pMediaControl->Run();
			if(FAILED(hr)) 
				ShowError(hr);
		}
	}
}

void
moDsGraph::Stop() {
	HRESULT hr;
	if(m_pFilterGraph) {
		if(m_pMediaControl) {
			hr = m_pMediaControl->Stop();
			if(FAILED(hr)) 
				ShowError(hr);
		}
	}
}

void
moDsGraph::Pause() {
	HRESULT hr;
	if(m_pFilterGraph) {
		if(m_pMediaControl) {
			hr = m_pMediaControl->Pause();
			if(FAILED(hr)) 
				ShowError(hr);
		}
	}
}

void
moDsGraph::Seek( MOuint frame ) {
	HRESULT hr;
	if(m_pFilterGraph) {
		if(m_pMediaControl) {
			if(m_pMediaSeeking) {
				hr = m_pMediaSeeking->IsFormatSupported(&TIME_FORMAT_FRAME);
				if( hr == S_OK ) {
					hr = m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_FRAME);
					if(SUCCEEDED(hr))
					{
						// Seek to frame number 20.
						LONGLONG rtNow = frame;
						hr = m_pMediaSeeking->SetPositions(
							&rtNow, AM_SEEKING_AbsolutePositioning, 
							NULL, AM_SEEKING_NoPositioning);
					}
					if(FAILED(hr)) 
						ShowError(hr);
				} else if ( hr == S_FALSE ) {
					if (frame>10) {
						//skip
						hr = S_OK;
						
					}
					hr = m_pMediaSeeking->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME);
					if ( hr == S_OK ) {
						hr = m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);						
						if(SUCCEEDED(hr))
						{
							// Seek to frame number 20.
							LONGLONG rtNow = frame;
							
							rtNow = rtNow * m_VideoFormat.m_TimePerFrame;
							/*
							if ( m_VideoFormat.m_VideoMode == PAL ) {
								rtNow = rtNow * 400000; //en cuadros
							} else if ( m_VideoMode == moDsGraph::NTSC ) {
								rtNow = rtNow * 333667; //en cuadros
							}
							*/
							hr = m_pMediaSeeking->SetPositions(
								&rtNow, AM_SEEKING_AbsolutePositioning, 
								NULL, AM_SEEKING_NoPositioning);
						}
						if(FAILED(hr)) 
							ShowError(hr);
					} else ShowError(hr);
				} else ShowError(hr);
			}
		}
	}
}

MOulong
moDsGraph::GetFramesLength() {
	HRESULT hr;
	LONGLONG dur = 0;
	if(m_pFilterGraph) {
			if(m_pMediaControl) {
				hr = m_pMediaSeeking->IsFormatSupported(&TIME_FORMAT_FRAME);
				if( hr == S_OK ) {
					hr = m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_FRAME);
					if(SUCCEEDED(hr))
					{
						if(m_pMediaSeeking) {
							hr = m_pMediaSeeking->GetDuration(&dur);
							if(SUCCEEDED(hr))
								return dur;
							else ShowError(hr);
						}
					} else ShowError(hr);
				} else if (hr==S_FALSE) {
					hr = m_pMediaSeeking->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME);
					if( hr == S_OK ) {
						hr = m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
						if(SUCCEEDED(hr))
						{
							if(m_pMediaSeeking) {
								hr = m_pMediaSeeking->GetDuration(&dur);
								if(SUCCEEDED(hr)) {
									dur = dur / m_VideoFormat.m_TimePerFrame; //en cuadros
									/*
									if ( m_VideoMode == moDsGraph::PAL ) {
										dur = dur / 400000; //en cuadros
									} else if ( m_VideoMode == moDsGraph::NTSC ) {
										dur = dur / 333667; //en cuadros
									}
									*/
									return dur;
								} else ShowError(hr);
							}
						} else ShowError(hr);											
					} else ShowError(hr);	
				} else ShowError(hr);
			}
	}
	return dur;
}

bool
moDsGraph::IsRunning() {
	HRESULT hr;
	if(m_pFilterGraph) {
		if(m_pMediaControl) {
			OAFilterState filterstate;
			hr = m_pMediaControl->GetState( 12, &filterstate);
			if(SUCCEEDED(hr)) {
				hr = m_pMediaControl->Pause();
				if(SUCCEEDED(hr)) {
					hr = m_pMediaControl->Run();
				} else return false;
				if(SUCCEEDED(hr)) {
					return(filterstate == State_Running);
				} return false;
			} else ShowError(hr);
		}		
	}
	return false;
}

MObyte *
moDsGraph::GetFrameBuffer( MOlong *size ) {
	/*
	HRESULT hr;
	MObyte *pbuf = NULL;


	if(m_pSampleGrabber) {
		hr = m_pSampleGrabber->GetCurrentBuffer( size, NULL);
		if(SUCCEEDED(hr)) {
			if((*size)>0) {
				pbuf = new MObyte [(*size)];
				hr = m_pSampleGrabber->GetCurrentBuffer( size, pbuf);
				if(FAILED(hr)) ShowError(hr);
				return pbuf;
			}		
		} else ShowError(hr);
	}
	*/
	return NULL;
}

bool 
moDsGraph::ShowError( HRESULT hr ) {
    if(FAILED(hr))
    {
        TCHAR szErr[MAX_ERROR_TEXT_LEN];
        DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);
        if(res == 0)
        {
            wsprintf(szErr, "Unknown Error: 0x%2x", hr);
        }
        MessageBox(0, szErr, TEXT("Error!"), MB_OK | MB_ICONERROR);
		return false;
    } else return true;
}

void
moDsGraph::SetVideoFormat( AM_MEDIA_TYPE * pmt ) {

	if (pmt->cbFormat >= sizeof(VIDEOINFOHEADER)) {

		if ( pmt->formattype == FORMAT_VideoInfo ) {

			VIDEOINFOHEADER *pVih = reinterpret_cast<VIDEOINFOHEADER*>(pmt->pbFormat);
			
			m_VideoFormat.m_Width = pVih->bmiHeader.biWidth;
			m_VideoFormat.m_Height = pVih->bmiHeader.biHeight;
			m_VideoFormat.m_BitCount = pVih->bmiHeader.biBitCount;
			m_VideoFormat.m_BitRate = pVih->dwBitRate;
			m_VideoFormat.m_TimePerFrame = pVih->AvgTimePerFrame;
			m_VideoFormat.m_BufferSize = pmt->lSampleSize;
			m_VideoFormat.SetVideoMode();

		} else if (pmt->formattype == FORMAT_MPEGVideo) {
			MPEG1VIDEOINFO *pMh = reinterpret_cast<MPEG1VIDEOINFO*>(pmt->pbFormat);
			VIDEOINFOHEADER *pVih = &(pMh->hdr);
		
			m_VideoFormat.m_Width = pVih->bmiHeader.biWidth;
			m_VideoFormat.m_Height = pVih->bmiHeader.biHeight;
			m_VideoFormat.m_BitCount = pVih->bmiHeader.biBitCount;
			m_VideoFormat.m_BitRate = pVih->dwBitRate;
			m_VideoFormat.m_TimePerFrame = pVih->AvgTimePerFrame;
			m_VideoFormat.m_BufferSize = pVih->bmiHeader.biWidth * pVih->bmiHeader.biHeight * 3;
			m_VideoFormat.SetVideoMode();

		} else if (pmt->formattype == FORMAT_MPEG2Video) {

			MPEG2VIDEOINFO *pMh = reinterpret_cast<MPEG2VIDEOINFO*>(pmt->pbFormat);
			VIDEOINFOHEADER2 *pVih = &(pMh->hdr);
		
			m_VideoFormat.m_Width = pVih->bmiHeader.biWidth;
			m_VideoFormat.m_Height = pVih->bmiHeader.biHeight;
			m_VideoFormat.m_BitCount = pVih->bmiHeader.biBitCount;
			m_VideoFormat.m_BitRate = pVih->dwBitRate;
			m_VideoFormat.m_TimePerFrame = pVih->AvgTimePerFrame;
			m_VideoFormat.m_BufferSize = pVih->bmiHeader.biWidth * pVih->bmiHeader.biHeight * 3;
			m_VideoFormat.SetVideoMode();

		}
			
	}

}

IPin * moDsGraph::GetInPin( IBaseFilter * pFilter, int nPin )
{
    IPin* pComPin=NULL;
    GetPin(pFilter, PINDIR_INPUT, nPin, &pComPin);
    return pComPin;
}


IPin * moDsGraph::GetOutPin( IBaseFilter * pFilter, int nPin )
{
    IPin* pComPin=NULL;
    GetPin(pFilter, PINDIR_OUTPUT, nPin, &pComPin);
    return pComPin;
}

HRESULT
moDsGraph::GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    IEnumPins* pEnum;
    *ppPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr)) 
        return hr;

    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;

    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir =(PIN_DIRECTION)3;

        pPin->QueryDirection(&pindir);
        if(pindir == dirrequired)
        {
            if(iNum == 0)
            {
                *ppPin = pPin;  // Return the pin's interface
                hr = S_OK;      // Found requested pin, so clear error
                break;
            }
            iNum--;
        } 

        pPin->Release();
    } 

    return hr;
}


void
moDsGraph::CheckMediaType( IPin* p_Pin ) {

	HRESULT hr;
	IEnumMediaTypes* ppEnum;
	AM_MEDIA_TYPE* pMediaTypes;


	hr = p_Pin->EnumMediaTypes( &ppEnum);

	if(SUCCEEDED(hr)) {

		while( ppEnum->Next( 1, &pMediaTypes, NULL) == S_OK ) {
			if ( pMediaTypes->majortype == MEDIATYPE_Video ) { //VIDEO
				if ( pMediaTypes->formattype == FORMAT_VideoInfo ) {

					VIDEOINFOHEADER *pVih = reinterpret_cast<VIDEOINFOHEADER*>(pMediaTypes->pbFormat);
					m_VideoFormat.m_Width = pVih->bmiHeader.biWidth;
					m_VideoFormat.m_Height = pVih->bmiHeader.biHeight;
					m_VideoFormat.m_BitCount = pVih->bmiHeader.biBitCount;
					m_VideoFormat.m_BitRate = pVih->dwBitRate;
					m_VideoFormat.m_TimePerFrame = pVih->AvgTimePerFrame;
					m_VideoFormat.m_BufferSize = pMediaTypes->lSampleSize;
					m_VideoFormat.SetVideoMode();
				}
			}
		}
	}
	ppEnum->Release();	
}

#endif



