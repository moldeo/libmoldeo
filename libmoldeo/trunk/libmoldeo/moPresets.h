/*******************************************************************************

                              moPresets.h

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

#ifndef __MO_PRESETS_H__
#define __MO_PRESETS_H__

#include "moTypes.h"
#include "moConfig.h"
#include "moEffectState.h"

class LIBMOLDEO_API moPresets : public moAbstract
{
public:
    MOint presetnum;
    MOint paramnum;
//void SetCantPreset(MOint);
//void SetCantParams(MOint);
    MOint** preset;//presetnum*paramnum
    moEffectState* state;//*presetnum
    MOboolean* setting;

    moPresets();
    moPresets(MOint,MOint);
    virtual ~moPresets();

    MOboolean Init( MOint, MOint);//(0) cuantos presets voy a guardar,(1) el archivo de configuracion para saber cuantos parametros tengo que guardar
    MOboolean Finish();
    const moEffectState& Load( MOint p, moConfig* conf, const moEffectState& fxstate );
    void Save(MOint p, moConfig* conf, const moEffectState& fxstate);
};

#endif
