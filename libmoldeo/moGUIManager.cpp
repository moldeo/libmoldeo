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

    m_VerticesBuffer = NULL;
    m_VerticesUVBuffer = NULL;
    m_ColorBuffer = NULL;
    m_NormalsBuffer = NULL;
}

moGeometry::moGeometry( moGeometryType p_type ) {
    m_Type = p_type;
    m_VerticesBuffer = NULL;
    m_VerticesUVBuffer = NULL;
    m_ColorBuffer = NULL;
    m_NormalsBuffer = NULL;
}

moGeometry::~moGeometry() {

}

moGeometry&
moGeometry::operator=(const moGeometry& p_src ) {

    m_Vertices = p_src.m_Vertices;
    m_VerticesUvs = p_src.m_VerticesUvs;
    m_Colors = p_src.m_Colors;
    m_Normals = p_src.m_Normals;
/*
    m_ColorBuffer = p_src.m_ColorBuffer;
    m_VerticesBuffer = p_src.m_VerticesBuffer;
    m_VerticesUVBuffer = p_src.m_VerticesUVBuffer;
*/
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
        JSON+= FloatToStr(pt.X(),2,2) + moText(",");
        JSON+= FloatToStr(pt.Y(),2,2) + moText(",");
        JSON+= FloatToStr(pt.Z(),2,2);
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
        JSON+= FloatToStr(m_VerticesBuffer[i*3],2,2) + moText(",");
        JSON+= FloatToStr(m_VerticesBuffer[i*3+1],2,2) + moText(",");
        JSON+= FloatToStr(m_VerticesBuffer[i*3+2],2,2);
        JSON+= "}";
        comma = ",";
    }
    JSON+= "],";
    //JSON+= "    { 0.0, 0.0, 0.0 }";
    JSON+= "}";

    return JSON;
}


float*
moGeometry::GetVerticesBuffer() {

    if (m_Faces.Count()==0) return NULL;

    m_VerticesBuffer = new float [ m_Faces.Count()*3*3 ];
    m_VerticesUVBuffer = new float [ m_Faces.Count()*3*2 ];
    m_ColorBuffer = new float [ m_Faces.Count()*3*3 ];
    m_NormalsBuffer = new float [ m_Faces.Count()*3*3 ];

    if (m_VerticesBuffer && m_VerticesUVBuffer && m_ColorBuffer ) {

        for( MOuint fi=0;fi<m_Faces.Count(); fi++) {

            moFace& Face( m_Faces[fi] );

            for( int pi=0; pi<3; pi++ ) {

                int vpos = Face[pi];
                int indexv3 = fi*9+pi*3;///i*9 = each face is a triangle, 3 components * 3 vertices
                int indexv2 = fi*6+pi*2;
                ///position
                m_VerticesBuffer[ indexv3 ] = m_Vertices[ vpos ].X();
                m_VerticesBuffer[ indexv3 + 1 ] = m_Vertices[ vpos ].Y();
                m_VerticesBuffer[ indexv3 + 2 ] = m_Vertices[ vpos ].Z();
                ///texture uv
                m_VerticesUVBuffer[ indexv2 ] = m_VerticesUvs[vpos].X();
                m_VerticesUVBuffer[ indexv2 + 1 ] = m_VerticesUvs[vpos].Y();
                ///color r,g,b
                m_ColorBuffer[ indexv3 ] = m_Colors[vpos].X();
                m_ColorBuffer[ indexv3 + 1 ] = m_Colors[vpos].Y();
                m_ColorBuffer[ indexv3 + 2 ] = m_Colors[vpos].Z();

                m_NormalsBuffer[ indexv3 ] = m_Normals[vpos].X();
                m_NormalsBuffer[ indexv3 + 1 ] = m_Normals[vpos].Y();
                m_NormalsBuffer[ indexv3 + 2 ] = m_Normals[vpos].Z();

            }
        }
    } else return NULL;

    return &m_VerticesBuffer[0];
}

float*
moGeometry::GetColorBuffer() {
    return m_ColorBuffer;
}

float*
moGeometry::GetVerticesUVBuffer() {
    return m_VerticesUVBuffer;
}

float*
moGeometry::GetNormalsBuffer() {
    return m_NormalsBuffer;
}


moBoxGeometry::moBoxGeometry( float width, float height,float depth, int wsegments, int hsegments, int dsegments ) : moGeometry( MO_GEOMETRY_BOX ) {

}

moBoxGeometry::~moBoxGeometry(  ) {

}



moPlaneGeometry::moPlaneGeometry( float width, float height, int widthSegments, int heightSegments )
: moGeometry(MO_GEOMETRY_PLANE) {

    m_VerticesBuffer = NULL;
    m_VerticesUVBuffer = NULL;
    m_ColorBuffer = NULL;

    m_Name = "MyPlane";
    m_Type = MO_GEOMETRY_PLANE;

    m_Vertices.Init( 0, moVector3f( 0.0, 0.0, 0.0) );
    m_VerticesUvs.Init( 0,  moTCoord( 0.0, 0.0 ) );
    m_Normals.Init( 0, moVector3f( 0.0, 0.0, 0.0) );

    m_Faces.Init( 0, moFace( 0, 0, 0 ) );
    m_FaceVertexUvs.Init( 0, moTCoord( 0.0, 0.0 ) );
    m_Colors.Init( 0, moColor( 0.0, 0.0, 0.0 ) );


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

    m_Normals.Add( moVector3f( 0.0, 0.0, 1.0) );
    m_Normals.Add( moVector3f( 0.0, 0.0, 1.0) );
    m_Normals.Add( moVector3f( 0.0, 0.0, 1.0) );
    m_Normals.Add( moVector3f( 0.0, 0.0, 1.0) );

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
/*
    //set buffers for shaders
    m_VerticesBuffer = new float [ m_Vertices.Count()*3 ];
    if (m_VerticesBuffer)
    for( MOuint i=0;i<m_Vertices.Count(); i++) {
        m_VerticesBuffer[i*3] = m_Vertices[i].X();
        m_VerticesBuffer[i*3+1] = m_Vertices[i].Y();
        m_VerticesBuffer[i*3+2] = m_Vertices[i].Z();
    }


    m_VerticesUVBuffer = new float [ m_VerticesUvs.Count()*2 ];
    if (m_VerticesUVBuffer)
    for( MOuint i=0;i<m_VerticesUvs.Count(); i++) {
        m_VerticesUVBuffer[i*2] = m_VerticesUvs[i].X();
        m_VerticesUVBuffer[i*2+1] = m_VerticesUvs[i].Y();
    }

    m_ColorBuffer = new float [ m_Vertices.Count()*3 ];
    if (m_ColorBuffer)
    for( MOuint i=0;i<m_Colors.Count(); i++) {
        m_ColorBuffer[i*2] = m_Colors[i].X();
        m_ColorBuffer[i*2+1] = m_Colors[i].Y();
    }
*/
}

moPlaneGeometry::~moPlaneGeometry() {
/*
    if (m_VerticesBuffer) { delete[] m_VerticesBuffer; m_VerticesBuffer = NULL; }
    if (m_VerticesUVBuffer) { delete[] m_VerticesUVBuffer; m_VerticesUVBuffer = NULL; }
    if (m_ColorBuffer) { delete[] m_ColorBuffer; m_ColorBuffer = NULL; }
    */
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

#include "moDebugManager.h"

moSphereGeometry::moSphereGeometry( float radius, int widthSegments, int heightSegments, float phiStart, float phiLength, float thetaStart, float thetaLength )
: moGeometry( MO_GEOMETRY_SPHERE ) {


    m_Name = "MySphere";
    m_Type = MO_GEOMETRY_SPHERE;

    //m_Vertices.Init( 0, moVector3f( 0.0, 0.0, 0.0) );
    //m_VerticesUvs.Init( 0,  moTCoord( 0.0, 0.0 ) );
    m_Faces.Init( 0, moFace( 0, 0, 0 ) );
    m_FaceVertexUvs.Init( 0, moTCoord( 0.0, 0.0 ) );
    //m_Colors.Init( 0, moColor( 0.0, 0.0, 0.0 ) );



    ///INITIALIZATION AND save attributes
    //radius = ;
    widthSegments = max( 3, widthSegments );
    heightSegments = max( 2, heightSegments );
    int vertexCount = ( ( widthSegments + 1 ) * ( heightSegments + 1 ) );
    m_Vertices.Init( vertexCount, moVector3f( 0.0, 0.0, 0.0) );
    m_VerticesUvs.Init( vertexCount, moTCoord( 0.0, 0.0) );
    m_Colors.Init( vertexCount, moColor( 1.0, 1.0, 1.0 ) );
    m_Normals.Init( vertexCount, moColor( 0.0, 0.0, 1.0 ) );

/*
    int vertexCount = 4;

    m_Vertices.Set( 0, moVector3f( -1.0*radius, -1.0*radius, 0.0) );
    m_VerticesUvs.Set( 0, moTCoord( 0.0, 1.0 ) );
    m_Colors.Set(  0, moColor( 1.0, 1.0, 1.0 ) );

    m_Vertices.Set( 1, moVector3f( -1.0*radius, 1.0*radius, 0.0) );
    m_VerticesUvs.Set( 1, moTCoord( 0.0, 0.0 ) );
    m_Colors.Set(  1, moColor( 1.0, 1.0, 1.0 ) );

    m_Vertices.Set( 2, moVector3f( 1.0*radius, 1.0*radius, 0.0) );
    m_VerticesUvs.Set( 2, moTCoord( 1.0, 0.0 ) );
    m_Colors.Set(  2, moColor( 1.0, 1.0, 1.0 ) );

    m_Vertices.Set( 3, moVector3f( 1.0*radius, -1.0*radius, 0.0) );
    m_VerticesUvs.Set( 3, moTCoord( 1.0, 1.0 ) );
    m_Colors.Set(  3, moColor( 1.0, 1.0, 1.0 ) );

    m_Faces.Add( moFace( 0, 1, 2 ) );
    m_Faces.Add( moFace( 2, 3, 0 ) );
*/

    ///GENERATE GEOMETRY VERTEX,TEXTUREMAPPING AND COLORS
    int index = 0;
    float thetaEnd = thetaStart + thetaLength;
    for ( int j = 0; j <= heightSegments; j++ ) {

      float v = ((float)j) / (float)heightSegments;

      for ( int i = 0; i <= widthSegments; i++ ) {

        float u = ((float)i) / ((float)widthSegments);

        float px = - radius * moMathf::Cos( phiStart + u * phiLength ) * moMathf::Sin( thetaStart + v * thetaLength );
        float py = radius * moMathf::Cos( thetaStart + v * thetaLength );
        float pz = radius * moMathf::Sin( phiStart + u * phiLength ) * moMathf::Sin( thetaStart + v * thetaLength );

        moVector3f normal( px, py, pz );
        normal.Normalize();
        moVector3f position( px, py, pz );
        moTCoord tcoord( u, 1-v );
        m_Vertices.Set( index, position );
        m_VerticesUvs.Set( index, tcoord );
        m_Normals.Set( index, normal );
        index ++;
      }
    }

	for ( int j = 0; j < heightSegments; j ++ ) {

		for ( int i = 0; i < widthSegments; i ++ ) {

			int i1 = j*(widthSegments+1) + i + 1;
			int i2 = j*(widthSegments+1) + i;
			int i3 = (j + 1)*(widthSegments+1) + i;
			int i4 = (j + 1)*(widthSegments+1) + i + 1;

			if ( j != 0 || thetaStart > 0 ) {
                m_Faces.Add( moFace( i1, i2, i4 ) );
            }
			if ( j != heightSegments - 1 || thetaEnd < moMathf::PI ) {
                m_Faces.Add( moFace( i2, i3, i4 ) );
            }

		}

	}
/*
	this.setIndex( new ( positions.count > 65535 ? THREE.Uint32Attribute : THREE.Uint16Attribute )( indices, 1 ) );
	this.addAttribute( 'position', positions );
	this.addAttribute( 'normal', normals );
	this.addAttribute( 'uv', uvs );

	this.boundingSphere = new THREE.Sphere( new THREE.Vector3(), radius );

    */

    /**
    set buffers for shaders (GL_TRIANGLES mode, 3 triangle vertices (v1,v2,v3) x 3 components(x,y,z) = 9 values for each Face

     A----C
      | /
      |/
     B

     A = (x,y,z)
     B = (x,y,z)
     C = (x,y,z)

    */
    /*
    m_VerticesBuffer = new float [ m_Faces.Count()*3*3 ];
    m_VerticesUVBuffer = new float [ m_Faces.Count()*3*2 ];
    m_ColorBuffer = new float [ m_Faces.Count()*3*3 ];

    if (m_VerticesBuffer && m_VerticesUVBuffer && m_ColorBuffer ) {
        for( MOuint i=0;i<m_Faces.Count(); i++) {
            moFace& Face( m_Faces[i] );
            for( int pi=0; pi<3; pi++ ) {
                int vpos = Face[pi];
                int indexv3 = i*9+pi;///i*9 = each face is a triangle, 3 components * 3 vertices
                int indexv2 = i*6+pi;
                ///position
                m_VerticesBuffer[ indexv3 ] = m_Vertices[ vpos ].X();
                m_VerticesBuffer[ indexv3 + 1 ] = m_Vertices[ vpos ].Y();
                m_VerticesBuffer[ indexv3 + 2 ] = m_Vertices[ vpos ].Z();
                ///texture uv
                m_VerticesUVBuffer[ indexv2 ] = m_VerticesUvs[vpos].X();
                m_VerticesUVBuffer[ indexv2 + 1 ] = m_VerticesUvs[vpos].Y();
                ///color r,g,b
                m_ColorBuffer[ indexv3 ] = m_Colors[vpos].X();
                m_ColorBuffer[ indexv3 + 1 ] = m_Colors[vpos].Y();
                m_ColorBuffer[ indexv3 + 2 ] = m_Colors[vpos].Z();

            }
        }
    }
    */

/*
    moDebugManager::Message( " SphereGeometry :" + IntToStr(vertexCount));
    for( int i=0; i<vertexCount; i++ ) {

        moVertex& vv( m_Vertices[i] );
        moColor& cc( m_Colors[i] );
        moTCoord& tc( m_VerticesUvs[i] );

        moDebugManager::Message( " i:" + IntToStr(i)
        + " px: " + FloatToStr(vv.X(),2,2)
        + " py: " + FloatToStr(vv.Y(),2,2)
        + " pz: " + FloatToStr(vv.Z(),2,2)
        + " cr: " + FloatToStr( cc.X(),2,2)
        + " cg: " + FloatToStr( cc.Y(),2,2)
        + " cb: " + FloatToStr( cc.Z(),2,2)
        + " tu: " + FloatToStr( tc.X(),2,2)
        + " tv: " + FloatToStr( tc.Y(),2,2)

         );
    }

    for( int i=0; i<m_Faces.Count(); i++ ) {
        for( int pi=0; pi<3; pi++ ) {
            int indexv3 = i*9+pi;///i*9 = each face is a triangle, 3 components * 3 vertices
            int indexv2 = i*6+pi;
            moVertex vv(    m_VerticesBuffer[indexv3],
                            m_VerticesBuffer[indexv3+1],
                            m_VerticesBuffer[indexv3+2]
                            );
            moColor cc( m_ColorBuffer[indexv3],
                        m_ColorBuffer[indexv3+1],
                        m_ColorBuffer[indexv3+2] );
            moTCoord tc(  m_VerticesUVBuffer[indexv2],
                          m_VerticesUVBuffer[indexv2+1] );

            moDebugManager::Message( " Face i:" + IntToStr(i)
            + " px: " + FloatToStr(vv.X(),2,2)
            + " py: " + FloatToStr(vv.Y(),2,2)
            + " pz: " + FloatToStr(vv.Z(),2,2)
            + " cr: " + FloatToStr( cc.X(),2,2)
            + " cg: " + FloatToStr( cc.Y(),2,2)
            + " cb: " + FloatToStr( cc.Z(),2,2)
            + " tu: " + FloatToStr( tc.X(),2,2)
            + " tv: " + FloatToStr( tc.Y(),2,2)

             );
         }
    }
*/
}

moSphereGeometry::~moSphereGeometry() {
/*
    if (m_VerticesBuffer) { delete[] m_VerticesBuffer; m_VerticesBuffer = NULL; }
    if (m_VerticesUVBuffer) { delete[] m_VerticesUVBuffer; m_VerticesUVBuffer = NULL; }
    if (m_ColorBuffer) { delete[] m_ColorBuffer; m_ColorBuffer = NULL; }
    */
}





moObject3D::moObject3D() {

    ///TODO: make some 3d initialization here! complete!

    m_Position = moPosition( 0.0, 0.0, 0.0 );
    m_Rotation = moVector3f( 0.0, 0.0, 0.0 );
    m_Scale = moVector3f( 1.0, 1.0, 1.0 );
    m_ModelMatrix.MakeIdentity();
    m_Parent = NULL;
    m_Id = 0;

}


/**
*    moMaterialBase (Color/Opacity/Blending/Depth Material)
*/

moMaterialBase::moMaterialBase() {
    m_Id = -1;
    m_Name = "";
    m_Type = "MaterialBase";
    m_fOpacity = 1.0;
    m_bDepthTest = true;
    m_bDepthWrite = true;
    m_bInitialized = true;
    m_iBlending = MO_BLENDING_TRANSPARENCY;
    m_bTransparent = true;
    m_iBlendEquation = 0;
    m_fAlphaTest = true;
    m_iBlendDst = 0;
    m_iBlendSrc = 0;
    m_fWireframeWidth = 0.0;
    m_fTextWSegments = 1.0;
    m_fTextHSegments = 1.0;
    m_vLight = moVector3f( -1, -1, -1 );
}
moMaterialBase::moMaterialBase(int p_Id, const moText& p_Name) {
  (*this) = moMaterialBase();
  m_Id = p_Id;
  m_Name = p_Name;
}

const moMaterialBase&
moMaterialBase::operator= ( const moMaterialBase& p_src ) {
    m_Id = p_src.m_Id;
    m_Name = p_src.m_Name;
    m_fOpacity = p_src.m_fOpacity;
    m_bTransparent = p_src.m_bTransparent;
    m_iBlending = p_src.m_iBlending;
    m_iBlendSrc = p_src.m_iBlendSrc;
    m_iBlendDst = p_src.m_iBlendDst;
    m_iBlendEquation = p_src.m_iBlendEquation;
    m_bDepthTest = p_src.m_bDepthTest;
    m_bDepthWrite = p_src.m_bDepthWrite;
    m_fWireframeWidth = p_src.m_fWireframeWidth;
    m_fTextWSegments = p_src.m_fTextWSegments;
    m_fTextHSegments = p_src.m_fTextHSegments;
    m_vLight = p_src.m_vLight;

    m_iPolygonOffset = p_src.m_iPolygonOffset;
    m_iPolygonOffsetFactor = p_src.m_iPolygonOffsetFactor;
    m_iPolygonOffsetUnits = p_src.m_iPolygonOffsetUnits;
    m_fAlphaTest = p_src.m_fAlphaTest;
    m_fOverdraw = p_src.m_fOverdraw;
    m_iSides = p_src.m_iSides;
    m_Type = p_src.m_Type;
    return (*this);
}


/**
*    moMaterial (Map Material)
*/


moMaterial::moMaterial() : moMaterialBase() {
    m_Map = NULL;
    m_Type = "Material";
    m_Color = moColor( 1.0, 1.0, 1.0 );
    m_MapGLId = -1;
}

const moMaterial&
moMaterial::operator= ( const moMaterial& p_src ) {
  m_Id = p_src.m_Id;
  m_Name = p_src.m_Name;
  m_fOpacity = p_src.m_fOpacity;
  m_bTransparent = p_src.m_bTransparent;
  m_iBlending = p_src.m_iBlending;
  m_iBlendSrc = p_src.m_iBlendSrc;
  m_iBlendDst = p_src.m_iBlendDst;
  m_iBlendEquation = p_src.m_iBlendEquation;
  m_bDepthTest = p_src.m_bDepthTest;
  m_bDepthWrite = p_src.m_bDepthWrite;
  m_fWireframeWidth = p_src.m_fWireframeWidth;
  m_fTextWSegments = p_src.m_fTextWSegments;
  m_fTextHSegments = p_src.m_fTextHSegments;
  m_vLight = p_src.m_vLight;

  m_iPolygonOffset = p_src.m_iPolygonOffset;
  m_iPolygonOffsetFactor = p_src.m_iPolygonOffsetFactor;
  m_iPolygonOffsetUnits = p_src.m_iPolygonOffsetUnits;
  m_fAlphaTest = p_src.m_fAlphaTest;
  m_fOverdraw = p_src.m_fOverdraw;
  m_iSides = p_src.m_iSides;
  m_Type = p_src.m_Type;

  m_Map = p_src.m_Map;
  m_MapGLId = p_src.m_MapGLId;
  m_Color = p_src.m_Color;
  m_SpecularColor = p_src.m_SpecularColor;
  m_AmbientColor = p_src.m_AmbientColor;

  m_PolygonMode = p_src.m_PolygonMode;
  m_Blending = p_src.m_Blending;
  return (*this);
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
	m_OpHandle = (MO_HANDLE) NULL;
	m_VisHandle = (MO_HANDLE) NULL;
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
