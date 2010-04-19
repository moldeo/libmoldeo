/*******************************************************************************

                              moVideoManager.cpp

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
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

#include "moVideoManager.h"


#include "moArray.h"
moDefineDynamicArray(moCircularVideoBuffers)
moDefineDynamicArray(moVideoFrames)
moDefineDynamicArray(moVideoBuffers)
moDefineDynamicArray(moLiveSystemPtrs)
moDefineDynamicArray(moVideoBufferPaths)

//===========================================
//
//				moVideoFrame
//
//===========================================

moVideoFrame::moVideoFrame() {
	m_FrameSize = 0;
	hmem = NULL;
	options = 0;
	fif = FIF_UNKNOWN;
}

moVideoFrame::~moVideoFrame() {
	Finish();
}

MOboolean
moVideoFrame::Init( ) {
	m_bInitialized =  (m_FrameSize>0);
	return m_bInitialized;
}

//FreeImage_SaveToMemory(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, FIMEMORY *stream, int flags FI_DEFAULT(0));
MOboolean
moVideoFrame::Init( moText bufferformat, FIBITMAP* pImageResult ) {
	if (!hmem) {
		hmem = FreeImage_OpenMemory();
	}
	if (hmem) {

		if ( bufferformat == moText("JPG")) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYNORMAL;
		} else if ( bufferformat == moText("JPGSUPERB") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYSUPERB;
		} else if ( bufferformat == moText("JPGBAD") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYBAD;
		} else if ( bufferformat == moText("JPGAVERAGE") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYAVERAGE;
		} else if ( bufferformat == moText("JPGGOOD") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYGOOD;
		} else if ( bufferformat == moText("TGA") ) {
			fif = FIF_TARGA;
			options = 0;
		}
		FreeImage_SaveToMemory( fif, pImageResult, hmem, options );
		m_FrameSize = FreeImage_TellMemory(hmem);
	}
	return Init();
}

MOboolean
moVideoFrame::Finish() {

	if (hmem) {
		FreeImage_CloseMemory( hmem );
		hmem = NULL;
	}

	m_FrameSize = 0;
	hmem = NULL;
	fif = FIF_UNKNOWN;

	return true;
}


//===========================================
//
//				moVideoBuffer
//
//===========================================

moVideoBuffer::moVideoBuffer() : moTextureAnimated() {
	m_pResourceManager = NULL;
	m_type = MO_TYPE_VIDEOBUFFER;

	m_XSource = 0;
	m_YSource = 0;
	m_SourceWidth = 0;
	m_SourceHeight = 0;
}

moVideoBuffer::~moVideoBuffer() {
	Finish();
}

MOboolean  moVideoBuffer::Init() {

	if (!m_pResourceManager)
		return false;

	return true;
}

MOboolean
moVideoBuffer::Init( moText bufferpath, moText bufferformat, moResourceManager* p_pResourceManager, MOuint width, MOuint height, MOuint xsource, MOuint ysource, MOuint sourcewidth, MOuint sourceheight, MOint interpolation, MOint interpolationjump, MOint interpolationtime  ) {
	m_pResourceManager = p_pResourceManager;

	m_BufferPath = bufferpath;
	m_BufferFormat = bufferformat;
	m_XSource = xsource;
	m_YSource = ysource;
	m_SourceWidth = sourcewidth;
	m_SourceHeight = sourceheight;

	BuildEmpty( width, height);

	SetInterpolation( interpolationjump, interpolationtime );
	ActivateInterpolation( interpolation );

	/*
	moShaderManager* SM = m_pResourceManager->GetShaderMan();
	moTextureManager* TM = m_pResourceManager->GetTextureMan();

	m_pShaderCopy = SM->GetShader(SM->GetShaderIndex(moText("shaders/Copy.cfg"),true) );
	*/
	m_Frames.Init( 0, NULL);

	return Init();
}

MOboolean  moVideoBuffer::Finish() {
	for(MOuint i=0; i<m_Frames.Count(); i++) {
		moVideoFrame* pVideoFrame = m_Frames[i];
		if (pVideoFrame)
			pVideoFrame->Finish();
		delete pVideoFrame;
	}
	m_Frames.Empty();
	m_pResourceManager = NULL;
	return true;
}

MOboolean
moVideoBuffer::LoadImage( FIBITMAP* pImage , MOuint indeximage ) {

	FIBITMAP* pImageResult = NULL;
	FIBITMAP* pImageCropped = NULL;
	FIBITMAP* pImageScaled = NULL;

	if ( m_width!=FreeImage_GetWidth(pImage) || m_height!=FreeImage_GetHeight(pImage) ) {
		//CROP MODE
		pImageCropped = FreeImage_Copy( pImage, m_XSource , m_YSource , m_XSource + m_SourceWidth , m_YSource+m_SourceHeight );
		pImageResult = pImageCropped;

	} else pImageResult = pImage;

	//RESCALE
	if ( m_width != m_SourceWidth || m_height != m_SourceHeight ) {
		/*
		FILTER_BOX Box, pulse, Fourier window, 1st order (constant) B-Spline
		FILTER_BILINEAR Bilinear filter
		FILTER_BSPLINE 4th order (cubic) B-Spline
		FILTER_BICUBIC Mitchell and Netravali's two-param cubic filter
		FILTER_CATMULLROM Catmull-Rom spline, Overhauser spline
		FILTER_LANCZOS3
		*/
		pImageScaled = FreeImage_Rescale( pImageResult, m_width, m_height, FILTER_BICUBIC );
		if (pImageScaled) {
			FreeImage_Unload(pImageResult);
			pImageResult = pImageScaled;
		}
	}

	moVideoFrame* pVideoFrame = new moVideoFrame(  );
	if (pVideoFrame) {
		pVideoFrame->Init( m_BufferFormat, pImageResult );
		m_Frames.Add(pVideoFrame);
	}

	if (pImageResult!=pImage)
		FreeImage_Unload(pImageResult);

	m_nFrames = m_Frames.Count();
	m_fFramesPerSecond = 25.0;

	return (pVideoFrame!=NULL);
}
/*
MOboolean moVideoBuffer::LoadFromVideo(  moText p_moviefile ) {
	//
	return true;
}
*/
/*
*/
void moVideoBuffer::GetFrame( MOuint p_i ) {

	if ( p_i<m_Frames.Count()) {

		moVideoFrame* pVideoFrame = m_Frames[p_i];

		if (pVideoFrame) {

			FIBITMAP *m_pImage;
			//FIMEMORY	*hmem;

			MOuint p_format;

			//MOint FrameSize =
			FreeImage_TellMemory(pVideoFrame->hmem);
			FreeImage_SeekMemory( pVideoFrame->hmem, 0L, SEEK_SET);

			//hmem = FreeImage_OpenMemory( pVideoFrame->hmem,  pVideoFrame->m_FrameSize);
			//FreeImage_TellMemory(VideoFrame->hmem);
			// load an image from the memory stream
			m_pImage = FreeImage_LoadFromMemory( pVideoFrame->fif, pVideoFrame->hmem, 0);

			switch (FreeImage_GetBPP(m_pImage))
			{
				case 8: // 8 bit, indexed or grayscale
					m_param.internal_format = GL_RGB;
					p_format = GL_LUMINANCE;
					break;
				case 16: // 16 bits
					break;
				case 24: // 24 bits
					m_param.internal_format = GL_RGB;
					if (FreeImage_GetBlueMask(m_pImage) == 0x000000FF) p_format = GL_BGR;
					else p_format = GL_RGB;
					break;
				case 32: // 32 bits
					m_param.internal_format = GL_RGBA;
					if (FreeImage_GetBlueMask(m_pImage) == 0x000000FF) p_format = GL_BGRA_EXT;
					else p_format = GL_RGBA;
					break;
				default:
					break;
			}
			SetBuffer( m_width, m_height, FreeImage_GetBits(m_pImage), p_format);
			FreeImage_Unload( m_pImage );
		}
	}

}

//===========================================
//
//				moCircularVideoBuffer
//
//===========================================

moCircularVideoBuffer::moCircularVideoBuffer() {
	m_pResourceManager = NULL;
	m_type = MO_TYPE_VIDEOBUFFER;

	m_WriteIndex = 0;
	m_ReadIndex = 0;
	m_XSource = 0;
	m_YSource = 0;
	m_SourceWidth = 0;
	m_SourceHeight = 0;
}

moCircularVideoBuffer::~moCircularVideoBuffer() {
	Finish();
}

MOboolean  moCircularVideoBuffer::Init() {

	if (!m_pResourceManager)
		return false;

	return true;
}

MOboolean
moCircularVideoBuffer::Init( moText videoinput, moText bufferformat, moResourceManager* p_pResourceManager,  MOint frames, MOint width, MOint height, MOint xsource, MOint ysource, MOint sourcewidth, MOint sourceheight ) {
	m_pResourceManager = p_pResourceManager;

	m_VideoInput = videoinput;
	m_BufferFormat = bufferformat;
	m_XSource = xsource;
	m_YSource = ysource;
	m_SourceWidth = sourcewidth;
	m_SourceHeight = sourceheight;

	BuildEmpty( width, height);


	//DLL_API FIBITMAP *DLL_CALLCONV FreeImage_Allocate(int width, int height, int bpp, unsigned red_mask FI_DEFAULT(0), unsigned green_mask FI_DEFAULT(0), unsigned blue_mask FI_DEFAULT(0));

	FIBITMAP*	pBlack;
	pBlack = FreeImage_Allocate( width, height, 24 );

	for(MOint i=0; i<frames; i++) {
		moVideoFrame*	pVideoFrame = new moVideoFrame();
		if (pVideoFrame) {
			pVideoFrame->Init( bufferformat, pBlack );
			m_Frames.Add(pVideoFrame);
		}
	}

	m_nFrames = m_Frames.Count();


	return Init();
}

MOboolean  moCircularVideoBuffer::Finish() {
	for(MOuint i=0; i<m_Frames.Count(); i++) {
		moVideoFrame* pVideoFrame = m_Frames[i];
		if (pVideoFrame)
			pVideoFrame->Finish();
		delete pVideoFrame;
	}
	m_Frames.Empty();
	m_pResourceManager = NULL;
	return true;
}

MOboolean
moCircularVideoBuffer::LoadSample( moVideoSample* pvideosample ) {


	FIBITMAP* pImage = NULL;
	moBucket*	pBucket = NULL;

	if (!pvideosample) return false;

//DLL_API FIBITMAP *DLL_CALLCONV FreeImage_ConvertFromRawBits(
	//BYTE *bits, int width, int height, int pitch, unsigned bpp, unsigned red_mask, unsigned green_mask, unsigned blue_mask, BOOL topdown FI_DEFAULT(FALSE));
	pBucket = (moBucket*)pvideosample->m_pSampleBuffer;
	if (!pBucket) return false;
	pImage = FreeImage_ConvertFromRawBits( pBucket->GetBuffer(),
		pvideosample->m_VideoFormat.m_Width,
		pvideosample->m_VideoFormat.m_Height,
		pvideosample->m_VideoFormat.m_Width*3,
		24,
		0xFF0000,
		0x00FF00,
		0x0000FF);

	FIBITMAP* pImageResult = NULL;
	FIBITMAP* pImageCropped = NULL;
	FIBITMAP* pImageScaled = NULL;




	if ( m_width!=FreeImage_GetWidth(pImage) || m_height!=FreeImage_GetHeight(pImage) ) {
		//CROP MODE
		pImageCropped = FreeImage_Copy( pImage, m_XSource , m_YSource , m_XSource + m_SourceWidth , m_YSource+m_SourceHeight );
		pImageResult = pImageCropped;

	} else pImageResult = pImage;

	//RESCALE
	if ( m_width != m_SourceWidth || m_height != m_SourceHeight ) {
		/*
		FILTER_BOX Box, pulse, Fourier window, 1st order (constant) B-Spline
		FILTER_BILINEAR Bilinear filter
		FILTER_BSPLINE 4th order (cubic) B-Spline
		FILTER_BICUBIC Mitchell and Netravali's two-param cubic filter
		FILTER_CATMULLROM Catmull-Rom spline, Overhauser spline
		FILTER_LANCZOS3
		*/
		pImageScaled = FreeImage_Rescale( pImageResult, m_width, m_height, FILTER_BICUBIC );
		if (pImageScaled) {
			FreeImage_Unload(pImageResult);
			pImageResult = pImageScaled;
		}
	}

	//aqui cambia
	m_WriteIndex++;

	//Circularity
	if ( m_WriteIndex>= (MOint)m_Frames.Count() ) {
		m_WriteIndex = 0;
	}

	m_ReadIndex = m_WriteIndex;

	moVideoFrame* pVideoFrame = m_Frames[m_WriteIndex];
	if (pVideoFrame) {
		pVideoFrame->Finish();
		pVideoFrame->Init( m_BufferFormat, pImageResult );
	}

	if (pImageResult!=pImage)
		FreeImage_Unload(pImageResult);

	FreeImage_Unload(pImage);

	m_fFramesPerSecond = 25.0;

	return (pVideoFrame!=NULL);
}
/*
MOboolean moVideoBuffer::LoadFromVideo(  moText p_moviefile ) {
	//
	return true;
}
*/
/*
*/
void moCircularVideoBuffer::GetFrame( MOuint p_i ) {

	if ( p_i<m_Frames.Count()) {

		MOint ddd = ( m_ReadIndex - (int)p_i);
		if (ddd<0) ddd = m_Frames.Count() + ddd;
		p_i = ddd % m_Frames.Count();

		moVideoFrame* pVideoFrame = m_Frames[p_i];

		if (pVideoFrame) {

			FIBITMAP *m_pImage;
			//FIMEMORY	*hmem;

			MOuint p_format;

			//MOint FrameSize =
			FreeImage_TellMemory(pVideoFrame->hmem);
			FreeImage_SeekMemory( pVideoFrame->hmem, 0L, SEEK_SET);

			//hmem = FreeImage_OpenMemory( pVideoFrame->hmem,  pVideoFrame->m_FrameSize);
			//FreeImage_TellMemory(VideoFrame->hmem);
			// load an image from the memory stream
			m_pImage = FreeImage_LoadFromMemory( pVideoFrame->fif, pVideoFrame->hmem, 0);

			switch (FreeImage_GetBPP(m_pImage))
			{
				case 8: // 8 bit, indexed or grayscale
					m_param.internal_format = GL_RGB;
					p_format = GL_LUMINANCE;
					break;
				case 16: // 16 bits
					break;
				case 24: // 24 bits
					m_param.internal_format = GL_RGB;
					if (FreeImage_GetBlueMask(m_pImage) == 0x000000FF) p_format = GL_BGR;
					else p_format = GL_RGB;
					break;
				case 32: // 32 bits
					m_param.internal_format = GL_RGBA;
					if (FreeImage_GetBlueMask(m_pImage) == 0x000000FF) p_format = GL_BGRA_EXT;
					else p_format = GL_RGBA;
					break;
				default:
					break;
			}
			SetBuffer( m_width, m_height, FreeImage_GetBits(m_pImage), p_format);
			FreeImage_Unload( m_pImage );
		}
	}

}


//===========================================
//
//				moVideoBufferPath
//
//===========================================

moVideoBufferPath::moVideoBufferPath() {
	m_ImagesProcessed = 0;
	m_ActualImage = 0;
	m_pResourceManager = NULL;
	m_pDirectory = NULL;
	m_bLoadCompleted = false;

}

moVideoBufferPath::~moVideoBufferPath() {
	Finish();
}

MOboolean  moVideoBufferPath::Init() {

	m_bInitialized = true;

	return m_bInitialized;
}

MOboolean
moVideoBufferPath::Init( moResourceManager* pResources, moText videobufferpath ) {

	m_pResourceManager = pResources;

	if (!m_pResourceManager)
		return false;

	m_VideoBufferPath = videobufferpath;

	m_CompletePath = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)m_VideoBufferPath;;

	m_pDirectory = m_pResourceManager->GetFileMan()->GetDirectory( m_CompletePath );


	return Init();
}

MOboolean  moVideoBufferPath::Finish() {

	m_VideoBuffers.Empty();

	return true;
}

MOboolean
moVideoBufferPath::LoadCompleted() {
	return m_bLoadCompleted;
}

MOboolean moVideoBufferPath::UpdateImages( MOint maxfiles ) {

	//carga los frames desde los archivos
	moFile* pFile;

	MOint counter = 0;

	if (!m_pDirectory) return false;

	if (m_ActualImage>=(MOint)m_pDirectory->GetFiles().Count()) {
		m_bLoadCompleted = true;
		return true;
	}

	if (m_ActualImage==0)
		pFile = m_pDirectory->FindFirst();
	else
		pFile = m_pDirectory->Find(m_ActualImage);

	if (pFile)
	do {
		if ( pFile->GetType()==MO_FILETYPE_LOCAL && pFile->Exists()) {

			//LOAD AND UNCOMPRESS IMAGE
			FREE_IMAGE_FORMAT fif;
			fif = FreeImage_GetFileType( pFile->GetCompletePath(), 0);

			if( fif == FIF_UNKNOWN ) {
				// try to guess the file format from the file extension
				fif = FreeImage_GetFIFFromFilename(pFile->GetCompletePath());
			}

			if( (fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif) ) {
				//decodificamos el archivo
				FIBITMAP* pImage;
				pImage = FreeImage_Load( fif, pFile->GetCompletePath(), 0);

				//ONCE LOADED SAVE ON EVERY VIDEOBUFFER
				if (pImage) {

					for(MOuint b = 0; b<m_VideoBuffers.Count(); b++) {

						moVideoBuffer* pVideoBuffer = m_VideoBuffers[b];

						if (pVideoBuffer)
							pVideoBuffer->LoadImage( pImage, m_ActualImage );
					}
					FreeImage_Unload(pImage);
					pImage = NULL;
					m_ImagesProcessed++;
				}
			}
		}

		m_ActualImage++;
		counter++;
		if (counter==maxfiles && maxfiles!=(-1))
			break;

	} while ( (pFile = m_pDirectory->FindNext()) );

	return true;
}


//===========================================
//
//				moVideoManager
//
//===========================================

moVideoManager::moVideoManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_VIDEO );
	SetName("videomanager");

	m_pLiveSystems = NULL;
}

moVideoManager::~moVideoManager()
{
    Finish();
}

MOboolean moVideoManager::Init()
{
	moCaptureDevices* pPreferredDevices;
    MOuint preferreddevices;
	MOuint autoreconnect;
	MOuint nvalues;
    moText confignamecompleto;

	m_bAutoReconnect = false;

    // Loading config file.
	if (!m_pResourceManager) return false;

	confignamecompleto = m_pResourceManager->GetDataMan()->GetDataPath();
	confignamecompleto +=  (moText)GetConfigName();
    confignamecompleto +=  moText(".cfg");

	if (m_Config.LoadConfig(confignamecompleto)!=MO_CONFIG_OK ) {
		moText text = "Couldn't load videomanager config";
		MODebug2->Error(text + (moText)confignamecompleto );
		return false;
	}

  moText msg = "In moVideoManager::Init ***********************************************\n";
  msg+= moText("Initializing Live...\n");
  MODebug2->Message( msg );


  preferreddevices = m_Config.GetParamIndex("preferreddevices");//FIREWIRE, WEBCAMS, LIVEVIDEO[movie]
	autoreconnect = m_Config.GetParamIndex("autoreconnect");//FIREWIRE, WEBCAMS, LIVEVIDEO[movie]
	MOuint videoin = m_Config.GetParamIndex("videoin");

	moText videoinname;

	MODebug2->Message( "VideoManager:: Generating videoin textures" );

	for(MOuint i=0; i<m_Config.GetValuesCount(videoin); i++ ) {
		videoinname = m_Config.GetParam(videoin).GetValue(i).GetSubValue(0).Text();
		if(videoinname!=moText("")) {
			 int tid = m_pResourceManager->GetTextureMan()->AddTexture( MO_TYPE_TEXTURE, videoinname);
			 if(tid>-1) Images.Add( m_pResourceManager->GetTextureMan()->GetTexture(tid));
		 	MODebug2->Message( moText("VideoManager:: Added ") + moText(videoinname) );
		}
	}
	/**	LOAD AUTORECONNECT PARAMETER...	*/
	m_bAutoReconnect = (bool) m_Config.GetParam((MOint)autoreconnect).GetValue().GetSubValue().Int();

	/**        LOAD PREFERRED DEVICES CONFIGURATION	*/
	pPreferredDevices = new moCaptureDevices();

	nvalues = m_Config.GetValuesCount( preferreddevices );
	m_Config.SetCurrentParamIndex(preferreddevices);
	/*
MO_LIVE_LABELNAME	0
MO_LIVE_DEVICENAME	1
MO_LIVE_WIDTH		2
MO_LIVE_HEIGHT		3
MO_LIVE_BITCOUNT	4
	*/
	for( MOuint i = 0; i < nvalues; i++) {

		m_Config.SetCurrentValueIndex(preferreddevices, i);

		moVideoFormat VF;
		moCaptureDevice CD( m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_DEVICENAME).Text(), moText(""), moText("") );

		CD.SetCodeName( m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_LABELNAME).Text() );

		VF.m_Width = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_WIDTH).Int();
		VF.m_Height = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_HEIGHT).Int();
		VF.m_BitCount = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_BITCOUNT).Int();

		CD.SetVideoFormat( VF );

		pPreferredDevices->Add( CD );
		//for( int k = 1; k < m_Config.GetParam().GetSubCount(); k++);
	}



//INITIALIZING LiveSystems

	//try to connect to all
	m_pLiveSystems = new moLiveSystems();

	if ( m_pLiveSystems->LoadLiveSystems( pPreferredDevices ) ) {
		//los inicializa...
		for( MOuint i = 0; i < m_pLiveSystems->Count(); i++) {
			moLiveSystemPtr pLS = m_pLiveSystems->Get(i);
			if (pLS && pLS->GetCaptureDevice().IsPresent()) {
				if (pLS->Init()) {
				    MODebug2->Message( moText(pLS->GetCaptureDevice().GetName()) + " initialized");
                } else {
                    MODebug2->Error( moText(pLS->GetCaptureDevice().GetName()) + " not initialized");
                }
			}

		}


	}


	MOuint circularbuffer = m_Config.GetParamIndex("circularbuffer");

	moText videobufferinput;
	moText videobuffername;
	moText videobufferformat;
	MOint	xsource, ysource, width, height, sourcewidth, sourceheight, frames;
	MOint	interpolation, interpolationjump, interpolationtime;

	if (circularbuffer>0)
	for(MOuint i=0; i<m_Config.GetValuesCount(circularbuffer); i++ ) {

		videobufferinput = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARBUFFERINPUT).Text();
		videobuffername = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARBUFFERNAME).Text();
		videobufferformat = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARBUFFERFORMAT).Text();

		frames = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARBUFFERFRAMES).Int();
		width = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARWIDTH).Int();
		height = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARHEIGHT).Int();
		xsource = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARSOURCEXOFFSET).Int();
		ysource = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARSOURCEYOFFSET).Int();
		sourcewidth = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARSOURCEWIDTH).Int();
		sourceheight = m_Config.GetParam(circularbuffer).GetValue(i).GetSubValue(MO_VIDEO_CIRCULARSOURCEHEIGHT).Int();

		if(videobufferinput!=moText("")) {
			moCircularVideoBuffer* pCircularBuffer = NULL;
			int tid = m_pResourceManager->GetTextureMan()->AddTexture( MO_TYPE_CIRCULARVIDEOBUFFER, videobuffername );
			if(tid>-1) {
				pCircularBuffer =  (moCircularVideoBuffer*) m_pResourceManager->GetTextureMan()->GetTexture(tid);
			}
			if (pCircularBuffer) {
				pCircularBuffer->Init(  videobufferinput, videobufferformat, GetResourceManager(), frames, width, height, xsource, ysource, sourcewidth, sourceheight);
				m_CircularVideoBuffers.Add(pCircularBuffer);
			}
		}

	}

	MOuint videobuffer = m_Config.GetParamIndex("videobuffer");

	moText videobufferpath;
	//moText videobuffername;
	//moText videobufferformat;
	//MOint	xsource, ysource, width, height, sourcewidth, sourceheight;
	if (videobuffer>0)
	for(MOuint i=0; i<m_Config.GetValuesCount(videobuffer); i++ ) {

		videobufferpath = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(0).Text();

		moVideoBufferPath* pVideoBufferPath = new moVideoBufferPath();

		if (pVideoBufferPath) {

			pVideoBufferPath->Init( GetResourceManager(), videobufferpath );
			m_VideoBufferPaths.Add(pVideoBufferPath);

			for(MOuint j=0; j < ( m_Config.GetParam(videobuffer).GetValue(i).GetSubValueCount() - 1 ) / 11; j++) {

				videobuffername = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_BUFFERNAME).Text();
				videobufferformat = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_BUFFERFORMAT).Text();

				width = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_WIDTH).Int();
				height = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_HEIGHT).Int();
				xsource = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEXOFFSET).Int();
				ysource = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEYOFFSET).Int();
				sourcewidth = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEWIDTH).Int();
				sourceheight = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEHEIGHT).Int();

				interpolation = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEINTERPOLATION).Int();
				interpolationjump = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEINTERPOLATIONJUMP).Int();
				interpolationtime = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEINTERPOLATIONTIME).Int();

				if(videobufferpath!=moText("")) {
					moVideoBuffer* pVideoBuffer = NULL;
					int tid = m_pResourceManager->GetTextureMan()->AddTexture( MO_TYPE_VIDEOBUFFER, videobuffername );
					if(tid>-1) {
						pVideoBuffer =  (moVideoBuffer*) m_pResourceManager->GetTextureMan()->GetTexture(tid);
					}
					if (pVideoBuffer) {
						pVideoBuffer->Init(  videobufferpath, videobufferformat, GetResourceManager(), width, height, xsource, ysource, sourcewidth, sourceheight, interpolation, interpolationjump, interpolationtime);
						pVideoBufferPath->m_VideoBuffers.Add(pVideoBuffer);
					}
				}
			}
		}
	}

	delete pPreferredDevices;

	m_bLoading = true;
	m_bInitialized = true;

	return true;
}

MOswitch moVideoManager::SetStatus(MOdevcode devcode, MOswitch state)
{
    return true;
}

MOswitch moVideoManager::GetStatus(MOdevcode devcode)
{

	//devcode ahora deberia ser:
	//LIVESYSTEM01 0
	//LIVESYSTEM02 1
	//LIVESYSTEM03 2
	//LIVESYSTEM04 3
	if (m_pLiveSystems) {

		return m_pLiveSystems->GetStatus( devcode );

	}

	return 0;
    //return(-1);
}

//in this device, GetValue returns the size of the buffer corresponding of a sample of the actual devicecode
MOint moVideoManager::GetValue(MOdevcode devcode)
{
	if( m_pLiveSystems) {

		moLiveSystemPtr pLS = m_pLiveSystems->Get( devcode );

		if (pLS) {

			moVideoGraph* pVideoGraph = pLS->GetVideoGraph();

			if (pVideoGraph) {

				return pVideoGraph->GetVideoFormat().m_BufferSize;

			}

		}
	}

    return(-1);
}

MOdevcode moVideoManager::GetCode(moText strcod)
{
	//SNAPSHOTS
	if (m_pLiveSystems) {
		//recorre los livesystems, y por cada nombre....chequea el codigo
		for(MOuint i =0; i< m_pLiveSystems->Count(); i++) {
			moLiveSystemPtr pLS = m_pLiveSystems->Get(i);
			if (pLS) {
				if ( pLS->GetCodeName() == strcod ) {
					return i;
				}
			}
		}
	}

    return(-1);
}


//esto se ejecuta por cada CICLO PRINCIPAL DE moConsole(frame)
//por ahora no nos sirve de nada, porque no mandamos events....ni procesamos events...
//al menos que Ligia...o algun device especial(nada mas el hecho de que se haya
//enchufado la camara por ejemplo
//podriamos poner una funcion aqui de reconocimiento de DV....
void moVideoManager::Update(moEventList *Events)
{
	moBucket *pbucket = NULL;
	moEvent *actual,*tmp;
	moLiveSystemPtr pLS;
	moVideoSample* pSample = NULL;

	if(!m_pLiveSystems) return;

	if ( GetId() == MO_MOLDEOOBJECT_UNDEFINED_ID ) return;

	if (m_bAutoReconnect) {
		m_pLiveSystems->UpdateLiveSystems();
	}

	actual = Events->First;
	//recorremos todos los events y parseamos el resultado
	//borrando aquellos que ya usamos
	while(actual!=NULL) {
		if(actual->deviceid == GetId() ) {
			pLS = m_pLiveSystems->Get(actual->devicecode);
			if ( pLS!=NULL ) {
				moBucketsPool* pBucketsPool = pLS->GetBucketsPool();
				if ( pBucketsPool!=NULL ) {
					pBucketsPool->DestroyRetreivedBucket();
				}
				pSample = (moVideoSample*)actual->pointer;
				delete pSample;
				pSample = NULL;
			}
			tmp = actual->next;
			Events->Delete(actual);
			actual = tmp;
		} else actual = actual->next;//no es el que necesitamos...
	}


	//aquí moLive da de alta en la lista un evento con su id, devicecode, size y pointer
	//a cada vez que tiene un Bucket lleno....
	for( MOuint i =0; i< m_pLiveSystems->Count(); i++) {

		pLS = m_pLiveSystems->Get(i);

		if ( pLS!=NULL ) {

			moBucketsPool* pBucketsPool = pLS->GetBucketsPool();

			if ( pBucketsPool!=NULL ) {

				if ( pBucketsPool->IsFull() ) {

					pbucket = pBucketsPool->RetreiveBucket();

					pSample = new moVideoSample( pLS->GetVideoGraph()->GetVideoFormat(), (MOpointer) pbucket );

					if(pSample!=NULL) {
						moTexture * ts =(moTexture*)Images[i];
						if(ts!=NULL)
						{

							if ((ts->GetGLId() == 0) || (ts->GetWidth() != pSample->m_VideoFormat.m_Width))
							{
								if (ts->GetGLId() != 0) ts->Finish();
								ts->BuildEmpty(pSample->m_VideoFormat.m_Width,  pSample->m_VideoFormat.m_Height);
							}

							ts->SetBuffer( pbucket->GetBuffer(), GL_BGR_EXT);

							//Update Circular buffers
							for( MOuint c=0; c<m_CircularVideoBuffers.Count(); c++) {

								moCircularVideoBuffer*	pCircularVideoBuffer;
								pCircularVideoBuffer = m_CircularVideoBuffers[c];
								if (pCircularVideoBuffer &&
									pCircularVideoBuffer->GetVideoInput() == ts->GetName() ) {

									pCircularVideoBuffer->LoadSample( pSample );

								}

							}
						}

						//post to other moldeo objects
						Events->Add( GetId(), i, -1, (unsigned char*)pSample);
						//MODebug2->Push( moText("moVideoManager::Update Video Sample") );

					}
				}

			}

		}
	}


	//LOADING ROUTINE
	for(MOuint k=0; k<m_VideoBufferPaths.Count(); k++) {

		moVideoBufferPath*	pVideoBufferPath = m_VideoBufferPaths[k];

		if (pVideoBufferPath && !pVideoBufferPath->LoadCompleted()) {
			pVideoBufferPath->UpdateImages( 10 );
			MODebug2->Push( pVideoBufferPath->m_VideoBufferPath + moText(":") + IntToStr(pVideoBufferPath->m_ImagesProcessed));
		}

	}
}

//================================
// Solo soporta 4 camaras DV, 4 webcams, o 2 videos en vivo
//================================
MOpointer moVideoManager::GetPointer( MOdevcode devcode ) {

	moBucket *pbucket;

	if (m_pLiveSystems) {

		moLiveSystemPtr pLS = m_pLiveSystems->Get( devcode );

		if (pLS) {
			if( pLS->GetBucketsPool() != NULL ) {
				pbucket = pLS->GetBucketsPool()->RetreivedBucket();
				if(pbucket!=NULL) {
					moVideoSample* pSample = NULL;
					pSample = pLS->GetVideoSample();
					if (pSample!=NULL) {
						pSample->SetVideoSample( pLS->GetVideoGraph()->GetVideoFormat(), (MOpointer) pbucket->GetBuffer());
						return (MOpointer)pSample;
					}
				}
			}
		}

	}

	return NULL;
}


MOboolean moVideoManager::Finish()
{
	if (m_pLiveSystems) {
		m_pLiveSystems->UnloadLiveSystems();
		delete m_pLiveSystems;
		m_pLiveSystems = NULL;
	}
	for(MOuint k=0;k<m_VideoBufferPaths.Count();k++) {
		moVideoBufferPath*	pVideoBufferPath = m_VideoBufferPaths[k];
		if (pVideoBufferPath) {
			pVideoBufferPath->Finish();
			delete pVideoBufferPath;
		}
	}
	m_VideoBufferPaths.Empty();
	return true;
}



//=============================================================================================
//
//=============================================================================================


moLiveSystem::moLiveSystem() {

	m_pBucketsPool = NULL;
	m_pVideoGraph = NULL;
	m_pVideoSample = NULL;

	m_CodeName = "";
	m_Type = LST_UNKNOWN;

}


moLiveSystem::~moLiveSystem() {
	Finish();
}

moLiveSystem::moLiveSystem( moText p_CodeName, moLiveSystemType p_Type ) {

	m_pBucketsPool = NULL;
	m_pVideoGraph = NULL;
	m_pVideoSample = NULL;

	m_CodeName = p_CodeName;
	m_Type = p_Type;


}

moLiveSystem::moLiveSystem( moCaptureDevice p_capdev) {

	m_pBucketsPool = NULL;
	m_pVideoGraph = NULL;
	m_pVideoSample = NULL;

	m_CaptureDevice = p_capdev;

	//en fuincion del device path: elegimos este
	if ( p_capdev.GetPath() != moText("") ) {
		m_Type = LST_VIDEOCAMERA;
	}
}

bool
moLiveSystem::Init() {

	if ( m_pBucketsPool!=NULL ) Finish();

	m_pBucketsPool = new moBucketsPool;

	#ifdef MO_WIN32
        #ifdef MO_DIRECTSHOW
		moDsGraph* pDsGraph;
		pDsGraph = new moDsGraph();
		m_pVideoGraph = (moVideoGraph*) pDsGraph;
		#else
		moGsGraph* pGsGraph;
		pGsGraph = new moGsGraph();
		m_pVideoGraph = (moVideoGraph*) pGsGraph;
		#endif
	#else
		moGsGraph* pGsGraph;
		pGsGraph = new moGsGraph();
		m_pVideoGraph = (moVideoGraph*) pGsGraph;
		//ETC...
	#endif

    if ( m_pVideoGraph ) {
        if (m_pVideoGraph->InitGraph()) {
            if( m_pVideoGraph->BuildLiveGraph( m_pBucketsPool , m_CaptureDevice ) ) {
                m_CaptureDevice.SetVideoFormat( m_pVideoGraph->GetVideoFormat() );
                m_pVideoGraph->Play();
                //printf("Device initialized.....\n");
                m_pVideoSample = new moVideoSample( m_pVideoGraph->GetVideoFormat(), NULL );
            } else Finish();
        } else Finish();
    } else Finish();



	return (m_pBucketsPool && m_pVideoGraph && m_pVideoSample);

}

void
moLiveSystem::Finish() {
	if (m_pVideoGraph!=NULL) {
		m_pVideoGraph->FinishGraph();
		delete m_pVideoGraph;
		m_pVideoGraph = NULL;
	}

	if (m_pBucketsPool!=NULL) {
		delete m_pBucketsPool;
		m_pBucketsPool = NULL;
	}

	if (m_pVideoSample) {
		delete m_pVideoSample;
		m_pVideoSample = NULL;
	}
}

void
moLiveSystem::SetCodeName( moText p_CodeName ) {
	m_CodeName = p_CodeName;
}

void
moLiveSystem::SetDeviceName( moText p_DeviceName ) {
	m_DeviceName = p_DeviceName;
}

void
moLiveSystem::SetType( moLiveSystemType p_Type ) {
	m_Type = p_Type;
}

moLiveSystemType
moLiveSystem::GetType() {
	return m_Type;
}

moText
moLiveSystem::GetCodeName() {
	return m_CodeName;
}

moText
moLiveSystem::GetDeviceName() {
	return m_DeviceName;
}

moVideoGraph*
moLiveSystem::GetVideoGraph() {

	return (moVideoGraph*)m_pVideoGraph;

}

moVideoSample*
moLiveSystem::GetVideoSample() {

	return m_pVideoSample;

}

void
moLiveSystem::SetCaptureDevice( moCaptureDevice p_capdev ) {
	m_CaptureDevice = p_capdev;
}

moCaptureDevice
moLiveSystem::GetCaptureDevice() {
	return m_CaptureDevice;
}

moBucketsPool*
moLiveSystem::GetBucketsPool() {
	return m_pBucketsPool;
}

//=============================================================================================
//
//=============================================================================================

moLiveSystems::moLiveSystems() {

        m_pVideoFramework = NULL;

        #ifdef MO_DIRECTSHOW
        	m_pVideoFramework = (moVideoFramework*) new moDsFramework();
		#endif

		#ifdef MO_GSTREAMER
			m_pVideoFramework = new moGsFramework();
        #endif

		#ifdef MO_QUICKTIME
			m_pVideoFramework = (moVideoFramework*) new moQtFramework();
		#endif
}


moLiveSystems::~moLiveSystems() {
        if (m_pVideoFramework!=NULL) {
            delete m_pVideoFramework;
            m_pVideoFramework = NULL;
        }
}

bool
moLiveSystems::LoadLiveSystems( moCaptureDevices* p_pPreferredDevices ) {

	moText CodeStr;
	int i;

	moCaptureDevices* pCapDevs = m_pVideoFramework->LoadCaptureDevices();

	//genera los descriptores de dispositivos de captura...
	m_pVideoFramework->SetPreferredDevices( p_pPreferredDevices );


	/**

	*/
	for( i = 0; i < (int)pCapDevs->Count(); i++) {
		moLiveSystemPtr pLS = new moLiveSystem( pCapDevs->Get(i) );
		if (pLS) {

			CodeStr = "LIVEIN";
			CodeStr+= IntToStr(i);

			pLS->SetCodeName( CodeStr );

			Add( pLS );
		}
	}

	//y el resto de los sistemas los inicializamos vacios
	for( i = pCapDevs->Count(); i < 7; i++) {
		moCaptureDevice Cap( moText(""), moText(""), moText("") );
		Cap.Present(false);
		moLiveSystemPtr pLS = new moLiveSystem( Cap );
		if (pLS) {

			CodeStr = "LIVEIN";
			CodeStr+= IntToStr(i);

			pLS->SetCodeName( CodeStr );

			Add( pLS );
		}
	}

	return ( pCapDevs->Count() > 0 );

}

bool
moLiveSystems::UpdateLiveSystems() {

	moCaptureDevices* pCapDevs = m_pVideoFramework->UpdateCaptureDevices();

	int oldCount = Count();

	for( MOuint i = 0; i < pCapDevs->Count(); i++) {

		if ( pCapDevs->Get(i).IsPresent() ) {

				moLiveSystemPtr pLS = Get(i);

				if ( pLS!=NULL ) {
					if ( pLS->GetCaptureDevice().GetPath() == moText("") &&
						!pLS->GetCaptureDevice().IsPresent() ) {

						//NEW ONE!!!!!!!!!!
						pLS->SetCaptureDevice( pCapDevs->Get(i) );
						//Check if it is really present...maybe it was unplugged entering this loop...
						if (m_pVideoFramework->CheckCaptureDevice( i )) {
							m_pVideoFramework->SetPreferredFormat( pCapDevs->Get(i) );
							pLS->Init();
						}

					} else if ( pLS->GetCaptureDevice().GetPath() == pCapDevs->Get(i).GetPath() &&
								!pLS->GetCaptureDevice().IsPresent()) {
						//RECONNECT: OLD ONE
						if (m_pVideoFramework->CheckCaptureDevice( i )) {
							moCaptureDevice Cap;
							Cap = pLS->GetCaptureDevice();
							Cap.Present(true);
							pLS->SetCaptureDevice( Cap );
							pLS->Init();
						}
					}
				}

		/*
			if (i>=Count()) {
				//ITS A NEW ONE!!! ADD A LIVESYSTEM TO HANDLE IT
				moLiveSystemPtr pLS = new moLiveSystem( pCapDevs->Get(i) );
				if (pLS) {

					moText CodeStr = "LIVE";
					CodeStr+= IntToStr(i);

					pLS->SetCodeName( CodeStr );

					Add( pLS );//beware, count = count + 1
					pLS->Init();
				}
			} else {

				//IT'S AN OLD ONE, BUT MAYBE WASNT CONNECTED BEFORE
				moLiveSystemPtr pLS = Get(i);
				if ( pLS!=NULL ) {
					if ( !pLS->GetVideoGraph()->IsRunning() ) {
						//Check if it is really present...maybe it was unplugged entering this loop...
						if (m_VideoFramework.CheckCaptureDevice( i )) {
							pLS->Init();
						} else {
							pLS->Finish();
						}
					}
				}

			}	*/
		} else {

			//UNCONNECT IT !!
			if ( i<Count() ) {
				moLiveSystemPtr pLS = Get(i);
				if ( pLS!=NULL && pLS->GetCaptureDevice().IsPresent() ) {
					if (pLS->GetVideoGraph()!=NULL) {
						if ( !pLS->GetVideoGraph()->IsRunning() ) {
							pLS->Finish();//we will reconnect it later if the device is present again
							moCaptureDevice Cap;
							Cap = pLS->GetCaptureDevice();
							Cap.Present(false);
							pLS->SetCaptureDevice( Cap );

						}
					}
				}
			}

		}
	}


	return ( (int)Count() > oldCount );

}

bool
moLiveSystems::UnloadLiveSystems() {

	moLiveSystemPtr pLS = NULL;

	m_pVideoFramework->CleanCaptureDevices();

	for( MOuint i= 0; i< Count(); i++ ) {
		pLS = Get(i);
		if (pLS!=NULL) {
			pLS->Finish();
			delete pLS;
			pLS = NULL;
		}
	}
	Empty();
	return (Count()==0);
}

moVideoFramework*
moLiveSystems::GetVideoFramework() {

	return	m_pVideoFramework;

}

bool
moLiveSystems::GetStatus( MOdevcode p_devcode ) {

	moLiveSystemPtr pLS;
	pLS = Get( p_devcode );
	if (pLS!=NULL) {
		if (p_devcode==0) {
			return pLS->GetCaptureDevice().IsPresent();
		}
		return pLS->GetCaptureDevice().IsPresent();
	}
	return false;

}


//===========================================
//
//				moVideoIn
//
//===========================================

moVideoIn::moVideoIn() {

}

moVideoIn::~moVideoIn() {

}

MOboolean
moVideoIn::Init() {
	return true;
}

MOboolean
moVideoIn::Finish() {
	return true;
}


//===========================================
//
//				moVideoOut
//
//===========================================


moVideoOut::moVideoOut() {

}

moVideoOut::~moVideoOut() {

}


MOboolean
moVideoOut::Init() {
	return true;
}

MOboolean
moVideoOut::Finish() {
	return true;
}


