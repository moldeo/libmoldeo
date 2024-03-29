/*******************************************************************************

                              moConsole.cpp

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

#include <moConsole.h>
#include "moSceneEffect.h"

#define NOSDL 1

#ifndef NOSDL

    #ifdef MO_WIN32
        #include <SDL.h>
    #endif

    #ifdef MO_LINUX
        #include <SDL/SDL.h>
    #endif

    #ifdef MO_MACOSX
        #include <SDL/SDL.h>
    #endif

#else
	#define SDL_KEYDOWN 2
	#define SDL_KEYDOWN_SDL2 0x300
	#define SDLK_ESCAPE 27
	#define SDLK_SCANCODE_MASK (1<<30)
	#define SDLK_F12 ( 69 | SDLK_SCANCODE_MASK )
#endif



#undef main
#include <moDataManager.h>
#include <moFileManager.h>

#include "moArray.h"
moDefineDynamicArray( moPresetParams );


moPresetParamDefinition::moPresetParamDefinition() {

}

moPresetParamDefinition::moPresetParamDefinition(const moPresetParamDefinition &src) {
    (*this) = src;
}

moPresetParamDefinition::~moPresetParamDefinition() {

}

moPresetParamDefinition &moPresetParamDefinition:: operator = (const moPresetParamDefinition &src) {
    m_ObjectId = src.m_ObjectId;
    m_ParamIndex = src.m_ParamIndex;
    m_ValueIndex = src.m_ValueIndex;
    m_State = src.m_State;
	return (*this);
}



moConsole::moConsole() : moMoldeoObject() {

	m_bIODeviceManagerDefault = true;
	m_pIODeviceManager = NULL;
	m_pResourceManager = NULL;

	m_bExternalResources = false;

  ///Important settings for CONSOLE (using inlets and outlets...)
  this->SetName("__console__");
  this->SetLabelName("__console__");
  this->SetType( MO_OBJECT_CONSOLE );

	ticks = 0;
	fps_current = 0;
	fps_mean = 0;
	fps_count = 0;
}

moConsole::~moConsole()
{
    Finish();
}

bool moConsole::LabelNameExists( const moText& labelname ) {

  int mobid = -1;

  mobid = GetObjectId(labelname);
  if (mobid>-1) {
      moMoldeoObject* pObj = GetObjectByIdx(mobid);
      if (pObj) {
        moText obj_labelname =  pObj->GetLabelName();
        if ( obj_labelname!=labelname ) {
          return false;
        }
      }
      else
        return false;
  }

  return ( mobid > -1 );
}

int
moConsole::RelativeToGeneralIndex( int relativeindex, moMoldeoObjectType p_type ) {

  int mindex = -1;

  switch(p_type) {
    case MO_OBJECT_EFFECT:
      mindex = m_pResourceManager->Resources().Count();
      mindex+= m_pIODeviceManager->IODevices().Count();
      mindex+= m_EffectManager.PreEffects().Count();
      mindex+= m_EffectManager.PostEffects().Count();
      mindex+= relativeindex;
      break;
    case MO_OBJECT_PREEFFECT:
      mindex = m_pResourceManager->Resources().Count();
      mindex+= m_pIODeviceManager->IODevices().Count();
      mindex+= relativeindex;
      break;
    case MO_OBJECT_POSTEFFECT:
      mindex = m_pResourceManager->Resources().Count();
      mindex+= m_pIODeviceManager->IODevices().Count();
      mindex+= m_EffectManager.PreEffects().Count();
      mindex+= relativeindex;
      break;
    case MO_OBJECT_MASTEREFFECT:
      mindex = m_pResourceManager->Resources().Count();
      mindex+= m_pIODeviceManager->IODevices().Count();
      mindex+= m_EffectManager.PreEffects().Count();
      mindex+= m_EffectManager.PostEffects().Count();
      mindex+= m_EffectManager.Effects().Count();
      mindex+= relativeindex;
      break;
    case MO_OBJECT_IODEVICE:
      mindex = m_pResourceManager->Resources().Count();
      mindex+= relativeindex;
      break;
    case MO_OBJECT_RESOURCE:
      mindex = 0;
      mindex+= relativeindex;
      break;
    case MO_OBJECT_CONSOLE:
      mindex = m_pResourceManager->Resources().Count();
      mindex+= m_pIODeviceManager->IODevices().Count();
      mindex+= m_EffectManager.PreEffects().Count();
      mindex+= m_EffectManager.PostEffects().Count();
      mindex+= m_EffectManager.Effects().Count();
      mindex+= m_EffectManager.MasterEffects().Count();
      break;

    default:
      break;

  }
  if (mindex==-1) MODebug2->Error("moConsole::RelativeToGeneralIndex > type not found");
  return mindex;

}

moMoldeoObjects& moConsole::GetMoldeoObjects() {
  return m_MoldeoObjects;
}

moEffectManager& moConsole::GetEffectManager() {

    return m_EffectManager;

}

void moConsole::SetIODeviceManager( moIODeviceManager*	p_IODeviceManager ) {

	if (p_IODeviceManager!=NULL)
		m_pIODeviceManager = p_IODeviceManager;

}

void moConsole::InitResources( moResourceManager *pResourceManager,
						moText	p_apppath,
						moText	p_datapath,
						moConfig&	p_consoleconfig,
						MOint p_render_to_texture_mode,
						MOint p_screen_width, MOint p_screen_height,
						MOint p_render_width, MOint p_render_height,
                        MO_HANDLE p_OpWindowHandle,
                        MO_DISPLAY p_Display) {


	if (pResourceManager==NULL) {
		SetResourceManager( new moResourceManager());
	} else {
		m_bExternalResources = true;
		SetResourceManager( pResourceManager );
	}

	if (m_pResourceManager) {
		m_pResourceManager->Init(
                    p_apppath,
                    p_datapath,
					p_consoleconfig,
					p_render_to_texture_mode,
					p_screen_width, p_screen_height,
					p_render_width, p_render_height,
					p_OpWindowHandle,
					p_Display);
	}
}


MOboolean moConsole::Init()
{
	return Init( moText(""), moText("data"), moText("data/console.mol"), NULL, NULL, RENDERMANAGER_MODE_NORMAL,
		MO_DEF_SCREEN_WIDTH,
		MO_DEF_SCREEN_HEIGHT,
		MO_DEF_RENDER_WIDTH,
		MO_DEF_RENDER_HEIGHT );

}


MOboolean moConsole::Init(
                        moText p_apppath,
                        moText p_datapath,
						moText p_consoleconfig,
						moIODeviceManager* p_pIODeviceManager,
						moResourceManager*  p_pResourceManager,
					  moRenderManagerMode p_render_to_texture_mode,
					  MOint p_screen_width, MOint p_screen_height,
					  MOint p_render_width, MOint p_render_height,
                      MO_HANDLE p_OpWindowHandle,
                      MO_DISPLAY p_Display)
{

	moText text;
	//int verif;
	//int a,b;

	idebug = -1;
	iligia = -1;
	iborrado = -1;
  m_ConsoleScript = moText("");
  //moMoldeoObjectType consoletype = moGetStrType( moText("console") );

	srand( time(NULL) );

	//if no IODeviceManager defined by the user, use default
	if (MODebug2) MODebug2->Message(moText("moConsole:: Initializing IODevice's Manager."));

	if( p_pIODeviceManager == NULL ) {
		m_bIODeviceManagerDefault = true;
		m_pIODeviceManager = new moIODeviceManager();
		if (m_pIODeviceManager)
			m_pIODeviceManager->Init();
	} else {
		m_bIODeviceManagerDefault = false;
		m_pIODeviceManager = p_pIODeviceManager;
	}

    if (MODebug2) MODebug2->Message(moText("moConsole::Init > Initializing Effects Manager."));
	m_EffectManager.Init();
	m_EffectManager.m_pEffectManager = &m_EffectManager;

	//Inicializando Estado de la Consola
	m_ConsoleState.Init();

    //==========================================================================
	//   CARGAMOS EL ARCHIVO DE CONFIGURACION
	//==========================================================================

  SetConfigName( p_consoleconfig );

  ///we must initialize the parametrization here
  ///moldeoobect::init siempre antes que los moDefineParamIndex....y justo despues del LoadConfig
  if (!moMoldeoObject::Init()) {
    Finish();
    return false;
  }

  ///TODO: ahora todos los parametros script de moldeoobject se llaman "script", hay que eliminar cualquier referencia anterior
  /// en la consola se llaman: consolescript
  if (__iscript==MO_PARAM_NOT_FOUND) __iscript = m_Config.GetParamIndex(moText("consolescript"));

  if (MODebug2) MODebug2->Message(moText("moConsole::Init > mol project opening....success "));


  //if () {
      moText mode = m_Config.Text("outputmode");

      moText rendermode = m_Config.Text("rendermode");

      if ( rendermode != moText("") ) {
          if (rendermode==moText("RENDERMANAGER_MODE_NORMAL")) {
              MODebug2->Message("moConsole :: Render Mode forced to RENDERMANAGER_MODE_NORMAL");
              p_render_to_texture_mode = RENDERMANAGER_MODE_NORMAL;
          } else if (rendermode==moText("RENDERMANAGER_MODE_FRAMEBUFFER")) {
              MODebug2->Message("moConsole :: Render Mode forced to RENDERMANAGER_MODE_FRAMEBUFFER");
              p_render_to_texture_mode = RENDERMANAGER_MODE_FRAMEBUFFER;
          } else if (rendermode==moText("RENDERMANAGER_MODE_VDPAU")) {
              MODebug2->Message("moConsole :: Render Mode forced to RENDERMANAGER_MODE_VDPAU");
              p_render_to_texture_mode = RENDERMANAGER_MODE_VDPAU;
          }
      }

      moText renderwidth = m_Config.GetParam("renderresolution").GetValue().GetSubValue(0).Text();
      moText renderheight = m_Config.GetParam("renderresolution").GetValue().GetSubValue(1).Text();

      moText screenwidth = m_Config.GetParam("outputresolution").GetValue().GetSubValue(0).Text();
      moText screenheight = m_Config.GetParam("outputresolution").GetValue().GetSubValue(1).Text();


      if (renderwidth!=moText("") && renderheight!=moText("")) {
          int i_render_width = atoi(renderwidth);
          int i_render_height = atoi(renderheight);

          if ( 0<i_render_width && i_render_width<6144 ) {
              p_render_width = i_render_width;
          }

          if ( 0<i_render_height && i_render_height<6144 ) {
              p_render_height = i_render_height;
          }
      }

      if (screenwidth!=moText("") && screenheight!=moText("")) {
          int i_screenwidth = atoi(screenwidth);
          int i_screenheight = atoi(screenheight);

          if ( 0<i_screenwidth && i_screenwidth<6144 ) {
              p_screen_width = i_screenwidth;
          }

          if ( 0<i_screenheight && i_screenheight<6144 ) {
              p_screen_height = i_screenheight;
          }
      }


  //}

	// Verificar que el nro de version sea correcto //
  //...
  ///los recursos se cargan antes que el moMoldeoObject::Init
  ///ya que algunos parametros necesitan de todos los recursos para levantar
  ///ejemplo: moMathFunction....

  if (MODebug2) MODebug2->Message(moText("moConsole::Init > Initializing Resource Manager. ")
                                  + moText( " app path: ") + p_apppath
                                  + moText( " data path: ") + p_datapath
                                  + moText( " config: ") + m_Config.GetName()
                                  + moText( " render mode: ") + IntToStr(p_render_to_texture_mode)
                                  + moText( " screen size: ") + IntToStr(p_screen_width) + moText("x") + IntToStr(p_screen_height)
                                  + moText( " render size: ") + IntToStr(p_render_width) + moText("x") + IntToStr(p_render_height) );

	InitResources(  p_pResourceManager,
                    p_apppath,
					p_datapath,
					m_Config,
					p_render_to_texture_mode,
					p_screen_width, p_screen_height,
					p_render_width, p_render_height,
					p_OpWindowHandle,
					p_Display);

  m_EffectManager.m_pResourceManager = GetResourceManager();


  ///cargamos los inlets, outlets de la consola...
  moMoldeoObject::CreateConnectors();

	moDefineParamIndex( CONSOLE_DEVICES, moText("devices") );
	moDefineParamIndex( CONSOLE_EFFECT, moText("effect") );
	moDefineParamIndex( CONSOLE_PREEFFECT, moText("preeffect") );
	moDefineParamIndex( CONSOLE_EFFECT, moText("effect") );
	moDefineParamIndex( CONSOLE_POSTEFFECT, moText("posteffect") );
	moDefineParamIndex( CONSOLE_MASTEREFFECT, moText("mastereffect") );
	moDefineParamIndex( CONSOLE_RESOURCES, moText("resources") );
	moDefineParamIndex( CONSOLE_ON, moText("mastereffects_on") );
	moDefineParamIndex( CONSOLE_FULLDEBUG, moText("fulldebug") );
	//moDefineParamIndex( CONSOLE_SCRIPT, moText("consolescript") );
	moDefineParamIndex( CONSOLE_OUTPUTMODE, moText("outputmode") );
	moDefineParamIndex( CONSOLE_OUTPUTRESOLUTION, moText("outputresolution") );
	moDefineParamIndex( CONSOLE_RENDERMODE, moText("rendermode") );
	moDefineParamIndex( CONSOLE_RENDERRESOLUTION, moText("renderresolution") );
	moDefineParamIndex( CONSOLE_CLIP1, moText("clip1") );
	moDefineParamIndex( CONSOLE_CLIP2, moText("clip2") );
	moDefineParamIndex( CONSOLE_CLIP3, moText("clip3") );
	moDefineParamIndex( CONSOLE_SCREENSHOTS, moText("screenshots") );
	moDefineParamIndex( CONSOLE_VIDEOSHOTS, moText("videoshots") );


/** WARNING

    TODO: check
    */
    /** the order of initialization is important:
        some tweaks are needed for example:

        if an IODevice create a texture and a Resource needs it, The resource must be loaded after the IODevice.

        But if the IODevice needs a texture created by a Resource, the opposite is what we need.

        Solutions:
            1) BIG ONE
                Each time a new texture is created:
                    the texture manager seek in a buffer for any previous attempt to load this texture,
                    so it could notify the object who was needing this texture.

                TextureManager must register the unloaded textures ? YES
                    If they no longer exists on any configuration param, it could forgot about them.
            2) SIMPLE: creating the textures in console.
            3) WEIRD: creating all the textures before we load it
            4) IODevices were not supposed to create any texture ? so:
                a) Always create and register additional texture in TextureManager (needs his own custom config)
                b) Then make a Preload Texture method/function for all objects
    */
	//LoadResources();
	/**TODO: implementar un LoadObjects() General que tambien asigne los ids*/
	LoadIODevices();
	LoadResources();

	LoadPreEffects();
	LoadPostEffects();
	LoadEffects();
	LoadMasterEffects();

	MOboolean m_bMasterEffects_On = m_Config.GetParam( moText("mastereffects_on") ).GetValue().GetSubValue(0).Int();
	if (m_bMasterEffects_On) {
        //
  }

  m_MoldeoObjects.Add( (moMoldeoObject*)this );

  ///RECONNECT!!!!!

	UpdateMoldeoIds();


  ///UPDATE VIDEOMANAGER FOR CAMERAS!!!! TO TEXTURE
  if (m_pResourceManager->GetVideoMan() && m_pResourceManager->GetRenderMan()) {

    if (MODebug2) MODebug2->Message(moText("moConsole::Init > calling VideoManager update function."));
    //m_pResourceManager->GetRenderMan()->BeginUpdateObject();
    //m_pResourceManager->GetVideoMan()->Update( m_pIODeviceManager->GetEvents() );
    //m_pResourceManager->GetRenderMan()->EndUpdateObject();
  }

	///Finalmente inicializamos los efectos
	///asigna Inlets y outlets...
	if (MODebug2) MODebug2->Message(moText("moConsole::Init > InitializeAllEffects."));
	this->InitializeAllEffects();

	if (MODebug2) MODebug2->Message(moText("moConsole::Init > StartMasterEffects."));
	this->StartMasterEffects();

  ///CONECTAMOS Inlets con Outlets
  if (MODebug2) MODebug2->Message(moText("moConsole::Init > LoadConnections."));
  LoadConnections();

  ///rerun because of possible ids needed
    if (MODebug2) MODebug2->Message(moText("moConsole::Init > ScriptExeInit."));
    ScriptExeInit();

    m_bInitialized = true;


    moDataMessage* pMessageStayingAlive = new moDataMessage();
    pMessageStayingAlive->Add( moData( "consoleget" ) );
    ProcessMoldeoAPIMessage(pMessageStayingAlive);


    return Initialized();
}

void
moConsole::UpdateMoldeoIds() {

  ///RECREATE ALL REFERENCES int this order
  /**
  RESOURCES
  IODEVICES
  PREEFFECTS
  POSTEFFECTS
  EFFECTS
  MASTEREFFECTS
  */

  int max = RelativeToGeneralIndex( 0, MO_OBJECT_CONSOLE ) + 1;
  m_MoldeoObjects.Empty();
  m_MoldeoObjects.Init( max, NULL);

  m_MoldeoSceneObjects.Empty();

  for( MOuint i=0; i<m_pResourceManager->Resources().Count(); i++ ) {
    moResource* pResource = m_pResourceManager->Resources().GetRef(i);
    m_MoldeoObjects.Set( RelativeToGeneralIndex( i, MO_OBJECT_RESOURCE ), pResource );
  }

  for( MOuint i=0; i<m_pIODeviceManager->IODevices().Count(); i++ ) {
    moIODevice* pIODevice = m_pIODeviceManager->IODevices().GetRef(i);
    m_MoldeoObjects.Set( RelativeToGeneralIndex( i, MO_OBJECT_IODEVICE ), pIODevice );
  }

  for( MOuint i=0; i<m_EffectManager.PreEffects().Count(); i++ ) {
    moPreEffect* pFx = m_EffectManager.PreEffects().GetRef(i);
    m_MoldeoObjects.Set( RelativeToGeneralIndex( i, MO_OBJECT_PREEFFECT ), pFx );
  }

  for( MOuint i=0; i<m_EffectManager.PostEffects().Count(); i++ ) {
    moPostEffect* pFx = m_EffectManager.PostEffects().GetRef(i);
    m_MoldeoObjects.Set( RelativeToGeneralIndex( i, MO_OBJECT_POSTEFFECT ), pFx );
  }

  for( MOuint i=0; i<m_EffectManager.Effects().Count(); i++ ) {
    moEffect* pFx = m_EffectManager.Effects().GetRef(i);
    m_MoldeoObjects.Set( RelativeToGeneralIndex( i, MO_OBJECT_EFFECT ), pFx );
  }

  for( MOuint i=0; i<m_EffectManager.MasterEffects().Count(); i++ ) {
    moMasterEffect* pFx = m_EffectManager.MasterEffects().GetRef(i);
    m_MoldeoObjects.Set( RelativeToGeneralIndex( i, MO_OBJECT_MASTEREFFECT ), pFx );
  }

  m_MoldeoObjects.Set( RelativeToGeneralIndex( 0, MO_OBJECT_CONSOLE ), this);

	///SET Moldeo Objects Unique Id's
	for( MOuint i=0; i<m_MoldeoObjects.Count(); i++) {

    moMoldeoObject* mobject = m_MoldeoObjects.GetRef(i);
    if (mobject==NULL) continue;
    int old_MoldeoId = mobject->GetId();
    int new_Moldeoid = MO_MOLDEOOBJECTS_OFFSET_ID + i;

    moEventList* pEvents = m_pIODeviceManager->GetEvents();
    if (pEvents) {
      	moEvent *actual=NULL,*tmp;
        moMessage *pmessage;

        if (pEvents) actual = pEvents->First;

        ///Eventos MO_ACTION_MOLDEOAPI_EVENT_RECEIVE a retraducir
        while(actual!=NULL) {
          tmp = actual->next;
          ///procesamos aquellos Outlet q estan dirigidos a este objeto
          if (actual->deviceid==MO_IODEVICE_CONSOLE
              && actual->devicecode == MO_ACTION_MOLDEOAPI_EVENT_RECEIVE
              && actual->reservedvalue0 == old_MoldeoId
              && actual->reservedvalue3 == MO_DATAMESSAGE) {
                actual->reservedvalue0 = new_Moldeoid;
          }
          actual = tmp;
        }

    }
		if (mobject) mobject->SetId( new_Moldeoid );
	}

  ///PROCESSING SCENE OBJECTS (recursive)

	for( MOuint i=0; i<m_EffectManager.Effects().Count(); i++ ) {
    moEffect* pFx = m_EffectManager.Effects().GetRef(i);
    if (pFx)
      if (pFx->GetName()=="scene") {
        moSceneEffect* pScene = (moSceneEffect*) pFx;
        pScene->UpdateMoldeoIds( m_MoldeoSceneObjects );
      }
  }

  ///SET UNIQUE IDS!!!
  /*
  for( MOuint i=0; i<m_MoldeoSceneObjects.Count(); i++) {
		moMoldeoObject* mobject = m_MoldeoSceneObjects.GetRef(i);
		if (mobject) mobject->SetId(MO_MOLDEOSCENEOBJECTS_OFFSET_ID + i);
	}
	*/

}

void
moConsole::LoadConnections() {
	///check for each outlet connector on MoldeoObject's connections to inlets...
	MOuint i,j,k,l,m;

	UpdateMoldeoIds();

	///Connect outlets to inlets....
	for( i=0; i<(m_MoldeoObjects.Count()+m_MoldeoSceneObjects.Count()); i++) {

		moMoldeoObject* psrcobject = NULL;
		if ( i<m_MoldeoObjects.Count())
        psrcobject = m_MoldeoObjects[i];
		else
        psrcobject = m_MoldeoSceneObjects[i-m_MoldeoObjects.Count()];

		if (psrcobject) {

            moOutlets* poutlets = psrcobject->GetOutlets();

            ///run over the outlet connectors connections...
            for( j=0; j<poutlets->Count(); j++) {

                moOutlet* p_outlet = poutlets->Get(j);

                moConnections* p_connections = p_outlet->GetConnections();

                for( k = 0; k < p_connections->Count(); k++ ) {

                    moConnection *p_connection = p_connections->GetRef(k);//get the connection to update

                    moText DestinationMoldeoLabelName = p_connection->GetDestinationMoldeoLabelName();
                    moText DestinationConnectorLabelName = p_connection->GetDestinationConnectorLabelName();
                    bool connector_found;
                    bool object_found;

                    connector_found = false;
                    object_found = false;

                    ///search for moldeolabelname
                    ///search for connector labelname
                    for( l=0; l<(m_MoldeoObjects.Count()+m_MoldeoSceneObjects.Count()); l++) {

                        moMoldeoObject* pdstobject = NULL;

                        if ( l<m_MoldeoObjects.Count())
                          pdstobject = m_MoldeoObjects[l];
                        else
                          pdstobject = m_MoldeoSceneObjects[l-m_MoldeoObjects.Count()];

                        if (pdstobject) {
                            if ( pdstobject->GetLabelName()==DestinationMoldeoLabelName ) {
                                object_found = true;
                                ///update destination id
                                p_connection->SetDestinationMoldeoId(pdstobject->GetId());
                                for( m = 0; m < pdstobject->GetInlets()->Count() ; m++) {
                                    moInlet* pinlet = pdstobject->GetInlets()->Get(m);
                                    if (pinlet->GetConnectorLabelName()==DestinationConnectorLabelName) {
                                        connector_found = true;
                                        //update destination connector id
                                        p_connection->SetDestinationConnectorId( pinlet->GetConnectorId() );
                                        MODebug2->Log(
                                            moText("moConsole::LoadConnections > Object <") +
                                            psrcobject->GetLabelName() +
                                            moText("> Outlet <") +
                                            p_outlet->GetConnectorLabelName() +
                                            moText("> connected succesfully to") +

                                            moText(" Object <") +
                                            pdstobject->GetLabelName() +
                                            moText("> Inlet <") +
                                            pinlet->GetConnectorLabelName() +
                                            moText(">")
                                            );
                                    }
                                }
                            }
                        }
                    } //end search

                    if (!object_found) {
                        MODebug2->Error(
                                            moText("moConsole::LoadConnections > From Object <") +
                                            psrcobject->GetLabelName() +
                                            moText("> to Object <") +
                                            DestinationMoldeoLabelName +
                                            moText("> object label not found")
                                            );
                    } else if (!connector_found) {
                        MODebug2->Error(
                                            moText("moConsole::LoadConnections > From Object <") +
                                            psrcobject->GetLabelName() +
                                            moText("> to Object <") +
                                            DestinationMoldeoLabelName +
                                            moText("> Inlet <") +
                                            DestinationConnectorLabelName +
                                            moText("> connector label not found")
                                            );
                    }

                }
            }
		}
	}

}


void
moConsole::LoadObjects( moMoldeoObjectType fx_type ) {

  if (fx_type==MO_OBJECT_UNDEFINED) {

    for(int i=0;i<MO_OBJECT_TYPES;i++ ) {
      this->LoadObjects( (moMoldeoObjectType)i );
    }
    return;
  }

  moText text,fxname,cfname,lblname,keyname;
	MOint efx,i,N;
	bool activate = true;
	moEffect*	peffect = NULL;

	moText fx_string = moMobDefinition::GetTypeToName(fx_type);

  efx = m_Config.GetParamIndex(fx_string);
  m_Config.SetCurrentParamIndex(efx);
	N = m_Config.GetValuesCount(efx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole::LoadObjects > Loading Object configs...") );
		MODebug2->Message( moText("moConsole::LoadObjects > Objects number: ") + IntToStr(N)  );
	}

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i < N; i++) {
			Draw();
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();

      if (m_Config.GetParam().GetValue().GetSubValueCount()>=6)
        keyname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_KEY).Text();

      if (m_Config.GetParam().GetValue().GetSubValueCount()>=4)
        activate = (m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_ON).Int()>0);


			moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {
                if ((moText)fxname!=moText("nil")) {

                    peffect = (moEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname, keyname, MO_OBJECT_EFFECT, efx, i, activate );

                    if (peffect) {
                      m_MoldeoObjects.Add( (moMoldeoObject*) peffect );
                    }

                    if (MODebug2) {
                      MODebug2->Message( moText("moConsole::LoadObjects > ") + completecfname );
                    }


                } else {
                    peffect = NULL;
                    m_EffectManager.Effects().Add(peffect);
                    m_EffectManager.AllEffects().Add(peffect);
                    m_MoldeoObjects.Add( (moMoldeoObject*) peffect );
                }
			} else {
			    MODebug2->Error(moText("moConsole::LoadObjects > Error: Config File doesn't exist : ") + (moText)completecfname);
      }
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Message(moText("moConsole::LoadObjects > Effects loaded !"));

}



//==========================================================================
//   CARGAMOS LOS DISPOSITIVOS
//==========================================================================
void
moConsole::LoadIODevices() {

	moText text, fxname, cfname, lblname, keyname;
	MOint devices, i, ndevices;
	bool activate = true;
	moIODevice*	pdevice = NULL;

	devices = m_Config.GetParamIndex("devices");
	ndevices = m_Config.GetParam(devices).GetValuesCount();

	m_Config.SetCurrentParamIndex(devices);
    m_pIODeviceManager->Init();
	m_Config.FirstValue();

	for( i=0; i<ndevices; i++) {
		fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
		cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
		lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();


    if (m_Config.GetParam().GetValue().GetSubValueCount()>=4)
      activate = (m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_ON).Int()>0);

    if (m_Config.GetParam().GetValue().GetSubValueCount()>=6)
      keyname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_KEY).Text();

    moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
    moFile FullCF( completecfname );

    if ( FullCF.Exists() ) {
        pdevice = m_pIODeviceManager->NewIODevice( fxname, cfname, lblname, keyname,  MO_OBJECT_IODEVICE, devices, i, activate );
        if (pdevice) {
            m_MoldeoObjects.Add( (moMoldeoObject*) pdevice );
            pdevice->SetResourceManager( m_pResourceManager );
            pdevice->Init();
            if (activate) pdevice->Activate();
        } else MODebug2->Error( moText("moConsole::LoadIODevices > Couldn't create the device:") + moText(fxname));
		} else {
		    MODebug2->Error(moText("moConsole::LoadIODevices > Error: Config File doesn't exist : ") + (moText)completecfname);
        }
		m_Config.NextValue();

    }
	if (MODebug2) MODebug2->Message( moText("moConsole::LoadIODevices > IODevices loaded.") );

  ///check if we have any netoscin or out, configured in port 3334, and 3335
  ///respectively
  AddMoldeoAPIDevices();

}

void moConsole::UnloadIODevices() {
    if (m_pIODeviceManager)
        while(m_pIODeviceManager->IODevices().Count()>0) {
            if( m_pIODeviceManager->IODevices().GetRef(0) != NULL ) {
                m_pIODeviceManager->RemoveIODevice( 0 );
            }
        }
}

void
moConsole::AddMoldeoAPIDevices() {

  bool MoldeoAPISender = false;//in port 3335
  bool MoldeoAPIListener = false;//in port 3334
  ///check if oscin or oscout are present
  if (m_pIODeviceManager)
      for( MOuint i=0; i<(m_pIODeviceManager->IODevices().Count()); i++ ) {
        moIODevice* pDevice = m_pIODeviceManager->IODevices().GetRef(i);
        if (pDevice) {
          if ( pDevice->GetName()=="netoscin" ) {
            if (
              (pDevice->GetConfig()->Text( "hosts" ) == "127.0.0.1"
                && pDevice->GetConfig()->Int( "port" ) == 3334)
              || ( pDevice->GetLabelName()=="moldeoapioscin")
            ) {
                MoldeoAPIListener = true;
                if (MoldeoAPISender) break;
            }
          }

          if ( pDevice->GetName()=="netoscout" ) {
            if (
            ( pDevice->GetConfig()->Text( "hosts" ) == "127.0.0.1"
              && pDevice->GetConfig()->GetValue( "hosts" ).GetSubValue(1).Int() == 3335)
              ||
              (pDevice->GetLabelName()=="moldeoapioscout")
            ) {
                  MoldeoAPISender = true;
                 if (MoldeoAPIListener) break;
            }
          }
        }
      }

  moIODevice* pdevice =NULL;
  moMobDefinition MD;
  if (!MoldeoAPIListener) {
      MD.SetConfigName("moldeoapioscin");
      MD.SetLabelName("moldeoapioscin");
      MD.SetName("netoscin");
      MD.SetConsoleParamIndex( m_Config.GetParamIndex("devices") );
      MD.SetConsoleValueIndex( m_pIODeviceManager->IODevices().Count() );
      pdevice = m_pIODeviceManager->NewIODevice( MD.GetName(), MD.GetConfigName(), MD.GetLabelName(), MD.GetKeyName(), MO_OBJECT_IODEVICE, MD.GetMobIndex().GetParamIndex(), MD.GetMobIndex().GetValueIndex() );
      if (pdevice) {
          m_MoldeoObjects.Add( (moMoldeoObject*) pdevice );
          moFile pFile( DataMan()->GetDataPath() + moSlash + MD.GetConfigName() + moText(".cfg") );
          if (!pFile.Exists()) pdevice->GetConfig()->CreateDefault( pFile.GetCompletePath() );
          pdevice->SetResourceManager( m_pResourceManager );
          pdevice->Init();
          pdevice->Activate();
      } else MODebug2->Error( moText("moConsole::LoadIODevices > Couldn't create the device:") + moText("moldeoapioscin"));

  }
  if (!MoldeoAPISender) {
      MD.SetConfigName("moldeoapioscout");
      MD.SetLabelName("moldeoapioscout");
      MD.SetName("netoscout");
      MD.SetConsoleParamIndex( m_Config.GetParamIndex("devices") );
      MD.SetConsoleValueIndex( m_pIODeviceManager->IODevices().Count() );
      pdevice = m_pIODeviceManager->NewIODevice( MD.GetName(), MD.GetConfigName(), MD.GetLabelName(), MD.GetKeyName(), MO_OBJECT_IODEVICE, MD.GetMobIndex().GetParamIndex(), MD.GetMobIndex().GetValueIndex() );
      if (pdevice) {
          m_MoldeoObjects.Add( (moMoldeoObject*) pdevice );
          moFile pFile( DataMan()->GetDataPath() + moSlash + MD.GetConfigName() + moText(".cfg") );
          if (!pFile.Exists()) pdevice->GetConfig()->CreateDefault( pFile.GetCompletePath() );
          pdevice->SetResourceManager( m_pResourceManager );
          pdevice->Init();
          pdevice->Activate();
      } else MODebug2->Error( moText("moConsole::LoadIODevices > Couldn't create the device:") + moText("moldeoapioscout"));

  }

}

//==========================================================================
//   CARGAMOS LOS EFFECTS MAESTROS
//==========================================================================
void
moConsole::LoadMasterEffects() {
	moText text, fxname, cfname, lblname, keyname;
	MOint N,i,mtfx;
	bool activate = true;
	moMasterEffect*	pmastereffect = NULL;

	mtfx = m_Config.GetParamIndex("mastereffect");
	m_Config.SetCurrentParamIndex(mtfx);
	N = m_Config.GetValuesCount(mtfx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole::LoadMasterEffects > Loading MasterEffects configs...") );
		MODebug2->Message( moText("moConsole::LoadMasterEffects > Master Effects number: ") + IntToStr(N)  );
	}

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=6)
        keyname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_KEY).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=4)
        activate = (m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_ON).Int()>0);

      moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {
                pmastereffect = (moMasterEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname, keyname,  MO_OBJECT_MASTEREFFECT, mtfx, i, activate);
                if (pmastereffect) {
                    m_MoldeoObjects.Add( (moMoldeoObject*) pmastereffect );
                    pmastereffect->SetResourceManager( m_pResourceManager );
                    pmastereffect->Set( &m_EffectManager, &m_ConsoleState );
                    if( pmastereffect->GetName() == moText("ligia") ) {
                                iligia=i;
                                pmastereffect->Init();
                                pmastereffect->Activate();
                    }
                } else MODebug2->Error( moText("moConsole::LoadMasterEffects > Couldn't load specific Master-Effect:") + moText(fxname));
			} else {
			    MODebug2->Error(moText("moConsole::LoadMasterEffects > Error: Config File doesn't exist : ") + (moText)completecfname);
            }
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Message( moText("moConsole::LoadMasterEffects > Master-Effects loaded.") );

}

void moConsole::UnloadMasterEffects() {

    while(m_EffectManager.MasterEffects().Count()>0) {
		if( m_EffectManager.MasterEffects().GetRef(0) != NULL ) {
			m_EffectManager.RemoveEffect( 0, MO_OBJECT_MASTEREFFECT );
		}
	}

}

//==========================================================================
//   CARGAMOS LOS PRE-EFFECTS
//==========================================================================
void
moConsole::LoadPreEffects() {

	moText text,fxname,cfname,lblname,keyname;
	MOint prfx,i,N;
  bool activate = true;
	moPreEffect* ppreeffect;

	prfx = m_Config.GetParamIndex("preeffect");
	m_Config.SetCurrentParamIndex(prfx);
	N = m_Config.GetValuesCount(prfx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole::LoadPreEffects > Loading PreEffects configs...") );
		MODebug2->Message( moText("moConsole::LoadPreEffects > Pre-Effects number: ") + IntToStr(N) );
	}

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {

			if(iborrado!=-1) Draw();

			//creando nuevo efecto
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=6)
        keyname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_KEY).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=4)
        activate = (m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_ON).Int()>0);

      moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname + moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {

                ppreeffect = (moPreEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname, keyname,  MO_OBJECT_PREEFFECT, prfx, i, activate);

                if (ppreeffect) {
                    m_MoldeoObjects.Add( (moMoldeoObject*) ppreeffect );
                    //ppreeffect->SetResourceManager( m_pResourceManager );
                    if ( ppreeffect->GetName() == moText("erase") ) {
                        iborrado = i;
                        if (ppreeffect->Init()) {

                            MODebug2->Message("moConsole::LoadPreEffects > Erase initialized");

                            MOint pre,on;
                            MOint paramindex, valueindex;

                            paramindex = ppreeffect->GetMobDefinition().GetMobIndex().GetParamIndex();
                            valueindex = ppreeffect->GetMobDefinition().GetMobIndex().GetValueIndex();

                            pre = m_Config.GetParam(paramindex  ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_PRE).Int();
                            on = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_ON).Int();
                            if (pre>=0) ppreeffect->GetConfig()->SetCurrentPreConf(pre);
                            if (on>0) ppreeffect->Activate();

                            //MODebug2->Message("moConsole::LoadPreEffects > Init passed for object: " + ppreeffect->GetName() + " config: " + ppreeffect->GetConfigName() + " label: " + ppreeffect->GetLabelName() );

                        }
                    }
                } else MODebug2->Error( moText("moConsole::LoadPreEffects > Couldn't load specific Pre-Effect:") + moText(fxname));
                m_Config.NextValue();
			} else {
			    MODebug2->Error(moText("moConsole::LoadPreEffects > Error: Config File doesn't exist : ") + (moText)completecfname);
      }
		}
	}

	if (MODebug2) MODebug2->Message(moText("moConsole::LoadPreEffects > Pre-Effects loaded !"));
}

void moConsole::UnloadPreEffects() {

    while(m_EffectManager.PreEffects().Count()>0) {
		if( m_EffectManager.PreEffects().GetRef(0) != NULL ) {
			m_EffectManager.RemoveEffect( 0, MO_OBJECT_PREEFFECT );
		}
	}
}

//==========================================================================
//   CARGAMOS LOS EFFECTS
//==========================================================================
void
moConsole::LoadEffects() {

	moText text,fxname,cfname,lblname,keyname;
	MOint efx,i,N;
	bool activate = true;
	moEffect*	peffect = NULL;

    efx = m_Config.GetParamIndex("effect");
    m_Config.SetCurrentParamIndex(efx);
	N = m_Config.GetValuesCount(efx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole::LoadEffects > Loading Effects configs...") );
		MODebug2->Message( moText("moConsole::LoadEffects > Effects number: ") + IntToStr(N)  );
	}

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i < N; i++) {
			Draw();
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=6)
        keyname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_KEY).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=4)
        activate = (m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_ON).Int()>0);

			moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {
                if ((moText)fxname!=moText("nil")) {

                    peffect = (moEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname, keyname, MO_OBJECT_EFFECT, efx, i, activate );

                    if (peffect) {
                      m_MoldeoObjects.Add( (moMoldeoObject*) peffect );
                    }

                    if (MODebug2) {
                      MODebug2->Message( moText("moConsole::LoadingEffect > ") + completecfname );
                    }


                } else {
                    ///add a NIL effect...dangerous!
                    peffect = NULL;
                    m_EffectManager.Effects().Add(peffect);
                    m_EffectManager.AllEffects().Add(peffect);
                    m_MoldeoObjects.Add( (moMoldeoObject*) peffect );
                }
			} else {
			    MODebug2->Error(moText("moConsole::LoadEffects > Error: Config File doesn't exist : ") + (moText)completecfname);
            }
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Message(moText("moConsole::LoadEffects > Effects loaded !"));
}

void moConsole::UnloadEffects() {

	while(m_EffectManager.Effects().Count()>0) {
		if( m_EffectManager.Effects().GetRef(0) != NULL ) {
			m_EffectManager.RemoveEffect( 0, MO_OBJECT_EFFECT );
		}
    }

}

//==========================================================================
//   CARGAMOS LOS POST-EFFECTS
//==========================================================================

void
moConsole::LoadPostEffects() {

	moText text, fxname, cfname, lblname, keyname;
	MOint ptfx,i,N;
	bool activate = true;
	moPostEffect*	posteffect = NULL;

	ptfx = m_Config.GetParamIndex("posteffect");
	m_Config.SetCurrentParamIndex(ptfx);
	N = m_Config.GetValuesCount(ptfx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole::LoadPostEffects > Loading PostEffects configs...") );
		MODebug2->Message( moText("moConsole::LoadPostEffects > Post Effects number: ") + IntToStr(N)  );
	}

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {
			if(idebug!=-1) { Draw(); }
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=6)
        keyname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_KEY).Text();
      if (m_Config.GetParam().GetValue().GetSubValueCount()>=4)
        activate = (m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_ON).Int()>0);

            moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {

                posteffect = (moPostEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname, keyname,  MO_OBJECT_POSTEFFECT, ptfx , i, activate );
                if (posteffect) {
                    m_MoldeoObjects.Add( (moMoldeoObject*) posteffect );
                    //posteffect->SetResourceManager( m_pResourceManager );
                    if(posteffect->GetName() == moText("debug")) {
                            idebug = i;
                            posteffect->Init();
                            posteffect->Activate();
                    }
                } else MODebug2->Error( moText("moConsole::LoadPostEffects > Couldn't load specific Post Effect:") + moText(fxname));
			} else {
			    MODebug2->Error(moText("moConsole::LoadPostEffects > Error: Config File doesn't exist : ") + (moText)completecfname);
			}
			m_Config.NextValue();
		}
	}
	if (MODebug2) MODebug2->Message(moText("moConsole::LoadPostEffects > Post Effects loaded !"));
}

void moConsole::UnloadPostEffects() {

    while(m_EffectManager.PostEffects().Count()>0) {
		if( m_EffectManager.PostEffects().GetRef(0) != NULL ) {
			m_EffectManager.RemoveEffect( 0, MO_OBJECT_POSTEFFECT );
		}
	}
}

//==========================================================================
//   CARGAMOS LOS RECURSOS
//==========================================================================



//LOAD RESOURCE PLUGINS
void
moConsole::LoadResources() {

	moText resname, cfname, lblname, keyname;
	bool activate = true;

	int paramindex = m_Config.GetParamIndex(moText("resources"));
	///TODO: chequear validez de este indice
  int resource_valueindex = -1;

	moParam& presources(m_Config.GetParam(moText("resources")));
	presources.FirstValue();
	for(MOuint r=0; r<presources.GetValuesCount(); r++) {
		moResource* presource = NULL;

		resname = presources[MO_SELECTED][MO_CFG_RESOURCE].Text();
		cfname = presources[MO_SELECTED][MO_CFG_RESOURCE_CONFIG].Text();
		lblname = presources[MO_SELECTED][MO_CFG_RESOURCE_LABEL].Text();
    if (presources[MO_SELECTED].GetSubValueCount()>=4)
      activate = (presources[MO_SELECTED].GetSubValue(MO_CFG_EFFECT_ON).Int()>0);
    if (presources[MO_SELECTED].GetSubValueCount()>=6)
      keyname = presources[MO_SELECTED].GetSubValue(MO_CFG_EFFECT_KEY).Text();

		MOint rid = m_pResourceManager->GetResourceIndex( lblname );

		if(rid>-1) presource = m_pResourceManager->GetResource(rid);

		if (presource) {
		    MODebug2->Message( moText("moConsole::LoadResources > Already loaded plugin resource: ") + (moText)resname );
		} else {
			//maybe a plugin
			resource_valueindex = r;
			presource = m_pResourceManager->NewResource(resname, cfname, lblname, keyname, paramindex, resource_valueindex, activate);
			if (presource) {
                presource->SetConfigName(cfname);
                presource->SetLabelName(lblname);
                presource->SetResourceManager( m_pResourceManager );
                if (presource->Init()) {
                    if (activate) presource->Activate();
                    else presource->Deactivate();
                    MODebug2->Message( moText("moConsole::LoadResources > Loaded plugin resource: ") + (moText)resname );
                } else MODebug2->Error( moText("moConsole::LoadResources > Error: Loading plugin resource: ") + (moText)resname );
            }
		}
		presources.NextValue();
	}

	//Agrega los Resources a MoldeoObjects
	if(m_pResourceManager!=NULL) {

		for(MOuint i = 0; i<m_pResourceManager->Resources().Count(); i++) {
			moMoldeoObject* mobject = m_pResourceManager->GetResource(i);
			if (mobject) {
				m_MoldeoObjects.Add( mobject );
			}
		}

	}

  if (MODebug2) MODebug2->Message(moText("moConsole::LoadResources > Resources Plugin loaded !"));

}

void moConsole::UnloadResources() {
    if (m_pResourceManager)
    for(int i=m_pResourceManager->Resources().Count()-1; i>=0; i--) {
        if (m_pResourceManager->Resources().GetRef(i)!=NULL) {
            m_pResourceManager->RemoveResource(i);
        }
    }
}

//==========================================================================
//INICIALIZACION DE CADA EFFECT Y ASIGNACION DE CODIGOS DE DISPOSITIVO(cargados del config de cada efecto)
//==========================================================================

void
moConsole::InitializeAllEffects() {

	int dg;
	moEffect*	p_effect = NULL;

	dg = m_Config.GetParamIndex("fulldebug");


	for(MOuint i=0; i<m_EffectManager.AllEffects().Count(); i++ ) {
		p_effect = m_EffectManager.AllEffects().GetRef(i);
		if( p_effect !=NULL) {

            moMobDefinition MD = p_effect->GetMobDefinition();

            Draw();

            MOint pre,on;
            MOint paramindex = MD.GetMobIndex().GetParamIndex();
            MOint valueindex = MD.GetMobIndex().GetValueIndex();
            moEffectState fxstate = p_effect->GetEffectState();

			if(m_Config.GetParam(dg).GetValue().GetSubValue(0).Text()==moText("yes")) {
				fxstate.fulldebug = MO_ACTIVATED;
			} else {
				fxstate.fulldebug = MO_DEACTIVATED;
			}

			if( p_effect->GetName()!=moText("debug") && p_effect->GetName()!=moText("erase")
				&& p_effect->GetName()!=moText("ligia")) {

				    bool res = p_effect->Init();


					if (res) {

                        pre = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_PRE).Int();
                        on = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_ON).Int();

                        if (pre>=0) p_effect->GetConfig()->SetCurrentPreConf(pre);
                        if (on>0) p_effect->Activate();
                        else p_effect->Deactivate();

                        // Sucio codigo agregado rapidamente para poder asignar los efectos a teclas arbitrarias de las 4 filas
                        // del teclado:
                        /*
                        MOint idx;
                        moText key;
                        key = m_Config.GetParam( pEIx->GetParamIndex() ).GetValue( pEIx->GetValueIndex() ).GetSubValue(MO_CFG_EFFECT_KEY).Text();
                        idx = ConvertKeyNameToIdx(key);
                        p_effect->keyidx = idx;
                        */
					} else {
            MODebug2->Error("Error Initializing Effect: " + p_effect->GetName() + " Label: " + p_effect->GetLabelName() + " Cfg: " + p_effect->GetConfigName() );
					}
			} else {

                pre = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_PRE).Int();
                on = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_ON).Int();

                if (pre>=0) p_effect->GetConfig()->SetCurrentPreConf(pre);
                if (on>0) p_effect->Activate();
                else p_effect->Deactivate();

                // TODO: para poder asignar los efectos a teclas arbitrarias de las 4 filas
                // del teclado:
                /*
                MOint idx;
                moText key;
                key = m_Config.GetParam( pEIx->GetParamIndex() ).GetValue( pEIx->GetValueIndex() ).GetSubValue(MO_CFG_EFFECT_KEY).Text();
                idx = ConvertKeyNameToIdx(key);
                p_effect->keyidx = idx;
                */
			}
			//carga c�digos...
			p_effect->LoadCodes( m_pIODeviceManager );
		}
	}

}

void moConsole::FinalizeAllEffects() {

	moEffect*	p_effect = NULL;

	for( MOuint i=0; i< m_EffectManager.AllEffects().Count(); i++ ) {
		p_effect = m_EffectManager.AllEffects().GetRef(i);
		if( p_effect!=NULL ) {
			p_effect->Finish();
		}
	}
}

void
moConsole::StartMasterEffects() {

	moText text;
	moEffect*	p_effect = NULL;
	MOuint i;

    text = moText("Turning on MasterEffects...");
	if (MODebug2) MODebug2->Push(text);

	//==========================================================================
	//PRENDEMOS LOS EFFECTS MAESTROS y ...LARGAMOS
	//==========================================================================
	for( i=0; i<m_EffectManager.MasterEffects().Count(); i++ ) {
		p_effect = m_EffectManager.MasterEffects().GetRef(i);
		if( p_effect!=NULL ) {
			Draw();
			p_effect->Activate();
		}
	}
	if( m_EffectManager.MasterEffects().Count()>0 ) {
		p_effect = m_EffectManager.MasterEffects().GetRef(0);
		if(p_effect!=NULL)
			p_effect->Activate();
	}
	if( m_EffectManager.MasterEffects().Count()>1 ) {
		p_effect = m_EffectManager.MasterEffects().GetRef(1);
		if(p_effect!=NULL)
			p_effect->Activate();
	}
	if (MODebug2) MODebug2->Push( moText("moConsole::StartMasterEffects > Master effects on.") );

}

void moConsole::StopMasterEffects() {

	moEffect*	pEffect = NULL;

    if (MODebug2) MODebug2->Push( moText("moConsole::StartMasterEffects > turning off MasterEffects.") );

	if(m_EffectManager.MasterEffects().Count()>0) {
		pEffect = m_EffectManager.MasterEffects().GetRef(0);
		if( pEffect!=NULL )
			pEffect->Deactivate();
	}
	if( m_EffectManager.MasterEffects().Count()>1 ) {
		pEffect = m_EffectManager.MasterEffects().GetRef(1);
		if(pEffect!=NULL)
			pEffect->Deactivate();
	}
	for(MOuint i=0;i<m_EffectManager.MasterEffects().Count();i++) {
		pEffect = m_EffectManager.MasterEffects().GetRef(i);
		if( pEffect!=NULL ) {
			Draw();
			pEffect->Deactivate();
		}
	}

	if (MODebug2) MODebug2->Push( moText("moConsole::StartMasterEffects > MasterEffects turned OFF.") );

}

MOulong
moConsole::GetTicks() {

	return moGetTicks();

}

void
moConsole::GLSwapBuffers() {

	//SDL_GL_SwapBuffers();
}

void
moConsole::GUIYield() {
  //no hace nada
  return;
}

void moConsole::ScriptExeDraw() {
    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Draw")) {

            m_pResourceManager->GetGLMan()->SaveGLState();
#ifndef OPENGLESV2
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
#endif
            m_pResourceManager->GetGLMan()->SetOrthographicView();
            m_pResourceManager->GetGLMan()->SetMoldeoGLState();

#ifndef OPENGLESV2
            glEnable(GL_BLEND);
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
#endif
            SelectScriptFunction("Draw");
            RunSelectedFunction();
            m_pResourceManager->GetGLMan()->RestoreGLState();
        }
    }
}

void
moConsole::Draw() {
	MOuint i;
	moEffect* pEffect = NULL;
	moText savename, framesavename;

	if (m_pResourceManager==NULL) return;
	moRenderManager* RenderMan = m_pResourceManager->GetRenderMan();

  //GUIYield();

  //MODebug2->Message("out 2");
  //exit(1);

	if (RenderMan==NULL) return;

  if (m_bInitialized) ScriptExeRun();

	MOswitch borrar = MO_ACTIVATED;
    MOboolean pre_effect_on = false;

	if(m_ConsoleState.pause==MO_DEACTIVATED && m_pResourceManager->GetRenderMan() ) {

		//m_ConsoleState.tempo.ticks = GetTicks();
    ConsoleModeUpdate();
/*
        MODebug2->Push( "Console: tempo.on: " + IntToStr( (int)m_ConsoleState.tempo.Started() )
                    + " tempo.pause_on: " + IntToStr( (int)m_ConsoleState.tempo.Paused())
                    + " tempo.ticks: " + IntToStr( m_ConsoleState.tempo.ticks )
                    + " tempo.ang: " + FloatToStr( m_ConsoleState.tempo.ang ) );
*/
        RenderMan = m_pResourceManager->GetRenderMan();
		if (RenderMan) RenderMan->BeginDraw();

		//Se dibujan los m_PreEffects
		for(i=1; i<m_EffectManager.PreEffects().Count(); i++ ) {
			pEffect = m_EffectManager.PreEffects().GetRef(i);
			if( pEffect ) {
				if( pEffect->Activated() ) {
					pre_effect_on = true;
					RenderMan->BeginDrawEffect();
					pEffect->Draw(&m_ConsoleState.tempo);
					RenderMan->EndDrawEffect();
					borrar = MO_DEACTIVATED;
				}
			}
		}

		if(borrar==MO_ACTIVATED)
		{
			if( m_EffectManager.PreEffects().Count()>0 ) {
				pEffect = m_EffectManager.PreEffects().GetRef(0);
				if( pEffect ) {
					if( pEffect->Activated() )
					{
						if (RenderMan) RenderMan->BeginDrawEffect();
						pEffect->Draw(&m_ConsoleState.tempo);
						if (RenderMan) RenderMan->EndDrawEffect();
					}
					else
						if (!pre_effect_on && !RenderMan->IsRenderToFBOEnabled() && !RenderMan->RenderResEqualScreenRes())
							// All the preeffects are disabled, including erase. And the screen resolution is different from
							// the render resolution. So the last screen image has to be redrawn at render resolution.
							RenderMan->DrawTexture(MO_RENDER_RESOLUTION, MO_FINAL_TEX);

				}
			}
		}

    ///3D STEREOSCOPIC RENDER METHOD
    if (m_ConsoleState.stereooutput==MO_ACTIVATED) {
        ///Dibujamos los efectos con capacidad stereo

        ///ojo izquierdo
        for( i=0; i<m_EffectManager.Effects().Count(); i++ ) {

            pEffect = m_EffectManager.Effects().GetRef(i);

            if(pEffect) {
                moEffectState fxstate = pEffect->GetEffectState();
                if (fxstate.stereo==MO_ACTIVATED) {

                    fxstate.stereoside = MO_STEREO_LEFT;
                    pEffect->SetEffectState( fxstate );

                    if(pEffect->Activated()) {
                            RenderMan->BeginDrawEffect();
                            pEffect->Draw(&m_ConsoleState.tempo);
                            RenderMan->EndDrawEffect();
                    }

                }
            }
        }

        RenderMan->CopyRenderToTexture(MO_LEFT_TEX);

        ///ojo derecho
        for( i=0; i<m_EffectManager.Effects().Count(); i++ ) {

            pEffect = m_EffectManager.Effects().GetRef(i);

            if(pEffect) {
                moEffectState fxstate = pEffect->GetEffectState();
                if (fxstate.stereo==MO_ACTIVATED) {

                    fxstate.stereoside = MO_STEREO_RIGHT;
                    pEffect->SetEffectState( fxstate );

                    if(pEffect->Activated()) {
                            RenderMan->BeginDrawEffect();
                            pEffect->Draw(&m_ConsoleState.tempo);
                            RenderMan->EndDrawEffect();
                    }

                }
            }
        }

        RenderMan->CopyRenderToTexture(MO_RIGHT_TEX);

    } else {
        ///NORMAL METHOD

        //Se dibujan los Effects
        for( i=0; i<m_EffectManager.Effects().Count(); i++ ) {

            pEffect = m_EffectManager.Effects().GetRef(i);
            if(pEffect) {
                if(pEffect->Activated()) {
                        RenderMan->BeginDrawEffect();
                        pEffect->Draw(&m_ConsoleState.tempo);
                        RenderMan->EndDrawEffect();
                        //RenderMan->CopyRenderToTexture(MO_EFFECTS_TEX);
                }
            }
        }

        RenderMan->CopyRenderToTexture(MO_EFFECTS_TEX);
    }

		//sedibujan los post Effects
		for(i=0;i<m_EffectManager.PostEffects().Count();i++) {

			pEffect = m_EffectManager.PostEffects().GetRef(i);

			if(pEffect) {
				if(pEffect->Activated()) {
					RenderMan->BeginDrawEffect();
					pEffect->Draw(&m_ConsoleState.tempo);
					RenderMan->EndDrawEffect();
				}
			}
		}


		if (RenderMan) RenderMan->CopyRenderToTexture(MO_FINAL_TEX);

//se dibujan los Effects masters (en funcion del modo...) si tiene una sola pantalla... o dos pantallas...
// 1 sola pantalla:
// si esta extendida puede usar un canvas que cubra todo, y solo dibuja la interface en la primera y el resultado en la segunda...
// aqui ganamos velocidad!!!! que perdemos
    if (1==1) {
      DrawMasterEffects( RenderMan->RenderWidth(), RenderMan->RenderHeight() );
    }

		if (m_bInitialized) ScriptExeDraw();


		//aca controlamos los fps
		if( m_ConsoleState.setfps == MO_ACTIVATED ) {
			m_ConsoleState.fps1 = GetTicks();
			while(( m_ConsoleState.fps1 - m_ConsoleState.fps0 ) <(1000 / m_ConsoleState.fps) ) {
				m_ConsoleState.fps1 = GetTicks();
			}
		}


		if (RenderMan) RenderMan->EndDraw();

		if (RenderMan && ( RenderMan->IsRenderToFBOEnabled() || !RenderMan->RenderResEqualScreenRes())  )
			RenderMan->DrawTexture(MO_SCREEN_RESOLUTION, MO_FINAL_TEX);

		this->GLSwapBuffers();

    RenderMan->PreviewShot(false);
	}

	m_ConsoleState.fps0 = GetTicks();
}


void
moConsole::DrawMasterEffects(int interface_width, int interface_height) {

  MOuint i;
	moEffect* pEffect = NULL;
	moText savename, framesavename;
	moRenderManager* RenderMan = m_pResourceManager->GetRenderMan();

  GUIYield();


	if (RenderMan==NULL) return;

  if (interface_width!=0 && interface_height!=0) RenderMan->SetInterfaceView( interface_width, interface_height);

		for(i=0;i<m_EffectManager.MasterEffects().Count();i++) {

			pEffect = m_EffectManager.MasterEffects().GetRef(i);
			if(pEffect) {
				if(pEffect->Activated()) {
					RenderMan->BeginDrawEffect();
					pEffect->Draw( &m_ConsoleState.tempo );
					RenderMan->EndDrawEffect();
				}
			}
		}

}


MOboolean
moConsole::Finish() {

  m_MoldeoObjects.Finish();

	StopMasterEffects();

	FinalizeAllEffects();

    UnloadResources();
	UnloadIODevices();
	UnloadEffects();
	UnloadPreEffects();
	UnloadPostEffects();
	UnloadMasterEffects();

	m_EffectManager.Finish();

	if (m_pResourceManager) {
		m_pResourceManager->Finish();
		if (!m_bExternalResources) {
			delete m_pResourceManager;
			m_pResourceManager = NULL;
		}
		m_pResourceManager = NULL;
	}

	if (m_bIODeviceManagerDefault) {
		if (m_pIODeviceManager) {
			m_pIODeviceManager->Finish();
			delete m_pIODeviceManager;
			m_pIODeviceManager = NULL;
		}
	}

	//FINALIZAMOS LOS RECURSOS (liberando memoria)
	m_ConsoleState.Finish();
	m_Config.DeleteConfig();//finally we unload the console configuration

	m_bInitialized = false;
	m_bConnectorsLoaded = false;

	return true;
}


int
moConsole::Interaction() {

	MOuint i;
	moEffect*	pEffect = NULL;
	moEffect* pPanel = NULL;
	moEffect* pChannel = NULL;
	if (!m_pResourceManager) return -1;
	moRenderManager* RenderMan = m_pResourceManager->GetRenderMan();

  if (!RenderMan) {
    cout << "no renderman!" << endl;
    return -1;
  }
/*
  timecodeticks = moGetTicks();
  minutes = timecodeticks / (1000*60);
  seconds = (timecodeticks - minutes*1000*60) / 1000;
  frames = (timecodeticks - minutes*1000*60 - seconds*1000 ) * 25 / 1000;

  Texto+= moText(" ticks: ") + (moText)IntToStr(timecodeticks,10) +
          moText(" ang: ") + (moText)FloatToStr((float)tempogral->ang) +
          moText(" timecode: ") + (moText)IntToStr(minutes, 2) + moText(":") + (moText)IntToStr(seconds,2) + moText(":") + (moText)IntToStr(frames,2);
*/


  ticksprevious = ticks;
	ticks = moGetTicksAbsolute();
	tickselapsed = ticks - ticksprevious;

	fps_current = 1000.0 / tickselapsed;
	fps_mean += fps_current;

	fps_count++;
	if (fps_count % 10 == 0)
	{
		fps_mean /= 10;
		fps_text = moText("FPS = ") + (moText)FloatToStr(fps_mean);
		fps_mean = 0;
		fps_count = 0;
	}


	//_IODEVICE ACTUALIZA
	RenderMan->BeginUpdate();
	if(m_pIODeviceManager!=NULL) {
		RenderMan->BeginUpdateDevice();

		m_pIODeviceManager->Update();
		moEvent* event = m_pIODeviceManager->GetEvents()->First;

		while(event!=NULL) {
      //MODebug2->Message("moConsole::Interaction > event present.");
      if ( event->deviceid == MO_IODEVICE_KEYBOARD ) {
              //MODebug2->Message("moConsole::Interaction > KEYBOARD activity (devicecode:" + IntToStr(event->devicecode) + " SDL_KEYDOWN:" + IntToStr(SDL_KEYDOWN));
              if (event->devicecode == SDL_KEYDOWN || event->devicecode == SDL_KEYDOWN_SDL2 ) {
                  MODebug2->Message("moConsole::Interaction > KEY DOWN (SDL_KEYDOWN)");
                  if ( event->reservedvalue0 == SDLK_ESCAPE ) {
                      MODebug2->Message("moConsole::Interaction > ESCAPE pressed");
                      //m_ConsoleState.finish = MO_TRUE;
                  }

                  if ( event->reservedvalue0 == SDLK_F12 ) {
                    //recreate window...
                    MODebug2->Message("moConsole::Interaction > F12 pressed");
                    ///m_ConsoleState.fullscreen ???
                  }
              }
          }
       if ( event->deviceid == MO_IODEVICE_MOUSE ) {
        //MODebug2->Message("moConsole::Interaction > Mouse device working");
       }
      event = event->next;
    }


		RenderMan->EndUpdateDevice();
	}
	RenderMan->EndUpdate();

	//INTERACCION EFFECTS MAESTROS

	for( i=0; i<m_EffectManager.MasterEffects().Count(); i++) {
		pEffect = m_EffectManager.MasterEffects().GetRef(i);
		moEffectState fxstate = pEffect->GetEffectState();
		if(pEffect!=NULL) {
			if(pEffect->Activated()) {
				pEffect->Interaction( m_pIODeviceManager );
			}
		}
	}


	///INTERACCION EFFECTS
	///TODO, cambiar el indice fijo por dinamico....

	///channel0 y panel deben estar deshabilitados(las keys asociadas a ellos quedan liberadas)
		//se pasa el control de events al CanalMaestro array[0], el sabra a quien pasar el control
	if ( m_EffectManager.MasterEffects().Count() > 2 ) {

		pChannel = m_EffectManager.MasterEffects().GetRef(0);
		//pChannel = m_EffectManager.GetEffectByLabel( "channel0" );
		pPanel = m_EffectManager.MasterEffects().GetRef(2);
		//pPanel = m_EffectManager.GetEffectByLabel( "panel" );

		if (pPanel && pChannel)
		if( !pChannel->Activated() && !pPanel->Activated() ) {
		    /// signifa que el canal y el panel estan desactivados....
		    /// modo de captura....   ( control del efecto seleccionado por el channel... )
			pChannel->Interaction( m_pIODeviceManager );
		}
	} else {
      ///MODO DIRECTO!!!!! todos los efectos prendidos reciben Interaccion
	    for( MOuint all=0;all<m_EffectManager.AllEffects().Count(); all++) {
	        pEffect = m_EffectManager.AllEffects().GetRef(all);
	        if ( pEffect && pEffect->Activated() ) {
                pEffect->Interaction( m_pIODeviceManager );
	        }
      }
  }


	//TAREAS ESPECIALES
	//reinicializa
	if(m_ConsoleState.reinit) {
		Finish();
		Init();
		m_ConsoleState.reinit = MO_FALSE;
	}


	return (m_ConsoleState.finish==MO_FALSE);
}

int moConsole::SendMoldeoAPIMessage( moDataMessage* p_pDataMessage ) {

  ///TODO: SendMoldeoAPIMessage > add this message to MoldeoAPIMessagesToSend
  /// MoldeoAPIMessagesToSend.Add( *p_pDataMessage );
  /*
  if (ApiMessage) {
      ApiMessage->Text();
  }
  */
  //MODebug2->Message( "moConsole::SendMoldeoAPIMessage > " );

  m_pIODeviceManager->GetEvents()->Add( MO_IODEVICE_CONSOLE,
                                        MO_ACTION_MOLDEOAPI_EVENT_SEND,
                                        -1, 0, 0, MO_DATAMESSAGE, p_pDataMessage );


  return 0;
}


#include <sstream>
#include <iostream>

int moConsole::ProcessMoldeoAPIMessage( moDataMessage* p_pDataMessage ) {

  if (p_pDataMessage==NULL) {
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > DataMessage is NULL");
      return 1;
  }

  if (p_pDataMessage->Count()==0) {
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > DataMessage is empty");
      return 2;
  }
  moData MoldeoApiCommandData = p_pDataMessage->Get(0);
  moText MoldeoAPICommand = p_pDataMessage->Get(0).ToText();
  std::string skey = (char*)MoldeoAPICommand;

  moMoldeoActionType MappedType = (moMoldeoActionType)m_ReactionListenerManager.m_MapStrToActionType[skey];

/**/
/*
  moText fullMessageText = "";
  for( int k=0;k<(int)p_pDataMessage->Count();k++) {
    fullMessageText+= " k:" + IntToStr(k);
    fullMessageText+="[" + p_pDataMessage->Get(k).ToText()+"]";
  }
  */
/*
  MODebug2->Message( moText("moConsole::ProcessMoldeoAPIMessage > Processing Moldeo API Message: ")
                    + MoldeoAPICommand
                    + moText(" count:") + IntToStr(p_pDataMessage->Count())
                    + moText(" mapped type:") +IntToStr(MappedType)
                    + moText(" fullmessage:") + fullMessageText  );
*/
/**/

  moEffect* fxObject;
  moMoldeoObject* MObject;
  moText arg0;
  int arg1Int;
  moText arg1Text;
  int arg2Int;
  moText arg2Text;
  moText arg3Text;

  moText argRed,argGreen,argBlue,argAlpha;
  unsigned int r,g,b;

  moText EffectStateJSON = "";
  moText FullObjectJSON = "";
  moText fieldSeparation = ",";

  moMobState MobState;
  moEffectState EffectState;
  moDataMessage* pMessageToSend;
  moData pData("PRESENTATION");
  moMessage* newMessage = NULL;

  switch( MappedType ) {


    // VALUES

    case MO_ACTION_VALUE_ADD:
    case MO_ACTION_VALUE_DELETE:
    case MO_ACTION_VALUE_SET:
    case MO_ACTION_VALUE_REFRESH:
    case MO_ACTION_VALUE_GET:
    case MO_ACTION_VALUE_SAVE:
{
      //MODebug2->Message("MO_ACTION_VALUE_SET");
      arg0  = p_pDataMessage->Get(1).ToText();//MOBLABEL
      MODebug2->Message(arg0);
      //fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      MObject = GetObjectByIdx( GetObjectId( arg0 ) );
      moConfig* pConfig = NULL;
      if (MObject) {
         pConfig = MObject->GetConfig();
        //MODebug2->Message("MO_ACTION_VALUE_SET fxObject ok");
        arg1Text  = p_pDataMessage->Get(2).ToText();//PARAMNAME or ID
      }
      else {
        MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > fxObject not found for " + arg0 );
        return -1;
      }
      if (MappedType==MO_ACTION_VALUE_SAVE) {
        //guardar ese valor en el config!!!
        // guardamos el config completo mejor...
        MObject->GetConfig()->SaveConfig();
        return -1;
      }

      if (MappedType==MO_ACTION_VALUE_SET
          ||
          MappedType==MO_ACTION_VALUE_ADD
          ||
          MappedType==MO_ACTION_VALUE_DELETE
          ||
          MappedType==MO_ACTION_VALUE_REFRESH
          )
        arg2Text  = p_pDataMessage->Get(3).ToText();//PARAMVALUEINDEX ///si es 0:1 > indice 0, subindice 1...

      int subvalue = -1;
      bool issubvalue = false;
      if (arg1Text=="color_0") { arg1Text = "color"; subvalue=0;issubvalue=true; }
      if (arg1Text=="color_1") { arg1Text = "color"; subvalue=1;issubvalue=true; }
      if (arg1Text=="color_2") { arg1Text = "color"; subvalue=2;issubvalue=true; }
      if (arg1Text=="color_3") { arg1Text = "color"; subvalue=3;issubvalue=true; }
      if (arg1Text=="particlecolor_0") { arg1Text = "particlecolor"; subvalue=0;issubvalue=true; }
      if (arg1Text=="particlecolor_1") { arg1Text = "particlecolor"; subvalue=1;issubvalue=true; }
      if (arg1Text=="particlecolor_2") { arg1Text = "particlecolor"; subvalue=2;issubvalue=true; }
      if (arg1Text=="particlecolor_3") { arg1Text = "particlecolor"; subvalue=3;issubvalue=true; }
      MODebug2->Message(moText("Search Param:")+pConfig->GetParam(arg1Text).GetParamDefinition().GetName());
      if (pConfig->GetParam(arg1Text).GetParamDefinition().GetName()==arg1Text) {
        MODebug2->Message(moText("Search Param not found"));
      } else {
        //check if last char is a Number
        MODebug2->Message(arg1Text+" L:"+IntToStr(arg1Text.Length()));
        if (arg1Text.Length()>3) {
          moText colorparam = arg1Text;
          colorparam.Left(arg1Text.Length()-3);// [paramcolorname][_0]
          MODebug2->Message(colorparam);
          MODebug2->Message(pConfig->GetParam(colorparam).GetParamDefinition().GetName());
          if ( pConfig->GetParam(colorparam).GetParamDefinition().GetName() == colorparam ) {
            moText tsubv = arg1Text;
            MODebug2->Message(tsubv);
            tsubv.Right(1);//only last char
            MODebug2->Message(tsubv);
            arg1Text = colorparam;
            MODebug2->Message(arg1Text);
            if (isdigit(tsubv[0])) {
              subvalue = StrToInt(tsubv);
              issubvalue=true;
              MODebug2->Message(IntToStr(subvalue));
            }
          }
        }

      }

      moParam& rParam( pConfig->GetParam(arg1Text));


      arg2Int = atoi( arg2Text );

      ///Add Values!
      if ( MappedType==MO_ACTION_VALUE_ADD && (int)rParam.GetValuesCount()<=arg2Int && pConfig ) {

        MODebug2->Message("moConsole::ProcessMoldeoAPIMessage Adding > value for "+MObject->GetLabelName()+" preconfig: "+arg2Text);
        moText pName = rParam.GetParamDefinition().GetName();
        moParamDefinition pParamDef = pConfig->GetConfigDefinition()->GetParamDefinition( pName );
        //moValue newValue = pParamDef.GetDefaultValue();
        moValue newValue = rParam.GetValue( rParam.GetValuesCount()-1 );

        /// crea valores hasta completar el indice (arg2Int) (luego esperamos se rellenen
        /// con valores reales definidos por el usuario.
        /// si alguien pide el valor de la posicion xxx
        /// y xxx es mas grande que la cantidad de valores:
        ///   se crean todos los valores
        int valcount = arg2Int - rParam.GetValuesCount() + 1;
        for( int sum=0; sum < valcount ; sum++ ) {
            MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > Adding Value at index:" + IntToStr( rParam.GetValuesCount() )
                              + " newvalue: " + newValue.ToJSON() );
            rParam.AddValue( newValue );
            SetValue( MObject->GetId(), rParam.GetParamDefinition().GetIndex(), rParam.GetValuesCount()-1, newValue );
        }
        arg3Text = newValue.GetSubValue(0).ToText();

        /**
        *
        * FULL PSEUDO CODE
        *

        si ya existe una preconfiguraci�n (arg2Int) .
            si no existe el parametro en esa preconfiguracion (apuntando al arg2Int)
                lo agrega a esa preconfiguracion
            en cambio si ya existe lo apunta a este nuevo indice arg2Int

        sino genero una preconfiguracion nueva
            recorro los parametros de este objeto y agrego como parametro todo los valores que tengan mas de un valor
                (la idea es que si hay mas de un valor, entonces susceptiblemente es un parametro que es dinamico desde el punto de vista del usuario)
            el indice de este parametro es el mas cercano a arg2Int o directamente el arg2Int esperando que el preconfig entienda que debera seleccionar el mas cercano
                (o sea si hay 2 valores y el preconfig es el 3, automaticamente definira <P name="alpha">2</P>)
        */

        if (pConfig==NULL) return -1;
        moPreConfig preCfg;
        bool param_exist = false;
        int ip=0;


        ///Adding to preconfig (available)
        if (  arg2Int < pConfig->GetPreConfCount() ) {
          /// si ya existe una preconfiguraci�n correlativa a ese indice (arg2Int) .

          for(int k=0; k < pConfig->GetPreConfCount(); k++ ) {
            preCfg = pConfig->GetPreconfig( k );

            /// ADD MISSING PARAMETERS THAT HAVE BEEN
            /// CUSTOMIZED (so they have values count > 1)
            for( int pi=0; pi < pConfig->GetParamsCount(); pi++) {
                moParam& rParamx( pConfig->GetParam(pi) );

                if ( rParamx.GetValuesCount()>1 /** only add customized*/) {
                     moPreconfigParamIndex preIndexA;
                     for( ip=0, param_exist = false; ip< (int)preCfg.m_PreconfIndexes.Count(); ip++ ) {
                        preIndexA = preCfg.m_PreconfIndexes.Get(ip);
                        if (preIndexA.m_ParamName == rParamx.GetParamDefinition().GetName() ) {
                          param_exist = true;
                          break;
                        }
                      }

                      /// SI existe
                      if (param_exist) {
                        ///lo referenciamos a esta nueva posicion creada...
                        preIndexA.m_ValueIndex = k;
                        preCfg.m_PreconfIndexes.Set( ip, preIndexA );
                      } else {
                        preIndexA.m_ParamName = rParamx.GetParamDefinition().GetName();
                        preIndexA.m_ParamIndex = rParamx.GetParamDefinition().GetIndex();
                        preIndexA.m_ValueIndex = momin( rParamx.GetValuesCount()-1, k );
                        preCfg.m_PreconfIndexes.Add( preIndexA );
                      }

                }
            }

            pConfig->SetPreconfig( k, preCfg.m_PreconfIndexes );

          }

        } else {

        ///Creating preconfigs to make (arg2int) available

          ///se incluye todos los parametros que tiene al menos un valor en esta preconfiguracion... (en ese indice)
          MODebug2->Message("moConsole::ProcessMoldeoAPIMessage > Adding Value > a new value");
          if (arg2Int<64) {
            pConfig->AddPreconfig( arg2Int );
          } else {
            MODebug2->Error("Too many preconfigs! Warning or Error: " + IntToStr(arg2Int) );
          }

        }
        /**
        *
        *
        */
        ///Set and Notify value
        MappedType = MO_ACTION_VALUE_SET;

      }

      moValue& rValue( rParam.GetValue(arg2Int) );

      if (MappedType==MO_ACTION_VALUE_DELETE) {
          rParam.DeleteValue( arg2Int );
          return 0;
      }
      if (subvalue==-1) subvalue = 0;
      moValueBase& VB( rValue.GetSubValue(subvalue) );

      if (MappedType==MO_ACTION_VALUE_SET && p_pDataMessage->Count()>=5) {
        arg3Text  = p_pDataMessage->Get(4).ToText();//VALUE
      } else {
        arg3Text = VB.ToText();
      }

      MODebug2->Message("MO_ACTION_VALUE_SET settings: arg1Text (param):" + arg1Text
                        + " arg2Text (preconf): ["+arg2Text+"] arg2Int: "+ IntToStr(arg2Int)
                        + " arg3Text (val): " + arg3Text
                        + " VB( subvalue: "+IntToStr(subvalue)+" ): " + VB.TypeToText() + ")"
                        );

      if (rParam.GetParamDefinition().GetType()==MO_PARAM_COLOR && issubvalue==false ) {

        argRed = arg3Text;
        argGreen = arg3Text;
        argBlue = arg3Text;
        MODebug2->Message("color");

        if (arg3Text.Left(0)=="#") {
          argRed.Mid(1,2);
          argGreen.Mid(3,2);
          argBlue.Mid(5,2);

          r = HexToInt( argRed );
          g = HexToInt( argGreen );
          b = HexToInt( argBlue );
          //MODebug2->Message("color: red: " + argRed + " green:" + argGreen + " blue:" + argBlue);
          rValue.GetSubValue(0).SetFun( FloatToStr((float)r*1.0/255.0) );
          rValue.GetSubValue(1).SetFun( FloatToStr((float)g*1.0/255.0) );
          rValue.GetSubValue(2).SetFun( FloatToStr((float)b*1.0/255.0) );
          //MODebug2->Message("color: red: " + IntToStr(r) + " green:" + IntToStr(g) + " blue:" + IntToStr(b));
        }
      }
      else { switch(VB.Type()) {
          case MO_DATA_FUNCTION:
            VB.SetFun( arg3Text );
            break;
          case MO_DATA_TEXT:
          case MO_DATA_SOUNDSAMPLE:
          case MO_DATA_IMAGESAMPLE:
          case MO_DATA_IMAGESAMPLE_FILTERED:
          case MO_DATA_IMAGESAMPLE_TEXTUREBUFFER:

            switch(rParam.GetParamDefinition().GetType()) {
              case MO_PARAM_TEXT:
              case MO_PARAM_SCRIPT:
              case MO_PARAM_FILE:
              case MO_PARAM_TEXTUREFOLDER:
                VB.SetText(arg3Text);
                break;
              case MO_PARAM_TEXTURE:
                if (m_pResourceManager->GetDataMan()->InData(arg3Text)) {
                  //make relative to datapath
                  moText relpath = m_pResourceManager->GetDataMan()->MakeRelativeToData(arg3Text);
                  if (relpath!="") {
                     arg3Text = relpath;
                  }
                } else {
                  //try to import file
                  moFile importFile( arg3Text );
                  m_pResourceManager->GetDataMan()->ImportFile( importFile.GetAbsolutePath() );
                  arg3Text = importFile.GetFullName();
                }
                VB.SetText(arg3Text);
                break;
              case MO_PARAM_SOUND:
                {
                  if (m_pResourceManager->GetDataMan()->InData(arg3Text)) {
                    //make relative to datapath
                    arg3Text = m_pResourceManager->GetDataMan()->MakeRelativeToData(arg3Text);
                  } else {
                    //try to import file
                    moFile importFile( arg3Text );
                    m_pResourceManager->GetDataMan()->ImportFile( importFile.GetAbsolutePath() );
                    arg3Text = importFile.GetFullName();
                  }
                  VB.SetText(arg3Text);
                }
                break;
              default:
                MODebug2->Warning("no param type implemented:"+rParam.GetParamDefinition().GetTypeStr() );
                break;
            }
            break;
          case MO_DATA_NUMBER:
            VB.SetInt( (int)atoi(arg3Text) );
            break;
          case MO_DATA_NUMBER_INT:
            VB.SetInt( (int)atoi(arg3Text) );
            break;
          case MO_DATA_NUMBER_FLOAT:
            VB.SetFloat( (float)atof(arg3Text) );
            break;
          case MO_DATA_NUMBER_DOUBLE:
            VB.SetDouble( (double)atof(arg3Text) );
            break;
          case MO_DATA_NUMBER_CHAR:
            VB.SetInt( (char)atoi(arg3Text) );
            break;
          case MO_DATA_NUMBER_LONG:
            VB.SetLong( (long)atoi(arg3Text) );
            break;
          default:
            break;
        }
      }

      moValue fullValueToCopy = rValue;

      if (MappedType==MO_ACTION_VALUE_REFRESH) {
        RefreshValue( MObject->GetId(), rParam.GetParamDefinition().GetIndex(), arg2Int, true );
        MappedType = MO_ACTION_VALUE_GET;
      } else {
        SetValue( MObject->GetId(), rParam.GetParamDefinition().GetIndex(), arg2Int, fullValueToCopy );
      }

      MappedType = MO_ACTION_VALUE_GET;

      if (MappedType==MO_ACTION_VALUE_GET) {

        moText fullValueJSON = rValue.ToJSON();
        MODebug2->Message("valueget " + fullValueJSON );
        pMessageToSend = new moDataMessage();
        if (pMessageToSend) {
            pMessageToSend->Add( moData("valueget") );
            pMessageToSend->Add( moData( arg0 ) );//mob
            pMessageToSend->Add( moData( arg1Text ) );//param
            pMessageToSend->Add( moData( arg2Text ) );//preconf
            pMessageToSend->Add( moData( fullValueJSON ) );
            SendMoldeoAPIMessage( pMessageToSend );
        }

      }

      return 0;
}
      break;

    // OBJECTS

    case MO_ACTION_OBJECT_ENABLE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();

      //buscar este efecto y prenderlo...
      MObject = GetObjectByIdx(GetObjectId( arg0 ));
      if (MObject) {
          ObjectEnable( MObject->GetId() );

          /** SEND IT UPDATED!!!*/
          MoldeoApiCommandData.SetText( "objectgetstate" );
          p_pDataMessage->Set( 0, MoldeoApiCommandData );
          p_pDataMessage->Set( 2, moData("") );
          ProcessMoldeoAPIMessage( p_pDataMessage );
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_ENABLE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_OBJECT_DISABLE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();

      //buscar este efecto y prenderlo...
      MObject = GetObjectByIdx(GetObjectId( arg0 ));
      if (MObject) {
          ObjectDisable( MObject->GetId() );

          /** SEND IT UPDATED!!!*/
          MoldeoApiCommandData.SetText( "objectgetstate" );
          p_pDataMessage->Set( 0, MoldeoApiCommandData );
          p_pDataMessage->Set( 2, moData("") );
          ProcessMoldeoAPIMessage( p_pDataMessage );
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_ENABLE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_OBJECT_GETSTATE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      MObject = GetObjectByIdx( GetObjectId( arg0 ) );
      if (MObject) {
          moMobState ObjectState = MObject->GetState();
          moText ObjectStateJSON = ObjectState.ToJSON();

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("objectgetstate") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( ObjectStateJSON ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + ObjectStateJSON );
              /** send it: but we need an id */
              SendMoldeoAPIMessage( pMessageToSend );
          }

          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_GETSTATE > [" + arg0+"] not found!" );
      }
      break;

    // EFFECTS

    case MO_ACTION_EFFECT_ENABLE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();

      //buscar este efecto y prenderlo...
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
          ObjectEnable( fxObject->GetId() );

          /** SEND IT UPDATED!!!*/
          MoldeoApiCommandData.SetText( "effectgetstate" );
          p_pDataMessage->Set( 0, MoldeoApiCommandData );
          p_pDataMessage->Set( 2, moData("") );
          ProcessMoldeoAPIMessage( p_pDataMessage );
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_ENABLE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_EFFECT_DISABLE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();

      //buscar este efecto y prenderlo...
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
          ObjectDisable( fxObject->GetId() );

          /** SEND IT UPDATED!!!*/
          MoldeoApiCommandData.SetText( "effectgetstate" );
          p_pDataMessage->Set( 0, MoldeoApiCommandData );
          p_pDataMessage->Set( 2, moData("") );
          ProcessMoldeoAPIMessage( p_pDataMessage );
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_DISABLE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_EFFECT_GETSTATE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
          EffectState = fxObject->GetEffectState();
          EffectStateJSON = EffectState.ToJSON();

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("effectgetstate") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( EffectStateJSON ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
              /** send it: but we need an id */
              SendMoldeoAPIMessage( pMessageToSend );
          }

          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_GETSTATE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_EFFECT_SETSTATE:
      {
      if (p_pDataMessage->Count()<4) {
        return -1;
      }
      arg0  = p_pDataMessage->Get(1).ToText();
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
        arg1Text  = p_pDataMessage->Get(2).ToText();
        arg2Text  = p_pDataMessage->Get(3).ToText();

        if ( arg1Text == moText("alpha") ) {
            if ( arg2Text == moText("increment") ) {

              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > alpha increment" );
              fxObject->Alpha( 0.01 );

            } else if ( arg2Text == moText("decrement") ) {

              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > alpha decrement" );
              fxObject->Alpha( -0.01 );

            } else if( p_pDataMessage->Get(3).Type() != MO_DATA_TEXT ) {
              EffectState = fxObject->GetEffectState();
              EffectState.alpha = p_pDataMessage->Get(3).Float();
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > EffectState updating to: " + FloatToStr(EffectState.alpha) );
              fxObject->SetEffectState( EffectState );
              EffectState = fxObject->GetEffectState();
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > EffectState updated: " + FloatToStr(EffectState.alpha) );
            }
        }

        if ( arg1Text == moText("tempo") ) {
            if ( arg2Text == moText("increment") ) {
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > tempo increment" );
              fxObject->TempoDelta( 0.01 );
            } else if ( arg2Text == moText("decrement") ) {
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > tempo decrement" );
              fxObject->TempoDelta( -0.01 );
            } else if ( arg2Text == moText("beatpulse") ) {
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > tempo beatpulse" );
              fxObject->BeatPulse();
            } else if( p_pDataMessage->Get(3).Type() != MO_DATA_TEXT ) {
              fxObject->SetTempoDelta( p_pDataMessage->Get(3).Float() );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > EffectState updating tempo.delta to: " + FloatToStr(fxObject->GetEffectState().tempo.delta) );
            }
        }
        EffectState = fxObject->GetEffectState();
        SetEffectState( fxObject->GetId(), EffectState );

        /** SEND IT UPDATED!!!*/
        MoldeoApiCommandData.SetText( "effectgetstate" );
        p_pDataMessage->Set( 0, MoldeoApiCommandData );
        p_pDataMessage->Set( 2, moData("") );
        ProcessMoldeoAPIMessage( p_pDataMessage );
        return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_SETSTATE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_EFFECT_PLAY:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
          EffectPlay(fxObject->GetId());
          EffectState = fxObject->GetEffectState();
          EffectStateJSON = EffectState.ToJSON();

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("effectgetstate") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( EffectStateJSON ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
              /** send it: but we need an id */
              SendMoldeoAPIMessage( pMessageToSend );
          }
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_PLAY > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_EFFECT_PAUSE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
          EffectPause(fxObject->GetId());
          EffectState = fxObject->GetEffectState();
          EffectStateJSON = EffectState.ToJSON();

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("effectgetstate") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( EffectStateJSON ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
              /** send it: but we need an id */
              SendMoldeoAPIMessage( pMessageToSend );
          }
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_PAUSE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_EFFECT_PLAY_PAUSE:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
          EffectPlayPause(fxObject->GetId());
          EffectState = fxObject->GetEffectState();
          EffectStateJSON = EffectState.ToJSON();

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("effectgetstate") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( EffectStateJSON ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
              /** send it: but we need an id */
              SendMoldeoAPIMessage( pMessageToSend );
          }
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_PLAY_PAUSE > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_EFFECT_STOP:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      fxObject = m_EffectManager.GetEffectByLabel( arg0 );
      if (fxObject) {
          EffectStop(fxObject->GetId());
          EffectState = fxObject->GetEffectState();
          EffectStateJSON = EffectState.ToJSON();

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("effectgetstate") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( EffectStateJSON ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
              /** send it: but we need an id */
              SendMoldeoAPIMessage( pMessageToSend );
          }
          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_EFFECT_STOP > [" + arg0+"] not found!" );
      }
      break;

    /** VALUE SET


    */

    case MO_ACTION_PARAM_SET:
      // code, mob, param, info
      {
        if (p_pDataMessage->Count()<4) {
          return -1;
        }
        arg0  = p_pDataMessage->Get(1).ToText();
        MObject = GetObjectByIdx(GetObjectId(arg0) );
        if (MObject) {
          arg1Text  = p_pDataMessage->Get(2).ToText();//param
          arg2Text  = p_pDataMessage->Get(3).ToText();//attribute
        } else return -1;

        moParam& pparam( MObject->GetConfig()->GetParam( arg1Text ));
        if (arg2Text=="property") {
          arg3Text  = p_pDataMessage->Get(4).ToText();//property value "empty" or "published"
          pparam.GetParamDefinition().SetProperty( arg3Text );
        } else if(arg2Text=="interpolation") {
          moParamInterpolation ParamInter = pparam.GetParamDefinition().GetInterpolation();
          arg3Text  = p_pDataMessage->Get(4).ToText();//linear|easein|easeinout|easout
          pparam.GetParamDefinition().GetInterpolation().SetInterpolationFunction( arg3Text );
        } else if(arg2Text=="duration") {
          arg3Text  = p_pDataMessage->Get(4).ToText();//linear|easein|easeinout|easout
          moParamInterpolation ParamInter = pparam.GetParamDefinition().GetInterpolation();
          pparam.GetParamDefinition().GetInterpolation().SetDuration( atoi(arg3Text) );
        }

        SetParam( MObject->GetId(), pparam.GetParamDefinition().GetIndex(), pparam.GetParamDefinition() );


      /** SEND IT UPDATED!!!*/
      MoldeoApiCommandData.SetText( "paramget" );
      p_pDataMessage->Set( 0, MoldeoApiCommandData );
      p_pDataMessage->Set( 1, moData(arg0) );
      p_pDataMessage->Set( 2, moData(arg1Text) );
      p_pDataMessage->Set( 3, moData("") );
      p_pDataMessage->Set( 4, moData("") );
      ProcessMoldeoAPIMessage( p_pDataMessage );
      }
      break;

    case MO_ACTION_PARAM_GET:
      {
        //MODebug2->Message("MO_ACTION_VALUE_SET");
        arg0  = p_pDataMessage->Get(1).ToText();//MOBLABEL
        MODebug2->Message(arg0);
        MObject = GetObjectByIdx(GetObjectId(arg0) );

        if (MObject) {
          //MODebug2->Message("MO_ACTION_VALUE_SET fxObject ok");
          arg1Text  = p_pDataMessage->Get(2).ToText();//PARAMNAME or ID
        }
        else
          return -1;

        moParam mParam = MObject->GetConfig()->GetParam(arg1Text);

        //
        moText FullParamJSON = mParam.ToJSON();
        MODebug2->Message( FullParamJSON );

        pMessageToSend = new moDataMessage();
        if (pMessageToSend) {
            pMessageToSend->Add( moData("paramget") );
            //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
            pMessageToSend->Add( moData( arg0 ) );
            pMessageToSend->Add( moData( FullParamJSON ) );
            //pMessageToSend->Add( moData( "{'testing': 0}" ) );
            //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
            // send it: but we need an id
            SendMoldeoAPIMessage( pMessageToSend );
        }
      }
      break;


    /** PRECONFIGS

    */

    case MO_ACTION_PRECONFIG_SET:
      {
        arg0  = p_pDataMessage->Get(1).ToText();
        arg1Int = p_pDataMessage->Get(2).Int();

        MObject = GetObjectByIdx(GetObjectId(arg0) );
        if (MObject) {
            //fxObject->GetConfig()->SetCurrentPreConf( arg1Int );
            if ( 0 <= arg1Int && arg1Int<MObject->GetConfig()->GetPreConfCount()) {
              this->SetPreconf( MObject->GetId(), arg1Int );
            } else {
              MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > MO_ACTION_PRECONFIG_SET > preconfig index [" + IntToStr(arg1Int)+"] not found!" );
            }
            return 0;
        }
        MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > MO_ACTION_PRECONFIG_SET > Moldeo Object [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_PRECONFIG_ADD:
      {
        arg0  = p_pDataMessage->Get(1).ToText();//label object
        arg1Int = p_pDataMessage->Get(2).Int();//preconfig index

        MObject = GetObjectByIdx(GetObjectId(arg0) );
        if (MObject) {
            //fxObject->GetConfig()->SetCurrentPreConf( arg1Int );
            if ( 0 <= arg1Int && arg1Int<MObject->GetConfig()->GetPreConfCount()) {
              //this->SetPreconf( fxObject->GetId(), arg1Int );
              //do nothing
            } else {
              MObject->GetConfig()->AddPreconfig( arg1Int );
              FullObjectJSON = MObject->ToJSON();
              MODebug2->Message(FullObjectJSON);

              pMessageToSend = new moDataMessage();
              if (pMessageToSend) {
                  pMessageToSend->Add( moData("objectget") );
                  //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
                  pMessageToSend->Add( moData( arg0 ) );
                  pMessageToSend->Add( moData( FullObjectJSON ) );
                  //pMessageToSend->Add( moData( "{'testing': 0}" ) );
                  //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
                  // send it: but we need an id
                  SendMoldeoAPIMessage( pMessageToSend );
              }
            }
            return 0;
        }
        MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > MO_ACTION_PRECONFIG_SET > Moldeo Object [" + arg0+"] not found!" );
      }
      break;

  /** OBJECTS

  */

    case MO_ACTION_OBJECT_ADD:
    case MO_ACTION_OBJECT_SET:
      {
          /// adding an object
          moMobDefinition dfNewMob;
          dfNewMob.SetName( p_pDataMessage->Get(1).ToText() );
          dfNewMob.SetLabelName( p_pDataMessage->Get(2).ToText() );
          dfNewMob.SetConfigName( p_pDataMessage->Get(3).ToText() );
          dfNewMob.SetType( (moMoldeoObjectType) p_pDataMessage->Get(4).Int() );
          dfNewMob.SetKeyName( p_pDataMessage->Get(5).ToText() );
          dfNewMob.SetActivate( p_pDataMessage->Get(6).Int() );
          dfNewMob.SetFatherLabelName( p_pDataMessage->Get(7).ToText() );
          dfNewMob.SetMoldeoId( p_pDataMessage->Get(8).Int() );

          if (MappedType==MO_ACTION_OBJECT_ADD) MODebug2->Message("objectadd");
          MODebug2->Message("from:");
          for( int k=0; k<p_pDataMessage->Count(); k++) {
            MODebug2->Message( p_pDataMessage->Get(k).ToText() );
          }
          MODebug2->Message("to:");
          MODebug2->Message(dfNewMob.ToJSON());
          int status = -1;
          try {
            if (MappedType==MO_ACTION_OBJECT_ADD) status = this->NewMob( dfNewMob );
            if (MappedType==MO_ACTION_OBJECT_SET) status = this->SetMob( dfNewMob );
          } catch(...) {
            Error("MO_ACTION_OBJECT_ADD or MO_ACTION_OBJECT_SET error");
          }
          if (status!=-1) {
            pMessageToSend = new moDataMessage();
            if (pMessageToSend) {
              pMessageToSend->Add( moData( "consoleget" ) );
              ProcessMoldeoAPIMessage(pMessageToSend);
            }
            return 0;
          } else {
            pMessageToSend = new moDataMessage();
            if (pMessageToSend) {
              pMessageToSend->Add( moData( "consoleerror" ) );
              pMessageToSend->Add( moData( "Error adding or saving new mob." ) );
              pMessageToSend->Add( moData( IntToStr(status) ) );
              ProcessMoldeoAPIMessage(pMessageToSend);
            }
            return 0;
          }

      }
      break;

    case MO_ACTION_OBJECT_MOVE:
      {
        arg0  = p_pDataMessage->Get(1).ToText();//mob label
        arg1Int  = p_pDataMessage->Get(2).Int();//position (-1),(+1), 0..Count (in the type of array)
        arg2Int  = p_pDataMessage->Get(3).Int();//is_relative 0, 1
        MODebug2->Message( moText(" MO_ACTION_OBJECT_MOVE: ")+arg0+moText(",")+IntToStr(arg1Int)+moText(",")+IntToStr(arg2Int) );
        MObject = NULL;
        fxObject = m_EffectManager.GetEffectByLabel( arg0 );
        if (fxObject) {
            //FullObjectJSON = fxObject->ToJSON();
            //MODebug2->Message(FullObjectJSON);
            this->MoveMob( fxObject->GetMobDefinition(), arg1Int, (arg2Int==1) );

        } else {
          int idobj = GetObjectId( arg0 );
          MObject = GetObjectByIdx( idobj );
          //TODO: sometimes for iodevices it's necessary to move the objects
          if (MObject && MObject->GetMobDefinition().GetType()!=MO_OBJECT_IODEVICE
                        && MObject->GetMobDefinition().GetType()!=MO_OBJECT_RESOURCE) {
            //FullObjectJSON = "{ 'object': " + MObject->ToJSON();
            //FullObjectJSON+= "}";
            //MODebug2->Message( FullObjectJSON );
            this->MoveMob( MObject->GetMobDefinition(), arg1Int, (arg2Int==1) );
          }
        }

        if (fxObject || MObject) {
/*
            pMessageToSend = new moDataMessage();
            if (pMessageToSend) {
                pMessageToSend->Add( moData("objectget") );
                //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
                pMessageToSend->Add( moData( arg0 ) );
                pMessageToSend->Add( moData( FullObjectJSON ) );
                //pMessageToSend->Add( moData( "{'testing': 0}" ) );
                //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
                // send it: but we need an id
                SendMoldeoAPIMessage( pMessageToSend );
            }
*/
            pMessageToSend = new moDataMessage();
            if (pMessageToSend) {
              pMessageToSend->Add( moData( "consoleget" ) );
              ProcessMoldeoAPIMessage(pMessageToSend);
            }
            return 0;
        }
        MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_GET > [" + arg0+"] not found!" );
      }
      break;

    case MO_ACTION_OBJECT_DELETE:
      {
        /// DELETE
        arg0  = p_pDataMessage->Get(1).ToText();//mob label
        fxObject = m_EffectManager.GetEffectByLabel( arg0 );
        if (fxObject) {
            //FullObjectJSON = fxObject->ToJSON();
            //MODebug2->Message(FullObjectJSON);
            this->DeleteMob( fxObject->GetMobDefinition() );

        } else {
          int idobj = GetObjectId( arg0 );
          MObject = GetObjectByIdx( idobj );
          //TODO: sometimes for iodevices it's necessary to move the objects
          if (MObject && MObject->GetMobDefinition().GetType()!=MO_OBJECT_IODEVICE
                        && MObject->GetMobDefinition().GetType()!=MO_OBJECT_RESOURCE) {
            this->DeleteMob( MObject->GetMobDefinition() );
          }
        }
        if (fxObject || MObject) {
            pMessageToSend = new moDataMessage();
            if (pMessageToSend) {
              pMessageToSend->Add( moData( "consoleget" ) );
              ProcessMoldeoAPIMessage(pMessageToSend);
            }
            return 0;
        }
      }
      break;

    case MO_ACTION_OBJECT_DUPLICATE:
      {
///
      }
      break;


    case MO_ACTION_OBJECT_GET:
      {
        arg0  = p_pDataMessage->Get(1).ToText();
        MObject = NULL;
        fxObject = m_EffectManager.GetEffectByLabel( arg0 );
        if (fxObject) {
            FullObjectJSON = fxObject->ToJSON();
            MODebug2->Message(FullObjectJSON);
        } else {
          int idobj = GetObjectId( arg0 );
          MObject = GetObjectByIdx( idobj );
          if (MObject) {
            FullObjectJSON = "{ 'object': " + MObject->ToJSON();
            FullObjectJSON+= "}";
            MODebug2->Message( FullObjectJSON );
          }
        }

        if (fxObject || MObject) {
            pMessageToSend = new moDataMessage();
            if (pMessageToSend) {
                pMessageToSend->Add( moData("objectget") );
                //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
                pMessageToSend->Add( moData( arg0 ) );
                pMessageToSend->Add( moData( FullObjectJSON ) );
                //pMessageToSend->Add( moData( "{'testing': 0}" ) );
                //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
                // send it: but we need an id
                SendMoldeoAPIMessage( pMessageToSend );
            }

            return 0;
        }
        MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_GET > [" + arg0+"] not found!" );
      }
      break;

  case MO_ACTION_OBJECT_GETCONFIG:
      {
        arg0  = p_pDataMessage->Get(1).ToText();
        int idobj = GetObjectId( arg0 );
        MObject = GetObjectByIdx( idobj );
        if (MObject) {
            FullObjectJSON = "{ 'objectconfig': " + MObject->GetConfig()->ToJSON();
            FullObjectJSON+= "}";
            MODebug2->Message( FullObjectJSON );

            pMessageToSend = new moDataMessage();
            if (pMessageToSend) {
                pMessageToSend->Add( moData("objectgetconfig") );
                //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
                pMessageToSend->Add( moData( arg0 ) );
                pMessageToSend->Add( moData( FullObjectJSON ) );
                //pMessageToSend->Add( moData( "{'testing': 0}" ) );
                //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
                // send it: but we need an id
                SendMoldeoAPIMessage( pMessageToSend );
            }

            return 0;
        }
        MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_GETCONFIG > [" + arg0+"] not found!" );
      }
      break;

  case MO_ACTION_OBJECT_GETPRECONFIG:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      arg1Int  = p_pDataMessage->Get(2).Int();
      int idobj = GetObjectId( arg0 );
      MObject = GetObjectByIdx( idobj );
      if (MObject) {
          moPreConfig pC = MObject->GetConfig()->GetPreconfig(arg1Int);
          FullObjectJSON = "{ 'preconfig': " + pC.ToJSON();
          FullObjectJSON+=  ", 'position': "+IntToStr(arg1Int)+" }";
          MODebug2->Message( FullObjectJSON );

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("objectgetpreconfig") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( FullObjectJSON ) );
              //pMessageToSend->Add( moData( "{'testing': 0}" ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
              // send it: but we need an id
              SendMoldeoAPIMessage( pMessageToSend );
          }

          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_GETPRECONFIG > [" + arg0+"] not found!" );
      }
      break;


  case MO_ACTION_OBJECT_GETPRECONFIGS:
      {
      arg0  = p_pDataMessage->Get(1).ToText();
      arg1Int  = p_pDataMessage->Get(2).Int();// [0,2] = 3 preconfigs [0,-1]
      arg2Int  = p_pDataMessage->Get(3).Int();// -1 > final
      MObject = NULL;
      moPreConfig pC = fxObject->GetConfig()->GetPreconfig(arg1Int);
      if (MObject) {
          FullObjectJSON = "{ 'preconfig': " + pC.ToJSON();
          FullObjectJSON+= ", 'position': "+IntToStr(arg1Int)+" }";
          MODebug2->Message(FullObjectJSON);

          pMessageToSend = new moDataMessage();
          if (pMessageToSend) {
              pMessageToSend->Add( moData("objectgetpreconfigs") );
              //pMessageToSend->Add( moData("ANY_LISTENER_ID") ); /// identifier for last message
              pMessageToSend->Add( moData( arg0 ) );
              pMessageToSend->Add( moData( FullObjectJSON ) );
              //pMessageToSend->Add( moData( "{'testing': 0}" ) );
              //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
              // send it: but we need an id
              SendMoldeoAPIMessage( pMessageToSend );
          }

          return 0;
      }
      MODebug2->Error("moConsole::ProcessMoldeoAPIMessage > "+MoldeoAPICommand+" > MO_ACTION_OBJECT_GETPRECONFIGS > [" + arg0+"] not found!" );
      }
      break;


  /** CONSOLE

  */

    case MO_ACTION_CONSOLE_PRESENTATION:
      {
      //m_pIODeviceManager->GetEvents()->Add( 7878, SDL_KEYDOWN, SDLK_F12 );

      newMessage = new moMessage( 999 /**PLAYER*/,
                                    999 /**TO PLAYER PRESENTATION*/,
                                    -1,
                                     pData );
      m_pIODeviceManager->GetEvents()->Add( (moEvent*) newMessage );
      MODebug2->Message("MO_ACTION_CONSOLE_PRESENTATION");
      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_SCREENSHOT:
      {
      //ScreenShot();
      if (m_pResourceManager->GetRenderMan()->Screenshot(moText(""), m_LastScreenshot)) {
        pMessageToSend = new moDataMessage();
        if (pMessageToSend) {
            pMessageToSend->Add( moData("consolescreenshot") );
            pMessageToSend->Add( moData("success") );
            ///JSON INFO
            pMessageToSend->Add( moData("{\"lastscreenshot\" : \""+m_LastScreenshot + "\"}" ) );
            SendMoldeoAPIMessage( pMessageToSend );
        }
      }
      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_PREVIEW_SHOT:
      {
      //ScreenShot();
      if (m_pResourceManager->GetRenderMan()->PreviewShot( true ) ) {
        pMessageToSend = new moDataMessage();
        if (pMessageToSend) {
            pMessageToSend->Add( moData("consolepreviewshot") );
            pMessageToSend->Add( moData("success") );
            ///JSON INFO
            //pMessageToSend->Add( moData("{\"lastscreenshot\" : \""+m_LastScreenshot + "\"}" ) );
            SendMoldeoAPIMessage( pMessageToSend );
        }
      }

      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_SAVE:
      {
      //ScreenShot();
      ///SAVING ALL
      MODebug2->Message("moConsole::Processing > Saving ALL");
      //Save("");
      for( int fx=0; fx<(int)m_MoldeoObjects.Count(); fx++ ) {
        m_MoldeoObjects[fx]->Save();
        /*m_MoldeoObjects[fx]->GetConfig()->SaveConfig();*/
      }

      pMessageToSend = new moDataMessage();
      if (pMessageToSend) {
          pMessageToSend->Add( moData("consolesave") );
          pMessageToSend->Add( moData("success") );
          SendMoldeoAPIMessage( pMessageToSend );
      }
      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_SAVEAS:
      {
      MODebug2->Message("moConsole::Processing > Save As");
      arg0  = p_pDataMessage->Get(1).ToText();//DIRECTORY
      ///si no existe ese directorio crearlo!!!
      MODebug2->Message("moConsole::Processing > Save As: FROM: " + m_pResourceManager->GetDataMan()->GetDataPath()
                        + " TO: "+ arg0);
      if ( moFileManager::CopyDirectory( m_pResourceManager->GetDataMan()->GetDataPath(), arg0 ) ) {

        moFile molFile(  m_pResourceManager->GetDataMan()->GetConsoleConfigName() );

        MODebug2->Message("moConsole::Processing > Save As OK!!!");
        m_pResourceManager->GetDataMan()->Init( m_pResourceManager->GetDataMan()->GetAppPath(),
                                               arg0,
                                               molFile.GetFullName() );
         MODebug2->Message(" changing path: " + m_pResourceManager->GetDataMan()->GetDataPath()
                          + " mol file:" + molFile.GetFullName() );

        for( int fx=0; fx<(int)m_MoldeoObjects.Count(); fx++ ) {
          moFile cfgName( m_MoldeoObjects[fx]->GetConfig()->GetName() );
          m_MoldeoObjects[fx]->GetConfig()->SaveConfig( arg0+moSlash+cfgName.GetFullName() );
        }
        pMessageToSend = new moDataMessage();
        if (pMessageToSend) {
            pMessageToSend->Add( moData("consolesaveas") );
            pMessageToSend->Add( moData("success") );
            ///JSON INFO
            pMessageToSend->Add( moData("{\"projectfullpath\":\""+arg0 + "\"}" ) );
            SendMoldeoAPIMessage( pMessageToSend );
        }

      } else {
        pMessageToSend = new moDataMessage();
        if (pMessageToSend) {
            pMessageToSend->Add( moData("consolesaveas") );
            pMessageToSend->Add( moData("failed") );
            SendMoldeoAPIMessage( pMessageToSend );
        }
      }
      }
      break;

    case MO_ACTION_CONSOLE_PLAY:
      {
      ConsolePlay();

      EffectStateJSON = m_ConsoleState.ToJSON();

      pMessageToSend = new moDataMessage();

      if (pMessageToSend) {
          pMessageToSend->Add( moData("consolegetstate") );
          pMessageToSend->Add( moData("__console__") );
          pMessageToSend->Add( moData( EffectStateJSON ) );
          /** send it: but we need an id */
          SendMoldeoAPIMessage( pMessageToSend );
      }

      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_STOP:
      {
      ConsoleStop();

      EffectStateJSON = m_ConsoleState.ToJSON();

      pMessageToSend = new moDataMessage();

      if (pMessageToSend) {
          pMessageToSend->Add( moData("consolegetstate") );
          pMessageToSend->Add( moData("__console__" ) );
          pMessageToSend->Add( moData( EffectStateJSON ) );
          /** send it: but we need an id */
          SendMoldeoAPIMessage( pMessageToSend );
      }
      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_PAUSE:
      {
      ConsolePause();

      EffectStateJSON = m_ConsoleState.ToJSON();

      pMessageToSend = new moDataMessage();

      if (pMessageToSend) {
          pMessageToSend->Add( moData("consolegetstate") );
          pMessageToSend->Add( moData("__console__" ) );
          pMessageToSend->Add( moData( EffectStateJSON ) );
          /** send it: but we need an id */
          SendMoldeoAPIMessage( pMessageToSend );
      }
      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_RECORD_SESSION:
      {
      MODebug2->Message("moConsole::Processing > Console Record Session");
      ConsoleRecordSession();

      EffectStateJSON = m_ConsoleState.ToJSON();

      pMessageToSend = new moDataMessage();

      if (pMessageToSend) {
          pMessageToSend->Add( moData("consolerecordsession") );
          pMessageToSend->Add( moData("__console__" ) );
          pMessageToSend->Add( moData( EffectStateJSON ) );
          /** send it: but we need an id */
          SendMoldeoAPIMessage( pMessageToSend );
      }
      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_RENDER_SESSION:
      {
        arg0  = p_pDataMessage->Get(1).ToText();//QUALITY! JPG,JPGGOOD...
        ConsoleRenderSession( arg0 );


        moText rendered_folder = DataMan()->GetSession()->GetRenderedFolder();

        MODebug2->Message("moConsole::Processing > Console Render Session (quality): " + arg0);
        MODebug2->Message("moConsole::Processing > Console Render Session (rendered_folder): " + rendered_folder);

        moText RenderSessionInfo = "{";
        moText data_session = DataMan()->GetSession()->ToJSON();
        EffectStateJSON = m_ConsoleState.ToJSON();

        RenderSessionInfo+= moText("'consolestate': ") + EffectStateJSON;
        RenderSessionInfo+= moText(",") + moText("'session': ") + DataMan()->GetSession()->ToJSON();
        RenderSessionInfo+= "}";

        pMessageToSend = new moDataMessage();

        MODebug2->Message("moConsole::Processing > Console Render Session: " + RenderSessionInfo);

        if (pMessageToSend) {
            pMessageToSend->Add( moData("consolerendersession") );
            pMessageToSend->Add( moData("__console__" ) );
            pMessageToSend->Add( moData( RenderSessionInfo ) );
            /** send it: but we need an id */
            SendMoldeoAPIMessage( pMessageToSend );
        }
        return 0;
      }
      break;

    case MO_ACTION_CONSOLE_GET:
      {
      FullObjectJSON = "{";
      FullObjectJSON+= moText("'datapath': '")+m_pResourceManager->GetDataMan()->GetDataPath()+"'";
      FullObjectJSON+= fieldSeparation+"'apppath': '"+m_pResourceManager->GetDataMan()->GetAppPath()+"'";
      FullObjectJSON+= fieldSeparation+"'appdatapath': '"+m_pResourceManager->GetDataMan()->GetAppDataPath()+"'";
      FullObjectJSON+= fieldSeparation+"'configname': '"+m_pResourceManager->GetDataMan()->GetConsoleConfigName()+"'";
      //FullObjectJSON+= fieldSeparation+"'config': "+this->m_Config.ToJSON();
      //FullObjectJSON+= fieldSeparation+"'MapEffects':  {";
      FullObjectJSON+= fieldSeparation+"'MapObjects':  {";
      fieldSeparation = "";
      /*
      for(int i=0;i<(int)this->m_EffectManager.AllEffects().Count(); i++ ) {
        moEffect* Fx = m_EffectManager.AllEffects().Get(i);
        FullObjectJSON+= fieldSeparation+"'"+Fx->GetLabelName()+"': '" + Fx->GetKeyName() +"'";
        fieldSeparation = ",";
      }
      */

      for(int i=0;i<(int)this->m_MoldeoObjects.Count(); i++ ) {
        moMoldeoObject* Mobj = m_MoldeoObjects.Get(i);
        if (Mobj) {
            FullObjectJSON+= fieldSeparation+"'"+Mobj->GetLabelName()+"': {";
            FullObjectJSON+= "'lbl': '" + Mobj->GetLabelName() +"',";
            FullObjectJSON+= "'name': '" + Mobj->GetName() +"',";
            FullObjectJSON+= "'cfg': '" + Mobj->GetConfigName() +"',";
            FullObjectJSON+= "'key': '" + Mobj->GetKeyName() +"',";
            FullObjectJSON+= "'cla': '" + Mobj->GetMobDefinition().GetTypeToClass( Mobj->GetMobDefinition().GetType() ) +"',";
            FullObjectJSON+= "'pari': '" + IntToStr(Mobj->GetMobDefinition().GetMobIndex().GetParamIndex()) +"',";
            FullObjectJSON+= "'vali': '" + IntToStr(Mobj->GetMobDefinition().GetMobIndex().GetValueIndex()) +"',";
            FullObjectJSON+= "'acti': '" + IntToStr((int)Mobj->GetMobDefinition().GetActivate()==1) +"',";
            FullObjectJSON+= "'ix': '" + IntToStr(i) +"',";
            FullObjectJSON+= "'id': '" + IntToStr(Mobj->GetId()) +"'";
            FullObjectJSON+= "}";
            fieldSeparation = ",";
        }
      }

      FullObjectJSON+= "}";

      FullObjectJSON+= "}";

      pMessageToSend = new moDataMessage();
      if (pMessageToSend) {
          pMessageToSend->Add( moData("consoleget") );
          pMessageToSend->Add( moData("__console__") ); /// identifier for last message
          pMessageToSend->Add( moData( FullObjectJSON ) );
          //pMessageToSend->Add( moData( "{'testing': 0}" ) );
          //MODebug2->Message( "moConsole::ProcessMoldeoAPIMessage > replying: " + EffectStateJSON );
          // send it: but we need an id
          SendMoldeoAPIMessage( pMessageToSend );
      }

      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_GETSTATE:
      {
      /** must return console state */
      EffectStateJSON = m_ConsoleState.ToJSON();

      pMessageToSend = new moDataMessage();

      if (pMessageToSend) {
          pMessageToSend->Add( moData("consolegetstate") );
          pMessageToSend->Add( moData("__console__" ) );
          pMessageToSend->Add( moData( EffectStateJSON ) );
          /** send it: but we need an id */
          //MODebug2->Message("moConsole::ProcessMoldeoAPIMessage > DataMessage is MO_ACTION_CONSOLE_GETSTATE > "+EffectStateJSON);
          SendMoldeoAPIMessage( pMessageToSend );
      }

      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_SETSTATE:
      {
      /** must set the new actual console state */
      return 0;
      }
      break;

    case MO_ACTION_CONSOLE_GET_PLUGINS:
      {
        FullObjectJSON = "[";
        moText fieldSeparation = "";
        MODebug2->Message( "MO_ACTION_CONSOLE_GET_PLUGINS" );
        MODebug2->Message( m_pResourceManager->GetDataMan()->GetPluginDefinitions().Count() );
        moPluginDefinitions &Plugins( m_pResourceManager->GetDataMan()->GetPluginDefinitions() );
        for( int i=0; i< (int) Plugins.Count(); i++ ) {
          moPluginDefinition& PluginDef( Plugins[i] );
          FullObjectJSON+= fieldSeparation + PluginDef.ToJSON();
          fieldSeparation = moText(",");
        }
        FullObjectJSON+= "]";
        pMessageToSend = new moDataMessage();
        if (pMessageToSend) {
            pMessageToSend->Add( moData("consolegetplugins") );
            pMessageToSend->Add( moData("__console__") ); /// identifier for last message
            pMessageToSend->Add( moData( FullObjectJSON ) );
            SendMoldeoAPIMessage( pMessageToSend );
        }

        return 0;
      }
      break;

    default:

      break;

  }

  return -1; /*PROCESS NOT OK*/
}

void moConsole::Error( const moText &p_message ) {
  MODebug2->Error( p_message );
}

int moConsole::NewMob( const moMobDefinition &p_MobDef ) {

    moText confignamecomplete = "";
    bool res = false;

    //p_MobDesc.SetProjectDescriptor( GetProject() );

    moMobDefinition pMobDef = p_MobDef;//p_MobDesc.GetMobDefinition();

    moEffect* pEffect = NULL;
    moResource* pResource = NULL;
    MOint rid = -1;
    moMoldeoObject* pMOB = NULL;
    moMoldeoObject* pFatherMOB = NULL;
    moEffectManager* pFxManager = NULL;

    moConfig* pConfig = NULL;

    if (pMobDef.GetMoldeoFatherId()==-1) {
        pConfig = &m_Config;
        pFxManager = &m_EffectManager;
    }
    else if (pMobDef.GetMoldeoFatherId()>=0) {
        pFatherMOB = GetObjectByIdx( pMobDef.GetMoldeoFatherId() );
        if (pFatherMOB && pFatherMOB->GetName()=="scene") {
          moSceneEffect* pScene = (moSceneEffect*) pFatherMOB;
          pConfig = pScene->GetConfig();
          pFxManager = &pScene->GetEffectManager();
        }
    }

    switch( (int) pMobDef.GetType()) {
        case MO_OBJECT_PREEFFECT:
              pMobDef.SetConsoleParamIndex( pConfig->GetParamIndex("preeffect") );
              pMobDef.SetConsoleValueIndex( pFxManager->PreEffects().Count() );
              pMOB = (moMoldeoObject*)pFxManager->New( pMobDef );
              pEffect = (moEffect*) pMOB;
              if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "preeffect" );
              break;
          case MO_OBJECT_EFFECT:
              pMobDef.SetConsoleParamIndex( pConfig->GetParamIndex("effect") );
              pMobDef.SetConsoleValueIndex( pFxManager->Effects().Count() );
              pMOB = (moMoldeoObject*)pFxManager->New( pMobDef );
              pEffect = (moEffect*) pMOB;
              if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "effect" );
              break;
          case MO_OBJECT_POSTEFFECT:
              pMobDef.SetConsoleParamIndex( pConfig->GetParamIndex("posteffect") );
              pMobDef.SetConsoleValueIndex( pFxManager->PostEffects().Count() );
              pMOB = (moMoldeoObject*)pFxManager->New( pMobDef );
              pEffect = (moEffect*) pMOB;
              if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "posteffect" );
              break;
          case MO_OBJECT_MASTEREFFECT:
              {
                  pMobDef.SetConsoleParamIndex( pConfig->GetParamIndex("mastereffect") );
                  pMobDef.SetConsoleValueIndex( pFxManager->MasterEffects().Count() );
                  pMOB = (moMoldeoObject*) pFxManager->New( pMobDef );
                  pEffect = (moEffect*) pMOB;
                  moMasterEffect* pMaster = (moMasterEffect*) pMOB;
                  if (pMaster) pMaster->Set( &m_EffectManager, &m_ConsoleState );
                  if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "mastereffect" );
              }
              break;
          case MO_OBJECT_IODEVICE:
              pMobDef.SetConsoleParamIndex( pConfig->GetParamIndex("devices") );
              pMobDef.SetConsoleValueIndex( m_pIODeviceManager->IODevices().Count() );
              pMOB = (moMoldeoObject*) m_pIODeviceManager->NewIODevice( pMobDef.GetName(), pMobDef.GetConfigName(), pMobDef.GetLabelName(), pMobDef.GetKeyName(), MO_OBJECT_IODEVICE, pMobDef.GetMobIndex().GetParamIndex(), pMobDef.GetMobIndex().GetValueIndex() );
              if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "iodevice" );
              break;
          case MO_OBJECT_RESOURCE:
              pMobDef.SetConsoleParamIndex( m_Config.GetParamIndex("resources") );
              pMobDef.SetConsoleValueIndex( m_Config.GetParam("resources").GetValuesCount() );
              rid = m_pResourceManager->GetResourceIndex( pMobDef.GetLabelName() );
              if(rid>-1) pResource = m_pResourceManager->GetResource(rid);

              if (pResource ) {
                  //do nothing
                  if (pResource->GetLabelName()!=pMobDef.GetLabelName()) {
                      pResource->SetConfigName( pMobDef.GetConfigName() );
                      pResource->SetLabelName( pMobDef.GetLabelName() );
                  } else Error( moText("Resource already loaded: ") + (moText)pResource->GetLabelName() );
              } else
              if (m_pResourceManager->NewResource( pMobDef.GetName(), pMobDef.GetConfigName(), pMobDef.GetLabelName(), pMobDef.GetKeyName() )) {
                  rid = m_pResourceManager->GetResourceIndex( pMobDef.GetLabelName() );
                  if (rid>=0) {
                      pResource = m_pResourceManager->GetResource(rid);
                      if (pResource) {
                          pResource->SetConfigName( pMobDef.GetConfigName() );
                          pResource->SetLabelName( pMobDef.GetLabelName() );
                      }
                  }
              }
              pMOB = (moMoldeoObject*) pResource;

              pMOB->SetMobDefinition( pMobDef );

              if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "resource" );
              break;
          default:
              Error(moText("moConsole::NewMob: No mob type defined."));
              return -1;
              break;
      }

      if (pMOB) {

          if (pMOB->GetConfigName().Length()>0) {

              confignamecomplete = m_pResourceManager->GetDataMan()->GetDataPath();
              confignamecomplete +=  moSlash + moText(pMOB->GetConfigName());
              confignamecomplete +=  moText(".cfg");

              pMOB->GetDefinition();

              ///Creates the config file based on definition plugin
              moFile pFile(confignamecomplete);

              ///veamos si existe antes...
              if (!pFile.Exists()) {
                  ///si no existe lo creamos...
                  res = pMOB->GetConfig()->CreateDefault( confignamecomplete );

              } else {
                  ///si existe lo cargamos...
                  res = true;

              }

              if (res) {
                  pMOB->GetConfig()->UnloadConfig();

                  pMOB->SetResourceManager( m_pResourceManager );

                  res = pMOB->Init();
                  if (res) {
                      if (pEffect) {
                          pEffect->LoadCodes( m_pIODeviceManager );
                          if (pMOB->GetMobDefinition().GetActivate()) { pEffect->Activate(); }
                      }
                  } else {
                      Error( moText("moConsole::NewMob Couldn't initialized effect" ) );
                  }

                  //pEffect->Draw( &m_EffectState.tempo );
                  /** TODO: convert to XML !!!*/
                  moValue effectvalue( pMOB->GetName(), "TXT" );
                  effectvalue.AddSubValue( pMOB->GetConfigName() , "TXT" );
                  effectvalue.AddSubValue( pMOB->GetLabelName() , "TXT" );
                  effectvalue.AddSubValue( "0" , "NUM" );
                  if (pMOB->GetMobDefinition().GetActivate())
                    effectvalue.AddSubValue( "1" , "NUM" );
                  else
                    effectvalue.AddSubValue( "0" , "NUM" );

                  if (pMOB->GetMobDefinition().GetKeyName()!="")
                    effectvalue.AddSubValue( pMOB->GetMobDefinition().GetKeyName() , "TXT" );
                  else
                    effectvalue.AddSubValue( "" , "TXT" );

                  if (pConfig) pConfig->GetParam( pMOB->GetMobDefinition().GetMobIndex().GetParamIndex() ).AddValue( effectvalue );

                  LoadConnections();
                  moMobDefinition MDef = pMOB->GetMobDefinition();
                  bool validity = MDef.IsValid();
                  if (validity==false) {
                    Error( moText("moConsole::NewMob:: Source > MobDefinition Invalid") );
                  } else {
                    //ProjectUpdated( m_ProjectDescriptor );
                    MODebug2->Message( "NewMob OK:" + MDef.ToJSON());
                    return 1;
                  }

              } else {
                  Error( moText("moConsole::NewMob Couldn't create default configuration") );
              }

          } else {
              Error( moText("moConsole::NewMob Config filename undefined" ) );
          }
      } else {
          Error( moText("moConsole::NewMob Couldn't create effect: " ) + pMobDef.GetName() );
      }

      return -1;

}

int moConsole::SetMob( const moMobDefinition &p_MobDef ) {
  moMoldeoObject* MObject = NULL;
  moMoldeoObject* pFatherMOB = NULL;
  moEffectManager* pFxManager = NULL;
  moConfig* pConfig = NULL;

  MODebug2->Message("SetMob");

  MObject = GetObjectByIdx( GetObjectId( p_MobDef.GetLabelName() ) );
  if (!MObject) MObject = GetObjectByIdx( p_MobDef.GetMoldeoId() );

  if (p_MobDef.GetMoldeoFatherId()==-1) {
      pConfig = &m_Config;
      pFxManager = &m_EffectManager;
  }
  else if (p_MobDef.GetMoldeoFatherId()>=0) {
      pFatherMOB = GetObjectByIdx( p_MobDef.GetMoldeoFatherId() );
      if (pFatherMOB && pFatherMOB->GetName()=="scene") {
        moSceneEffect* pScene = (moSceneEffect*) pFatherMOB;
        pConfig = pScene->GetConfig();
        pFxManager = &pScene->GetEffectManager();
      }
  }

  if (MObject) {
    //SAVE IN OBJECT
    moMobDefinition MobDef = MObject->GetMobDefinition();
    MobDef.SetLabelName( p_MobDef.GetLabelName() );
    MobDef.SetConfigName( p_MobDef.GetConfigName() );
    MobDef.SetKeyName( p_MobDef.GetKeyName() );
    MobDef.SetActivate( p_MobDef.GetActivate() );
    MObject->SetMobDefinition( MobDef );

    //SAVE IN PARAMETERS CONFIG
    moMoldeoObject* pMOB = MObject;
    moValue effectvalue( pMOB->GetName(), "TXT" );
    effectvalue.AddSubValue( pMOB->GetConfigName() , "TXT" );
    effectvalue.AddSubValue( pMOB->GetLabelName() , "TXT" );
    effectvalue.AddSubValue( "0" , "NUM" );
    if (pMOB->GetMobDefinition().GetActivate())
      effectvalue.AddSubValue( "1" , "NUM" );
    else
      effectvalue.AddSubValue( "0" , "NUM" );

    if (pMOB->GetMobDefinition().GetKeyName()!="")
      effectvalue.AddSubValue( pMOB->GetMobDefinition().GetKeyName() , "TXT" );
    else
      effectvalue.AddSubValue( "" , "TXT" );

    if (pConfig) pConfig->GetParam( pMOB->GetMobDefinition().GetMobIndex().GetParamIndex() ).GetValue( pMOB->GetMobDefinition().GetMobIndex().GetValueIndex() ) = effectvalue;

    if (pMOB->GetConfigName().Length()>0) {

        moText confignamecomplete = m_pResourceManager->GetDataMan()->GetDataPath();
        confignamecomplete +=  moSlash + moText(pMOB->GetConfigName());
        confignamecomplete +=  moText(".cfg");

        ///Creates the config file based on definition plugin
        moFile pFile(confignamecomplete);

        ///veamos si existe antes...
        if (!pFile.Exists()) {
            ///si no existe lo creamos...
            pMOB->GetConfig()->SaveConfig(confignamecomplete);
        } else {
            ///si existe preguntamos si sobreescribimos o no?
            //pMOB->GetConfig()->SaveConfig(confignamecomplete);
            MODebug2->Message( "Config exists, overwriting? No is default. Yes not implemented" );
        }
    }
    MODebug2->Message( "Set Mob ok" );
    return 1;
  } else {
    MODebug2->Error( "Set Mob, Mob not found, check label name and moldeo id." );
  }
  return -1;
}

int moConsole::AddChildMob( const moMobDefinition &p_MobDef, const moMobDefinition &p_MobDefFather ) {

  moMobDefinition pMobDef = p_MobDef;
  /*moMoldeoObject* pMOB = GetObject( p_MobDesc );*/
  /*really must be check by NewMob... it shouldnt let create MOBs with same label and configname...*/

  pMobDef.SetMoldeoFatherId( p_MobDefFather.GetMoldeoId() );

  return NewMob( pMobDef );
}


int moConsole::MoveMob( const moMobDefinition& p_MobDef, int position, MOboolean is_relative ) {

  ///chequea el objeto a mover

  moMoldeoObject* pObj = NULL;
  moMoldeoObject* pObjAux = NULL;
  int array_count = 0;
  moMobDefinition pMobDef = p_MobDef;
  moMoldeoObjectType MobType = pMobDef.GetType();
  int index_object = pMobDef.GetMobIndex().GetValueIndex();

  pObj = m_MoldeoObjects.GetRef(
                            RelativeToGeneralIndex( index_object, MobType )
                             );

  ///si el valor del indice nuevo es valido (esta en rango)
  switch( pMobDef.GetType() ) {

    case MO_OBJECT_EFFECT:
      array_count = m_EffectManager.Effects().Count();
      break;
    case MO_OBJECT_PREEFFECT:
      array_count = m_EffectManager.PreEffects().Count();
      break;
    case MO_OBJECT_POSTEFFECT:
      array_count = m_EffectManager.PostEffects().Count();
      break;
    case MO_OBJECT_MASTEREFFECT:
      array_count = m_EffectManager.MasterEffects().Count();
      break;

    case MO_OBJECT_IODEVICE:
      array_count = m_pIODeviceManager->IODevices().Count();
      break;
    case MO_OBJECT_RESOURCE:
      //array_count = m_pResourceManager->Resources().Count();
      array_count = 0;
      Error( "Sorry. Changing order of resources is not allowed yet." );
      return -1;
      break;
    default:
      array_count = 0;
      break;

  }

  if (is_relative) {
    position = index_object + position;
    if (position<0) {
      position = 0;
    }
    if (position>=array_count) {
      position = array_count - 1;
    }
  }
  MODebug2->Message( moText("Moving from ")+IntToStr(index_object)
                    +moText(" to ")+IntToStr(position)
                    +moText("/")+IntToStr(array_count));

      if ( position >= 0 && position < array_count ) {

          ///hace un switch entre el objeto q estaba ahi y este...
          //p_MobDesc.GetMobDefinition().GetMobIndex().GetValueIndex()
          //moEffect* pObjFx = m_EffectManager.Effects().GetRef(index_object);
          int paramindex = pMobDef.GetMobIndex().GetParamIndex();
          int valueindex = pMobDef.GetMobIndex().GetValueIndex();

          int preconf1 = m_Config.GetValue( paramindex, valueindex ).GetSubValue(3).Int();
          int active1 = m_Config.GetValue( paramindex, valueindex ).GetSubValue(4).Int();

          ///intercambiamos los objetos dentro del array del effectmanager


          int diff = position - index_object;
          if (diff==0) {
            ///no se hace nada
          } else if (diff>0) {
            ///position es mas que index_object, el objeto baja en los indices

            /// se reordenan los objetos entre el position y el index_object....
            /// arrancando desde el index_object hasta el position
            for( int o = index_object; o < (position); o++ ) {

              ///intercambiamos los objetos... en el array
              pObjAux = m_MoldeoObjects.GetRef( RelativeToGeneralIndex( o+1, MobType ) );

              if ( MO_OBJECT_EFFECT<=MobType && MobType<=MO_OBJECT_PREEFFECT) m_EffectManager.Set( o, pObjAux );
              else if (MobType==MO_OBJECT_IODEVICE) m_pIODeviceManager->IODevices().Set( o, (moIODevice*)pObjAux );

              m_MoldeoObjects.Set( RelativeToGeneralIndex( o, MobType ) , pObjAux );

              ///intercambiamos los valores en el config y actualizamos los mobindex.... importante!!
              //moMobIndex mindex2 = pObjFxAux->GetMobDefinition().GetMobIndex();
              moMobIndex mindex2 = pObjAux->GetMobDefinition().GetMobIndex();
              int preconf2 = m_Config.GetValue( paramindex, mindex2.GetValueIndex() ).GetSubValue(3).Int();
              int active2 = m_Config.GetValue( paramindex, mindex2.GetValueIndex() ).GetSubValue(4).Int();

              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(0).SetText( pObjAux->GetName() );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(1).SetText( pObjAux->GetConfigName() );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(2).SetText( pObjAux->GetLabelName() );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(3).SetInt( preconf2 );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(4).SetInt( active2 );
              pObjAux->SetConsoleValueIndex( o );

            }
            ///al fin...
            if ( MO_OBJECT_EFFECT<=MobType && MobType<=MO_OBJECT_PREEFFECT) m_EffectManager.Set( position, pObj );
            else if (MobType==MO_OBJECT_IODEVICE) m_pIODeviceManager->IODevices().Set( position, (moIODevice*)pObj );

            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(0).SetText( pObj->GetName() );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(1).SetText( pObj->GetConfigName() );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(2).SetText( pObj->GetLabelName() );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(3).SetInt( preconf1 );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(4).SetInt( active1 );
            pObj->SetConsoleValueIndex( position );
            int mobsindx = RelativeToGeneralIndex( position, MobType );
            m_MoldeoObjects.Set( mobsindx, pObj );

          } else if (diff<0) {
            ///position es menos que index_object, el objeto sube en los indices

            /// se reordenan los objetos entre el position y el index_object....
            /// arrancando desde el position hasta el index_object
            for( int o = index_object ; o >= (position+1)  ; o-- ) {

             pObjAux = m_MoldeoObjects.GetRef( RelativeToGeneralIndex( o-1, MobType ) );

              if ( MO_OBJECT_EFFECT<=MobType && MobType<=MO_OBJECT_PREEFFECT) m_EffectManager.Set( o, pObjAux );
              else if (MobType==MO_OBJECT_IODEVICE) m_pIODeviceManager->IODevices().Set( o, (moIODevice*)pObjAux );

              m_MoldeoObjects.Set( RelativeToGeneralIndex( o, MobType ), pObjAux );

              moMobIndex mindex2 = pObjAux->GetMobDefinition().GetMobIndex();
              int preconf2 = m_Config.GetValue( paramindex, mindex2.GetValueIndex() ).GetSubValue(3).Int();
              int active2 = m_Config.GetValue( paramindex, mindex2.GetValueIndex() ).GetSubValue(4).Int();

              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(0).SetText( pObjAux->GetName() );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(1).SetText( pObjAux->GetConfigName() );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(2).SetText( pObjAux->GetLabelName() );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(3).SetInt( preconf2 );
              m_Config.GetParam( paramindex ).GetValue( o ).GetSubValue(4).SetInt( active2 );

              pObjAux->SetConsoleValueIndex( o );

            }
            ///al fin...
            if ( MO_OBJECT_EFFECT<=MobType && MobType<=MO_OBJECT_PREEFFECT) m_EffectManager.Set( position, pObj );
            else if (MobType==MO_OBJECT_IODEVICE) m_pIODeviceManager->IODevices().Set( position, (moIODevice*)pObj );

            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(0).SetText( pObj->GetName() );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(1).SetText( pObj->GetConfigName() );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(2).SetText( pObj->GetLabelName() );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(3).SetInt( preconf1 );
            m_Config.GetParam( paramindex ).GetValue( position ).GetSubValue(4).SetInt( active1 );
            pObj->SetConsoleValueIndex( position );
            int mobsindx = RelativeToGeneralIndex( position, MobType );
            m_MoldeoObjects.Set( mobsindx, pObj );
          }

      }

  LoadConnections();

  return 1;

}


int moConsole::DuplicateMob( const moMobDefinition& p_MobDef ) {

  ///duplicar el archivo de configuracion agregandole un numero al final...
  moText datapath;
  moText filenameduplicate;
  moFile newfile;
  moText filename = p_MobDef.GetConfigName();
  moText labelname = p_MobDef.GetLabelName();
  moText labelnameduplicate;
  int mStatus = -1;
  moMobDefinition pMobDef = p_MobDef;

  if (m_pResourceManager) {

    datapath = moMoldeoObject::m_pResourceManager->GetDataMan()->GetDataPath();

    filename = filename;
    labelname = labelname;

    filenameduplicate = filename + moText(".cfg");

    newfile.SetCompletePath( datapath + filenameduplicate );

    int count = 1;
    while( newfile.Exists() ) {
      filenameduplicate = filename + IntToStr( count );
      newfile.SetCompletePath( datapath + filenameduplicate + moText(".cfg") );
      count++;
    }

    m_pResourceManager->GetFileMan()->CopyFile( datapath + filename + moText(".cfg"), datapath + filenameduplicate + moText(".cfg")  );


    labelnameduplicate = labelname;

    ///modificar el labelname agreg�ndole un n�mero al final
    count = 1;
    while(LabelNameExists(labelnameduplicate)) {
      labelnameduplicate = labelname + IntToStr( count );
      count++;
    }

    ///importar este nuevo Mob...!!! que facil!!!
    pMobDef.SetConfigName( filenameduplicate );
    pMobDef.SetLabelName( labelnameduplicate );

    mStatus = NewMob( pMobDef );

  }

  return mStatus;
}

int moConsole::DeleteMob( const moMobDefinition& p_MobDef ) {

  moMoldeoObject* pMOB;

  pMOB = GetObjectByIdx(p_MobDef.GetMoldeoId());

  bool consoleinit = m_bInitialized;

  moMobDefinition pMobDef = p_MobDef;

  moMoldeoObject* pFatherMOB = NULL;
  moEffectManager* pFxManager = NULL;

  moConfig* pConfig = NULL;
  moMobIndex mobindex = pMobDef.GetMobIndex();
  int removeid = pMOB->GetMobDefinition().GetMobIndex().GetValueIndex();

  if (pMobDef.GetMoldeoFatherId()==-1) {
      pConfig = &m_Config;
      pFxManager = &m_EffectManager;
  }
  else if (pMobDef.GetMoldeoFatherId()>=0) {
      pFatherMOB = GetObjectByIdx( pMobDef.GetMoldeoFatherId() );
      if (pFatherMOB && pFatherMOB->GetName()=="scene") {
        moSceneEffect* pScene = (moSceneEffect*) pFatherMOB;
        pConfig = pScene->GetConfig();
        pFxManager = &pScene->GetEffectManager();
        //removeid = pFxManager->GetEffectId( pMobDef.GetLabelName() );
      }
  }



      if (pMOB && pFxManager) {

          if ( pMOB->GetId() >= MO_MOLDEOOBJECTS_OFFSET_ID ) {

              //just wait until we delete the object
              //important to avoid conflicts on loop...
              //maybe MUST BE A LOCK
              m_bInitialized = false;




              if (removeid>-1) {
                  switch( (int)pMOB->GetType() ) {
                      case MO_OBJECT_PREEFFECT:
                      case MO_OBJECT_EFFECT:
                      case MO_OBJECT_POSTEFFECT:
                      case MO_OBJECT_MASTEREFFECT:

                          if (pFxManager->RemoveEffect( removeid, pMOB->GetType())) {
                              ///remove from config
                              pConfig->GetParam( mobindex.GetParamIndex() ).DeleteValue( removeid );
                          }
                          break;
                      case MO_OBJECT_IODEVICE:
                          if ( m_pIODeviceManager ) {
                              m_pIODeviceManager->IODevices().Remove( removeid );
                              ///remove from config
                              m_Config.GetParam( mobindex.GetParamIndex() ).DeleteValue( removeid );
                          }
                          break;
                      case MO_OBJECT_RESOURCE:
                          if (m_pResourceManager && m_pResourceManager->RemoveResource( removeid ) ) {
                              ///remove from config
                              m_Config.GetParam( mobindex.GetParamIndex() ).DeleteValue( removeid );
                          }
                          break;
                  }
              }

              LoadConnections();

              ///back to previous state
              //m_bInitialized = consoleinit;

              ///we've finished, update project
              //ProjectUpdated( GetProject() );

          }
      }


  return 1;
}

void
moConsole::ProcessConsoleMessage( moMessage* p_pMessage ) {

    moText action;
    moText object_label;
    moText position;
    moText param_label;
    moText value_index;
    //int valueidx = -1;

    //int objectid = 0;
    //int paramid = 0;
    //moMoldeoObject* Object = NULL;
    //moEffect* pEffect = NULL;
    moParam pParam;

    if (p_pMessage) {

        moText actions = p_pMessage->m_Data.ToText();

        //cout << "Console receiving message: " << actions << endl;
        //moTextArray exploded = actions.Explode( moText(",") );

        MODebug2->Message( moText("moConsole::ProcessConsoleMessage > Actions:  ")
                          + actions );



/*


        switch(exploded.Count()) {
            case 2:
                action = exploded[0];
                object_label = exploded[1];

                if (action=="show") {
                    objectid = GetObjectId(object_label) - MO_MOLDEOOBJECTS_OFFSET_ID;

                    Object = m_MoldeoObjects[objectid];

                    if (Object && Object->GetConfig()) {
                        pEffect = (moEffect*)Object;
                        pEffect->Enable();
                        pEffect->TurnOn();
                        MODebug2->Message( moText("moConsole::ProcessConsoleMessage > show ")
                                          +Object->GetLabelName() );
                    } else {
                        MODebug2->Error( moText("moConsole::ProcessConsoleMessage > show : object not founded : id:")
                                        +(moText)IntToStr(objectid) + " label:" + object_label );
                    }
                }

                break;
            case 3:
                action = exploded[0];
                object_label = exploded[1];
                position = exploded[2];
                break;
            case 4:
                action = exploded[0];
                object_label = exploded[1];
                param_label = exploded[2];
                value_index = exploded[3];
                valueidx = atoi(value_index);

                if (action=="setcurrentvalue") {
                    objectid = GetObjectId(object_label)  - MO_MOLDEOOBJECTS_OFFSET_ID;
                    Object = m_MoldeoObjects[objectid];

                    if (Object && Object->GetConfig()) {

                        paramid = Object->GetConfig()->GetParamIndex(param_label);
                        Object->GetConfig()->SetCurrentValueIndex( paramid, valueidx );

                        pParam = Object->GetConfig()->GetParam(paramid);

                        MODebug2->Message( moText("moConsole::ProcessConsoleMessage: SetObjectCurrentValue")+Object->GetLabelName() + " Param:" + pParam.GetParamDefinition().GetName() );
                    } else {
                        MODebug2->Error( moText("imoConsole::ProcessConsoleMessage: SetObjectCurrentValue : object not founded : id:")+(moText)IntToStr(objectid) );
                    }
                }


                break;
        }

        if (exploded.Count()>=2) {
            cout << "Console received message: action: " << action << " object: " << object_label << " position:" << position << " param:" << param_label << " value:" << value_index << " valueidx:" << valueidx << endl;
        }

*/
    }

}


void
moConsole::Update() {
    if (!m_pResourceManager) return;
	moRenderManager* RenderMan = m_pResourceManager->GetRenderMan();

    m_ScreenshotInterval = m_Config.Int(moR(CONSOLE_SCREENSHOTS));

    if (m_ScreenshotInterval>30) {
        if (!m_ScreenshotTimer.Started()) {
            m_ScreenshotTimer.Start();
        } else {

            if ( m_ScreenshotTimer.Duration()>m_ScreenshotInterval ) {
                m_pResourceManager->GetRenderMan()->Screenshot(moText(""),m_LastScreenshot);
                m_ScreenshotTimer.Stop();
                m_ScreenshotTimer.Start();
            }

        }
    }


  ///TODO: each Object see all events and process a few... can and should be optimized
  /// optimization: only send a partial event list to every object, filtered by
  /// moMoldeoObject->GetMobDefinition()->GetMoldeoId()
	RenderMan->BeginUpdate();
	if (m_pIODeviceManager) {
		for(MOuint i = 0; i<(m_MoldeoObjects.Count()+m_MoldeoSceneObjects.Count()); i++) {
			RenderMan->BeginUpdateObject();
			moMoldeoObject* pMOB = NULL;
			if (i<m_MoldeoObjects.Count())
        pMOB = m_MoldeoObjects[i];
			else
        pMOB = m_MoldeoSceneObjects[i-m_MoldeoObjects.Count()];
			if (pMOB) {
                if (pMOB->GetType()!=MO_OBJECT_IODEVICE)///MO_OBJECT_IODEVICE WERE ALREADY UPDATED VIA m_pIODeviceManager->Update()
                    if (pMOB->Activated())
                      pMOB->Update(m_pIODeviceManager->GetEvents());
			}
			RenderMan->EndUpdateObject();
		}

		m_pIODeviceManager->PurgeEvents();
	}
	RenderMan->EndUpdate();

  moEventList* pEvents = m_pIODeviceManager->GetEvents();
  int nevents = 0;
  if (pEvents) {
      moEvent *actual=NULL,*tmp;
     // moMessage *pmessage;

      if (pEvents) actual = pEvents->First;

      ///Procesamos los eventos recibidos de los MoldeoObject Outlets
      while(actual!=NULL) {
        nevents++;
        tmp = actual->next;
        ///procesamos aquellos Outlet q estan dirigidos a este objeto
        //moText debugstr = actual->ToJSON();

        if (  actual->deviceid==MO_IODEVICE_CONSOLE
            && actual->devicecode == MO_ACTION_MOLDEOAPI_EVENT_SEND
            && actual->reservedvalue3 == MO_DATAMESSAGE) {
                moDataMessage* mpDataMessage = (moDataMessage*) actual->pointer;
                if (mpDataMessage) {
                  //tmpstr = "MOLDEOAPI MO_DATAMESSAGE > count:" + mpDataMessage->Count();
                  delete mpDataMessage;
                }
                actual->pointer = NULL;
                pEvents->Delete(actual);
        }

        if (actual->deviceid>=MO_MOLDEOOBJECTS_OFFSET_ID) {
          moMoldeoObject* pobj = GetObjectByIdx( actual->deviceid );
          //debugstr = pobj->GetLabelName()+"("+IntToStr(actual->deviceid)+") >> "+actual->ToJSON();
        }
        actual = tmp;

        //MODebug2->Message( debugstr );
      }

      //if ()
        //MODebug2->Message("fps:"+fps_text+" nevents:"+IntToStr(nevents));

  }

  //Procesamos aquellos Mensajes enviados con acciones
  moEvent* actual = m_pIODeviceManager->GetEvents()->First;
  moEvent* tmp = NULL;
  while(actual!=NULL) {

      if ( actual->deviceid==MO_IODEVICE_CONSOLE
        &&
           actual->reservedvalue3 == MO_MESSAGE ) {

          moMessage* ConsoleMessage = (moMessage*)actual;
          this->ProcessConsoleMessage(ConsoleMessage);
      }

      if ( ( actual->deviceid == MO_IODEVICE_CONSOLE )
            && ( actual->devicecode == MO_ACTION_MOLDEOAPI_EVENT_RECEIVE )
            && ( actual->reservedvalue3 == MO_DATAMESSAGE ) ) {

          moDataMessage* MoldeoAPIMessage = (moDataMessage*)actual->pointer;
          this->ProcessMoldeoAPIMessage( MoldeoAPIMessage );
     }

      tmp = actual;
      actual = tmp->next;
  }

  ///Use a parameter (live status) to send each frame / or nth frame a status from the console
  moDataMessage* pMessageToSend = new moDataMessage();
  if (pMessageToSend) {
    pMessageToSend->Add( moData( "consolegetstate" ) );
    ProcessMoldeoAPIMessage(pMessageToSend);
    delete pMessageToSend;
  }

	moMoldeoObject::Update( m_pIODeviceManager->GetEvents() );
}

moConfigDefinition *
moConsole::GetDefinition( moConfigDefinition *p_configdefinition ) {

	//default: alpha, color, syncro
	p_configdefinition = moMoldeoObject::GetDefinition( p_configdefinition );
	p_configdefinition->Add( moText("devices"), MO_PARAM_MOLDEO_OBJECT, CONSOLE_DEVICES );
	p_configdefinition->Add( moText("preeffect"), MO_PARAM_MOLDEO_OBJECT, CONSOLE_PREEFFECT );
	p_configdefinition->Add( moText("effect"), MO_PARAM_MOLDEO_OBJECT, CONSOLE_EFFECT );
	p_configdefinition->Add( moText("posteffect"), MO_PARAM_MOLDEO_OBJECT, CONSOLE_POSTEFFECT );
	p_configdefinition->Add( moText("mastereffect"), MO_PARAM_MOLDEO_OBJECT, CONSOLE_MASTEREFFECT );
	p_configdefinition->Add( moText("resources"), MO_PARAM_MOLDEO_OBJECT, CONSOLE_RESOURCES );

	p_configdefinition->Add( moText("mastereffects_on"), MO_PARAM_NUMERIC, (MOint)CONSOLE_ON, moValue("0","NUM").Ref(), moText("off,on") );
	p_configdefinition->Add( moText("fulldebug"), MO_PARAM_NUMERIC, CONSOLE_FULLDEBUG, moValue("0","NUM").Ref()  );

	//obsoleto
	//p_configdefinition->Add( moText("consolescript"), MO_PARAM_SCRIPT, CONSOLE_SCRIPT );

	p_configdefinition->Add( moText("outputmode"), MO_PARAM_TEXT, CONSOLE_OUTPUTMODE );
	p_configdefinition->Add( moText("outputresolution"), MO_PARAM_TEXT, CONSOLE_OUTPUTRESOLUTION );

	p_configdefinition->Add( moText("rendermode"), MO_PARAM_TEXT, CONSOLE_RENDERMODE );
	p_configdefinition->Add( moText("renderresolution"), MO_PARAM_TEXT, CONSOLE_RENDERRESOLUTION );

	p_configdefinition->Add( moText("clip1"), MO_PARAM_TEXT, CONSOLE_CLIP1 );
	p_configdefinition->Add( moText("clip2"), MO_PARAM_TEXT, CONSOLE_CLIP2 );
	p_configdefinition->Add( moText("clip3"), MO_PARAM_TEXT, CONSOLE_CLIP3 );

	p_configdefinition->Add( moText("screenshots"), MO_PARAM_NUMERIC, CONSOLE_SCREENSHOTS, moValue("0","NUM").Ref() );
	p_configdefinition->Add( moText("videoshots"), MO_PARAM_NUMERIC, CONSOLE_VIDEOSHOTS, moValue("0","NUM").Ref() );

	return p_configdefinition;
}

// Ugly, ugly and dirty...
MOint
moConsole::ConvertKeyNameToIdx(moText& name) {
	if(!stricmp( name, "SDLK_1" ) ) return 0;
	else if(!stricmp( name, "SDLK_2" ) ) return 1;
	else if(!stricmp( name, "SDLK_3" ) ) return 2;
	else if(!stricmp( name, "SDLK_4" ) ) return 3;
	else if(!stricmp( name, "SDLK_5" ) ) return 4;
	else if(!stricmp( name, "SDLK_6" ) ) return 5;
	else if(!stricmp( name, "SDLK_7" ) ) return 6;
	else if(!stricmp( name, "SDLK_8" ) ) return 7;
	else if(!stricmp( name, "SDLK_9" ) ) return 8;
	else if(!stricmp( name, "SDLK_10" ) ) return 9;
	else if(!stricmp( name, "SDLK_q" ) ) return 10;
	else if(!stricmp( name, "SDLK_w" ) ) return 11;
	else if(!stricmp( name, "SDLK_e" ) ) return 12;
	else if(!stricmp( name, "SDLK_r" ) ) return 13;
	else if(!stricmp( name, "SDLK_t" ) ) return 14;
	else if(!stricmp( name, "SDLK_y" ) ) return 15;
	else if(!stricmp( name, "SDLK_u" ) ) return 16;
	else if(!stricmp( name, "SDLK_i" ) ) return 17;
	else if(!stricmp( name, "SDLK_o" ) ) return 18;
	else if(!stricmp( name, "SDLK_p" ) ) return 19;
	else if(!stricmp( name, "SDLK_a" ) ) return 20;
	else if(!stricmp( name, "SDLK_s" ) ) return 21;
	else if(!stricmp( name, "SDLK_d" ) ) return 22;
	else if(!stricmp( name, "SDLK_f" ) ) return 23;
	else if(!stricmp( name, "SDLK_g" ) ) return 24;
	else if(!stricmp( name, "SDLK_h" ) ) return 25;
	else if(!stricmp( name, "SDLK_j" ) ) return 26;
	else if(!stricmp( name, "SDLK_k" ) ) return 27;
	else if(!stricmp( name, "SDLK_l" ) ) return 28;
	else if(!stricmp( name, "SDLK_SEMICOLON" ) ) return 29;
	else if(!stricmp( name, "SDLK_z" ) ) return 30;
	else if(!stricmp( name, "SDLK_x" ) ) return 31;
	else if(!stricmp( name, "SDLK_c" ) ) return 32;
	else if(!stricmp( name, "SDLK_v" ) ) return 33;
	else if(!stricmp( name, "SDLK_b" ) ) return 34;
	else if(!stricmp( name, "SDLK_n" ) ) return 35;
	else if(!stricmp( name, "SDLK_m" ) ) return 36;
	else if(!stricmp( name, "SDLK_COMMA" ) ) return 37;
	else if(!stricmp( name, "SDLK_PERIOD" ) ) return 38;
	else if(!stricmp( name, "SDLK_SLASH" ) ) return 39;
	return -1;
}

int moConsole::ProcessSessionKey( const moDataSessionKey & p_session_key ) {
  moMoldeoActionType action = p_session_key.GetActionType();
  //APPLY!!!!
  switch( action ) {
    case MO_ACTION_CONSOLE_PLAY:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_CONSOLE_PLAY" );
      ConsolePlay();
      break;
    case MO_ACTION_CONSOLE_PAUSE:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_CONSOLE_PAUSE" );
      ConsolePause();
      break;
    case MO_ACTION_CONSOLE_STOP:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_CONSOLE_STOP" );
      ConsoleStop();
      break;

    case MO_ACTION_PRECONFIG_SET:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_PRECONFIG_SET" );
      SetPreconf( p_session_key.m_ObjectId, p_session_key.m_PreconfId  );
      break;

    case MO_ACTION_VALUE_SET:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_VALUE_SET" );
      SetValue( p_session_key.m_ObjectId, p_session_key.m_ParamId, p_session_key.m_ValueId, p_session_key.m_Value );
      break;

    case MO_ACTION_PARAM_SET:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_PARAM_SET" );
      SetParam( p_session_key.m_ObjectId, p_session_key.m_ParamId, p_session_key.m_ParamDefinition );
      break;

    case MO_ACTION_EFFECT_ENABLE:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_EFFECT_ENABLE" );
      ObjectEnable( p_session_key.m_ObjectId );
      break;

    case MO_ACTION_EFFECT_DISABLE:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_EFFECT_DISABLE" );
      ObjectDisable( p_session_key.m_ObjectId );
      break;

    case MO_ACTION_EFFECT_PLAY:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_EFFECT_PLAY" );
      EffectPlay( p_session_key.m_ObjectId );
      break;

    case MO_ACTION_EFFECT_PAUSE:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_EFFECT_PAUSE" );
      EffectPause( p_session_key.m_ObjectId );
      break;

    case MO_ACTION_EFFECT_PLAY_PAUSE:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_EFFECT_PLAY_PAUSE" );
      EffectPlayPause( p_session_key.m_ObjectId );
      break;

    case MO_ACTION_EFFECT_STOP:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_EFFECT_STOP" );
      EffectStop( p_session_key.m_ObjectId );
      break;

    case MO_ACTION_UNDEFINED:
      break;
    case MO_ACTION_VALUE_ADD:
    case MO_ACTION_VALUE_DELETE:
    case MO_ACTION_VALUE_SAVE:
    case MO_ACTION_VALUE_GET:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_VALUE_###" );
      break;
    case MO_ACTION_CONSOLE_PRESET_ADD:
    case MO_ACTION_CONSOLE_PRESET_DELETE:
    case MO_ACTION_CONSOLE_PRESET_SAVE:
    case MO_ACTION_CONSOLE_PRESET_SET:
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type processed: MO_ACTION_CONSOLE_PRESET_###" );
      break;
    default:
      {
      moDataSessionKey sk = p_session_key;
      moText json = sk.ToJSON();
      MODebug2->Message( "moConsole::ProcessSessionKey > Action Type not processed: " + json );
      }
      break;
  };

  return 0;
}

int moConsole::ProcessSessionEventKey( const moDataSessionEventKey & p_session_event_key ) {

  moDataSessionEventKey sek = p_session_event_key;

  //int mtimecode = sek.GetTimecode();

  return 0;

}

int
moConsole::Save( const moText& p_save_filename ) {
  bool save_success = true;
  int fx=0;
  for( fx=0; fx<(int)m_MoldeoObjects.Count(); fx++ ) {
    moMoldeoObject* MObj = m_MoldeoObjects[fx];
    if (MObj) {
        moMobDefinition MobDef = MObj->GetMobDefinition();
        int valueindex = MobDef.GetMobIndex().GetValueIndex();
        int paramindex = MobDef.GetMobIndex().GetParamIndex();

        moValueBase& vCFG_Effect_Config( GetConfig()->GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_CONFIG) );
        vCFG_Effect_Config.SetText( MobDef.GetConfigName() );

        moValueBase& vCFG_Effect_Label( GetConfig()->GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_LABEL) );
        vCFG_Effect_Label.SetText( MobDef.GetLabelName() );

        moValueBase& vCFG_Effect_Name( GetConfig()->GetParam( paramindex ).GetValue( valueindex ).GetSubValue(0) );
        vCFG_Effect_Name.SetText( MobDef.GetName() );

        moValueBase& vCFG_Effect_Activate( GetConfig()->GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_ON) );
        vCFG_Effect_Activate.SetInt( (int)(MobDef.GetActivate()) );

        moValueBase& vCFG_Effect_KeyName( GetConfig()->GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_KEY) );
        vCFG_Effect_KeyName.SetText( MobDef.GetKeyName() );

        save_success = save_success && (MObj->Save( "" )>0);
    }
  }
  int res = moMoldeoObject::Save( p_save_filename );
  save_success = save_success && (res>0);
  return res;
}


void moConsole::ConsoleModeUpdate() {

  /**
  chequear que haya una sesion activa en el DataManager...
  */
  moDataSession* loadedSession = GetResourceManager()->GetDataMan()->GetSession();
  /**
  moldeoplayer -mol xx.mol -mos session.mos -outputmode "AUTOPLAY" -mode "LIVE"|"RECORD"|"PLAYBACK"|"RENDER"
  */
  //session object
  if (loadedSession==NULL) { MODebug2->Error("moConsole::ConsoleModeUpdate > no session"); return; }


  switch( m_ConsoleState.m_Mode ) {

    case MO_CONSOLE_MODE_LIVE:
      {
      //normal mode: do nothing
      	m_ConsoleState.tempo.Duration();
        m_ConsoleState.tempo.getTempo();
      }
      break;

    case MO_CONSOLE_MODE_PLAY_SESSION:
      {
      //playing back last session loaded
      //
        //loaded/loading session
        if (!loadedSession->Loaded())
            if (!loadedSession->LoadSession()) { MODebug2->Error("moConsole::ConsoleModeUpdate > no loaded session"); return; }

        //session ended?
        if (loadedSession->SessionEnded()) { MODebug2->Message("moConsole::ConsoleModeUpdate > session playback ended."); return; }

        // 1000 / 20fps = 50 ms
        // 1000 / 24fps = 41 ms = 24 frames + 16/24
        // 1000 / 25fps = 40 ms = 25 frames
        // 1000 / 30fps = 50 ms
        m_ConsoleState.step_interval = 41;

        moGetTicksAbsoluteStep( m_ConsoleState.step_interval );

        //process next keys
        ProcessSessionKey( loadedSession->NextKey( m_ConsoleState ) );

      }
      break;

    case MO_CONSOLE_MODE_RECORD_SESSION:
      {
        m_ConsoleState.tempo.Duration();
        m_ConsoleState.tempo.getTempo();

      }
      break;

    case MO_CONSOLE_MODE_RENDER_SESSION:
      {
        m_ConsoleState.step_interval = 41;
        long tickis = moGetTicksAbsoluteStep( m_ConsoleState.step_interval );
        MODebug2->Message("moConsole::ConsoleModeUpdate > render session: " + IntToStr(tickis) );
        int console_timecode = m_ConsoleState.tempo.Duration();

        if (console_timecode==0)  {
              m_ConsoleState.tempo.Fix();
              m_ConsoleState.tempo.Stop();
              m_ConsoleState.tempo.Start();
        }

        m_ConsoleState.tempo.getTempo();

        MODebug2->Message("moConsole::ConsoleModeUpdate > render session: m_ConsoleState.tempo.Duration: " + IntToStr(console_timecode) );

        ProcessSessionKey( loadedSession->NextKey( m_ConsoleState ) );
        loadedSession->StepRender( m_ConsoleState );

      }
      break;

    default:
      /**  */
      break;
  };
}

void moConsole::ConsolePlaySession() {
  MODebug2->Message("moConsole::ConsolePlaySession");
  if (m_pResourceManager==NULL) return;
  GetResourceManager()->GetDataMan()->GetSession()->Playback(m_ConsoleState);
}

void moConsole::ConsoleRecordSession() {
  MODebug2->Message("moConsole::ConsoleRecordSession ");
  if (m_pResourceManager==NULL) return;
  m_pResourceManager->GetDataMan()->GetSession()->Record( m_ConsoleState );

}

void moConsole::ConsoleRenderSession( const moText& p_frame_quality ) {
  MODebug2->Message("moConsole::ConsoleRenderSession");
  if (m_pResourceManager==NULL) return;
  m_ConsoleState.m_RenderFrameQuality = p_frame_quality;
  m_pResourceManager->GetDataMan()->GetSession()->Render( m_ConsoleState );
}

void moConsole::ConsoleSaveSessionAs() {
  MODebug2->Message("moConsole::ConsoleSaveSessionAs");
  if (m_pResourceManager==NULL) return;
}

void moConsole::ConsolePlay() {

    if (m_ConsoleState.tempo.State()!=MO_TIMERSTATE_PLAYING) {
      m_ConsoleState.tempo.Start();
    }

    if (moIsTimerPaused()) {
        moContinueTimer();
    } else {
        moStartTimer();
    }

    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      if (m_pResourceManager==NULL) return;
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_CONSOLE_PLAY );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }
}

void moConsole::ConsolePause() {
    moPauseTimer();
    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      if (m_pResourceManager==NULL) return;
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_CONSOLE_PAUSE );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }
}

void moConsole::ConsoleStop() {
    moStopTimer();
    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      if (m_pResourceManager==NULL) return;
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_CONSOLE_STOP );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }

}

moConsoleMode moConsole::GetConsoleMode() {

    return m_ConsoleState.m_Mode;
}

moTimerState moConsole::GetConsoleState() {

    return moGetTimerState();
}

int
moConsole::SetEffectState( int m_MoldeoObjectId, const moEffectState& p_effect_state ) {
  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object && Object->GetConfig()) {

    moEffect* Effect = ((moEffect*)Object);
    Effect->SetEffectState( p_effect_state );

    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_EFFECT_SETSTATE, Object->GetId(),  p_effect_state );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }

    return 1;
  } else return -1;
  return 0;
}

int
moConsole::SetParam( int m_MoldeoObjectId, int m_ParamId, const moParamDefinition &p_param_definition ) {

  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object && Object->GetConfig()) {
    moParam& Param( Object->GetConfig()->GetParam(m_ParamId));
    moParamDefinition mpdef = p_param_definition;
    Param.SetParamDefinition( mpdef );

    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_PARAM_SET, Object->GetId(),  m_ParamId, Param.GetParamDefinition() );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }

    return 1;
  }
  return 0;
}

int moConsole::SetValue( int m_MoldeoObjectId, int m_ParamId, int m_ValueId, const moValue &p_value ) {

  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object && Object->GetConfig()) {
    moParam& Param( Object->GetConfig()->GetParam(m_ParamId));
    moValue& Value( Param.GetValue(m_ValueId) );
    Value = p_value;
    Object->ResolveValue( Param, m_ValueId );

    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_VALUE_SET, Object->GetId(),  m_ParamId, m_ValueId, p_value );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }

    return 1;
  }

  return 0;
}

int moConsole::RefreshValue( int m_MoldeoObjectId, int m_ParamId, int m_ValueId, bool p_Refresh ) {

  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );

  if (Object && Object->GetConfig()) {
    moParam& Param( Object->GetConfig()->GetParam(m_ParamId));
    //moValue& Value( Param.GetValue(m_ValueId) );
    if (p_Refresh) Object->RefreshValue( Param, m_ValueId );

    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_VALUE_REFRESH, Object->GetId(),  m_ParamId, m_ValueId, -1 );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }

    return 1;
  }

  return 0;
}

int
moConsole::EffectPlay( int m_MoldeoObjectId ) {
  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object) {
    moEffect* fxEffect = (moEffect*)Object;
    fxEffect->Play();
    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_EFFECT_PLAY, Object->GetId()  );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }
    return 1;
  }
  return 0;
}

int
moConsole::EffectPlayPause( int m_MoldeoObjectId ) {
  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object) {
    moEffect* fxEffect = (moEffect*)Object;
    fxEffect->PlayPause();
    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_EFFECT_PLAY_PAUSE, Object->GetId()  );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }
    return 1;
  }
  return 0;
}

int
moConsole::EffectPause( int m_MoldeoObjectId ) {
  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object) {
    moEffect* fxEffect = (moEffect*)Object;
    fxEffect->Pause();
    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_EFFECT_PAUSE, Object->GetId()  );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }
    return 1;
  }
  return 0;
}


int
moConsole::EffectStop( int m_MoldeoObjectId ) {
  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object) {
    moEffect* fxEffect = (moEffect*)Object;
    fxEffect->Stop();
    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_EFFECT_STOP, Object->GetId()  );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }
    return 1;
  }
  return 0;
}

int
moConsole::ObjectEnable( int m_MoldeoObjectId ) {

  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object) {

        if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
          moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_OBJECT_ENABLE, Object->GetId()  );
          GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
        }

        switch ( Object->GetType() ) {
            case MO_OBJECT_EFFECT:
            case MO_OBJECT_PREEFFECT:
            case MO_OBJECT_POSTEFFECT:
            case MO_OBJECT_MASTEREFFECT:
              {
                moEffect* pEffect = (moEffect*) Object;
                //pEffect->Enable();
                pEffect->TurnOn();
              }
                return 1;
                break;
            case MO_OBJECT_IODEVICE:
            case MO_OBJECT_RESOURCE:
                {
                Object->Activate();
                return 1;
                }
                break;
            default:
                break;
        };

  } else return -1;

  return 0;
}

int
moConsole::ObjectDisable( int m_MoldeoObjectId ) {

  moMoldeoObject* Object = this->GetObjectByIdx( m_MoldeoObjectId );
  if (Object) {

        if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
          moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_OBJECT_DISABLE, Object->GetId()  );
          GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
        }

        switch ( Object->GetType() ) {
            case MO_OBJECT_EFFECT:
            case MO_OBJECT_PREEFFECT:
            case MO_OBJECT_POSTEFFECT:
            case MO_OBJECT_MASTEREFFECT:
                {
                  moEffect* pEffect = (moEffect*) Object;
                  //pEffect->Enable();
                  pEffect->TurnOff();
                }
                return 1;
                break;
            case MO_OBJECT_IODEVICE:
            case MO_OBJECT_RESOURCE:
                {
                Object->Deactivate();
                return 1;
                }
                break;
            default:
                break;
        };

  } else return -1;

  return 0;
}


int moConsole::GetPreset() {

    return 0;
}

void moConsole::SetPreset( int presetid ) {
  //
  m_PresetParams.Get( presetid );
}

int moConsole::GetPreconf( int objectid ) {
    moMoldeoObject* MOB;
    MOB = m_MoldeoObjects.Get(objectid);
    return MOB->GetConfig()->GetCurrentPreConf();
}

void moConsole::SetPreconf( int objectid, int preconfid ) {
    moMoldeoObject* Object = GetObjectByIdx(objectid);
    if (Object && Object->GetConfig()) {
      Object->GetConfig()->SetCurrentPreConf( preconfid );
    }

    if (m_ConsoleState.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
      moDataSessionKey key( moGetTicksAbsolute(), MO_ACTION_PRECONFIG_SET, objectid, preconfid );
      GetResourceManager()->GetDataMan()->GetSession()->AddKey( key );
    }
}


void moConsole::SetTicks( int ticksid ) {
    moSetDuration( ticksid );
}

int moConsole::GetObjectId( const moText& p_objectlabelname ) {

    for( MOuint i=0; i<m_MoldeoObjects.Count(); i++) {

        if (p_objectlabelname == m_MoldeoObjects[i]->GetLabelName()) {
            return m_MoldeoObjects[i]->GetId();
        }

    }

    for( MOuint i=0; i<m_MoldeoSceneObjects.Count(); i++) {

        if (p_objectlabelname == m_MoldeoSceneObjects[i]->GetLabelName()) {
            return m_MoldeoSceneObjects[i]->GetId();
        }

    }

    return -1;
}

moMoldeoObject* moConsole::GetObjectByIdx( int p_object_id ) {

  moMoldeoObject* returnMOB = NULL;
  //if ( p_object_id )
  int object_id_to_index =  p_object_id - MO_MOLDEOOBJECTS_OFFSET_ID;

  if (0<=object_id_to_index && object_id_to_index<(int)m_MoldeoObjects.Count()) {
    returnMOB = m_MoldeoObjects[object_id_to_index];
    return returnMOB;
  }

  int object_id_to_subscene_index =  p_object_id - MO_MOLDEOSCENEOBJECTS_OFFSET_ID;

  if (0<=object_id_to_subscene_index && object_id_to_subscene_index<(int)m_MoldeoSceneObjects.Count()) {
    returnMOB = m_MoldeoSceneObjects[object_id_to_subscene_index];
    return returnMOB;
  }

  return returnMOB;
}


int moConsole::GetDirectoryFileCount( const moText& p_path ) {

    moDirectory* pDir;
    pDir = NULL;
    moText completepath;
    completepath = m_pResourceManager->GetDataMan()->GetDataPath() + (moText)p_path;
    pDir = m_pResourceManager->GetFileMan()->GetDirectory(completepath);
    if (pDir) {
        pDir->Update();
        return pDir->GetFiles().Count();
    }

    return -1;
}

void moConsole::RegisterFunctions()
{
    ///first inherit methods from MoldeoObjects
    moMoldeoObject::RegisterFunctions();

    ///register our own methods starting with RegisterBaseFunction
    RegisterBaseFunction("Play");//0
    RegisterFunction("Pause");//1
    RegisterFunction("Stop");//2
    RegisterFunction("State");//3
    RegisterFunction("GetObjectId");//4

    RegisterFunction("GetPreset");//5
    RegisterFunction("SetPreset");//6
    RegisterFunction("SetTicks");//7
    RegisterFunction("GetTicks");//8

    RegisterFunction("GetObjectPreconf");//9
    RegisterFunction("SetObjectPreconf");//10
    RegisterFunction("ObjectEnable");//11
    RegisterFunction("ObjectDisable");//12

    RegisterFunction("GetObjectParamIndex");//13
    RegisterFunction("GetObjectParamValues");//14
    RegisterFunction("GetObjectCurrentValue");//15
    RegisterFunction("SetObjectCurrentValue");//16


    RegisterFunction("GetObjectDataIndex");//17
    RegisterFunction("GetObjectData");//18
    RegisterFunction("SetObjectData");//19

    ///Specific for effects

    RegisterFunction("GetEffectState");//20
    RegisterFunction("SetEffectState");//21


    ///Specific for devices

    RegisterFunction("GetDeviceCode");//22
    RegisterFunction("GetDeviceCodeId");//23
    RegisterFunction("AddEvent");//24

    ///GENERAL
    RegisterFunction("GetDirectoryFileCount");//25
    RegisterFunction("Screenshot");//26

    RegisterFunction("EffectPlay");//27
    RegisterFunction("EffectStop");//28
    RegisterFunction("EffectPause");//29
    RegisterFunction("EffectTimerState");//30

    ResetScriptCalling();

}

int moConsole::ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber)
{

    switch ( iFunctionNumber - m_iMethodBase )
    {
        case 0:
            ResetScriptCalling();
            return luaPlay(vm);
        case 1:
            ResetScriptCalling();
            return luaPause(vm);
        case 2:
            ResetScriptCalling();
            return luaStop(vm);
        case 3:
            ResetScriptCalling();
            return luaState(vm);
        case 4:
            ResetScriptCalling();
            return luaGetObjectId(vm);
        case 5:
            ResetScriptCalling();
            return luaGetPreset(vm);
        case 6:
            ResetScriptCalling();
            return luaSetPreset(vm);
        case 7:
            ResetScriptCalling();
            return luaSetTicks(vm);
        case 8:
            ResetScriptCalling();
            return luaGetTicks(vm);



        case 9:
            ResetScriptCalling();
            return luaGetObjectPreconf(vm);
        case 10:
            ResetScriptCalling();
            return luaSetObjectPreconf(vm);
        case 11:
            ResetScriptCalling();
            return luaObjectEnable(vm);
        case 12:
            ResetScriptCalling();
            return luaObjectDisable(vm);



        case 13:
            ResetScriptCalling();
            return luaGetObjectParamIndex(vm);
        case 14:
            ResetScriptCalling();
            return luaGetObjectParamValues(vm);
        case 15:
            ResetScriptCalling();
            return luaGetObjectCurrentValue(vm);
        case 16:
            ResetScriptCalling();
            return luaSetObjectCurrentValue(vm);

        case 17:
            ResetScriptCalling();
            return luaGetObjectDataIndex(vm);
        case 18:
            ResetScriptCalling();
            return luaGetObjectData(vm);
        case 19:
            ResetScriptCalling();
            return luaSetObjectData(vm);


        ///for Effects
        case 20:
            ResetScriptCalling();
            return luaGetEffectState(vm);
        case 21:
            ResetScriptCalling();
            return luaSetEffectState(vm);


        ///for IODevices
        case 22:
            ResetScriptCalling();
            return luaGetDeviceCode(vm);
        case 23:
            ResetScriptCalling();
            return luaGetDeviceCodeId(vm);

        case 24:
            ResetScriptCalling();
            return luaAddEvent(vm);

        ///special case FileManager
        case 25:
            ResetScriptCalling();
            return luaGetDirectoryFileCount(vm);

        case 26:
            ResetScriptCalling();
            return luaScreenshot(vm);

        case 27:
            ResetScriptCalling();
            return luaEffectPlay(vm);
        case 28:
            ResetScriptCalling();
            return luaEffectStop(vm);
        case 29:
            ResetScriptCalling();
            return luaEffectPause(vm);
        case 30:
            ResetScriptCalling();
            return luaEffectTimerState(vm);


        default:
            NextScriptCalling();
            return moMoldeoObject::ScriptCalling( vm, iFunctionNumber );
	}
}

int moConsole::luaPlay( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;
    if (state)
      ConsolePlay();

    return 0;
}

int moConsole::luaPause( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;
    if (state)
      ConsolePause();

    return 0;
}

int moConsole::luaStop( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;

    if (state)
      ConsoleStop();

    return 0;
}

int moConsole::luaState( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;

    moTimerState elstate = GetConsoleState();
    int retstate = (int) elstate;
    lua_pushnumber( state, (lua_Number) retstate);

    return 1;
}

int moConsole::luaGetObjectId(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    char *objectlabelname = (char *) lua_tostring (state, 1);

    int objectid = -1;

    objectid = this->GetObjectId( objectlabelname );

    lua_pushnumber(state, (lua_Number) objectid );

    if (objectid==-1) {
        MODebug2->Error( moText("Object doesnt exists: ")+(moText)objectlabelname );
    }

    return 1;
}

int moConsole::luaGetPreset(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    lua_pushnumber(state, (lua_Number) this->GetPreset() );

    return 1;
}

int moConsole::luaSetPreset(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint presetid = (MOint) lua_tonumber (state, 1);

    this->SetPreset( presetid );

    return 0;
}

int moConsole::luaGetObjectPreconf(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        lua_pushnumber(state, (lua_Number) Object->GetConfig()->GetCurrentPreConf() );
    } else {
        lua_pushnumber(state, (lua_Number) -1 );
        MODebug2->Error( moText("in moConsole script: GetObjectPreconf : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 1;
}

int moConsole::luaSetObjectPreconf(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    MOint preconfid = (MOint) lua_tonumber (state, 2);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        SetPreconf( objectid, preconfid );
    } else {
        MODebug2->Error( moText("in console script: SetObjectPreconf : object not founded : id:")+(moText)IntToStr(objectid)+moText(" preconfid:")+(moText)IntToStr(preconfid) );
    }

    return 0;
}

int moConsole::luaSetTicks(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint ticksint = (MOint) lua_tonumber (state, 1);

    this->SetTicks(ticksint);

    return 0;
}

int moConsole::luaGetTicks(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    lua_pushnumber(state, (lua_Number) moGetTicks() );

    return 1;
}

int moConsole::luaObjectEnable(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        ObjectEnable( objectid );
    } else {
        MODebug2->Error( moText("in console script: ObjectEnable : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}

int moConsole::luaObjectDisable(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        ObjectDisable( objectid );
    } else {
        MODebug2->Error( moText("in console script: ObjectDisable : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}

int moConsole::luaEffectPlay(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig() && (
        (Object->GetMobDefinition().GetType()>=MO_OBJECT_EFFECT &&
        Object->GetMobDefinition().GetType()<=MO_OBJECT_MASTEREFFECT)
        || Object->GetMobDefinition().GetType()<=MO_OBJECT_CONSOLE
    )) {
        EffectPlay(Object->GetId());
    } else {
        MODebug2->Error( moText("in console script: EffectPlay : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}

int moConsole::luaEffectPlayPause(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig() && (
        (Object->GetMobDefinition().GetType()>=MO_OBJECT_EFFECT &&
        Object->GetMobDefinition().GetType()<=MO_OBJECT_MASTEREFFECT)
        || Object->GetMobDefinition().GetType()<=MO_OBJECT_CONSOLE
    )) {
        EffectPlayPause(Object->GetId());
    } else {
        MODebug2->Error( moText("in console script: EffectPlayPause : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}

int moConsole::luaEffectStop(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig() && (
        (Object->GetMobDefinition().GetType()>=MO_OBJECT_EFFECT &&
        Object->GetMobDefinition().GetType()<=MO_OBJECT_MASTEREFFECT)
        || Object->GetMobDefinition().GetType()<=MO_OBJECT_CONSOLE
    )) {
        EffectStop(Object->GetId());
    } else {
        MODebug2->Error( moText("in console script: EffectStop : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}

int moConsole::luaEffectPause(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig() && (
        (Object->GetMobDefinition().GetType()>=MO_OBJECT_EFFECT &&
        Object->GetMobDefinition().GetType()<=MO_OBJECT_MASTEREFFECT)
        || Object->GetMobDefinition().GetType()<=MO_OBJECT_CONSOLE
    )) {
        EffectPause(Object->GetId());
    } else {
        MODebug2->Error( moText("in console script: EffectPause : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}


int moConsole::luaEffectTimerState(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = NULL;

    Object = GetObjectByIdx(objectid);

    moText playstr = "none";

    if (Object && Object->GetConfig() && (
        (Object->GetMobDefinition().GetType()>=MO_OBJECT_EFFECT &&
        Object->GetMobDefinition().GetType()<=MO_OBJECT_MASTEREFFECT)
        || Object->GetMobDefinition().GetType()<=MO_OBJECT_CONSOLE
    )) {
        moEffect* fxEffect = (moEffect*)Object;
        if (fxEffect) {
            moTimer ptimer = fxEffect->GetEffectState().tempo;
            playstr = ptimer.StateToStr();
        }
    }

    lua_pushstring( state, playstr );

    return 1;
}


int moConsole::luaGetObjectParamIndex(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    char *text = (char *) lua_tostring (state, 2);

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        if (Object->GetConfig()->GetParamIndex(text)<0) {
         MODebug2->Error( moText("moConsole::luaGetObjectParamIndex > param not found: ") + text );
        }
        lua_pushnumber(state, (lua_Number) Object->GetConfig()->GetParamIndex(text) );
    }

    return 1;

}


int moConsole::luaGetObjectParamValues(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    char *text = (char *) lua_tostring (state, 2);

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if ( Object && Object->GetConfig() ) {

        if (Object->GetConfig()->GetParamIndex(text)<0) {
         MODebug2->Error( moText("moConsole::luaGetObjectParamValues > param not found: ") + text );
         lua_pushnumber(state, (lua_Number) 0 );
         return 1;
        }
        lua_pushnumber(state, (lua_Number) Object->GetConfig()->GetParam(text).GetValuesCount() );

    }

    return 1;
}


int moConsole::luaSetObjectCurrentValue(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    MOint paramid = (MOint) lua_tonumber (state, 2);
    MOint valueid = (MOint) lua_tonumber (state, 3);

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        Object->GetConfig()->SetCurrentValueIndex( paramid, valueid );

        moParam pParam = Object->GetConfig()->GetParam(paramid);

        //MODebug2->Message( moText("in lua console script: SetObjectCurrentValue")+Object->GetLabelName() + " Param:" + pParam.GetParamDefinition().GetName() );
    } else {
        MODebug2->Error( moText("in console script: SetObjectCurrentValue : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    return 0;
}


int moConsole::luaGetObjectCurrentValue(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    MOint paramid = (MOint) lua_tonumber (state, 2);

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        int valueid = Object->GetConfig()->GetParam( paramid ).GetIndexValue();
        lua_pushnumber(state, (lua_Number) valueid );
        MODebug2->Message( moText("in lua console script: GetObjectCurrentValue")+Object->GetLabelName() );
        return 1;
    } else {
        MODebug2->Error( moText("in lua console script: GetObjectCurrentValue : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    return 0;
}


int moConsole::luaGetObjectDataIndex(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    char *text = (char *) lua_tostring (state, 2);
    int inletid = -1;

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object) {
        inletid = Object->GetInletIndex( text );
    } else {
        MODebug2->Error( moText("in console script: GetObjectDataIndex : object not founded : id:")+(moText)IntToStr(objectid)+moText(" for param:")+moText(text) );
    }

    lua_pushnumber(state, (lua_Number) inletid );

    return 1;
}

int moConsole::luaGetObjectData(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    MOint inletid = (MOint) lua_tonumber (state, 2);

    //MODebug2->Message( "in console script: GetObjectData : moldeo objectid: " + moText((long)objectid) );

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object) {
        //MODebug2->Message( "in console script: GetObjectData : moldeo Object: " + moText((long)Object) );
        //MODebug2->Message( "in console script: GetObjectData : moldeo Object: " + Object->GetLabelName() );
        moInlet* pInlet = Object->GetInlets()->Get(inletid);
        if (pInlet) {
            //MODebug2->Message( "in console script: GetObjectData : moldeo Inlet: " + moText((long)pInlet) );
            //MODebug2->Message( "in console script: GetObjectData : moldeo Inlet: " + pInlet->GetConnectorLabelName() );
            moData* pData = pInlet->GetData();
            if (pData) {
                //MODebug2->Message( "in console script: GetObjectData : moldeo object: " + Object->GetLabelName()
                //                  + " inlet: " + pInlet->GetConnectorLabelName() + " Data:" + moText((long)pData)
                //                  + " Type:" + moText( (int)pData->Type() ) );

                switch(pData->Type()) {

                    case MO_DATA_FUNCTION:
                        lua_pushnumber(state, (lua_Number) pData->Eval() );
                        return 1;

                    case MO_DATA_NUMBER:
                    case MO_DATA_NUMBER_CHAR:
                    case MO_DATA_NUMBER_INT:
                    case MO_DATA_NUMBER_LONG:
                    case MO_DATA_NUMBER_MIDI:
                        //MODebug2->Message( "Inlet Number:" + IntToStr(pData->Long() ) );
                        lua_pushnumber(state, (lua_Number) pData->Long() );
                        return 1;

                    case MO_DATA_3DMODELPOINTER:
                    case MO_DATA_FONTPOINTER:
                    case MO_DATA_IMAGESAMPLE:
                    case MO_DATA_IMAGESAMPLE_FILTERED:
                    case MO_DATA_IMAGESAMPLE_TEXTUREBUFFER:
                        lua_pushnumber(state, (lua_Number) (long)pData->Pointer() );
                        return 1;

                    case MO_DATA_VECTOR2I:
                        lua_pushnumber(state, (lua_Number) pData->Vector2i()->X() );
                        lua_pushnumber(state, (lua_Number) pData->Vector2i()->Y() );
                        return 2;

                    case MO_DATA_VECTOR3I:
                        lua_pushnumber(state, (lua_Number) pData->Vector3i()->X() );
                        lua_pushnumber(state, (lua_Number) pData->Vector3i()->Y() );
                        lua_pushnumber(state, (lua_Number) pData->Vector3i()->Z() );
                        return 3;

                    case MO_DATA_VECTOR4I:
                        lua_pushnumber(state, (lua_Number) pData->Vector4i()->X() );
                        lua_pushnumber(state, (lua_Number) pData->Vector4i()->Y() );
                        lua_pushnumber(state, (lua_Number) pData->Vector4i()->Z() );
                        lua_pushnumber(state, (lua_Number) pData->Vector4i()->W() );
                        return 4;

                    case MO_DATA_VECTOR2F:
                        lua_pushnumber(state, (lua_Number) pData->Vector2d()->X() );
                        lua_pushnumber(state, (lua_Number) pData->Vector2d()->Y() );
                        return 2;

                    case MO_DATA_VECTOR3F:
                        lua_pushnumber(state, (lua_Number) pData->Vector3d()->X() );
                        lua_pushnumber(state, (lua_Number) pData->Vector3d()->Y() );
                        lua_pushnumber(state, (lua_Number) pData->Vector3d()->Z() );
                        return 3;

                    case MO_DATA_VECTOR4F:
                        lua_pushnumber(state, (lua_Number) pData->Vector4d()->X() );
                        lua_pushnumber(state, (lua_Number) pData->Vector4d()->Y() );
                        lua_pushnumber(state, (lua_Number) pData->Vector4d()->Z() );
                        lua_pushnumber(state, (lua_Number) pData->Vector4d()->W() );
                        return 4;

                    case MO_DATA_MESSAGE:
                    case MO_DATA_MESSAGES:
                        lua_pushstring(state, pData->ToText() );
                        return 1;

                    case MO_DATA_NUMBER_DOUBLE:
                    case MO_DATA_NUMBER_FLOAT:
                        lua_pushnumber(state, (lua_Number) pData->Double() );
                        return 1;

                    case MO_DATA_TEXT:
                        lua_pushstring(state, pData->Text() );
                        return 1;

                    default:
                        //MODebug2->Error( moText("in console script: GetObjectData : inlet found but type not available ")+(moText)pData->TypeToText() );
                        MODebug2->Error( moText("in console script: GetObjectData : inlet found but type not available "));
                        break;
                }
            } else {
              MODebug2->Error( moText("in console script: GetObjectData : inlet found but data is null!!!") );
            }
        } else {
            MODebug2->Error( moText("in console script: GetObjectData : inlet not found : id:")+(moText)IntToStr(inletid) );
        }
    } else {
        MODebug2->Error( moText("in console script: GetObjectData : object not found : id:")+(moText)IntToStr(objectid) );
    }

    moText tres("invalid");
    lua_pushstring( state, tres );
    return 1;
}

int moConsole::luaSetObjectData(moLuaVirtualMachine& vm) {
    double deluavalor = -1.0f;
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    MOint inletid = (MOint) lua_tonumber (state, 2);

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object) {
        moInlet* pInlet = Object->GetInlets()->Get(inletid);
        if (pInlet) {
            moData* pData = pInlet->GetInternalData();
            if (pData) {
                switch(pData->Type()) {
                    case MO_DATA_NUMBER:
                    case MO_DATA_NUMBER_CHAR:
                    case MO_DATA_NUMBER_INT:
                    case MO_DATA_NUMBER_LONG:
                    case MO_DATA_NUMBER_MIDI:
                        pData->SetLong( (MOlong) lua_tonumber ( state, 3 ) );
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_3DMODELPOINTER:
                    case MO_DATA_FONTPOINTER:
                    case MO_DATA_IMAGESAMPLE:
                    case MO_DATA_IMAGESAMPLE_FILTERED:
                    case MO_DATA_IMAGESAMPLE_TEXTUREBUFFER:
                        //pData->SetLong( (MOlong) lua_tonumber ( state, 3 ) );
                        return 0;break;

                    case MO_DATA_VECTOR2I:
                        (*pData->Vector2i()) = moVector2i(  (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ) );
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_VECTOR3I:
                        (*pData->Vector3i()) = moVector3i(  (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ),
                                                            (MOlong) lua_tonumber ( state, 5 ) );
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_VECTOR4I:
                        (*pData->Vector4i()) = moVector4i(  (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ),
                                                            (MOlong) lua_tonumber ( state, 5 ),
                                                            (MOlong) lua_tonumber ( state, 6 ) );
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_VECTOR2F:
                        (*pData->Vector2d()) = moVector2d(  (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ));
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_VECTOR3F:
                        (*pData->Vector3d()) = moVector3d(  (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ),
                                                            (MOdouble) lua_tonumber ( state, 5 ));
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_VECTOR4F:
                        (*pData->Vector4d()) = moVector4d(  (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ),
                                                            (MOdouble) lua_tonumber ( state, 5 ),
                                                            (MOdouble) lua_tonumber ( state, 6 ) );
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_MESSAGE:
                    case MO_DATA_MESSAGES:
                        return 0;break;

                    case MO_DATA_NUMBER_DOUBLE:
                    case MO_DATA_NUMBER_FLOAT:
                        deluavalor = (MOdouble) lua_tonumber ( state, 3 );
                        pData->SetDouble( deluavalor );
                        pInlet->Update(true);
                        return 0;break;

                    case MO_DATA_TEXT:
                        //lua_pushstring(state, pData->Text() );
                        pData->SetText( lua_tostring ( state, 3 ) );
                        pInlet->Update(true);
                        return 0;
                        break;
                    default:
                      break;
                }
            }
        } else {
          MODebug2->Error( moText("in console script: SetObjectData : inlet id not found : id:")+(moText)IntToStr(inletid) );
        }
    } else {
        MODebug2->Error( moText("in console script: SetObjectData : object not found : id:")+(moText)IntToStr(objectid) );
    }

    return 0;
}


int moConsole::luaSetEffectState(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);

    moMoldeoObject* Object = GetObjectByIdx(objectid);
    moEffect*       pEffect = NULL;

    moEffectState fxstate;


    if (Object && Object->GetConfig()) {

        switch (Object->GetType()) {
            case MO_OBJECT_EFFECT:
            case MO_OBJECT_PREEFFECT:
            case MO_OBJECT_POSTEFFECT:
            case MO_OBJECT_MASTEREFFECT:
                pEffect = (moEffect*) Object;
                fxstate = pEffect->GetEffectState();
                fxstate.alpha = (MOfloat) lua_tonumber (state, 2);
                fxstate.tint = (MOfloat) lua_tonumber (state, 3);
                fxstate.tintr = (MOfloat) lua_tonumber (state, 4);
                fxstate.tintg = (MOfloat) lua_tonumber (state, 5);
                fxstate.tintb = (MOfloat) lua_tonumber (state, 6);
                fxstate.tempo.ang = (MOfloat) lua_tonumber (state, 7);

                SetEffectState( Object->GetId(), fxstate);
                break;
            default:
                break;
        }

    } else {
        MODebug2->Error( moText("in console script: SetEffectState : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    return 0;

}

int moConsole::luaGetEffectState(moLuaVirtualMachine& vm) {
    lua_State *luastate = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (luastate, 1);

    moMoldeoObject* Object = GetObjectByIdx(objectid);
    moEffect*       pEffect = NULL;


    if (Object && Object->GetConfig()) {
        switch (Object->GetType()) {
            case MO_OBJECT_EFFECT:
            case MO_OBJECT_PREEFFECT:
            case MO_OBJECT_POSTEFFECT:
            case MO_OBJECT_MASTEREFFECT:
                pEffect = (moEffect*) Object;
                lua_pushnumber(luastate, (lua_Number) pEffect->GetEffectState().alpha );
                lua_pushnumber(luastate, (lua_Number) pEffect->GetEffectState().tint );
                lua_pushnumber(luastate, (lua_Number) pEffect->GetEffectState().tintr );
                lua_pushnumber(luastate, (lua_Number) pEffect->GetEffectState().tintg );
                lua_pushnumber(luastate, (lua_Number) pEffect->GetEffectState().tintb );
                lua_pushnumber(luastate, (lua_Number) pEffect->GetEffectState().tempo.ang );
                return 6;

            default:
                MODebug2->Error( moText("in console script: GetEffectState : not an Effect! ") + (moText)Object->GetLabelName()   );
                break;
        }
    } else {
        MODebug2->Error( moText("in console script: GetEffectState : object or config not founded > id: ")
                        +(moText)IntToStr(objectid)
                        + moText(" label name: ")
                        + (moText)Object->GetLabelName()  );
    }

    return 0;
}

int moConsole::luaGetDeviceCode(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    MOint devicecode = (MOint) lua_tonumber (state, 2);
    MOint codevalue = -1;

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        if (Object->GetType()==MO_OBJECT_IODEVICE) {
            moIODevice* pDevice = (moIODevice*) Object;
            if (pDevice->GetStatus(devicecode)) {
                codevalue = pDevice->GetValue( devicecode );
            }
        }
        lua_pushnumber(state, (lua_Number) codevalue );
    } else {
        MODebug2->Error( moText("in console script: GetDeviceCode : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 1;
}

int moConsole::luaGetDeviceCodeId(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1);
    char *devicecodestr = (char *) lua_tostring (state, 2);
    MOint devicecode = -1;

    moMoldeoObject* Object = GetObjectByIdx(objectid);

    if (Object && Object->GetConfig()) {
        if (Object->GetType()==MO_OBJECT_IODEVICE) {
            moIODevice* pDevice = (moIODevice*) Object;
            devicecode = pDevice->GetCode(devicecodestr);
        }
        lua_pushnumber(state, (lua_Number) devicecode );
    } else {
        MODebug2->Error( moText("in console script: GetDeviceCodeId : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 1;
}

int
moConsole::luaAddEvent(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint deviceid = (MOint) lua_tonumber (state, 1);
    MOint devicecode = (MOint) lua_tonumber (state, 2);
    MOint val0 = (MOint) lua_tonumber (state, 3);
    MOint val1 = (MOint) lua_tonumber (state, 4);
    MOint val2 = (MOint) lua_tonumber (state, 5);
    MOint val3 = (MOint) lua_tonumber (state, 6);

    moEventList* pEvents;

    pEvents = m_pIODeviceManager->GetEvents();

    if (pEvents) {
        pEvents->Add( deviceid, devicecode, val0, val1,val2,val3 );
    }

    return 0;

}


int moConsole::luaGetDirectoryFileCount(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    char *pathname = (char *) lua_tostring (state, 1);

    int filecount = -1;

    filecount = this->GetDirectoryFileCount( pathname );

    lua_pushnumber(state, (lua_Number) filecount );

    if (filecount==-1) {
        MODebug2->Error( moText("console lua script: GetDirectoryFileCount > Directory doesn't exist") );
    }

    return 1;

}

int moConsole::luaScreenshot(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    char *pathname = (char *) lua_tostring (state, 1);

    int res = -1;

    //filecount = this->GetDirectoryFileCount( pathname );

    //lua_pushnumber(state, (lua_Number) filecount );
/*
    if (filecount==-1) {
        MODebug2->Error( moText("console lua script: GetDirectoryFileCount > Directory doesn't exist") );
    }
*/
    res = this->m_pResourceManager->GetRenderMan()->Screenshot( moText(pathname), m_LastScreenshot );

    lua_pushnumber(state, (lua_Number) res );

    return 1;

}

const moText& moConsole::ToJSON() {
  return moMoldeoObject::ToJSON();
}

int
moConsole::TestScreen( const moDisplay& p_display_info ) {


    moShaderManager* pSMan;
    moGLManager* pGLMan;
    moRenderManager* pRMan;
    moTextureManager* pTMan;

    if (m_pResourceManager==NULL) {
    m_pResourceManager = new moResourceManager();
    m_pResourceManager->Init( "", "", m_Config, 0, p_display_info.Resolution().Width(), p_display_info.Resolution().Height() );
    //m_pResourceManager->Init( "", "", m_Config, 0, 1280, 720);
    }

    //int tick = moGetTicksAbsolute( true );
    m_ConsoleState.step_interval = 40;
    float stepi = m_ConsoleState.step_interval;
    float steps = moGetTicksAbsoluteStep( m_ConsoleState.step_interval );
    float progress = (steps/stepi)/120.0;

    glClearColor( 1.0f - progress, 1.0f - progress, 1.0f - progress, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if (m_pResourceManager) {
    pSMan = m_pResourceManager->GetShaderMan();
    if (!pSMan) return 0;

    pGLMan = m_pResourceManager->GetGLMan();
    if (!pGLMan) return 0;

    pRMan = m_pResourceManager->GetRenderMan();
    if (!pRMan) return 0;

    pTMan = m_pResourceManager->GetTextureMan();
    if (!pTMan) return 0;

    } else return 0;





/**

CORE PLANET


*/

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //moPlaneGeometry Plane2( 1.0, 1.0, 1, 1 );

    ///MATERIAL
    moMaterial Mat;
      Mat.m_Map = pTMan->GetTexture(pTMan->GetTextureMOId( "default", false ));
      Mat.m_MapGLId = Mat.m_Map->GetGLId();
      Mat.m_Color = moColor( 1.0, 1.0, 1.0 );
      Mat.m_fTextWSegments = 13.0f;
      Mat.m_fTextHSegments = 13.0f;
      Mat.m_vLight = moVector3f( -1.0, -1.0, -1.0 );
      Mat.m_vLight.Normalize();
      //Mat.m_PolygonMode = MO_POLYGONMODE_LINE;
      Mat.m_PolygonMode = MO_POLYGONMODE_FILL;
      Mat.m_fWireframeWidth = 0.0005f;

    ///GEOMETRY
    moSphereGeometry Sphere( 0.5, 13, 13 );

    ///MESH MODEL (aka SCENE NODE)
    moGLMatrixf Model;
    Model.MakeIdentity()
         .Rotate(   360.0*progress*moMathf::DEG_TO_RAD, 0.0, 1.0, 0.0 )
         .Translate(    0.0, 0.0, -2.618 + 0.618*progress );
    moMesh Mesh( Sphere, Mat );
    Mesh.SetModelMatrix(Model);

    ///CAMERA PERSPECTIVE
    moCamera3D Camera3D;
    pGLMan->SetDefaultPerspectiveView( p_display_info.Resolution().Width(), p_display_info.Resolution().Height() );
    //  Camera3D.MakePerspective(60.0f, p_display_info.Proportion(), 0.01f, 1000.0f );
    Camera3D = pGLMan->GetProjectionMatrix();

    ///RENDERING
    pRMan->Render( &Mesh, &Camera3D );

/**

LOGO PLENO (sin perspectiva)


*/
    glClear( GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ///MESH MATERIAL
    moMaterial Mat2;
    Mat2.m_Map = pTMan->GetTexture(pTMan->GetTextureMOId( "moldeotrans", false ));
    Mat2.m_MapGLId = Mat2.m_Map->GetGLId();
    Mat2.m_Color = moColor( 1.0, 1.0, 1.0 );
    Mat2.m_vLight = moVector3f( -1.0, -1.0, -1.0 );
    Mat2.m_vLight.Normalize();

    ///MESH GEOMETRY
    moPlaneGeometry Plane3( 1.0, 0.33, 1, 1 );

    ///MESH MODEL
    moGLMatrixf Model2;
    Model2.MakeIdentity();
    Model2.Scale( 1.0, 1.0, 1.0 );
    Model2.Translate( 0.0, 0.0, 0.0 );
    moMesh Mesh2( Plane3, Mat2 );
    Mesh2.SetModelMatrix(Model2);

    ///CAMERA PERSPECTIVE
    moCamera3D Camera3D2;
    pGLMan->SetDefaultOrthographicView( p_display_info.Resolution().Width(), p_display_info.Resolution().Height() );
    Camera3D2 = pGLMan->GetProjectionMatrix();

    ///RENDERING
    pRMan->Render( &Mesh2, &Camera3D2 );



    return 1;
}
