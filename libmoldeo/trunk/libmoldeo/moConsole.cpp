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
  Andres Colubri

*******************************************************************************/

#include <moConsole.h>

#ifdef MO_WIN32
  #include <SDL.h>
#endif

#ifdef MO_LINUX
    #include <SDL/SDL.h>
#endif

#ifdef MO_MACOSX
    #include <SDL/SDL.h>
#endif

#include <moArray.h>

#include <moDataManager.h>
#include <moFileManager.h>

moDefineDynamicArray( moPresetParams )


moConsole::moConsole() {

	m_bIODeviceManagerDefault = true;
	m_pIODeviceManager = NULL;
	m_pResourceManager = NULL;

	m_bExternalResources = false;

    ///Important settings for CONSOLE (using inlets and outlets...)
    this->SetName("__console__");
    this->SetLabelName("__console__");
    this->SetType( MO_OBJECT_CONSOLE );

}

moConsole::~moConsole()
{
    Finish();
}

moEffectManager& moConsole::GetEffectManager() {

    return m_EffectManager;

}

void moConsole::SetIODeviceManager( moIODeviceManager*	p_IODeviceManager ) {

	if (p_IODeviceManager!=NULL)
		m_pIODeviceManager = p_IODeviceManager;

}

void moConsole::InitResources( moResourceManager *pResourceManager,
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
		m_pResourceManager->Init( p_datapath,
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
	return Init( moText("data"), moText("data/console.mol"), NULL, NULL, RENDERMANAGER_MODE_NORMAL,
		MO_DEF_SCREEN_WIDTH,
		MO_DEF_SCREEN_HEIGHT,
		MO_DEF_RENDER_WIDTH,
		MO_DEF_RENDER_HEIGHT );

}


MOboolean moConsole::Init( moText p_datapath,
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
	int verif;
	//int a,b;

	idebug = -1;
	iligia = -1;
	iborrado = -1;
    m_ConsoleScript = moText("");

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

    if (MODebug2) MODebug2->Message(moText("moConsole:: Initializing Effects Manager."));
	m_EffectManager.Init();

	//Inicializando Estado de la Consola
	state.Init();

    //==========================================================================
	//   CARGAMOS EL ARCHIVO DE CONFIGURACION
	//==========================================================================

    if (MODebug2) MODebug2->Message(moText("moConsole:: Opening Console Config Project (.mol).")  + (moText)p_consoleconfig);

    this->GetDefinition();
	verif = m_Config.LoadConfig( p_consoleconfig ) ;//este parametro debe pasarse desde fuera
	if(verif != MO_CONFIG_OK) {

        if (MODebug2) MODebug2->Error(moText("moConsole::  .mol file invalid, check XML syntax..."));

	    Finish();
		//then
		return false;
	} else SetConfigName(p_consoleconfig);

    if (MODebug2) MODebug2->Message(moText("moConsole:: mol project opening....success "));

	moDefineParamIndex( CONSOLE_OUTPUTMODE, moText("outputmode") );
	moDefineParamIndex( CONSOLE_OUTPUTRESOLUTION, moText("outputresolution") );
	moDefineParamIndex( CONSOLE_RENDERMODE, moText("rendermode") );
	moDefineParamIndex( CONSOLE_RENDERRESOLUTION, moText("renderresolution") );

    //if () {
        moText mode = m_Config[moR(CONSOLE_OUTPUTMODE)][MO_SELECTED][0].Text();

        moText rendermode = m_Config[moR(CONSOLE_RENDERMODE)][MO_SELECTED][0].Text();
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

        moText renderwidth = m_Config[moR(CONSOLE_RENDERRESOLUTION)][MO_SELECTED][0].Text();
        moText renderheight = m_Config[moR(CONSOLE_RENDERRESOLUTION)][MO_SELECTED][1].Text();

        moText screenwidth = m_Config[moR(CONSOLE_OUTPUTRESOLUTION)][MO_SELECTED][0].Text();
        moText screenheight = m_Config[moR(CONSOLE_OUTPUTRESOLUTION)][MO_SELECTED][1].Text();


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
/*
        moText clip1_p1_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][0].Text();
        moText clip1_p1_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][1].Text();

        moText clip1_p2_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][2].Text();
        moText clip1_p2_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][3].Text();

        moText clip1_p3_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][4].Text();
        moText clip1_p3_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][5].Text();

        moText clip1_p4_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][6].Text();
        moText clip1_p4_x = m_Config[moR(CONSOLE_CLIP1)][MO_SELECTED][7].Text();
*/

    //}

	// Verificar que el nro de version sea correcto //
    //...
    ///los recursos se cargan antes que el moMoldeoObject::Init
    ///ya que algnos parametros necesitan de todos los recursos para levantar
    ///ejemplo: moMathFunction....

    if (MODebug2) MODebug2->Message(moText("moConsole:: Initializing Resource Manager."));
	InitResources(  p_pResourceManager,
					p_datapath,
					m_Config,
					p_render_to_texture_mode,
					p_screen_width, p_screen_height,
					p_render_width, p_render_height,
					p_OpWindowHandle,
					p_Display);

    ///wem ust initialize the parametrization here
    ///moldeoobect::init siempre antes que los moDefineParamIndex....
    moMoldeoObject::Init();


	moDefineParamIndex( CONSOLE_DEVICES, moText("devices") );
	moDefineParamIndex( CONSOLE_EFFECT, moText("effect") );
	moDefineParamIndex( CONSOLE_PREEFFECT, moText("preeffect") );
	moDefineParamIndex( CONSOLE_EFFECT, moText("effect") );
	moDefineParamIndex( CONSOLE_POSTEFFECT, moText("posteffect") );
	moDefineParamIndex( CONSOLE_MASTEREFFECT, moText("mastereffect") );
	moDefineParamIndex( CONSOLE_RESOURCES, moText("resources") );
	moDefineParamIndex( CONSOLE_ON, moText("mastereffects_on") );
	moDefineParamIndex( CONSOLE_FULLDEBUG, moText("fulldebug") );
	moDefineParamIndex( CONSOLE_SCRIPT, moText("consolescript") );
	moDefineParamIndex( CONSOLE_OUTPUTMODE, moText("outputmode") );
	moDefineParamIndex( CONSOLE_OUTPUTRESOLUTION, moText("outputresolution") );
	moDefineParamIndex( CONSOLE_RENDERMODE, moText("rendermode") );
	moDefineParamIndex( CONSOLE_RENDERRESOLUTION, moText("renderresolution") );
	moDefineParamIndex( CONSOLE_CLIP1, moText("clip1") );
	moDefineParamIndex( CONSOLE_CLIP2, moText("clip2") );
	moDefineParamIndex( CONSOLE_CLIP3, moText("clip3") );


	LoadResources();
	LoadIODevices();
	LoadPreEffects();
	LoadPostEffects();
	LoadEffects();
	LoadMasterEffects();

	state.m_nAllEffects = m_EffectManager.AllEffects().Count();

	this->InitializeAllEffects();
	this->StartMasterEffects();


	MOboolean m_bMasterEffects_On = m_Config.GetParam( moText("mastereffects_on") ).GetValue().GetSubValue(0).Int();
	if (m_bMasterEffects_On) {
        //
    }

    m_MoldeoObjects.Add( (moMoldeoObject*)this );

	LoadConnections();

    InitScript();
    RegisterFunctions();


	m_bInitialized = true;

	return Initialized();
}

void
moConsole::LoadConnections() {
	///check for each outlet connector on MoldeoObject's connections to inlets...
	MOuint i,j,k,l,m;

	///SET Moldeo Objects Unique Id's
	for( i=0; i<m_MoldeoObjects.Count(); i++) {
		moMoldeoObject* mobject = m_MoldeoObjects.Get(i);
		if (mobject) mobject->SetId(MO_MOLDEOOBJECTS_OFFSET_ID + i);
	}


	///Connect outlets to inlets....
	for( i=0; i<m_MoldeoObjects.Count(); i++) {

		moMoldeoObject* psrcobject = m_MoldeoObjects[i];

		if (psrcobject) {

            moOutlets* poutlets = psrcobject->GetOutlets();

            ///run over the outlet connectors connections...
            for( j=0; j<poutlets->Count(); j++) {

                moOutlet* p_outlet = poutlets->Get(j);

                moConnections* p_connections = p_outlet->GetConnections();

                for( k = 0; k < p_connections->Count(); k++ ) {

                    moConnection *p_connection = p_connections->Get(k);//get the connection to update

                    moText DestinationMoldeoLabelName = p_connection->GetDestinationMoldeoLabelName();
                    moText DestinationConnectorLabelName = p_connection->GetDestinationConnectorLabelName();

                    ///search for moldeolabelname
                    ///search for connector labelname
                    for( l=0; l<m_MoldeoObjects.Count(); l++) {

                        moMoldeoObject* pdstobject = m_MoldeoObjects[l];

                        if (pdstobject) {
                            if ( pdstobject->GetLabelName()==DestinationMoldeoLabelName ) {
                                ///update destination id
                                p_connection->SetDestinationMoldeoId(pdstobject->GetId());
                                for( m = 0; m < pdstobject->GetInlets()->Count() ; m++) {
                                    moInlet* pinlet = pdstobject->GetInlets()->Get(m);
                                    if (pinlet->GetConnectorLabelName()==DestinationConnectorLabelName) {
                                        //update destination connector id
                                        p_connection->SetDestinationConnectorId( pinlet->GetConnectorId() );
                                    }
                                }
                            }
                        }
                    }

                }
            }
		}
	}
}


//==========================================================================
//   CARGAMOS LOS DISPOSITIVOS
//==========================================================================
void
moConsole::LoadIODevices() {

	moText text, fxname, cfname, lblname;
	MOint devices, i, ndevices;
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

        moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
        moFile FullCF( completecfname );

        if ( FullCF.Exists() ) {
            pdevice = m_pIODeviceManager->NewIODevice( fxname, cfname, lblname,  MO_OBJECT_IODEVICE, devices, i );
            if (pdevice) {
                m_MoldeoObjects.Add( (moMoldeoObject*) pdevice );
                pdevice->SetResourceManager( m_pResourceManager );
                if(pdevice!=NULL) {
                    pdevice->SetResourceManager( m_pResourceManager );
                    pdevice->Init();
                } else MODebug2->Error( moText("moConsole:: Couldn't load a device:") + moText(fxname));
            } else MODebug2->Error( moText("moConsole:: Couldn't create the device:") + moText(fxname));
		} else {
		    MODebug2->Error(moText("moConsole::LoadMasterEffects Error: Config File doesn't exist : ") + (moText)completecfname);
        }
		m_Config.NextValue();

    }
	if (MODebug2) MODebug2->Message( moText("moConsole:: IODevices loaded.") );
}

void moConsole::UnloadIODevices() {
    if (m_pIODeviceManager)
        while(m_pIODeviceManager->IODevices().Count()>0) {
            if( m_pIODeviceManager->IODevices().Get(0) != NULL ) {
                m_pIODeviceManager->RemoveIODevice( 0 );
            }
        }
}

//==========================================================================
//   CARGAMOS LOS EFFECTS MAESTROS
//==========================================================================
void
moConsole::LoadMasterEffects() {
	moText text, fxname, cfname, lblname;
	MOint N,i,mtfx;
	moMasterEffect*	pmastereffect = NULL;

	mtfx = m_Config.GetParamIndex("mastereffect");
	m_Config.SetCurrentParamIndex(mtfx);
	N = m_Config.GetValuesCount(mtfx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole:: Loading MasterEffects configs...") );
		MODebug2->Message( moText("moConsole:: Master Effects.") + IntToStr(N)  );
	}

	state.m_nMasterEffects = N;

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();

            moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {
                pmastereffect = (moMasterEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname,  MO_OBJECT_MASTEREFFECT, mtfx, i);
                if (pmastereffect) {
                    m_MoldeoObjects.Add( (moMoldeoObject*) pmastereffect );
                    pmastereffect->SetResourceManager( m_pResourceManager );
                    pmastereffect->Set( &m_EffectManager, &state );
                    if( pmastereffect->GetName() == moText("ligia") ) {
                                iligia=i;
                                pmastereffect->Init();
                                //pmastereffect->state.on = MO_ON;
                    }
                } else MODebug2->Error( moText("moConsole:: Couldn't load Master Effect:") + moText(fxname));
			} else {
			    MODebug2->Error(moText("moConsole::LoadMasterEffects Error: Config File doesn't exist : ") + (moText)completecfname);
            }
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Message( moText("moConsole:: Master Effects loaded.") );

}

void moConsole::UnloadMasterEffects() {

    while(m_EffectManager.MasterEffects().Count()>0) {
		if( m_EffectManager.MasterEffects().Get(0) != NULL ) {
			m_EffectManager.RemoveEffect( 0, MO_OBJECT_MASTEREFFECT );
		}
	}

}

//==========================================================================
//   CARGAMOS LOS PRE-EFFECTS
//==========================================================================
void
moConsole::LoadPreEffects() {

	moText text,fxname,cfname,lblname;
	MOint prfx,i,N;
	moPreEffect* ppreeffect;

	prfx = m_Config.GetParamIndex("preeffect");
	m_Config.SetCurrentParamIndex(prfx);
	N = m_Config.GetValuesCount(prfx);

	if (MODebug2) {
		MODebug2->Message( moText("Loading PreEffects configs...") );
		MODebug2->Message( moText(" Pre-Effects.") + IntToStr(N) );
	}

	state.m_nPreEffects = N;

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {
			if(iborrado!=-1) Draw();
			//creando nuevo efecto
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();


            moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname + moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {

                ppreeffect = (moPreEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname,  MO_OBJECT_PREEFFECT, prfx, i);
                if (ppreeffect) {
                    m_MoldeoObjects.Add( (moMoldeoObject*) ppreeffect );
                    ppreeffect->SetResourceManager( m_pResourceManager );
                    if ( ppreeffect->GetName() == moText("erase") ) {
                        iborrado = i;
                        if (ppreeffect->Init()) {
                            MOint pre,on;
                            MOint paramindex, valueindex;
                            paramindex = ppreeffect->GetMobDefinition().GetMobIndex().GetParamIndex();
                            valueindex = ppreeffect->GetMobDefinition().GetMobIndex().GetValueIndex();
                            pre = m_Config.GetParam(paramindex  ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_PRE).Int();
                            on = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_ON).Int();
                            if (pre>=0) ppreeffect->GetConfig()->SetCurrentPreConf(pre);
                            if (on>0) ppreeffect->state.on = true;
                        }
                    }
                } else MODebug2->Error( moText("moConsole:: Couldn't load Pre Effect:") + moText(fxname));			m_Config.NextValue();
			} else {
			    MODebug2->Error(moText("moConsole::LoadPreEffects Error: Config File doesn't exist : ") + (moText)completecfname);
            }
		}
	}

	if (MODebug2) MODebug2->Message(moText("moConsole:: Pre Effects loaded."));
}

void moConsole::UnloadPreEffects() {

    while(m_EffectManager.PreEffects().Count()>0) {
		if( m_EffectManager.PreEffects().Get(0) != NULL ) {
			m_EffectManager.RemoveEffect( 0, MO_OBJECT_PREEFFECT );
		}
	}
}

//==========================================================================
//   CARGAMOS LOS EFFECTS
//==========================================================================
void
moConsole::LoadEffects() {

	moText text,fxname,cfname,lblname;
	MOint efx,i,N;
	moEffect*	peffect = NULL;

    efx = m_Config.GetParamIndex("effect");
    m_Config.SetCurrentParamIndex(efx);
	N = m_Config.GetValuesCount(efx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole:: Loading Effects configs...") );
		MODebug2->Message( moText("moConsole:: Effects.") + IntToStr(N)  );
	}

	state.m_nEffects = N;

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i < N; i++) {
			Draw();
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();

			moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {
                if ((moText)fxname!=moText("nil")) {
                    peffect = (moEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname,  MO_OBJECT_EFFECT, efx, i);
                    if (peffect) {
                        m_MoldeoObjects.Add( (moMoldeoObject*) peffect );
                        peffect->SetResourceManager( m_pResourceManager );
                    }
                } else {
                    peffect = NULL;
                    m_EffectManager.Effects().Add(peffect);
                    m_EffectManager.AllEffects().Add(peffect);
                    m_MoldeoObjects.Add( (moMoldeoObject*) peffect );
                }
			} else {
			    MODebug2->Error(moText("moConsole::LoadEffects Error: Config File doesn't exist : ") + (moText)completecfname);
            }
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Message(moText("moConsole:: Effects loaded."));
}

void moConsole::UnloadEffects() {

	while(m_EffectManager.Effects().Count()>0) {
		if( m_EffectManager.Effects().Get(0) != NULL ) {
			m_EffectManager.RemoveEffect( 0, MO_OBJECT_EFFECT );
		}
    }

}

//==========================================================================
//   CARGAMOS LOS POST-EFFECTS
//==========================================================================

void
moConsole::LoadPostEffects() {

	moText text, fxname, cfname, lblname;
	MOint ptfx,i,N;
	moPostEffect*	posteffect = NULL;

	ptfx = m_Config.GetParamIndex("posteffect");
	m_Config.SetCurrentParamIndex(ptfx);
	N = m_Config.GetValuesCount(ptfx);

	if (MODebug2) {
		MODebug2->Message( moText("moConsole:: Loading PostEffects configs...") );
		MODebug2->Message( moText("moConsole:: Post Effects.") + IntToStr(N)  );
	}

	state.m_nPostEffects = N;

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {
			if(idebug!=-1) { Draw(); }
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();

            moText completecfname = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)cfname+ moText(".cfg");
			moFile FullCF( completecfname );

			if ( FullCF.Exists() ) {

                posteffect = (moPostEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname,  MO_OBJECT_POSTEFFECT, ptfx , i );
                if (posteffect) {
                    m_MoldeoObjects.Add( (moMoldeoObject*) posteffect );
                    posteffect->SetResourceManager( m_pResourceManager );
                    if(posteffect->GetName() == moText("debug")) {
                            idebug = i;
                            posteffect->Init();
                            posteffect->state.on = MO_ON;
                    }
                } else MODebug2->Error( moText("moConsole:: Couldn't load Post Effect:") + moText(fxname));
			} else {
			    MODebug2->Error(moText("moConsole::LoadPostEffects Error: Config File doesn't exist : ") + (moText)completecfname);
			}
			m_Config.NextValue();
		}
	}
	if (MODebug2) MODebug2->Message(moText("moConsole:: Post Effects loaded."));
}

void moConsole::UnloadPostEffects() {

    while(m_EffectManager.PostEffects().Count()>0) {
		if( m_EffectManager.PostEffects().Get(0) != NULL ) {
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

	moText resname;
	moText cfname;
	moText lblname;

	moParam& presources(m_Config.GetParam(moText("resources")));
	presources.FirstValue();
	for(MOuint r=0; r<presources.GetValuesCount(); r++) {
		moResource* presource = NULL;

		resname = presources[MO_SELECTED][MO_CFG_RESOURCE].Text();
		cfname = presources[MO_SELECTED][MO_CFG_RESOURCE_CONFIG].Text();
		lblname = presources[MO_SELECTED][MO_CFG_RESOURCE_LABEL].Text();

		MOint rid = m_pResourceManager->GetResourceId( resname );

		if(rid>-1) presource = m_pResourceManager->GetResource(rid);

		if (presource) {
		    MODebug2->Message( moText("moConsole:: Already loaded plugin resource: ") + (moText)resname );
		} else {
			//maybe a plugin
			if (m_pResourceManager->NewResource(resname)) {
				rid = m_pResourceManager->GetResourceId(resname);
				if (rid>=0) {
					presource = m_pResourceManager->GetResource(rid);
					if (presource) {
						presource->SetConfigName(cfname);
						presource->SetLabelName(lblname);
						if (presource->Init()) {
						    MODebug2->Message( moText("moConsole:: Loaded plugin resource: ") + (moText)resname );
                        } else MODebug2->Error( moText("moConsole:: Error: Unloaded plugin resource: ") + (moText)resname );
					}
				}
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

    if (MODebug2) MODebug2->Message(moText("moConsole:: Resources Plugin loaded."));

}

void moConsole::UnloadResources() {
    for(int i=m_pResourceManager->Resources().Count()-1; i>=0; i--) {
        if (m_pResourceManager->Resources().Get(i)!=NULL) {
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
		p_effect = m_EffectManager.AllEffects().Get(i);
		if( p_effect !=NULL) {
            Draw();
			if(m_Config.GetParam(dg).GetValue().GetSubValue(0).Text()==moText("yes")) {
				p_effect->state.fulldebug = MO_ACTIVATED;
			} else {
				p_effect->state.fulldebug = MO_DEACTIVATED;
			}
			if( p_effect->GetName()!=moText("debug") && p_effect->GetName()!=moText("erase")
				&& p_effect->GetName()!=moText("ligia")) {
				    bool res = p_effect->Init();

					if (res) {

                        MOint pre,on;
                        MOint paramindex = p_effect->GetMobDefinition().GetMobIndex().GetParamIndex();
                        MOint valueindex = p_effect->GetMobDefinition().GetMobIndex().GetValueIndex();

                        pre = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_PRE).Int();
                        on = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_ON).Int();

                        if (pre>=0) p_effect->GetConfig()->SetCurrentPreConf(pre);
                        if (on>0) p_effect->state.on = MO_ON;
                        else p_effect->state.on = MO_OFF;

                        // Sucio codigo agregado rapidamente para poder asignar los efectos a teclas arbitrarias de las 4 filas
                        // del teclado:
                        /*
                        MOint idx;
                        moText key;
                        key = m_Config.GetParam( pEIx->GetParamIndex() ).GetValue( pEIx->GetValueIndex() ).GetSubValue(MO_CFG_EFFECT_KEY).Text();
                        idx = ConvertKeyNameToIdx(key);
                        p_effect->keyidx = idx;
                        */
                        // Fabri, despues implementa mejor esto... ;-)
                        //lo saqué!!!
					}
			} else {

                MOint pre,on;
                MOint paramindex = p_effect->GetMobDefinition().GetMobIndex().GetParamIndex();
                MOint valueindex = p_effect->GetMobDefinition().GetMobIndex().GetValueIndex();

                pre = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_PRE).Int();
                on = m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(MO_CFG_EFFECT_ON).Int();

                if (pre>=0) p_effect->GetConfig()->SetCurrentPreConf(pre);
                if (on>0) p_effect->state.on = MO_ON;
                else p_effect->state.on = MO_OFF;

                // Sucio codigo agregado rapidamente para poder asignar los efectos a teclas arbitrarias de las 4 filas
                // del teclado:
                /*
                MOint idx;
                moText key;
                key = m_Config.GetParam( pEIx->GetParamIndex() ).GetValue( pEIx->GetValueIndex() ).GetSubValue(MO_CFG_EFFECT_KEY).Text();
                idx = ConvertKeyNameToIdx(key);
                p_effect->keyidx = idx;
                */
                // Fabri, despues implementa mejor esto... ;-)
			}
			//carga cÃ³digos...
			p_effect->LoadCodes( m_pIODeviceManager );
		}
	}

}

void moConsole::FinalizeAllEffects() {

	moEffect*	p_effect = NULL;

	for( MOuint i=0; i< m_EffectManager.AllEffects().Count(); i++ ) {
		p_effect = m_EffectManager.AllEffects().Get(i);
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
		p_effect = m_EffectManager.MasterEffects().Get(i);
		if( p_effect!=NULL ) {
			Draw();
			p_effect->state.on = MO_ACTIVATED;
		}
	}
	if( m_EffectManager.MasterEffects().Count()>0 ) {
		p_effect = m_EffectManager.MasterEffects().Get(0);
		if(p_effect!=NULL)
			p_effect->state.on = MO_ON;
	}
	if( m_EffectManager.MasterEffects().Count()>1 ) {
		p_effect = m_EffectManager.MasterEffects().Get(1);
		if(p_effect!=NULL)
			p_effect->state.on = MO_ON;
	}
	if (MODebug2) MODebug2->Push( moText("moConsole:: Master effects on.") );

}

void moConsole::StopMasterEffects() {

	moEffect*	pEffect = NULL;

    if (MODebug2) MODebug2->Push( moText("moConsole:: turning off MasterEffects.") );

	if(m_EffectManager.MasterEffects().Count()>0) {
		pEffect = m_EffectManager.MasterEffects().Get(0);
		if( pEffect!=NULL )
			pEffect->state.on = MO_OFF;
	}
	if( m_EffectManager.MasterEffects().Count()>1 ) {
		pEffect = m_EffectManager.MasterEffects().Get(1);
		if(pEffect!=NULL)
			pEffect->state.on = MO_OFF;
	}
	for(MOuint i=0;i<m_EffectManager.MasterEffects().Count();i++) {
		pEffect = m_EffectManager.MasterEffects().Get(i);
		if( pEffect!=NULL ) {
			Draw();
			pEffect->state.on = MO_DEACTIVATED;
		}
	}

	if (MODebug2) MODebug2->Push( moText("moConsole:: MasterEffects turned OFF.") );

}

MOulong
moConsole::GetTicks() {

	return moGetTicks();

}

void
moConsole::GLSwapBuffers() {

	SDL_GL_SwapBuffers();
}

void
moConsole::Draw() {
	MOuint i;
	moEffect* pEffect = NULL;
	moText savename, framesavename;
	moRenderManager* RenderMan = m_pResourceManager->GetRenderMan();

	if (RenderMan==NULL) return;

    moText cs;
    cs = m_Config[moR(CONSOLE_SCRIPT)][MO_SELECTED][0].Text();

    //MODebug2->Push( moText("cs:") + (moText)cs );
    //MODebug2->Push( moText("m_ConsoleScript:") +  (moText)m_ConsoleScript );

	if ((moText)m_ConsoleScript!=cs && IsInitialized()) {

        m_ConsoleScript = cs;
        moText fullscript = m_pResourceManager->GetDataMan()->GetDataPath()+ moSlash + (moText)m_ConsoleScript;

        MODebug2->Message(moText("Console script loading : ") + (moText)fullscript );

        if ( CompileFile(fullscript) ) {

            MODebug2->Message(moText("Console script loaded : ") + (moText)fullscript );

            moText toffset=moText("");

            toffset = m_Config[moR(CONSOLE_SCRIPT)][MO_SELECTED][1].Text();
            if (toffset!=moText("")) {
                m_ScriptTimecodeOffset = atoi( toffset );
            } else {
                m_ScriptTimecodeOffset = 0;
            }

            SelectScriptFunction( "Init" );
            AddFunctionParam( (int)m_ScriptTimecodeOffset );
            RunSelectedFunction();

        } else MODebug2->Error(moText("couldnt compile lua script ") + (moText)fullscript );
	}

    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Run")) {
            SelectScriptFunction("Run");
            RunSelectedFunction(1);
        }
    }

	MOswitch borrar = MO_ACTIVATED;
    MOboolean pre_effect_on = false;

	if(state.pause==MO_DEACTIVATED) {

		state.tempo.ticks = GetTicks();
		state.tempo.getTempo();

		RenderMan->BeginDraw();

		//Se dibujan los m_PreEffects
		for(i=1; i<m_EffectManager.PreEffects().Count(); i++ ) {
			pEffect = m_EffectManager.PreEffects().Get(i);
			if( pEffect!=NULL ) {
				if( pEffect->state.on==MO_ON ) {
					pre_effect_on = true;
					RenderMan->BeginDrawEffect();
					pEffect->Draw(&state.tempo);
					RenderMan->EndDrawEffect();
					borrar = MO_DEACTIVATED;
				}
			}
		}

		if(borrar==MO_ACTIVATED)
		{
			if( m_EffectManager.PreEffects().Count()>0 ) {
				pEffect = m_EffectManager.PreEffects().Get(0);
				if( pEffect!=NULL ) {
					if( pEffect->state.on==MO_ON)
					{
						RenderMan->BeginDrawEffect();
						pEffect->Draw(&state.tempo);
						RenderMan->EndDrawEffect();
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
        if (state.stereooutput==MO_ACTIVATED) {
            ///Dibujamos los efectos con capacidad stereo

            ///ojo izquierdo
            for( i=0; i<m_EffectManager.Effects().Count(); i++ ) {
                pEffect = m_EffectManager.Effects().Get(i);
                if(pEffect!=NULL && pEffect->state.stereo==MO_ACTIVATED) {
                    pEffect->state.stereoside = MO_STEREO_LEFT;
                    if(pEffect->state.on==MO_ON) {
                            RenderMan->BeginDrawEffect();
                            pEffect->Draw(&state.tempo);
                            RenderMan->EndDrawEffect();
                    }
                }
            }

            RenderMan->CopyRenderToTexture(MO_LEFT_TEX);

            ///ojo derecho
            for( i=0; i<m_EffectManager.Effects().Count(); i++ ) {
                pEffect = m_EffectManager.Effects().Get(i);
                if(pEffect!=NULL && pEffect->state.stereo==MO_ACTIVATED) {
                    pEffect->state.stereoside = MO_STEREO_RIGHT;
                    if(pEffect->state.on==MO_ON) {
                            RenderMan->BeginDrawEffect();
                            pEffect->Draw(&state.tempo);
                            RenderMan->EndDrawEffect();
                    }

                    ///reset state to NONE so you can draw like always...
                    pEffect->state.stereoside = MO_STEREO_NONE;
                }
            }

            RenderMan->CopyRenderToTexture(MO_RIGHT_TEX);

        } else {
            ///NORMAL METHOD

            //Se dibujan los Effects
            for( i=0; i<m_EffectManager.Effects().Count(); i++ ) {
                pEffect = m_EffectManager.Effects().Get(i);
                if(pEffect!=NULL) {
                    if(pEffect->state.on==MO_ON) {
                            RenderMan->BeginDrawEffect();
                            pEffect->Draw(&state.tempo);
                            RenderMan->EndDrawEffect();
                    }
                }
            }

            RenderMan->CopyRenderToTexture(MO_EFFECTS_TEX);
        }

		//sedibujan los post Effects
		for(i=0;i<m_EffectManager.PostEffects().Count();i++) {
			pEffect = m_EffectManager.PostEffects().Get(i);
			if(pEffect!=NULL) {
				if(pEffect->state.on==MO_ON) {
					RenderMan->BeginDrawEffect();
					pEffect->Draw(&state.tempo);
					RenderMan->EndDrawEffect();
				}
			}
		}

		//se dibujan los Effects masters
		for(i=0;i<m_EffectManager.MasterEffects().Count();i++) {
			pEffect = m_EffectManager.MasterEffects().Get(i);
			if(pEffect!=NULL) {
				if(pEffect->state.on==MO_ON) {
					RenderMan->BeginDrawEffect();
					pEffect->Draw(&state.tempo);
					RenderMan->EndDrawEffect();
				}
			}
		}

		RenderMan->CopyRenderToTexture(MO_FINAL_TEX);

		//aca controlamos los fps
		if( state.setfps == MO_ACTIVATED ) {
			state.fps1 = GetTicks();
			while(( state.fps1 - state.fps0 ) <(1000 / state.fps) ) {
				state.fps1 = GetTicks();
			}
		}

		RenderMan->EndDraw();

		if (RenderMan->IsRenderToFBOEnabled() || !RenderMan->RenderResEqualScreenRes())
			RenderMan->DrawTexture(MO_SCREEN_RESOLUTION, MO_FINAL_TEX);

		this->GLSwapBuffers();
	}

	state.fps0 = GetTicks();
}


MOboolean
moConsole::Finish() {

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
	}

	if (m_bIODeviceManagerDefault) {
		if (m_pIODeviceManager) {
			m_pIODeviceManager->Finish();
			delete m_pIODeviceManager;
			m_pIODeviceManager = NULL;
		}
	}

	m_MoldeoObjects.Finish();

	//FINALIZAMOS LOS RECURSOS (liberando memoria)
	state.Finish();
	m_Config.DeleteConfig();//finally we unload the console configuration

	m_bInitialized = false;

	return true;
}


int
moConsole::Interaction() {

	MOuint i;
	moEffect*	pEffect = NULL;
	moEffect* pPanel = NULL;
	moEffect* pChannel = NULL;
	moRenderManager* RenderMan = m_pResourceManager->GetRenderMan();

	//_IODEVICE ACTUALIZA
	RenderMan->BeginUpdate();
	if(m_pIODeviceManager!=NULL) {
		RenderMan->BeginUpdateDevice();
		m_pIODeviceManager->Update();
		RenderMan->EndUpdateDevice();
	}
	RenderMan->EndUpdate();

	//INTERACCION EFFECTS MAESTROS

	for( i=0; i<m_EffectManager.MasterEffects().Count(); i++) {
		pEffect = m_EffectManager.MasterEffects().Get(i);
		if(pEffect!=NULL) {
			if(pEffect->state.on == MO_ACTIVATED) {
				pEffect->Interaction( m_pIODeviceManager );
			}
		}
	}


	///INTERACCION EFFECTS
	///TODO, cambiar el indice fijo por dinamico....

	///channel0 y panel deben estar deshabilitados(las keys asociadas a ellos quedan liberadas)
		//se pasa el control de events al CanalMaestro array[0], el sabra a quien pasar el control
	if ( m_EffectManager.MasterEffects().Count() > 2 ) {
		pChannel = m_EffectManager.MasterEffects().Get(0);
		pPanel = m_EffectManager.MasterEffects().Get(2);
		if (pPanel && pChannel)
		if((pChannel->state.on != MO_ACTIVATED) && (pPanel->state.on != MO_ACTIVATED) ) {
		    ///signifa que el canal y el panel estan desactivados....
			pChannel->Interaction( m_pIODeviceManager );
		}
	} else {
        ///MODO DIRECTO!!!!! todos los efectos prendidos reciben Interaccion
	    for(int all=0;all<m_EffectManager.AllEffects().Count(); all++) {
	        moEffect* pEffect = NULL;

	        pEffect = m_EffectManager.AllEffects().Get(all);
	        if (pEffect && pEffect->state.on==MO_ON) {
                pEffect->Interaction( m_pIODeviceManager );
	        }
        }
    }


	//TAREAS ESPECIALES
	//reinicializa
	if(state.reinit) {
		Finish();
		Init();
		state.reinit = MO_FALSE;
	}


	return state.finish;
}

void
moConsole::Update() {
	moRenderManager* RenderMan = m_pResourceManager->GetRenderMan();

	RenderMan->BeginUpdate();
	if (m_pIODeviceManager)
		for(MOuint i = 0; i<m_MoldeoObjects.Count(); i++) {
			RenderMan->BeginUpdateObject();
			moMoldeoObject* pMOB = m_MoldeoObjects[i];
			if (pMOB) {
                if (pMOB->GetType()!=MO_OBJECT_IODEVICE)
                    pMOB->Update(m_pIODeviceManager->GetEvents());
			}
			RenderMan->EndUpdateObject();
		}
	RenderMan->EndUpdate();
}

moConfigDefinition *
moConsole::GetDefinition( moConfigDefinition *p_configdefinition ) {

	//default: alpha, color, syncro
	p_configdefinition = moMoldeoObject::GetDefinition( p_configdefinition );
	p_configdefinition->Add( moText("devices"), MO_PARAM_TEXT, CONSOLE_DEVICES );
	p_configdefinition->Add( moText("preeffect"), MO_PARAM_TEXT, CONSOLE_PREEFFECT );
	p_configdefinition->Add( moText("effect"), MO_PARAM_TEXT, CONSOLE_EFFECT );
	p_configdefinition->Add( moText("posteffect"), MO_PARAM_TEXT, CONSOLE_POSTEFFECT );
	p_configdefinition->Add( moText("mastereffect"), MO_PARAM_TEXT, CONSOLE_MASTEREFFECT );
	p_configdefinition->Add( moText("resources"), MO_PARAM_TEXT, CONSOLE_RESOURCES );

	p_configdefinition->Add( moText("mastereffects_on"), MO_PARAM_NUMERIC, CONSOLE_ON, moValue("0","NUM").Ref() );
	p_configdefinition->Add( moText("fulldebug"), MO_PARAM_NUMERIC, CONSOLE_FULLDEBUG, moValue("0","NUM").Ref()  );

	p_configdefinition->Add( moText("consolescript"), MO_PARAM_SCRIPT, CONSOLE_SCRIPT );

	p_configdefinition->Add( moText("outputmode"), MO_PARAM_TEXT, CONSOLE_OUTPUTMODE );
	p_configdefinition->Add( moText("outputresolution"), MO_PARAM_TEXT, CONSOLE_OUTPUTRESOLUTION );

	p_configdefinition->Add( moText("rendermode"), MO_PARAM_TEXT, CONSOLE_RENDERMODE );
	p_configdefinition->Add( moText("renderresolution"), MO_PARAM_TEXT, CONSOLE_RENDERRESOLUTION );

	p_configdefinition->Add( moText("clip1"), MO_PARAM_TEXT, CONSOLE_CLIP1 );
	p_configdefinition->Add( moText("clip2"), MO_PARAM_TEXT, CONSOLE_CLIP2 );
	p_configdefinition->Add( moText("clip3"), MO_PARAM_TEXT, CONSOLE_CLIP3 );

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

void moConsole::ConsolePlay() {
    if (moTimeManager::MoldeoTimer->Paused())
        moContinueTimer();
    else
        moStartTimer();
}

void moConsole::ConsolePause() {
    moPauseTimer();
}

void moConsole::ConsoleStop() {
    moStopTimer();
}

moTimerState moConsole::ConsoleState() {
    if ( moTimeManager::MoldeoTimer->Paused() ) {
        return MO_TIMERSTATE_PAUSED;
    } else if ( moTimeManager::MoldeoTimer->Started() ) {
        return MO_TIMERSTATE_PLAYING;
    }
    return MO_TIMERSTATE_STOPPED;
}


int moConsole::GetPreset() {

    return 0;
}

void moConsole::SetPreset( int presetid ) {


}

int moConsole::GetPreconf( int objectid ) {

        return 0;
}

void moConsole::SetPreconf( int objectid, int preconfid ) {
    m_MoldeoObjects[objectid]->GetConfig()->SetCurrentPreConf( preconfid );

}


void moConsole::SetTicks( int ticksid ) {
    moSetDuration( ticksid );
}

int moConsole::GetObjectId( moText p_objectlabelname ) {
    int i;
    for(i=0; i<m_MoldeoObjects.Count(); i++) {

        if (p_objectlabelname == m_MoldeoObjects[i]->GetLabelName()) {
            return m_MoldeoObjects[i]->GetId();
        }

    }
    return -1;
}

int moConsole::GetDirectoryFileCount( moText p_path ) {
    int i;
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
    RegisterFunction("GetObjectCurrentValue");//14
    RegisterFunction("SetObjectCurrentValue");//15


    RegisterFunction("GetObjectDataIndex");//16
    RegisterFunction("GetObjectData");//17
    RegisterFunction("SetObjectData");//18

    ///Specific for effects

    RegisterFunction("GetEffectState");//19
    RegisterFunction("SetEffectState");//20


    ///Specific for devices

    RegisterFunction("GetDeviceCode");//21
    RegisterFunction("GetDeviceCodeId");//22
    RegisterFunction("AddEvent");//23

    ///GENERAL
    RegisterFunction("GetDirectoryFileCount");//24

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
            return luaGetObjectCurrentValue(vm);
        case 15:
            ResetScriptCalling();
            return luaSetObjectCurrentValue(vm);

        case 16:
            ResetScriptCalling();
            return luaGetObjectDataIndex(vm);
        case 17:
            ResetScriptCalling();
            return luaGetObjectData(vm);
        case 18:
            ResetScriptCalling();
            return luaSetObjectData(vm);


        ///for Effects
        case 19:
            ResetScriptCalling();
            return luaGetEffectState(vm);
        case 20:
            ResetScriptCalling();
            return luaSetEffectState(vm);


        ///for IODevices
        case 21:
            ResetScriptCalling();
            return luaGetDeviceCode(vm);
        case 22:
            ResetScriptCalling();
            return luaGetDeviceCodeId(vm);

        case 23:
            ResetScriptCalling();
            return luaAddEvent(vm);

        ///special case FileManager
        case 24:
            ResetScriptCalling();
            return luaGetDirectoryFileCount(vm);

        default:
            NextScriptCalling();
            return moMoldeoObject::ScriptCalling( vm, iFunctionNumber );
	}
}

int moConsole::luaPlay( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;

    ConsolePlay();

    return 0;
}

int moConsole::luaPause( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;

    ConsolePause();

    return 0;
}

int moConsole::luaStop( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;

    ConsoleStop();

    return 0;
}

int moConsole::luaState( moLuaVirtualMachine& vm ) {

    lua_State *state = (lua_State *) vm;

    moTimerState elstate = ConsoleState();
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

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

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

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    MOint preconfid = (MOint) lua_tonumber (state, 2);

    moMoldeoObject* Object = NULL;

    if ( 0<=objectid && objectid<m_MoldeoObjects.Count() )
        Object = m_MoldeoObjects[objectid];

    if (Object && Object->GetConfig()) {
        Object->GetConfig()->SetCurrentPreConf( preconfid );
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

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;

    moMoldeoObject* Object = NULL;
    moEffect* pEffect = NULL;

    if ( 0<=objectid && objectid<m_MoldeoObjects.Count() )
        Object = m_MoldeoObjects[objectid];

    if (Object && Object->GetConfig()) {
        switch ( Object->GetType() ) {
            case MO_OBJECT_EFFECT:
            case MO_OBJECT_PREEFFECT:
            case MO_OBJECT_POSTEFFECT:
            case MO_OBJECT_MASTEREFFECT:
                pEffect = (moEffect*) Object;
                //pEffect->Enable();
                pEffect->TurnOn();
                break;
            default:
                break;
        }
    } else {
        MODebug2->Error( moText("in console script: ObjectEnable : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}

int moConsole::luaObjectDisable(moLuaVirtualMachine& vm)
{
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;

    moMoldeoObject* Object = NULL;
    moEffect* pEffect = NULL;

    if ( 0<=objectid && objectid<m_MoldeoObjects.Count() )
        Object = m_MoldeoObjects[objectid];

        if (Object && Object->GetConfig()) {
            switch ( Object->GetType() ) {
                case MO_OBJECT_EFFECT:
                case MO_OBJECT_PREEFFECT:
                case MO_OBJECT_POSTEFFECT:
                case MO_OBJECT_MASTEREFFECT:
                    pEffect = (moEffect*) Object;
                    //pEffect->Enable();
                    pEffect->TurnOff();
                    break;
                default:
                    break;
            }
    } else {
        MODebug2->Error( moText("in console script: ObjectDisable : object not founded : id:")+(moText)IntToStr(objectid));
    }

    return 0;
}


int moConsole::luaGetObjectParamIndex(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    char *text = (char *) lua_tostring (state, 2);

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

    if (Object && Object->GetConfig()) {
        lua_pushnumber(state, (lua_Number) Object->GetConfig()->GetParamIndex(text) );
    }

    return 1;

}

int moConsole::luaSetObjectCurrentValue(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    MOint paramid = (MOint) lua_tonumber (state, 2);
    MOint valueid = (MOint) lua_tonumber (state, 3);

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

    if (Object && Object->GetConfig()) {
        Object->GetConfig()->SetCurrentValueIndex( paramid, valueid );
    } else {
        MODebug2->Error( moText("in console script: SetObjectValue : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    return 0;
}


int moConsole::luaGetObjectCurrentValue(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    MOint paramid = (MOint) lua_tonumber (state, 2);

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

    if (Object && Object->GetConfig()) {
        int valueid = Object->GetConfig()->GetParam( paramid ).GetIndexValue();
        lua_pushnumber(state, (lua_Number) valueid );
        return 1;
    } else {
        MODebug2->Error( moText("in console script: GetObjectValue : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    return 0;
}


int moConsole::luaGetObjectDataIndex(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    char *text = (char *) lua_tostring (state, 2);
    int inletid = -1;

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

    if (Object) {
        inletid = Object->GetInletIndex( text );
    } else {
        MODebug2->Error( moText("in console script: GetObjectDataIndex : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    lua_pushnumber(state, (lua_Number) inletid );

    return 1;
}

int moConsole::luaGetObjectData(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    MOint inletid = (MOint) lua_tonumber (state, 2);

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

    if (Object) {
        moInlet* pInlet = Object->GetInlets()->Get(inletid);
        if (pInlet) {
            moData* pData = pInlet->GetData();
            if (pData)
                switch(pData->Type()) {
                    case MO_DATA_NUMBER:
                    case MO_DATA_NUMBER_CHAR:
                    case MO_DATA_NUMBER_INT:
                    case MO_DATA_NUMBER_LONG:
                    case MO_DATA_NUMBER_MIDI:
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
                        break;

                    case MO_DATA_NUMBER_DOUBLE:
                    case MO_DATA_NUMBER_FLOAT:
                        lua_pushnumber(state, (lua_Number) pData->Double() );
                        return 1;

                    case MO_DATA_TEXT:
                        lua_pushstring(state, pData->Text() );
                        return 1;

                }
        }
    } else {
        MODebug2->Error( moText("in console script: GetObjectData : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    return 0;
}

int moConsole::luaSetObjectData(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    MOint inletid = (MOint) lua_tonumber (state, 2);

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

    if (Object) {
        moInlet* pInlet = Object->GetInlets()->Get(inletid);
        if (pInlet) {
            moData* pData = pInlet->GetData();
            if (pData)
                switch(pData->Type()) {
                    case MO_DATA_NUMBER:
                    case MO_DATA_NUMBER_CHAR:
                    case MO_DATA_NUMBER_INT:
                    case MO_DATA_NUMBER_LONG:
                    case MO_DATA_NUMBER_MIDI:
                        pData->SetLong( (MOlong) lua_tonumber ( state, 3 ) );
                        return 0;

                    case MO_DATA_3DMODELPOINTER:
                    case MO_DATA_FONTPOINTER:
                    case MO_DATA_IMAGESAMPLE:
                    case MO_DATA_IMAGESAMPLE_FILTERED:
                    case MO_DATA_IMAGESAMPLE_TEXTUREBUFFER:
                        //pData->SetLong( (MOlong) lua_tonumber ( state, 3 ) );
                        return 0;

                    case MO_DATA_VECTOR2I:
                        (*pData->Vector2i()) = moVector2i(  (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ) );
                        return 0;

                    case MO_DATA_VECTOR3I:
                        (*pData->Vector3i()) = moVector3i(  (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ),
                                                            (MOlong) lua_tonumber ( state, 5 ) );
                        return 0;

                    case MO_DATA_VECTOR4I:
                        (*pData->Vector4i()) = moVector4i(  (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ),
                                                            (MOlong) lua_tonumber ( state, 5 ),
                                                            (MOlong) lua_tonumber ( state, 6 ) );
                        return 0;

                    case MO_DATA_VECTOR2F:
                        (*pData->Vector2d()) = moVector2d(  (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ));
                        return 0;

                    case MO_DATA_VECTOR3F:
                        (*pData->Vector3d()) = moVector3d(  (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ),
                                                            (MOdouble) lua_tonumber ( state, 5 ));
                        return 0;

                    case MO_DATA_VECTOR4F:
                        (*pData->Vector4d()) = moVector4d(  (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ),
                                                            (MOdouble) lua_tonumber ( state, 5 ),
                                                            (MOdouble) lua_tonumber ( state, 6 ) );
                        return 0;

                    case MO_DATA_MESSAGE:
                    case MO_DATA_MESSAGES:
                        return 0;

                    case MO_DATA_NUMBER_DOUBLE:
                    case MO_DATA_NUMBER_FLOAT:
                        pData->SetDouble( (MOdouble) lua_tonumber ( state, 3 ) );
                        return 0;

                    case MO_DATA_TEXT:
                        //lua_pushstring(state, pData->Text() );
                        pData->SetText( lua_tostring ( state, 3 ) );
                        return 0;

                }
        }
    } else {
        MODebug2->Error( moText("in console script: GetObjectData : object not founded : id:")+(moText)IntToStr(objectid) );
    }

    return 0;
}


int moConsole::luaSetEffectState(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;

    moMoldeoObject* Object = m_MoldeoObjects[objectid];
    moEffect*       pEffect = NULL;

    moEffectState fxstate;


    if (Object && Object->GetConfig()) {

        switch (Object->GetType()) {
            case MO_OBJECT_EFFECT:
            case MO_OBJECT_PREEFFECT:
            case MO_OBJECT_POSTEFFECT:
            case MO_OBJECT_MASTEREFFECT:
                pEffect = (moEffect*) Object;
                fxstate = pEffect->state;
                fxstate.alpha = (MOfloat) lua_tonumber (state, 2);
                fxstate.tint = (MOfloat) lua_tonumber (state, 3);
                fxstate.tintr = (MOfloat) lua_tonumber (state, 4);
                fxstate.tintg = (MOfloat) lua_tonumber (state, 5);
                fxstate.tintb = (MOfloat) lua_tonumber (state, 6);
                fxstate.tempo.ang = (MOfloat) lua_tonumber (state, 7);

                pEffect->state = fxstate;
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
    lua_State *state = (lua_State *) vm;

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;

    moMoldeoObject* Object = m_MoldeoObjects[objectid];
    moEffect*       pEffect = NULL;


    if (Object && Object->GetConfig()) {
        switch (Object->GetType()) {
            case MO_OBJECT_EFFECT:
            case MO_OBJECT_PREEFFECT:
            case MO_OBJECT_POSTEFFECT:
            case MO_OBJECT_MASTEREFFECT:
                pEffect = (moEffect*) Object;
                lua_pushnumber(state, (lua_Number) pEffect->state.alpha );
                lua_pushnumber(state, (lua_Number) pEffect->state.tint );
                lua_pushnumber(state, (lua_Number) pEffect->state.tintr );
                lua_pushnumber(state, (lua_Number) pEffect->state.tintg );
                lua_pushnumber(state, (lua_Number) pEffect->state.tintb );
                lua_pushnumber(state, (lua_Number) pEffect->state.tempo.ang );
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

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    MOint devicecode = (MOint) lua_tonumber (state, 2);
    MOint codevalue = -1;

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

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

    MOint objectid = (MOint) lua_tonumber (state, 1) - MO_MOLDEOOBJECTS_OFFSET_ID;
    char *devicecodestr = (char *) lua_tostring (state, 2);
    MOint devicecode = -1;

    moMoldeoObject* Object = m_MoldeoObjects[objectid];

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
