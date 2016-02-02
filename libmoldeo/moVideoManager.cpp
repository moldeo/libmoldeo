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


*******************************************************************************/

#include "moVideoManager.h"

#ifndef MO_DIRECTSHOW
#ifndef MO_QUICKTIME
//  #define MO_GSTREAMER 1
#endif // MO_QUICKTIME
#endif // MO_DIRECTSHOW

#include "moArray.h"
moDefineDynamicArray(moCircularVideoBuffers)
moDefineDynamicArray(moVideoFrames)
moDefineDynamicArray(moVideoBuffers)
moDefineDynamicArray(moLiveSystemPtrs)
moDefineDynamicArray(moVideoBufferPaths)

#include "FreeImage.h"

//===========================================
//
//				moVideoFrame
//
//===========================================

moVideoFrame::moVideoFrame() {
	m_FrameSize = 0;
	hmem = NULL;
	options = 0;
	fif = (int)FIF_UNKNOWN;
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
moVideoFrame::Init( moText bufferformat, moBitmap* pImageResult ) {

    FIBITMAP* _pImageResult = (FIBITMAP*)pImageResult;

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
		} else if ( bufferformat == moText("PNG") ) {
			fif = FIF_PNG;
			options = 0;
		} else {
		    MODebug2->Error( moText("moVideoFrame::Init")
                        + moText(" bufferformat unsupported!!") );
            return false;
        }
		FreeImage_SaveToMemory( (FREE_IMAGE_FORMAT)fif, _pImageResult, (FIMEMORY*)hmem, options );
		m_FrameSize = FreeImage_TellMemory((FIMEMORY*)hmem);
		if (m_FrameSize==0) {
            MODebug2->Error( moText("moVideoFrame::Init")
                        + moText(" Couldn't save image to memory, format may be unsupported") );
            return false;
		}
	}
	return Init();
}

MOboolean
moVideoFrame::Finish() {

	if (hmem) {
		FreeImage_CloseMemory( (FIMEMORY*)hmem );
		hmem = NULL;
	}

	m_FrameSize = 0;
	hmem = NULL;
	fif = (int)FIF_UNKNOWN;

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
moVideoBuffer::LoadImage( moBitmap* pImage, int indeximage ) {

    FIBITMAP* _pImage = (FIBITMAP*)pImage;
	FIBITMAP* pImageResult = NULL;
	FIBITMAP* pImageCropped = NULL;
	FIBITMAP* pImageScaled = NULL;

	if ( m_width!=FreeImage_GetWidth(_pImage) || m_height!=FreeImage_GetHeight(_pImage) ) {
		//CROP MODE
		pImageCropped = FreeImage_Copy( _pImage, m_XSource , m_YSource , m_XSource + m_SourceWidth , m_YSource+m_SourceHeight );
		pImageResult = pImageCropped;

	} else pImageResult = _pImage;

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

  moVideoFrame* pVideoFrame = NULL;

  if (indeximage==-1 || indeximage == (int)m_Frames.Count() ) {
    pVideoFrame = new moVideoFrame();
    if (pVideoFrame) {
      pVideoFrame->Init( m_BufferFormat, pImageResult );
      m_Frames.Add(pVideoFrame);
    }
  } else if ( indeximage < (int)m_Frames.Count() ) {
    pVideoFrame = m_Frames.Get(indeximage);
    if (pVideoFrame) {
      pVideoFrame->Finish();
      pVideoFrame->Init( m_BufferFormat, pImageResult );
    }
  }

	if (pImageResult!=_pImage)
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

			FIBITMAP *pImage;
			//FIMEMORY	*hmem;

			MOuint p_format;

			//MOint FrameSize =
			FreeImage_TellMemory((FIMEMORY*)pVideoFrame->hmem);
			FreeImage_SeekMemory( (FIMEMORY*)pVideoFrame->hmem, 0L, SEEK_SET);

			//hmem = FreeImage_OpenMemory( pVideoFrame->hmem,  pVideoFrame->m_FrameSize);
			//FreeImage_TellMemory(VideoFrame->hmem);
			// load an image from the memory stream
			pImage = FreeImage_LoadFromMemory( (FREE_IMAGE_FORMAT)pVideoFrame->fif, (FIMEMORY*)pVideoFrame->hmem, 0);

			if (pImage) {
                switch (FreeImage_GetBPP(pImage))
                {
                    case 8: // 8 bit, indexed or grayscale
                        m_param.internal_format = GL_RGB;
                        p_format = GL_LUMINANCE;
                        break;
                    case 16: // 16 bits
                        break;
                    case 24: // 24 bits
                        m_param.internal_format = GL_RGB;
#ifndef OPENGLESV2
                        if (FreeImage_GetBlueMask(pImage) == 0x000000FF) p_format = GL_BGR;
                        else
#endif
			p_format = GL_RGB;
                        break;
                    case 32: // 32 bits
                        m_param.internal_format = GL_RGBA;
#ifndef OPENGLESV2
                        if (FreeImage_GetBlueMask(pImage) == 0x000000FF) p_format = GL_BGRA_EXT;
                        else
#endif
			p_format = GL_RGBA;
                        break;
                    default:
                        break;
                }
                SetBuffer( m_width, m_height, FreeImage_GetBits(pImage), p_format);
                FreeImage_Unload( pImage );

                this->m_ActualFrame = p_i;

			} else MODebug2->Error( moText("moVideoBuffer::GetFrame") +
                                    moText("Couldn't load image from memory, texture name: ") +
                                    moText(this->GetName())
                                    );
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

	///RESCALE
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

			FIBITMAP *pImage;
			//FIMEMORY	*hmem;

			MOuint p_format;

			//MOint FrameSize =
			FreeImage_TellMemory((FIMEMORY*)pVideoFrame->hmem);
			FreeImage_SeekMemory( (FIMEMORY*)pVideoFrame->hmem, 0L, SEEK_SET);

			//hmem = FreeImage_OpenMemory( pVideoFrame->hmem,  pVideoFrame->m_FrameSize);
			//FreeImage_TellMemory(VideoFrame->hmem);
			// load an image from the memory stream
			pImage = FreeImage_LoadFromMemory( (FREE_IMAGE_FORMAT)pVideoFrame->fif, (FIMEMORY*)pVideoFrame->hmem, 0);

			switch (FreeImage_GetBPP(pImage))
			{
				case 8: // 8 bit, indexed or grayscale
					m_param.internal_format = GL_RGB;
					p_format = GL_LUMINANCE;
					break;
				case 16: // 16 bits
					break;
				case 24: // 24 bits
					m_param.internal_format = GL_RGB;
#ifndef OPENGLESV2
					if (FreeImage_GetBlueMask(pImage) == 0x000000FF) p_format = GL_BGR;
					else
#endif
p_format = GL_RGB;
					break;
				case 32: // 32 bits
					m_param.internal_format = GL_RGBA;
#ifndef OPENGLESV2
					if (FreeImage_GetBlueMask(pImage) == 0x000000FF) p_format = GL_BGRA_EXT;
					else
#endif
p_format = GL_RGBA;
					break;
				default:
					break;
			}
			SetBuffer( m_width, m_height, FreeImage_GetBits(pImage), p_format);
			FreeImage_Unload( pImage );
		}
	}

}


void  moCircularVideoBuffer::StartRecording( long at_position  ) {
  if (at_position!=-1 && at_position>=0 && at_position<(long)m_Frames.Count()) {
    m_WriteIndex = at_position;
  }
  if (!m_RecTimer.Started()) {
    m_RecTimer.Start();
  } else {
    m_RecTimer.Continue();
  }
}

void  moCircularVideoBuffer::PauseRecording() {
  m_RecTimer.Pause();
}

void  moCircularVideoBuffer::ContinueRecording() {
  m_RecTimer.Continue();
}

void  moCircularVideoBuffer::JumpRecording( long at_position ) {
  //m_RecTimer.Start();
  //chequear
  if ( at_position>=0 && at_position<(long)m_Frames.Count()) {
    m_WriteIndex = at_position;
  }
}

void  moCircularVideoBuffer::StopRecording() {
  m_RecTimer.Stop();
}

bool  moCircularVideoBuffer::IsRecording() {
  return m_RecTimer.Started();
}

long   moCircularVideoBuffer::GetRecordPosition() {
  return m_WriteIndex;
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
	m_TotalFiles = 0;

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

    if (m_pDirectory) this->m_TotalFiles = (MOint)m_pDirectory->GetFiles().Count();

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

moText      moVideoBufferPath::GetPath() {
    return this->m_VideoBufferPath;
}

moText      moVideoBufferPath::GetCompletePath() {
    return this->m_CompletePath;
}


int         moVideoBufferPath::GetTotalFiles() {
    return this->m_TotalFiles;
}

int         moVideoBufferPath::GetImagesProcessed() {
    return this->m_ImagesProcessed;
}

int         moVideoBufferPath::GetActualImage() {
    return this->m_ActualImage;
}

MOboolean moVideoBufferPath::UpdateImages( MOint maxfiles ) {

	//carga los frames desde los archivos
	moFile* pFile;

	MOint counter = 0;

	if (!m_pDirectory) return false;

    this->m_TotalFiles = (MOint)m_pDirectory->GetFiles().Count();

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
					MODebug2->Message( "image processed: "+pFile->GetCompletePath());
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
	SetLabelName("videomanager");

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

  moResource::Init();

  m_pLiveSystems = new moLiveSystems();
  m_pLiveSystems->Init(0, NULL);



	if (m_Config.IsConfigLoaded()) {

      moText msg = "In moVideoManager::Init from config file ***********************\n";
      msg+= moText("Initializing Live...\n");
      MODebug2->Message( msg );

      preferreddevices = m_Config.GetParamIndex("preferreddevices");//FIREWIRE, WEBCAMS, LIVEVIDEO[movie]
      autoreconnect = m_Config.GetParamIndex("autoreconnect");//FIREWIRE, WEBCAMS, LIVEVIDEO[movie]
      //MOuint videoin = m_Config.GetParamIndex("videoin");

     // moText videoinname;

/*
      MODebug2->Message( "VideoManager:: Generating videoin textures" );
      for(MOuint i=0; i<m_Config.GetValuesCount(videoin); i++ ) {
        videoinname = m_Config.GetParam(videoin).GetValue(i).GetSubValue(0).Text();
        if(videoinname!=moText("")) {
           int tid = m_pResourceManager->GetTextureMan()->AddTexture( MO_TYPE_TEXTURE, videoinname);
           if(tid>-1) Images.Add( m_pResourceManager->GetTextureMan()->GetTexture(tid));
          MODebug2->Message( moText("VideoManager:: Added ") + moText(videoinname) );
        }
      }
      */
      /**	LOAD AUTORECONNECT PARAMETER...	*/
      m_bAutoReconnect = (bool) m_Config.GetParam((MOint)autoreconnect).GetValue().GetSubValue().Int();

      /**        LOAD PREFERRED DEVICES CONFIGURATION	*/
      pPreferredDevices = new moCaptureDevices();

      nvalues = m_Config.GetValuesCount( preferreddevices );
      m_Config.SetCurrentParamIndex(preferreddevices);
      /*
    MO_LIVE_LABELNAME	0
    MO_LIVE_DEVICENAME	1
    MO_LIVE_COLORMODE 2
    MO_LIVE_WIDTH		3
    MO_LIVE_HEIGHT		4
    MO_LIVE_BITCOUNT	5
    MO_LIVE_SCALE_WIDTH 6
    MO_LIVE_SCALE_HEIGHT 7
    MO_LIVE_FLIPH 8
    MO_LIVE_FLIPV 9
      */
      MODebug2->Message(moText("Setting preferred devices"));
      for( MOuint i = 0; i < nvalues; i++) {

        m_Config.SetCurrentValueIndex(preferreddevices, i);

        MOint srcw(0),srch(0),srcbpp(0),flH(0),flV(0);
        MOint ncount = m_Config.GetParam().GetValue().GetSubValueCount();


        ( MO_LIVE_WIDTH < ncount ) ? srcw = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_WIDTH).Int() : srcw = 0;
        ( MO_LIVE_HEIGHT < ncount ) ? srch = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_HEIGHT).Int() : srch = 0;
        ( MO_LIVE_BITCOUNT < ncount ) ? srcbpp = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_BITCOUNT).Int() : srcbpp = 0;
        ( MO_LIVE_FLIPH < ncount ) ? flH = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_FLIPH).Int() : flH = 0;
        ( MO_LIVE_FLIPV < ncount ) ? flV = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_FLIPV).Int() : flV = 0;

        moVideoFormat VF;
        moCaptureDevice CD( m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_DEVICENAME).Text(), moText(""), moText(""), 0, srcw, srch, srcbpp, flH, flV );

        CD.SetLabelName( m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_LABELNAME).Text() );

        VF.m_Width = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_SCALEWIDTH).Int();
        VF.m_Height = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_SCALEHEIGHT).Int();
        VF.m_BitCount = m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_BITCOUNT).Int();

        VF.m_ColorMode = (moColorMode)m_Config.GetParam().GetValue().GetSubValue(MO_LIVE_COLORMODE).Int();

        CD.SetVideoFormat( VF );
        CD.SetPreffered( true );

        pPreferredDevices->Add( CD );
        MODebug2->Message(moText("Added preferred device setting: Device Name: ")+CD.GetName());
        //for( int k = 1; k < m_Config.GetParam().GetSubCount(); k++);
      }



    //INITIALIZING LiveSystems

      //try to connect to all predefined


      MODebug2->Message(moText("Finally load live systems grabber for each device...."));


      if (!m_pLiveSystems) return false;

      m_pLiveSystems->GetVideoFramework()->SetPreferredDevices( pPreferredDevices );

      GetCaptureDevices();

      for( int c=0; c<(int)m_CaptureDevices.Count(); c++) {
        moCaptureDevice CapDev = m_CaptureDevices.Get(c);
        if (CapDev.IsPreferred()) {
          moCamera* Cam = CreateCamera( CapDev );
          if (Cam==NULL) MODebug2->Error("moVideoManager::Init > could create Camera: " + CapDev.GetName()+ " Label: " + CapDev.GetLabelName() );
        }

      }

      //if ( m_pLiveSystems->LoadLiveSystems( pPreferredDevices ) ) {
        //los inicializa...
       // for( MOuint i = 0; i < m_pLiveSystems->Count(); i++) {

      //    moLiveSystemPtr pLS = m_pLiveSystems->Get(i);

      //    if (pLS && pLS->GetCaptureDevice().IsPresent()) {

     //       if (pLS->Init()) {

    //            MODebug2->Message( moText(pLS->GetCaptureDevice().GetName()) + " initialized");

/*
                if ( pLS->GetVideoGraph()->GetVideoFormat().m_WaitForFormat == false  && 1==2 ) {

                    moBucketsPool* pBucketsPool = pLS->GetBucketsPool();

                    if ( pBucketsPool!=NULL ) {

                        if ( pBucketsPool->IsFull() ) {

                            moBucket* pbucket = pBucketsPool->RetreiveBucket();

                            if ( pbucket && pLS->GetVideoGraph() ) {

                                //moVideoSample* pSample = new moVideoSample( pLS->GetVideoGraph()->GetVideoFormat(), (MOpointer) pbucket );

                                ///procesamos el GetState para que ejecute la iteracion del loop interno...obligatorio!!
                                pLS->GetVideoGraph()->GetState();

                                //if( pSample!=NULL ) {

                                  //moTexture * ts =(moTexture*)Images[i];
                                  moTexture* ts = pLS->GetTexture();

                                  if(ts!=NULL)
                                  {

                                    if ((ts->GetGLId() <= 0) || (ts->GetWidth() != pLS->GetVideoGraph()->GetVideoFormat().m_Width))
                                    {
                                      if (ts->GetGLId() != 0) ts->Finish();
                                      ts->BuildEmpty(pLS->GetVideoGraph()->GetVideoFormat().m_Width,  pLS->GetVideoGraph()->GetVideoFormat().m_Height);
                                    }

                                    MOubyte* pbuffer = pbucket->GetBuffer();

                                    if (pbuffer) {
                                      pbucket->Lock();
                                      ts->SetBuffer( pbuffer, GL_BGR_EXT);
                                      pbucket->Unlock();
                                    }
                                }


                            }

                        }
                    }
                }*/
  //          }
  //        }

  //      }


      //} else MODebug2->Error(moText("Error. No livesystems loaded??...."));


      MOuint circularbuffer = m_Config.GetParamIndex("circularbuffer");

      moText videobufferinput;
      moText videobuffername;
      moText videobufferformat;
      MOint	xsource, ysource, width, height, sourcewidth, sourceheight, frames;
      MOint	interpolation, interpolation_jump, interpolation_time;

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
            pCircularBuffer->StartRecording();
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
            interpolation_jump = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEINTERPOLATIONJUMP).Int();
            interpolation_time = m_Config.GetParam(videobuffer).GetValue(i).GetSubValue(j*11+MO_VIDEO_SOURCEINTERPOLATIONTIME).Int();

            if(videobufferpath!=moText("")) {
              moVideoBuffer* pVideoBuffer = NULL;
              int tid = m_pResourceManager->GetTextureMan()->AddTexture( MO_TYPE_VIDEOBUFFER, videobuffername );
              if(tid>-1) {
                pVideoBuffer =  (moVideoBuffer*) m_pResourceManager->GetTextureMan()->GetTexture(tid);
              }
              if (pVideoBuffer) {
                pVideoBuffer->Init(  videobufferpath, videobufferformat, GetResourceManager(), width, height, xsource, ysource, sourcewidth, sourceheight, interpolation, interpolation_jump, interpolation_time);
                pVideoBufferPath->m_VideoBuffers.Add(pVideoBuffer);
              }
            }
          }
        }
      }

      delete pPreferredDevices;

	} else {

      /// TODO: create values from default
      /// making accesible default camera if present with default value
      /// ....

  }


	m_bLoading = true;
	m_bInitialized = true;

	return true;
}

MOswitch moVideoManager::SetStatus(MOdevcode devcode, MOswitch state)
{

    if (devcode == 0 /**RELOAD CAPTURE DEVICES*/) {
      if (state==MO_ON) {
        GetCaptureDevices(true);
      }
    }
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
				if ( pLS->GetLabelName() == strcod ) {
					return i;
				}
			}
		}
	}

    return(-1);
}

moCamera* moVideoManager::GetCamera( int cam_idx ) {
  if (m_pLiveSystems) {
    return m_pLiveSystems->Get(cam_idx);
  }
  return NULL;
}

moCamera*
moVideoManager::CreateCamera( const moCaptureDevice& p_CapDev ) {

  MODebug2->Message("moVideoManager::CreateCamera > " + p_CapDev.GetLabelName()+" for device:"+p_CapDev.GetName() );

/// create
  moLiveSystem* pLS = new moLiveSystem(p_CapDev);
  moTextureManager* TMan = m_pResourceManager->GetTextureMan();

  if (!pLS) return NULL;

  if (!pLS->Init()) return NULL;

  MODebug2->Message( moText("moVideoManager::CreateCamera > Capture Device ")
                    + pLS->GetCaptureDevice().GetName()
                    + " initialized");

  int mid = TMan->AddTexture( MO_TYPE_TEXTURE, pLS->GetLabelName() );

  moTexture * ts = NULL;

  if (mid>-1) ts = TMan->GetTexture(mid);

  if(ts)
  {
    pLS->SetTexture(ts);
    MODebug2->Message("moVideoManager::CreateCamera > texture added ok: " + ts->GetName() );
  } else {
    MODebug2->Error("moVideoManager::CreateCamera > texture not added. " +ts->GetName()  );
  }

/*
  if ( pLS->GetVideoGraph()->GetVideoFormat().m_WaitForFormat == false  && 1==2 ) {

      moBucketsPool* pBucketsPool = pLS->GetBucketsPool();

      if ( pBucketsPool==NULL ) return NULL;

      if ( pBucketsPool->IsFull() ) {

        moBucket* pbucket = pBucketsPool->RetreiveBucket();

        if ( pbucket && pLS->GetVideoGraph() ) {

          //moVideoSample* pSample = new moVideoSample( pLS->GetVideoGraph()->GetVideoFormat(), (MOpointer) pbucket );

          ///procesamos el GetState para que ejecute la iteracion del loop interno...obligatorio!!
          pLS->GetVideoGraph()->GetState();


          int mid = TMan->AddTexture( MO_TYPE_TEXTURE, pLS->GetLabelName() );
          moTexture * ts = NULL;
          if (mid>-1) ts = TMan->GetTexture(mid);
          pLS->SetTexture(ts);
          if(ts!=NULL)
          {
            MODebug2->Log("moVideoManager::CreateCamera > texture added ok: " + ts->GetName() );
            if ((ts->GetGLId() <= 0) || (ts->GetWidth() != pLS->GetVideoGraph()->GetVideoFormat().m_Width))
            {
              if (ts->GetGLId() != 0) ts->Finish();
              ts->BuildEmpty(pLS->GetVideoGraph()->GetVideoFormat().m_Width,  pLS->GetVideoGraph()->GetVideoFormat().m_Height);
            }

            MOubyte* pbuffer = pbucket->GetBuffer();

            if (pbuffer) {
              pbucket->Lock();
              ts->SetBuffer( pbuffer, GL_BGR_EXT);
              pbucket->Unlock();
            }

          }
      }
    }
  }
  */

  m_pLiveSystems->Add( pLS );
  MODebug2->Message( moText("Added LiveSystem: Label Name: ") + pLS->GetCaptureDevice().GetLabelName()
                    + moText(" Device Name:") + pLS->GetCaptureDevice().GetName() );

  return (moCamera*)pLS;
}

moCamera*
moVideoManager::GetCameraByName( const moText& camera, bool load, moCaptureDevice customCD ) {

  moCamera* Cam = NULL;

  if (m_pLiveSystems) {
    for( int c=0; c<(int)m_pLiveSystems->Count(); c++ ) {
      Cam = m_pLiveSystems->Get(c);
      if (Cam) {
        if (Cam->GetCaptureDevice().GetLabelName()==camera
            ||
            Cam->GetCaptureDevice().GetName()==camera
            ||
            ( camera=="default" && c>=0 && Cam->GetVideoGraph() )
           ) {
          return Cam;
        }
      }
    }
  }

  for(int d=0;d<(int)m_CaptureDevices.Count();d++) {
    moCaptureDevice m_CapDev = m_CaptureDevices[d];
    if (m_CapDev.GetName()==camera
        ||
        m_CapDev.GetLabelName()==camera
        ||
        (  camera=="default" && d>=0 && m_CapDev.IsPresent() )
         ) {
        ///Try to create it!!!
        if (load) {
          moCaptureDevice newCD = m_CapDev;

          if ( m_pResourceManager->GetRenderMan()->IsTextureNonPowerOf2Disabled() ) {
            //chequear video format
            newCD.GetVideoFormat().m_Width = 256;
            newCD.GetVideoFormat().m_Height = 256;
            //newCD.GetVideoFormat().m_ColorMode = (moColorMode) ;
            newCD.GetVideoFormat().m_BitCount = 24;
            newCD.GetVideoFormat().m_ColorMode = (moColorMode) 0;

          }

          // apply custom capture device values from customCD
          if ( customCD.GetLabelName()!=""
              && customCD.GetVideoFormat().m_Width>0
              && customCD.GetVideoFormat().m_Height>0 ) {
            newCD = customCD;
            newCD.SetName( m_CapDev.GetName() );
            newCD.SetLabelName( m_CapDev.GetLabelName() );
            if ( m_pResourceManager->GetRenderMan()->IsTextureNonPowerOf2Disabled() ) {
              moMathi mathI;
              if ( !mathI.IsPowerOfTwo( newCD.GetVideoFormat().m_Width ) ) {
                newCD.GetVideoFormat().m_Width = moTexture::NextPowerOf2( newCD.GetVideoFormat().m_Width );
              }
              if ( !mathI.IsPowerOfTwo( newCD.GetVideoFormat().m_Height ) ) {
                newCD.GetVideoFormat().m_Height = moTexture::NextPowerOf2( newCD.GetVideoFormat().m_Height );
              }
            }
          }

          Cam = CreateCamera( newCD );
          return Cam;
        }
      }
  }

  return NULL;
}

int moVideoManager::GetCameraCount() {
  if (m_pLiveSystems)
    return m_pLiveSystems->Count();
  return 0;
}

const moTextArray&
moVideoManager::GetCameraNames() {

  /** Loads capture devices.... preferred devices go first... or they are just OPENED */
  ///
  ///m_pLiveSystems->GetVideoFramework();
  ///m_pLiveSystems->GetStatus(devcode);
  return m_CameraDevices;
}

const moCaptureDevices&
moVideoManager::GetCaptureDevices( bool reload ) {
  if (m_pLiveSystems && m_pLiveSystems->GetVideoFramework()) {
    if (reload) {
      m_CaptureDevices = (*m_pLiveSystems->GetVideoFramework()->LoadCaptureDevices());
    } /*else m_CaptureDevices = (*m_pLiveSystems->GetVideoFramework()->GetCaptureDevices());*/
  }
  return m_CaptureDevices;
}

moCircularVideoBuffer* moVideoManager::GetCircularVideoBuffer( int cb_idx ) {
  return m_CircularVideoBuffers.GetRef(cb_idx);
}

int moVideoManager::GetCircularVideoBufferCount() {
  return m_CircularVideoBuffers.Count();
}

moVideoBufferPath* moVideoManager::GetVideoBufferPath( int vb_idx ) {
  return m_VideoBufferPaths.GetRef(vb_idx);
}

int moVideoManager::GetVideoBufferPathCount() {
  return m_VideoBufferPaths.Count();
}

moVideoBuffer* moVideoManager::GetVideoBuffer( int vb_idx ) {

  moVideoBuffer* pVideoBuffer = NULL;

  moVideoBufferPath* pVideoBufferPath = m_VideoBufferPaths.GetRef(vb_idx);

  if (pVideoBufferPath) {
    if (pVideoBufferPath->m_VideoBuffers.Count()>0) {
        pVideoBuffer = pVideoBufferPath->m_VideoBuffers.GetRef(0);
    }
  }
  return pVideoBuffer;
}

int moVideoManager::GetVideoBufferCount() {
  return m_VideoBufferPaths.Count();
}

#include <gst/gst.h>

moText moVideoManager::NanosecondsToTimecode( MOulonglong duration ) {

  moText timecodestr;
  MOulonglong years,oneyear;
  MOulonglong days,oneday;
  MOulonglong hours,onehour;
  MOulonglong minutes,oneminute;
  MOulonglong seconds,onesecond;
  MOulonglong miliseconds,onemilisecond;
  //MOulonglong nanos;

  //nanos = 0;
  onemilisecond = GST_MSECOND;
  onesecond = GST_SECOND;
  oneminute = 60 * onesecond;
  onehour = 60 * oneminute;
  oneday = 24 * onehour;
  oneyear = 365 * oneday;

  years = duration / oneyear; duration = duration - years * oneyear;
  days = duration / oneday; duration = duration - days * oneday;
  hours = duration / onehour; duration = duration - hours * onehour;
  minutes = duration / oneminute; duration = duration - minutes * oneminute;
  seconds = duration / onesecond; duration = duration - seconds * onesecond;
  miliseconds = duration / onemilisecond;  duration = duration - miliseconds * onemilisecond;
  //nanos = duration - seconds * onesecond;
  timecodestr = "";
  if (years>0) timecodestr = IntToStr(years) + " years ";
  if (days>0) timecodestr+= IntToStr(days) + " days ";
  if (hours>0) { timecodestr+= IntToStr(hours,2) + ":"; }
  else  { timecodestr+= "00:"; }
  if (minutes>0) { timecodestr+= IntToStr(minutes,2) + ":"; }
  else { timecodestr+= "00:"; }
  if (seconds>0) { timecodestr+= IntToStr(seconds,2) + "."; }
  else { timecodestr+= "00."; }
  if (miliseconds>0) { timecodestr+= IntToStr(miliseconds/10,2); }
  else { timecodestr+= "00"; }
  return timecodestr;
}

moText moVideoManager::FramesToTimecode( MOulonglong duration, double framespersecond ) {

  moText timecodestr;
  double years,oneyear;
  double days,oneday;
  double hours,onehour;
  double minutes,oneminute;
  double seconds,onesecond;
  double miliseconds,onemilisecond;
  //double nanos;
  double frames,durationF;

  //nanos = 0;
  onemilisecond = framespersecond/1000.0f;
  onesecond = framespersecond;
  oneminute = 60 * onesecond;
  onehour = 60 * oneminute;
  oneday = 24 * onehour;
  oneyear = 365 * oneday;
  durationF = duration;

  years = moMathd::Floor(durationF / oneyear); duration = duration - (long)(years * oneyear);durationF=duration;
  days = moMathd::Floor(durationF / oneday); duration = duration - (long)(days * oneday);durationF=duration;
  hours = moMathd::Floor(durationF / onehour); duration = duration - (long)(hours * onehour);durationF=duration;
  minutes = moMathd::Floor(durationF / oneminute); duration = duration - (long)(minutes * oneminute);durationF=duration;
  seconds = moMathd::Floor(durationF / onesecond); duration = duration - (long)(seconds * onesecond);durationF=duration;
  frames = moMathd::Floor(durationF); duration = duration - (long)(frames*1.0);durationF=duration;
  miliseconds = moMathd::Floor(durationF / onemilisecond);  duration = duration - (long)(miliseconds * onemilisecond);durationF=duration;

  //nanos = duration - seconds * onesecond;
  timecodestr =  "";
  if (years>0) timecodestr = IntToStr((long)years) + " years ";
  if (days>0) timecodestr+= IntToStr((long)days) + " days ";
  if (hours>0) { timecodestr+= IntToStr((long)hours) + ":"; }
  else { timecodestr+= "00:"; }
  if (minutes>0) { timecodestr+= IntToStr((long)minutes,2) + ":"; }
  else { timecodestr+= "00:"; }
  if (seconds>0) {timecodestr+= IntToStr((long)seconds,2) + ":"; }
  else { timecodestr+= "00:"; }
  if (frames>0) { timecodestr+= IntToStr((long)frames,2) + "."; }
  else { timecodestr+= "00."; }
  if (miliseconds>0) { timecodestr+= IntToStr((int)miliseconds); }
  else { timecodestr+= "000"; }

  return timecodestr;

}


//esto se ejecuta por cada CICLO PRINCIPAL DE moConsole(frame)
//por ahora no nos sirve de nada, porque no mandamos events....ni procesamos events...
//al menos que Ligia...o algun device especial(nada mas el hecho de que se haya
//enchufado la camara por ejemplo
//podriamos poner una funcion aqui de reconocimiento de DV....
void moVideoManager::Update(moEventList * p_EventList)
{
	moBucket *pbucket = NULL;
	moEvent *actual,*tmp;
	moLiveSystemPtr pLS;
	moVideoSample* pSample = NULL;
	MOubyte* pbuffer = NULL;

	if(!m_pLiveSystems) return;

	if ( GetId() == MO_MOLDEOOBJECT_UNDEFINED_ID ) return;

	if (m_bAutoReconnect) {
		m_pLiveSystems->UpdateLiveSystems();
	}

	actual = p_EventList->First;
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
			p_EventList->Delete(actual);
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

					if ( pbucket && pLS->GetVideoGraph() ) {

            pSample = new moVideoSample( pLS->GetVideoGraph()->GetVideoFormat(), (MOpointer) pbucket );

            ///procesamos el GetState para que ejecute la iteracion del loop interno...obligatorio!!
            pLS->GetVideoGraph()->GetState();

            if( pSample!=NULL ) {
              //moTexture * ts =(moTexture*)Images[i];
              moTexture* ts = (moTexture*) pLS->GetTexture();

              if(ts!=NULL)
              {

                if ((ts->GetGLId() == 0) || (ts->GetWidth() != pSample->m_VideoFormat.m_Width))
                {
                  if (ts->GetGLId() != 0) ts->Finish();
                  ts->BuildEmpty( pSample->m_VideoFormat.m_Width,  pSample->m_VideoFormat.m_Height );
                }

                pbuffer = pbucket->GetBuffer();

                if (pbuffer) {
                  pbucket->Lock();
/**
#ifndef OPENGLESV2
                  ts->SetBuffer( pbuffer, GL_BGR_EXT);
#else
		  ts->SetBuffer( pbuffer, GL_RGB);
#endif
    */

                    moVideoFormat vf = pSample->m_VideoFormat;
                    if (vf.m_RedMask==255 && vf.m_BufferSize>0) {
                        ///INVERT RED AND BLUE
                        int bypp = (vf.m_BitCount>>3);
                        for( int a=0; a<(int)vf.m_BufferSize; a+=bypp ) {
                            BYTE* pix = &pbuffer[a];
                            BYTE u = pix[0];
                            pix[0] = pix[2];
                            pix[2] = u;
                        }
                    }

                  ts->SetBuffer( pbuffer, GL_RGB);
                  pbucket->Unlock();
                }

                //Update Circular buffers
                for( MOuint c=0; c<m_CircularVideoBuffers.Count(); c++) {

                  moCircularVideoBuffer*	pCircularVideoBuffer;
                  pCircularVideoBuffer = m_CircularVideoBuffers[c];
                  if (pCircularVideoBuffer &&
                    pCircularVideoBuffer->GetVideoInput() == ts->GetName() ) {

                    if (pCircularVideoBuffer->IsRecording()) {
                      pCircularVideoBuffer->LoadSample( pSample );
                    }

                  }

                }
              }

              //post to other moldeo objects

              //p_EventList->Add( GetId(), i, -1, (unsigned char*)pSample);
              delete pSample;

              //MODebug2->Push( moText("moVideoManager::Update Video Sample") );

            }
            pBucketsPool->DestroyRetreivedBucket();

					} ///finish with pbucket

				}

			}

		}
	}


	//LOADING ROUTINE
	for(MOuint k=0; k<m_VideoBufferPaths.Count(); k++) {

		moVideoBufferPath*	pVideoBufferPath = m_VideoBufferPaths[k];

		if (pVideoBufferPath && !pVideoBufferPath->LoadCompleted()) {
			pVideoBufferPath->UpdateImages( 1 );
			MODebug2->Push( pVideoBufferPath->GetPath() + moText(":") + IntToStr(pVideoBufferPath->GetImagesProcessed()));
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

	m_Type = LST_UNKNOWN;

}


moLiveSystem::~moLiveSystem() {
	Finish();
}


moLiveSystem::moLiveSystem( moCaptureDevice p_capdev) {

	m_pBucketsPool = NULL;
	m_pVideoGraph = NULL;
	m_pVideoSample = NULL;
	m_pTexture = NULL;

	m_CaptureDevice = p_capdev;

	//en fuincion del device path: elegimos este
	if ( p_capdev.GetPath() != moText("") ) {
		m_Type = LST_VIDEOCAMERA;
	}
}

bool
moLiveSystem::Init() {

	if ( m_pBucketsPool!=NULL ) Finish();

	m_pBucketsPool = new moBucketsPool();

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
moLiveSystem::SetTexture( moTexture* p_Texture ) {
  m_pTexture = p_Texture;
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
moLiveSystem::GetLabelName() {
	return m_CaptureDevice.GetLabelName();
}

moText
moLiveSystem::GetDeviceName() {
	return m_CaptureDevice.GetName();
}

moVideoGraph*
moLiveSystem::GetVideoGraph() {

	return (moVideoGraph*)m_pVideoGraph;

}

moVideoSample*
moLiveSystem::GetVideoSample() {

	return m_pVideoSample;

}


moTexture*
moLiveSystem::GetTexture() {
  return m_pTexture;
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

	//genera los descriptores de dispositivos de captura...
	m_pVideoFramework->SetPreferredDevices( p_pPreferredDevices );

	moCaptureDevices* pCapDevs = m_pVideoFramework->LoadCaptureDevices();

	/**

	*/
	/*
	for( i = 0; i < (int)pCapDevs->Count(); i++) {
		moLiveSystemPtr pLS = new moLiveSystem( pCapDevs->Get(i) );
		if (pLS) {

			Add( pLS );
			moDebugManager::Message( moText("Added LiveSystem: CodeName:") + (moText)CodeStr + moText(" Device Name:") + pCapDevs->Get(i).GetName() );
		}
	}
*/
	//y el resto de los sistemas los inicializamos vacios
	/*
	for( i = pCapDevs->Count(); i < 7; i++) {
		moCaptureDevice Cap( moText(""), moText(""), moText("") );
		Cap.Present(false);
		moLiveSystemPtr pLS = new moLiveSystem( Cap );
		if (pLS) {
			Add( pLS );
		}
	}
*/
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



