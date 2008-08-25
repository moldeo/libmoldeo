/*******************************************************************************

                              moGLManager.cpp

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

#include "moGLManager.h"

moGLManager::moGLManager()
{

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_GL );

	SetName("GL Manager");

	m_gpu_vendor_code = 0;
	m_gpu_ventor_string = "";

	m_current_fbo = m_previous_fbo = 0;

	m_current_read_buffer = m_current_draw_buffer = 0;
	m_previous_read_buffer = m_previous_draw_buffer = 0;
}

moGLManager::~moGLManager()
{
	Finish();
}

MOboolean moGLManager::Init()
{
	QueryGPUVendorString();
	m_current_fbo = 0;
	glGetIntegerv(GL_DRAW_BUFFER, &m_current_draw_buffer);
	glGetIntegerv(GL_READ_BUFFER, &m_current_read_buffer);
	return true;
}

MOboolean moGLManager::Finish()
{
	return true;
}

MOboolean moGLManager::CheckErrors(moText p_location)
{
	GLuint errnum;
	moText errstr;
	MOboolean error = false;
	while ((errnum = glGetError()))
	{
		error = true;
		errstr = (char *)gluErrorString(errnum);
		if (p_location != moText("")) errstr += moText(" at ") + moText(p_location);
		//if (MODebug != NULL) MODebug->Push(errstr);
	}
	return error;
}

void moGLManager::QueryGPUVendorString()
{
    char vendor[80];
    char *glvendor = NULL;
    glvendor = (char*)glGetString(GL_VENDOR);
    if (glvendor!=NULL) {
        strcpy(vendor, glvendor);
        m_gpu_ventor_string = vendor;

        if (strstr(vendor, "NVIDIA") != NULL) m_gpu_vendor_code = MO_GPU_NV;
        else if (strstr(vendor, "ATI") != NULL) m_gpu_vendor_code = MO_GPU_ATI;
        else if (strstr(vendor, "INTEL") != NULL) m_gpu_vendor_code = MO_GPU_INTEL;
        else m_gpu_vendor_code = MO_GPU_OTHER;
    } else m_gpu_vendor_code = MO_GPU_OTHER;
}

void moGLManager::SetPerspectiveView(MOint p_width, MOint p_height)
{
	float screen_ratio = (float) p_width /(float) p_height;

	glViewport(0, 0, p_width, p_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, screen_ratio, 0.1f, 1024.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void moGLManager::SetOrthographicView(MOint p_width, MOint p_height)
{
	glViewport(0, 0, p_width, p_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0, p_width, 0.0, p_height);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

MOint moGLManager::GetRenderMode()
{
	MOint render_mode;
	glGetIntegerv(GL_RENDER_MODE, &render_mode);
	return render_mode;
}

void moGLManager::SetRenderMode(MOint p_mode)
{
	// Valid modes are GL_RENDER, GL_SELECT and GL_FEEDBACK.
	glRenderMode(p_mode);
}

// Setting up rendering parameters commonly used in moldeo effects.
void moGLManager::SetMoldeoGLState()
{
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing.
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do.
	glEnable(GL_BLEND);									// Enables blending.
	glEnable(GL_TEXTURE_2D);							// Enables texturing.
}

// Sets the variables changed by SetMoldeoGLState to the default GL settings.
void moGLManager::SetDefaultGLState()
{
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing.
	glDisable(GL_BLEND);								// Disables blending.
	glDisable(GL_TEXTURE_2D);							// Disables texturing.
}

void moGLManager::SetDefaultPixelStorageModes()
{

	glPixelStorei(GL_PACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_PACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

}

void moGLManager::SaveGLState()
{
	// For more info about the GL attributes, see apendix B of the Red Book:
	// http://cs-sdl.sourceforge.net/index.php/Red_Book_Appendix_B

	SaveFramebuffer();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
	SaveGLMatrices();
}

void moGLManager::SaveGLMatrices()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glMatrixMode(GL_TEXTURE);
    glPushMatrix();
}

void moGLManager::SaveView()
{
	glPushAttrib(GL_VIEWPORT_BIT);
	SaveGLMatrices();
}

void moGLManager::SaveFramebuffer()
{
	m_saved_fbo = m_current_fbo;
	m_saved_read_buffer = m_current_read_buffer;
	m_saved_draw_buffer = m_current_draw_buffer;
}

void moGLManager::RestoreGLState()
{
	RestoreFramebuffer();
	RestoreGLMatrices();
    glPopAttrib();
}

void moGLManager::RestoreGLMatrices()
{
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glMatrixMode(GL_TEXTURE);
    glPopMatrix();
}

void moGLManager::RestoreView()
{
	RestoreGLMatrices();
	glPopAttrib();
}

void moGLManager::RestoreFramebuffer()
{
    m_current_fbo = m_saved_fbo;
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_current_fbo);

	m_current_read_buffer = m_saved_read_buffer;
	glReadBuffer(m_current_read_buffer);

	m_current_draw_buffer = m_saved_draw_buffer;
	glDrawBuffer(m_current_draw_buffer);
}

moTexParam moGLManager::BuildFPTexParam(MOboolean p_16bits, MOushort p_num_components)
{
    moTexParam result;

    if (m_gpu_vendor_code == MO_GPU_NV)
    {
        result.target = GL_TEXTURE_RECTANGLE_NV;
        if (p_16bits)
			if (p_num_components == 4) result.internal_format = GL_FLOAT_RGBA16_NV;
			else result.internal_format = GL_FLOAT_R16_NV;
        else
			if (p_num_components == 4) result.internal_format = GL_FLOAT_RGBA32_NV;
			else result.internal_format = GL_FLOAT_R32_NV;
    }
    else if (m_gpu_vendor_code == MO_GPU_ATI)
    {
        result.target = GL_TEXTURE_RECTANGLE_ARB;
        if (p_16bits)
			if (p_num_components == 4) result.internal_format = GL_RGBA_FLOAT16_ATI;
			else result.internal_format = GL_LUMINANCE_FLOAT16_ATI;
        else
			if (p_num_components == 4) result.internal_format = GL_RGBA_FLOAT32_ATI;
			else result.internal_format = GL_LUMINANCE_FLOAT32_ATI;
    }
    else
    {
        result.target = GL_TEXTURE_RECTANGLE_ARB;
        if (p_16bits)
			if (p_num_components == 4) result.internal_format = GL_RGBA16F_ARB;
			else result.internal_format = GL_LUMINANCE16F_ARB;
        else
			if (p_num_components == 4) result.internal_format = GL_RGBA32F_ARB;
			else result.internal_format = GL_LUMINANCE32F_ARB;
    }

	// FP textures don't have filtering nor wrapping.
	result.mag_filter = GL_NEAREST;
	result.min_filter = GL_NEAREST;
	result.wrap_s = GL_CLAMP_TO_EDGE;
	result.wrap_t = GL_CLAMP_TO_EDGE;

	return result;
}

MOboolean moGLManager::RectTexture(GLenum p_target)
{
	return (p_target == GL_TEXTURE_RECTANGLE_NV) ||
	       (p_target == GL_TEXTURE_RECTANGLE_ARB);
}

MOboolean moGLManager::FPTexture(GLint p_internal_format)
{
	return (p_internal_format == GL_FLOAT_RGBA16_NV) ||
			(p_internal_format == GL_FLOAT_RGBA32_NV) ||
			(p_internal_format == GL_FLOAT_R16_NV) ||
			(p_internal_format == GL_FLOAT_R32_NV) ||
			(p_internal_format == GL_RGBA_FLOAT16_ATI) ||
			(p_internal_format == GL_RGBA_FLOAT32_ATI) ||
			(p_internal_format == GL_LUMINANCE_FLOAT16_ATI) ||
			(p_internal_format == GL_LUMINANCE_FLOAT32_ATI) ||
			(p_internal_format == GL_RGBA16F_ARB) ||
			(p_internal_format == GL_RGBA32F_ARB) ||
			(p_internal_format == GL_LUMINANCE16F_ARB) ||
			(p_internal_format == GL_LUMINANCE32F_ARB);
}

MOboolean moGLManager::MipMapTexture(GLint p_min_filter)
{
	return (p_min_filter == GL_NEAREST_MIPMAP_NEAREST) ||
			(p_min_filter == GL_LINEAR_MIPMAP_NEAREST) ||
			(p_min_filter == GL_NEAREST_MIPMAP_LINEAR) ||
			(p_min_filter == GL_LINEAR_MIPMAP_LINEAR);
}

void moGLManager::SetCurrentFBO(MOuint m_fbo)
{
	m_current_fbo = m_fbo;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_current_fbo);
}

void moGLManager::SetCurrentReadBuffer(MOint p_buffer)
{
	m_current_read_buffer = p_buffer;
	glReadBuffer(m_current_read_buffer);
}

void moGLManager::SetCurrentDrawBuffer(MOint p_buffer)
{
	m_current_draw_buffer = p_buffer;
	glDrawBuffer(m_current_draw_buffer);
}

void moGLManager::SaveFBOState()
{
	m_previous_fbo = m_current_fbo;
	m_previous_read_buffer = m_current_read_buffer;
	m_previous_draw_buffer = m_current_draw_buffer;
}

void moGLManager::RestoreFBOState()
{
	SetCurrentFBO(m_previous_fbo);
	SetCurrentReadBuffer(m_previous_read_buffer);
	SetCurrentDrawBuffer(m_previous_draw_buffer);
}
