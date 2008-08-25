#include "moTypes.h"
#include "moResourceManager.h"
#include "moConfig.h"
#include "moParam.h"
#include "moTextureManager.h"
#include "moEffectState.h"

#ifndef __MO_MODELOS3D_H
#define __MO_MODELOS3D_H

#define MO_3DMODEL_ERROR NULL

typedef enum mo3dModelType {

  MO_MODEL_X3D = 0,
  MO_MODEL_3DS = 1,
  MO_MODEL_OBJ = 2,
  MO_MODEL_MD2 = 3,
  MO_MODEL_MD3 = 4

};

typedef struct mo3dModelParam {

    MOuint  nObjects;

};


//===========  3D  ================//
class LIBMOLDEO_API CVector3
{
public:
    float x, y, z;
};

// This is our 2D point class.  This will be used to store the UV coordinates.
class LIBMOLDEO_API CVector2
{
public:
    float x, y;
};

// This is our face structure.  This is is used for indexing into the vertex
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct moFace
{
    int vertIndex[3];           // indicies for the verts that make up this triangle
    int coordIndex[3];          // indicies for the tex coords to texture this face
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.

struct moTextureMapInfo
{
	Uint16 id;	// id for type identification( opacity map, specular map, ...)
	char  strFile[255];         // The texture file name(If this is set it's a texture map)
	int   textureId;             // the texture ID
	float uTile;
	float vTile;
	float uOffset;
	float vOffset;
	float rotation;
};

struct moMaterialInfo
{

	char  strName[255];         // The material name
        MOubyte  color[3];             // The color of the object (R, G, B)

	MOubyte colorAmbient[3];
	MOubyte colorDiffuse[3];
	MOubyte colorSpecular[3];

	float shininess;
	float shininessStrength;

	float transparency;

	bool twoSided;
	bool wireFrame;

	std::vector<moTextureMapInfo> texMaps;
} ;




// This holds all the information for our model/scene.
// You should eventually turn into a robust class that
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct mo3dObject
{
    int  numOfVerts;            // The number of verts in the model
    int  numOfFaces;            // The number of faces in the model
    int  numTexVertex;          // The number of texture coordinates
    int  materialID;            // The texture ID to use, which is the index into our texture array
    bool bHasTexture;           // This is TRUE if there is a texture map for this object
    char strName[255];          // The name of the object
    CVector3  *pVerts;          // The object's vertices
    CVector3  *pNormals;        // The object's normals
    CVector2  *pTexVerts;       // The texture's UV coordinates
    moFace *pFaces;              // The faces information of the object
};

// This holds our model information.  This should also turn into a robust class.
// We use STL's(Standard Template Library) vector class to ease our link list burdens. :)
struct mo3dModel
{
	moText	name;
    int numOfObjects;                   // The number of objects in the model
    int numOfMaterials;                 // The number of materials for the model
    vector<moMaterialInfo> pMaterials;   // The list of material information(Textures and colors)
    vector<mo3dObject> pObject;          // The object list for our model
};

#include "3ds/3ds.h"

/// Clase base para los modelos tridimensionales
/**
* De esta clase deber�an derivar los objetos tridimensionales importados por los recursos
*
*/
class LIBMOLDEO_API mo3DModel : public moAbstract {

    public:

        mo3DModel();
        virtual ~mo3DModel();

        virtual MOboolean Init();
        virtual MOboolean Init( mo3dModel* p_pModel ) {
            m_pModel = p_pModel;
        }
        virtual MOboolean Finish();

        virtual void Draw( moEffectState *state, GLuint g_ViewMode );
        virtual void Update();
        virtual void Interaction();

    protected:

        /** 3ds model structure */
        mo3dModel* m_pModel;

};

class LIBMOLDEO_API mo3dModelManager :  public moResource {

public:
	moTextureManager	*Textures;

	MOuint nModels;
	MOuint nMaxModels;
	mo3dModel **Models;

	mo3dModelManager();
	mo3dModelManager(int);
	virtual ~mo3dModelManager();

	MOboolean Init();
	MOboolean Finish();

	mo3dModel* Get3dModel(moText namemodelo);

	void MoldeoLogo(long ticks=0);


private:

    mo3DModel*  m_pMoldeoLogo;

	mo3dModel* Load3dModel( moText namemodelo, moText datapath = "");


};

class LIBMOLDEO_API mo3dModelManagerRef : public moAbstract{
public:

	MOuint nModels;
	mo3dModel **Models;

	mo3dModelManagerRef();
	virtual ~mo3dModelManagerRef();

	MOboolean Init(moConfig*,MOuint,mo3dModelManager *);
	void Draw(int imodel, moEffectState* state, GLuint g_ViewMode);
	int Add(moText,mo3dModelManager*);
	MOboolean Finish();
	mo3dModel* Get(int i);
	//MOuint Get(int);//sin sincronizacion , como venga: si tiene frames por segundos...
	//MOuint Get(int,int);//sin sincronizacion , un cuadro arbitrario
	//MOuint Get(int,moTempo*);//con syncro
	//MOuint GetType(int);//devuelve el type al que corresponde la imagen esa

	private:
	void LoadModels(moConfig*,MOuint,mo3dModelManager*);
};

#endif
