/*******************************************************************************

                              moRenderManager.cpp

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

#include "moRenderManager.h"

moRenderManager::moRenderManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_RENDER );
	SetName("Render Manager");

	m_pGLManager = NULL;
	m_pFBManager = NULL;
	m_pTextureManager = NULL;

	m_RenderLock.Unlock();
}

moRenderManager::~moRenderManager() {
	m_RenderLock.Unlock();
}

bool moRenderManager::Lock() {
	return m_RenderLock.Lock();
}

bool moRenderManager::Unlock() {
	//it is not necessary to check who has unlocked the process ??
	return m_RenderLock.Unlock();
}

MOboolean moRenderManager::Init(MOint p_render_to_texture_mode,
						   MOint p_screen_width, MOint p_screen_height,
						   MOint p_render_width, MOint p_render_height)
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		//MODebug->Push("GLEW Error: "+ moText(glewGetErrorString(err)));
	}
    //MODebug->Push("Using GLEW " + moText(glewGetString(GLEW_VERSION)));

	m_render_to_texture_mode = p_render_to_texture_mode;

	if (m_pResourceManager){
		m_pGLManager = m_pResourceManager->GetGLMan();
		m_pFBManager = m_pResourceManager->GetFBMan();
		m_pTextureManager = m_pResourceManager->GetTextureMan();
	}
	if (!(m_pGLManager && m_pFBManager && m_pTextureManager)) return false;

	m_render_width = p_render_width;
   	m_render_height = p_render_height;

	m_render_tex_moid[0] = m_pTextureManager->AddTexture("render_texture", m_render_width, m_render_height);
	m_render_tex_moid[1] = m_pTextureManager->AddTexture("screen_texture", m_render_width, m_render_height);
	m_render_tex_moid[2] = m_pTextureManager->AddTexture("effects_texture", m_render_width, m_render_height);
	m_render_tex_moid[3] = m_pTextureManager->AddTexture("final_texture", m_render_width, m_render_height);

    m_screen_width = p_screen_width;
   	m_screen_height = p_screen_height;

    m_pGLManager->SetMoldeoGLState();
	m_pGLManager->SetPerspectiveView(m_screen_width, m_screen_height);

	if (GLEW_EXT_framebuffer_object)
	{
		m_fbo_idx = m_pFBManager->CreateFBO();
		MOuint attach_point;
		for (int i = 0; i < 4; i++)
		{
			m_pFBManager->GetFBO(m_fbo_idx)->AddTexture(m_render_width, m_render_height,
								m_pTextureManager->GetTexParam(m_render_tex_moid[i]),
								m_pTextureManager->GetGLId(m_render_tex_moid[i]), attach_point);
			m_pTextureManager->GetTexture(m_render_tex_moid[i])->SetFBO(m_pFBManager->GetFBO(m_fbo_idx));
			m_pTextureManager->GetTexture(m_render_tex_moid[i])->SetFBOAttachPoint(attach_point);
			m_render_attach_points[i] = attach_point;
		}
		m_pFBManager->GetFBO(m_fbo_idx)->AddDepthStencilBuffer();
	}
	return true;
}

MOboolean moRenderManager::Finish()
{
	if (GLEW_EXT_framebuffer_object) m_pFBManager->DeleteFBO(m_fbo_idx);

	m_pGLManager = NULL;
	m_pFBManager = NULL;
	m_pTextureManager = NULL;

	return true;
}

MOboolean moRenderManager::IsRenderToFBOEnabled()
{
    return (m_render_to_texture_mode == MO_RENDER_TO_TEXTURE_FBOBJECT) && GLEW_EXT_framebuffer_object;
}

MOboolean moRenderManager::RenderResEqualScreenRes()
{
    return (m_screen_width == m_render_width) && (m_screen_height == m_render_height);
}

void moRenderManager::SetRenderToTexMode(MOint p_render_to_texture_mode)
{
    m_render_to_texture_mode = p_render_to_texture_mode;
}

void moRenderManager::BeginUpdate()
{
}

void moRenderManager::BeginUpdateDevice()
{
	Lock();
	m_pGLManager->SaveGLState();
}

void moRenderManager::EndUpdateDevice()
{
    m_pGLManager->RestoreGLState();
    Unlock();
}

void moRenderManager::EndUpdate()
{
}

void moRenderManager::BeginUpdateObject()
{
	Lock();
	m_pGLManager->SaveGLState();
}

void moRenderManager::EndUpdateObject()
{
    m_pGLManager->RestoreGLState();
    Unlock();
}

void moRenderManager::BeginDraw()
{
	if (IsRenderToFBOEnabled())
		m_pFBManager->BindFBO(m_fbo_idx, m_render_attach_points[0]);
}

void moRenderManager::BeginDrawEffect()
{
    Lock();
	m_pGLManager->SaveGLState();
	m_pGLManager->SetPerspectiveView(m_render_width, m_render_height);
	m_saved_screen = false;
}

void moRenderManager::EndDrawEffect()
{
    m_pGLManager->RestoreGLState();
    Unlock();
}

void moRenderManager::EndDraw()
{
	if (IsRenderToFBOEnabled())
		m_pFBManager->UnbindFBO();
}

void moRenderManager::DrawTexture(MOint p_resolution, MOint p_tex_num)
{
	if (p_resolution == MO_SCREEN_RESOLUTION) DrawTexture(m_screen_width, m_screen_height, p_tex_num);
	else if (p_resolution == MO_RENDER_RESOLUTION) DrawTexture(m_render_width, m_render_height, p_tex_num);
}

void moRenderManager::DrawTexture(MOint p_width, MOint p_height, MOint p_tex_num)
{
	if (ValidSourceTexNum(p_tex_num))
	{
        Lock();
	    m_pGLManager->SaveGLState();

        m_pGLManager->SetOrthographicView(p_width, p_height);

		moTexture* ptex = m_pTextureManager->GetTexture(m_render_tex_moid[p_tex_num]);
        glBindTexture(GL_TEXTURE_2D, ptex->GetGLId());
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0.0);
            glVertex2f(0, 0);

            glTexCoord2f(ptex->GetMaxCoordS(), 0.0);
            glVertex2f(p_width, 0);

            glTexCoord2f(ptex->GetMaxCoordT(), ptex->GetMaxCoordS());
            glVertex2f(p_width, p_height);

            glTexCoord2f(0, ptex->GetMaxCoordT());
            glVertex2f(0, p_height);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);

	    m_pGLManager->RestoreGLState();
        Unlock();
	}
}

void moRenderManager::SaveScreen()
{
	if (!m_saved_screen)
	{
		CopyRenderToTexture(1);
		m_saved_screen = true;
	}
}

void moRenderManager::CopyRenderToTexture(MOint p_tex_num)
{
	if (ValidDestTexNum(p_tex_num))
	{

		if (IsRenderToFBOEnabled())
		{
		    m_pFBManager->BindFBO(m_fbo_idx);
			m_pFBManager->SetReadTexture(m_render_attach_points[0]);
		}
		else m_pFBManager->BindScreenFB();

		glBindTexture(GL_TEXTURE_2D, m_pTextureManager->GetGLId(m_render_tex_moid[p_tex_num]));
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_render_width, m_render_height, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (IsRenderToFBOEnabled()) m_pFBManager->UnbindFBO();
		else m_pFBManager->UnbindScreenFB();
	}
}

MOint moRenderManager::RenderTexGLId(MOint p_tex_num)
{
	if (ValidSourceTexNum(p_tex_num)) return m_pTextureManager->GetGLId(m_render_tex_moid[p_tex_num]);
	else return -1;
}


MOboolean moRenderManager::MultiTextureSupported()
{
	return GLEW_ARB_multitexture;
}

MOboolean moRenderManager::FramebufferObjectSupported()
{
	return GLEW_EXT_framebuffer_object;
}

MOboolean moRenderManager::ShadersSupported()
{
	return GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_shading_language_100;
}

MOint moRenderManager::ScreenWidth() {

    GLint  	params[4];

    glGetIntegerv( GL_VIEWPORT, &params[0]);

    m_screen_width = params[2];
    m_screen_height = params[3];

    return m_screen_width;
}

MOint moRenderManager::ScreenHeight() {

    GLint   	params[4];

    glGetIntegerv( GL_VIEWPORT, &params[0]);

    m_screen_width = params[2];
    m_screen_height = params[3];

    return m_screen_height;
}


void moRenderManager::SetView( int p_width, int p_height ) {

    glViewport(0, 0, p_width, p_height);
    m_screen_width = p_width;
	m_screen_height = p_height;
	m_pGLManager->SetPerspectiveView(m_screen_width, m_screen_height);



}
