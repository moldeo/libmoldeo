/*******************************************************************************

                              moSceneEffect.h

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

#ifndef __MO_SCENE_EFFECT_H__
#define __MO_SCENE_EFFECT_H__

#include "moSequenceEffect.h"
#include "moConsole.h"
#include "moEffectManager.h"

enum moSceneParamIndex {
   SCENE_INLET,
   SCENE_OUTLET,
   SCENE_SCRIPT,
   SCENE_ALPHA,
   SCENE_COLOR,
   SCENE_SYNC,
   SCENE_PHASE,
   SCENE_PREEFFECTS,
   SCENE_EFFECTS,
   SCENE_POSTEFFECTS,
   SCENE_STATES,
   SCENE_SEQUENCE_MODE, /** mode is by default: 0:fixed (no automatic advance between states), 1: automatic move to next sequence, 2:  */
   SCENE_SEQUENCE_DURATION, /** if mode is 1 (sequence mode) > the sequence is play in TOTAL_DURATION */
   SCENE_SEQUENCE_LOOP, /** if mode is 1: and loop is 1: sequence loop over the states... */
   SCENE_CREATE_LAYER /** create and independent FB */
 };


/// moKeyEffect
/**
  moKeyEffect is a Key object that reference one of the preconfiguration of a Scene Effects
*/
class LIBMOLDEO_API moKeyEffect {
  public:

    moText m_label_name;
    MOint  m_preconfig_index;
    MOint  m_active;
    moText m_action;

    MOint  m_fx_index;

    moKeyEffect() {
      m_label_name = "undefined";
      m_preconfig_index = 0;
      m_active = false;
      m_action = "";

      m_fx_index = -1;
    }

    virtual  ~moKeyEffect() {
    }

     moKeyEffect(const moKeyEffect& src ) {
      (*this) = src;
     }

     moKeyEffect& operator = ( const moKeyEffect& src ) {
        m_label_name = src.m_label_name;
        m_preconfig_index = src.m_preconfig_index;
        m_active = src.m_active;
        m_fx_index = src.m_fx_index;
        m_action = src.m_action;
        return (*this);
     }

     int Set( const moText& p_XmlText );

};

///moKeys
/**
  Arreglo de moKeyEffects
*/
moDeclareExportedDynamicArray( moKeyEffect, moKeys );

 class LIBMOLDEO_API moSceneState {

    public:

      moSceneState() {

        m_state_name = "";

        m_mode  = "auto";

        m_in = -1;
        m_out = -1;

        m_action = "";

      }
      virtual ~moSceneState() {}

      moSceneState(const moSceneState& src ) {
        (*this) = src;
      }

      moSceneState& operator = ( const moSceneState& src ) {

        m_state_name = src.m_state_name;

        m_mode = src.m_mode;

        m_in = src.m_in;
        m_out = src.m_out;
        m_action = src.m_action;

        m_SceneKeys = src.m_SceneKeys;
        m_Timer = src.m_Timer;
        return (*this);
      }

      int Set( const moText& p_XmlText );

      moText        m_state_name;


      moText        m_mode;

      int           m_in;
      int           m_out;

      moText        m_action;

      moKeys        m_SceneKeys;
      moTimer       m_Timer;
 };

/**
 * moSceneEffect
 * el efecto escena es una agrupación de efectos. Se comporta igual que un efecto, pero además permite la creación de una
 * capa de renderización independiente.
 * La escena agrupa estos efectos, pero también los administra de tal forma a generar sub-escenas que llamaremos Estados de Escena (moSceneState).
 * Cada uno de estos estados se define como un grupo de Claves de Escena (moSceneKey) que permiten incluir cada efecto,
 * seleccionar su pre-configuración y la acción (pause,stop,play) que se reproducirá.
 *
 * Posee algunos modos automáticos: "secuencia": los efectos son tratados como una secuencia
 *
 * @see moMoldeoObject
 * @see moSequenceEffect
 * @see moEffect
 * @see moSceneState
 * @see moSceneKey
 */
class LIBMOLDEO_API moSceneEffect : public moMasterEffect
{
  public:

      moSceneEffect();
      virtual ~moSceneEffect();

      MOboolean Init();
      void Draw(moTempo*,moEffectState* parentstate=NULL);
      MOboolean Finish();

      void Interaction(moIODeviceManager *);
      void LoadCodes(moIODeviceManager *);
      moConfigDefinition *GetDefinition( moConfigDefinition *p_configdefinition );

      /**
       * método de actualización de datos del objeto.
       * @param p_EventList puntero a la lista de eventos.
       */
      virtual void Update( moEventList* p_EventList );//checks p_EventList for events/messages

      void UpdateParameters();

      int UpdateSceneState( int i_state );
      int SetSceneState( int i_state );
      int NextSceneState( int i_state );

      moEffectManager&  GetEffectManager() {
        return m_EffectManager;
      }

      int GetObjectId( moText p_label_name );
      int UpdateMoldeoIds( moMoldeoObjects &p_MoldeoSceneObjects );

  private:

      int               m_i_scene_states;
      int               m_n_scene_states;
      moSceneState      m_SceneStates[1024];
      moEffectManager   m_EffectManager;
};

#endif

