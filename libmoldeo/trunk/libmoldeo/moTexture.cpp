/*******************************************************************************

                                moTexture.cpp

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

#include <moTexture.h>
#include <FreeImage.h>
#include <moFileManager.h>
#include <moDataManager.h>

#include "moArray.h"
moDefineDynamicArray(moTextureArray)

//===========================================
//
//				moTexture
//
//===========================================

moTexture::moTexture()
{
	m_pFile = NULL;
	m_bBuildedFromFile = false;
	m_type = MO_TYPE_TEXTURE;
	SetName(moText(""));
	m_moid = -1;
	m_glid = 0;

	m_width = 0;
    m_height = 0;
	m_components = 0;
	m_max_coord_s = 0.0;
	m_max_coord_t = 0.0;

	m_pResourceManager = NULL;
	m_gl = NULL;
	m_pDataMan = NULL;
	m_pFileMan = NULL;
	m_fbo = NULL;
	m_fbo_attach_point = MO_UNDEFINED;

	m_param = MODefTex2DParams;

    Luminance = -1;
    Contrast = -1;
}

moTexture::~moTexture()
{
	Finish();
}

MOboolean moTexture::Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param)
{
	m_pFile = NULL;
	SetName(p_name);
	m_moid = p_moid;
	if (p_res) {
		m_pResourceManager = p_res;
		m_gl = p_res->GetGLMan();
		m_pDataMan = p_res->GetDataMan();
		m_pFileMan = p_res->GetFileMan();
	}
	m_param = p_param;
	m_bInitialized = (m_gl!=NULL) && (m_pDataMan!=NULL) && (m_pFileMan!=NULL);
	return m_bInitialized;
}

MOboolean moTexture::Finish()
{
    if ((m_type != MO_TYPE_TEXTURE_MULTIPLE) && (0 < m_glid))
	{
		glDeleteTextures(1, &m_glid);
		m_glid = 0;
	}
	m_bInitialized = false;
	return true;
}


MOboolean moTexture::BuildEmpty(MOuint p_width, MOuint p_height)
{
    int ii;
	/*if ( 0 < m_glid )	{
	    //ii = m_glid;
		glDeleteTextures(1, &m_glid);
		m_glid = 0;
		//m_glid = ii;
	}*/

	if (m_glid<=0) {
        glGenTextures(1, &m_glid);
	}


	CalculateSize(p_width, p_height);
	return Build();
}

MOboolean moTexture::BuildFromBuffer(MOuint p_width, MOuint p_height, GLvoid* p_buffer, GLenum p_format, GLenum p_type)
{
	BuildEmpty(p_width, p_height);
	return SetBuffer(p_width, p_height, p_buffer, p_format, p_type);
}

MOboolean moTexture::BuildFromFile(moText p_filename)
{
	MOboolean res;
	moFile*	pFile = NULL;

	//check for file
	if ( m_pFileMan->Load(p_filename)) {
		pFile = m_pFileMan->GetFile(p_filename);
	} else return false;
	if ( pFile==NULL ) return false;
	m_pFile = pFile;

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *m_pImage = NULL;

	if ( m_pFile->GetType()==MO_FILETYPE_LOCAL && m_pFile->Exists()) {

		fif = FreeImage_GetFileType( m_pFile->GetCompletePath(), 0);

		if( fif == FIF_UNKNOWN ) {
			// try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(m_pFile->GetCompletePath());
		}

		if( (fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif) ) {
			// ok, let's load the file
			m_pImage = FreeImage_Load( fif, m_pFile->GetCompletePath(), 0);
		}
	} else if ( m_pFile->IsRemote() ) {
		//set an internal flag for texture to always check if file has finished downloading
		//and build
		if ( m_pFile->GetStatus()==MO_FILESTATUS_READY) {
			//we check from filename
			fif = FreeImage_GetFIFFromFilename(m_pFile->GetCompletePath());
			if( (fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif) ) {
				// ok, let's load the file
				m_pImage = FreeImage_LoadFromMemory( fif, (FIMEMORY*)m_pFile->GetData(), 0);
			}
		}
	}

	if (m_pImage != NULL)
	{
	    MOuint p_width;
	    MOuint p_height;
	    FIBITMAP* pImageScaled = NULL;

	    p_width = FreeImage_GetWidth(m_pImage);
        p_height = FreeImage_GetHeight(m_pImage);

        if ( ( FreeImage_GetWidth(m_pImage) % 4 ) != 0 || ( FreeImage_GetHeight(m_pImage) % 4) == 0 ) {

            p_width= FreeImage_GetWidth(m_pImage) / 4;
            p_width=p_width * 4;

            p_height = FreeImage_GetHeight(m_pImage) / 4;
            p_height = p_height* 4;

            pImageScaled = FreeImage_Rescale( m_pImage, p_width, p_height, FILTER_BICUBIC );
            if (pImageScaled) {
                FreeImage_Unload(m_pImage);
                m_pImage = pImageScaled;
            }
        }

		MOuint p_format;

		m_param.target = GL_TEXTURE_2D;
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
				if (FreeImage_GetRedMask(m_pImage) == 0xFF0000) p_format = GL_BGR;
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

		BuildEmpty(p_width, p_height);
		FlipBufferVert((MOubyte *)FreeImage_GetBits(m_pImage), FreeImage_GetBPP(m_pImage) / 8 );
		res = SetBuffer(p_width, p_height, FreeImage_GetBits(m_pImage), p_format);

		FreeImage_Unload(m_pImage);
	}
	else {
		if (MODebug2 != NULL) MODebug2->Error(moText("Error at image load: ") + (moText)p_filename);
		res = false;
	}

    m_bBuildedFromFile = res;

	return res;
}

MOboolean moTexture::SupportedFile(moText p_filename)
{
	moText extension = p_filename;
	extension.Right(3);
	return (!stricmp(extension,"tga") ||
		!stricmp(extension,"jpg") ||
		!stricmp(extension,"png") ||
		!stricmp(extension,"gif") ||
		!stricmp(extension,"bmp") ||
		!stricmp(extension,"xpm") ||
		!stricmp(extension,"ppm"));
}

MOboolean moTexture::Load( moParam* p_param )
{
	moValue* pValue = &p_param->GetValue();
	return Load( pValue );
}

MOboolean moTexture::Load( moValue* p_value )
{
	SetName( p_value->GetSubValue(0).Text() );
	moText namefull = m_pDataMan->GetDataPath() + moSlash + (moText)m_name;
	return BuildFromFile(namefull);
}

MOboolean moTexture::SetBuffer(GLvoid* p_buffer, GLenum p_format, GLenum p_type)
{
	return SetBuffer(m_width, m_height, p_buffer, p_format, p_type);
}

MOboolean moTexture::SetBuffer(MOuint p_width, MOuint p_height, GLvoid* p_buffer, GLenum p_format, GLenum p_type)
{
	glBindTexture(m_param.target, m_glid);

	// Aqui hay que destruir la textura si los nuevos alto y anchos son diferentes de los actuales!!!!

	if ((m_gl != NULL) && m_gl->MipMapTexture(m_param.min_filter))
		gluBuild2DMipmaps(m_param.target, m_param.internal_format, p_width, p_height, p_format, p_type, p_buffer);
	else
        glTexSubImage2D(m_param.target, 0, 0, 0, p_width, p_height, p_format, p_type, p_buffer);
    glBindTexture(m_param.target, 0);

	//if (m_gl != NULL) return !m_gl->CheckErrors("copying buffer to texture");
	//else return true;

	return true;
}

MOboolean moTexture::GetBuffer(GLvoid* p_buffer, GLenum p_format, GLenum p_type)
{
	glBindTexture(m_param.target, m_glid);
	glGetTexImage(m_param.target, 0, p_format, p_type, p_buffer);
	glBindTexture(m_param.target, 0);

	//if (m_gl != NULL) return !m_gl->CheckErrors("copying texture to buffer");
	//else return true;
	return true;
}

bool moTexture::CalculateLuminanceAndConstrast( int x0, int y0, int x1, int y1 ) {

    //FREE_IMAGE_FORMAT fif;
    FIBITMAP* fbitmap = NULL;
    FIBITMAP* fbitmapcopy = NULL;
    //configint options = 0;
    BYTE* tempbuffer = NULL;

    if (GetHeight()==0 || GetWidth()==0) return false;

    if (x0 == 0 && y0 == 0 && x1 == 0 && y1 == 0) {

        tempbuffer = new BYTE [ GetHeight() * GetWidth() * 4 ];
        if (tempbuffer==NULL) return false;
        if (!GetBuffer( tempbuffer, GL_RGBA, GL_UNSIGNED_BYTE ) ) return false;
        int bpp = 32;
        int pitch = 4 * GetWidth();
        fbitmap = FreeImage_ConvertFromRawBits( (BYTE*)tempbuffer, GetWidth(), GetHeight(), pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false );
        if (tempbuffer)
                delete [] tempbuffer;

    } else {

        int ww = (x1 - x0 - 1);
        int hh = (y1 - y0 - 1);
        if (ww>0 && hh>0) {
            tempbuffer = new BYTE [ GetHeight() * GetWidth() * 4 ];
            if (tempbuffer==NULL) return false;
            //glReadPixels( x0, y0, ww, hh, GL_RGBA, GL_UNSIGNED_BYTE, tempbuffer);
            if (!GetBuffer( tempbuffer, GL_RGBA, GL_UNSIGNED_BYTE ) ) return false;
            int bpp = 32;
            int pitch = 4 * GetWidth();
            fbitmap = FreeImage_ConvertFromRawBits( (BYTE*)tempbuffer, GetWidth(), GetHeight(), pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false );
            fbitmapcopy = FreeImage_Copy( fbitmap, x0, y0, x1, y1 );
            FreeImage_Unload(fbitmap);
            fbitmap = fbitmapcopy;
            if (tempbuffer)
                delete [] tempbuffer;

        } else {
            MODebug2->Error(moText("moTexture::CalculateLuminanceAndConstrast ") + moText("freeimage problem"));
            return false;
        }

    }

    if (fbitmap && FreeImage_GetHistogram( (FIBITMAP *)fbitmap, (DWORD*)Histogram, FICC_BLACK )) {

            ///calculate luminance
            Luminance = 0;
            long Total = 0;

            for(int ih=0; ih <256; ih++) {
                Total+= Histogram[ih];
                Luminance+= Histogram[ih]*ih;

            }

            Luminance = Luminance / Total;

            ///Calculating contrast difference or luminance variance
            int Group[5];
            int Totals[5];

            Group[0] = 0;
            Group[1] = 0;
            Group[2] = 0;
            Group[3] = 0;
            Group[4] = 0;

            Totals[0] = 0;
            Totals[1] = 0;
            Totals[2] = 0;
            Totals[3] = 0;
            Totals[4] = 0;


            int zone = 0;
            long dif = 0;
            for(int ih=0; ih <256; ih++) {
                zone = ih / 52;
                dif = ( Histogram[ih]*(ih - Luminance)*(ih - Luminance));
                Group[zone]+= dif;
                Totals[zone]+= Histogram[ih];
            }

            Contrast = 0;
            for(int z=0;z<5;z++) {
                if (Totals[z]>0) Contrast+= Group[z] / Totals[z];
            }
            Contrast = Contrast / 5;

            MODebug2->Push( moText("moTexture::CalculateLuminanceAndConstrast Histogram OK"));

    } else MODebug2->Error( moText("moTexture::CalculateLuminanceAndConstrast Histogram error"));

    if (fbitmap) FreeImage_Unload( fbitmap );

}

MOuint moTexture::SetFBOandAttachPoint(moFBO* p_fbo)
{
	MOuint AttachPt;
	SetFBO(p_fbo);
	p_fbo->AddTexture(GetWidth(), GetHeight(), GetTexParam(), GetGLId(), AttachPt);
	SetFBOAttachPoint(AttachPt);
	return AttachPt;
}

MOuint moTexture::GetDataWidth()
{
	if ((m_gl != NULL) && m_gl->RectTexture(m_param.target)) return m_width;
	else return (MOuint)(m_max_coord_s * m_width);
}

MOuint moTexture::GetDataHeight()
{
	if ((m_gl != NULL) && m_gl->RectTexture(m_param.target)) return m_height;
	else return (MOuint)(m_max_coord_t * m_height);
}

void moTexture::FlipBufferVert(MOubyte *pBuffer, MOint p_depth)
{
	MOubyte *top;
	MOubyte *bottom;
	MOubyte *tmp_buffer;
	int lineWidth = m_width * p_depth;

	tmp_buffer = new MOubyte[m_width * p_depth];

	top =(MOubyte*)pBuffer;
	bottom =(MOubyte*)((BYTE*)pBuffer + lineWidth*(m_height - 1));
	for(MOuint i = 0; i < (m_height >> 1); i++)
	{
		memcpy(tmp_buffer, top, lineWidth);
		memcpy(top, bottom, lineWidth);
		memcpy(bottom, tmp_buffer, lineWidth);

		top =(MOubyte*)((BYTE*) top + lineWidth);
		bottom =(MOubyte*)((BYTE*) bottom - lineWidth);
	}
}

void moTexture::CopyFromTex(moTexture* p_src_tex, MOboolean p_copy_glid, MOboolean p_copy_moid, MOboolean p_copy_type, MOboolean p_copy_name)
{
	if (p_src_tex != NULL)
	{
	    m_gl = p_src_tex->m_gl;

	    m_fbo = p_src_tex->m_fbo;
	    m_fbo_attach_point = p_src_tex->m_fbo_attach_point;

	    if (p_copy_type) m_type = p_src_tex->m_type;
	    if (p_copy_moid) m_moid = p_src_tex->m_moid;
	    if (p_copy_glid) m_glid = p_src_tex->m_glid;
	    if (p_copy_name) m_name = p_src_tex->m_name;

	    m_param = p_src_tex->m_param;
	    m_width = p_src_tex->m_width;
	    m_height = p_src_tex->m_height;
	    m_components = p_src_tex->m_components;
	    m_max_coord_s = p_src_tex->m_max_coord_s;
	    m_max_coord_t = p_src_tex->m_max_coord_t;
	}
}

moTexture &moTexture::operator = (const moTexture &p_src_tex)
{
	m_gl = p_src_tex.m_gl;

	m_fbo = p_src_tex.m_fbo;
	m_fbo_attach_point = p_src_tex.m_fbo_attach_point;

	m_type = p_src_tex.m_type;
	m_moid = p_src_tex.m_moid;
	m_glid = p_src_tex.m_glid;
	m_name = p_src_tex.m_name;

	m_param = p_src_tex.m_param;
	m_width = p_src_tex.m_width;
	m_height = p_src_tex.m_height;
	m_components = p_src_tex.m_components;
	m_max_coord_s = p_src_tex.m_max_coord_s;
	m_max_coord_t = p_src_tex.m_max_coord_t;

	return *this;
}

void moTexture::SetParam()
{
	glTexParameteri(m_param.target, GL_TEXTURE_MIN_FILTER, m_param.min_filter);
	glTexParameteri(m_param.target, GL_TEXTURE_MAG_FILTER, m_param.mag_filter);
	glTexParameteri(m_param.target, GL_TEXTURE_WRAP_S, m_param.wrap_s);
	glTexParameteri(m_param.target,	GL_TEXTURE_WRAP_T, m_param.wrap_t);
}

void moTexture::CalculateSize(MOuint p_width, MOuint p_height)
{
	if ((m_gl != NULL) && m_gl->RectTexture(m_param.target))
	{
		m_width = p_width;
		m_height = p_height;
		m_max_coord_s = p_width;
		m_max_coord_t = p_height;
	}/*
	else
	{
		m_width = p_width;
		m_height = p_height;
		m_max_coord_s = 1.0;
		m_max_coord_t = 1.0;
	}*/
	else if (!GLEW_ARB_texture_non_power_of_two)
	{
        m_width = NextPowerOf2(p_width);
		m_height = NextPowerOf2(p_height);
		m_max_coord_s = (float)p_width / (float)m_width;
		m_max_coord_t = (float)p_height / (float)m_height;
    }
	else
	{
		m_width = p_width;
		m_height = p_height;
		m_max_coord_s = 1.0;
		m_max_coord_t = 1.0;

	}

}

MOuint moTexture::NextPowerOf2(MOuint p_seed)
{
	MOuint i;
	for (i = 1; i < p_seed; i *= 2);
	return i;
}

MOboolean moTexture::Build()
{
	glBindTexture(m_param.target, m_glid);
	SetParam();
	glTexImage2D(m_param.target, 0, m_param.internal_format, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGetTexLevelParameteriv(m_param.target, 0, GL_TEXTURE_COMPONENTS, &m_components);
	//if (m_gl != NULL) return !m_gl->CheckErrors("texture build");
	//else return true;
	return true;
}


moText  moTexture::CreateThumbnail( moText p_bufferformat, int w, int h, moText newfilename ) {

    moText thumbnailfilename;

//    FIMEMORY* hmem = NULL;

    if ( newfilename==moText("") ) {

        //old version
        //moThm
        moFile SrcFile( m_name );
        thumbnailfilename = (moText)m_pDataMan->GetDataPath() + moSlash + SrcFile.GetFileName();

        /*
        // new version:
        moFile SrcFile( m_name );
        moDirectory Dir( (moText)m_pDataMan->GetDataPath() + moThm );
        if (  ! Dir.Exists() ) {

        }
        thumbnailfilename = (moText)m_pDataMan->GetDataPath() + moSlash + (moText)m_name + moText(".thm");
        //thumbnailfilename = (moText)m_pDataMan->GetDataPath() + moThm + moSlash + SrcFile.GetFileName() + moText(".thm");
        */


    } else {
        thumbnailfilename = newfilename;
    }

    FREE_IMAGE_FORMAT fif;
    FIBITMAP* fbitmap = NULL;
    int options = 0;

    BYTE* tempbuffer = NULL;

    if (GetHeight()==0 || GetWidth()==0)
        return moText("");

    tempbuffer = new BYTE [ GetWidth() * GetHeight() * 3 ];

    if (tempbuffer==NULL)
        return moText("");

    if (!GetBuffer( tempbuffer, GL_RGB, GL_UNSIGNED_BYTE ) )
        return moText("");

    FILE* fp;

/*
    fp = fopen( thumbnailfilename ,"wb" );
    fwrite( tempbuffer, sizeof(BYTE), GetWidth() * GetHeight() * 3 , fp );
    fclose(fp);
    */

    //return moText("");

    int bpp = 24;
    int pitch = 3 * GetWidth();


    fbitmap = FreeImage_ConvertFromRawBits( (BYTE*)tempbuffer, GetWidth(), GetHeight(), pitch, bpp, 0x0000FF, 0x00FF00, 0xFF0000, false );

    //unsigned width = FreeImage_GetWidth(fbitmap);
    //unsigned height = FreeImage_GetHeight(fbitmap);
    //unsigned ppitch = FreeImage_GetPitch(fbitmap);
    //FREE_IMAGE_TYPE image_type = FreeImage_GetImageType( fbitmap );
    //BYTE *bits = (BYTE*)FreeImage_GetBits(fbitmap);
/*
    FILE* fp = fopen( thumbnailfilename ,"wb" );
    fwrite( FreeImage_, sizeof(BYTE), m_width * m_height * 3 , fp );
    fclose(fp);
    */

    //FIBITMAP* fbitmap2 = NULL;
    fbitmap = FreeImage_Rescale( fbitmap, w, h, FILTER_BICUBIC );

    if ( p_bufferformat == moText("PNG")) {
        fif = FIF_PNG;
        options = PNG_DEFAULT;
        thumbnailfilename+= moText(".png");
    } else
    if ( p_bufferformat == moText("JPG")) {
        fif = FIF_JPEG;
        options = JPEG_QUALITYNORMAL;
        thumbnailfilename+= moText(".jpg");
    } else if ( p_bufferformat == moText("JPGSUPERB") ) {
        fif = FIF_JPEG;
        options = JPEG_QUALITYSUPERB;
        thumbnailfilename+= moText(".jpg");
    } else if ( p_bufferformat == moText("JPGBAD") ) {
        fif = FIF_JPEG;
        options = JPEG_QUALITYBAD;
        thumbnailfilename+= moText(".jpg");
    } else if ( p_bufferformat == moText("JPGAVERAGE") ) {
        fif = FIF_JPEG;
        options = JPEG_QUALITYAVERAGE;
        thumbnailfilename+= moText(".jpg");
    } else if ( p_bufferformat == moText("JPGGOOD") ) {
        fif = FIF_JPEG;
        options = JPEG_QUALITYGOOD;
        thumbnailfilename+= moText(".jpg");
    } else if ( p_bufferformat == moText("TGA") ) {
        fif = FIF_TARGA;
        options = 0;
        thumbnailfilename+= moText(".tga");
    }

    FreeImage_Save( fif, fbitmap, thumbnailfilename, options );

    if (tempbuffer)
        delete [] tempbuffer;

    if (fbitmap) {
            FreeImage_Unload( fbitmap );
    }

    //return ( moText("width:") + IntToStr(width) + moText("height:") + IntToStr(height) + moText("pitch") + IntToStr(ppitch) + moText("fif") + IntToStr(fif) + moText("oo") + IntToStr((int)fbitmap)  );

    return thumbnailfilename;

}


//===========================================
//
//				moTextureMemory
//
//===========================================

moTextureMemory::moTextureMemory() : moTexture() {
    reference_counter = 0;
    m_SizeInMemory = 0;
    m_bBitmapInMemory = false;
    hmem = NULL;
    fif = 0;
    m_BufferFormat = moText("JPG");
    Luminance = -1;
    Contrast = -1;

    int i = 0;
    for( i = 0; i<256; i++) Histogram[i] = 0;

}

moTextureMemory::~moTextureMemory() {

}

MOboolean moTextureMemory::Init( moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param ) {

    moTexture::Init( p_name, p_moid, p_res, p_param );

}

MOboolean moTextureMemory::Init( moText p_name, MOuint p_moid, moResourceManager* p_res, moText bufferformat, moBitmap* pImageResult, moTexParam p_param ) {

    FIBITMAP* _pImageResult = (FIBITMAP*)pImageResult;

    m_BufferFormat = bufferformat;

    if (pImageResult) LoadFromBitmap(pImageResult);

    return moTexture::Init( p_name, p_moid, p_res, p_param );
}

MOboolean moTextureMemory::LoadFromBitmap( moBitmap* p_bitmap ) {

    if ( m_bBitmapInMemory && hmem!=NULL ) {
        ///if there were already something , delete it! size may be different now!!!
        reference_counter = 1;
        ReleaseReference();
        ///close memory
        FreeImage_CloseMemory((FIMEMORY*)hmem);
        hmem = NULL;
    }

    if (!hmem) {
		hmem = FreeImage_OpenMemory();
	}
	if (hmem) {

		if ( m_BufferFormat == moText("JPG")) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYNORMAL;
		} else if ( m_BufferFormat == moText("JPGSUPERB") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYSUPERB;
		} else if ( m_BufferFormat == moText("JPGBAD") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYBAD;
		} else if ( m_BufferFormat == moText("JPGAVERAGE") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYAVERAGE;
		} else if ( m_BufferFormat == moText("JPGGOOD") ) {
			fif = FIF_JPEG;
			options = JPEG_QUALITYGOOD;
		} else if ( m_BufferFormat == moText("TGA") ) {
			fif = FIF_TARGA;
			options = 0;
		} else if ( m_BufferFormat == moText("PNG") ) {
			fif = FIF_PNG;
			options = 0;
		} else if ( m_BufferFormat == moText("XPM") ) {
			fif = FIF_XPM;
			options = 0;
		} else if ( m_BufferFormat == moText("RAW") ) {
		    fif = FIF_PPMRAW;
		    options = 0;
        }
		//syntax: FreeImage_SaveToMemory(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, FIMEMORY *stream, int flags FI_DEFAULT(0));
		if ( FreeImage_SaveToMemory( (FREE_IMAGE_FORMAT)fif, (FIBITMAP *)p_bitmap, (FIMEMORY*)hmem, options ) ) {
		    m_SizeInMemory = FreeImage_TellMemory((FIMEMORY*)hmem);
            m_bBitmapInMemory = true;

            m_width = FreeImage_GetWidth((FIBITMAP *)p_bitmap);
            m_height = FreeImage_GetHeight((FIBITMAP *)p_bitmap);

            ///Calculate luminance and contrast from histogram

            if (FreeImage_GetHistogram( (FIBITMAP *)p_bitmap, (DWORD*)Histogram, FICC_BLACK )) {

                ///calculate luminance
                Luminance = 0;
                long Total = 0;

                for(int ih=0; ih <256; ih++) {
                    Total+= Histogram[ih];
                    Luminance+= Histogram[ih]*ih;

                }

                Luminance = Luminance / Total;



                ///Calculating contrast difference or luminance variance
                int Group[5];
                int Totals[5];

                Group[0] = 0;
                Group[1] = 0;
                Group[2] = 0;
                Group[3] = 0;
                Group[4] = 0;

                Totals[0] = 0;
                Totals[1] = 0;
                Totals[2] = 0;
                Totals[3] = 0;
                Totals[4] = 0;


                int zone = 0;
                long dif = 0;
                for(int ih=0; ih <256; ih++) {
                    zone = ih / 52;
                    dif = ( Histogram[ih]*(ih - Luminance)*(ih - Luminance));
                    Group[zone]+= dif;
                    Totals[zone]+= Histogram[ih];
                }

                Contrast = 0;
                for(int z=0;z<5;z++) {
                    if (Totals[z]>0) Contrast+= Group[z] / Totals[z];
                }
                Contrast = Contrast / 5;

            } else MODebug2->Error( moText("moTextureMemory::LoadFromBitmap Histogram error"));



            //MODebug2->Push( moText("moTextureMemory::LoadFromBitmap success: hmem:") + IntToStr((int)hmem));
            //MODebug2->Push( moText("moTextureMemory::LoadFromBitmap luminance:") + IntToStr(Luminance) + moText(" contrast:") + IntToStr(Contrast) );
        } else m_bBitmapInMemory = false;

	}

	return m_bBitmapInMemory;

}

MOboolean moTextureMemory::BuildFromMemory() {

    if (hmem!=NULL && m_glid>0) {
        FIBITMAP *pImage;
        //FIMEMORY	*hmem;

        MOuint _format;

        //MOint FrameSize =
        FreeImage_TellMemory((FIMEMORY*)hmem);
        FreeImage_SeekMemory( (FIMEMORY*)hmem, 0L, SEEK_SET);

        //hmem = FreeImage_OpenMemory( pVideoFrame->hmem,  pVideoFrame->m_FrameSize);
        //FreeImage_TellMemory(VideoFrame->hmem);
        // load an image from the memory stream
        pImage = FreeImage_LoadFromMemory( (FREE_IMAGE_FORMAT)fif, (FIMEMORY*)hmem, 0);

        switch (FreeImage_GetBPP(pImage))
        {
            case 8: // 8 bit, indexed or grayscale
                m_param.internal_format = GL_RGB;
                _format = GL_LUMINANCE;
                break;
            case 16: // 16 bits
                break;
            case 24: // 24 bits
                m_param.internal_format = GL_RGB;
                if (FreeImage_GetBlueMask(pImage) == 0x000000FF) _format = GL_BGR;
                else _format = GL_RGB;
                break;
            case 32: // 32 bits
                m_param.internal_format = GL_RGBA;
                if (FreeImage_GetBlueMask(pImage) == 0x000000FF) _format = GL_BGRA_EXT;
                else _format = GL_RGBA;
                break;
            default:
                m_param.internal_format = GL_RGBA;
                _format = GL_RGBA;
                break;
        }
        ///just execute this time for building the texture really in card memory
        Build();
        ///then apply the buffer
        FlipBufferVert((MOubyte *)FreeImage_GetBits(pImage), FreeImage_GetBPP(pImage) / 8 );
        SetBuffer( m_width, m_height, FreeImage_GetBits(pImage), _format);
        FreeImage_Unload( pImage );
        //MODebug2->Push( moText("moTextureMemory::BuildFromMemory: success: hmem:") + IntToStr((int)hmem) + moText("glid:") + IntToStr(m_glid));
        return true;
    } else {
        //MODebug2->Error( moText("moTextureMemory::BuildFromMemory: Error GLID or Memory never assigned. hmem:") + IntToStr((int)hmem) + moText("glid:") + IntToStr(m_glid));
        return false;
    }
}


MOboolean moTextureMemory::BuildFromBitmap( moBitmap* p_bitmap, moText p_bufferformat )
{
	MOboolean res = false;

	m_BufferFormat = p_bufferformat;

	if (p_bitmap != NULL)
	{
	    ///Save into memory
        res = LoadFromBitmap( p_bitmap );
        ///BuildFromMemory();

	}
	else {
		if (MODebug2 != NULL) MODebug2->Error(moText("Error at image build from bitmap: [pointer is null] ") + (moText) this->GetName());
		res = false;
	}

	return res;
}


int  moTextureMemory::GetReference() {


    //si la referencia es 0:
    //asigna la textura
    if (this->Initialized()) {

        if (reference_counter==0) {
            if (m_bBitmapInMemory) {
                glGenTextures(1, &m_glid);
                CalculateSize(m_width, m_height);
                if (BuildFromMemory()) {
                    //MODebug2->Push( moText("moTextureMemory::GetReference success: glid: ") + IntToStr(m_glid) );
                    reference_counter++;
                }
            } else MODebug2->Push( moText("moTextureMemory::GetReference no bitmap in memory ") );
        }
    } else MODebug2->Error( moText("moTextureMemory::GetReference Error: object not initialized") );

    return ((int)reference_counter);
}

void  moTextureMemory::ReleaseReference() {
    if (reference_counter>0) {
          reference_counter--;
          if ( reference_counter==0 ) {
              ///deleting texture from opengl memory
              if (m_glid>0) {
                glDeleteTextures(1,&m_glid);
                m_glid = 0;
              }
          }
    }
}

MOboolean moTextureMemory::Finish() {

	if (hmem) {
		FreeImage_CloseMemory( (FIMEMORY*)hmem );
		hmem = NULL;
	}

	m_SizeInMemory = 0;
	hmem = NULL;
	fif = (int)FIF_UNKNOWN;

    return moTexture::Finish();

}

//===========================================
//
//				moTextureAnimated
//
//===========================================

moTextureAnimated::moTextureAnimated() : moTexture()
{
	m_FrameJump = 0;
	m_FrameNext = 0;
	m_FramePrevious = 0;
	m_FrameStart = 0;
	m_FrameEnd = 0;
	m_nFrames = 0;
	m_InterpolationTime = 0;
	m_bInterpolation = false;
	m_pShaderCopy = NULL;
	m_pShaderInterpolate = NULL;
}

moTextureAnimated::~moTextureAnimated()
{
	Finish();
}

MOboolean moTextureAnimated::Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param)
{
	m_FrameJump = 0;
	m_FrameNext = 0;
	m_FramePrevious = 0;
	m_FrameStart = 0;
	m_FrameEnd = 0;
	m_nFrames = 0;
	m_InterpolationTime = 0;
	m_bInterpolation = false;
	m_bInterpolating = false;
	m_pShaderCopy = NULL;
	m_pShaderInterpolate = NULL;
	m_pCopyStart = NULL;
	m_pCopyEnd = NULL;
	m_pInterpolator = NULL;

	return moTexture::Init(p_name, p_moid, p_res, p_param);
}

MOboolean moTextureAnimated::Finish()
{
    if (m_pInterpolator) {
        delete m_pInterpolator;
        m_pInterpolator = NULL;
    }
    if (m_pCopyStart) {
        delete m_pCopyStart;
        m_pCopyStart = NULL;
    }
    if (m_pCopyEnd) {
        delete m_pCopyEnd;
        m_pCopyEnd = NULL;
    }
	return moTexture::Finish();
}

void
moTextureAnimated::SetInterpolation( MOuint p_FrameJump, MOuint p_InterpolationTime ) {

	m_FrameJump = p_FrameJump;
	m_InterpolationTime = p_InterpolationTime;

	int m_idx;
    moFBO* m_pFBO;
	moTexture* m_pTex;

	moShaderManager* SM = m_pResourceManager->GetShaderMan();
	moTextureManager* TM = m_pResourceManager->GetTextureMan();
	moFBManager* FM = m_pResourceManager->GetFBMan();

	m_idx = FM->CreateFBO();
	m_pFBO = FM->GetFBO(m_idx);

	m_pShaderCopy = SM->GetShader(SM->GetShaderIndex(moText("shaders/Copy.cfg"),true) );
	m_pShaderInterpolate = SM->GetShader(SM->GetShaderIndex(moText("shaders/TexInterpolator.cfg"),true) );

	MOint idxstart = TM->AddTexture( moText(this->m_name)+moText("copyStart"), m_width, m_height);
	MOint idxend = TM->AddTexture(moText(this->m_name)+moText("copyEnd"), m_width, m_height);

	//copia del frame A
	m_srcstart.Empty();
	m_srcstart.Add((moTexture*)this);

	m_pTex = TM->GetTexture(idxstart);
	m_dststart.Empty();
	m_dststart.Add(m_pTex);
	m_pTex->SetFBOandAttachPoint(m_pFBO);

	//copia del frame B
	m_srcend.Empty();
	m_srcend.Add((moTexture*)this);

    m_pTex = TM->GetTexture(idxend);
	m_dstend.Empty();
	m_dstend.Add(m_pTex);
	m_pTex->SetFBOandAttachPoint(m_pFBO);

	//interpolacion con fuente A y B
	m_srcinterpol.Empty();
	m_srcinterpol.Add( TM->GetTexture(idxstart));
	m_srcinterpol.Add( TM->GetTexture(idxend));

    m_pTex = (moTexture*)this;
	m_dstinterpol.Empty();
	m_dstinterpol.Add(m_pTex);
	m_pTex->SetFBOandAttachPoint(m_pFBO);
    if (!(m_pCopyStart && m_pCopyEnd && m_pInterpolator)) {
        if (!m_pInterpolator) m_pInterpolator = new moTextureFilter();
        if (m_pCopyStart) m_pCopyStart = new moTextureFilter();
        if (m_pCopyEnd) m_pCopyEnd = new moTextureFilter();
    }
    if (m_pCopyStart && m_pCopyEnd && m_pInterpolator) {
        m_pCopyStart->Init( m_pResourceManager->GetGLMan(), m_pResourceManager->GetRenderMan(), m_srcstart, m_dststart, m_pShaderCopy );
        m_pCopyEnd->Init( m_pResourceManager->GetGLMan(), m_pResourceManager->GetRenderMan(), m_srcend, m_dstend, m_pShaderCopy );
        m_pInterpolator->Init(  m_pResourceManager->GetGLMan(), m_pResourceManager->GetRenderMan(), m_srcinterpol, m_dstinterpol, m_pShaderInterpolate  );
    }
}

MOboolean
moTextureAnimated::IsInterpolating() {
	return m_bInterpolating;
}

MOboolean
moTextureAnimated::IsInterpolationActive() {
	return m_bInterpolation;
}

MOboolean
moTextureAnimated::ActivateInterpolation( MOboolean activate ) {
	if (m_pShaderCopy && m_pShaderInterpolate && activate) {
		m_bInterpolation = true;
	} else {
		m_bInterpolation = false;
		m_bInterpolating = false;
	}
	return m_bInterpolation;
}


//Cuando necesitamos interpolacion?
//
//      Suavizado de saltos:
//			1) se paso el intervalo maximo entre el cuadro actual y el siguiente
//			2) todavia se esta en un ciclo de interpolacion
//
//		Camara lenta: (a implementar)
//			1) el tiempo maximo entre dos cuadros contiguos fue superado, entonces se generará un cuadro intermedio proporcional
//			2) todavia se esta en un ciclo de interpolacion
MOboolean
moTextureAnimated::NeedsInterpolation() {
	// pasamos la barrera del m_FrameJump
	// !!!chequear que el next y el previous esten correctamente fijados
	//MODebug2->Push( moText("Pr:")+IntToStr(m_FramePrevious)+moText("Nxt:")+IntToStr(m_FrameNext) );

	if (!m_bInterpolation)
		return false;
	if (!m_bInterpolating) {
		if ( fabs( (double)((double)m_FrameNext - (double)m_FramePrevious) ) > (double)m_FrameJump ) {
			m_FrameStart = m_FramePrevious;
			m_FrameEnd = m_FrameNext;
			//desactivamos la interpolacion para poder acceder a los cuadros correspondientes libremente
			m_bInterpolating = false;
			ActivateInterpolation(false);
			m_pCopyStart->Apply( m_FrameStart );
			m_pCopyEnd->Apply( m_FrameEnd );
			//activamos nuevamente la interpolacion una vez copiados los cuadros
			ActivateInterpolation(true);
			m_bInterpolating = true;
			//fijamos nuevamente el previous y el next
			m_FramePrevious = m_FrameStart;
			m_FrameNext = m_FrameEnd;
			m_StartTime = moGetTicks();
			return true;
		}
	} else {
		if ( m_InterpolationPosition >= 1.0 ) {
			m_StartTime = 0;
			m_bInterpolating = false;
		}
	}
	return m_bInterpolating;
}


MOint
moTextureAnimated::Interpolate() {
	moTextFilterParam filterparam;
	if ( m_pShaderCopy && m_pShaderInterpolate && m_bInterpolation) {
		if (m_bInterpolating) {
			MOuint m_ActualTime = moGetTicks();
			m_InterpolationPosition = (float)( m_ActualTime - m_StartTime ) / (m_InterpolationTime);
			if ( m_InterpolationPosition >= 1.0) m_InterpolationPosition = 1.0;
			if ( m_InterpolationPosition <= 0.0) m_InterpolationPosition = 0.0;
			filterparam.par_flt1 = m_InterpolationPosition;
			m_pInterpolator->Apply( m_InterpolationPosition, 1.0, &filterparam );
			//MODebug2->Push( moText("IP:")+FloatToStr(m_InterpolationPosition) );
			return 1;
		}
	}
	return 0;
}


// NOTA: este algoritmo fija el cuadro next emparejando el moTempo con la cantidad de frames...
MOint
moTextureAnimated::GetGLId(moTempo *p_tempo) {
	//segun el tempo elige el cuadro!
	int t;
	float ft,PeliV;


	if(p_tempo==NULL) {
			t = m_Time - moGetTicks();
			ft =(t / 1000) * m_fFramesPerSecond;//frames que deberian haber pasado en este lapso...

			if(ft >= 0.99)	{

					m_FrameNext++;

					if( m_FrameNext >= m_nFrames ) {
						m_FrameNext = 0;
					}

			}

	} else {
		PeliV = fmod( float( p_tempo->ang ), float(2*moMathf::PI)) /(2*moMathf::PI);
		m_FrameNext = min(MOuint(PeliV * m_nFrames), m_nFrames - 1);
	}

	if (NeedsInterpolation()) {
		Interpolate();
	} else {
		if(m_FramePrevious!=m_FrameNext) this->GetFrame( m_FrameNext );
		m_FramePrevious = m_FrameNext;
	}

	return m_glid;
}

// NOTA: este devuelve simplemente el glid correspondiente al cuadro interno "i"
//como en este caso no estan definidos las sub-texturas, ya q depende de cada implementaci�
//devuelve el m_glid de moTexture
MOint
moTextureAnimated::GetGLId( MOuint p_i ) {

	m_FrameNext = p_i;

	if (NeedsInterpolation()) {
		Interpolate();
	} else {
		if(m_FramePrevious!=p_i) this->GetFrame(p_i);
		m_FramePrevious = m_FrameNext = p_i;
	}

	return m_glid;
}

MOint moTextureAnimated::GetGLId( MOfloat p_cycle ) {

	MOfloat PeliV;

	PeliV = fmod( (float)p_cycle, (float)1.0 );
	m_FrameNext = min(MOuint(PeliV * m_nFrames), m_nFrames - 1);

	if (NeedsInterpolation()) {
		Interpolate();
	} else {
		if(m_FramePrevious!=m_FrameNext) this->GetFrame( m_FrameNext );
		m_FramePrevious = m_FrameNext;
	}
	return m_glid;

}

void
moTextureAnimated::GetFrame( MOuint p_i) {

  ///always needs implementation??? must be pure virtual.

	return;
}

//===========================================
//
//				moTextureMultiple
//
//===========================================

moTextureMultiple::moTextureMultiple() : moTextureAnimated()
{
	m_type = MO_TYPE_TEXTURE_MULTIPLE;
}

moTextureMultiple::~moTextureMultiple()
{
	Finish();
}

MOboolean moTextureMultiple::Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param)
{
	moTextureAnimated::Init(p_name, p_moid, p_res, p_param);

	m_nFrames = 0;
	m_fFramesPerSecond = 24.0;
	m_textures_array.Init(0, NULL);
	return true;
}

MOboolean moTextureMultiple::Finish()
{
	m_textures_array.Finish();
	moTextureAnimated::Finish();
	return true;
}

MOboolean moTextureMultiple::SupportedFile(moText p_filename)
{
	moText begin = p_filename;
	begin.Left(7);
	return !stricmp(begin,"multiple");
}

MOboolean moTextureMultiple::Load(moParam* p_param)
{
    moValue* pValue = &p_param->GetValue();
    return Load(pValue);
}

MOboolean moTextureMultiple::Load(moValue * p_value)
{
	SetName(p_value->GetSubValue(0).Text());

	int nframes = p_value->GetSubValueCount() - 1;
	moText fn;
	moTexture* newtex;
	for (int i = 0; i < nframes; i++)
	{
		fn = m_pDataMan->GetDataPath() + moSlash + p_value->GetSubValue(i + 1).Text();
		if (moTexture::SupportedFile(fn))
		{
			if (BuildFromFile(fn))
			{
				newtex = new moTexture;
				newtex->SetName(fn);
				newtex->CopyFromTex((moTexture*)this, true, false, false, false);
				m_textures_array.Add(newtex);
			}
		}
	}
	SetFrameCount(m_textures_array.Count());

	m_bBuildedFromFile = (0 < m_textures_array.Count());

	return 0 < m_textures_array.Count();
}

MOboolean moTextureMultiple::Load(moText p_name, moTextureArray &p_textures, MOuint p_id0, MOuint p_id1)
{
	SetName(p_name);
	m_textures_array.Copy(p_textures, p_id0, p_id1);
	SetFrameCount(m_textures_array.Count());
	return 0 < m_textures_array.Count();
}

void moTextureMultiple::SetTextureCount(MOuint p_tex_count)
{
	m_textures_array.Init(p_tex_count, NULL);
	SetFrameCount(p_tex_count);
}

void moTextureMultiple::GetFrame(MOuint p_i)
{
	if (ValidTexture(p_i)) this->CopyFromTex(m_textures_array[p_i], true, false, false, false);
	else m_glid = 0;
}

void moTextureMultiple::SetFrame(MOuint p_i, moTexture* p_texture)
{
	if (p_i < m_textures_array.Count())
		if (ValidTexture(p_i)) m_textures_array[p_i]->CopyFromTex(p_texture, true, true, true, true);
		else m_textures_array[p_i] = p_texture;
}

void moTextureMultiple::AddFrame(moTexture* p_texture)
{
	m_textures_array.Add(p_texture);
	m_nFrames++;
}

MOboolean moTextureMultiple::ValidTexture(MOuint p_i)
{
	return ((p_i < (MOuint)m_textures_array.Count()) && (m_textures_array[p_i] != NULL));
}

//===========================================
//
//				moMovie
//
//===========================================

moMovie::moMovie() : moTextureAnimated()
{
	m_type = MO_TYPE_MOVIE;
	m_pGraph = NULL;
}

moMovie::~moMovie()
{
	Finish();
}

MOboolean moMovie::Init(moText p_name, MOuint p_moid, moResourceManager* p_res, moTexParam p_param)
{
	moTextureAnimated::Init(p_name, p_moid, p_res, p_param);
    if(!m_pGraph) {

        #ifdef MO_DIRECTSHOW
            m_pGraph = (moVideoGraph*) new moDsGraph();
        #endif

        #ifdef MO_GSTREAMER
            m_pGraph = (moVideoGraph*) new moGsGraph();
        #endif
    }
	return true;
}

MOboolean moMovie::Finish()
{
	if (m_pGraph!=NULL)
	{
		m_pGraph->FinishGraph();
		delete m_pGraph;
		m_pGraph = NULL;
	}
	moTextureAnimated::Finish();
	return true;
}

MOboolean moMovie::SupportedFile(moText p_filename)
{
	moText extension = p_filename.Right(3);
	return (!stricmp(extension,"avi") || !stricmp(extension,"mov") || !stricmp(extension,"mpg") || !stricmp(extension,"mp4")|| !stricmp(extension,"ogg") || !stricmp(extension,"mpv") || !stricmp(extension,"mkv") || !stricmp(extension,"m2v"));
}

MOboolean moMovie::LoadMovieFile(moText p_filename)
{
	moText extension;
	MOboolean result = false;
	bool quicktime = false;

	extension = p_filename;
	extension.Right(3);

	if (extension==moText("mov")) quicktime = true;

	MOulong frames;
	MOboolean graphloaded = false;
	if (m_pGraph)
	{
       if (!m_pGraph->InitGraph())
       return false;

		if (quicktime)
		{
			///QUICKTIME FOR DIRECTSHOW
			graphloaded = m_pGraph->BuildLiveQTVideoGraph( p_filename, &m_BucketsPool);
		}
		else
		{
			///UNIVERSAL
			graphloaded = m_pGraph->BuildLiveVideoGraph( p_filename, &m_BucketsPool);
		}

		if ( graphloaded )
		{
			m_pGraph->Pause();

			frames = m_pGraph->GetFramesLength();
			frameprevious = 0;

			SetFramesPerSecond(( (double) m_pGraph->GetVideoFormat().m_FrameRate ) /  (double)( 100.0 ) );
			m_param = MODefTex2DParams;
			result = BuildEmpty(m_pGraph->GetVideoFormat().m_Width, m_pGraph->GetVideoFormat().m_Height);
			if (result)
			{
				lastframe = frames;
				SetFrameCount( frames );
			}
		}
	}

	if (result) m_bBuildedFromFile = true;

	return result;
}

MOboolean moMovie::Load(moParam * p_param)
{
	moValue* pValue = &p_param->GetValue();
	return Load( pValue );
}

MOboolean moMovie::Load(moValue * p_value)
{
	moText namefull;

	SetName( p_value->GetSubValue(0).Text());

	namefull = m_pDataMan->GetDataPath() + moSlash + (moText)m_name;

	return LoadMovieFile(namefull);
}

void moMovie::EnableVideo(int enable) {}

void moMovie::EnableAudio(int enable) {}

void moMovie::GetFrame( MOuint p_i )
{
	if(m_pGraph)
	{
    moStreamState state = m_pGraph->GetState();

		m_pGraph->Seek( p_i );
		m_pGraph->Pause();

		if (!m_BucketsPool.IsEmpty()) {

      moBucket* pbucket = NULL;
      MOubyte* pbuffer = NULL;

      pbucket = m_BucketsPool.RetreiveBucket();

      if (pbucket) {
        pbuffer = pbucket->GetBuffer();
        if (pbuffer) {

          ///lock to prevent any data or reference loss...
          pbucket->Lock();
          SetBuffer(pbuffer, GL_BGR_EXT );
          pbucket->Unlock();

          pbucket->EmptyBucket();
        }
        m_BucketsPool.DestroyRetreivedBucket();
      }
		}
		/*
		else {

      MODebug2->Error(  moText("moMovie::GetFrame()")
                      + moText(" m_BucketsPool is EMPTY !!!")
                      + moText(" state:")
                      + m_pGraph->StateToText(state) );

    }
    */
	} else {
      MODebug2->Error( moText("moMovie::GetFrame()") + moText(" m_pGraph is NULL !!!") );
  }
}
