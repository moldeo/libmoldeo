/*******************************************************************************

                              moFBManager.cpp

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

#include "moFBManager.h"

//===========================================
//
//				moFBManager
//
//===========================================

moFBManager::moFBManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_FB );
  SetName("fbmanager");
  SetLabelName("fbmanager");
	m_glmanager = NULL;
}

moFBManager::~moFBManager()
{
	Finish();
}

MOboolean moFBManager::Init()
{
	if ( m_pResourceManager ) {
		m_glmanager = m_pResourceManager->GetGLMan();
	}
	m_fbo_array.Init(0, NULL);
	GetScreenBuffers();
	m_fbo_stack_index = -1;
	m_current_fbo = -1;
	return (m_glmanager!=NULL);
}

MOboolean moFBManager::Finish()
{
	m_fbo_array.Finish();
	return true;
}

MOuint moFBManager::CreateFBO()
{
	moFBO* new_fbo = new moFBO();
	new_fbo->Init(m_glmanager);
	m_fbo_array.Add(new_fbo);
	return m_fbo_array.Count() - 1;
}

MOboolean moFBManager::DeleteFBO(MOuint p_fbo)
{
	if ( p_fbo!=(MOuint)MO_UNDEFINED && p_fbo < m_fbo_array.Count())
	{
		delete m_fbo_array.GetRef(p_fbo);
		m_fbo_array.Remove(p_fbo);
		UnbindAllFBO();
		return true;
	}
	else return false;
}

MOint moFBManager::GetFBO(MOuint p_target, MOuint p_internal_format, MOuint p_width, MOuint p_height)
{
    for (MOuint i = 0; i < m_fbo_array.Count(); i++)
	{
		if ((m_fbo_array.Get(i)->GetTarget() == p_target) &&
		    (m_fbo_array.Get(i)->GetTarget() == p_internal_format) &&
		    (m_fbo_array.Get(i)->GetTarget() == p_width) &&
		    (m_fbo_array.Get(i)->GetTarget() == p_height))
			return i;
	}
	return -1;
}

void moFBManager::BindScreenFB()
{
    if (m_glmanager->GetCurrentFBO() != 0)
	{
		m_fbo_stack_index++;
		m_fbo_stack[m_fbo_stack_index] = (MOuint)m_current_fbo;

		m_draw_buffer_stack[m_fbo_stack_index] = m_glmanager->GetCurrentDrawBuffer();
		m_read_buffer_stack[m_fbo_stack_index] = m_glmanager->GetCurrentReadBuffer();

        m_glmanager->SetCurrentFBO(0);
		m_glmanager->SetCurrentDrawBuffer(m_screen_draw_buffer);
        m_glmanager->SetCurrentReadBuffer(m_screen_read_buffer);
	    m_current_fbo = -1;
	}
}

void moFBManager::UnbindScreenFB()
{
	if (m_glmanager->GetCurrentFBO() == 0) UnbindFBO();
}

MOboolean moFBManager::BindFBO(MOuint p_fbo)
{
	if ( p_fbo!=(MOuint)MO_UNDEFINED && p_fbo < m_fbo_array.Count())
	{
		m_fbo_stack_index++;
		m_fbo_stack[m_fbo_stack_index] = m_current_fbo;

		m_draw_buffer_stack[m_fbo_stack_index] = m_glmanager->GetCurrentDrawBuffer();
		m_read_buffer_stack[m_fbo_stack_index] = m_glmanager->GetCurrentReadBuffer();

		m_fbo_array[p_fbo]->Bind();
		m_current_fbo = p_fbo;
		return true;
	}
	else return false; // Wrong FBO index.
}

MOboolean moFBManager::BindFBO(MOuint p_fbo, MOuint p_attach_point)
{
	if (BindFBO(p_fbo))
	{
		m_fbo_array[p_fbo]->SetDrawTexture(p_attach_point);
		return true;
	}
	else return false;
}

MOboolean moFBManager::BindFBO(MOuint p_fbo, MOuint p_num_attach_points, MOuint *p_attach_point_list)
{
	if (BindFBO(p_fbo))
	{
		SetDrawTextures(p_num_attach_points, p_attach_point_list);
		return true;
	}
	else return false;
}

MOboolean moFBManager::AddTexture(MOuint p_fbo, moTexture* p_pTex, MOuint &p_attach_point)
{
	if ( p_fbo!=(MOuint)MO_UNDEFINED && p_fbo < m_fbo_array.Count())
	{
		m_fbo_array[p_fbo]->AddTexture(p_pTex->GetWidth(), p_pTex->GetHeight(), p_pTex->GetTexParam(), p_pTex->GetGLId(), p_attach_point);
		return true;
	}
	else return false;
}

MOboolean moFBManager::AttachTexture(MOuint p_fbo, moTexture* p_pTex, MOuint p_attach_point)
{
	if ( p_fbo!=(MOuint)MO_UNDEFINED && p_fbo < m_fbo_array.Count())
	{
		m_fbo_array[p_fbo]->AttachTexture(p_pTex->GetWidth(), p_pTex->GetHeight(), p_pTex->GetTexParam(), p_pTex->GetGLId(), p_attach_point);
		return true;
	}
	else return false;
}

void moFBManager::SetDrawTexture(MOuint p_attach_point)
{
	if (-1 < m_current_fbo) m_fbo_array[m_current_fbo]->SetDrawTexture(p_attach_point);
}

void moFBManager::SetDrawTextures(MOuint p_num_attach_points, MOuint *p_attach_point_list)
{
	if (-1 < m_current_fbo)
	{
		GLsizei n = p_num_attach_points;

		for (int i = 0; i < n; i++) m_color_buffers[i] = m_fbo_array[m_current_fbo]->GetColorAttachPoint(p_attach_point_list[i]);
		glDrawBuffers(n, m_color_buffers);
	}
}

void moFBManager::SetReadTexture(MOuint p_attach_point)
{
	if (-1 < m_current_fbo) m_fbo_array[m_current_fbo]->SetReadTexture(p_attach_point);
}

void moFBManager::SetDrawTextureByGLId(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_def_attach_point)
{
	if (-1 < m_current_fbo) m_fbo_array[m_current_fbo]->SetDrawTexture(p_glid, p_width, p_height, p_param, p_def_attach_point);
}

void moFBManager::SetDrawTexturesByGLId(MOuint p_num_glids, MOuint *p_glid_list, MOuint p_width, MOuint p_height, const moTexParam& p_param)
{
	if (-1 < m_current_fbo)
	{
		int i;
		GLsizei n = p_num_glids;

		for (i = 0; i < n; i++)
		    m_fbo_array[m_current_fbo]->SetDrawTexture(p_glid_list[i], p_width, p_height, p_param, i);

		for (int i = 0; i < n; i++) m_color_buffers[i] = m_fbo_array[m_current_fbo]->GetColorAttachPoint(i);
		glDrawBuffers(n, m_color_buffers);
	}
}

void moFBManager::SetReadTextureByGLId(MOuint p_glid, MOuint p_width, MOuint p_height, const moTexParam& p_param, MOuint p_def_attach_point)
{
	if (-1 < m_current_fbo) m_fbo_array[m_current_fbo]->SetReadTexture(p_glid, p_width, p_height, p_param, p_def_attach_point);
}

MOint moFBManager::SetDrawTexture(moTexture* p_pTex, MOuint p_def_attach_point)
{
	MOint result = 1;
	if ((-1 < m_current_fbo) && (p_pTex != NULL))
	{
		if (p_pTex->GetFBO() == NULL)
			// Attaching by GL id to the currently binded fbo.
			SetDrawTextureByGLId(p_pTex->GetGLId(), p_pTex->GetWidth(), p_pTex->GetHeight(), p_pTex->GetTexParam(), p_def_attach_point);
		else
			// The texture is associated to a particular fbo.
			if (p_pTex->GetFBO() == m_fbo_array[m_current_fbo])
				if ( (MOuint)MO_UNDEFINED != p_pTex->GetFBOAttachPoint() )
					// The texture is associated to a particular attachement point.
					SetDrawTexture(p_pTex->GetFBOAttachPoint());
				else
					// Attaching by GL id to the currently binded fbo.
					SetDrawTextureByGLId(p_pTex->GetGLId(), p_pTex->GetWidth(), p_pTex->GetHeight(), p_pTex->GetTexParam(), p_def_attach_point);
			else result = 0; // Failure the currently binded fbo is different to the fbo assigned to the texture.
	}
	else result = 0; // There is no fbo currently binded or the texture is NULL.
	return result;
}

MOint moFBManager::SetDrawTextures(moTextureArray &p_tex_array)
{
	MOint result = 1;
	if (-1 < m_current_fbo)
	{
		int i;
		GLsizei n = p_tex_array.Count();

		for (i = 0; i < n; i++)
			if (p_tex_array[i] != NULL)
				m_fbo_array[m_current_fbo]->SetDrawTexture(p_tex_array[i]->GetGLId(), p_tex_array[i]->GetWidth(), p_tex_array[i]->GetHeight(), p_tex_array[i]->GetTexParam(), i);

		for (int i = 0; i < n; i++) m_color_buffers[i] = m_fbo_array[m_current_fbo]->GetColorAttachPoint(i);
		glDrawBuffers(n, m_color_buffers);
	}
	return result;
}

MOint moFBManager::SetReadTexture(moTexture* p_pTex, MOuint p_def_attach_point)
{
	MOint result = 1;
	if ((-1 < m_current_fbo) && (p_pTex != NULL))
	{
		if (p_pTex->GetFBO() == NULL)
			// Attaching by GL id to the currently binded fbo.
			SetReadTextureByGLId(p_pTex->GetGLId(), p_pTex->GetWidth(), p_pTex->GetHeight(), p_pTex->GetTexParam(), p_def_attach_point);
		else
			// The texture is associated to a particular fbo.
			if (p_pTex->GetFBO() == m_fbo_array[m_current_fbo])
				if ( (MOuint)MO_UNDEFINED != p_pTex->GetFBOAttachPoint())
					// The texture is associated to a particular attachement point.
					SetReadTexture(p_pTex->GetFBOAttachPoint());
				else
					// Attaching by GL id to the currently binded fbo.
					SetReadTextureByGLId(p_pTex->GetGLId(), p_pTex->GetWidth(), p_pTex->GetHeight(), p_pTex->GetTexParam(), p_def_attach_point);
			else result = 0; // Failure the currently binded fbo is different to the fbo assigned to the texture.
	}
	else result = 0; // There is no fbo currently binded or the texture is NULL.
	return result;
}

void moFBManager::UnbindFBO()
{
	if (0 < m_fbo_stack_index)
	{
		MOuint previous_fbo = m_fbo_stack[m_fbo_stack_index];
		m_fbo_array[previous_fbo]->Bind();
        m_current_fbo = previous_fbo;

		MOuint prev_draw_buffer = m_draw_buffer_stack[m_fbo_stack_index];
		MOuint prev_read_buffer = m_read_buffer_stack[m_fbo_stack_index];

		m_glmanager->SetCurrentDrawBuffer(prev_draw_buffer);
		m_glmanager->SetCurrentReadBuffer(prev_read_buffer);

		m_fbo_stack_index--;
	}
	else
	{
	    m_glmanager->SetCurrentFBO(0);
 	    m_glmanager->SetCurrentDrawBuffer(m_screen_draw_buffer);
	    m_glmanager->SetCurrentReadBuffer(m_screen_read_buffer);
		m_fbo_stack_index = -1;
	    m_current_fbo = -1;
	}
}

void moFBManager::UnbindAllFBO()
{
    m_glmanager->SetCurrentFBO(0);
    m_glmanager->SetCurrentDrawBuffer(m_screen_draw_buffer);
    m_glmanager->SetCurrentReadBuffer(m_screen_read_buffer);
	m_fbo_stack_index = -1;
    m_current_fbo = -1;
}

void moFBManager::GetScreenBuffers()
{
	glGetIntegerv(GL_DRAW_BUFFER, &m_screen_draw_buffer);
	glGetIntegerv(GL_READ_BUFFER, &m_screen_read_buffer);
}
