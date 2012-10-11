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


*******************************************************************************/

#include "moResourceManager.h"
#include <moDebugManager.h>
#include <moNetManager.h>
#include <moFileManager.h>
#include <moFilterManager.h>
#include <moTimeManager.h>
#include <moDataManager.h>
#include <moMathManager.h>
#include <moShaderManager.h>
#include <moFBManager.h>
#include <moGLManager.h>
#include <moDecoderManager.h>
#include <moSoundManager.h>
#include <mo3dModelManager.h>
#include <moFontManager.h>
#include <moGUIManager.h>
#include <moScriptManager.h>

#include "moArray.h"
moDefineDynamicArray(moResources)

moResource::moResource() {
	SetType(MO_OBJECT_RESOURCE);
	m_ResourceType = MO_RESOURCETYPE_UNDEFINED;

}

moResource::~moResource() {

}


MOboolean moResource::Init() {

  if (!(GetConfigName()==moText(""))) {
    if (moMoldeoObject::Init()) {
      if (moMoldeoObject::CreateConnectors()) {
        return true;
      } else {
        MODebug2->Error("Errors creating connectors or loading param values for " + GetConfigName() + " configuration file");
        return false;
      }
    }
  }

  MODebug2->Message("No " + GetName() + " configuration file defined. Continue.");

  return true;

}

MOboolean moResource::Finish() {

	return true;
}

moResourceType
moResource::GetResourceType() {

	return m_ResourceType;

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

moResource*
moResourceManager::NewResource( moText p_resname,  moText p_configname, moText p_labelname, int paramindex, int valueindex  ) {

    moResource* pResource = moNewResource( p_resname, m_Plugins );
    if (pResource) {
        pResource->SetConfigName(p_configname);
        pResource->SetLabelName(p_labelname);
        pResource->SetConsoleParamIndex( paramindex );
        pResource->SetConsoleValueIndex( valueindex );
        AddResource( pResource  );
    } else {
        MODebug2->Error("moResourceManager::NewResource Error creating resource "+p_resname);
    }

    return pResource;

}

MOboolean
moResourceManager::AddResource( moResource* m_pResource ) {

	if (m_pResource) {
		m_pResource->SetResourceManager(this);
		m_Resources.Add(m_pResource);
	}

	return (m_pResource!=NULL && m_Resources.GetRef(m_Resources.Count()-1)==m_pResource);
}

MOboolean
moResourceManager::RemoveResource( MOint p_ID ) {

    moResource* pResource;
    pResource = m_Resources.GetRef(p_ID);
    ///Tratamos de borrarlo de los recursos de plugins
    if (pResource) {
        if (moDeleteResource( pResource, m_Plugins )) {
                ///si no es parte de un plugin lo eliminamos a mano
                ///no hacemos nada! esto deberia hacer el Finish()
                ///ya que este se encargo de crearlos
                m_Resources.Remove(p_ID);
                return true;
        }
    }
    return false;
}

moResources&
moResourceManager::Resources() {
	return m_Resources;
}

moResource*
moResourceManager::GetResource( MOint p_index ) {

	return m_Resources.GetRef(p_index);

}


MOint
moResourceManager::GetResourceIndex( moText p_labelname ) {

	for(MOuint i=0;i<m_Resources.Count();i++) {
		if ( m_Resources.GetRef(i) != NULL ) {
			if ( m_Resources.GetRef(i)->GetLabelName() == p_labelname) {
				return i;
			}
		}
	}
	return -1;//not found
}

moText
moResourceManager::GetResourceName( MOint p_index ) {

	if ( m_Resources.Get(p_index) != NULL ) {
		return m_Resources.GetRef(p_index)->GetName();
	}

	return moText("");//not found

}

moText
moResourceManager::GetResourceLabelName( MOint p_index ) {

	if ( m_Resources.Get(p_index) != NULL ) {
		return m_Resources.GetRef(p_index)->GetLabelName();
	}

	return moText("");//not found

}


moResourceType
moResourceManager::GetResourceType( MOint p_index ) {
	if ( m_Resources.GetRef(p_index) ) {
		return m_Resources.GetRef(p_index)->GetResourceType();
	} else return MO_RESOURCETYPE_UNDEFINED;
}

moResource*
moResourceManager::GetResourceByType( moResourceType p_type )
{
	for(MOuint i=0;i<m_Resources.Count();i++) {
		if ( m_Resources.GetRef(i) ) {
			if ( m_Resources.GetRef(i)->GetResourceType() == p_type) {
				return m_Resources.GetRef(i);
			}
		}
	}
	return NULL;//not found
}




MOboolean
moResourceManager::Init(
                        const moText& p_apppath,
                        const moText& p_datapath,
						moConfig&  p_consoleconfig,
						MOint p_render_to_texture_mode,
						MOint p_screen_width,
						MOint p_screen_height,
						MOint p_render_width,
						MOint p_render_height,
						MO_HANDLE p_OpWindowHandle,
						MO_DISPLAY p_Display) {

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

	if ( GetResourceByType( MO_RESOURCETYPE_DECODER )==NULL )
		AddResource( new moDecoderManager() );


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

	///Asigna configname, y labelname a los recursos PREDETERMINADOS en caso de encontrarse en el config
	moText resname;
	moText cfname;
	moText lblname;

  ///TODO: chequear errores...
	moParam& presources(p_consoleconfig.GetParam(moText("resources")));

	presources.FirstValue();

	for(MOuint r=0; r<presources.GetValuesCount(); r++) {

		moResource* pResource = NULL;

		resname = presources[MO_SELECTED][MO_CFG_RESOURCE].Text();
		cfname = presources[MO_SELECTED][MO_CFG_RESOURCE_CONFIG].Text();
		lblname = presources[MO_SELECTED][MO_CFG_RESOURCE_LABEL].Text();

		MOint rid = GetResourceIndex( lblname );

		if(rid>-1) pResource = GetResource(rid);

		if (pResource) {
			pResource->SetConfigName(cfname);
			pResource->SetLabelName(lblname);
            pResource->SetConsoleParamIndex( presources.GetParamDefinition().GetIndex() );
            pResource->SetConsoleValueIndex( r );
		}
		presources.NextValue();
	}

    if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Embedded Resources."));
	MODebugMan = (moDebugManager*) GetResourceByType( MO_RESOURCETYPE_DEBUG );
	if (MODebugMan) {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Debug Man Resource."));
	    if (!MODebugMan->Init())
            MODebug2->Error(moText("moResourceManager:: Debug Man. Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Debug Man. Creation Error."));
    }

	MONetMan = (moNetManager*) GetResourceByType( MO_RESOURCETYPE_NET );
	if (MONetMan) {
	    if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Net Man Resource."));
	    if (!MONetMan->Init())
	        MODebug2->Error(moText("moResourceManager:: Net Man. Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Net Man. Creation Error."));
    }

	MOFileMan = (moFileManager*) GetResourceByType( MO_RESOURCETYPE_FILE );
	if (MOFileMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing File Man Resource."));
	    if (!MOFileMan->Init())
            MODebug2->Error(moText("moResourceManager:: File Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: File Man Creation Error."));
    }


	MODataMan = (moDataManager*) GetResourceByType( MO_RESOURCETYPE_DATA );
	if (MODataMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Data Man Resource."));
	    if (!MODataMan->Init( p_apppath, p_datapath, p_consoleconfig.GetName()))
            MODebug2->Error(moText("moResourceManager:: Data Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Data Man Creation Error."));
    }

	MOFilterMan = (moFilterManager*) GetResourceByType( MO_RESOURCETYPE_FILTER );
	if (MOFilterMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Filter Man Resource."));
	    if (!MOFilterMan->Init())
            MODebug2->Error(moText("moResourceManager:: Filter Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Filter Man Creation Error."));
    }

	MOTimeMan = (moTimeManager*) GetResourceByType( MO_RESOURCETYPE_TIME );
	if (MOTimeMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Time Man Resource."));
	    if (!MOTimeMan->Init())
            MODebug2->Error(moText("moResourceManager:: Time Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Time Man Creation Error."));
    }

	MOGLMan = (moGLManager*) GetResourceByType( MO_RESOURCETYPE_GL );
	if (MOGLMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing GL Man Resource."));
	    if (!MOGLMan->Init())
            MODebug2->Error(moText("moResourceManager:: GL Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: GL Man Creation Error."));
    }

	MOFBMan = (moFBManager*) GetResourceByType( MO_RESOURCETYPE_FB );
	if (MOFBMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing FrameBuffer Man Resource."));
	    if (!MOFBMan->Init())
            MODebug2->Error(moText("moResourceManager:: FrameBuffer Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: FrameBuffer Man Creation Error."));
    }

	MOTextureMan = (moTextureManager*)  GetResourceByType( MO_RESOURCETYPE_TEXTURE );
	if (MOTextureMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Texture Man Resource."));
	    if (!MOTextureMan->Init())
            MODebug2->Error(moText("moResourceManager:: Texture Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Texture Man Creation Error."));
    }

	MODecoderMan = (moDecoderManager*)  GetResourceByType( MO_RESOURCETYPE_DECODER );
	if (MODecoderMan)  {
        if (MODebug2) MODebug2->Message(moText("moDecoderMan:: Initializing Decoder Manager Resource."));
	    if (!MODecoderMan->Init())
            MODebug2->Error(moText("moDecoderMan:: Decoder Manager Initialization Error."));
	} else {
	    MODebug2->Error(moText("moDecoderMan:: Decoder Manager Creation Error."));
  }


	MORenderMan = (moRenderManager*) GetResourceByType( MO_RESOURCETYPE_RENDER );
	if (MORenderMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Render Man Resource."));
	    if (!MORenderMan->Init( (moRenderManagerMode)p_render_to_texture_mode, p_screen_width, p_screen_height, p_render_width, p_render_height ))
            MODebug2->Error(moText("moResourceManager:: Render Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Render Man Creation Error."));
    }

	MOMathMan = (moMathManager*) GetResourceByType( MO_RESOURCETYPE_MATH );
	if (MOMathMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Math Man Resource."));
	    if (!MOMathMan->Init())
            MODebug2->Error(moText("moResourceManager:: Math Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Math Man Creation Error."));
    }

	MOShaderMan = (moShaderManager*) GetResourceByType( MO_RESOURCETYPE_SHADER );
	if (MOShaderMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Shsder Man Resource."));
	    if (!MOShaderMan->Init())
            MODebug2->Error(moText("moResourceManager:: Shader Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Shader Man Creation Error."));
    }

	MOFontMan = (moFontManager*) GetResourceByType( MO_RESOURCETYPE_FONT );
	if (MOFontMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Font Man Resource."));
	    if (!MOFontMan->Init())
            MODebug2->Error(moText("moResourceManager:: Font Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Font Man Creation Error."));
    }

	MOGuiMan = (moGUIManager*)  GetResourceByType( MO_RESOURCETYPE_GUI );
	if (MOGuiMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing GUI Man Resource."));
	    if (!MOGuiMan->Init( p_OpWindowHandle, p_Display ))
            MODebug2->Error(moText("moResourceManager:: GUI Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: GUI Man Creation Error."));
    }

	MOSoundMan = (moSoundManager*)  GetResourceByType( MO_RESOURCETYPE_SOUND );
	if (MOSoundMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Sound Man Resource."));
	    if (!MOSoundMan->Init())
            MODebug2->Error(moText("moResourceManager:: Sound Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Sound Man Creation Error."));
    }

	MOVideoMan = (moVideoManager*)  GetResourceByType( MO_RESOURCETYPE_VIDEO );
	if (MOVideoMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Video Man Resource."));
	    if (!MOVideoMan->Init()) {
            MODebug2->Error(moText("moResourceManager:: Video Man Initialization Error."));
	    }
	} else {
	    MODebug2->Error(moText("moResourceManager:: Video Man Creation Error."));
    }

	MOModelMan = (mo3dModelManager*)  GetResourceByType( MO_RESOURCETYPE_MODEL );
	if (MOModelMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Model Man Resource."));
	    if (!MOModelMan->Init())
            MODebug2->Error(moText("moResourceManager:: Model Man Initialization Error."));
	} else {
	    MODebug2->Error(moText("moResourceManager:: Model Man Creation Error."));
    }

	MOScriptMan = (moScriptManager*)  GetResourceByType( MO_RESOURCETYPE_SCRIPT );
	if (MOScriptMan)  {
        if (MODebug2) MODebug2->Message(moText("moResourceManager:: Initializing Script Man Resource."));
	    if (!MOScriptMan->Init()) {
            MODebug2->Error(moText("moResourceManager:: Script Man Initialization Error."));
	    }
	} else {
	    MODebug2->Error(moText("moResourceManager:: Script Man Creation Error."));
    }

	m_Plugins.Init( 0, NULL);

	m_bInitialized = true;

    if (MODebug2) MODebug2->Message(moText("moResourceManager:: Embedded Resources Ready."));

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
	MODecoderMan = NULL;

	m_Resources.Empty();

	m_bInitialized = false;

	return true;
}
