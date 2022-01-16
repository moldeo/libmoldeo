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


*******************************************************************************/

#include <moRenderManager.h>
#include <moEffect.h>


#include "moArray.h"
moDefineDynamicArray( moRenderClips )
moDefineDynamicArray( moDisplays )

moDisplay::moDisplay() {
  m_DisplayResolution.width = 0;
  m_DisplayResolution.height = 0;
  m_DisplayResolution.aspect = 1.0;
}

moDisplay::moDisplay( int w, int h) {
  m_DisplayResolution.width = w;
  m_DisplayResolution.height = h;
  m_DisplayResolution.aspect = 1.0;
  if ( !(h==0) != !(w==0) ) {
    m_DisplayResolution.aspect = (int)(h==0) - (int) (w==0);
  }
  if (h!=0 && w!=0) {
    m_DisplayResolution.aspect =(float)w;
    m_DisplayResolution.aspect*= 1.0 / ((float)h);
  }
}

moDisplay::moDisplay( const moDisplay& p_src ) {
  (*this)=p_src;
}

moDisplay::~moDisplay() {

}
moDisplay&
moDisplay::operator= ( const moDisplay& p_src ) {

  m_DisplayResolution = p_src.m_DisplayResolution;
  return (*this);
}





moRenderManager::moRenderManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_RENDER );
	SetName("rendermanager");
	SetLabelName("rendermanager");

  m_pResourceManager = NULL;
	m_pGLManager = NULL;
	m_pFBManager = NULL;
	m_pTextureManager = NULL;
	m_pDecoderManager = NULL;
	m_pSHManager = NULL;


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

int   moRenderManager::CreateLayer( moEffect* p_scene_effect ) {
  p_scene_effect = NULL;
  return 0;
}

void   moRenderManager::RenderLayer( int layer_id ) {
  layer_id = 0;
}

MOboolean moRenderManager::Init( moRenderManagerMode p_render_to_texture_mode,
						   MOint p_screen_width, MOint p_screen_height,
						   MOint p_render_width, MOint p_render_height)
{

	if (m_pResourceManager){
		m_pGLManager = m_pResourceManager->GetGLMan();
		m_pFBManager = m_pResourceManager->GetFBMan();
		m_pTextureManager = m_pResourceManager->GetTextureMan();
		m_pSHManager =  m_pResourceManager->GetShaderMan();
	}
	if (!(m_pGLManager && m_pFBManager && m_pTextureManager && m_pSHManager)) return false;

	m_render_to_texture_mode = p_render_to_texture_mode;
	m_render_width = p_render_width;
  m_render_height = p_render_height;
  m_screen_width = p_screen_width;
  m_screen_height = p_screen_height;

  MODebug2->Message(moText("moRenderManager::Init > Setting Render Manager Mode to:")+ IntToStr(m_render_to_texture_mode));
  MODebug2->Message(moText("moRenderManager::Init > Setting Render Manager for Render: ")+ IntToStr(m_render_width)+"x"+ IntToStr(m_render_height)
                          + " for Screen: " + IntToStr(m_screen_width)+"x"+ IntToStr(m_screen_height)  );

	if ( m_render_to_texture_mode == RENDERMANAGER_MODE_VDPAU ) {

	  m_pDecoderManager = m_pResourceManager->GetDecoderMan();
	  if (m_pDecoderManager!=NULL) {
        MODebug2->Message("moRenderManager::Init > Decoder Manager Acquired.");
    }
  }

/*
  if ( m_pGLManager->CreateContext( m_screen_width, m_screen_height )) {
      MODebug2->Error("moRenderManager::Init > Couldn't create GL Context");
      return false;
  } else {
      MODebug2->Message("moRenderManager::Init > GL Context Created");
  }
*/
    /*** GLEW INIT */
#ifndef OPENGLESV2
  MODebug2->Message("moRenderManager::Init > Initializing glew.");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong.
		MODebug2->Error(moText("moRenderManager::Init > GLEW Error: ")+ moText((char*)glewGetErrorString(err)));
		cout << glewGetErrorString(err) << endl;
		return false;

	}
  MODebug2->Message("moRenderManager::Init > glew initialized succesfully!");

  GLint max_tex_size = 0.0f;
  GLint max_tex_rect_size = 0.0f;
  GLint max_tex_buf_size = 0.0f;
//  GLint64 max_tex_buf_size64 = 0.0f;
//  GLint max_tex_buf_size_arb = 0.0f;


  MODebug2->Message( moText("moRenderManager::Init > Using GLEW ") + moText((char*)glewGetString(GLEW_VERSION)) );
  MODebug2->Message( moText("moRenderManager::Init >       glActiveTextureARB: ") + IntToStr( (long)glActiveTextureARB) );
  MODebug2->Message( moText("moRenderManager::Init >       glMultiTexCoord2fARB: ") + IntToStr( (long)glMultiTexCoord2fARB ) );
  MODebug2->Message( moText("moRenderManager::Init >       GLEW_ARB_texture_non_power_of_two: ") + IntToStr(GLEW_ARB_texture_non_power_of_two) );
  MODebug2->Message( moText("moRenderManager::Init >       GLEW_ARB_color_buffer_float: ") + IntToStr(GLEW_ARB_color_buffer_float) );
  MODebug2->Message( moText("moRenderManager::Init >       GLEW_ARB_multitexture: ") + IntToStr(GLEW_ARB_multitexture) );
  MODebug2->Message( moText("moRenderManager::Init >       GL_EXT_gpu_shader4: ") + IntToStr(GL_EXT_gpu_shader4) );
  MODebug2->Message( moText("moRenderManager::Init >       GLEW_EXT_geometry_shader4: ") + IntToStr(GLEW_EXT_geometry_shader4) );
  MODebug2->Message( moText("moRenderManager::Init >       GLEW_ARB_imaging: ") + IntToStr(GLEW_ARB_imaging) );
  MODebug2->Message( moText("moRenderManager::Init >       GLEW_ARB_shading_language_100: ") + IntToStr(GLEW_ARB_shading_language_100) );
  MODebug2->Message( moText("moRenderManager::Init >       GLEW_EXT_framebuffer_object: ") + IntToStr(GLEW_EXT_framebuffer_object) );
  MODebug2->Message( moText("moRenderManager::Init >       GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB: ") + IntToStr(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB) );
  MODebug2->Message( moText("moRenderManager::Init >       GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB: ") + IntToStr(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB) );
  MODebug2->Message( moText("moRenderManager::Init >       GL_MAX_COLOR_ATTACHMENTS: ") + IntToStr(GL_MAX_COLOR_ATTACHMENTS) );

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
  MODebug2->Message( moText("moRenderManager::Init >       Max Texture Size: ") + IntToStr(max_tex_size) );
  glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &max_tex_rect_size);
  MODebug2->Message( moText("moRenderManager::Init >       Max Texture Rectangle Size: ") + IntToStr(max_tex_rect_size) );
  glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &max_tex_buf_size);
  MODebug2->Message( moText("moRenderManager::Init >       Max Texture Buffer Size: ") + IntToStr(max_tex_buf_size) );
  //glGetInteger64v(GL_MAX_TEXTURE_BUFFER_SIZE, &max_tex_buf_size64);
  //MODebug2->Message( moText("moRenderManager::Init >       Max Texture Buffer Size (64): ") + IntToStr(max_tex_buf_size64) );
  //glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE_ARB, &max_tex_buf_size_arb);
  //MODebug2->Message( moText("moRenderManager::Init >       Max Texture Buffer Size Arb: ") + IntToStr(max_tex_buf_size_arb) );

#endif
	if (m_render_tex_moid[0]==-1) {
	    m_render_tex_moid[0] = m_pTextureManager->AddTexture("render_texture", m_render_width, m_render_height);
	} else {
	    //resize textures
	    m_pTextureManager->GetTexture( m_render_tex_moid[0])->BuildEmpty(m_render_width, m_render_height);
	}


	if (m_render_tex_moid[1]==-1) {
	    m_render_tex_moid[1] = m_pTextureManager->AddTexture("screen_texture", m_render_width, m_render_height);
	} else {
	    //resize textures
	    m_pTextureManager->GetTexture( m_render_tex_moid[1])->BuildEmpty(m_render_width, m_render_height);
	}


	if (m_render_tex_moid[2]==-1) {
	    m_render_tex_moid[2] = m_pTextureManager->AddTexture("effects_texture", m_render_width, m_render_height);
	} else {
	    //resize textures
	    m_pTextureManager->GetTexture( m_render_tex_moid[2])->BuildEmpty(m_render_width, m_render_height);
	}


	if (m_render_tex_moid[3]==-1) {
	    m_render_tex_moid[3] = m_pTextureManager->AddTexture("final_texture", m_render_width, m_render_height);
	} else {
	    //resize textures
	    m_pTextureManager->GetTexture( m_render_tex_moid[3])->BuildEmpty(m_render_width, m_render_height);
	}



	if (m_render_tex_moid[4]==-1) {
	    m_render_tex_moid[4] = m_pTextureManager->AddTexture("left_texture", m_render_width/2, m_render_height);
	} else {
	    //resize textures
	    m_pTextureManager->GetTexture( m_render_tex_moid[4])->BuildEmpty(m_render_width, m_render_height);
	}
	if (m_render_tex_moid[5]==-1) {
	    m_render_tex_moid[5] = m_pTextureManager->AddTexture("right_texture", m_render_width/2, m_render_height);
	} else {
	    //resize textures
	    m_pTextureManager->GetTexture( m_render_tex_moid[5])->BuildEmpty(m_render_width, m_render_height);
	}


   	if (m_OutputConfiguration.m_RenderResolution.width==0) m_OutputConfiguration.m_RenderResolution.width = m_render_width;
   	if (m_OutputConfiguration.m_RenderResolution.height==0) m_OutputConfiguration.m_RenderResolution.height = m_render_height;

   	if (m_OutputConfiguration.m_OutputResolution.width==0) m_OutputConfiguration.m_OutputResolution.width = m_screen_width;
   	if (m_OutputConfiguration.m_OutputResolution.height==0) m_OutputConfiguration.m_OutputResolution.height = m_screen_height;

    m_pGLManager->SetMoldeoGLState();
	m_pGLManager->SetPerspectiveView(m_screen_width, m_screen_height);

/// && (m_render_to_texture_mode == RENDERMANAGER_MODE_FRAMEBUFFER || m_render_to_texture_mode==RENDERMANAGER_MODE_VDPAU)

	if ( /*IsRenderToFBOEnabled()*/ GLEW_EXT_framebuffer_object )
	{
	    m_pGLManager->SetFrameBufferObjectActive();
        MODebug2->Message( moText("moRenderManager::Init > Using framebuffer_object: creating one fbo per predefined textures (4). ") );
		m_fbo_idx = m_pFBManager->CreateFBO();

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

	} else MODebug2->Message( moText("moRenderManager::Init > Framebuffer objects unavailable.") );
	return true;
}

MOboolean moRenderManager::Finish()
{
	if (IsRenderToFBOEnabled()) m_pFBManager->DeleteFBO(m_fbo_idx);

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
	if (m_pGLManager)
        m_pGLManager->SaveGLState();
}

void moRenderManager::EndUpdateDevice()
{
    if (m_pGLManager)
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
		if (m_pFBManager) m_pFBManager->BindFBO(m_fbo_idx, m_render_attach_points[0]);

    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
}

void moRenderManager::BeginDrawEffect()
{
  Lock();
	if (m_pGLManager) m_pGLManager->SaveGLState();
	if (m_pGLManager) m_pGLManager->SetPerspectiveView(m_render_width, m_render_height);
	m_saved_screen = false;
}

int
moRenderManager::Render( moObject3D* p_pObj, moCamera3D* p_pCamera ) {

    if (p_pObj==NULL || p_pCamera==NULL) return 0;
    if (!m_pTextureManager) return 0;
    if (!m_pGLManager) return 0;
    if (!m_pSHManager) return 0;
    if ( !m_pSHManager->GetRenderShader().Initialized() ) return 0;

    if (p_pObj->GetChilds().Count()) {
      for(int child=0; child<p_pObj->GetChilds().Count(); child++) {
        moObject3D* p_child_obj = (moObject3D*)p_pObj->GetChilds().Get(child);
        Render( p_child_obj, p_pCamera );
      }
    }

/*
    const float vx[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
         0.5f, 0.5f, 0.0f,

         0.5f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         -0.5f, -0.5f, 0.0f
    };
    const float cx[] = {
        1.0f, 0.5f, 0.5f,
        0.5f, 1.0f, 0.5f,
        0.5f, 0.5f, 1.0f,

        0.5f, 0.5f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };
    const float tx[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };*/

    //moGLManager* pGLMan = m_pGLManager;

    moGeometry& Geo( p_pObj->m_Geometry );
    moMaterial& Mat( p_pObj->m_Material );
    //const moGLMatrixf& PMatrix( p_src.GetProjection() );
    moGLMatrixf& PMatrix( *p_pCamera );
    const moGLMatrixf& MMatrix( p_pObj->GetModelMatrix() );
    moGLMatrixf Result;
    Result = MMatrix*PMatrix;


    MOuint color_index = m_pSHManager->GetRSHColorIndex();
    MOuint opacity_index = m_pSHManager->GetRSHOpacityIndex();
    MOuint colors_index = m_pSHManager->GetRSHColorsIndex();
    MOuint position_index = m_pSHManager->GetRSHPositionIndex();
    MOuint normal_index = m_pSHManager->GetRSHNormalIndex();
    MOuint texcoord_index = m_pSHManager->GetRSHTexCoordIndex();
    MOuint texcoordedge_index = m_pSHManager->GetRSHTexCoordEdgeIndex();
    MOuint matrix_index = m_pSHManager->GetRSHProjectionMatrixIndex();
    MOuint texture_index = m_pSHManager->GetRSHTextureIndex();
    MOuint wireframe_index = m_pSHManager->GetRSHWireframeWidthIndex();
    MOuint tex_wsegments_index = m_pSHManager->GetRSHTexWSegmentsIndex();
    MOuint tex_hsegments_index = m_pSHManager->GetRSHTexHSegmentsIndex();
    MOuint light_index = m_pSHManager->GetRSHLightIndex();

    if ( m_pSHManager->GetRenderShader().Initialized() ) {
        m_pSHManager->GetRenderShader().StartShader();
    } else return 0;
    //m_pGLManager->SetDefaultPerspectiveView( ScreenWidth(), ScreenHeight() );
    //moGLMatrixf& PMatrix( pGLMan->GetProjectionMatrix() );
    //moGLMatrixf& MMatrix( pGLMan->GetModelMatrix() );
    const moFaceArray& Faces(Geo.GetFaces());
    //const moVertexArray& Vertices(Geo.GetVertices());
    const float* Gpx = Geo.GetVerticesBuffer();
    const float* Gcx = Geo.GetColorBuffer();
    const float* Gtx = Geo.GetVerticesUVBuffer();
    const float* Gnx = Geo.GetNormalsBuffer();

    int facesCount = Faces.Count();


    //PMatrix.MakeIdentity();
    //MMatrix.MakeIdentity();
    //MMatrix.Scale( 0.5, 0.5, 0.5 );
    //MMatrix.Rotate( ((float)steps/(float)stepi)*1.0*moMathf::DEG_TO_RAD, 1.0, 1.0, 1.0 );
    //MMatrix.Translate( 0.5f, 0.5f, -steps/1000.0f-3.0f );
    //MMatrix.Translate( 0.0f, 0.0f, -4.0f );
    //MODebug2->Message( "model:\n"+MMatrix.ToJSON() );
    //MODebug2->Message( "projection\n"+PMatrix.ToJSON() );
    //Result = MMatrix*PMatrix;

    const float *pfv = Result.GetPointer();
    //MODebug2->Message( "Result:\n"+Result.ToJSON() );

    moTexture* pMap = Mat.m_Map;
    if (pMap) {
        //int Tglid = pMap->GetGLId();
        glEnable( GL_TEXTURE_2D );
        glActiveTexture( GL_TEXTURE0 );///ACTIVATE TEXTURE UNIT 0
        glBindTexture( GL_TEXTURE_2D, Mat.m_MapGLId );
        //MODebug2->Message( "Tglid:\n"+IntToStr(Tglid) );
    }
    glUniformMatrix4fv( matrix_index, 1, GL_FALSE, pfv );
    glUniform1i( texture_index, 0 );///Pass TEXTURE UNIT 0 (use glActiveTexture and glBindTexture )
    glUniform1f( wireframe_index, Mat.m_fWireframeWidth );
    glUniform1f( tex_wsegments_index, Mat.m_fTextWSegments );
    glUniform1f( tex_hsegments_index, Mat.m_fTextHSegments );
    glUniform3fv( light_index, 1, &Mat.m_vLight[0] );
    glUniform3fv( color_index, 1, &Mat.m_Color[0] );
    glUniform1f( opacity_index, Mat.m_fOpacity );

    glEnableVertexAttribArray( position_index );
    glVertexAttribPointer( position_index, 3, GL_FLOAT, false, 0, &Gpx[0] );  // Set data type and location.

    glEnableVertexAttribArray( colors_index );
    glVertexAttribPointer( colors_index, 3, GL_FLOAT, false, 0, &Gcx[0] );

    glEnableVertexAttribArray( texcoord_index );
    glVertexAttribPointer( texcoord_index, 2, GL_FLOAT, false, 0, &Gtx[0] );  // Set data type and location.

    glEnableVertexAttribArray( texcoordedge_index );
    glVertexAttribPointer( texcoordedge_index, 2, GL_FLOAT, false, 0, &Gtx[0] );  // Set data type and location.
    //int vertexCount = p_src.m_Geometry.GetVertices().Count();
    //int facesCount = p_src.m_Geometry.GetFaces().Count();
    glEnableVertexAttribArray( normal_index );
    glVertexAttribPointer( normal_index, 3, GL_FLOAT, false, 0, &Gnx[0] );

    switch(Mat.m_PolygonMode) {

        case MO_POLYGONMODE_LINE:
            for(int i = 0; i < facesCount*3; i += 3)
                glDrawArrays(GL_LINE_LOOP, i, 3);
            break;

        case MO_POLYGONMODE_POINT:
                glDrawArrays(GL_POINTS, 0, facesCount*3 );
            break;

        case MO_POLYGONMODE_FILL:
        default:
            glDrawArrays( GL_TRIANGLES, 0, facesCount*3 ); //
            break;

    }

    glDisableVertexAttribArray( position_index );
    glDisableVertexAttribArray( colors_index );
    glDisableVertexAttribArray( texcoord_index );
    glDisableVertexAttribArray( texcoordedge_index );
    glDisableVertexAttribArray( normal_index );

    if (m_pSHManager->GetRenderShader().Initialized()) {
        m_pSHManager->GetRenderShader().StopShader();
    }


    return 1;
}


void moRenderManager::EndDrawEffect()
{
    if (m_pGLManager) m_pGLManager->RestoreGLState();
    Unlock();
}

void moRenderManager::EndDraw()
{
  uint8_t* ImageBuffer = new uint8_t[ 1 * 1 * 4];
  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, ImageBuffer);
  //MODebug2->Message("RGBA: "+IntToStr(ImageBuffer[0]) +","+ IntToStr(ImageBuffer[1])+","+IntToStr(ImageBuffer[2])+","+IntToStr(ImageBuffer[3]));

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

void moRenderManager::DrawTexture( MOint p_width, MOint p_height, MOint p_tex_num )
{
	if (ValidSourceTexNum(p_tex_num) && m_pGLManager )
	{
        Lock();

	    m_pGLManager->SaveGLState();

        m_pGLManager->SetOrthographicView(p_width, p_height, 0.0, p_width, 0.0, p_height );

		moTexture* ptex = m_pTextureManager->GetTexture( m_render_tex_moid[p_tex_num] );

        glBindTexture(GL_TEXTURE_2D, ptex->GetGLId());
        glEnable(GL_ALPHA);
        glDisable(GL_DEPTH_TEST);       // Disables Depth Testing
        glDepthMask(GL_FALSE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

#ifndef OPENGLESV2
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
#else

#endif
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

int screenshots_c = 0;
int previewshotmax = 4;

//moText pathname, moText& screenshot_result
bool moRenderManager::PreviewShot( bool shot_start ) {

/*start absolute timer, one shot each*/
  if( shot_start && !m_PreviewShotTimer.Started()) {
    m_PreviewShotTimer.Stop();
    m_PreviewShotTimer.Start();
    m_PreviewShotIteration = 0;
  }

  if( m_PreviewShotTimer.Started()) {
    if (m_PreviewShotTimer.Duration()>=1000) {

      moText sshot = m_pResourceManager->GetDataMan()->GetDataPath()+moSlash+moText("previewshots");
      moText preview_shot_filename = moText("preview_shot_")+IntToStr(m_PreviewShotIteration,7);
      moText result;
      m_pResourceManager->GetRenderMan()->Screenshot( sshot, result, "JPG", preview_shot_filename );
      MODebug2->Message( "moRenderManager::PreviewShot() > " + result );
      m_PreviewShotIteration+=1;
      m_PreviewShotTimer.Stop();
      m_PreviewShotTimer.Start();
      if (m_PreviewShotIteration==previewshotmax) {
        m_PreviewShotTimer.Stop();
      }
    }
  }

  return true;
}

bool moRenderManager::Screenshot( moText pathname, moText& screenshot_result, const moText& image_format, const moText& file_pattern ) {

        if (m_render_tex_moid[MO_SCREEN_TEX]) {

            moTexture* TexScreen = m_pTextureManager->GetTexture(  m_render_tex_moid[MO_SCREEN_TEX]  );

            if (TexScreen) {
                /** Atamos la textura de TexScreen : SCREEN_TEXTURE*/
                glBindTexture(GL_TEXTURE_2D, TexScreen->GetGLId()  );
                /** TODO: Copia los pixeles de la pantalla a la textura atada
                *    aqui podriamos usar el glCopyTexSubImage....(para mantener potencias de dos)
                */
                glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, TexScreen->GetWidth(), TexScreen->GetHeight(), 0 );
                glBindTexture(GL_TEXTURE_2D, 0);


                time_t rawtime;
                char strbuffer[0x100];
                struct tm * timeinfo;

                //srand(2);
                //int randcode = rand();


                time(&rawtime);
                timeinfo = localtime ( &rawtime );
                strftime ( strbuffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo );
                moText datetime = strbuffer;
                strftime ( strbuffer, 80, "%Y-%m-%d", timeinfo );
                moText date = strbuffer;


                moText screenshot_filename;
                moText pat_date = moText("{DATE}");
                moText pat_date_time = moText("{DATETIME}");
                moText pat_number = moText("{####}");
                screenshot_filename = file_pattern;
                //screenshot_filename.Replace( pat_date, date );
                //screenshot_filename.Replace( pat_date_time, datetime );
                //screenshot_filename.Replace( pat_number, IntToStr( screenshots_c, 4 ) );
                if (screenshot_filename=="") {
                    screenshot_filename= "screen_" + IntToStr( screenshots_c, 7 );
                    screenshots_c+= 1;
                }

                if (
                    screenshot_filename=="image_{DATETIME}_{####}.png"
                    ) {

                 //=="image_{DATETIME}_{####}.png")
                  screenshot_filename= "screen_" + IntToStr( screenshots_c, 7 );
                  screenshots_c+= 1;

                }



                moText screenshot_name;
                moText screenshot_path;
                if (pathname=="") {
                    screenshot_path = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + moText("moldeo_screenshots");

                    //screenshot_name = screenshot_path + moSlash + moText(datetime)+ moText("_") + IntToStr(screenshots_c);
                    screenshot_name = screenshot_path + moSlash + screenshot_filename;
                } else {
                    screenshot_path = pathname;
                    //screenshot_name = screenshot_path + moSlash + moText(datetime)+ moText("_")+ IntToStr(screenshots_c);
                    screenshot_name = screenshot_path + moSlash + screenshot_filename;
                }

                /** crear directorio si no existe*/
                //if ( !moDirectory( screenshot_path ).Exists() ) {
                if (!moFileManager::DirectoryExists(screenshot_path)) {
                    if (!moFileManager::CreateDirectory(screenshot_path) ) {
                        MODebug2->Error("could not create directory: "+ screenshot_path);
                        return false;
                    }
                }

                /** Generamos un archivo de imagen a partir de la textura TexScreen actualizada*/
                screenshot_result = TexScreen->CreateThumbnail( image_format, m_pResourceManager->GetRenderMan()->ScreenWidth(), m_pResourceManager->GetRenderMan()->ScreenHeight(), screenshot_name );

            }

        }


        return true;

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

    moTexture* pScreenText;

    pScreenText = m_pTextureManager->GetTexture( m_render_tex_moid[p_tex_num] );

    if (pScreenText) {
        glBindTexture(GL_TEXTURE_2D, pScreenText->GetGLId() );
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_render_width, m_render_height, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

		if (IsRenderToFBOEnabled()) m_pFBManager->UnbindFBO();
		else m_pFBManager->UnbindScreenFB();
	}
}

MOint moRenderManager::RenderTexGLId(MOint p_tex_num)
{
	if (ValidSourceTexNum(p_tex_num)) return m_pTextureManager->GetGLId(m_render_tex_moid[p_tex_num]);
	else return -1;
}


MOboolean moRenderManager::MultiTextureSupported() const
{
	return GLEW_ARB_multitexture;
}

MOboolean moRenderManager::FramebufferObjectSupported() const
{
	return GLEW_EXT_framebuffer_object;
}

MOboolean moRenderManager::ShadersSupported() const
{
	return GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_shading_language_100;

}

MOboolean moRenderManager::IsTextureNonPowerOf2Disabled() const {
  return !(GLEW_ARB_texture_non_power_of_two);
}

MOint moRenderManager::ScreenWidth()  const {
/*
    GLint  	params[4];

    glGetIntegerv( GL_VIEWPORT, &params[0]);

    m_screen_width = params[2];
    m_screen_height = params[3];
*/
    return m_screen_width;
}

MOint moRenderManager::ScreenHeight()  const {
/*
    GLint   	params[4];

    glGetIntegerv( GL_VIEWPORT, &params[0]);

    m_screen_width = params[2];
    m_screen_height = params[3];
*/
    return m_screen_height;
}

MOint moRenderManager::InterfaceWidth() const {
    return m_interface_width;
}

MOint moRenderManager::InterfaceHeight() const {

    return m_interface_height;
}


float moRenderManager::ScreenProportion()  const {

    float prop = 1.0;
    int w = ScreenWidth();
    int h = ScreenHeight();

    if ( w == 0 || h == 0 ) {
        w  = 1; h = 1; prop = 1.0;
    }
    else {
        prop = (float) h / (float) w;
    }

    return prop;

}


MOint moRenderManager::RenderWidth()  const {
    return m_render_width;
}

MOint moRenderManager::RenderHeight()  const {
    return m_render_height;
}

float moRenderManager::RenderProportion()  const {

    float prop = 1.0;
    int w = RenderWidth();
    int h = RenderHeight();

    if ( w == 0 || h == 0 ) {
        w  = 1; h = 1; prop = 1.0;
    }
    else {
        prop = (float) h / (float) w;
    }

    return prop;

}

void moRenderManager::SetView( int p_width, int p_height ) {

    glViewport(0, 0, p_width, p_height);
    m_screen_width = p_width;
	m_screen_height = p_height;
	m_pGLManager->SetPerspectiveView(m_screen_width, m_screen_height);



}


void moRenderManager::SetInterfaceView( int p_width, int p_height ) {
  m_interface_width = p_width;
  m_interface_height = p_height;

}
