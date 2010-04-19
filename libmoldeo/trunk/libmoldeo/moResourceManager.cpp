/*******************************************************************************

                              moResourceManager.cpp

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

#include "moResourceManager.h"


#include "moArray.h"
moDefineDynamicArray(moResources)

moResource::moResource() {
	SetType(MO_OBJECT_RESOURCE);
	m_ResourceType = MO_RESOURCETYPE_UNDEFINED;

}

moResource::~moResource() {

}


MOboolean moResource::Init() {

	return true;
}

MOboolean moResource::Finish() {

	return true;
}

moResourceType
moResource::GetResourceType() {

	return m_ResourceType;

}

moText
moResource::GetResourceName() {
	return m_ResourceName;
}

void
moResource::SetResourceType( moResourceType p_restype ) {

	m_ResourceType = p_restype;

}


//===========================================
//
//				moResourceManager
//
//===========================================



moResourceManager::moResourceManager() {
        m_Resources.Init( 0, NULL );

		//predefined managers
		MOFileMan = NULL;
		MOFilterMan = NULL;
		MONetMan = NULL;
		MOTimeMan = NULL;
		MODataMan = NULL;
		MOFBMan = NULL;
		MOGLMan = NULL;
		MORenderMan = NULL;
		MOShaderMan = NULL;
		MOMathMan = NULL;
		MOFontMan = NULL;
		MOGuiMan = NULL;
		MOScriptMan = NULL;
		MOTextureMan = NULL;
		MOSoundMan = NULL;
		MOVideoMan = NULL;
		MOModelMan = NULL;
		MODebugMan = NULL;
}

moResourceManager::~moResourceManager() {
}

MOboolean
moResourceManager::NewResource( moText p_resname, int paramindex, int valueindex  ) {

    moResource* pResource = moNewResource( p_resname, m_Plugins );
    if (pResource) {
        pResource->GetMobDefinition().GetMobIndex().SetParamIndex( paramindex );
        pResource->GetMobDefinition().GetMobIndex().SetValueIndex( valueindex );
    } else return false;

	return AddResource( pResource  );

}

MOboolean
moResourceManager::AddResource( moResource* m_pResource ) {

	if (m_pResource) {
		m_pResource->SetResourceManager(this);
		m_Resources.Add(m_pResource);
	}

	return (m_pResource!=NULL && m_Resources.Get(m_Resources.Count()-1)==m_pResource);
}

MOboolean
moResourceManager::RemoveResource( MOint p_ID ) {

	m_Resources.Remove(p_ID);

	return true;
}

moResources&
moResourceManager::Resources() {
	return m_Resources;
}

moResource*
moResourceManager::GetResource( MOint p_ID ) {

	return m_Resources.Get(p_ID);

}


MOint
moResourceManager::GetResourceId( moText p_resname ) {

	for(MOuint i=0;i<m_Resources.Count();i++) {
		if ( m_Resources.Get(i) != NULL ) {
			if ( m_Resources.Get(i)->GetName() == p_resname) {
				return i;
			}
		}
	}
	return -1;//not found
}

moText
moResourceManager::GetResourceName( MOint p_ID ) {

	if ( m_Resources.Get(p_ID) != NULL ) {
		return m_Resources.Get(p_ID)->GetResourceName();
	}

	return moText("");//not found

}

moResourceType
moResourceManager::GetResourceType( MOint p_ID ) {
	if ( m_Resources.Get(p_ID) != NULL ) {
		return m_Resources.Get(p_ID)->GetResourceType();
	} else return MO_RESOURCETYPE_UNDEFINED;
}

moResource*
moResourceManager::GetResourceByType( moResourceType p_type )
{
	for(MOuint i=0;i<m_Resources.Count();i++) {
		if ( m_Resources.Get(i) != NULL ) {
			if ( m_Resources.Get(i)->GetResourceType() == p_type) {
				return m_Resources.Get(i);
			}
		}
	}
	return NULL;//not found
}




MOboolean
moResourceManager::Init( const moText& p_datapath,
						moConfig&  p_consoleconfig,
						MOint p_render_to_texture_mode,
						MOint p_screen_width,
						MOint p_screen_height,
						MOint p_render_width,
						MOint p_render_height,
						MO_HANDLE p_OpWindowHandle) {

	if ( GetResourceByType( MO_RESOURCETYPE_DEBUG ) == NULL )
		AddResource( new moDebugManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_NET ) == NULL )
		AddResource( new moNetManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_FILE ) == NULL )
		AddResource( new moFileManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_FILTER ) == NULL )
		AddResource( new moFilterManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_TIME ) == NULL )
		AddResource( new moTimeManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_DATA ) == NULL )
		AddResource( new moDataManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_MATH ) == NULL )
		AddResource( new moMathManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_SHADER ) == NULL )
		AddResource( new moShaderManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_FB )==NULL )
		AddResource( new moFBManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_GL )==NULL )
		AddResource( new moGLManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_RENDER )==NULL )
		AddResource( new moRenderManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_TEXTURE )==NULL )
		AddResource( new moTextureManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_SOUND )==NULL )
		AddResource( new moSoundManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_VIDEO )==NULL )
		AddResource( new moVideoManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_MODEL )==NULL )
		AddResource( new mo3dModelManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_FONT )==NULL )
		AddResource( new moFontManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_GUI )==NULL )
		AddResource( new moGUIManager() );

	if ( GetResourceByType( MO_RESOURCETYPE_SCRIPT )==NULL )
		AddResource( new moScriptManager() );

	//Asigna configname, y labelname a los recursos en caso de encontrarse en el config
	moText resname;
	moText cfname;
	moText lblname;

	moParam& presources(p_consoleconfig.GetParam(moText("resources")));

	presources.FirstValue();

	for(MOuint r=0; r<presources.GetValuesCount(); r++) {

		moResource* presource = NULL;

		resname = presources[MO_SELECTED][MO_CFG_RESOURCE].Text();
		cfname = presources[MO_SELECTED][MO_CFG_RESOURCE_CONFIG].Text();
		lblname = presources[MO_SELECTED][MO_CFG_RESOURCE_LABEL].Text();

		MOint rid = GetResourceId( resname );

		if(rid>-1) presource = GetResource(rid);

		if (presource) {
			presource->SetConfigName(cfname);
			presource->SetLabelName(lblname);
		}
		presources.NextValue();
	}

	MODebugMan = (moDebugManager*) GetResourceByType( MO_RESOURCETYPE_DEBUG );
	if (MODebugMan) MODebugMan->Init();

	MONetMan = (moNetManager*) GetResourceByType( MO_RESOURCETYPE_NET );
	if (MONetMan) MONetMan->Init();

	MOFileMan = (moFileManager*) GetResourceByType( MO_RESOURCETYPE_FILE );
	if (MOFileMan) MOFileMan->Init();

	MODataMan = (moDataManager*) GetResourceByType( MO_RESOURCETYPE_DATA );
	if (MODataMan) MODataMan->Init( p_datapath, p_consoleconfig.GetName());

	MOFilterMan = (moFilterManager*) GetResourceByType( MO_RESOURCETYPE_FILTER );
	if (MOFilterMan) MOFilterMan->Init();

	MOTimeMan = (moTimeManager*) GetResourceByType( MO_RESOURCETYPE_TIME );
	if (MOTimeMan) MOTimeMan->Init();

	MOGLMan = (moGLManager*) GetResourceByType( MO_RESOURCETYPE_GL );
	if (MOGLMan) MOGLMan->Init();

	MOFBMan = (moFBManager*) GetResourceByType( MO_RESOURCETYPE_FB );
	if (MOFBMan) MOFBMan->Init();

	MOTextureMan = (moTextureManager*)  GetResourceByType( MO_RESOURCETYPE_TEXTURE );
	if (MOTextureMan) MOTextureMan->Init();

	MORenderMan = (moRenderManager*) GetResourceByType( MO_RESOURCETYPE_RENDER );
	if (MORenderMan) MORenderMan->Init( p_render_to_texture_mode, p_screen_width, p_screen_height, p_render_width, p_render_height );

	MOMathMan = (moMathManager*) GetResourceByType( MO_RESOURCETYPE_MATH );
	if (MOMathMan) MOMathMan->Init();

	MOShaderMan = (moShaderManager*) GetResourceByType( MO_RESOURCETYPE_SHADER );
	if (MOShaderMan) MOShaderMan->Init();

	MOFontMan = (moFontManager*) GetResourceByType( MO_RESOURCETYPE_FONT );
	if (MOFontMan) MOFontMan->Init();

	MOGuiMan = (moGUIManager*)  GetResourceByType( MO_RESOURCETYPE_GUI );
	if (MOGuiMan) MOGuiMan->Init( p_OpWindowHandle );

	MOSoundMan = (moSoundManager*)  GetResourceByType( MO_RESOURCETYPE_SOUND );
	if (MOSoundMan) MOSoundMan->Init();

	MOVideoMan = (moVideoManager*)  GetResourceByType( MO_RESOURCETYPE_VIDEO );
	if (MOVideoMan) MOVideoMan->Init();

	MOModelMan = (mo3dModelManager*)  GetResourceByType( MO_RESOURCETYPE_MODEL );
	if (MOModelMan) MOModelMan->Init();

	MOScriptMan = (moScriptManager*)  GetResourceByType( MO_RESOURCETYPE_SCRIPT );
	if (MOScriptMan) MOScriptMan->Init();

	m_Plugins.Init( 0, NULL);

	m_bInitialized = true;

	return Initialized();
}

MOboolean
moResourceManager::Finish() {

	for(MOuint i=0; i<m_Resources.Count(); i++) {
		if (m_Resources[i]) {
			m_Resources[i]->Finish();
		}
	}

	for(MOuint i=0; i<m_Resources.Count(); i++) {
		if (m_Resources[i]) {
			delete m_Resources[i];
		}
	}

	MOFileMan = NULL;
	MOFilterMan = NULL;
	MONetMan = NULL;
	MOTimeMan = NULL;
	MODataMan = NULL;
	MOFBMan = NULL;
	MOGLMan = NULL;
	MORenderMan = NULL;
	MOShaderMan = NULL;
	MOMathMan = NULL;
	MOFontMan = NULL;
	MOGuiMan = NULL;
	MOScriptMan = NULL;
	MOTextureMan = NULL;
	MOSoundMan = NULL;
	MOVideoMan = NULL;
	MOModelMan = NULL;
	MODebugMan = NULL;

	m_Resources.Empty();

	m_bInitialized = false;

	return true;
}
