/*******************************************************************************

                              moSequenceEffect.cpp

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

#include "moSequenceEffect.h"
/*

moSequenceEffect::moSequenceEffect() {
	SetName("sequence");
	SetType(MO_OBJECT_EFFECT);
}

moSequenceEffect::~moSequenceEffect() {
	Finish();
}

MOboolean moSequenceEffect::Init()
{
  if (!PreInit()) return false;

  moDefineParamIndex( SEQUENCE_INLET, "inlet" );
  moDefineParamIndex( SEQUENCE_OUTLET, "outlet" );
  moDefineParamIndex( SEQUENCE_SCRIPT, "script" );
  moDefineParamIndex( SEQUENCE_ALPHA, "alpha" );
  moDefineParamIndex( SEQUENCE_COLOR, "color" );
  moDefineParamIndex( SEQUENCE_SYNC, "syncro" );
  moDefineParamIndex( SEQUENCE_PHASE, "phase" );
  moDefineParamIndex( SEQUENCE_EFFECTS, "effects" );
  moDefineParamIndex( SEQUENCE_STATES, "sequence_states" );
  moDefineParamIndex( SEQUENCE_POSITION, "sequence_position" );
  moDefineParamIndex( SEQUENCE_TOTAL_DURATION, "sequence_total_duration" );
  moDefineParamIndex( SEQUENCE_PAUSED, "sequence_paused" );
  moDefineParamIndex( SEQUENCE_MODE, "sequence_mode" );
  moDefineParamIndex( SEQUENCE_LOOP, "sequence_loop" );

  m_EffectManager.Init();
	m_EffectManager.m_pEffectManager = &m_EffectManager;
	m_EffectManager.m_pResourceManager = GetResourceManager();

  m_Config.SetCurrentParam( "effects" );

  int pre=-1,on=-1;

	if (m_Config.FirstValue()) {
    do {
      moValue& mVal(m_Config.GetCurrentValue());
      moMobDefinition MoldeoObjectDef( mVal.GetSubValue( MO_CFG_EFFECT).Text(), mVal.GetSubValue( MO_CFG_EFFECT_CONFIG).Text(),
                                        MO_OBJECT_EFFECT, mVal.GetSubValue( MO_CFG_EFFECT_LABEL).Text() );
      moEffect* newEffect = m_EffectManager.New( MoldeoObjectDef );
      if (newEffect) {
          newEffect->Init();
          pre = mVal.GetSubValue(MO_CFG_EFFECT_PRE).Int();
          on = mVal.GetSubValue(MO_CFG_EFFECT_ON).Int();
          if (pre>=0) newEffect->GetConfig()->SetCurrentPreConf(pre);
          if (on>0) newEffect->Activate();
          else newEffect->Deactivate();
      }
    } while (m_Config.NextValue());
	}


  ///LEVANTA LOS ESTADOS de la SECUENCIA
  m_Config.SetCurrentParam( "sequence_states" );
	m_n_sequence_states = m_Config.GetCurrentParam().GetValuesCount();

	int i_sequence_states = 0;

	if (m_Config.FirstValue()) {
    do {
        moValue& mVal(m_Config.GetCurrentValue());
        moSequenceState SequenceState;

        if (mVal.GetSubValueCount()>1) {
          SequenceState.m_state_name = mVal.GetSubValue( 0 ).Text();
          SequenceState.Set( mVal.GetSubValue( 1 ).Text());
        }

        m_SequenceStates[i_sequence_states] = SequenceState;


        MODebug2->Message("sequence_states: " + mVal.GetSubValue( 0 ).Text() );
        i_sequence_states+= 1;
    } while (m_Config.NextValue());
	}
	m_Config[ moR(SEQUENCE_STATES)].SetIndexValue(0);

  m_Config.SetCurrentPreConf( 0 );

	return true;
}

int
moSequenceEffect::UpdateSequenceState( int i_state ) {

  if (i_state>=m_n_sequence_states) return -1;
  moSequenceState& SeqState( m_SequenceStates[i_state] );

  ///check auto -> dont wait -> if timer has reached duration
  /// or timer has reached (total_duration/m_n_sequence_states)
  ///

  if ( SeqState.m_Timer.Duration() ) {

  }

  return 0;
}

int
moSequenceEffect::NextSequenceState( int i_state ) {

  int i_next_state = i_state + 1;

  if (0<=i_state && i_state<m_n_sequence_states) {
    MODebug2->Message("Next Sequence.");
  } else {
    if (m_Config.Int( moR(SEQUENCE_LOOP)) > 0) {
      i_state = 0;
      MODebug2->Message("Looping Sequence.");
    } else {
      i_state = m_n_sequence_states-1;
      MODebug2->Message("End of Sequence reached.");
      return -1;
    }
  }

  return SetSequenceState( i_state );
}

int moSequenceEffect::SetSequenceState( int i_state ) {

  int key = 0;
  moEffect* pEffect=NULL;

  if (i_state >= m_n_sequence_states) return -1;

  moSequenceState& SeqState( m_SequenceStates[i_state] );

  ///Start timer
  SeqState.m_Timer.Start();

  /// Deactivate all effects
  for( key=0; key<m_EffectManager.AllEffects().Count(); key++) {

    pEffect = m_EffectManager.AllEffects().Get(key);

    if (pEffect) {
      pEffect->Deactivate();
    }

  }

  ///Activate this sequence key Effect
  pEffect = m_EffectManager.GetEffectByLabel( SeqState.m_Key.m_label_name );

  if (pEffect) {
    if (SeqState.m_Key.m_active>0) pEffect->Activate();
    if (SeqState.m_Key.m_preconfig_index>=0) pEffect->GetConfig()->SetCurrentPreConf( SeqState.m_Key.m_preconfig_index );
  }

  return 0;
}

void moSequenceEffect::UpdateParameters() {

  ///next sequence is next sequence in config param "sequence_states"
  if (m_i_sequence_states!=m_Config[ moR(SEQUENCE_STATES)].GetIndexValue()) {
      m_i_sequence_states = m_Config[ moR(SEQUENCE_STATES)].GetIndexValue();
      SetSequenceState(m_i_sequence_states);
  } else {
    //i_sequence_states = m_Config[ moR(SEQUENCE_STATES)].GetIndexValue()
    UpdateSequenceState(m_i_sequence_states);
  }

}

void moSequenceEffect::Draw( moTempo* tempogral,moEffectState* parentstate)
{
  moEffect* pEffect = NULL;
  moRenderManager* RenderMan = GetResourceManager()->GetRenderMan();

  UpdateParameters();

  BeginDraw( tempogral, parentstate);

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();                                     // Store The Modelview Matrix
  glLoadIdentity();


  for( int i=0; i<m_EffectManager.Effects().Count(); i++ ) {

      pEffect = m_EffectManager.Effects().GetRef(i);
      if(pEffect) {
          if(pEffect->Activated()) {
                  RenderMan->BeginDrawEffect();
                  pEffect->Draw(&m_EffectState.tempo);
                  RenderMan->EndDrawEffect();
          }
      }
  }

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();									// Restore The Old Projection Matrix

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix

	EndDraw();
}

MOboolean moSequenceEffect::Finish()
{
	return PreFinish();
}

void moSequenceEffect::Interaction(moIODeviceManager *consolaes) {
  consolaes = NULL;///unused
}


void
moSequenceEffect::LoadCodes(moIODeviceManager *consolaesarray) {

	//cargamos el especifico a este luego el de los efectos dentro del array
	moEffect::LoadCodes(consolaesarray);


}
*/
