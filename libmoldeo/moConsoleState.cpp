/*******************************************************************************

                              moConsoleState.cpp

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

#include "moConsoleState.h"

moConsoleState::moConsoleState() {

	automatic = MO_DEACTIVATED;
	pause = MO_DEACTIVATED;
	finish = MO_FALSE;
	reinit = MO_FALSE;
	setfps = MO_DEACTIVATED;

	fps = 30;
	fps0 = 0;
	fps1 = 1;

  stereooutput = MO_DEACTIVATED;
	savescreen = MO_DEACTIVATED;
	frame = 0;

  m_Mode = MO_CONSOLE_MODE_LIVE;
  m_RenderFrameQuality = "JPGGOOD";
}

moConsoleState::~moConsoleState() {
	Finish();
}

MOboolean
moConsoleState::Init() {

	automatic = MO_DEACTIVATED;
	pause = MO_DEACTIVATED;
	finish = MO_FALSE;
	reinit = MO_FALSE;
	setfps = MO_DEACTIVATED;

	fps = 30;
	fps0 = 0;
	fps1 = 1;

  m_Mode = MO_CONSOLE_MODE_LIVE;

	return true;
}

MOboolean
moConsoleState::Finish() {

	automatic = MO_DEACTIVATED;
	pause = MO_DEACTIVATED;
	finish = MO_FALSE;
	reinit = MO_FALSE;
	setfps = MO_DEACTIVATED;

	fps = 30;
	fps0 = 0;
	fps1 = 1;
/*
	m_nEffects = 0;
	m_nPreEffects = 0;
	m_nPostEffects = 0;
	m_nMasterEffects = 0;
	m_nAllEffects = 0;
*/
	return true;
}

const moText&
moConsoleState::ToJSON() {

  moText fieldSeparation = ",";
  moEffectState::ToJSON();
  moText tmode = "";
  if (m_Mode==MO_CONSOLE_MODE_LIVE) tmode = "live";
  if (m_Mode==MO_CONSOLE_MODE_PLAY_SESSION) tmode = "playsession";
  if (m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) tmode = "recordsession";
  if (m_Mode==MO_CONSOLE_MODE_RENDER_SESSION) tmode = "rendersession";

  moText json = "{";
  json+= "'mode': '"+tmode+"'";
  json+= fieldSeparation + "'effectstate':"+fullJSON;
  json+= "}";

  fullJSON = json;
  return fullJSON;
}




