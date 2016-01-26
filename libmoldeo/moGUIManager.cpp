/*******************************************************************************

                              moGUIManager.cpp

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

#include "moGUIManager.h"
#include <moFontManager.h>

#include "moArray.h"
moDefineDynamicArray(moGuiObjectArray)
moDefineDynamicArray(moAttributeArray)

moCamera3D::moCamera3D( const moCamera3D& p_src ) : moCamera3DBase() {
  (*this) = p_src;
}

moCamera3D&
moCamera3D::operator=( const moCamera3D& p_src ) {
  m_Position = p_src.m_Position;
  m_Center = p_src.m_Center;
  return (*this);
}

moCamera3D&
moCamera3D::operator=( const moCamera3DBase& p_src ) {
  moCamera3DBase& m( (*this) );
  m = p_src;
  return (*this);
}





moGeometry::moGeometry() {
  m_Type = MO_GEOMETRY_UNDEFINED;
}

moGeometry::moGeometry( moGeometryType p_type ) {
  m_Type = p_type;
}

moGeometry::~moGeometry() {

}

moGeometry&
moGeometry::operator=(const moGeometry& p_src ) {

    m_Vertices = p_src.m_Vertices;
    m_VerticesUvs = p_src.m_VerticesUvs;
    m_Colors = p_src.m_Colors;
    m_ColorBuffer = p_src.m_ColorBuffer;
    m_VerticesBuffer = p_src.m_VerticesBuffer;
    m_VerticesUVBuffer = p_src.m_VerticesUVBuffer;
    m_Faces = p_src.m_Faces;
    m_FaceVertexUvs = p_src.m_FaceVertexUvs;
    m_Name = p_src.m_Name;
    m_Type = p_src.m_Type;
    return (*this);
}

MOboolean moGeometry::Init() {
  return moAbstract::Init();
}

MOboolean moGeometry::Init( moResourceManager* pResourceManager ) {
  SetResourceManager(pResourceManager);
  return pResourceManager!=NULL;
}

MOboolean moGeometry::Finish() {
  return true;
}

void
moGeometry::applyMatrix( const moGLMatrixf &p_ModelMatrix ) {
  moGLMatrixf tf = p_ModelMatrix;
}

moText
moGeometry::TypeToStr( moGeometryType p_type ) {

    moText result;

    switch(p_type) {
        case MO_GEOMETRY_POINT:
            result = moText("PointGeometry");
            break;
        case MO_GEOMETRY_CIRCLE:
            result = moText("CircleGeometry");
            break;
        case MO_GEOMETRY_CYLINDER:
            result = moText("CylinderGeometry");
            break;
        case MO_GEOMETRY_DODECAHEDRON:
            result = moText("DodecahedronGeometry");
            break;
        case MO_GEOMETRY_EXTRUDE:
            result = moText("ExtrudeGeometry");
            break;
        case MO_GEOMETRY_ICOSAHEDRON:
            result = moText("IcosahedronGeometry");
            break;
        case MO_GEOMETRY_PLANE:
            result = moText("PlaneGeometry");
            break;
        case MO_GEOMETRY_POLYHEDRON:
            result = moText("PolyhedronGeometry");
            break;
        case MO_GEOMETRY_RING:
            result = moText("RingGeometry");
            break;
        case MO_GEOMETRY_SHAPE:
            result = moText("ShapeGeometry");
            break;
        case MO_GEOMETRY_SPHERE:
            result = moText("SphereGeometry");
            break;
        case MO_GEOMETRY_TETRAHEDRON:
            result = moText("TetrahedronGeometry");
            break;
        case MO_GEOMETRY_TEXT:
            result = moText("TextGeometry");
            break;
        case MO_GEOMETRY_TUBE:
            result = moText("TubeGeometry");
            break;
        case MO_GEOMETRY_BOX:
            result = moText("BoxGeometry");
            break;

        case MO_GEOMETRY_UNDEFINED:
            result = moText("undefined Geometry");
            break;

        default:
            result = moText("unset");
            break;
    }
    return result;
}

moText
moGeometry::ToJSON() {
    moText JSON;
    moText comma("");
    JSON = "{";
    JSON+= "'name': "+m_Name;
    JSON+= ",'type': "+TypeToStr(m_Type);

    //concatenate all Vertices
    JSON+= "',vertices': [";
    for( MOuint i=0; i<m_Vertices.Count(); i++) {

        moVector3f pt = m_Vertices[i];

        JSON+= comma;
        JSON+= "    {";
        JSON+= pt.X() + moText(",");
        JSON+= pt.Y() + moText(",");
        JSON+= pt.Z();
        JSON+= "}";
        comma = ",";
    }
    JSON+= "],";
    JSON+= "'verticesBuffer': [";
    comma = "";
    if (m_VerticesBuffer)
    for( MOuint i=0; i<m_Vertices.Count(); i++) {

        JSON+= comma;
        JSON+= "    {";
        JSON+= m_VerticesBuffer[i*3] + moText(",");
        JSON+= m_VerticesBuffer[i*3+1] + moText(",");
        JSON+= m_VerticesBuffer[i*3+2];
        JSON+= "}";
        comma = ",";
    }
    JSON+= "],";
    //JSON+= "    { 0.0, 0.0, 0.0 }";
    JSON+= "}";

    return JSON;
}



moBoxGeometry::moBoxGeometry( float width, float height,float depth, int wsegments, int hsegments, int dsegments ) : moGeometry( MO_GEOMETRY_BOX ) {

}

moBoxGeometry::~moBoxGeometry(  ) {

}



moPlaneGeometry::moPlaneGeometry( float width, float height, int widthSegments, int heightSegments ) : moGeometry(MO_GEOMETRY_PLANE) {

    m_VerticesBuffer = NULL;
    m_VerticesUVBuffer = NULL;
    m_ColorBuffer = NULL;

    m_Name = "MyPlane";
    m_Type = MO_GEOMETRY_PLANE;

    m_Vertices.Init( 0, moVector3f( 0.0, 0.0, 0.0) );
    m_Vertices.Empty();

    m_VerticesUvs.Init( 0,  moTCoord( 0.0, 0.0 ) );
    m_VerticesUvs.Empty();

    m_Faces.Init( 0, moFace( 0, 0, 0 ) );
    m_Faces.Empty();

    m_FaceVertexUvs.Init( 0, moTCoord( 0.0, 0.0 ) );
    m_FaceVertexUvs.Empty();

    m_Colors.Init( 0, moColor( 0.0, 0.0, 0.0 ) );
    m_Colors.Empty();


    float dw = 0.5*width;
    float dh = 0.5*height;

    /**
    VERTICES { -1,-1,0.0,  -1,1,0.0,  1,-1,0.0, 1,1,0.0 }

    1---3    y|\
    |\  |     |
    | \ |     |
    |  \|     |_____\
    0---2    z       x

    TCOORDS { 0.0,1.0,  0.0,0.0,  1.0,1.0, 1.0, 0.0 }

    T1---T3    T1=A(0,0) ----> T3=B(1,0)
     |\  |               |\ 1|
     | \ |               | \ |
     |  \|               |0 \|
    T0---T2    T0=D(0,1) <---- T2=C(1,1)

    */
    m_Vertices.Add( moVector3f( -dw, -dh, 0.0) );
    m_Vertices.Add( moVector3f( -dw,  dh, 0.0) );
    m_Vertices.Add( moVector3f(  dw, -dh, 0.0) );
    m_Vertices.Add( moVector3f(  dw,  dh, 0.0) );

    m_VerticesUvs.Add( moVector2f( 0.0, 1.0) );
    m_VerticesUvs.Add( moVector2f( 0.0, 0.0) );
    m_VerticesUvs.Add( moVector2f( 1.0, 1.0) );
    m_VerticesUvs.Add( moVector2f( 1.0, 0.0) );

    m_Colors.Add( moColor( 1.0, 0.0, 0.0 ) );
    m_Colors.Add( moColor( 0.0, 1.0, 0.0 ) );
    m_Colors.Add( moColor( 0.0, 0.0, 1.0 ) );
    m_Colors.Add( moColor( 1.0, 1.0, 1.0 ) );

    /** FACES*/

    m_Faces.Add( moFace( 0, 1, 2 ) );
    m_Faces.Add( moFace( 2, 3, 1 ) );

    m_FaceVertexUvs.Add( moVector2f( 0.0, 1.0) );
    m_FaceVertexUvs.Add( moVector2f( 0.0, 0.0) );
    m_FaceVertexUvs.Add( moVector2f( 1.0, 1.0) );

    m_FaceVertexUvs.Add( moVector2f( 1.0, 1.0) );
    m_FaceVertexUvs.Add( moVector2f( 1.0, 0.0) );
    m_FaceVertexUvs.Add( moVector2f( 0.0, 0.0) );

    //set buffers for shaders
    m_VerticesBuffer = new float [ m_Vertices.Count()*3 ];
    m_VerticesUVBuffer = new float [ m_VerticesUvs.Count()*2 ];
    m_ColorBuffer = new float [ m_Vertices.Count()*3 ];

    if (m_VerticesBuffer)
    for( MOuint i=0;i<m_Vertices.Count(); i++) {
        m_VerticesBuffer[i*3] = m_Vertices[i].X();
        m_VerticesBuffer[i*3+1] = m_Vertices[i].Y();
        m_VerticesBuffer[i*3+2] = m_Vertices[i].Z();
    }


    if (m_VerticesUVBuffer)
    for( MOuint i=0;i<m_VerticesUvs.Count(); i++) {
        m_VerticesUVBuffer[i*2] = m_VerticesUvs[i].X();
        m_VerticesUVBuffer[i*2+1] = m_VerticesUvs[i].Y();
    }

    if (m_ColorBuffer)
    for( MOuint i=0;i<m_Colors.Count(); i++) {
        m_ColorBuffer[i*2] = m_Colors[i].X();
        m_ColorBuffer[i*2+1] = m_Colors[i].Y();
    }

}

moPlaneGeometry::~moPlaneGeometry() {
    if (m_VerticesBuffer) { delete[] m_VerticesBuffer; m_VerticesBuffer = NULL; }
    if (m_VerticesUVBuffer) { delete[] m_VerticesUVBuffer; m_VerticesUVBuffer = NULL; }
    if (m_ColorBuffer) { delete[] m_ColorBuffer; m_ColorBuffer = NULL; }
}


moPolyhedronGeometry::moPolyhedronGeometry() {

}
moPolyhedronGeometry::moPolyhedronGeometry( const moVector3fArray& p_Vertices,
                                           const moVector3iArray& p_Faces,
                                           float radius,
                                           float detail ) : moGeometry( MO_GEOMETRY_POLYHEDRON ) {

  //radius, detail
  m_Vertices = p_Vertices;
  m_Faces = p_Faces;
  //calculate projection to sphere with radius
  //tesselate using "detail" factor: 0 (no tesselation), 1 (1 subdivision), 2 (2 subdivisions), N (N subdivisions)
}

moPolyhedronGeometry::~moPolyhedronGeometry() {

}



moSphereGeometry::moSphereGeometry( float radius, int widthSegments, int heightSegments, float phiStart, float phiLength, float thetaStart, float thetaLength )
: moGeometry( MO_GEOMETRY_SPHERE ) {

    m_VerticesBuffer = NULL;
    m_VerticesUVBuffer = NULL;
    m_ColorBuffer = NULL;

    m_Name = "MySphere";
    m_Type = MO_GEOMETRY_SPHERE;

    m_Vertices.Init( 0, moVector3f( 0.0, 0.0, 0.0) );
    m_Vertices.Empty();

    m_VerticesUvs.Init( 0,  moTCoord( 0.0, 0.0 ) );
    m_VerticesUvs.Empty();

    m_Faces.Init( 0, moFace( 0, 0, 0 ) );
    m_Faces.Empty();

    m_FaceVertexUvs.Init( 0, moTCoord( 0.0, 0.0 ) );
    m_FaceVertexUvs.Empty();

    m_Colors.Init( 0, moColor( 0.0, 0.0, 0.0 ) );
    m_Colors.Empty();


    radius = radius || 50;

    widthSegments = max( 3, widthSegments );
    heightSegments = max( 2, heightSegments );

    float thetaEnd = thetaStart + thetaLength;
    int vertexCount = ( ( widthSegments + 1 ) * ( heightSegments + 1 ) );

    for ( int y = 0; y <= heightSegments; y++ ) {

      //verticesRow = [];

      float v = ((float)y) / (float)heightSegments;

      for ( int x = 0; x <= widthSegments; x++ ) {

        float u = ((float)x) / ((float)widthSegments);

        float px = - radius * moMathf::Cos( phiStart + u * phiLength ) * moMathf::Sin( thetaStart + v * thetaLength );
        float py = radius * moMathf::Cos( thetaStart + v * thetaLength );
        float pz = radius * moMathf::Sin( phiStart + u * phiLength ) * moMathf::Sin( thetaStart + v * thetaLength );

        moVector3f normal( px, py, pz );
        /*
        positions.setXYZ( index, px, py, pz );
        normals.setXYZ( index, normal.x, normal.y, normal.z );
        uvs.setXY( index, u, 1 - v );

        verticesRow.push( index );
        index ++;
        */
      }
      //vertices.push( verticesRow );
    }
/*
	var indices = [];

	for ( var y = 0; y < heightSegments; y ++ ) {

		for ( var x = 0; x < widthSegments; x ++ ) {

			var v1 = vertices[ y ][ x + 1 ];
			var v2 = vertices[ y ][ x ];
			var v3 = vertices[ y + 1 ][ x ];
			var v4 = vertices[ y + 1 ][ x + 1 ];

			if ( y !== 0 || thetaStart > 0 ) indices.push( v1, v2, v4 );
			if ( y !== heightSegments - 1 || thetaEnd < Math.PI ) indices.push( v2, v3, v4 );

		}

	}

	this.setIndex( new ( positions.count > 65535 ? THREE.Uint32Attribute : THREE.Uint16Attribute )( indices, 1 ) );
	this.addAttribute( 'position', positions );
	this.addAttribute( 'normal', normals );
	this.addAttribute( 'uv', uvs );

	this.boundingSphere = new THREE.Sphere( new THREE.Vector3(), radius );

    */
/*
    m_Vertices.Add( moVector3f( -dw, -dh, 0.0) );
    m_Vertices.Add( moVector3f( -dw,  dh, 0.0) );
    m_Vertices.Add( moVector3f(  dw, -dh, 0.0) );
    m_Vertices.Add( moVector3f(  dw,  dh, 0.0) );

    m_VerticesUvs.Add( moVector2f( 0.0, 1.0) );
    m_VerticesUvs.Add( moVector2f( 0.0, 0.0) );
    m_VerticesUvs.Add( moVector2f( 1.0, 1.0) );
    m_VerticesUvs.Add( moVector2f( 1.0, 0.0) );

    m_Colors.Add( moColor( 1.0, 0.0, 0.0 ) );
    m_Colors.Add( moColor( 0.0, 1.0, 0.0 ) );
    m_Colors.Add( moColor( 0.0, 0.0, 1.0 ) );
    m_Colors.Add( moColor( 1.0, 1.0, 1.0 ) );
*/
    /** FACES*/
/*
    m_Faces.Add( moFace( 0, 1, 2 ) );
    m_Faces.Add( moFace( 2, 3, 1 ) );

    m_FaceVertexUvs.Add( moVector2f( 0.0, 1.0) );
    m_FaceVertexUvs.Add( moVector2f( 0.0, 0.0) );
    m_FaceVertexUvs.Add( moVector2f( 1.0, 1.0) );

    m_FaceVertexUvs.Add( moVector2f( 1.0, 1.0) );
    m_FaceVertexUvs.Add( moVector2f( 1.0, 0.0) );
    m_FaceVertexUvs.Add( moVector2f( 0.0, 0.0) );

    //set buffers for shaders
    m_VerticesBuffer = new float [ m_Vertices.Count()*3 ];
    m_VerticesUVBuffer = new float [ m_VerticesUvs.Count()*2 ];
    m_ColorBuffer = new float [ m_Vertices.Count()*3 ];

    if (m_VerticesBuffer)
    for( MOuint i=0;i<m_Vertices.Count(); i++) {
        m_VerticesBuffer[i*3] = m_Vertices[i].X();
        m_VerticesBuffer[i*3+1] = m_Vertices[i].Y();
        m_VerticesBuffer[i*3+2] = m_Vertices[i].Z();
    }


    if (m_VerticesUVBuffer)
    for( MOuint i=0;i<m_VerticesUvs.Count(); i++) {
        m_VerticesUVBuffer[i*2] = m_VerticesUvs[i].X();
        m_VerticesUVBuffer[i*2+1] = m_VerticesUvs[i].Y();
    }

    if (m_ColorBuffer)
    for( MOuint i=0;i<m_Colors.Count(); i++) {
        m_ColorBuffer[i*2] = m_Colors[i].X();
        m_ColorBuffer[i*2+1] = m_Colors[i].Y();
    }
*/
}

moSphereGeometry::~moSphereGeometry() {
    if (m_VerticesBuffer) { delete[] m_VerticesBuffer; m_VerticesBuffer = NULL; }
    if (m_VerticesUVBuffer) { delete[] m_VerticesUVBuffer; m_VerticesUVBuffer = NULL; }
    if (m_ColorBuffer) { delete[] m_ColorBuffer; m_ColorBuffer = NULL; }
}


//===========================================
//
//				moGUIManager
//
//===========================================


moGUIManager::moGUIManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_GUI );

	SetName("guimanager");
	SetLabelName("guimanager");
/*
	hOpWnd = NULL;
	hVisWnd = NULL;
	*/
	m_OpHandle = NULL;
	m_VisHandle = NULL;
}


moGUIManager::~moGUIManager() {

	Finish();

}

MOboolean
moGUIManager::Init(/*HWND p_hOpWnd, HWND p_hVisWnd*/) {
/*
	if (p_hOpWnd != NULL)
	{
		hOpWnd = p_hOpWnd;
	}
	else
	{
		SDL_SysWMinfo wmInfo;
		if (SDL_GetWMInfo(&wmInfo)) hOpWnd = wmInfo.window;
	}

	if (p_hVisWnd != NULL)
	{
		hVisWnd = p_hVisWnd;
	}
	else
	{
	}

	MODebug2->Push("Handle of Operation Window = " + IntToStr((int)hOpWnd));
	MODebug2->Push("Handle of Visualization Window = " + IntToStr((int)hVisWnd));
*/
	return true;
}

MOboolean
moGUIManager::Init( MO_HANDLE p_OpWindowHandle, MO_DISPLAY p_Display ) {
    SetDisplay( p_Display );
    SetOpWindowHandle( p_OpWindowHandle );
    return true;
}

MOboolean
moGUIManager::Finish() {
	return true;
}

moWindow*
moGUIManager::NewWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextHeap &textheap ) {

  MODebug2->Message("moGUIManager::NewWindow NOT IMPLEMENTED > x: " + FloatToStr(x)
                    + " y: " + FloatToStr(y)
                    + " width: " + FloatToStr(width)
                    + " height: " + FloatToStr(height) );
  textheap.Push("NewWindow");
	return NULL;
}

void
moGUIManager::DisplayInfoWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextArray &pTexts ) {

	moWindow*	pWindow = NULL;
	pWindow = new moWindow();
	if (pWindow) {
		if (pWindow->Init( m_pResourceManager, x, y , width, height)) {
			pWindow->SetInfo( pTexts );
			pWindow->Draw();
		}
	}

	delete pWindow;
}

void
moGUIManager::SetDisplay( MO_DISPLAY p_Display )  {

    m_Display = p_Display;

}

void
moGUIManager::SetOpWindowHandle( MO_HANDLE p_OpHandle )  {

    m_OpHandle = p_OpHandle;

}

void
moGUIManager::SetVisWindowHandle( MO_HANDLE p_VisHandle )  {

    m_VisHandle = p_VisHandle;

}

MO_DISPLAY
moGUIManager::GetDisplay()  {

    return m_Display;

}


MO_HANDLE
moGUIManager::GetOpWindowHandle()  {

    return m_OpHandle;

}

MO_HANDLE
moGUIManager::GetVisWindowHandle()  {

    return m_VisHandle;

}

//===========================================
//
//				moGUIObject
//
//===========================================

moGuiObject::moGuiObject() {
	m_pResourceManager = NULL;
}

moGuiObject::~moGuiObject() {
	Finish();
}

MOboolean
moGuiObject::Init( moResourceManager* pResourceManager ) {
	m_pResourceManager = pResourceManager;
	if (m_pResourceManager)
		m_bInitialized = true;
	return moAbstract::Init();
}

MOboolean
moGuiObject::Finish() {
	return false;
}


//===========================================
//
//				moWidget
//
//===========================================

moWidget::moWidget() {

}

moWidget::~moWidget() {
	Finish();
}


//===========================================
//
//				moWindow
//
//===========================================

moWindow::moWindow() {

}

moWindow::~moWindow() {
	Finish();
}

MOboolean
moWindow::Init( moResourceManager* pResourceManager, MOfloat x, MOfloat y, MOfloat width, MOfloat height ) {

	m_bInitialized = moWidget::Init(pResourceManager);

	m_X = x;
	m_Y = y;
	m_Width = width;
	m_Height = height;

	return moAbstract::Init();

}

void
moWindow::SetInfo( moTextArray &pTexts ) {

	MOuint i;

	m_Texts.Empty();

	for(i=0; i<pTexts.Count(); i++) {
		m_Texts.Add(pTexts[i]);
	}

}

void
moWindow::Draw() {

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	glBindTexture( GL_TEXTURE_2D, 0 );
	glColor4f( 0.3, 0.3, 0.3, 0.75 );

#ifndef OPENGLESV2
	glBegin(GL_QUADS);
		glTexCoord2f( 0, 0);
		glVertex2f( m_X, m_Y);

		glTexCoord2f( 0, 1);
		glVertex2f(  m_X+m_Width, m_Y);

		glTexCoord2f( 1, 1);
		glVertex2f(  m_X+m_Width,  m_Y+m_Height);

		glTexCoord2f( 1, 0);
		glVertex2f( m_X,  m_Y+m_Height);
	glEnd();
#endif
	moFont* pFont = NULL;
	pFont = m_pResourceManager->GetFontMan()->GetFonts()->Get(0);

	if (pFont) {
		//pFont->SetForegroundColor( 0.0, 1.0, 0.0 );
		///tamaño en proporcio al alto de la ventana... y la cantidad de lineas...
		if (m_Texts.Count()>0 && m_Height>0) {
      float size = (float)m_Height / (float)m_Texts.Count();
      glColor4f( 1.0, 1.0, 1.0, 1.0 );
#ifndef OPENGLESV2
      glScalef( size, size, size);
#endif
      //MODebug2->Push("size:"+FloatToStr(size));
      for( MOuint i=0; i<m_Texts.Count(); i++ ) {
        //MODebug2->Push("text: i " +  m_Texts[i]+ "m_X:"+FloatToStr(m_X)+ "m_Y:"+FloatToStr(m_Y) );
        //pFont->Draw( m_X , m_Y + m_Height - (size*1.5)*(i+1)  ,  m_Texts[i], size);
        pFont->Draw( m_X/size , m_Y/size + i*1.2 ,  m_Texts[i], 1 );
      }
    }
	}

}

void
moWindow::Interaction() {

}

void
moWindow::Update() {


}

//===========================================
//
//				mo3dWidget
//
//===========================================

mo3dWidget::mo3dWidget() {

}

mo3dWidget::~mo3dWidget() {

}

void
mo3dWidget::Draw() {

}

void
mo3dWidget::Interaction() {

}

void
mo3dWidget::Update() {


}
