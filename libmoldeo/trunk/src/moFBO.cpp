/*******************************************************************************

                                moFBO.cpp

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

#include "moFBO.h"

#include "moArray.cpp"
moDefineDynamicArray(moFBOArray)

//===========================================
//
//				moFBO
//
//===========================================

moFBO::moFBO()
{
	m_gl = NULL;
	m_fbo = 0;
	m_num_color_attach_points = 0;
    m_has_depth_buffer = m_has_stencil_buffer = false;
	m_width = 0;
	m_height = 0;
}

moFBO::~moFBO()
{
	Finish();
}

MOboolean  moFBO::Init(moGLManager* p_gl)
{
	m_gl = p_gl;
	glGenFramebuffersEXT(1, &m_fbo);
	m_num_color_attach_points = 0;
    m_has_depth_buffer = m_has_stencil_buffer = false;
	m_width = 0;
	m_height = 0;
    InitAttachPointsArray();
	return (m_gl!=NULL);
}

MOboolean moFBO::Finish()
{
	if (m_has_depth_buffer && m_has_stencil_buffer)
	{
		glDeleteTextures(1, &m_DepthStencilTex);
		m_has_depth_buffer = false;
		m_has_stencil_buffer = false;
	}
    if (0 < m_fbo)
	{
		glDeleteFramebuffersEXT(1, &m_fbo);
		m_fbo = 0;
	}
	return true;
}

MOboolean moFBO::AddDepthStencilBuffer()
{
	if (!m_has_depth_buffer && !m_has_stencil_buffer && (0 < m_width) && (0 < m_height))
	{
		glGenTextures(1, &m_DepthStencilTex);
		glBindTexture(GL_TEXTURE_2D, m_DepthStencilTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8_EXT,
					 m_width, m_height, 0, GL_DEPTH_STENCIL_EXT,
					 GL_UNSIGNED_INT_24_8_EXT, NULL);

		Bind();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
			                      GL_DEPTH_ATTACHMENT_EXT,
				                  GL_TEXTURE_2D, m_DepthStencilTex, 0);
	    Unbind();

		m_has_depth_buffer = true;
		m_has_stencil_buffer = true;
		return true;
	}
	else return false;
}

void moFBO::Bind()
{
	m_gl->SaveFBOState();
	m_gl->SetCurrentFBO(m_fbo);
}

void moFBO::Unbind()
{
	m_gl->RestoreFBOState();
}

void moFBO::SetReadTexture(MOuint p_attach_point)
{
	if (p_attach_point < m_num_color_attach_points)
		m_gl->SetCurrentReadBuffer(m_attach_points_array[p_attach_point]);
}

void moFBO::SetDrawTexture(MOuint p_attach_point)
{
	if (p_attach_point < m_num_color_attach_points)
	    m_gl->SetCurrentDrawBuffer(m_attach_points_array[p_attach_point]);
}

void moFBO::SetReadTexture(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_attach_point)
{
    MOint i = GetColorAttachPointIndex(p_glid);
	if (i == -1) AttachTexture(p_width, p_height, p_param, p_glid, p_attach_point);
	else p_attach_point = (MOuint)i;
	SetReadTexture(p_attach_point);
}

void moFBO::SetDrawTexture(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_attach_point)
{
    MOint i = GetColorAttachPointIndex(p_glid);
	if (i == -1) AttachTexture(p_width, p_height, p_param, p_glid, p_attach_point);
	else p_attach_point = (MOuint)i;
	SetDrawTexture(p_attach_point);
}

MOint moFBO::GetColorAttachPoint(MOuint p_attach_point)
{
    if (p_attach_point < m_num_color_attach_points)
		return m_attach_points_array[p_attach_point];
	else return -1;
}

MOint moFBO::GetTextureGLId(MOuint p_attach_point)
{
	if (p_attach_point < m_num_color_attach_points)
		return m_tex_glid_array[p_attach_point];
	else return -1;
}

MOint moFBO::GetColorAttachPointIndex(MOuint p_glid)
{
    for (MOuint i = 0; i < m_num_color_attach_points; i++)
        if (m_tex_glid_array[i] == p_glid) return i;
	return -1;
}

MOuint moFBO::AddTexture(MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_glid, MOuint &p_attach_point)
{
	bool valid_tex;
	MOuint status;
	if (m_num_color_attach_points < MO_MAX_COLOR_ATTACHMENTS_EXT)
	{
		if (m_num_color_attach_points == 0)
		{
			m_target = p_param.target;
			m_internal_format = p_param.internal_format;
			m_width = p_width;
			m_height = p_height;
			valid_tex = true;
		}
		else
		{
			valid_tex = (m_target == p_param.target) && (m_internal_format == p_param.internal_format) &&
						(m_width == p_width) && (m_height == p_height);
		}

		if (!valid_tex)
		{
			status = MO_FRAMEBUFFER_INVALID_TEX;
			return status;
		}

		Bind();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
									m_attach_points_array[m_num_color_attach_points],
									m_target, p_glid, 0);
		status = CheckStatus();
		Unbind();

		if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			m_tex_glid_array[m_num_color_attach_points] = p_glid;
			p_attach_point = m_num_color_attach_points;
			m_num_color_attach_points++;
		}
	}
	else status = MO_FRAMEBUFFER_FULL;
	return status;
}

MOuint moFBO::AttachTexture(MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_glid, MOuint p_attach_point)
{
	bool valid_tex;
	MOuint status;
	if (p_attach_point < MO_MAX_COLOR_ATTACHMENTS_EXT)
	{
		if (m_num_color_attach_points == 0)
		{
			m_target = p_param.target;
			m_internal_format = p_param.internal_format;
			m_width = p_width;
			m_height = p_height;
			valid_tex = true;
		}
		else
		{
			valid_tex = (m_target == p_param.target) && (m_internal_format == p_param.internal_format) &&
						(m_width == p_width) && (m_height == p_height);
		}

		if (!valid_tex)
		{
			status = MO_FRAMEBUFFER_INVALID_TEX;
			return status;
		}

		Bind();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
									m_attach_points_array[p_attach_point],
									m_target, p_glid, 0);
		status = CheckStatus();
		Unbind();

		if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			m_tex_glid_array[p_attach_point] = p_glid;
		}
	}
	else status = MO_FRAMEBUFFER_FULL;
	return status;
}

void moFBO::ClearAttachements()
{
	if (m_num_color_attach_points > 0)
	{
		Bind();
		for (MOuint i = 0; i < m_num_color_attach_points; i++)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
			    	 					m_attach_points_array[i],
					    				m_target, 0, 0);
		Unbind();
	}
}

MOuint moFBO::CheckStatus()
{
	if (MODebug != NULL) MODebug->Push("Framebuffer status: ");
	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_COMPLETE_EXT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
			break;
//		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
//			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT");
//			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
   			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			if (MODebug != NULL) MODebug->Push("GL_FRAMEBUFFER_UNSUPPORTED_EXT");
			break;
		default:
			if (MODebug != NULL) MODebug->Push("Unknown error");
			break;
	}
	return status;
}

MOboolean moFBO::IsValidTexture(MOuint p_width, MOuint p_height, const moTexParam& p_param)
{
	if (m_num_color_attach_points == 0) return true;
	else
		return (m_target == p_param.target) && (m_internal_format == p_param.internal_format) &&
			   (m_width == p_width) && (m_height == p_height);
}

void moFBO::InitAttachPointsArray()
{
	for (MOuint i = 0; i < MO_MAX_COLOR_ATTACHMENTS_EXT; i++)
		m_attach_points_array[i] = GL_COLOR_ATTACHMENT0_EXT + i;
}
