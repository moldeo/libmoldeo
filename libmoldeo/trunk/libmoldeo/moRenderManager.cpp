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

#include <moRenderManager.h>
#include <moEffect.h>

#include "moArray.cpp"
moDefineDynamicArray( moRenderClips )
moDefineDynamicArray( moDisplayOutputs )

moRenderManager::moRenderManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_RENDER );
	SetName("Render Manager");

	m_pGLManager = NULL;
	m_pFBManager = NULL;
	m_pTextureManager = NULL;

	m_pDecoderManager = NULL;


	m_render_tex_moid[0] = -1;
	m_render_tex_moid[1] = -1;
	m_render_tex_moid[2] = -1;
	m_render_tex_moid[3] = -1;
	m_render_tex_moid[4] = -1;
	m_render_tex_moid[5] = -1;

    m_OutputConfiguration.m_RenderResolution.width = 0;
    m_OutputConfiguration.m_RenderResolution.height = 0;

    m_OutputConfiguration.m_OutputResolution.width = 0;
    m_OutputConfiguration.m_OutputResolution.height = 0;

    m_pFramesPool = new moBucketsPool();

	m_RenderLock.Unlock();
}

moRenderManager::~moRenderManager() {
    delete m_pFramesPool;
	m_RenderLock.Unlock();
}

bool moRenderManager::Lock() {
	return m_RenderLock.Lock();
}

bool moRenderManager::Unlock() {
	//it is not necessary to check who has unlocked the process ??
	return m_RenderLock.Unlock();
}

void    moRenderManager::SetOutputConfiguration( moRenderOutputConfiguration p_output_configuration ) {

    m_OutputConfiguration = p_output_configuration;

}

moRenderOutputConfiguration& moRenderManager::GetOutputConfiguration() {

    return m_OutputConfiguration;

}

MOboolean moRenderManager::Init( moRenderManagerMode p_render_to_texture_mode,
						   MOint p_screen_width, MOint p_screen_height,
						   MOint p_render_width, MOint p_render_height)
{

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong.
		MODebug2->Error(moText("GLEW Error: ")+ moText((char*)glewGetErrorString(err)));
		cout << glewGetErrorString(err) << endl;


	}

    MODebug2->Message( moText("Using GLEW ") + moText((char*)glewGetString(GLEW_VERSION)));
    MODebug2->Message( moText("GLEW_ARB_texture_non_power_of_two: ") + moText(IntToStr(GLEW_ARB_texture_non_power_of_two)) );
    MODebug2->Message( moText("GLEW_ARB_color_buffer_float: ") + moText(IntToStr(GLEW_ARB_color_buffer_float))) ;
    MODebug2->Message( moText("GLEW_ARB_multitexture: ") + moText(IntToStr(GLEW_ARB_multitexture))) ;
    MODebug2->Message( moText("GL_EXT_gpu_shader4: ") + moText(IntToStr(GL_EXT_gpu_shader4))) ;
    MODebug2->Message( moText("GLEW_EXT_geometry_shader4: ") + moText(IntToStr(GLEW_EXT_geometry_shader4))) ;
    MODebug2->Message( moText("GLEW_ARB_imaging: ") + moText(IntToStr(GLEW_ARB_imaging))) ;
    MODebug2->Message( moText("GLEW_ARB_shading_language_100: ") + moText(IntToStr(GLEW_ARB_shading_language_100))) ;


	m_render_to_texture_mode = p_render_to_texture_mode;

  MODebug2->Message(moText("Setting Render Manager Mode to:")+ IntToStr(m_render_to_texture_mode));

	if ( m_render_to_texture_mode == RENDERMANAGER_MODE_VDPAU ) {

	  m_pDecoderManager = m_pResourceManager->GetDecoderMan();
	  if (m_pDecoderManager!=NULL) {
        MODebug2->Message("Decoder Manager Acquired.");
    }

  }

	if (m_pResourceManager){
		m_pGLManager = m_pResourceManager->GetGLMan();
		m_pFBManager = m_pResourceManager->GetFBMan();
		m_pTextureManager = m_pResourceManager->GetTextureMan();
	}
	if (!(m_pGLManager && m_pFBManager && m_pTextureManager)) return false;

	m_render_width = p_render_width;
   	m_render_height = p_render_height;


    //if (m_render_tex_moid[0]!=-1) m_pTextureManager->DeleteTexture(m_render_tex_moid[0]);
    //if (m_render_tex_moid[1]!=-1) m_pTextureManager->DeleteTexture(m_render_tex_moid[1]);
    //if (m_render_tex_moid[2]!=-1) m_pTextureManager->DeleteTexture(m_render_tex_moid[2]);
    //if (m_render_tex_moid[3]!=-1) m_pTextureManager->DeleteTexture(m_render_tex_moid[3]);
	if (m_render_tex_moid[0]==-1) {
	    m_render_tex_moid[0] = m_pTextureManager->AddTexture("render_texture", m_render_width, m_render_height);
	}
	if (m_render_tex_moid[1]==-1) {
	    m_render_tex_moid[1] = m_pTextureManager->AddTexture("screen_texture", m_render_width, m_render_height);
	}
	if (m_render_tex_moid[2]==-1) {
	    m_render_tex_moid[2] = m_pTextureManager->AddTexture("effects_texture", m_render_width, m_render_height);
	}
	if (m_render_tex_moid[3]==-1) {
	    m_render_tex_moid[3] = m_pTextureManager->AddTexture("final_texture", m_render_width, m_render_height);
	}

	if (m_render_tex_moid[4]==-1) {
	    m_render_tex_moid[4] = m_pTextureManager->AddTexture("left_texture", m_render_width/2, m_render_height);
	}
	if (m_render_tex_moid[5]==-1) {
	    m_render_tex_moid[5] = m_pTextureManager->AddTexture("right_texture", m_render_width/2, m_render_height);
	}


    m_screen_width = p_screen_width;
   	m_screen_height = p_screen_height;


   	if (m_OutputConfiguration.m_RenderResolution.width==0) m_OutputConfiguration.m_RenderResolution.width = m_render_width;
   	if (m_OutputConfiguration.m_RenderResolution.height==0) m_OutputConfiguration.m_RenderResolution.height = m_render_height;

   	if (m_OutputConfiguration.m_OutputResolution.width==0) m_OutputConfiguration.m_OutputResolution.width = m_screen_width;
   	if (m_OutputConfiguration.m_OutputResolution.height==0) m_OutputConfiguration.m_OutputResolution.height = m_screen_height;

    m_pGLManager->SetMoldeoGLState();
	m_pGLManager->SetPerspectiveView(m_screen_width, m_screen_height);

/// && (m_render_to_texture_mode == RENDERMANAGER_MODE_FRAMEBUFFER || m_render_to_texture_mode==RENDERMANAGER_MODE_VDPAU)
	if (GLEW_EXT_framebuffer_object)
	{
	    m_pGLManager->SetFrameBufferObjectActive();
        MODebug2->Message( moText("Using framebuffer_object: creating one fbo per predefined textures (4). ") );
		m_fbo_idx = m_pFBManager->CreateFBO();
		/*
		MOuint attach_point;
		for (int i = 0; i < 4; i++)
		{
			m_pFBManager->GetFBO(m_fbo_idx)->AddTexture(m_render_width, m_render_height,
								m_pTextureManager->GetTexParam(m_render_tex_moid[i]),
								m_pTextureManager->GetGLId(m_render_tex_moid[i]), attach_point);

			moTexture* pRenderTex = m_pTextureManager->GetTexture(m_render_tex_moid[i]);
			if (pRenderTex) {
			    pRenderTex->SetFBO(m_pFBManager->GetFBO(m_fbo_idx));
			    pRenderTex->SetFBOAttachPoint(attach_point);
			    m_render_attach_points[i] = attach_point;
			}
		}
		m_pFBManager->GetFBO(m_fbo_idx)->AddDepthStencilBuffer();
		*/
	} else MODebug2->Message( moText("Framebuffer objects unavailable.") );
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
    return (m_render_to_texture_mode == RENDERMANAGER_MODE_FRAMEBUFFER) && GLEW_EXT_framebuffer_object;
}

MOboolean moRenderManager::RenderResEqualScreenRes()
{
    return (m_screen_width == m_render_width) && (m_screen_height == m_render_height);
}

void moRenderManager::SetRenderToTexMode( moRenderManagerMode p_render_to_texture_mode)
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
  /*
	if (IsRenderToFBOEnabled())
		m_pFBManager->BindFBO(m_fbo_idx, m_render_attach_points[0]);*/
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
    ///add last frame to bucket pool
/*
    moBucket *pbucket=NULL;
    void*   pbuffer;
    int     size;

    if (m_pFramesPool)
   if(!m_pFramesPool->IsFull()) {

        moTexture* pFinalTexture = m_pTextureManager->GetTexture( m_pTextureManager->m_preview_texture_idx );

        size = pFinalTexture->GetWidth() * pFinalTexture->GetHeight() * 3;

        pbucket = new moBucket();
        if(pbucket!=NULL) {

            ///build buffer with 0
            pbucket->BuildBucket( size, 0 );

            ///populate buffer with tex image
            pFinalTexture->GetBuffer( (void*)pbucket->GetBuffer(), GL_BGR_EXT, GL_UNSIGNED_BYTE );

            if(m_pFramesPool->AddBucket( pbucket )) {

                //Width = 720;
                //Height = 480;
            }
        }
    }

*/
/*
	if (IsRenderToFBOEnabled())
		m_pFBManager->UnbindFBO();*/
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
