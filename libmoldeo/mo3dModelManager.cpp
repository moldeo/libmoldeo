/*******************************************************************************

                              mo3dModelManager.cpp

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
#include <string>
#include <mo3dModelManager.h>
#include <mo3ds.h>
#include <moDataManager.h>
#include <moTextureManager.h>


#include "moArray.h"
moDefineDynamicArray( moCoords )
moDefineDynamicArray( moSceneNodeArray )
moDefineDynamicArray( moSceneNodePointerArray )

MOulong moSceneNode::UID = 0;

MOulong moSceneNode::CreateUID() {
  moSceneNode::UID+= 1;
  return moSceneNode::UID;
}

moSceneNode::moSceneNode() {
  m_ProjectionMatrix.MakeIdentity();
  m_ModelMatrix.MakeIdentity();
}

moSceneNode::moSceneNode( const moText& name ) {
  m_Id = moSceneNode::CreateUID();
  m_Name = name;
}

moSceneNode::moSceneNode( const moSceneNode& p_src ) {
  (*this) = p_src;
}

moSceneNode&
moSceneNode::operator=(const moSceneNode& p_src) {
  m_ProjectionMatrix = p_src.m_ProjectionMatrix;
  m_ModelMatrix = p_src.m_ModelMatrix;
  m_Parent = p_src.m_Parent;
  m_ProjectionMatrix = p_src.m_ProjectionMatrix;
  m_Childrens = p_src.m_Childrens;
  return (*this);
}

moSceneNode::~moSceneNode() {

}

MOboolean moSceneNode::Init() {

  return true;

}

MOboolean moSceneNode::Finish() {
  return true;
}

moSceneNode*
moSceneNode::GetObjectByName( const moText& p_name ) {
  moSceneNode* nFind = NULL;
  for( MOuint i=0; i<m_Childrens.Count(); i++ ) {
      nFind = m_Childrens[i];

      if (nFind && nFind->GetName()==p_name) {
        return nFind;
      }

      if (nFind) nFind = nFind->GetObjectByName( p_name );
      if (nFind) return nFind;
  }
  return nFind;
}

moSceneNode*
moSceneNode::GetObjectById( MOulong p_id ) {
  moSceneNode* nFind = NULL;
  for( MOuint i=0; i<m_Childrens.Count(); i++ ) {
      nFind = m_Childrens[i];

      if (nFind && nFind->GetId()==p_id) {
        return nFind;
      }

      if (nFind) nFind = nFind->GetObjectById( p_id );
      if (nFind) return nFind;
  }
  return nFind;
}

moSceneNodePointer
moSceneNode::GetChild( int p_index_child ) {
  return m_Childrens.Get(p_index_child);
}

int moSceneNode::AddChild( moSceneNodePointer p_child_node ) {
    m_Childrens.Add( p_child_node );
    return 0;
}

int moSceneNode::RemoveChild( int p_node_index ) {
  m_Childrens.Remove( p_node_index );
  return 1;
}

int moSceneNode::RemoveChild( moSceneNodePointer p_child_node ) {

    if (p_child_node==NULL) return -1;
    //search for node
    for( int i=0; i<(int)m_Childrens.Count(); i++ ) {
      if (p_child_node==m_Childrens[i]) {
        m_Childrens.Remove(i);
        return 1;
      }
    }
    return 0;
}


void moSceneNode::Draw( moEffectState *state, GLuint g_ViewMode ) {

  // draw scene node
  moText json;

  if (state)
      json = state->ToJSON();

  MODebug2->Message( "moSceneNode::Draw > implement this! g_ViewMode: (" + IntToStr(g_ViewMode) + ") "
                    + json );
}

void moSceneNode::Update() {

}

void moSceneNode::Interaction() {

}






mo3DSModelSceneNode::mo3DSModelSceneNode() {
    m_pModel = NULL;
}


mo3DSModelSceneNode::~mo3DSModelSceneNode() {

}

MOboolean
mo3DSModelSceneNode::Init() {
    m_pModel = NULL;
    return true;
}

MOboolean mo3DSModelSceneNode::Finish() {
    return true;
}


void mo3DSModelSceneNode::Draw(moEffectState *state, GLuint g_ViewMode) {

	int i;
	float x,y;
	int TID;
	mo3DSMaterialInfo MAT;


    if (m_pModel)
	for(i = 0; i < m_pModel->numOfObjects; i++)
	{
		// Make sure we have valid objects just in case.(size() is in the vector class)
		if(m_pModel->pObject.size() <= 0) break;

		// Get the current object that we are displaying
		mo3DSObject *pObject = &m_pModel->pObject[i];

		// Check to see if this object has a texture map, if so bind the texture to it.
		//if pObject->materialID
		//if(pObject->pMaterials[pObject->materialID].texureId

		if(pObject->bHasTexture) {

			// Turn on texture mapping and turn off color
			glEnable(GL_TEXTURE_2D);

			// Reset the color to normal again
			glColor4f(state->tintr, state->tintg, state->tintb,state->alpha);

			// Bind the texture map to the object by it's materialID
			if(m_pModel->pMaterials.size() && pObject->materialID >= 0) {
				TID = -1;
				MAT = m_pModel->pMaterials[pObject->materialID];
				if ( MAT.texMaps.size())  {
					TID = MAT.texMaps[0].textureId;
				}
				if ( TID!=65535 && TID>0) {
					glBindTexture(GL_TEXTURE_2D, TID );
				} else {
					glBindTexture(GL_TEXTURE_2D, 1 );
				}
			}
		} else {

			// Turn off texture mapping and turn on color
			glDisable(GL_TEXTURE_2D);

			// Reset the color to normal again
			glColor4f( state->tintr, state->tintg, state->tintb, state->alpha );
		}

		// This determines if we are in wireframe or normal mode
#ifndef OPENGLESV2
		glBegin(g_ViewMode);					// Begin drawing with our selected mode(triangles or lines)

			// Go through all of the faces(polygons) of the object and draw them
			for(int j = 0; j < pObject->numOfFaces; j++)
			{
				// Go through each corner of the triangle and draw it.
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// Get the index for each point of the face
					int index = pObject->pFaces[j].vertIndex[whichVertex];

					// Give OpenGL the normal for this vertex.
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);

					// If the object has a texture associated with it, give it a texture coordinate.
					if(pObject->bHasTexture) {

						// Make sure there was a UVW map applied to the object or else it won't have tex coords.
						if(pObject->pTexVerts) {
							//BYTE *pColor = m_pModel->pMaterials[pObject->materialID].color;

							// Assign the current color to this model
							//glColor3ub(pColor[0]*state.tintr, pColor[1]*state.tintg, pColor[2]*state.tintb);
							//glColor4ub( pColor[0]*state->tintr, pColor[1]*state->tintg, pColor[2]*state->tintb,255*state->alpha );

							MOubyte *pColor = m_pModel->pMaterials[pObject->materialID].colorDiffuse;

							// Assign the current color to this model
							if (pColor!=NULL) {
								float rf = (float)pColor[0] * state->tintr;
								float rg = (float)pColor[1] * state->tintg;
								float rb = (float)pColor[2] * state->tintb;
								float af = 255.0 * state->alpha;
								glColor4f( rf, rg, rb, af);
							} else glColor4f(state->tintr, state->tintg, state->tintb,state->alpha);

							if ( MAT.texMaps.size()) {
								x = pObject->pTexVerts[ index ].x * MAT.texMaps[0].vTile + MAT.texMaps[0].vOffset;
								y = pObject->pTexVerts[ index ].y * MAT.texMaps[0].uTile + MAT.texMaps[0].uOffset;
							} else {
								x = pObject->pTexVerts[ index ].x;
								y = pObject->pTexVerts[ index ].y;
							}

							glTexCoord2f( x , y );
						}
					} else {

						// Make sure there is a valid material/color assigned to this object.
						// You should always at least assign a material color to an object,
						// but just in case we want to check the size of the material list.
						// if the size is at least one, and the material ID != -1,
						// then we have a valid material.
						if(m_pModel->pMaterials.size() && pObject->materialID >= 0)
						{
							// Get and set the color that the object is, since it must not have a texture
							MOubyte *pColor = m_pModel->pMaterials[pObject->materialID].colorDiffuse;

							// Assign the current color to this model
							if ( pColor!=NULL) {
								float rf = (float)pColor[0] * state->tintr;
								float rg = (float)pColor[1] * state->tintg;
								float rb = (float)pColor[2] * state->tintb;
								float af = 255.0 * state->alpha;
								glColor4f( rf, rg, rb, af);
							} else glColor4f(state->tintr, state->tintg, state->tintb,state->alpha);
						}
					}

					// Pass in the current vertex of the object(Corner of current face)
					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				}
			}

		glEnd();
#endif								// End the drawing
	}


}

void mo3DSModelSceneNode::Update() {

}

void mo3DSModelSceneNode::Interaction() {

}

//===========================================
//
//				mo3dModelManager
//
//===========================================



mo3dModelManager::mo3dModelManager() {
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_MODEL );
	SetName("3dmodelmanager");
	SetLabelName("3dmodelmanager");
	mTM = NULL;
	m_pMoldeoLogo = NULL;
/*
	nMaxModels = MO_MAX_MODELOS;
	Models = new mo3dModel* [nMaxModels];
	for(MOuint i=0;i<nMaxModels;i++)  Models[i] = NULL;
	nModels = 0;
	Textures = NULL;
*/
}

mo3dModelManager::~mo3dModelManager() {
	Finish();
}

MOboolean
mo3dModelManager::Init() {
  m_pMoldeoLogo = NULL;
	if ( m_pResourceManager ) {
		mTM = m_pResourceManager->GetTextureMan();
	}

	m_pMoldeoLogo = Load3dModel("moldeologo.3ds" );
	if (m_pMoldeoLogo) {
    MODebug2->Message("mo3dModelManager::Init > loaded Moldeo 3ds Model!!! ");
	}
	/*
	if (!m_pMoldeoLogo) {
		mo3dModel* pmodel = Load3dModel("moldeologo.3ds", m_pResourceManager->GetDataMan()->GetAppDataPath() );
		if (pmodel) {
			m_pMoldeoLogo = new mo3DModelSceneNode();
			m_pMoldeoLogo->Init(pmodel);
		}
	}
	*/
	return true;
}


//Devuelve el puntero generado
//en caso de no encontrar el archivo, devuelve MOTEXTURAS_ERROR

moSceneNode*
mo3dModelManager::Load3dModel( const moText& p_file_name ) {

//  MOint i,j;
  CLoad3DS g_Load3ds; // Clase importacion de escenas 3DS.

  moText strTexture;
  moText filename;
//  MOuint texid=MO_UNDEFINED;

  moFile File3D = m_pResourceManager->GetDataMan()->GetDataFile( p_file_name );

  if (!File3D.Exists()) {
    /// check also in App Data Path!
    File3D = m_pResourceManager->GetDataMan()->GetAppDataFile( p_file_name );
    if (!File3D.Exists()) {
      MODebug2->Error( "mo3dModelManager::Load3dModel > file not found! " + File3D.GetFullName() );
    }
  }

/*
	if(Models[nModels]!=NULL) {
		Models[nModels]->numOfMaterials = 0;
		Models[nModels]->numOfObjects = 0;

		if(g_Load3ds.Import3DS( Models[nModels], file3d) == true) {

			for(i = 0; i < Models[nModels]->numOfMaterials; i++)
			{
				//check all maps of the material
				for(j=0;j<(MOint)Models[nModels]->pMaterials[i].texMaps.size();j++) {
						// Check to see if there is a file name to load in this material
					if(strlen(Models[nModels]->pMaterials[i].texMaps[j].strFile) > 0)
					{
						// Use the name of the texture file to load the bitmap, with a texture ID(i).
						// We pass in our global texture array, the name of the texture, and an ID to reference it.
						//CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);
						strTexture = Models[nModels]->pMaterials[i].texMaps[j].strFile;
						strTexture = moText("objetos/materials/")+(moText)strTexture;
						texid = Textures->GetTextureMOId(strTexture, true);
					}

					// Set the texture ID for this material
					//if(texid!=MOTEXTURAS_ERROR) {
						Models[nModels]->pMaterials[i].texMaps[j].textureId = texid;
					//}
				}
			}
			Models[nModels]->name = Tname;
		} else {
			delete Models[nModels];
			Models[nModels] = NULL;
			return NULL;
		}
	} else return(NULL);

	nModels++;
	return Models[nModels-1];
	*/
	return NULL;
}


MOboolean
mo3dModelManager::Finish() {
  /*
	MOuint i;
	if(nModels>0) {
		for(i=0;i<nModels;i++) {
			if(Models[i]!=NULL) {
				for(int j = 0; j < Models[i]->numOfObjects; j++)
				{
					// Free the faces, normals, vertices, and texture coordinates.
					delete [] Models[i]->pObject[j].pFaces;
					delete [] Models[i]->pObject[j].pNormals;
					delete [] Models[i]->pObject[j].pVerts;
					delete [] Models[i]->pObject[j].pTexVerts;
				}

				delete Models[i];
				Models[i] = NULL;
			}
		}
	}
	if(Models!=NULL) delete [] Models;
	Models = NULL;
	nModels = 0;

	if (m_pMoldeoLogo) {
        delete m_pMoldeoLogo;
        m_pMoldeoLogo = NULL;
	}
*/
	return true;
}

moSceneNode*
mo3dModelManager::Get3dModel( const moText& p_object_name, bool force_load ) {

	//buscamos a ver si no lo cargamos ya
	//si no se encontro el modelo se lo carga
  moSceneNode* pFind = m_MotherNode.GetObjectByName( p_object_name );
  if (!pFind && force_load) {
    pFind = Load3dModel( p_object_name );
  }
	return( pFind );
}

void mo3dModelManager::MoldeoLogo(long ticks) {

    //animate

    cout << "mo3dModelManager::MoldeoLogo > ticks: " << ticks << endl;

/*
        moEffectState pstate;
        pstate.tintr = 1.0;
        pstate.tintg = 1.0;
        pstate.tintb = 1.0;
        pstate.tint = 1.0;
        pstate.alpha = 1.0;

		glClearColor( 0.0f, 0.0f, 0.3f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
#ifndef OPENGLESV2
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		// Transformations
		//GLfloat m[4][4];
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glRotatef(33, 0, 0 , 1 );
		glTranslatef( 0.0f, 0.0f, -4.0f );
		glRotatef( (ticks/100.0)*8, 0.0f, 1.0f, 0.0f );
		glScalef(3,3,3);
        //glTranslatef( 0.6f, 0.0f, 0.0f );

        if (m_pMoldeoLogo)
            m_pMoldeoLogo->Draw( &pstate, GL_TRIANGLES);
#endif
*/
}


//===========================================
//
//			mo3dModelManagerRef
//
//========================================


/*
void
mo3dModelManagerRef::LoadModels(moConfig *cfg,MOuint param,mo3dModelManager*M) {

	MOuint i;
	moText namemodelo;

	cfg->SetCurrentParamIndex(param);
	cfg->FirstValue();
	for(i=0;i<nModels;i++) {
		namemodelo =	cfg->GetParam().GetValue().GetSubValue(0).Text();
		Models[i] = M->Get3dModel(namemodelo);
		if(Models[i] != MO_3DMODEL_ERROR) {
			MODebug2->Message(moText("Modelo cargado: ") + (moText)Models[i]->name);
		} else {
			MODebug2->Error(moText("mo3dModelManagerRef::LoadModels > Modelo  no cargado: "));
		}
		cfg->NextValue();
	}
}

int mo3dModelManagerRef::Add(moText namemodelo,mo3dModelManager *M) {

	MOint i;

    mo3dModel* newModel = NULL;

	//y alli cargamos la textura
	newModel = M->Get3dModel(namemodelo);

	if(newModel != MO_3DMODEL_ERROR) {

        mo3dModel **ModelsAux = NULL;

        ModelsAux = new mo3dModel* [nModels+1];

        if(Models!=NULL) {
            //copio el contenido del buffer
            for(i=0;i<(int)nModels;i++) {
                ModelsAux[i] = Models[i];
            }
            //borro el buffer viejo
            delete[] Models;
            Models = NULL;
        }

        //aumentamos el array de uno
        nModels++;
        //recreamos el buffer con un elemento mas
        Models = new mo3dModel* [nModels];
        //lo completamos otra vez con los datos guardados en memoria
        if(Models!=NULL) {
            for(i=0;i<(int)nModels-1;i++) {
                Models[i] = ModelsAux[i];
            }
        }
        //nos paramos en el last
        i = nModels - 1 ;

        Models[i] = newModel;
        MODebug2->Message(moText("Modelo cargado: ") + (moText)Models[i]->name);

        delete [] ModelsAux;
        return i;
	} else {
		MODebug2->Error(moText("mo3dModelManagerRef::Add > Modelo  no cargado: "));
	}

	return MO_UNDEFINED;
}

mo3dModelManagerRef::mo3dModelManagerRef() {
	nModels = 0;
	Models = NULL;
}

mo3dModelManagerRef::~mo3dModelManagerRef() {
	Finish();
}


MOboolean
mo3dModelManagerRef::Init(moConfig * cfg, MOuint param, mo3dModelManager *M) {
        moText text;

	nModels = cfg->GetValuesCount(param);
        text = moText("nModels: ");
        text +=  IntToStr(nModels);
	MODebug2->Push(text);
	Models = new mo3dModel* [nModels];
	for(MOuint i=0;i<nModels;i++) Models[i] = NULL;

	MODebug2->Push(moText("Espacio asignado: cargando modelos... 8o|"));
	LoadModels(cfg,param,M);
	MODebug2->Push(moText("Models cargados 8oD"));
	return true;
}

MOboolean
mo3dModelManagerRef::Finish() {
	if(Models!=NULL) delete[] Models;
	Models = NULL;
	nModels = 0;
	return true;
}

mo3dModel*
mo3dModelManagerRef::Get(int i) {

	if((i>=0) &&(i<(MOint)nModels)) {
		return(Models[i]);
	} else {
                moText text;
                text = moText("Error(ajModelsRef): el indice: ");
                text += IntToStr(i);
                text += moText(" no existe.");
		MODebug2->Error(text);
		return(0);
	}
}



void mo3dModelManagerRef::Draw(int imodel, moEffectState *state, GLuint g_ViewMode) {

	// Since we know how many objects our model has, go through each of them.
	int i;
	float x,y;
	int TID;
	moMaterialInfo MAT;

    if ( imodel < (int)this->nModels )
	if (Models[imodel]!=NULL)
	for(i = 0; i < Models[imodel]->numOfObjects; i++)
	{
		// Make sure we have valid objects just in case.(size() is in the vector class)
		if(Models[imodel]->pObject.size() <= 0) break;

		// Get the current object that we are displaying
		mo3dObject *pObject = &Models[imodel]->pObject[i];

		// Check to see if this object has a texture map, if so bind the texture to it.
		//if pObject->materialID
		//if(pObject->pMaterials[pObject->materialID].texureId

		if(pObject->bHasTexture) {

			// Turn on texture mapping and turn off color
			glEnable(GL_TEXTURE_2D);

			// Reset the color to normal again
			glColor4f(state->tintr, state->tintg, state->tintb,state->alpha);

			// Bind the texture map to the object by it's materialID
			if(Models[imodel]->pMaterials.size() && pObject->materialID >= 0) {
				TID = -1;
				MAT = Models[imodel]->pMaterials[pObject->materialID];
				if ( MAT.texMaps.size())  {
					TID = MAT.texMaps[0].textureId;
				}
				if ( TID!=65535 && TID>0) {
					glBindTexture(GL_TEXTURE_2D, TID );
				} else {
					glBindTexture(GL_TEXTURE_2D, 1 );
				}
			}
		} else {

			// Turn off texture mapping and turn on color
			glDisable(GL_TEXTURE_2D);

			// Reset the color to normal again
			glColor4f( state->tintr, state->tintg, state->tintb, state->alpha );
		}

#ifndef OPENGLESV2
		// This determines if we are in wireframe or normal mode
		glBegin(g_ViewMode);					// Begin drawing with our selected mode(triangles or lines)

			// Go through all of the faces(polygons) of the object and draw them
			for(int j = 0; j < pObject->numOfFaces; j++)
			{
				// Go through each corner of the triangle and draw it.
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// Get the index for each point of the face
					int index = pObject->pFaces[j].vertIndex[whichVertex];

					// Give OpenGL the normal for this vertex.
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);

					// If the object has a texture associated with it, give it a texture coordinate.
					if(pObject->bHasTexture) {

						// Make sure there was a UVW map applied to the object or else it won't have tex coords.
						if(pObject->pTexVerts) {
							//BYTE *pColor = Models[imodel]->pMaterials[pObject->materialID].color;

							// Assign the current color to this model
							//glColor3ub(pColor[0]*state.tintr, pColor[1]*state.tintg, pColor[2]*state.tintb);
							//glColor4ub( pColor[0]*state->tintr, pColor[1]*state->tintg, pColor[2]*state->tintb,255*state->alpha );

							MOubyte *pColor = Models[imodel]->pMaterials[pObject->materialID].colorDiffuse;

							// Assign the current color to this model
							if (pColor!=NULL) {
								float rf = (float)pColor[0] * state->tintr;
								float rg = (float)pColor[1] * state->tintg;
								float rb = (float)pColor[2] * state->tintb;
								float af = 255.0 * state->alpha;
								glColor4f( rf, rg, rb, af);
							} else glColor4f(state->tintr, state->tintg, state->tintb,state->alpha);

							if ( MAT.texMaps.size()) {
								x = pObject->pTexVerts[ index ].x * MAT.texMaps[0].vTile + MAT.texMaps[0].vOffset;
								y = pObject->pTexVerts[ index ].y * MAT.texMaps[0].uTile + MAT.texMaps[0].uOffset;
							} else {
								x = pObject->pTexVerts[ index ].x;
								y = pObject->pTexVerts[ index ].y;
							}

							glTexCoord2f( x , y );
						}
					} else {

						// Make sure there is a valid material/color assigned to this object.
						// You should always at least assign a material color to an object,
						// but just in case we want to check the size of the material list.
						// if the size is at least one, and the material ID != -1,
						// then we have a valid material.
						if(Models[imodel]->pMaterials.size() && pObject->materialID >= 0)
						{
							// Get and set the color that the object is, since it must not have a texture
							MOubyte *pColor = Models[imodel]->pMaterials[pObject->materialID].colorDiffuse;

							// Assign the current color to this model
							if ( pColor!=NULL) {
								float rf = (float)pColor[0] * state->tintr;
								float rg = (float)pColor[1] * state->tintg;
								float rb = (float)pColor[2] * state->tintb;
								float af = 255.0 * state->alpha;
								glColor4f( rf, rg, rb, af);
							} else glColor4f(state->tintr, state->tintg, state->tintb,state->alpha);
						}
					}

					// Pass in the current vertex of the object(Corner of current face)
					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				}
			}

		glEnd();								// End the drawing
#endif
	}


}
*/
