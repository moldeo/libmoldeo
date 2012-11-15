/*******************************************************************************

                              moTextureManager.cpp

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
  Andres Colubri

*******************************************************************************/

#include "moTextureManager.h"
#include <FreeImage.h>
#include <moDataManager.h>
#include <moFileManager.h>

#include "moArray.h"
moDefineDynamicArray(moTextureBuffers)
moDefineDynamicArray(moTextureFrames)


/**
FreeImage error handler
@param fif Format / Plugin responsible for the error
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {

   if(fif != FIF_UNKNOWN) {
     //printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
   }
   moAbstract::MODebug2->Error(moText("FreeImage error:") + moText(message));

}


//===========================================
//
//				moTextureBuffer
//
//===========================================

moTextureBuffer::moTextureBuffer() {

	//m_type = MO_TYPE_TEXTUREBUFFER;
	m_ImagesProcessed = 0;
	m_ActualImage = 0;
	m_pResourceManager = NULL;
	m_pDirectory = NULL;
	m_bLoadCompleted = false;
	m_pBufferLevels = new moTextureFrames* [100];
	for( int i=0; i<100; i++ ) {
	    m_pBufferLevels[i] = new moTextureFrames [10];
	    for( int j = 0; j<10; j++ ) {
            m_pBufferLevels[i][j].Init( 0 , NULL );
        }
	}

	LevelDiagram = NULL;
    ///size of max_luminance: 100
    ///how many levels: 100
    LevelDiagram = new MObyte [ 100 * 100 * 3];

}

moTextureBuffer::~moTextureBuffer() {
	Finish();
}

MOboolean  moTextureBuffer::Init() {

	if (!m_pResourceManager)
		return false;



	return m_bInitialized;
}

MOboolean
moTextureBuffer::Init( moText p_foldername, moTextureBufferFormat p_bufferformat, int p_pattern_width, int p_pattern_height, moResourceManager* p_pResourceManager ) {

    moText str_format;

    pattern_width = p_pattern_width;
    pattern_height = p_pattern_height;

    if (p_bufferformat==MO_TEXTURE_BUFFER_FORMAT_LEVELS) {

        str_format = "LEVELS";

    } else if (p_bufferformat==MO_TEXTURE_BUFFER_FORMAT_PATTERNS) {

        str_format = "PATTERNS";

    }

    Init( p_foldername, str_format, p_pResourceManager );
}

MOboolean
moTextureBuffer::Init( moText p_foldername, moText p_bufferformat, moResourceManager* p_pResourceManager ) {
	m_pResourceManager = p_pResourceManager;

	m_FolderName = p_foldername;
	m_BufferPath = m_pResourceManager->GetDataMan()->GetDataPath() + (moText)p_foldername;
	m_BufferFormat = p_bufferformat;


	m_pDirectory = m_pResourceManager->GetFileMan()->GetDirectory( m_BufferPath );

	if (!m_pDirectory) return false;

	//BuildEmpty( width, height);

	/*
	moShaderManager* SM = m_pResourceManager->GetShaderMan();
	moTextureManager* TM = m_pResourceManager->GetTextureMan();

	m_pShaderCopy = SM->GetShader(SM->GetShaderIndex(moText("shaders/Copy.cfg"),true) );
	*/
	m_Frames.Init( 0, NULL);

    max_luminance = 0;//100
    min_luminance = 100;//0

    max_contrast = 0;
    min_contrast = 0;

    for( int L=0; L<100*100*3; L++) {
        LevelDiagram[L] = 0;
    }

	m_bInitialized = true;

	return Init();
}

MOboolean  moTextureBuffer::Finish() {
	for(MOuint i=0; i<m_Frames.Count(); i++) {
		moTextureMemory* pTextureMemory = m_Frames[i];
		if (pTextureMemory)
			pTextureMemory->Finish();
		delete pTextureMemory;
	}
	m_Frames.Empty();

	for(int j=0 ; j<100; j++) {
        if (m_pBufferLevels[j]!=NULL) {
            for(int k=0 ; k<10; k++) {
                m_pBufferLevels[j][k].Empty();
            }
            delete [] m_pBufferLevels[j];
        }
    }
    m_pBufferLevels = NULL;

    if (LevelDiagram) {
            delete [] LevelDiagram;
            LevelDiagram = NULL;
    }

	m_pResourceManager = NULL;
	return true;
}

MOboolean
moTextureBuffer::LoadCompleted() {
	return m_bLoadCompleted;
}

MOboolean moTextureBuffer::UpdateImages( MOint maxfiles ) {

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

					//for(MOuint b = 0; b<m_VideoBuffers.Count(); b++) {

						//moVideoBuffer* pVideoBuffer = m_VideoBuffers[b];

						//if (pVideoBuffer)
                    //LoadImage(  , pImage, m_ActualImage );
                    MODebug2->Push( moText("moTextureBuffer::UpdateImages > Trying to load image:") +  (moText)pFile->GetCompletePath() );
                    if ( LoadImage( m_FolderName + moSlash + pFile->GetFileName() , pImage, m_ActualImage ) ) {
                        m_ImagesProcessed++;
                        if ( m_ActualImage == (m_pDirectory->GetFiles().Count()-2) ) {
                            MODebug2->Log( moText(" ####TEXTUREBUFFER LEVEL HISTOGRAM####"));
                            moText barra;
                            moText nivel;
                            barra = moText("###################################################################################");

                            for(int k=0; k<100; k++) {
                                nivel = barra;
                                nivel.Left( m_pBufferLevels[k][0].Count() );
                                MODebug2->Log( moText(" level:") + IntToStr(k) + (moText)nivel );
                            }
                        }
                    }

					//}
					FreeImage_Unload(pImage);
					pImage = NULL;

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


MOboolean
moTextureBuffer::LoadImage( moText p_ImageName, moBitmap* pImage, int indeximage  ) {

    MOboolean res = false;
    FIBITMAP* _pImage = (FIBITMAP*)pImage;
	FIBITMAP* pImageResult = NULL;
	FIBITMAP* pImageCropped = NULL;
	FIBITMAP* pImageScaled = NULL;
    /*
	if ( m_width!=FreeImage_GetWidth(_pImage) || m_height!=FreeImage_GetHeight(_pImage) ) {
		//CROP MODE
		pImageCropped = FreeImage_Copy( _pImage, m_XSource , m_YSource , m_XSource + m_SourceWidth , m_YSource+m_SourceHeight );
		pImageResult = pImageCropped;

	} else
	*/

	pImageResult = _pImage;

    int w, h;
	if ( ( FreeImage_GetWidth(_pImage) % 4 ) != 0 || ( FreeImage_GetHeight(_pImage) % 4) == 0 ) {

        w = FreeImage_GetWidth(_pImage) / 4;
        w = w * 4;

        h = FreeImage_GetHeight(_pImage) / 4;
        h = h * 4;

        pImageScaled = FreeImage_Rescale( pImageResult, w, h, FILTER_BICUBIC );
		if (pImageScaled) {
			//FreeImage_Unload(pImageResult);
			pImageResult = pImageScaled;
		}
	}



    if (!GLEW_ARB_texture_non_power_of_two) {
        w = FreeImage_GetWidth(pImageResult);
        h = FreeImage_GetHeight(pImageResult);

        int i;
        for (i = 1; i < w; i *= 2);
        w = i;

        for (i = 1; i < h; i *= 2);
        h = i;

        w = momin(512,w);
        h = momin(512,h);

      pImageScaled =   FreeImage_Rescale( pImageResult, w, h, FILTER_BICUBIC );
    if (pImageScaled) {
			//FreeImage_Unload(pImageResult);
			pImageResult = pImageScaled;
		}
    }

	FreeImage_AdjustContrast( pImageResult, 0 );
	FreeImage_AdjustBrightness( pImageResult, 0 );



	//RESCALE: NOTE NECESARRY HERE
	//quizas podamos definir un máximo para el tamaño tanto ancho como alto
	//o como proporción
	//forzar proporcion y esas cosas....
	/*
	if ( m_width != m_SourceWidth || m_height != m_SourceHeight ) {

		//FILTER_BOX Box, pulse, Fourier window, 1st order (constant) B-Spline
		//FILTER_BILINEAR Bilinear filter
		//FILTER_BSPLINE 4th order (cubic) B-Spline
		//FILTER_BICUBIC Mitchell and Netravali's two-param cubic filter
		//FILTER_CATMULLROM Catmull-Rom spline, Overhauser spline
		//FILTER_LANCZOS3
		pImageScaled = FreeImage_Rescale( pImageResult, m_width, m_height, FILTER_BICUBIC );
		if (pImageScaled) {
			FreeImage_Unload(pImageResult);
			pImageResult = pImageScaled;
		}
	}
	*/

    moTextureMemory* pTextureMemory = NULL;

    int idx = m_pResourceManager->GetTextureMan()->AddTexture( MO_TYPE_TEXTUREMEMORY, p_ImageName );

	if (idx>-1) {

	    pTextureMemory = (moTextureMemory*) m_pResourceManager->GetTextureMan()->GetTexture(idx);

        if (pTextureMemory) {
            if (pTextureMemory->BuildFromBitmap( pImageResult, m_BufferFormat )) {

                m_Frames.Add(pTextureMemory);

                int contrastlevel = pTextureMemory->GetContrast() / 2000;
                int luminancelevel = (int)((float)pTextureMemory->GetLuminance() / (float)2.56 ) - 1;

                MODebug2->Message( moText("moTextureBuffer::LoadImage success : ") + (moText)pTextureMemory->GetName());
                #ifdef _DEBUG
                MODebug2->Message( moText("moTextureBuffer::LoadImage success : ") + (moText)pTextureMemory->GetName()
                                + moText(" width:") + IntToStr(pTextureMemory->GetWidth())
                                + moText(" height:") + IntToStr(pTextureMemory->GetHeight())
                                + moText(" luminance:") + IntToStr(pTextureMemory->GetLuminance())
                                + moText(" luminancelevel:") + IntToStr(luminancelevel) +
                                + moText(" contrast:") + IntToStr(pTextureMemory->GetContrast())
                                + moText(" contrastlevel:") + IntToStr(contrastlevel) );
                #endif

                contrastlevel = 0;

                if (luminancelevel>=100) {
                        MODebug2->Error( moText("moTextureBuffer::LoadImage Error: luminance out of bound:")+ IntToStr(pTextureMemory->GetLuminance()) );
                } else {
                        if (max_luminance<luminancelevel) {
                            max_luminance = luminancelevel;
                        } else if (min_luminance>luminancelevel) {
                            min_luminance = luminancelevel;
                        }
                }
                if (contrastlevel>=10) {
                        MODebug2->Error( moText("moTextureBuffer::LoadImage Error: contrast out of bound:")+ IntToStr(pTextureMemory->GetContrast()) );
                }
                if (0<=luminancelevel && luminancelevel<100 && 0<=contrastlevel && contrastlevel<10) {

                    m_pBufferLevels[luminancelevel][contrastlevel].Add( pTextureMemory );

                    int icc = momin( m_pBufferLevels[luminancelevel][contrastlevel].Count(), 99 );
                    LevelDiagram[ (luminancelevel + icc*100)*3 ] = 250;
                    LevelDiagram[ (luminancelevel+1 + icc*100)*3 ] = 250;
                    LevelDiagram[ (luminancelevel+2 + icc*100)*3 ] = 250;
                }

                res = true;
            } else {
                res = false;
                MODebug2->Error( moText("moTextureBuffer::LoadImage Error loading image:")+(moText)pTextureMemory->GetName());
            }
        }
	}

	if (pImageResult!=_pImage)
		FreeImage_Unload(pImageResult);

	//m_nFrames = m_Frames.Count();
	//m_fFramesPerSecond = 25.0;

	return (res);
}
/*
MOboolean moTextureBuffer::LoadFromVideo(  moText p_moviefile ) {
	//
	return true;
}
*/

moTextureFrames&
moTextureBuffer::GetBufferPatterns( moTexture* p_ImageReference, int x, int y, int width, int height ) {

    int index = 0;

    return m_pBufferPatterns[index];
}

moTextureFrames&
moTextureBuffer::GetBufferPatterns( const moTexture& p_ImageReference, int x, int y, int width, int height ) {

    int index = 0;

    return m_pBufferPatterns[index];
}


moTextureFrames&  moTextureBuffer::GetBufferLevels( int L, int C ) {

    if (0<=L && L<100 && 0<=C && C<10) {
        return m_pBufferLevels[L][C];
    } else {
        //devolvemos el oscuro
        return m_pBufferLevels[99][0];
    }

}


/*
*/
int moTextureBuffer::GetFrame( MOuint p_i ) {

	if ( p_i<m_Frames.Count()) {

		moTextureMemory* pTextureMemory = m_Frames[p_i];

		//GLId = pTextureMemory->GetReference();
		pTextureMemory->GetReference();
		return pTextureMemory->GetGLId();

    } else {
        MODebug2->Error(moText("moTextureBuffer::GetFrame Error: p_i out of range: p_i:") + IntToStr(p_i) + moText(" count:") + IntToStr(m_Frames.Count()) );
        return -1;
    }

}


void moTextureBuffer::ReleaseFrame( MOuint p_i ) {

	if ( p_i<m_Frames.Count()) {

		moTextureMemory* pTextureMemory = m_Frames[p_i];

		//GLId = pTextureMemory->GetReference();
		pTextureMemory->ReleaseReference();
    }
}

moTextureMemory* moTextureBuffer::GetTexture( MOuint p_i ) {
    if ( p_i<m_Frames.Count()) {

		moTextureMemory* pTextureMemory = m_Frames[p_i];
		return pTextureMemory;
    }
}


//===========================================
//
//				moTextureManager
//
//===========================================

moTextureManager::moTextureManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_TEXTURE );

	SetName("texturemanager");
  SetLabelName("texturemanager");

    // In your main program ...
    FreeImage_SetOutputMessage(FreeImageErrorHandler);

	m_glmanager = NULL;
	m_fbmanager = NULL;

	m_textures_buffers.Init(0,NULL);
}

moTextureManager::~moTextureManager()
{
	Finish();
}

MOboolean moTextureManager::Init()
{
	if (m_pResourceManager){
		m_glmanager = m_pResourceManager->GetGLMan();
		m_fbmanager = m_pResourceManager->GetFBMan();
	} else return false;

	m_textures_array.Init(0, NULL);

	m_preview_texture_idx = AddTexture("preview_texture", 1024, 1024);
	//AddTexture("texture_clip1", 1024, 768);
	//AddTexture("texture_clip2", 1024, 768);
	//AddTexture("texture_clip3", 1024, 768);

	int m_id_default = AddTexture( "default", 256, 256);
	moTexture* DefaultTexture =  GetTexture(m_id_default);
    if (DefaultTexture) DefaultTexture->BuildFromFile(
        moText(DATADIR) +
        moSlash +
        moText("icons") +
        moSlash +
        moText("moldeologo.png")
    );

	return (m_glmanager && m_fbmanager);
}

MOboolean moTextureManager::Finish()
{
	m_textures_array.Finish();

	m_glmanager = NULL;
	m_fbmanager = NULL;

	return true;
}

MOint moTextureManager::GetTextureMOId(moParam *param, MOboolean p_create_tex)
{
	MOint moid = GetTextureMOId(param->GetValue().GetSubValue(0).Text(), false);
	if (-1 < moid) return moid;
	else
		if (p_create_tex) return AddTexture(param);
		else return -1;
}

MOint moTextureManager::GetTextureMOId(moValue *value, MOboolean p_create_tex)
{
	MOint moid = GetTextureMOId( value->GetSubValue(0).Text(), false);
	if (-1 < moid) return moid;
	else
		if (p_create_tex) return AddTexture(value);
		else return -1;
}

MOint moTextureManager::GetTextureMOId(moText p_name, MOboolean p_create_tex)
{
	moTexture* ptex;
	if (p_name.Trim() == moText("")) return -1;
	for (MOuint i = 0; i < m_textures_array.Count(); i++)
	{
		ptex = m_textures_array[i];
		if ((ptex != NULL) && (!stricmp(ptex->GetName(), p_name))) return i;
	}
	if (p_create_tex) return AddTexture(p_name);
	else return -1;
}

MOint moTextureManager::GetTextureBuffer( moText p_foldername, MOboolean p_create_tex, moText p_bufferformat ) {

    moTextureBuffer* ptexbuffer;
	if (p_foldername.Trim() == moText("")) return -1;
	for (MOuint i = 0; i < m_textures_buffers.Count(); i++)
	{
		ptexbuffer = m_textures_buffers[i];
		if ((ptexbuffer != NULL) && (!stricmp(ptexbuffer->GetName(), p_foldername))) return i;
	}
	if (p_create_tex) return AddTextureBuffer( p_foldername, p_bufferformat );
	else return -1;

}


moTextureBuffer* moTextureManager::GetTextureBuffer( int idx ) {
    if (0<=idx && idx<(int)m_textures_array.Count()) {
        return m_textures_buffers[idx];
    } else return NULL;
}


MOint moTextureManager::GetTextureMOId(MOuint p_glid)
{
	moTexture* ptex;
	for (MOuint i = 0; i < m_textures_array.Count(); i++)
	{
		ptex = m_textures_array[i];
		if ((ptex != NULL) && (ptex->GetGLId() == p_glid)) return i;
	}
	return -1;
}

MOint moTextureManager::AddTexture(MOuint p_type, moText p_name, moTexParam p_tex_param)
{
	moTexture* ptex = CreateTexture(p_type, p_name, p_tex_param);
	if (ptex != NULL)
	{
		m_textures_array.Add(ptex);
		ptex->SetMOId(m_textures_array.Count() - 1);
		return ptex->GetMOId();
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moParam* p_param)
{
    moValue* pValue = &p_param->GetValue();
	return AddTexture( pValue );
}

MOint moTextureManager::AddTexture(moValue* p_value)
{
	moText name = p_value->GetSubValue(0).Text();
	MOuint type = GetTypeForFile(name);
	moTextureMultiple* ptex_mult;
	moMovie* ptex_mov;
	moVideoBuffer* ptex_videobuffer;
	moCircularVideoBuffer* ptex_circularvideobuffer;

	moTexture* ptex = CreateTexture(type, name);
	if (ptex != NULL)
	{
		MOboolean res = false;
		if (type == MO_TYPE_TEXTURE) res = ptex->Load( p_value );
		else if (type == MO_TYPE_TEXTURE_MULTIPLE)
		{
			ptex_mult = (moTextureMultiple*)ptex;
			res = CreateMultipleTex(ptex_mult, p_value);
		}
		else if (type == MO_TYPE_MOVIE)
		{
			ptex_mov = (moMovie*)ptex;
			res = ptex_mov->Load(p_value);
		}
		else if (type == MO_TYPE_VIDEOBUFFER)
		{
			ptex_videobuffer = (moVideoBuffer*)ptex;
			res = ptex_videobuffer->Load(p_value);
		}
		else if (type == MO_TYPE_CIRCULARVIDEOBUFFER)
		{
			ptex_circularvideobuffer = (moCircularVideoBuffer*)ptex;
			res = ptex_circularvideobuffer->Load(p_value);
		}
		if (res)
		{
			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();
		}
		else
		{
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moText p_name, MOuint p_width, MOuint p_height, moTexParam p_tex_param)
{
	moTexture* ptex = CreateTexture(MO_TYPE_TEXTURE, p_name, p_tex_param);
	if (ptex != NULL)
	{
		MOboolean res = ptex->BuildEmpty(p_width, p_height);
		if (res)
		{
			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();
		}
		else
		{
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moText p_name, MOuint p_width, MOuint p_height, GLvoid* p_buffer, GLenum p_pixel_format, GLenum p_pixel_type, moTexParam p_tex_param)
{
	moTexture* ptex = CreateTexture(MO_TYPE_TEXTURE, p_name, p_tex_param);
	if (ptex != NULL)
	{
		MOboolean res = ptex->BuildFromBuffer(p_width, p_height, p_buffer, p_pixel_format, p_pixel_type);
		if (res)
		{
			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();
		}
		else
		{
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOint moTextureManager::AddTexture(moText p_filename)
{
	moText name = p_filename;
	MOuint type = GetTypeForFile(name);
	moMovie* ptex_mov;

	moTexture* ptex = CreateTexture(type, name);
	p_filename = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)p_filename;
	if (ptex != NULL)
	{
		MOboolean res = false;
		if (type == MO_TYPE_TEXTURE) {

		  res = ptex->BuildFromFile(p_filename);

		} else if (type == MO_TYPE_MOVIE)	{

			ptex_mov = (moMovie*)ptex;
			res = ptex_mov->LoadMovieFile(p_filename);

		}


		if (res) {

			m_textures_array.Add(ptex);
			ptex->SetMOId(m_textures_array.Count() - 1);
			return ptex->GetMOId();

		}	else {
			delete ptex;
			return -1;
		}
	}
	else return -1;
}

MOint moTextureManager::AddTextureBuffer( moText p_foldername, moText p_bufferformat ) {

    moTextureBuffer* ptexbuffer = new moTextureBuffer();
    if (ptexbuffer) {

        if ( ptexbuffer->Init( p_foldername, p_bufferformat, m_pResourceManager ) ) {
            m_textures_buffers.Add( ptexbuffer );
            MODebug2->Push( moText("TextureBuffer created:") + (moText)p_foldername + moText(" idx:") +IntToStr((m_textures_buffers.Count() - 1)) );
            return (m_textures_buffers.Count() - 1);
        } else {
            MODebug2->Error( moText("moTextureManager::AddTextureBuffer Error: Initializing texturebuffer: ") + (moText)p_foldername );
        }

    } else {
        MODebug2->Error( moText("moTextureManager::AddTextureBuffer Error: Creating texturebuffer: ") + (moText)p_foldername );
    }

    return -1;

}

MOboolean moTextureManager::DeleteTexture(MOuint p_moid)
{
	if (ValidTexture(p_moid))
	{
		moTexture* ptex = m_textures_array[p_moid];
		if (ptex)
			delete ptex;
		m_textures_array.Remove(p_moid);
		for (MOuint i = p_moid; i < m_textures_array.Count(); i++) m_textures_array[i]->SetMOId(i);
		return true;
	}
	else return false;
}

MOuint moTextureManager::GetTypeForFile(moText p_filename)
{
	moText extension = p_filename;
	moText begin = p_filename;
	begin.Left(7);
	extension.Right(3);

	if (!stricmp(extension,"tga") || !stricmp(extension,"jpg") || !stricmp(extension,"png") || !stricmp(extension,"bmp") || !stricmp(extension,"tif") || !stricmp(extension,"xpm"))
		return MO_TYPE_TEXTURE;
	if (!stricmp(begin,"multiple"))
		return MO_TYPE_TEXTURE_MULTIPLE;
	if (!stricmp(extension,"mkv") || !stricmp(extension,"ogg") || !stricmp(extension,"avi") || !stricmp(extension,"mov") || !stricmp(extension,"mpg") || !stricmp(extension,"vob") || !stricmp(extension,"m2v") || !stricmp(extension,"mp4"))
		return MO_TYPE_MOVIE;

	return MO_TYPE_TEXTURE;
}

moTexture* moTextureManager::CreateTexture(MOuint p_type, moText p_name, moTexParam p_param)
{
	moTexture* ptex;
	moTextureMemory* ptex_mem;
	moTextureMultiple* ptex_mult;
	moMovie* ptex_movie;
	moVideoBuffer* ptex_videobuffer;
	moCircularVideoBuffer* ptex_circularvideobuffer;

	MOuint new_moid = m_textures_array.Count();

  if ( p_name.Trim().Length() == 0 ) {
    ///atencion una textura sin nombre....
    MODebug2->Error( moText("tratando de crear una textura sin nombre") );
    return NULL;
  }

	if (p_type == MO_TYPE_TEXTURE)
	{
		ptex = new moTexture;
	}
	else if (p_type == MO_TYPE_TEXTUREMEMORY)
	{
	    ptex_mem = new moTextureMemory;
		ptex = (moTexture*)ptex_mem;
	}
	else if (p_type == MO_TYPE_TEXTURE_MULTIPLE)
	{
		ptex_mult = new moTextureMultiple;
		ptex = (moTexture*)ptex_mult;
	}
	else if (p_type == MO_TYPE_MOVIE)
	{
		ptex_movie = new moMovie;
		ptex = (moTexture*)ptex_movie;
	}
	else if (p_type == MO_TYPE_VIDEOBUFFER)
	{
		ptex_videobuffer = new moVideoBuffer;
		ptex = (moTexture*)ptex_videobuffer;
	}
	else if (p_type == MO_TYPE_CIRCULARVIDEOBUFFER)
	{
		ptex_circularvideobuffer = new moCircularVideoBuffer;
		ptex = (moTexture*)ptex_circularvideobuffer;
	}
	else return NULL;

	ptex->Init(p_name, new_moid, m_pResourceManager, p_param);
	return ptex;
}

MOboolean moTextureManager::CreateMultipleTex(moTextureMultiple* p_tex_mult, moParam* p_param)
{
    moValue* pValue = &p_param->GetValue();
	return CreateMultipleTex(p_tex_mult, pValue);
}

MOboolean moTextureManager::CreateMultipleTex(moTextureMultiple* p_tex_mult, moValue* p_value)
{
	moText texfn;
	MOint moid;
	MOuint nfiles = p_value->GetSubValueCount() - 1;

	for (MOuint i = 0; i < nfiles; i++)
	{
		texfn = p_value->GetSubValue(i + 1).Text();
		moid = GetTextureMOId(texfn, true);
		if (-1 < moid) p_tex_mult->AddFrame(GetTexture(moid));
	}

	return 0 < p_tex_mult->GetTextureCount();
}

MOint moTextureManager::GetType(MOuint p_moid) {
    return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetType() : MO_UNDEFINED;

}

MOuint moTextureManager::GetGLId(MOuint p_moid) {
    return ValidTexture(p_moid) ? m_textures_array[p_moid]->GetGLId() : 0;
}


void moTextureManager::Update(moEventList *Events)
{

	//if ( GetId() == MO_MOLDEOOBJECT_UNDEFINED_ID ) return;

	/// Texture buffer loading routine, 10 x ,,,,
	for(MOuint k=0; k<m_textures_buffers.Count(); k++) {


		moTextureBuffer*	pTextureBuffer = m_textures_buffers[k];

		if (pTextureBuffer && !pTextureBuffer->LoadCompleted()) {

			MODebug2->Push( pTextureBuffer->GetName() + moText(":") + IntToStr(pTextureBuffer->GetImagesProcessed() ));
			pTextureBuffer->UpdateImages( 1 );
		}


	}

}

