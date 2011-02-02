/*******************************************************************************

                              moSceneEffect.cpp

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

#include "moSceneEffect.h"

moSceneEffect::moSceneEffect() {
	SetName("scene");
}

moSceneEffect::moSceneEffect(char *nom) {
	SetName(nom);
}

moSceneEffect::~moSceneEffect() {
	Finish();
}

//#include "moConsole.h"

MOboolean moSceneEffect::Init()
{
	MOuint i;

    PreInit();

	idp_alpha = m_Config.GetParamIndex("alpha");

	idp_effects = m_Config.GetParamIndex("effects");
	idp_scenes = m_Config.GetParamIndex("scenes");

	plugins.Init(0,NULL);

	//generamos el array para los efectos
	effects.Init(m_Config.GetValuesCount(idp_effects),NULL);

	//generamos los efectos
	m_Config.SetCurrentParamIndex(idp_effects);
	m_Config.FirstValue();
	for(i=0; i<effects.Count(); i++) {
		effects[i] = moNewEffect(m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT).Text(), plugins);
		if(effects[i]!=NULL) {
			effects[i]->SetResourceManager( m_pResourceManager );
			effects[i]->SetConfigName(  m_Config.GetParam().GetValue().GetSubValue(MO_CFG_EFFECT_CONFIG).Text() );
                        char* str = effects[i]->GetName(); //(by Andres)
			printf("escena nombre efecto %i: %s\n",i,str);
		}
		m_Config.NextValue();
	}

	//INICIALIZACION DE CADA EFFECT(LA ASIGNACION DE CODIGOS DE DISPOSITIVO SE HACE APARTE)
	for(i=0; i<effects.Count(); i++) {
		if(effects[i]!=NULL) {
			effects[i]->state.fulldebug = state.fulldebug;
			effects[i]->Init();
			effects[i]->state.on = MO_ON;
		}
	}

    if(m_Config.GetPreConfCount() > 0)
        m_Config.PreConfFirst();

	return true;
}

void moSceneEffect::Draw( moTempo* tempogral,moEffectState* parentstate)
{
	MOuint i;

    PreDraw( tempogral, parentstate);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();                                     // Store The Modelview Matrix
	glLoadIdentity();
    // Dejamos todo como lo encontramos //

	//Se dibujan los efectos
	for(i=0;i<effects.Count();i++) {
		if(effects[i]!=NULL) {
			if(effects[i]->state.on==MO_ON) {
				effects[i]->GetConfig()->SetCurrentPreConf( m_Config.GetParam(idp_scenes).GetValue().GetSubValue(i).Int());
				effects[i]->Draw(&state.tempo,&state);
			}
		}
	}


	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();									// Restore The Old Projection Matrix

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
}

MOboolean moSceneEffect::Finish()
{
	for(MOuint i=0; i<effects.Count(); i++) {
		if(effects[i]!=NULL) {
			effects[i]->Finish();
			moDeleteEffect( effects[i], plugins );
		}
		effects[i] = NULL;
	}
	effects.Empty();
	plugins.Finish();

    return PreFinish();
}

void moSceneEffect::Interaction(moIODeviceManager *consolaes) {
/**/

}


void
moSceneEffect::LoadCodes(moIODeviceManager *consolaesarray) {

	//cargamos el especifico a este luego el de los efectos dentro del array
	moEffect::LoadCodes(consolaesarray);

	//ASIGNACION DE CODIGOS DE DISPOSITIVO(cargados del config de cada efecto)
	for(MOuint i=0;i<effects.Count();i++) {
		if(effects[i]!=NULL) {
			effects[i]->LoadCodes(consolaesarray);
		}
	}


}
