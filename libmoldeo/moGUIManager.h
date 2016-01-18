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

typedef moVector3fArray moPointArray;


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
		virtual ~moGeometry();
		virtual MOboolean Init();
		virtual MOboolean Init( moResourceManager* pResourceManager );
		virtual MOboolean Finish();
    virtual moVector3fArray& GetVertices() {
      return m_Vertices;
    }
    virtual moVector3iArray& GetIndexes() {
      return m_Indexes;
    }
    virtual moVector3iArray& GetFaces() {
      return m_Faces;
    }

    moGeometryType GetType() {
      return m_Type;
    }

    void applyMatrix( const moGLMatrixf &p_ModelMatrix );


  protected:

    moText        m_Name;
    moGeometryType m_Type;

    moPointArray m_Vertices;

    moVector3iArray m_Indexes;
    moVector3iArray m_Faces;
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


class LIBMOLDEO_API moMaterial : public moResourceElement {

  public:

    moMaterial() {
    }
    moMaterial(int p_Id, const moText& p_Name) {
      m_Id = p_Id;
      m_Name = p_Name;
    }
    moMaterial( const moMaterial& p_src ) {
      (*this) = p_src;
    }

    const moMaterial& operator = ( const moMaterial& p_src ) {
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

      m_iPolygonOffset = p_src.m_iPolygonOffset;
      m_iPolygonOffsetFactor = p_src.m_iPolygonOffsetFactor;
      m_iPolygonOffsetUnits = p_src.m_iPolygonOffsetUnits;
      m_fAlphaTest = p_src.m_fAlphaTest;
      m_fOverdraw = p_src.m_fOverdraw;
      m_iSides = p_src.m_iSides;
      return (*this);
    }
    virtual ~moMaterial() {}


    int m_Id;
    moText m_Name;
    float m_fOpacity;
    bool m_bTransparent;
    int m_iBlending;
    int m_iBlendSrc;
    int m_iBlendDst;
    int m_iBlendEquation;
    bool m_bDepthTest;
    bool m_bDepthWrite;

    int m_iPolygonOffset;
    int m_iPolygonOffsetFactor;
    int m_iPolygonOffsetUnits;
    float m_fAlphaTest;
    float m_fOverdraw;
    int m_iSides;


};

class LIBMOLDEO_API moObject3D : public moSceneNode {
  public:
    moObject3D();
    virtual ~moObject3D();

};

class LIBMOLDEO_API moBone : public moSceneNode {
  public:
    moBone();
    virtual ~moBone();

};

class LIBMOLDEO_API moSprite : public moSceneNode {
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
    moPoints();
    virtual ~moPoints();

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


class LIBMOLDEO_API moMesh : public moResourceElement {
  public:
    moMesh();
    moMesh( const moGeometry& p_geometry, const moMaterial& p_material=moMaterial()  );
    virtual ~moMesh();

};

class LIBMOLDEO_API moShape : public moResourceElement {
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
    moSphereGeometry( float radius=1.0, int widthSegments=8, int heightSegments=8, float phiStart=0.0, float phiLength=moMathf::TWO_PI, float thetaStart=0.0, float thetaLength=moMathf::TWO_PI);
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
