#ifndef _3DS_H
#define _3DS_H

#include "mo3dModelManager.h"

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D	 // This gives the version of the mesh and is found right before the material and object information
#define VERSION       0x0002	 // This gives the version of the .3ds file
#define EDITKEYFRAME  0xB000	 // This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL      0xAFFF	 // This stored the texture info
#define OBJECT        0x4000	 // This stores the faces, vertices, etc...

//>------ sub defines of MATERIAL
#define MATNAME       0xA000	 // This holds the material name
#define MATDIFFUSE    0xA020

								 // This holds the ambient component color
#define MAT_AMBIENT_COLOR           0xA010
								 // This holds the color of the object/material
#define MAT_DIFFUSE_COLOR           0xA020
								 // This holds the specular component color
#define MAT_SPECULAR_COLOR          0xA030
#define MAT_SHININESS               0xA040
#define MAT_SHININESS_STRENGTH      0xA041
#define MAT_TRANSPARENCY            0xA050
#define MAT_TRANSPARENCY_FALLOFF    0xA052
#define MAT_TWO_SIDED               0xA081
#define MAT_WIRE_FRAME_ON           0xA085

/*
	0xA200 : Texture map 1
	  0xA33A : Texture map 2
	  0xA210 : Opacity map
	  0xA230 : Bump map
	  0xA33C : Shininess map
	  0xA204 : Specular map
	  0xA33D : Self illum. map
	  0xA220 : Reflection map
	  0xA33E : Mask for texture map 1
	  0xA340 : Mask for texture map 2
	  0xA342 : Mask for opacity map
	  0xA344 : Mask for bump map
	  0xA346 : Mask for shininess map
	  0xA348 : Mask for specular map
	  0xA34A : Mask for self illum. map
	  0xA34C : Mask for reflection map
*/
								 // This holds the file name of the texture
#define MAT_TEXTURE_MAP1_CHUNK      0xA200
#define MAT_TEXTURE_MAP2_CHUNK      0xA33A
#define MAT_OPACITY_MAP_CHUNK       0xA210
#define MAT_BUMP_MAP_CHUNK          0xA230
#define MAT_SHININESS_MAP_CHUNK     0xA33C
#define MAT_SPECULAR_MAP_CHUNK      0xA204
#define MAT_SELF_ILLUM_MAP_CHUNK    0xA33D
#define MAT_REFLECTION_MAP_CHUNK    0xA220

#define MAP_FILENAME                0xA300
#define MAP_V_SCALE                 0xA354
#define MAP_U_SCALE                 0xA356
#define MAP_U_OFFSET                0xA358
#define MAP_V_OFFSET                0xA35A
#define MAP_ROTATION                0xA35C

/*
		0xA360 : RGB Luma/Alpha tint 1
		0xA362 : RGB Luma/Alpha tint 2
		0xA364 : RGB tint R
		0xA366 : RGB tint G
		0xA368 : RGB tint B
*/

#define OBJECT_MESH   0x4100	 // This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
								 // The objects vertices
#define OBJECT_VERTICES     0x4110
								 // The objects faces
#define OBJECT_FACES        0x4120
								 // This is found if the object has a material, either texture map or color
#define OBJECT_MATERIAL     0x4130
								 // The UV texture coordinates
#define OBJECT_UV           0x4140

// Here is our structure for our 3DS indicies(since .3DS stores 4 unsigned shorts)
struct tIndices
{
								 // This will hold point1, 2, and 3 index's into the vertex array plus a visible flag
	unsigned short a, b, c, bVisible;
};

// This holds the chunk info
struct tChunk
{
	unsigned short int ID;		 // The chunk's ID
	unsigned int length;		 // The length of the chunk
	unsigned int bytesRead;		 // The amount of bytes read within that chunk
};

// This class handles all of the loading code
class LIBMOLDEO_API CLoad3DS
{
	public:

	  CLoad3DS ();			 // This inits the data members

    // This is the function that you call to load the 3DS
    bool Import3DS (mo3dModel * pModel, char *strFileName);

	private:

    CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

    // This adds 2 vectors together and returns the result
    CVector3 AddVector(CVector3 vVector1, CVector3 vVector2);

    // This divides a vector by a single number(scalar) and returns the result
    CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler);

    // This returns the cross product between 2 vectors
    CVector3 Cross(CVector3 vVector1, CVector3 vVector2);

    // This returns the normal of a vector
    CVector3 Normalize(CVector3 vNormal);

		// This reads in a string and saves it in the char array passed in
		int GetString (char *);

		// This reads the next chunk
		void ReadChunk (tChunk *);

		// This reads the next large chunk
		void ProcessNextChunk (mo3dModel * pModel, tChunk *);

		// This reads the object chunks
		void ProcessNextObjectChunk (mo3dModel * pModel, mo3dObject * pObject,
			tChunk *);

		// This reads the material chunks
		void ProcessNextMaterialChunk (mo3dModel * pModel, tChunk *);

		// This reads the RGB value for the object's color
		void ReadColorChunk (moMaterialInfo * pMaterial, tChunk * pChunk, int color);

		// This reads the objects vertices
		void ReadVertices (mo3dObject * pObject, tChunk *);

		// This reads the objects face information
		void ReadVertexIndices (mo3dObject * pObject, tChunk *);

		// This reads the texture coodinates of the object
		void ReadUVCoordinates (mo3dObject * pObject, tChunk *);

		// This reads in the material name assigned to the object and sets the materialID
		void ReadObjectMaterial (mo3dModel * pModel, mo3dObject * pObject,
			tChunk * pPreviousChunk);

		// This computes the vertex normals for the object(used for lighting)
		void ComputeNormals (mo3dModel * pModel);

		// This frees memory and closes the file
		void CleanUp ();

		// The file pointer
		FILE * m_FilePointer;

		// These are used through the loading process to hold the chunk information
		tChunk * m_CurrentChunk;
		tChunk * m_TempChunk;
};
#endif							 /*  */

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES *
//
// This file is created in the hopes that you can just plug it into your code
// easily.  You will probably want to query more chunks though for animation, etc..
//
//
// Ben Humphrey(DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//

