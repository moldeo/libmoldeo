/*******************************************************************************

                              moEffectManager.cpp

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

#include "moEffectManager.h"


//===========================================
//
//				moEffectManager
//
//===========================================

moEffectManager::moEffectManager() {
}

moEffectManager::~moEffectManager() {
	Finish();
}

moEffect*
moEffectManager::New( moMobDefinition& p_MobDefinition ) {

    return NewEffect(   p_MobDefinition.GetName(),
                        p_MobDefinition.GetConfigName(),
                        p_MobDefinition.GetLabelName(),
                        p_MobDefinition.GetType(),
                        p_MobDefinition.GetMobIndex().GetParamIndex(),
                        p_MobDefinition.GetMobIndex().GetValueIndex()
                    );
}

moEffect*
moEffectManager::NewEffect( moText p_resname, moText p_configname, moText p_labelname, moMoldeoObjectType p_type, MOint p_paramindex, MOint p_valueindex ) {

	moEffect* peffect = NULL;

	switch( p_type ) {
		case MO_OBJECT_EFFECT:
			if ( p_resname==moText("scene") ) {
				peffect = new moSceneEffect();
			} else if ( p_resname==moText("sequence") ) {
				peffect = new moSequenceEffect();
			} else {
				peffect = moNewEffect( p_resname, m_Plugins);
			}
			m_Effects.Add( peffect );
			break;

		case MO_OBJECT_PREEFFECT:
			peffect = moNewPreEffect( p_resname, m_PrePlugins );
			m_PreEffects.Add( (moPreEffect*)peffect );
			break;

		case MO_OBJECT_POSTEFFECT:
			peffect = moNewPostEffect( p_resname, m_PostPlugins );
			m_PostEffects.Add( (moPostEffect*)peffect );
			break;

		case MO_OBJECT_MASTEREFFECT:
			peffect = moNewMasterEffect( p_resname, m_MasterPlugins );
			m_MasterEffects.Add( (moMasterEffect*)peffect );
			break;
		default:
			break;
	}

	if (peffect) {
		peffect->SetConfigName( p_configname );
		peffect->SetLabelName( p_labelname );
        peffect->GetMobDefinition().GetMobIndex().SetParamIndex(p_paramindex);
        peffect->GetMobDefinition().GetMobIndex().SetValueIndex(p_valueindex);
		m_AllEffects.Add((moEffect*) peffect);
	}

	return (peffect);
}

MOboolean
moEffectManager::AddEffect( moEffect* m_pEffect ) {

    if (m_pEffect)
	switch( m_pEffect->GetType() ) {
		case MO_OBJECT_EFFECT:
			m_Effects.Add( m_pEffect );
			m_AllEffects.Add( m_pEffect );
			return true;
			break;
		case MO_OBJECT_PREEFFECT:
			m_PreEffects.Add( (moPreEffect*)m_pEffect );
			m_AllEffects.Add( m_pEffect );
			return true;
			break;
		case MO_OBJECT_POSTEFFECT:
			m_PostEffects.Add( (moPostEffect*)m_pEffect );
			m_AllEffects.Add( m_pEffect );
			return true;
			break;
		case MO_OBJECT_MASTEREFFECT:
			m_MasterEffects.Add( (moMasterEffect*)m_pEffect );
			m_AllEffects.Add( m_pEffect );
			return true;
			break;
		default:
			break;
	}

	return false;
}

MOboolean
moEffectManager::RemoveEffect( MOint p_ID, moMoldeoObjectType p_type ) {

	MOint all_ID;
	MOuint i;

	moEffect* peffect = NULL;
	moPreEffect* ppreeffect = NULL;
	moPostEffect* pposteffect = NULL;
	moMasterEffect* pmastereffect = NULL;

	switch( p_type ) {
		case MO_OBJECT_EFFECT:
			peffect = m_Effects.Get(p_ID);
			if (peffect) {
				all_ID = GetEffectId( peffect->GetLabelName() );
				moDeleteEffect( peffect, m_Plugins );
				m_Effects.Remove(p_ID);
				for(i=0;i<m_Effects.Count();i++) {
				    if (m_Effects[i]!=NULL) {
                        m_Effects[i]->GetMobDefinition().GetMobIndex().SetValueIndex(i);
				    }
				}
			}
			break;
		case MO_OBJECT_PREEFFECT:
			ppreeffect = m_PreEffects.Get(p_ID);
			if (ppreeffect) {
				all_ID = GetEffectId( ppreeffect->GetLabelName() );
				moDeletePreEffect( ppreeffect, m_PrePlugins );
				m_PreEffects.Remove(p_ID);
				for(i=0;i<m_PreEffects.Count();i++) {
				    if (m_PreEffects[i]) {
                        m_PreEffects[i]->GetMobDefinition().GetMobIndex().SetValueIndex(i);
				    }
				}
			}
			break;
		case MO_OBJECT_POSTEFFECT:
			pposteffect = m_PostEffects.Get(p_ID);
			if (pposteffect) {
				all_ID = GetEffectId( pposteffect->GetLabelName() );
				moDeletePostEffect( pposteffect, m_PostPlugins );
				m_PostEffects.Remove(p_ID);
				for(i=0;i<m_PostEffects.Count();i++) {
				    if (m_PostEffects[i]) {
                        m_PostEffects[i]->GetMobDefinition().GetMobIndex().SetValueIndex(i);
				    }
				}
			}
			break;
		case MO_OBJECT_MASTEREFFECT:
			pmastereffect = m_MasterEffects.Get(p_ID);
			if (pmastereffect) {
				all_ID = GetEffectId( pmastereffect->GetLabelName() );
				moDeleteMasterEffect( pmastereffect, m_MasterPlugins );
				m_MasterEffects.Remove(p_ID);
				for(i=0;i<m_MasterEffects.Count();i++)
                    if (m_MasterEffects[i]) {
                        m_MasterEffects[i]->GetMobDefinition().GetMobIndex().SetValueIndex(i);
                    }
			}
			break;
		default:
			break;
	}

	m_AllEffects.Remove(all_ID);

	return true;
}

MOint
moEffectManager::GetEffectId( moText p_labelname ) {

	for(MOuint i=0;i<m_AllEffects.Count();i++) {
		if ( m_AllEffects.Get(i) != NULL ) {
			if ( m_AllEffects.Get(i)->GetLabelName() == p_labelname) {
				return i;
			}
		}
	}
	return -1;//not found
}

moText
moEffectManager::GetEffectLabelName( MOint p_ID ) {

	if ( m_AllEffects.Get(p_ID) != NULL ) {
		return m_AllEffects.Get(p_ID)->GetLabelName();
	}

	return moText("");//not found

}

MOboolean
moEffectManager::Init() {

	m_PreEffects.Init( 0, NULL);
	m_Effects.Init( 0, NULL);
	m_PostEffects.Init( 0, NULL);
	m_MasterEffects.Init( 0, NULL);
	m_AllEffects.Init( 0, NULL);

	m_PrePlugins.Init( 0, NULL);
	m_Plugins.Init( 0, NULL);
	m_PostPlugins.Init( 0, NULL);
	m_MasterPlugins.Init( 0, NULL);

	return false;
}

MOboolean
moEffectManager::Finish() {

	m_PreEffects.Finish();
	m_Effects.Finish();
	m_PostEffects.Finish();
	m_MasterEffects.Finish();
	m_AllEffects.Finish();

	m_PrePlugins.Finish();
	m_Plugins.Finish();
	m_PostPlugins.Finish();
	m_MasterPlugins.Finish();

	return true;
}

moEffectsArray&
moEffectManager::AllEffects() {
	return m_AllEffects;
}

moPreEffectsArray&
moEffectManager::PreEffects() {
	return m_PreEffects;
}

moEffectsArray&
moEffectManager::Effects() {
	return m_Effects;
}

moPostEffectsArray&
moEffectManager::PostEffects() {
	return m_PostEffects;
}

moMasterEffectsArray&
moEffectManager::MasterEffects() {
	return m_MasterEffects;
}
