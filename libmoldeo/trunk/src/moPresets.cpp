/*******************************************************************************

                              moPresets.cpp

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

#include "moPresets.h"


moPresets::moPresets()
{
    preset = NULL;
	state = NULL;
	setting = NULL;
    presetnum = 0;
    paramnum = 0;
}

moPresets::~moPresets()
{
	Finish();
}

moPresets::moPresets( MOint cpre, MOint cpara) {
    presetnum = cpre;
    paramnum = cpara;

    preset = new MOint*[presetnum];
    state = new moEffectState[presetnum];
    setting = new MOboolean[presetnum];
    for( int i=0; i<presetnum; i++)
    {
        preset[i] = new MOint[paramnum];
        state[i].Init();
        setting[i] = MO_FALSE;
    }
}

MOboolean
moPresets::Init( MOint cpre, MOint cpara) {
    presetnum = cpre;
    paramnum = cpara;

    preset = new MOint*[presetnum];
    state = new moEffectState[presetnum];
    setting = new MOboolean[presetnum];
    for( int i=0; i<presetnum; i++)
    {
        preset[i] = new MOint[paramnum];
        state[i].Init();
        setting[i] = MO_FALSE;
    }
	return true;
}

MOboolean moPresets::Finish()
{
    if(preset!=NULL)
    {
        for( int i=0; i<presetnum; i++)
            if(preset[i]!=NULL)
				delete[] preset[i];
        delete[] preset;
    }

	if(state!=NULL)
			delete[] state;

	if(setting!=NULL)
			delete[] setting;

	return true;

}

void moPresets::Save( MOint p, moConfig *conf, moEffectState *est) {
	MOint i;
    for( i=0; i<paramnum; i++)
        preset[p][i] = conf->GetCurrentValueIndex( i);
    state[p] = *est;
    setting[p] = MO_TRUE;
}


void moPresets::Load( MOint p, moConfig *conf, moEffectState *est) {
	MOint i;
    if(setting[p])
    {
        for( i=0; i<paramnum; i++)
            conf->SetCurrentValueIndex( i, preset[p][i]);
        *est = state[p];
    }
}


