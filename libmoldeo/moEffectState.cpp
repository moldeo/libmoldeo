/*******************************************************************************

                              moEffectState.cpp

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

#include "moEffectState.h"

moEffectState::moEffectState() : moMobState() {
    Init();
}

moEffectState::~moEffectState() {
}


moEffectState::moEffectState(const moEffectState& src)  : moMobState( src ) {
    (*this) = src;
}

moEffectState&
moEffectState::operator=(const moEffectState& src) {

        m_Activated = src.m_Activated;
        m_Selected = src.m_Selected;
        on = src.on;
        synchronized = src.synchronized;
        enabled = src.enabled;
        magnitude = src.magnitude;
        amplitude = src.amplitude;
		alpha = src.alpha;

        tintr = src.tintr;
		tintg = src.tintg;
		tintb = src.tintb;

		tint = src.tint;
		tintc = src.tintc;
		tints = src.tints;

		stereo = src.stereo;
		stereoside = src.stereoside;
		fulldebug = src.fulldebug;

		tempo = src.tempo;

		return (*this);
}

MOboolean
moEffectState::Init() {

        on = MO_DEACTIVATED;
        synchronized = MO_ACTIVATED;
        enabled = MO_ACTIVATED;
        magnitude = 1.0;
        amplitude = 1.0;
		alpha = 1.0;

        tintr = 1.0;
		tintg = 1.0;
		tintb = 1.0;

		tint = 1.0;
		tintc = 0.0;
		tints = 0.0;

		stereo = MO_DEACTIVATED;
		stereoside = MO_STEREO_NONE;
		fulldebug = MO_DEACTIVATED;

		tempo.Init();

		return true;
}

MOboolean
moEffectState::Init(MOswitch D) {

        Init();
		fulldebug = D;

		return true;
}


void moEffectState::SetColorRGB( MOfloat r, MOfloat g, MOfloat b)  {
    tintr = r;
    tintr = g;
    tintr = b;
}

void moEffectState::SetColorCSV( MOfloat c, MOfloat s, MOfloat t)  {
    tintc = c;
    tints = s;
    tint = t;
    CSV2RGB();
}

void
moEffectState::CSV2RGB() {
	short C,S,V;

	C= (short)((float)tintc * (float)360);
	if (C>360) C=360;
	if (C<0) C = 0;


	S = (short)((float)tints * (float)255);
	if (S>255) S=255;
	if (S<0) S = 0;

	V = (short)((float)tint * (float)255);
	if (V>255) V=255;
	if (V<0) V = 0;

	HSVtoRGB( C, S, V, &tintr, &tintg, &tintb);

}





