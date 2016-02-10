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


*******************************************************************************/

#include "moGLManager.h"
#include "moRenderManager.h"

#ifdef MO_LINUX
extern "C"
{
  #include "gtk/gtk.h"
  #include "gdk/gdk.h"
  #include "gdk/gdkx.h"
}
#endif


const moGLMatrixf moGLMatrixf::ZERO( moMatrix4f(
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f) );
const moGLMatrixf moGLMatrixf::IDENTITY( moMatrix4f(
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f));

moGLMatrixf::moGLMatrixf( const moGLMatrixf& rkM ) : moMatrix4f( rkM.GetPointer() ) {
    //moMatrix4f *m = (moMatrix4f *) rkM;
/*    SetRow( 0, rkM.GetRow(0));
    SetRow( 1, rkM.GetRow(1));
    SetRow( 2, rkM.GetRow(2));
    SetRow( 3, rkM.GetRow(3));
    */
}



moGLMatrixf&
moGLMatrixf::operator= (const moGLMatrixf& rkM ) {

  SetRow( 0, rkM.GetRow(0));
  SetRow( 1, rkM.GetRow(1));
  SetRow( 2, rkM.GetRow(2));
  SetRow( 3, rkM.GetRow(3));
	return *this;
}


moGLMatrixf&
moGLMatrixf::MakeIdentity() {
    (*this) = moMatrix4f::IDENTITY;
    return (*this);
}

moGLMatrixf&
moGLMatrixf::MakeZero() {
    (*this) = moMatrix4f::ZERO;
    return (*this);
}


moGLMatrixf&
moGLMatrixf::MakePerspective( float fovy,  float aspect,  float znear,  float zfar ) {

    float ymax, xmax;
    ymax = znear * tanf ( 0.5f * fovy * moMathf::DEG_TO_RAD );
    xmax = ymax * aspect;
    MakeFrustrum( -xmax, xmax, -ymax, ymax, znear, zfar );
    return (*this);
}

moGLMatrixf&
moGLMatrixf::MakeLookAt( float eyeX,  float eyeY,  float eyeZ,  float centerX,  float centerY,  float centerZ,  float upX,  float upY,  float upZ ) {
//moGLMatrixf::MakeLookAt( float eyeX,  float eyeY,  float eyeZ,  float centerX,  float centerY,  float centerZ,  float upX,  float upY,  float upZ ) {
  moVector3f center3D( centerX, centerY, centerZ );
  moVector3f eyePosition3D( eyeX, eyeY, eyeZ );
  moVector3f upVector3D( upX, upY, upZ );
  moGLMatrixf& me(*this);
  moVector3f direction3D;
  moGLMatrixf matrix2, resultMatrix;
  //------------------
  direction3D.X() = center3D.X() - eyePosition3D.X();
  direction3D.Y()= center3D.Y() - eyePosition3D.Y();
  direction3D.Z() = center3D.Z() - eyePosition3D.Z();
  direction3D.Normalize();

  /*
  //------------------
  //Side = forward x up
  ComputeNormalOfPlane(side, forward, upVector3D);
  NormalizeVector(side);
  */
  moVector3f side3D, up;
  side3D = direction3D.UnitCross( upVector3D );


/*
  //------------------
  //Recompute up as: up = side x forward
  ComputeNormalOfPlane(up, side, forward);
*/
  up = side3D.Cross( direction3D );

  /*
  //------------------
  matrix2[0] = side[0];
  matrix2[4] = side[1];
  matrix2[8] = side[2];
  matrix2[12] = 0.0;
  //------------------
  matrix2[1] = up[0];
  matrix2[5] = up[1];
  matrix2[9] = up[2];
  matrix2[13] = 0.0;
  //------------------
  matrix2[2] = -forward[0];
  matrix2[6] = -forward[1];
  matrix2[10] = -forward[2];
  matrix2[14] = 0.0;
  //------------------
  matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
  matrix2[15] = 1.0;
  */

  matrix2[0][0] = side3D.X();
  matrix2[1][0] = side3D.Y();
  matrix2[2][0] = side3D.Z();
  matrix2[3][0] = 0.0;

  matrix2[0][1] = up.X();
  matrix2[1][1] = up.Y();
  matrix2[2][1] = up.Z();
  matrix2[3][1] = 0.0;

  matrix2[0][2] = -direction3D.X();
  matrix2[1][2] = -direction3D.Y();
  matrix2[2][2] = -direction3D.Z();
  matrix2[3][2] = 0.0;

  matrix2[0][3] = matrix2[1][3] = matrix2[2][3] = 0.0f;
  matrix2[3][3] = 1.0;
  //------------------
/*
  MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
  glhTranslatef2(resultMatrix,
                -eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
*/

  me = me * matrix2;
  me.Translate( -eyePosition3D.X(), -eyePosition3D.Y(), -eyePosition3D.Z() );


  return (*this);
}

moGLMatrixf&
moGLMatrixf::MakeFrustrum( float left, float right, float bottom, float top, float znear, float zfar  ) {
  float r_l = right - left;
  float t_b = top - bottom;
  float f_n = zfar - znear;
  float A = (right + left)/r_l;
  float B = (top + bottom)/t_b;
  float C = -(zfar + znear)/f_n;
  float D = -2*(zfar*znear)/f_n;

  MakeIdentity();
  moGLMatrixf& Me( *this );
  moGLMatrixf Result = moMatrix4f::IDENTITY;
  Result.SetRow( 0, moVector4f(   2.0 * znear / r_l,  0.0,                A,        0.0 ) );
  Result.SetRow( 1, moVector4f(   0.0,                2.0 * znear / t_b,  B,        0.0 ) );
  Result.SetRow( 2, moVector4f(   0.0,                0.0,                C,        D ) );
  Result.SetRow( 3, moVector4f(   0.0,                0.0,              -1.0,       0.0 ) );

  Me = Me * (Result.Transpose());
  return (*this);
}


/**

*/
moGLMatrixf&
moGLMatrixf::MakeOrthographic( float left, float right, float bottom, float top, float znear, float zfar  ) {
  float r_l = right - left;
  float t_b = top - bottom;
  float f_n = zfar - znear;
  float tx = -(right + left)/r_l;
  float ty = -(top + bottom)/t_b;
  float tz = -(zfar + znear)/f_n;
  MakeIdentity();
  moGLMatrixf& Me( *this );
  moGLMatrixf Result = moMatrix4f::IDENTITY;
  Result.SetRow( 0, moVector4f(   2.0 / r_l,  0.0,        0.0,        tx ) );
  Result.SetRow( 1, moVector4f(   0.0,        2.0 / t_b,  0.0,        ty ) );
  Result.SetRow( 2, moVector4f(   0.0,        0.0,        -2.0 / f_n, tz ) );

  Me = Me * (Result.Transpose());
  return (*this);
}

moGLMatrixf&
moGLMatrixf::Translate( float x, float y, float z ) {

  //moVector3f tr = moVector3f( x, y, z);
  moGLMatrixf& Me( *this );
  moGLMatrixf Result;
  Result.MakeIdentity();
  //const moGLMatrixf& m

  Result[0][3] = x;
  Result[1][3] = y;
  Result[2][3] = z;

  Me = Me*Result.Transpose();

	return (*this);
}

moGLMatrixf&
moGLMatrixf::Rotate( float angle, float rx, float ry, float rz ) {

  moGLMatrixf& Me( *this );
  moGLMatrixf Result;
  Result.MakeIdentity();

  moVector3f rotAxe3D( rx, ry, rz );
  rotAxe3D.Normalize();
  rx = rotAxe3D.X();
  ry = rotAxe3D.Y();
  rz = rotAxe3D.Z();
  float c = moMathf::Cos( angle );
  float s = moMathf::Sin( angle );

  if (rz!=0.0 && rx==0.0 && ry==0.0) {

    Result[0][0] = moMathf::Cos( angle );
    Result[0][1] = -moMathf::Sin( angle );
    Result[1][0] = moMathf::Sin( angle );
    Result[1][1] = moMathf::Cos( angle );
    Me = Me*Result.Transpose();

  } else {
    Result[0][0] = rx*rx*(1-c) + c;
    Result[0][1] = rx*ry*(1-c) - rz*s;
    Result[0][2] = rx*rz*(1-c) + ry*s;
    Result[0][3] = 0.0;

    Result[1][0] = rx*ry*(1-c) + rz*s;
    Result[1][1] = ry*ry*(1-c) + c;
    Result[1][2] = ry*rz*(1-c) - rx*s;
    Result[1][3] = 0.0;

    Result[2][0] = rx*rz*(1-c) - ry*s;
    Result[2][1] = ry*rz*(1-c) + rx*s;
    Result[2][2] = rz*rz*(1-c) + c;
    Result[2][3] = 0.0;

    Result[3][0] = 0.0;
    Result[3][1] = 0.0;
    Result[3][2] = 0.0;
    Result[3][3] = 1.0;
    Me = Me*Result.Transpose();
  }

  return (*this);
}

moGLMatrixf&
moGLMatrixf::Scale( float sx, float sy, float sz ) {

  moGLMatrixf& Me( *this );
  moGLMatrixf Result = moMatrix4f::IDENTITY;
  Result[0][0] = sx;
  Result[1][1] = sy;
  Result[2][2] = sz;
  Me = Me*Result.Transpose();

  return (*this);
}

moText
moGLMatrixf::ToJSON() const {
  moText JSON = "[";
  moText comma="",nline="";
  for(int j=0;j<4; j++) {
    JSON+= nline;
    for(int i=0;i<4; i++) {
      JSON+= comma + FloatToStr( (*this)[j][i] );
      comma=",";
    }
    nline="\n";
  }
  JSON+= "]";
  return JSON;
}




moGLManager::moGLManager()
{

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_GL );

	SetName("glmanager");
	SetLabelName("glmanager");

	m_Context = NULL;
	m_DisplayServer = NULL;
	m_DisplayScreen = NULL;
	m_DisplayWindow = NULL;

	m_gpu_vendor_code = 0;
	m_gpu_vendor_string = "undefined";
	m_gpu_renderer_string = "undefined";

	m_current_fbo = m_previous_fbo = 0;

	m_current_read_buffer = m_current_draw_buffer = 0;
	m_previous_read_buffer = m_previous_draw_buffer = 0;
	m_bFrameBufferObjectActive = false;
}

moGLManager::~moGLManager()
{
	Finish();
}

MOboolean moGLManager::Init()
{
	QueryGPUVendorString();
	m_current_fbo = 0;
	m_bFrameBufferObjectActive = false;

	m_Context = NULL;
	m_DisplayServer = NULL;
	m_DisplayScreen = NULL;
	m_DisplayWindow = NULL;
	m_gl_version = "none";
	m_gl_major_version = 0;
	m_gl_minor_version = 0;

#ifndef OPENGLESV2
	glGetIntegerv(GL_DRAW_BUFFER, &m_current_draw_buffer);
	glGetIntegerv(GL_READ_BUFFER, &m_current_read_buffer);


  m_gl_version = (char*) glGetString(GL_VERSION);
	glGetIntegerv(GL_MAJOR_VERSION, &m_gl_major_version);
	glGetIntegerv(GL_MINOR_VERSION, &m_gl_minor_version);

  if (m_gl_major_version==0) {
    std::string major = (char*)m_gl_version;
    major.substr( 0, major.find(".") );
    m_gl_major_version = (int) atoi(major.c_str() );
  }

  if (m_gl_minor_version==0) {
    std::string minor = (char*)m_gl_version;
    minor.substr( minor.find(".")+1, 1  );
    m_gl_minor_version = (int) atoi(minor.c_str() );
  }

  MODebug2->Message("moGLManager::Init > GL VERSION: "+m_gl_version );
  MODebug2->Message("moGLManager::Init > GL_MAJOR_VERSION: "+IntToStr(m_gl_major_version) );
  MODebug2->Message("moGLManager::Init > GL_MINOR_VERSION: "+IntToStr(m_gl_minor_version) );
  MODebug2->Message("moGLManager::Init > GPU VENDOR STRING is "+m_gpu_vendor_string );
  MODebug2->Message("moGLManager::Init > GPU RENDERER STRING is "+m_gpu_renderer_string );

#endif
	return true;
}

MOboolean moGLManager::Finish()
{
	return true;
}

void moGLManager::SetFrameBufferObjectActive( bool active ) {

    m_bFrameBufferObjectActive = active;

}

MOboolean moGLManager::CheckErrors(moText p_location)
{
	GLuint errnum;
	moText errstr;
	MOboolean error = false;

  return false;

	while ((errnum = glGetError()))
	{
	  //GL_NO_ERROR
	  //GL_INVALID_ENUM

		error = true;
		errstr = moText(" GL error code:") + IntToStr(errnum) + moText(" message > ");
#ifndef OPENGLESV2
		errstr+= (char *)gluErrorString(errnum);
#endif
		if (p_location != moText("")) errstr += moText(" at ") + moText(p_location);
		moDebugManager::Error("moGLManager::CheckErrors > errors: " + errstr);
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
        m_gpu_vendor_string = vendor;

        if (strstr(vendor, "NVIDIA") != NULL) m_gpu_vendor_code = MO_GPU_NV;
        else if (strstr(vendor, "ATI") != NULL) m_gpu_vendor_code = MO_GPU_ATI;
        else if (strstr(vendor, "INTEL") != NULL) m_gpu_vendor_code = MO_GPU_INTEL;
        else m_gpu_vendor_code = MO_GPU_OTHER;
    } else m_gpu_vendor_code = MO_GPU_OTHER;

    glvendor = (char*)glGetString(GL_RENDERER);
    if (glvendor!=NULL) {
        strcpy(vendor, glvendor);
        m_gpu_renderer_string = vendor;
    }

}

void moGLManager::SetPerspectiveView( MOint p_width, MOint p_height, double fovy,  double aspect,  double znear,  double zfar )
{
	glViewport(0, 0, p_width, p_height);
	m_Viewport = moGLViewport( p_width, p_height );
  if (aspect==0.0) {
    aspect = m_Viewport.GetProportion();
  }
  m_ProjectionMatrix.MakePerspective( fovy, aspect, znear, zfar );
	//m_ProjectionMatrix.MakePerspectiveProjection( moVector3f( 0.0, 1.0, 0.0 ), moVector3f(0.0, 0.0, 0.0), moVector3f(0.0, 0.0, -10.0) );

	//m_ProjectionMatrix.MakePerspective(  );
//  float* pfv = m_ProjectionMatrix[0];

#ifndef OPENGLESV2

/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, m_Viewport.GetProportion(), 0.01f, 1000.0f);
*/

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glLoadMatrixf( m_ProjectionMatrix.GetPointer() );

/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
*/
#endif
}

void moGLManager::SetDefaultPerspectiveView( MOint p_width, MOint p_height ) {
  SetPerspectiveView( p_width, p_height, 60.0f,  moDisplay(p_width,p_height).Proportion(),  0.01f,  1000.0f );
}

void moGLManager::SetDefaultOrthographicView( MOint p_width, MOint p_height ) {
  SetOrthographicView(  p_width,
                        p_height,
                        -0.5,
                        0.5,
                        moDisplay( p_width, p_height ).HeightToProportion(-0.5),
                        moDisplay( p_width, p_height ).HeightToProportion(0.5)
                      );
}

void moGLManager::SetOrthographicView(MOint p_width, MOint p_height, float left, float right, float bottom, float top, float znear, float zfar)
{
#ifndef OPENGLESV2
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#endif

    m_ProjectionMatrix.MakeOrthographic( left, right, bottom, top, znear, zfar );

    if (p_width!=0 || p_height!=0) {
      glViewport(0, 0, p_width, p_height);
      m_Viewport = moGLViewport( p_width, p_height );

#ifndef OPENGLESV2
      //gluOrtho2D(0.0, p_width, 0.0, p_height);
      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf( m_ProjectionMatrix.GetPointer() );
#endif
    } else {
      float prop;
      if (m_pResourceManager) {
        p_width = m_pResourceManager->GetRenderMan()->RenderWidth();
        p_height = m_pResourceManager->GetRenderMan()->RenderHeight();
      }
      if ( p_width == 0 || p_height == 0 ) { p_width  = 1; p_height = 1; prop = 1.0; }
      else {
        prop = (float) p_height / (float) p_width;
      }
      glViewport( 0, 0, p_width, p_height );
      m_Viewport = moGLViewport( p_width, p_height );
#ifndef OPENGLESV2
	//glOrtho( -0.5, 0.5, -0.5*prop, 0.5*prop, -1, 1);
      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf( m_ProjectionMatrix.GetPointer() );
#endif
      // Set Up An Ortho Screen
    }
#ifndef OPENGLESV2
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif

}


void moGLManager::LookAt( float eyeX,  float eyeY,  float eyeZ,  float centerX,  float centerY,  float centerZ,  float upX,  float upY,  float upZ ) {

  moGLMatrixf Lat;
  Lat.MakeIdentity();
  Lat.MakeLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ );

  //m_ModelMatrix.MakeIdentity();
  //m_ModelMatrix.MakeLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ );
  //m_ProjectionMatrix.MakeIdentity();
  m_ProjectionMatrix = Lat*m_ProjectionMatrix;
/*
  MODebug2->Message("moGLManager::LookAt > LookAt Matrix:" );
  MODebug2->Message( Lat.ToJSON() );
*/
#ifndef OPENGLESV2
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glLoadMatrixf( m_ProjectionMatrix.GetPointer() );
      //gluLookAt(		eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
      /*glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();*/
#endif
}


MOint moGLManager::GetRenderMode()
{
	MOint render_mode = 0;
#ifndef OPENGLESV2
	glGetIntegerv(GL_RENDER_MODE, &render_mode);
#endif
	return render_mode;
}

void moGLManager::SetRenderMode(MOint p_mode)
{
	// Valid modes are GL_RENDER, GL_SELECT and GL_FEEDBACK.
#ifndef OPENGLESV2
	glRenderMode(p_mode);
#endif
}

// Setting up rendering parameters commonly used in moldeo effects.
void moGLManager::SetMoldeoGLState()
{
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing.
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do.
	glEnable(GL_BLEND);									// Enables blending.
	glEnable(GL_TEXTURE_2D);
#ifndef OPENGLESV2
							// Enables texturing.
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);           // Polygon full filling mode, front and back
#endif
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
#ifndef OPENGLESV2
	glPixelStorei(GL_PACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_PACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
#endif

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void moGLManager::SaveGLState()
{
	// For more info about the GL attributes, see apendix B of the Red Book:
	// http://cs-sdl.sourceforge.net/index.php/Red_Book_Appendix_B

	SaveFramebuffer();
#ifndef OPENGLESV2
    glPushAttrib(GL_ALL_ATTRIB_BITS);
#endif
	SaveGLMatrices();
}

void moGLManager::SaveGLMatrices()
{
#ifndef OPENGLESV2
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glMatrixMode(GL_TEXTURE);
    glPushMatrix();
#endif
}

void moGLManager::SaveView()
{
#ifndef OPENGLESV2
	glPushAttrib(GL_VIEWPORT_BIT);
#endif
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
#ifndef OPENGLESV2
    glPopAttrib();
#endif
}

void moGLManager::RestoreGLMatrices()
{
#ifndef OPENGLESV2
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glMatrixMode(GL_TEXTURE);
    glPopMatrix();
#endif
}

void moGLManager::RestoreView()
{
	RestoreGLMatrices();
#ifndef OPENGLESV2
	glPopAttrib();
#endif
}

void moGLManager::RestoreFramebuffer()
{
    m_current_fbo = m_saved_fbo;
    if (m_bFrameBufferObjectActive) glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_current_fbo);

	m_current_read_buffer = m_saved_read_buffer;
#ifndef OPENGLESV2
	if (m_bFrameBufferObjectActive) glReadBuffer(m_current_read_buffer);
#endif
	m_current_draw_buffer = m_saved_draw_buffer;
#ifndef OPENGLESV2
	if (m_bFrameBufferObjectActive) glDrawBuffer(m_current_draw_buffer);
#endif
}

moTexParam moGLManager::BuildFPTexParam(MOboolean p_16bits, MOushort p_num_components)
{
    moTexParam result;
#ifndef OPENGLESV2
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
#endif
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

MOboolean moGLManager::RectTexture(GLenum p_target) const
{
	return
#ifndef OPENGLESV2
		(p_target == GL_TEXTURE_RECTANGLE_NV) ||
#endif
	       (p_target == GL_TEXTURE_RECTANGLE_ARB);
}

MOboolean moGLManager::FPTexture(GLint p_internal_format)
{
	return
#ifndef OPENGLESV2
			(p_internal_format == GL_FLOAT_RGBA16_NV) ||
			(p_internal_format == GL_FLOAT_RGBA32_NV) ||
			(p_internal_format == GL_FLOAT_R16_NV) ||
			(p_internal_format == GL_FLOAT_R32_NV) ||
			(p_internal_format == GL_RGBA_FLOAT16_ATI) ||
			(p_internal_format == GL_RGBA_FLOAT32_ATI) ||
			(p_internal_format == GL_LUMINANCE_FLOAT16_ATI) ||
			(p_internal_format == GL_LUMINANCE_FLOAT32_ATI) ||
#endif
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
	if (m_bFrameBufferObjectActive) glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_current_fbo);
}

void moGLManager::SetCurrentReadBuffer(MOint p_buffer)
{
	m_current_read_buffer = p_buffer;
#ifndef OPENGLESV2
	if (m_bFrameBufferObjectActive) glReadBuffer(m_current_read_buffer);
#endif
}

void moGLManager::SetCurrentDrawBuffer(MOint p_buffer)
{
	m_current_draw_buffer = p_buffer;
#ifndef OPENGLESV2
	if (m_bFrameBufferObjectActive) glDrawBuffer(m_current_draw_buffer);
#endif
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


int
moGLManager::CreateContext( int p_width, int p_height ) {

    MODebug2->Message( "moGLManager::CreateContext > p_width: " + IntToStr(p_width) + " p_height: " + IntToStr(p_height) );

    #ifdef MO_WIN

    GLuint PixelFormat;
    PIXELFORMATDESCRIPTOR pfd;
    hDC = GetDC(NULL);


    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 16;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    PixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, PixelFormat, &pfd);

    //  HGLRC
    this->m_Context = (moGLContext) wglCreateContext(hDC);
    wglMakeCurrent(hDC, (HGLRC) this->m_Context );

    #endif

    #ifdef MO_LINUXX

    /*
    If direct is True,
            then a direct-rendering context is created if the
            implementation supports direct rendering, if the connection is to an X
            server that is local, and if a direct-rendering context is available. (An implementation may return an indirect context when share_list is True.)
            If share_list is False,
            then a rendering context that renders through the X server is always created.
            Direct rendering provides a performance advantage in some implementations.
            However, direct-rendering contexts cannot be shared outside a single process,
            and they may be unable to render to GLX pixmaps.
    */

    int m_glxVersion = 0;
    int glxMajorVer, glxMinorVer;
    Display *XServerDisplay=NULL;
    GdkDisplay *XGdkDisplay=NULL;
    GLXFBConfig *fbc = NULL;
    GLXPbuffer OffScreenBuffer = 0;
    int nscreens = 0;

    //XServerDisplay = GDK_DISPLAY();
    //XServerDisplay = gdk_display_get_default();
    MODebug2->Message( "moGLManager::CreateContext > getting X server display" );
    //XServerDisplay = gdk_x11_get_default_xdisplay();
    if (m_DisplayServer==NULL) {
      XServerDisplay = XOpenDisplay(NULL);
      m_DisplayServer = (void*)XServerDisplay;
    } else {
      XServerDisplay = (Display *)m_DisplayServer;
      MODebug2->Message("moGLManager::CreateContext > display server connection already opened.");
    }
    //XServerDisplay = GDK_DISPLAY();
    //  gdk_display_get_screen ()
    //  gdk_display_get_n_screens ()

    if (XServerDisplay) {

        MODebug2->Message( "moGLManager::CreateContext > X server display OK." );

        XGdkDisplay = gdk_x11_lookup_xdisplay( XServerDisplay );
        if (!XGdkDisplay) XGdkDisplay = gdk_display_get_default ();

        if (XGdkDisplay) {
          MODebug2->Error("moGLManager::CreateContext > X Gdk Display OK! : " + IntToStr((MOulong)XGdkDisplay));
          nscreens = gdk_display_get_n_screens(XGdkDisplay);
        } else MODebug2->Error("moGLManager::CreateContext > X Gdk Display not found : " + IntToStr((MOulong)XGdkDisplay));

        MODebug2->Message("moGLManager::CreateContext > X Server Display found: " + IntToStr((MOulong)XServerDisplay) + " screens:" + IntToStr(nscreens) );

    } else {
        MODebug2->Error("moGLManager::CreateContext > X Server Display not found : " + IntToStr((MOulong)XServerDisplay));
        return false;
    }

    bool ok = glXQueryVersion( XServerDisplay, &glxMajorVer, &glxMinorVer);

    if (!ok)
        m_glxVersion = 10; // 1.0 by default
    else
        m_glxVersion = glxMajorVer*10 + glxMinorVer;

    MODebug2->Message("moGLManager::CreateContext > GLX Version (10=1.0,13=1.3,...): " + IntToStr(m_glxVersion) + "=" + FloatToStr( (double)(m_glxVersion)/(double) 10.0)  );

    if ( m_glxVersion >= 13)
    {
        // GLX >= 1.3
        //GLXFBConfig *fbc = gc->m_fbc;

        int nelements = 0;
        int attrib_list[] =
                        {
                        GLX_RENDER_TYPE, GLX_RGBA_BIT,
                        GLX_DOUBLEBUFFER, 1,
                        GLX_RED_SIZE, 8,
                        GLX_GREEN_SIZE, 8,
                        GLX_BLUE_SIZE, 8,
                        GLX_ALPHA_SIZE, 8,
                        GLX_DEPTH_SIZE, 16,
                        GLX_STENCIL_SIZE, 8,
                        0
                        };


        fbc = glXChooseFBConfig( XServerDisplay,
                            DefaultScreen(XServerDisplay)/*screen number*/,
                            attrib_list,
                            &nelements );


        if (fbc) {
            MODebug2->Message( "moGLManager::CreateContext > FBConfig OK!");
        } else {
            MODebug2->Error( "moGLManager::CreateContext > No FBConfig");
            return false;
        }

        this->m_Context = (moGLContext) glXCreateNewContext( XServerDisplay /*XServer Display*/,
                                        fbc[0] /*attribute list FB Config match*/,
                                        GLX_RGBA_TYPE /*type of rendering*/,
                                        NULL /*shared context  for display lists*/,
                                        GL_TRUE /*direct rendering to graphics system*/);
    }
    else
    {
        // GLX <= 1.2
        int attrib_list_vi[] =
                        {
                        GLX_RGBA,
                        GLX_DOUBLEBUFFER,
                        GLX_RED_SIZE, 8,
                        GLX_GREEN_SIZE, 8,
                        GLX_BLUE_SIZE, 8,
                        GLX_ALPHA_SIZE, 8,
                        GLX_DEPTH_SIZE, 16,
                        GLX_STENCIL_SIZE, 8,
                        0
                        };

        XVisualInfo *vi = glXChooseVisual( XServerDisplay,
                                           DefaultScreen(XServerDisplay), /*screen number*/
                                           attrib_list_vi );

        this->m_Context = (moGLContext) glXCreateContext( XServerDisplay,
                                        vi,
                                        NULL,
                                        GL_TRUE );
    }

    if (this->m_Context) {

        MODebug2->Message("moGLManager::CreateContext > creating offscreen buffer.");

        if ( m_glxVersion >= 13 ) {
            // GLX >= 1.3
            int pbuffer_attrib[] = {
            GLX_PBUFFER_WIDTH, p_width,
            GLX_PBUFFER_HEIGHT, p_height,
            GLX_LARGEST_PBUFFER, 0,
            GLX_PRESERVED_CONTENTS, 0,
            0 };
            OffScreenBuffer = glXCreatePbuffer( XServerDisplay, fbc[0], pbuffer_attrib );

            this->m_DisplayScreen = (void*) OffScreenBuffer;

            if (OffScreenBuffer) {
              MODebug2->Message("moGLManager::CreateContext > Offscreen buffer OK! GLPXBuffer: " + IntToStr( (MOulong) OffScreenBuffer )  );
            } else {
              MODebug2->Error("moGLManager::CreateContext > Offscreen buffer BAD... : " + IntToStr( (MOulong) OffScreenBuffer )  );
              return false;
            }
            if ( glXMakeContextCurrent( XServerDisplay, OffScreenBuffer, OffScreenBuffer, (GLXContext) this->m_Context ) ) {
              MODebug2->Message("moGLManager::CreateContext > Making Context current OK!");
            }
        } else {
            // GLX <= 1.2
            //glXMakeCurrent( XServerDisplay, GDK_WINDOW_XWINDOW(window), m_glContext );
        }

    }

    #endif

    #ifdef MO_MAC
        //CGL
        CGLCreateContext


        //COCOA
        //NSOpenGLView

        //CARBON
        //aglCreateContext

    #endif



    return (this->m_Context == NULL);
}
/*
int
moGLManager::CreateOffscreen( int p_width, int p_height, int  ) {

}
*/

moGLContext
moGLManager::GetContext() {
    return this->m_Context;
}


moDisplayServer
moGLManager::GetDisplayServer() {
    return this->m_DisplayServer;
}

moDisplayScreen
moGLManager::GetDisplayScreen() {
    return this->m_DisplayScreen;
}

moDisplayWindow
moGLManager::GetDisplayWindow() {
    return this->m_DisplayWindow;
}


void
moGLManager::SetContext(moGLContext p_Context) {
    m_Context = p_Context;
}

moGLMatrixf&
moGLManager::GetModelMatrix() {
  return m_ModelMatrix;
}

moGLMatrixf&
moGLManager::SetModelMatrix( const moGLMatrixf& p_mat4 ) {

  m_ModelMatrix = p_mat4;
  return m_ModelMatrix;
}

moGLMatrixf&
moGLManager::GetProjectionMatrix() {
  return m_ProjectionMatrix;
}

moGLMatrixf&
moGLManager::SetProjectionMatrix( const moGLMatrixf& p_mat4 ) {

  m_ProjectionMatrix = p_mat4;
  return m_ProjectionMatrix;
}


