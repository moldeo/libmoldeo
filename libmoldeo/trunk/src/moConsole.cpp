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

#include "moConsole.h"

moConsole::moConsole() {

	m_bIODeviceManagerDefault = true;
	m_pIODeviceManager = NULL;
	m_pResourceManager = NULL;

	m_bExternalResources = false;

}

moConsole::~moConsole()
{
    Finish();
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
                        MO_HANDLE p_OpWindowHandle) {

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
					p_OpWindowHandle);
	}
}


MOboolean moConsole::Init()
{
	return Init( moText("data"), moText("data/console.mol"), NULL, MO_RENDER_TO_TEXTURE_FBSCREEN,
		MO_DEF_SCREEN_WIDTH,
		MO_DEF_SCREEN_HEIGHT,
		MO_DEF_RENDER_WIDTH,
		MO_DEF_RENDER_HEIGHT );
}


MOboolean moConsole::Init( moText p_datapath,
						  moText p_consoleconfig,
						  moIODeviceManager* p_pIODeviceManager,
						 moResourceManager*  p_pResourceManager,
					  MOint p_render_to_texture_mode,
					  MOint p_screen_width, MOint p_screen_height,
					  MOint p_render_width, MOint p_render_height,
                      MO_HANDLE p_OpWindowHandle)
{

	moText text;
	int verif;
	//int a,b;

	idebug = -1;
	iligia = -1;
	iborrado = -1;

	srand( time(NULL) );

	//if no IODeviceManager defined by the user, use default
	if( p_pIODeviceManager == NULL ) {
		m_bIODeviceManagerDefault = true;
		m_pIODeviceManager = new moIODeviceManager();
		if (m_pIODeviceManager)
			m_pIODeviceManager->Init();
	} else {
		m_bIODeviceManagerDefault = false;
		m_pIODeviceManager = p_pIODeviceManager;
	}

	m_EffectManager.Init();

	//Inicializando Estado de la Consola
	state.Init();

    //==========================================================================
	//   CARGAMOS EL ARCHIVO DE CONFIGURACION
	//==========================================================================

	verif = m_Config.LoadConfig( p_consoleconfig ) ;//este parametro debe pasarse desde fuera
	if(verif != MO_CONFIG_OK) {
	    cout << " .mol file invalid, check XML syntax..."  << endl;
	    Finish();
		//then
		return false;
	} else SetConfigName(p_consoleconfig);

	moDefineParamIndex( CONSOLE_DEVICES, moText("devices") );
	moDefineParamIndex( CONSOLE_EFFECT, moText("effect") );
	moDefineParamIndex( CONSOLE_PREEFFECT, moText("preeffect") );
	moDefineParamIndex( CONSOLE_EFFECT, moText("effect") );
	moDefineParamIndex( CONSOLE_POSTEFFECT, moText("posteffect") );
	moDefineParamIndex( CONSOLE_MASTEREFFECT, moText("mastereffect") );
	moDefineParamIndex( CONSOLE_RESOURCES, moText("resources") );
	moDefineParamIndex( CONSOLE_FULLDEBUG, moText("fulldebug") );
	moDefineParamIndex( CONSOLE_ON, moText("mastereffects_on") );

	// Verificar que el nro de version sea correcto //
    //...
	InitResources(  p_pResourceManager,
					p_datapath,
					m_Config,
					p_render_to_texture_mode,
					p_screen_width, p_screen_height,
					p_render_width, p_render_height,
					p_OpWindowHandle);
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

	LoadConnections();

	m_bInitialized = true;

	return Initialized();
}

void
moConsole::LoadConnections() {
	//check for each outlet connector on MoldeoObject's connections to inlets...
	MOuint i,j,k,l,m;

	//SET Moldeo Objects Unique Id's
	for( i=0; i<m_MoldeoObjects.Count(); i++) {
		moMoldeoObject* mobject = m_MoldeoObjects.Get(i);
		if (mobject) mobject->SetId(MO_MOLDEOOBJECTS_OFFSET_ID + i);
	}

	//Connect outlets to inlets....
	for( i=0; i<m_MoldeoObjects.Count(); i++) {

		moMoldeoObject* psrcobject = m_MoldeoObjects[i];

		if (psrcobject) {

            moOutlets* poutlets = psrcobject->GetOutlets();

            //run over the outlet connectors connections...
            for( j=0; j<poutlets->Count(); j++) {

                moOutlet* p_outlet = poutlets->Get(j);

                moConnections* p_connections = p_outlet->GetConnections();

                for( k = 0; k < p_connections->Count(); k++ ) {

                    moConnection *p_connection = p_connections->Get(k);//get the connection to update

                    moText DestinationMoldeoLabelName = p_connection->GetDestinationMoldeoLabelName();
                    moText DestinationConnectorLabelName = p_connection->GetDestinationConnectorLabelName();

                    //search for moldeolabelname
                    //search for connector labelname
                    for( l=0; l<m_MoldeoObjects.Count(); l++) {

                        moMoldeoObject* pdstobject = m_MoldeoObjects[l];

                        if (pdstobject) {
                            if ( pdstobject->GetLabelName()==DestinationMoldeoLabelName ) {
                                //update destination id
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
		pdevice = m_pIODeviceManager->NewIODevice( fxname, cfname, lblname,  MO_OBJECT_IODEVICE, devices, i );
		if (pdevice) {
			m_MoldeoObjects.Add( (moMoldeoObject*) pdevice );
			pdevice->SetResourceManager( m_pResourceManager );
			if(pdevice!=NULL) {
				pdevice->SetResourceManager( m_pResourceManager );
				pdevice->Init();
			} else MODebug2->Push( moText("moConsole:: Couldn't load a device:") + moText(fxname));
		}
		m_Config.NextValue();

    }
	if (MODebug2) MODebug2->Push( moText("moConsole:: IODevices loaded.") );
}

void moConsole::UnloadIODevices() {
	if (m_pIODeviceManager)
    for(MOuint i=0; i < m_pIODeviceManager->IODevices().Count(); i++) {
		if( m_pIODeviceManager->IODevices().Get(i) != NULL ) {
			m_pIODeviceManager->RemoveIODevice( i );
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
		MODebug2->Push( moText("moConsole:: Loading MasterEffects configs...") );
		MODebug2->Push( moText("moConsole:: Master Effects.") + IntToStr(N)  );
	}

	state.m_nMasterEffects = N;

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();
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
			}
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Push( moText("moConsole:: Master Effects loaded.") );

}

void moConsole::UnloadMasterEffects() {

    for(MOuint i=0; i < m_EffectManager.MasterEffects().Count(); i++) {
		if( m_EffectManager.MasterEffects().Get(i) != NULL ) {
			m_EffectManager.RemoveEffect( i, MO_OBJECT_MASTEREFFECT );
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
		MODebug2->Push( moText("Loading PreEffects configs...") );
		MODebug2->Push( moText(" Pre-Effects.") + IntToStr(N) );
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
			ppreeffect = (moPreEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname,  MO_OBJECT_PREEFFECT, prfx, i);
			if (ppreeffect) {
				m_MoldeoObjects.Add( (moMoldeoObject*) ppreeffect );
				ppreeffect->SetResourceManager( m_pResourceManager );
				if ( ppreeffect->GetName() == moText("erase") ) {
					iborrado = i;
					printf("erase");
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
			}
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Push(moText("moConsole:: Pre Effects loaded."));
}

void moConsole::UnloadPreEffects() {

    for(MOuint i=0; i < m_EffectManager.PreEffects().Count(); i++) {
		if( m_EffectManager.PreEffects().Get(i) != NULL ) {
			m_EffectManager.RemoveEffect( i, MO_OBJECT_PREEFFECT );
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
		MODebug2->Push( moText("moConsole:: Loading Effects configs...") );
		MODebug2->Push( moText("moConsole:: Effects.") + IntToStr(N)  );
	}

	state.m_nEffects = N;

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i < N; i++) {
			Draw();
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();
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
			m_Config.NextValue();
		}
	}

	if (MODebug2) MODebug2->Push(moText("moConsole:: Effects loaded."));
}

void moConsole::UnloadEffects() {

    for(MOuint i=0; i < m_EffectManager.Effects().Count(); i++) {
		if( m_EffectManager.Effects().Get(i) != NULL ) {
			m_EffectManager.RemoveEffect( i, MO_OBJECT_EFFECT );
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
		MODebug2->Push( moText("moConsole:: Loading PostEffects configs...") );
		MODebug2->Push( moText("moConsole:: Post Effects.") + IntToStr(N)  );
	}

	state.m_nPostEffects = N;

	if(N>0) {
		m_Config.FirstValue();
		for( i=0; i<N; i++) {
			if(idebug!=-1) { Draw(); }
			fxname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text();
			cfname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text();
			lblname = m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_LABEL).Text();
			posteffect = (moPostEffect*)m_EffectManager.NewEffect( fxname, cfname, lblname,  MO_OBJECT_POSTEFFECT, ptfx , i );
			if (posteffect) {
				m_MoldeoObjects.Add( (moMoldeoObject*) posteffect );
				posteffect->SetResourceManager( m_pResourceManager );
				if(posteffect->GetName() == moText("debug")) {
						idebug = i;
						posteffect->Init();
						posteffect->state.on = MO_ON;
				}
			}
			m_Config.NextValue();
		}
	}
	if (MODebug2) MODebug2->Push(moText("moConsole:: Post Effects loaded."));
}

void moConsole::UnloadPostEffects() {

    for(MOuint i=0; i < m_EffectManager.PostEffects().Count(); i++) {
		if( m_EffectManager.PostEffects().Get(i) != NULL ) {
			m_EffectManager.RemoveEffect( i, MO_OBJECT_POSTEFFECT );
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

}

void moConsole::UnloadResources() {


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

    text = "Turning on MasterEffects...";
	if (MODebug2) MODebug2->Push(text);
	printf(text);


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

	return SDL_GetTicks();

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

	savename = "c:\\moldeo_session";

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



	//INTERACCION EFFECTS
	//channel0 y panel deben estar deshabilitados(las keys asociadas a ellos quedan liberadas)
		//se pasa el control de events al CanalMaestro array[0], el sabra a quien pasar el control
	if ( m_EffectManager.MasterEffects().Count() > 2 ) {
		pChannel = m_EffectManager.MasterEffects().Get(0);
		pPanel = m_EffectManager.MasterEffects().Get(2);
		if (pPanel && pChannel)
		if((pChannel->state.on != MO_ACTIVATED) && (pPanel->state.on != MO_ACTIVATED) ) {
			pChannel->Interaction( m_pIODeviceManager );
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
	p_configdefinition->Add( moText("mastereffects_on"), MO_PARAM_FUNCTION, CONSOLE_ON );
	p_configdefinition->Add( moText("fulldebug"), MO_PARAM_FUNCTION, CONSOLE_FULLDEBUG );
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
