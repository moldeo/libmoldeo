/*******************************************************************************

                                moGUIManager.h

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

#ifndef __MOGUIMANAGER_H
#define __MOGUIMANAGER_H

#include <moMathVector.h>
#include <moMathVector3.h>
#include <moMathVector4.h>
#include <mo3dModelManager.h>

typedef moVector3f moPointf;
typedef moVector3d moPointd;
typedef moPointf moPoint;
typedef moVertex3f moVertex;
typedef moVertex3f moPosition;
typedef moVector3i moFace;
typedef moMatrix3f moFace3;
typedef moVector2f moTCoord;
typedef moVertex3f moColorRGB;
typedef moVector4f moColorRGBA;
typedef moColorRGB moColor;
typedef moGLMatrixf moCameraMatrix;
typedef moGLMatrixf moCamera3DBase;

class LIBMOLDEO_API moCamera3D : public moCamera3DBase {
    public:
        moCamera3D() : moCamera3DBase() {
            m_Position = moPosition( 0.0, 0.0, -10.0);
            m_Center = moPosition( 0.0, 0.0, 0.0);
        }
        virtual ~moCamera3D() {

        }
        moCamera3D( const moCamera3D& p_src );
        moCamera3D& operator=( const moCamera3D& p_src );
        moCamera3D& operator=( const moCamera3DBase& p_src );

        moPosition m_Position;
        moPosition m_Center;

};

typedef moVector3fArray moPointArray;
typedef moVector3fArray moColorArray;
typedef moVector3fArray moColorRGBArray;
typedef moVector4fArray moColor4fArray;
typedef moVector4fArray moColorRGBAArray;
typedef moVector2fArray moTCoordArray;
typedef moVector3fArray moVertexArray;
typedef moVector3iArray moFaceArray;
typedef moMatrix3fArray moFace3Array;

enum moGeometryType {
  MO_GEOMETRY_UNDEFINED=-1,
  MO_GEOMETRY_POINT=0,
  MO_GEOMETRY_BOX=1,
  MO_GEOMETRY_CIRCLE=2,
  MO_GEOMETRY_CYLINDER=3,
  MO_GEOMETRY_SHAPE=4,
  MO_GEOMETRY_PLANE=5,
  MO_GEOMETRY_EXTRUDE=6,
  MO_GEOMETRY_RING=7,
  MO_GEOMETRY_SPHERE=8,
  MO_GEOMETRY_POLYHEDRON=9,
  MO_GEOMETRY_ICOSAHEDRON=10,
  MO_GEOMETRY_DODECAHEDRON=11,
  MO_GEOMETRY_TETRAHEDRON=13,
  MO_GEOMETRY_TEXT=14,
  MO_GEOMETRY_TUBE=15,
  MO_GEOMETRY_MAX=16
};


class LIBMOLDEO_API moAttribute : public moResourceElement {

    public:

        moAttribute( void* p_buffer=NULL, long p_itemsize=0, long p_length=0 ) { buffer=p_buffer; itemsize=p_itemsize; length=p_length; }
        virtual ~moAttribute() {}

    protected:

        void* buffer;
        long itemsize;
        long length;

};

moDeclareExportedDynamicArray( moAttribute, moAttributeArray);

/// Base abstracta de base para las geometrías.
/**
*	Todas las geometrías como Cubo, Esfera, Triangulo, .. derivan de esta clase
* que contiene un vector de vertices y otro de indices que referencian a esos vertices.
* El modo directo que consiste en simplemente usar el vector de vertices para dibujar la geometría, puede
* forzarse al llamar al constructor moGeometry( p_direct_mode = false|true  )
*
*/
class LIBMOLDEO_API moGeometry : public moResourceElement {

	public:
		moGeometry();
		moGeometry( moGeometryType p_type );
		virtual ~moGeometry();

		moGeometry( const moGeometry& p_src ) : moResourceElement(MO_RESOURCETYPE_GUI) {
            (*this) = p_src;
		}
		moGeometry& operator=(const moGeometry& p_src );
		virtual MOboolean Init();
		virtual MOboolean Init( moResourceManager* pResourceManager );
		virtual MOboolean Finish();

        virtual float* GetVerticesBuffer();
        virtual float* GetNormalsBuffer();
        virtual float* GetColorBuffer();
        virtual float* GetVerticesUVBuffer();

        virtual const moPointArray& GetVertices() const {
          return m_Vertices;
        }

        virtual const moPointArray& GetNormals() const {
          return m_Normals;
        }

        virtual const moTCoordArray& GetVerticesUV() const {
          return m_VerticesUvs;
        }

        virtual const moTCoordArray& GetFacesUV() const {
          return m_FaceVertexUvs;
        }

        virtual const moColorArray& GetColors() const {
            return m_Colors;
        }


        virtual const moFaceArray& GetFaces() const {
          return m_Faces;
        }

        moGeometryType GetType() const {
          return m_Type;
        }

        void applyMatrix( const moGLMatrixf &p_ModelMatrix );

        int addAttribute( const moText& p_AttributeName, moAttribute* p_attribute );
        int removeAttribute( const moText& p_AttributeName );
        moAttribute* getAttribute( const moText& p_AttributeName );

        static moText TypeToStr( moGeometryType p_type );
        virtual moText ToJSON();

  public:

        moText        m_Name;
        moGeometryType m_Type;

        moPointArray m_Vertices;
        moTCoordArray m_VerticesUvs;
        moVertexArray m_Normals;
        moColorArray    m_Colors;

        MOfloat*       m_VerticesBuffer;
        MOfloat*       m_NormalsBuffer;
        MOfloat*       m_VerticesUVBuffer;
        MOfloat*       m_ColorBuffer;

        moFaceArray m_Faces;//array of triangles, 3 points referencing each an index of m_Vertices.
        moTCoordArray m_FaceVertexUvs;//array of texture coordinates for each vertex, corresponding to each face from m_Faces
        moAttributeArray m_Attributes;/// MUST BE A MAP std::map<index,string>
};


class LIBMOLDEO_API moPath : public  moResourceElement {
  public:
    moPath();
    moPath( const moVector2fArray& p_Path );
    virtual ~moPath();

    void lineTo( float X, float Y);
    void moveTo( float X, float Y);

  protected:
    float offx,offy;
    moVector2fArray m_Path;

};


class LIBMOLDEO_API moMaterialBase : public moResourceElement {

  public:

    moMaterialBase();
    moMaterialBase(int p_Id, const moText& p_Name);
    moMaterialBase( const moMaterialBase& p_src ) : moResourceElement(MO_RESOURCETYPE_GUI) {
      (*this) = p_src;
    }
    const moMaterialBase& operator = ( const moMaterialBase& p_src );
    virtual ~moMaterialBase() {}


    int m_Id;
    moText m_Name;
    moText m_Type;

    float m_fOpacity;
    bool m_bTransparent;
    int m_iBlending;
    int m_iBlendSrc;
    int m_iBlendDst;
    int m_iBlendEquation;
    bool m_bDepthTest;
    bool m_bDepthWrite;
    float m_fWireframeWidth;
    float m_fTextWSegments;
    float m_fTextHSegments;
    moVector3f m_vLight;

    int m_iPolygonOffset;
    int m_iPolygonOffsetFactor;
    int m_iPolygonOffsetUnits;
    float m_fAlphaTest;
    float m_fOverdraw;
    int m_iSides;

};

class LIBMOLDEO_API moMaterial : public moMaterialBase {
    public:
        moMaterial();
        moMaterial( const moMaterial& p_src ) : moMaterialBase() {
          (*this) = p_src;
        }
        const moMaterial& operator= ( const moMaterial& p_src );
        virtual ~moMaterial() {}


        moColor m_SpecularColor;
        moColor m_AmbientColor;
        moColor m_Color;
        moTexture*   m_Map;
        MOint      m_MapGLId;
        moPolygonModes m_PolygonMode;
        moBlendingModes m_Blending;

};


class LIBMOLDEO_API moObject3D : public moSceneNode {
  public:
    moObject3D();
    moObject3D(const moGeometry& p_geometry, const moMaterial& p_material ) : moSceneNode() {
        m_Geometry = p_geometry;
        m_Material = p_material;
    }
    virtual ~moObject3D() {

    }

    moObject3D( const moObject3D& p_src ) : moSceneNode() {
        (*this) = p_src;
    }

    moObject3D& operator=( const moObject3D& p_src ) {
        m_Geometry = p_src.m_Geometry;
        m_Material = p_src.m_Material;
        m_Position = p_src.m_Position;
        m_Scale = p_src.m_Scale;
        m_Rotation = p_src.m_Rotation;
        return (*this);
    }

    void SetPosition( const moPosition& p_position ) {
      m_Position = p_position;
    }
    void SetRotation( const moPosition& p_rotation ) {
      m_Rotation = p_rotation;
    }
    void SetScale( const moPosition& p_scale ) {
      m_Scale = p_scale;
    }

    moGeometry  m_Geometry;
    moMaterial  m_Material;
    moPosition  m_Position;
    moVector3f  m_Scale;
    moVector3f  m_Rotation;

};

class LIBMOLDEO_API moBone : public moObject3D {
  public:
    moBone();
    virtual ~moBone();

};

class LIBMOLDEO_API moSprite : public moObject3D {
  public:
    moSprite();
    virtual ~moSprite();

};

class LIBMOLDEO_API moLine : public moObject3D {
  public:
    moLine();
    virtual ~moLine();

};

class LIBMOLDEO_API moLineSegments : public moObject3D {
  public:
    moLineSegments();
    virtual ~moLineSegments();

};

class LIBMOLDEO_API moPoints : public moObject3D {
  public:
    moPoints( const moGeometry& p_geometry, const moMaterial& p_material ) :  moObject3D(p_geometry, p_material) {

        m_Geometry = p_geometry;
        m_Material = p_material;

    }
    moPoints( const moPoints& p_src ) : moObject3D( p_src ) {
        (*this) = p_src;
    }

    moPoints& operator=( const moPoints& p_src ) {
        m_Geometry = p_src.m_Geometry;
        m_Material = p_src.m_Material;
        return (*this);
    }

    virtual ~moPoints() {
    }

};

class LIBMOLDEO_API moLOD : public moObject3D {
  public:
    moLOD();
    virtual ~moLOD();

};

class LIBMOLDEO_API moSkinnedMesh : public moObject3D {
  public:
    moSkinnedMesh();
    virtual ~moSkinnedMesh();

};

class LIBMOLDEO_API moSkeleton : public moObject3D {
  public:
    moSkeleton();
    virtual ~moSkeleton();

};


class LIBMOLDEO_API moMesh : public moObject3D {
  public:
    moMesh( const moGeometry& p_geometry, const moMaterial& p_material ) :  moObject3D(p_geometry, p_material) {

        m_Geometry = p_geometry;
        m_Material = p_material;

    }
    moMesh( const moMesh& p_src ) : moObject3D( p_src ) {
        (*this) = p_src;
    }

    moMesh& operator=( const moMesh& p_src ) {
        m_Geometry = p_src.m_Geometry;
        m_Material = p_src.m_Material;
        return (*this);
    }

    virtual ~moMesh() {
    }

};

class LIBMOLDEO_API moShape : public moObject3D {
  public:
    moShape();
    virtual ~moShape();

};

class LIBMOLDEO_API moBoxGeometry : public moGeometry {
  public:
    moBoxGeometry( float width=1.0, float height=1.0,float depth=1.0, int wsegments=1, int hsegments=1, int dsegments=1 );
    virtual ~moBoxGeometry();
};

class LIBMOLDEO_API moCircleGeometry : public moGeometry {
  public:
    moCircleGeometry( float radius=1.0, float segments=36 );
    virtual ~moCircleGeometry();
};

class LIBMOLDEO_API moCylinderGeometry : public moGeometry {
  public:
    moCylinderGeometry( float radiusTop=1.0, float radiusBottom=1.0, float height=1.0, int radiusSegments=20, int heightSegments=1, int openEnded=true, float thetaStart=0, float thetaLength=moMathf::TWO_PI );
    virtual ~moCylinderGeometry();
};





class LIBMOLDEO_API moPolyhedronGeometry : public moGeometry {
  public:
    moPolyhedronGeometry();
    moPolyhedronGeometry( const moVector3fArray& p_Vertices, const moVector3iArray& p_Faces, float radius=1.0, float detail=0.0 );
    virtual ~moPolyhedronGeometry();
};

class LIBMOLDEO_API moDodecahedronGeometry : public moGeometry {
  public:
    moDodecahedronGeometry( float radius=1.0, float detail=0.0 );
    virtual ~moDodecahedronGeometry();
};

class LIBMOLDEO_API moIcosahedronGeometry : public moGeometry {
  public:
    moIcosahedronGeometry( float radius=1.0, float detail=0.0 );
    virtual ~moIcosahedronGeometry();
};

class LIBMOLDEO_API moTetrahedronGeometry : public moGeometry {
  public:
    moTetrahedronGeometry( float radius=1.0, float detail=0.0 );
    virtual ~moTetrahedronGeometry();
};

class LIBMOLDEO_API moOctahedronGeometry : public moGeometry {
  public:
    moOctahedronGeometry( float radius=1.0, float detail=0.0 );
    virtual ~moOctahedronGeometry();
};

class LIBMOLDEO_API moShapeGeometry : public moGeometry {
  public:
    moShapeGeometry();
    virtual ~moShapeGeometry();
};

class LIBMOLDEO_API moPlaneGeometry : public moGeometry {
  public:
    moPlaneGeometry( float width=1.0, float height=1.0, int widthSegments =1.0, int heightSegments =1.0 );
    virtual ~moPlaneGeometry();
};

class LIBMOLDEO_API moExtrudeGeometry : public moGeometry {
  public:
    moExtrudeGeometry( moShapeGeometry p_shapes[], const moText& p_options );
    virtual ~moExtrudeGeometry();
};

class LIBMOLDEO_API moRingGeometry : public moGeometry {
  public:
    moRingGeometry( float innerRadius=0.5, float outerRadius=1.0, int thetaSegments=8, int phiSegments=8, float thetaStart=0.0, float thetaLength=moMathf::TWO_PI );
    virtual ~moRingGeometry ();
};

class LIBMOLDEO_API moSphereGeometry : public moGeometry {
  public:
    moSphereGeometry( float radius=1.0, int widthSegments=8, int heightSegments=8, float phiStart=0.0, float phiLength=moMathf::TWO_PI, float thetaStart=0.0, float thetaLength=moMathf::PI);
    virtual ~moSphereGeometry ();
};

/**
*
* parameters:
    size — Float. Size of the text.
    height — Float. Thickness to extrude text. Default is 50.
    curveSegments — Integer. Number of points on the curves. Default is 12.
    font — String. Font name.
    weight — String. Font weight (normal, bold).
    style — String. Font style (normal, italics).
    bevelEnabled — Boolean. Turn on bevel. Default is False.
    bevelThickness — Float. How deep into text bevel goes. Default is 10.
    bevelSize — Float. How far from text outline is bevel. Default is 8.

*
*/

class LIBMOLDEO_API moTextGeometry : public moGeometry {
  public:
    moTextGeometry( const moText& p_text, const moText& parameters );
    moTextGeometry( const moText& p_text,
                   float size = 1.0,
                   float height = 1.0,
                   int curveSegments=12,
                   const moText& font="Default",
                   const moText& weight="normal",
                   const moText& style="normal",
                   bool bevelEnabled=false,
                   int bevelThickness=10.0,
                   int bevelSize=8.0  );
    virtual ~moTextGeometry ();
};

/**
path — Curve - A path that inherits from the Curve base class
segments — Integer - The number of segments that make up the tube, default is 64
radius — Float - The radius of the tube, default is 1
radiusSegments — Integer - The number of segments that make up the cross-section, default is 8
closed — Float Is the tube open or closed, default is false
*/
class LIBMOLDEO_API moTubeGeometry : public moGeometry {
  public:
    moTubeGeometry( const moPath& path, int segments=64, float radius=1.0, int radiusSegments=8, bool closed=false);
    virtual ~moTubeGeometry ();
};


class LIBMOLDEO_API moAxis3D : public moGeometry {
  public:
    moAxis3D( float p_size=1.0 );
    virtual ~moAxis3D();
};

class LIBMOLDEO_API moBoundingBox3D : public moGeometry {
  public:
    moBoundingBox3D( const moGeometry& p_geometry );
    virtual ~moBoundingBox3D();
};


/// Base abstracta de los objetos de interface de usuario (GUI).
/**
*	Los moWidget 's derivan de este objeto y todos aquellos que necesiten ser accedidos por el usuario.
*/
class LIBMOLDEO_API moGuiObject : public moAbstract {

	public:
		moGuiObject();
		virtual ~moGuiObject();
		virtual MOboolean Init( moResourceManager* pResourceManager );
		virtual MOboolean Finish();

	protected:
		moResourceManager*	m_pResourceManager;

};

moDeclareExportedDynamicArray( moGuiObject*, moGuiObjectArray);

/// Objeto de interface de usuario tipo ventana.
/**
*	Los moWidget 's  son los elementos básicos del GUI ( Graphical User Interface ) de Moldeo.
*/
class LIBMOLDEO_API moWidget : public moGuiObject {

	public:
		moWidget();
		virtual ~moWidget();

		virtual void Draw() = 0;
		virtual void Interaction() = 0;
		virtual void Update() = 0;

};

/// Ventana simple
/**
*	Dibuja una ventana especificando posición y tamaño
*
*	@see moWidget
*	@see moGUIObject
*	@see moGUIManager
*/
class LIBMOLDEO_API moWindow : public moWidget {

	public:
		moWindow();
		virtual ~moWindow();
		virtual MOboolean Init( moResourceManager* pResourceManager, MOfloat x, MOfloat y, MOfloat width, MOfloat height );

		void	SetInfo( moTextArray &pTexts );

		virtual void Draw();
		virtual void Interaction();
		virtual void Update();

		moTextArray	m_Texts;

		MOfloat m_X;
		MOfloat m_Y;
		MOfloat m_Width;
		MOfloat m_Height;

};

/// moWidget 3D - objeto tridimensional
/**
*	Con este tipo de objetos introducimos el concepto de objetos GUI interfaceables tridimensionalmente
*
*
*	@see moWidget
*	@see moGUIObject
*	@see moGUIManager
*/
class LIBMOLDEO_API mo3dWidget : public moWidget, public moSceneNode {

	public:
		mo3dWidget();
		virtual ~mo3dWidget();

		virtual void Draw();
		virtual void Interaction();
		virtual void Update();

};

/// Administrador de recursos GUI
/**
*	Aquí se guardan los moWidgets generados y mostrados para el usuario.
*
*	@see moWidget
*	@see moGUIObject
*/
class LIBMOLDEO_API moGUIManager : public moResource {

	public:

		moGUIManager();
		virtual ~moGUIManager();
		virtual MOboolean Init();
		virtual MOboolean Init( MO_HANDLE p_OpHandle, MO_DISPLAY p_Display );
		virtual MOboolean Finish();


		moWindow* NewWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextHeap &textheap );

		//moWidget*	GetWidget( MOint widgetid );

		void	DisplayInfoWindow( MOfloat x, MOfloat y, MOfloat width, MOfloat height, moTextArray &pTexts );
		//void	DisplayGauge( MOfloat x, MOfloat y, MOfloat width, MOfloat height, MOfloat percent );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );

		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );
		//void	DisplayText( MOfloat x, MOfloat y, moText ptext );

    void    SetDisplay( MO_DISPLAY p_Display );
    void    SetOpWindowHandle( MO_HANDLE p_OpHandle );
    void    SetVisWindowHandle( MO_HANDLE p_VisHandle );

    MO_DISPLAY GetDisplay();
    MO_HANDLE GetOpWindowHandle();
    MO_HANDLE GetVisWindowHandle();
/*
		HWND GetOpWindowHandle() { return hOpWnd; }
		HWND GetVisWindowHandle() { return hVisWnd; }
		*/


	private:
	/*
		HWND hOpWnd;
		HWND hVisWnd;*/
		MO_DISPLAY  m_Display;
		MO_HANDLE   m_OpHandle;
		MO_HANDLE   m_VisHandle;

		moGuiObjectArray	m_GuiObjects;
};


#endif /* __MO_GUIMANAGER_H__ */
