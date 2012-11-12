/*******************************************************************************

                              moPreEffect.h

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

// Definicion:                                                                              //
// Este filtro se aplica ANTES de dibujada la nueva imagen(al principio de los efectos)    //

#ifndef __MO_PRE_EFFECT_H__
#define __MO_PRE_EFFECT_H__

#include "moEffect.h"

/// clase base para definir Pre-Efectos.
/**
 * clase base para definir Pre-Efectos.
 * los efectos son los objetos basicos y define cada uno una capa que será dibujada
 * en función del orden en el que ha sido cargado en la consola de efectos
 * los pre-efectos se cargan generalmente en la capa más baja, la primera a ser dibujada
 * un pre-efecto es por ejemplo el del borrado de la pantalla, así como aquellos efectos de procesamiento
 * sobre el fotograma anterior al actual
 * Es importante implementar las funciones de Init, Draw, Update e Interaction que son
 * necesarias para las operaciones de Inicialización, Dibujado, Actualización e Interacción.
 * @see moMoldeoObject
 * @see moEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */

class LIBMOLDEO_API moPreEffect: public moEffect
{
public:
    GLuint ScreenAll;
    moPreEffect();
    virtual ~moPreEffect();
    virtual MOboolean Init() = 0;
    virtual void Draw(moTempo*, moEffectState* = NULL) = 0;
    virtual MOboolean Finish() = 0;
};

moDeclareExportedDynamicArray( moPreEffect*, moPreEffectsArray);

#endif

