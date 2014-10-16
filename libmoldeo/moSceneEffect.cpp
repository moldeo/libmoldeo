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
  if (!PreInit()) return false;

  moDefineParamIndex( SCENE_INLET, "inlet" );
  moDefineParamIndex( SCENE_OUTLET, "outlet" );
  moDefineParamIndex( SCENE_SCRIPT, "script" );
  moDefineParamIndex( SCENE_ALPHA, "alpha" );
  moDefineParamIndex( SCENE_COLOR, "color" );
  moDefineParamIndex( SCENE_SYNC, "sync" );
  moDefineParamIndex( SCENE_PHASE, "phase" );
  moDefineParamIndex( SCENE_PREEFFECTS, "preeffects" );
  moDefineParamIndex( SCENE_PREEFFECTS, "posteffects" );
  moDefineParamIndex( SCENE_PREEFFECTS, "mastereffects" );
  moDefineParamIndex( SCENE_PREEFFECTS, "scene_states" );
  moDefineParamIndex( SCENE_CREATE_LAYER, "create_layer" );


	m_EffectManager.Init();

	/** CREATING NEW LAYER!! */
	//RenderMan->CreateNewLayer( this );
  m_Config.SetCurrentParam( "preeffects" );
	if (m_Config.FirstValue()) {
    do {
      moValue& mVal(m_Config.GetCurrentValue());
      moMobDefinition MoldeoObjectDef( mVal.GetSubValue( MO_CFG_EFFECT).Text(), mVal.GetSubValue( MO_CFG_EFFECT_CONFIG).Text(),
                                        MO_OBJECT_PREEFFECT, mVal.GetSubValue( MO_CFG_EFFECT_LABEL).Text() );
      m_EffectManager.New( MoldeoObjectDef );

    } while (m_Config.NextValue());
	}

  m_Config.SetCurrentParam( "effects" );
	if (m_Config.FirstValue()) {
    do {
      moValue& mVal(m_Config.GetCurrentValue());
      moMobDefinition MoldeoObjectDef( mVal.GetSubValue( MO_CFG_EFFECT).Text(), mVal.GetSubValue( MO_CFG_EFFECT_CONFIG).Text(),
                                        MO_OBJECT_EFFECT, mVal.GetSubValue( MO_CFG_EFFECT_LABEL).Text() );
      m_EffectManager.New( MoldeoObjectDef );

    } while (m_Config.NextValue());
	}

  m_Config.SetCurrentParam( "posteffects" );
	if (m_Config.FirstValue()) {
    do {
      moValue& mVal(m_Config.GetCurrentValue());
      moMobDefinition MoldeoObjectDef( mVal.GetSubValue( MO_CFG_EFFECT).Text(), mVal.GetSubValue( MO_CFG_EFFECT_CONFIG).Text(),
                                        MO_OBJECT_POSTEFFECT, mVal.GetSubValue( MO_CFG_EFFECT_LABEL).Text() );
      m_EffectManager.New( MoldeoObjectDef );

    } while (m_Config.NextValue());
	}
/*
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

			moEffectState fxstate = effects[i]->GetEffectState();
			fxstate.fulldebug = m_EffectState.fulldebug;

            effects[i]->SetEffectState( fxstate );
			effects[i]->Init();
			effects[i]->Activate();

		}
	}

    if(m_Config.GetPreConfCount() > 0)
        m_Config.PreConfFirst();
*/
	return true;
}

void moSceneEffect::Draw( moTempo* tempogral, moEffectState* parentstate)
{
	MOuint i;
  MOswitch borrar = MO_ACTIVATED;
  moEffect* pEffect = NULL;
  bool pre_effect_on = false;
  moRenderManager* RenderMan = GetResourceManager()->GetRenderMan();

  BeginDraw( tempogral, parentstate);

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();                                     // Store The Modelview Matrix
	glLoadIdentity();
    // Dejamos todo como lo encontramos //

	//Se dibujan los efectos
	/*
	for(i=0;i<effects.Count();i++) {
		if(effects[i]!=NULL) {
			if(effects[i]->Activated()) {
				effects[i]->GetConfig()->SetCurrentPreConf( m_Config.GetParam(idp_scenes).GetValue().GetSubValue(i).Int());

				moEffectState scene_state = GetEffectState();
				moTempo scene_tempo = GetEffectState().tempo;

				effects[i]->Draw(&scene_tempo, &scene_state );
			}
		}
	}
*/


  /** ACTIVAR EL PRE-EFFECT */
  for(i=1; i<m_EffectManager.PreEffects().Count(); i++ ) {
    pEffect = m_EffectManager.PreEffects().GetRef(i);
    if( pEffect ) {
      if( pEffect->Activated() ) {
        pre_effect_on = true;
        RenderMan->BeginDrawEffect();
        pEffect->Draw(&m_EffectState.tempo);
        RenderMan->EndDrawEffect();
        borrar = MO_DEACTIVATED;
      }
    }
  }

  if (borrar==MO_ACTIVATED) {

      if( m_EffectManager.PreEffects().Count()>0 ) {
        pEffect = m_EffectManager.PreEffects().GetRef(0);
        if( pEffect ) {
          if( pEffect->Activated() )
          {
            if (RenderMan) RenderMan->BeginDrawEffect();
            pEffect->Draw(&m_EffectState.tempo);
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

  /** ACTIVAR EL EFFECT */

  //Se dibujan los Effects
  for( i=0; i<m_EffectManager.Effects().Count(); i++ ) {

      pEffect = m_EffectManager.Effects().GetRef(i);
      if(pEffect) {
          if(pEffect->Activated()) {
                  RenderMan->BeginDrawEffect();
                  pEffect->Draw(&m_EffectState.tempo);
                  RenderMan->EndDrawEffect();
          }
      }
  }

  //RenderMan->CopyRenderToTexture(MO_EFFECTS_TEX);

  /** ACTIVAR EL POST-EFFECT */

  for(i=0;i<m_EffectManager.PostEffects().Count();i++) {

    pEffect = m_EffectManager.PostEffects().GetRef(i);

    if(pEffect) {
      if(pEffect->Activated()) {
        RenderMan->BeginDrawEffect();
        pEffect->Draw(&m_EffectState.tempo);
        RenderMan->EndDrawEffect();
      }
    }
  }


  if (RenderMan) RenderMan->CopyRenderToTexture(MO_FINAL_TEX);

  /** GUARDAR LA TEXTURA -> scene_xxx_texture */


	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();									// Restore The Old Projection Matrix

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix

	EndDraw();
}

MOboolean moSceneEffect::Finish()
{

  m_EffectManager.Finish();

  return PreFinish();
}

void moSceneEffect::Interaction(moIODeviceManager *consolaes) {
/** TODO: */
  consolaes = NULL;///unused
}

moConfigDefinition *
moSceneEffect::GetDefinition( moConfigDefinition *p_configdefinition ) {

	//default: alpha, color, syncro
	p_configdefinition = moEffect::GetDefinition( p_configdefinition );
	p_configdefinition->Add( moText("preeffects"), MO_PARAM_TEXT, SCENE_PREEFFECTS, moValue( "", MO_VALUE_TXT ) );
	p_configdefinition->Add( moText("effects"), MO_PARAM_TEXT, SCENE_EFFECTS, moValue( "", MO_VALUE_TXT ) );
	p_configdefinition->Add( moText("posteffects"), MO_PARAM_TEXT, SCENE_POSTEFFECTS, moValue( "", MO_VALUE_TXT ) );
	p_configdefinition->Add( moText("scene_states"), MO_PARAM_VECTOR, SCENE_SCENE_STATES, moValue( "0", MO_VALUE_NUM ) );
	p_configdefinition->Add( moText("create_layer"), MO_PARAM_NUMERIC, SCENE_CREATE_LAYER, moValue( "0", MO_VALUE_NUM ) );

	return p_configdefinition;
}


void
moSceneEffect::LoadCodes(moIODeviceManager *consolaesarray) {

	//cargamos el especifico a este luego el de los efectos dentro del array
	moEffect::LoadCodes(consolaesarray);

	//ASIGNACION DE CODIGOS DE DISPOSITIVO(cargados del config de cada efecto)
	/*
	for(MOuint i=0;i<effects.Count();i++) {
		if(effects[i]!=NULL) {
			effects[i]->LoadCodes(consolaesarray);
		}
	}
	*/


}

