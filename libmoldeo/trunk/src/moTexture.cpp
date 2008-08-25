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

#include "moTexture.h"

#include "moArray.cpp"
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
	glGenTextures(1, &m_glid);
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

	#ifdef USE_SDLIMAGE
	SDL_Surface *m_pImage = IMG_Load(p_filename);

	if (m_pImage != NULL)
	{
		MOuint p_width = m_pImage->w;
		MOuint p_height = m_pImage->h;
		MOuint p_format;

		m_param.target = GL_TEXTURE_2D;
		switch (m_pImage->format->BytesPerPixel)
		{
			case 1: // 8 bit, indexed or grayscale
				m_param.internal_format = GL_RGB;
				p_format = GL_LUMINANCE;
				break;
			case 2: // 16 bits
				break;
			case 3: // 24 bits
				m_param.internal_format = GL_RGB;
				if (m_pImage->format->Bmask == 0x000000FF) p_format = GL_BGR;
				else p_format = GL_RGB;
				break;
			case 4: // 32 bits
				m_param.internal_format = GL_RGBA;
				if (m_pImage->format->Bmask == 0x000000FF) p_format = GL_BGRA_EXT;
				else p_format = GL_RGBA;
				break;
			default:
				break;
		}

		BuildEmpty(p_width, p_height);
		FlipBufferVert((MOubyte *)m_pImage->pixels, m_pImage->format->BytesPerPixel);
		res = SetBuffer(p_width, p_height, m_pImage->pixels, p_format);

		SDL_FreeSurface(m_pImage);
	}
	#endif
	#ifdef USE_FREEIMAGE

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
		MOuint p_width = FreeImage_GetWidth(m_pImage);
		MOuint p_height = FreeImage_GetHeight(m_pImage);
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

		BuildEmpty(p_width, p_height);
		//FlipBufferVert((MOubyte *)FreeImage_GetBits(m_pImage), FreeImage_GetBPP(m_pImage) / 8 );
		res = SetBuffer(p_width, p_height, FreeImage_GetBits(m_pImage), p_format);

		FreeImage_Unload(m_pImage);
	}
	#endif
	else {
		if (MODebug != NULL) MODebug->Push(moText("Error at image load ") + IMG_GetError());
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
	}
	else
	{
		m_width = p_width;
		m_height = p_height;
		m_max_coord_s = 1.0;
		m_max_coord_t = 1.0;
	}
	/*
	else if (GLEW_ARB_texture_non_power_of_two)
	{
		m_width = p_width;
		m_height = p_height;
		m_max_coord_s = 1.0;
		m_max_coord_t = 1.0;
	}
	else
	{
            m_width = NextPowerOf2(p_width);
		m_height = NextPowerOf2(p_height);
		m_max_coord_s = p_width / m_width;
		m_max_coord_t = p_height / m_height;
	}
	*/
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

    if ( newfilename==moText("") )
        thumbnailfilename = (moText)m_pDataMan->GetDataPath() + moSlash + (moText)m_name + moText(".thm");
    else
        thumbnailfilename = newfilename;


    FREE_IMAGE_FORMAT fif;
    FIBITMAP* fbitmap = NULL;
    int options = 0;

    BYTE* tempbuffer = NULL;

    if (GetHeight()==0 || GetWidth()==0)
        return moText("");

    tempbuffer = new BYTE [ GetHeight() * GetWidth() * 4 ];

    if (tempbuffer==NULL)
        return moText("");

    if (!GetBuffer( tempbuffer, GL_RGBA, GL_UNSIGNED_BYTE ) )
        return moText("");
/*
    FILE* fp;

    fp = fopen( thumbnailfilename ,"wb" );
    fwrite( tempbuffer, sizeof(BYTE), m_width * m_height * 4 , fp );
    fclose(fp);
    */

    int bpp = 32;
    int pitch = 4 * GetWidth();


    fbitmap = FreeImage_ConvertFromRawBits( (BYTE*)tempbuffer, GetWidth(), GetHeight(), pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false );

    unsigned width = FreeImage_GetWidth(fbitmap);
    unsigned height = FreeImage_GetHeight(fbitmap);
    unsigned ppitch = FreeImage_GetPitch(fbitmap);
    FREE_IMAGE_TYPE image_type = FreeImage_GetImageType( fbitmap );
    BYTE *bits = (BYTE*)FreeImage_GetBits(fbitmap);

    FILE* fp = fopen( thumbnailfilename ,"wb" );
    fwrite( tempbuffer, sizeof(BYTE), m_width * m_height * 4 , fp );
    fclose(fp);
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

    //return ( moText("width:") + IntToStr(width) + moText("height:") + IntToStr(height) + moText("pitch") + IntToStr(ppitch) + moText("fif") + IntToStr(fif) + moText("oo") + IntToStr((int)fbitmap)  );

    return thumbnailfilename;

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
	//MODebug->Push( moText("Pr:")+IntToStr(m_FramePrevious)+moText("Nxt:")+IntToStr(m_FrameNext) );

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
			m_StartTime = SDL_GetTicks();
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
			MOuint m_ActualTime = SDL_GetTicks();
			m_InterpolationPosition = (float)( m_ActualTime - m_StartTime ) / (m_InterpolationTime);
			if ( m_InterpolationPosition >= 1.0) m_InterpolationPosition = 1.0;
			if ( m_InterpolationPosition <= 0.0) m_InterpolationPosition = 0.0;
			filterparam.par_flt1 = m_InterpolationPosition;
			m_pInterpolator->Apply( m_InterpolationPosition, 1.0, &filterparam );
			//MODebug->Push( moText("IP:")+FloatToStr(m_InterpolationPosition) );
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
			t = m_Time - SDL_GetTicks();
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
	SetNumberFrames(m_textures_array.Count());

	m_bBuildedFromFile = (0 < m_textures_array.Count());

	return 0 < m_textures_array.Count();
}

MOboolean moTextureMultiple::Load(moText p_name, moTextureArray &p_textures, MOuint p_id0, MOuint p_id1)
{
	SetName(p_name);
	m_textures_array.Copy(p_textures, p_id0, p_id1);
	SetNumberFrames(m_textures_array.Count());
	return 0 < m_textures_array.Count();
}

void moTextureMultiple::SetTextureCount(MOuint p_tex_count)
{
	m_textures_array.Init(p_tex_count, NULL);
	SetNumberFrames(p_tex_count);
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
	return (!stricmp(extension,"avi") || !stricmp(extension,"mov") || !stricmp(extension,"mpg"));
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
			//QUICKTIME FOR DIRECTSHOW
			graphloaded = m_pGraph->BuildLiveQTVideoGraph( p_filename, &m_BucketsPool);
		}
		else
		{
			//UNIVERSAL
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
				SetNumberFrames( frames );
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
	if(m_pGraph && !m_BucketsPool.IsEmpty())
	{
		m_pGraph->Seek( p_i );
		m_pGraph->Pause();
		moBucket* pbucket = NULL;
		MOubyte* pbuffer = NULL;
		pbucket = m_BucketsPool.RetreiveBucket();
		pbuffer = pbucket->GetBuffer();
		SetBuffer(pbuffer, GL_BGR_EXT );
		delete pbuffer;
		delete pbucket;
	}
}
