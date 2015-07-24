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
#include <moArray.h>
moDefineDynamicArray( moKeys )


/** ======================================================================= */
/** KEY EFFECT */
/** ======================================================================= */

#include <tinyxml.h>
int
moKeyEffect::Set( const moText& p_XmlText ) {
  TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );

  if (rootKey) {
    TiXmlElement* keyNode = rootKey->FirstChildElement("moKey");
    if (keyNode) {
      m_label_name = moText( keyNode->Attribute("label_name") );
      m_preconfig_index = atoi( moText( keyNode->Attribute("preconfig") ) );
      m_active = atoi( moText( keyNode->Attribute("active") ) );
      m_action = moText( keyNode->Attribute("action") );
      return 0;
    }
  }
  return -1;
}


/** ======================================================================= */
/** SCENE STATE */
/** ======================================================================= */


int
moSceneState::Set( const moText& p_XmlText ) {

  TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  //TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );
  TiXmlElement* sceneStateNode = m_XMLDoc.FirstChildElement("moSceneState");

  //if (rootKey) {

    //TiXmlElement* sceneStateNode = rootKey->FirstChildElement("moSceneState");
    if (sceneStateNode) {
      m_mode = moText( sceneStateNode->Attribute("mode") );
      m_in = atoi( sceneStateNode->Attribute("in") );
      m_out = atoi( sceneStateNode->Attribute("out") );
      m_action = moText( sceneStateNode->Attribute("action") );
      TiXmlElement* keyNode = sceneStateNode->FirstChildElement("moKey");
      while(keyNode) {
        moKeyEffect Key;
        Key.m_label_name = moText( keyNode->Attribute("label_name") );
        Key.m_preconfig_index = atoi( moText( keyNode->Attribute("preconfig") ) );
        Key.m_active = atoi( moText( keyNode->Attribute("active") ) );
        Key.m_action = moText( keyNode->Attribute("action") );
        m_SceneKeys.Add(Key);

        keyNode = keyNode->NextSiblingElement("moKey");
      }
      return 0;
    } else moDebugManager::Log( "No XML moSceneState in: " + p_XmlText );

  //} else moDebugManager::Error();
  return -1;
}



/** ======================================================================= */


moSceneEffect::moSceneEffect() {
	SetName("scene");
	SetType(MO_OBJECT_EFFECT);
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
  moDefineParamIndex( SCENE_SYNC, "syncro" );
  moDefineParamIndex( SCENE_PHASE, "phase" );
  moDefineParamIndex( SCENE_PREEFFECTS, "preeffect" );
  moDefineParamIndex( SCENE_EFFECTS, "effect" );
  moDefineParamIndex( SCENE_POSTEFFECTS, "posteffect" );
  moDefineParamIndex( SCENE_STATES, "scene_states" );
  moDefineParamIndex( SCENE_SEQUENCE_MODE, "sequence_mode" );
  moDefineParamIndex( SCENE_SEQUENCE_DURATION, "sequence_duration" );
  moDefineParamIndex( SCENE_SEQUENCE_LOOP, "sequence_loop" );
  moDefineParamIndex( SCENE_CREATE_LAYER, "create_layer" );

  m_i_scene_states = -1;
	m_EffectManager.Init();
	m_EffectManager.m_pEffectManager = &m_EffectManager;
	m_EffectManager.m_pResourceManager = GetResourceManager();

	moEffect* newEffect = NULL;
  bool res = false;
  int pre=-1,on=-1;
	/** CREATING NEW LAYER!! */
	//RenderMan->CreateNewLayer( this );

  m_Config.SetCurrentParam( "preeffect" );
	if (m_Config.FirstValue()) {
    do {
      moValue& mVal(m_Config.GetCurrentValue());
      moMobDefinition MoldeoObjectDef( mVal.GetSubValue( MO_CFG_EFFECT).Text(), mVal.GetSubValue( MO_CFG_EFFECT_CONFIG).Text(),
                                        MO_OBJECT_PREEFFECT, mVal.GetSubValue( MO_CFG_EFFECT_LABEL).Text() );

      newEffect = m_EffectManager.New( MoldeoObjectDef );
      if (newEffect) {
          res = newEffect->Init();
          pre = mVal.GetSubValue(MO_CFG_EFFECT_PRE).Int();
          on = mVal.GetSubValue(MO_CFG_EFFECT_ON).Int();
          if (pre>=0) newEffect->GetConfig()->SetCurrentPreConf(pre);
          if (on>0) newEffect->Activate();
          else newEffect->Deactivate();
      }
    } while (m_Config.NextValue());
	}


  m_Config.SetCurrentParam( "effect" );
	if (m_Config.FirstValue()) {
    do {
      moValue& mVal(m_Config.GetCurrentValue());
      moMobDefinition MoldeoObjectDef( mVal.GetSubValue( MO_CFG_EFFECT).Text(), mVal.GetSubValue( MO_CFG_EFFECT_CONFIG).Text(),
                                        MO_OBJECT_EFFECT, mVal.GetSubValue( MO_CFG_EFFECT_LABEL).Text() );
      newEffect = m_EffectManager.New( MoldeoObjectDef );
      if (newEffect) {
          res = newEffect->Init();
          pre = mVal.GetSubValue(MO_CFG_EFFECT_PRE).Int();
          on = mVal.GetSubValue(MO_CFG_EFFECT_ON).Int();
          if (pre>=0) newEffect->GetConfig()->SetCurrentPreConf(pre);
          if (on>0) newEffect->Activate();
          else newEffect->Deactivate();
      }

    } while (m_Config.NextValue());
	}

  m_Config.SetCurrentParam( "posteffect" );
	if (m_Config.FirstValue()) {
    do {
      moValue& mVal(m_Config.GetCurrentValue());
      moMobDefinition MoldeoObjectDef( mVal.GetSubValue( MO_CFG_EFFECT).Text(), mVal.GetSubValue( MO_CFG_EFFECT_CONFIG).Text(),
                                        MO_OBJECT_POSTEFFECT, mVal.GetSubValue( MO_CFG_EFFECT_LABEL).Text() );
      newEffect = m_EffectManager.New( MoldeoObjectDef );
      if (newEffect) {
          res = newEffect->Init();
          pre = mVal.GetSubValue(MO_CFG_EFFECT_PRE).Int();
          on = mVal.GetSubValue(MO_CFG_EFFECT_ON).Int();
          if (pre>=0) newEffect->GetConfig()->SetCurrentPreConf(pre);
          if (on>0) newEffect->Activate();
          else newEffect->Deactivate();
      }

    } while (m_Config.NextValue());
	}


	m_Config.SetCurrentParam( "scene_states" );
	m_n_scene_states = m_Config.GetCurrentParam().GetValuesCount();

	int i_scene_states = 0;
	if (m_Config.FirstValue()) {
    do {
        moValue& mVal(m_Config.GetCurrentValue());
        moSceneState SceneState;

        if (mVal.GetSubValueCount()>1) {
          SceneState.m_state_name = mVal.GetSubValue( 0 ).Text();
          SceneState.Set( mVal.GetSubValue( 1 ).Text());
        }

        m_SceneStates[i_scene_states] = SceneState;

        MODebug2->Message("scene_states: " + mVal.GetSubValue( 0 ).Text() );
        i_scene_states+= 1;
    } while (m_Config.NextValue());
	}
	m_Config[ moR(SCENE_STATES)].SetIndexValue(0);

  m_Config.SetCurrentPreConf( 0 );

	return true;
}

int
moSceneEffect::GetObjectId( moText p_label_name ) {

  moEffect* pEffect = m_EffectManager.GetEffectByLabel( p_label_name );

  if (pEffect) {

    return pEffect->GetId();

  }

  return -1;

}


int
moSceneEffect::UpdateMoldeoIds( moMoldeoObjects &p_MoldeoSceneObjects ) {

  for( int i=0; i<(int)m_EffectManager.AllEffects().Count(); i++ ) {

    moEffect* childFx = m_EffectManager.AllEffects().Get(i);

    moMobDefinition pChildDef = childFx->GetMobDefinition();
    pChildDef.SetMoldeoFatherId(this->GetId());
    pChildDef.SetFatherLabelName(this->GetLabelName());
    pChildDef.SetMoldeoId(MO_MOLDEOSCENEOBJECTS_OFFSET_ID + p_MoldeoSceneObjects.Count() );

    ///set definition in object
    childFx->SetMobDefinition(pChildDef);

    p_MoldeoSceneObjects.Add( (moMoldeoObject*) childFx );

    /** RECURSIVE */
    if ( childFx->GetName()=="scene" ) {
      moSceneEffect* pScene = (moSceneEffect*) childFx;
      pScene->UpdateMoldeoIds( p_MoldeoSceneObjects );
    }

  }
  return 0;
}

int
moSceneEffect::UpdateSceneState( int i_state ) {
/*
  if (0<=i_state && i_state<m_n_scene_states) {
    moSceneState& SceneState( m_SceneStates[i_state] );
*/
  ///check auto -> dont wait -> if timer has reached duration
  /// or timer has reached (total_duration/m_n_sequence_states)

  ///


/*
    if ( SceneState.m_Timer.Duration() ) {

    }
*/
  //}
  if (i_state>0) return i_state;

  return 0;
}

int
moSceneEffect::NextSceneState( int i_state ) {

  int i_next_state = i_state + 1;

  if (0<=i_next_state && i_next_state<m_n_scene_states) {
    MODebug2->Message("Next Scene State: " + IntToStr(i_next_state));
  } else {
    if (m_Config.Int( moR(SCENE_SEQUENCE_LOOP)) > 0) {
      i_next_state = 0;
      MODebug2->Message("Looping States.");
    } else {
      i_next_state = m_n_scene_states-1;
      MODebug2->Message("End of States reached.");
      return -1;
    }
  }

  return SetSceneState( i_next_state );
}

int moSceneEffect::SetSceneState( int i_state ) {

  int key = 0;
  moEffect* pEffect=NULL;

  if (i_state >= m_n_scene_states) return -1;

  moSceneState& SceneState( m_SceneStates[i_state] );

  ///Start timer
  SceneState.m_Timer.Start();

  /// Deactivate all effects

  for( key=0; key<(int)m_EffectManager.AllEffects().Count(); key++) {

    pEffect = m_EffectManager.AllEffects().Get(key);

    if (pEffect) {
      if (SceneState.m_action=="hideall"
          || SceneState.m_SceneKeys.Count()>0 ) {
        pEffect->Deactivate();
      }

      if (SceneState.m_action=="showall") {
        pEffect->Activate();
      }
    }

  }

  for( key=0; key<(int)SceneState.m_SceneKeys.Count(); key++) {
    moKeyEffect& Key(SceneState.m_SceneKeys[key]);

    pEffect = m_EffectManager.GetEffectByLabel( Key.m_label_name );

    if (pEffect) {
      if ( Key.m_active>0 ) pEffect->Activate();
      if ( Key.m_preconfig_index>=0 && pEffect->GetConfig()) pEffect->GetConfig()->SetCurrentPreConf( Key.m_preconfig_index );
      if ( Key.m_action=="stop") pEffect->Stop();
      if ( Key.m_action=="play") pEffect->Play();
      if ( Key.m_action=="pause") pEffect->Pause();
    }
  }

  return 0;
}


void moSceneEffect::UpdateParameters() {

  //int key = 0;

  ///next sequence is next sequence in config param "sequence_states"

  if (m_i_scene_states!=m_Config[ moR(SCENE_STATES)].GetIndexValue()) {
      m_i_scene_states = m_Config[ moR(SCENE_STATES)].GetIndexValue();
      SetSceneState(m_i_scene_states);
  } else {
    //i_sequence_states = m_Config[ moR(SEQUENCE_STATES)].GetIndexValue()
    UpdateSceneState(m_i_scene_states);
  }

}


void
moSceneEffect::Update( moEventList* p_EventList ) {

  UpdateParameters();

  moMoldeoObject::Update(p_EventList);

}

void moSceneEffect::Draw( moTempo* tempogral, moEffectState* parentstate)
{
	MOuint i;
  MOswitch borrar = MO_ACTIVATED;
  moEffect* pEffect = NULL;
  bool pre_effect_on = false;
  moRenderManager* RenderMan = GetResourceManager()->GetRenderMan();


  BeginDraw( tempogral, parentstate );

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();                                     // Store The Modelview Matrix
	glLoadIdentity();

  /** ACTIVAR EL PRE-EFFECT */
  for(i=1 /*starting from index 0+1 (first is expected to be ERASE*/; i<m_EffectManager.PreEffects().Count(); i++ ) {
    pEffect = m_EffectManager.PreEffects().GetRef(i);
    if( pEffect ) {
      if( pEffect->Activated() ) {
        pre_effect_on = true;
        RenderMan->BeginDrawEffect();

        moEffectState fstate = pEffect->GetEffectState();
        fstate.alpha = m_EffectState.alpha;
        pEffect->SetEffectState( fstate );

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

            moEffectState fstate = pEffect->GetEffectState();
            fstate.alpha = m_EffectState.alpha;
            pEffect->SetEffectState( fstate );

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

                  moEffectState fstate = pEffect->GetEffectState();
                  fstate.alpha = m_EffectState.alpha;
                  pEffect->SetEffectState( fstate );

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

        moEffectState fstate = pEffect->GetEffectState();
        fstate.alpha = m_EffectState.alpha;
        pEffect->SetEffectState( fstate );

        pEffect->Draw(&m_EffectState.tempo);

        RenderMan->EndDrawEffect();
      }
    }
  }

  ///TODO: corregir esto, debe hacerse este paso sobre la textura final de ete efecto (FBO)
  ///if (RenderMan) RenderMan->CopyRenderToTexture(MO_FINAL_TEX);

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
	p_configdefinition->Add( moText("preeffect"), MO_PARAM_TEXT, SCENE_PREEFFECTS );
	p_configdefinition->Add( moText("effect"), MO_PARAM_TEXT, SCENE_EFFECTS );
	p_configdefinition->Add( moText("posteffect"), MO_PARAM_TEXT, SCENE_POSTEFFECTS );

  moValue StateDefault( "State A", MO_VALUE_TXT );
  StateDefault.AddSubValue("<moSceneState action=\"showall\"/>","XML");

	p_configdefinition->Add( moText("scene_states"), MO_PARAM_TEXT, SCENE_STATES, StateDefault );
	p_configdefinition->Add( moText("sequence_mode"), MO_PARAM_NUMERIC, SCENE_SEQUENCE_MODE, moValue( "0", MO_VALUE_NUM ) );
	p_configdefinition->Add( moText("sequence_loop"), MO_PARAM_NUMERIC, SCENE_SEQUENCE_LOOP, moValue( "0", MO_VALUE_NUM ) );
  p_configdefinition->Add( moText("sequence_duration"), MO_PARAM_NUMERIC, SCENE_SEQUENCE_DURATION, moValue( "0", MO_VALUE_NUM ) );
	p_configdefinition->Add( moText("create_layer"), MO_PARAM_NUMERIC, SCENE_CREATE_LAYER, moValue( "0", MO_VALUE_NUM ) );

	return p_configdefinition;
}


void
moSceneEffect::LoadCodes(moIODeviceManager *consolaesarray) {

	//cargamos el especifico a este luego el de los efectos dentro del array
	moEffect::LoadCodes(consolaesarray);


}

