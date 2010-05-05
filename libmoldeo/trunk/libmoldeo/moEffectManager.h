/*******************************************************************************

								moEffectManager.h

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
  Andrs Colubri

*******************************************************************************/

#include "moTypes.h"
#include "moArray.h"
#include "moEffect.h"
#include "moPostEffect.h"
#include "moPreEffect.h"
#include "moMasterEffect.h"
#include "moSceneEffect.h"
#include "moSequenceEffect.h"
#include "moPlugin.h"
#include "moPostPlugin.h"
#include "moPrePlugin.h"
#include "moMasterPlugin.h"

#ifndef __MOEFFECTMANAGER_H
#define __MOEFFECTMANAGER_H

/// Administrador de efectos
/**
 * clase administradora de instancias de objetos derivados de moEffect
 * a travs de esta clase se tiene acceso a los efectos que han sido creados y pueden ser accedidos, modificados y controlados
 * @see moMoldeoObject
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */
class LIBMOLDEO_API moEffectManager:public moAbstract
{
	public:
		/**
		 * constructor genrico de la clase.
		 */
		moEffectManager ();
		virtual ~ moEffectManager ();
		virtual MOboolean Init ();
		virtual MOboolean Finish ();

		///Genera un nuevo efecto a partir de la definicin
		/**
		 * Si el efecto ya existe con esos parmetros devuelve NULL.
		 * @param p_MobDefinition la definicin del objeto
		 * @see NewEffect
		 * @return el puntero al objeto o bien NULL
		 */
		moEffect * New (moMobDefinition & p_MobDefinition);

		///Genera un nuevo efecto a partir de los parmetros correspondientes
		/**
		 * Si el efecto ya existe con esos parmetros devuelve NULL.
		 * @param p_resname
		 * @param p_configname
		 * @param p_labelname
		 * @param p_type
		 * @param p_paramindex
		 * @param p_valueindex
		 * @return el puntero al objeto o bien NULL
		 * @see moEffectIndex
		 * @see moConfig
		 */
		moEffect * NewEffect (moText p_resname, moText p_configname,
			moText p_labelname, moMoldeoObjectType p_type,
			MOint p_paramindex, MOint p_valueindex);

		/**
		 * Agrega un efecto previamente creado con la funcin NewEffect
		 */
		MOboolean AddEffect (moEffect * m_pEffect);

		/**
		 * Elimina un efecto previamente agregado al moEffectManager especificando el id y el tipo
		 * @param p_ID
		 * @param p_type
		 */
		MOboolean RemoveEffect (MOint p_ID, moMoldeoObjectType p_type);
		MOint GetEffectId (moText p_labelname);
		moText GetEffectLabelName (MOint p_ID);
		moMoldeoObjectType GetType (MOint p_ID);
		moEffectsArray & AllEffects ();
		moPreEffectsArray & PreEffects ();
		moEffectsArray & Effects ();
		moPostEffectsArray & PostEffects ();
		moMasterEffectsArray & MasterEffects ();
	private:
		// EFFECTS(TODOS,PREEFFECTS,EFFECTS;POSTEFFECTS)
		moPreEffectsArray m_PreEffects;
		moEffectsArray m_Effects;
		moPostEffectsArray m_PostEffects;
		moMasterEffectsArray m_MasterEffects;
		moPrePluginsArray m_PrePlugins;
		moPluginsArray m_Plugins;
		moPostPluginsArray m_PostPlugins;
		moMasterPluginsArray m_MasterPlugins;
		moEffectsArray m_AllEffects;
};
#endif							 /*  */
