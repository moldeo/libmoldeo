/*******************************************************************************

                              moMasterEffect.h

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

#ifndef __MO_MASTER_EFFECT_H__
#define __MO_MASTER_EFFECT_H__

#include "moEffect.h"
#include "moConsoleState.h"

class moEffectManager;

/**
 * clase base para definir Efectos Maestro.
 * los efectos maestro se diferencian de los pre-efectos, post-efectos y efectos en que
 * tienen acceso a modificar y crear efectos
 * Los efectos maestro predeterminados son por ejemplo: el moMasterEffectChannel, que puede modificar aquellos efectos que tiene seleccionado alterando sus parámetros de dibujado y sincronización.
 * El efecto maestro moMasterEffectPanel, enciende y apaga los efectos cargados en moConsole
 * @see moMoldeoObject
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */
class LIBMOLDEO_API moMasterEffect : public moEffect
{
public:

        /**
         * constructor genérico de la clase.
         */
        moMasterEffect();

        /**
         * destructor genérico de la clase.
         */
        virtual ~moMasterEffect();

		/**
		* Pre-Inicializador genérico derivado de moEffect.
		*/
		virtual MOboolean PreInit();

		/**
		* Inicializador genérico derivado de moAbstract.
		* @see moAbstract
		*/
        virtual MOboolean Init() = 0;

        /**
        * Función de asignación del puntero al Administrador de efectos a controlar
        * @see moEffectManager
        */
        virtual void Set( moEffectManager *pEffectManager, moConsoleState* cstate);

        /**
        * Función de dibujado derivada de moEffect
        * @see moEffect
        */
        virtual void Draw(moTempo*, moEffectState* parentstate = NULL) = 0;
    virtual void Interaction(moIODeviceManager*) = 0;
    virtual MOboolean Finish() = 0;

    protected:

        /**
        * Puntero al Administrador de efectos a controlar
        */
        moEffectManager*	m_pEffectManager;//cada effect maestro tiene acceso a los otros effects maestros

        /**
        * Puntero al estado general de la consola
        */
        moConsoleState *consolestate;    //el state de la consola

};
/*
template class LIBMOLDEO_API moDynamicArray<moMasterEffect*>;
typedef moDynamicArray<moMasterEffect*> moMasterEffectsArray;
*/
/**
 * moMasterEffectsArray: clase arreglo de punteros de objetos moMasterEffect
 */
moDeclareExportedDynamicArray( moMasterEffect*, moMasterEffectsArray);


#include "moEffectManager.h"

#endif

