/*******************************************************************************

                              moSequenceEffect.h

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

//========================================
//	MOAY CLASS
//
//	MO EFFECT ESCENA
//
//	09/03/2004
//
//========================================
//Descripción:
//Esta clase fue escrita con intención
//de superponer varios efectos en uno solo
//por ejemplo un efecto icono con un efecto imagen
//
//Funcionamiento:
//Funciona como cualquier otro efecto
//pero con funcionalidades de efecto maestro
//ya que todos los efectos del array heredan
//las propiedades de este mismo que a su vez
//es controla por un canal
//
//Notas importantes:
//Este efecto maestro en caracter de efecto escena
//no utiliza(por lo tanto no inicializa)
//la variable [consolestate] ni [alleffects]
//ya que creara sus propios efectos
//========================================

#ifndef __MO_SEQUENCE_EFFECT_H__
#define __MO_SEQUENCE_EFFECT_H__

#include "moConsole.h"
#include "moPlugin.h"
#include "moMasterEffect.h"

/// Efecto Secuencia
/**
 * moSequenceEffect
 * el efecto secuencia permite generar una secuencia de distintos efectos, que será tratado como un solo efecto con su ciclo de tiempo
 * independiente *
 * @see moMoldeoObject
 * @see moSceneEffect
 * @see moEffect
 */
class LIBMOLDEO_API moSequenceEffect : public moMasterEffect
{
public:
    //config
    MOint idp_effects;//indice del parametro escena
    MOint idp_alpha;//indice del parametro alpha
    MOint idp_sequence;//indice del parametro escenas

    //Arrays
    moEffectsArray effects;
    moPluginsArray plugins;

    /// Constructor
    /**
    * Constructor
    */
    moSequenceEffect();

    /// Constructor
    /**
    * Constructor
    */
    moSequenceEffect(char*);

    /// Destructor
    /**
    * Destructor
    */
    virtual ~moSequenceEffect();

    /// Inicialización
    /**
    * Inicialización
    */
    MOboolean Init();

    /// Dibujar
    /**
    * Dibujar
    */
    void Draw(moTempo*,moEffectState* parentstate=NULL);

    /// Finalización
    /**
    * Finalización
    */
    MOboolean Finish();

    /// Interacción
    /**
    * Interacción
    */
    void Interaction(moIODeviceManager *);
    void LoadCodes(moIODeviceManager *);
};

#endif

