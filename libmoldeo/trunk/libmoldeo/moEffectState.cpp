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
  Andrés Colubri

*******************************************************************************/

#include "moEffectState.h"

moEffectState::moEffectState() {
        on = MO_DEACTIVATED;
        synchronized = MO_ACTIVATED;
        enabled = MO_ACTIVATED;
        magnitude = 1.0;
        amplitude = 1.0;
		alpha = 1.0;
		tint = 1.0;
		tintr = 1.0;
		tintg = 1.0;
		tintb = 1.0;
		tintc = 0.0;
		tints = 0.0;
		fulldebug = MO_DEACTIVATED;
}

moEffectState::~moEffectState() {
}

MOboolean
moEffectState::Init() {
        on = MO_DEACTIVATED;
        synchronized = MO_ACTIVATED;
        enabled = MO_ACTIVATED;
        magnitude = 1.0;
        amplitude = 1.0;
		alpha = 1.0;
		tint = 1.0;
		tintr = 1.0;
		tintg = 1.0;
		tintb = 1.0;
		tintc = 0.0;
		tints = 0.0;
		//fulldebug = MO_DEACTIVATED;

		return true;
}

MOboolean
moEffectState::Init(MOswitch D) {
        on = MO_DEACTIVATED;
        synchronized = MO_ACTIVATED;
        enabled = MO_ACTIVATED;
        magnitude = 1.0;
        amplitude = 1.0;
		alpha = 1.0;
		tint = 1.0;
		tintr = 1.0;
		tintg = 1.0;
		tintb = 1.0;
		tintc = 0.0;
		tints = 0.0;
		fulldebug = D;

		return true;
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
