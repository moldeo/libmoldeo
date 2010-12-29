/*******************************************************************************

                              moPostEffect.h

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
  Andr� Colubri

*******************************************************************************/

#ifndef __MO_POST_EFFECT_H__
#define __MO_POST_EFFECT_H__

#include "moEffect.h"

/**
 * clase base para definir Post-Efectos.
 * los efectos son los objetos basicos y define cada uno una capa que será dibujada
 * en función del orden en el que ha sido cargado en la consola de efectos
 * los post-efectos se cargan generalmente en la capa más alta, la última a ser dibujada
 * Ejemplo: el post-efecto debug imprime en pantalla los mensajes de error y status de la consola por sobre todo el resto
 * Es importante implementar las funciones de Init, Draw, Update e Interaction que son
 * necesarias para las operaciones de Inicialización, Dibujado, Actualización e Interacción.
 * @see moMoldeoObject
 * @see moEffect
 * @see moPreEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */
class LIBMOLDEO_API moPostEffect: public moEffect {
public:
    moPostEffect();
    virtual ~moPostEffect();
    virtual MOboolean Init() = 0;
    virtual void Draw(moTempo*,moEffectState * parentstate = NULL) = 0;
    virtual MOboolean Finish() = 0;
};


moDeclareExportedDynamicArray( moPostEffect*, moPostEffectsArray);

#endif
