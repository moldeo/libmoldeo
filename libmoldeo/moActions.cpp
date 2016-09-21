/*******************************************************************************

                                moActions.cpp

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

#include "moActions.h"
#include <moArray.h>
moDefineDynamicArray( moReactionListeners )

/** ======================================= */

moAction::moAction() {
  m_ActionType = MO_ACTION_UNDEFINED;
}

moAction::moAction( moMoldeoActionType p_ActionType ) {
  m_ActionType = p_ActionType;
}

moAction::~moAction() {

}

/** ======================================= */

moReaction::moReaction() {
  m_ReactionType = MO_REACTION_UNDEFINED;
}

moReaction::moReaction( moMoldeoReactionType p_ReactionType ) {
  m_ReactionType = p_ReactionType;
}

moReaction::~moReaction() {

}

/** ======================================= */

moReactionListener::moReactionListener() {
  m_hostname = "MO_ACTION_LISTENER_HOST_UNDEFINED";
  m_port = MO_ACTION_LISTENER_PORT_UNDEFINED;
}

moReactionListener::moReactionListener( const moText &p_host, int p_port) {
  m_hostname = p_host;
  m_port = p_port;
}

moReactionListener::~moReactionListener() {

}



/** ======================================= */

TMapStrToActionType  moReactionListenerManager::m_MapStrToActionType;

moMoldeoActionType
moReactionListenerManager::StrToActionType( const moText& p_action_type_str ) {
  //m_MapStrToActionType
  std::string skey = (char*)p_action_type_str;
  return (moMoldeoActionType) m_MapStrToActionType[skey];
}
moReactionListenerManager::moReactionListenerManager() {
  //m_ReactionListeners

  /** values map actions*/
  m_MapStrToActionType["valueadd"] = MO_ACTION_VALUE_ADD;
  m_MapStrToActionType["valuedelete"] = MO_ACTION_VALUE_DELETE;
  m_MapStrToActionType["valuesave"] = MO_ACTION_VALUE_SAVE;
  m_MapStrToActionType["valuedelete"] = MO_ACTION_VALUE_DELETE;
  m_MapStrToActionType["valueget"] = MO_ACTION_VALUE_GET;
  m_MapStrToActionType["valueset"] = MO_ACTION_VALUE_SET;
  m_MapStrToActionType["valuerefresh"] = MO_ACTION_VALUE_REFRESH;
  m_MapStrToActionType["valuegetcomputed"] = MO_ACTION_VALUE_GETCOMPUTED;

  /** preconfigs map actions*/
  m_MapStrToActionType["preconfigadd"] = MO_ACTION_PRECONFIG_ADD;
  m_MapStrToActionType["preconfigdelete"] = MO_ACTION_PRECONFIG_DELETE;
  m_MapStrToActionType["preconfigsave"] = MO_ACTION_PRECONFIG_SAVE;
  m_MapStrToActionType["preconfigset"] = MO_ACTION_PRECONFIG_SET;

  /** parameters map actions*/
  m_MapStrToActionType["paramget"] = MO_ACTION_PARAM_GET;
  m_MapStrToActionType["paramset"] = MO_ACTION_PARAM_SET;
  m_MapStrToActionType["paramsave"] = MO_ACTION_PARAM_SAVE;


  /** object map actions*/
  m_MapStrToActionType["objectenable"] = MO_ACTION_OBJECT_ENABLE;
  m_MapStrToActionType["objectdisable"] = MO_ACTION_OBJECT_DISABLE;
  m_MapStrToActionType["objectadd"] = MO_ACTION_OBJECT_ADD;
  m_MapStrToActionType["objectduplicate"] = MO_ACTION_OBJECT_DUPLICATE;
  m_MapStrToActionType["objectimport"] = MO_ACTION_OBJECT_IMPORT;
  m_MapStrToActionType["objectget"] = MO_ACTION_OBJECT_GET;
  m_MapStrToActionType["objectgetstate"] = MO_ACTION_OBJECT_GETSTATE;
  m_MapStrToActionType["objectgetconfig"] = MO_ACTION_OBJECT_GETCONFIG;
  m_MapStrToActionType["objectgetpreconfig"] = MO_ACTION_OBJECT_GETPRECONFIG;
  m_MapStrToActionType["objectgetpreconfigs"] = MO_ACTION_OBJECT_GETPRECONFIGS;
  m_MapStrToActionType["objectsave"] = MO_ACTION_OBJECT_SAVE;
  m_MapStrToActionType["objectset"] = MO_ACTION_OBJECT_SET;
  m_MapStrToActionType["objectdelete"] = MO_ACTION_OBJECT_DELETE;
  m_MapStrToActionType["objectmove"] = MO_ACTION_OBJECT_MOVE;

  /** effect map actions */
  m_MapStrToActionType["effectsetstate"] = MO_ACTION_EFFECT_SETSTATE;
  m_MapStrToActionType["effectgetstate"] = MO_ACTION_EFFECT_GETSTATE;
  m_MapStrToActionType["effectenable"] = MO_ACTION_EFFECT_ENABLE;
  m_MapStrToActionType["effectdisable"] = MO_ACTION_EFFECT_DISABLE;
  m_MapStrToActionType["effectplay"] = MO_ACTION_EFFECT_PLAY;
  m_MapStrToActionType["effectpause"] = MO_ACTION_EFFECT_PAUSE;
  m_MapStrToActionType["effectstop"] = MO_ACTION_EFFECT_STOP;

  /** console map actions  */
  m_MapStrToActionType["consoleplay"] = MO_ACTION_CONSOLE_PLAY;
  m_MapStrToActionType["consoleplaysession"] = MO_ACTION_CONSOLE_PLAY_SESSION;
  m_MapStrToActionType["consolerecordsession"] = MO_ACTION_CONSOLE_RECORD_SESSION;
  m_MapStrToActionType["consolerendersession"] = MO_ACTION_CONSOLE_RENDER_SESSION;
  m_MapStrToActionType["consolesavesessionas"] = MO_ACTION_CONSOLE_SAVE_SESSION_AS;
  m_MapStrToActionType["consolepause"] = MO_ACTION_CONSOLE_PAUSE;
  m_MapStrToActionType["consolestop"] = MO_ACTION_CONSOLE_STOP;
  m_MapStrToActionType["consolesave"] = MO_ACTION_CONSOLE_SAVE;
  m_MapStrToActionType["consolesaveas"] = MO_ACTION_CONSOLE_SAVEAS;
  m_MapStrToActionType["consolescreenshot"] = MO_ACTION_CONSOLE_SCREENSHOT;
  m_MapStrToActionType["consolepreviewshot"] = MO_ACTION_CONSOLE_PREVIEW_SHOT;
  m_MapStrToActionType["consolepresentation"] = MO_ACTION_CONSOLE_PRESENTATION;
  m_MapStrToActionType["consolefullscreen"] = MO_ACTION_CONSOLE_FULLSCREEN;
  m_MapStrToActionType["consolefullscreensecondary"] = MO_ACTION_CONSOLE_FULLSCREEN_SECONDARY;
  m_MapStrToActionType["consolewindowed"] = MO_ACTION_CONSOLE_WINDOWED;
  m_MapStrToActionType["consoleget"] = MO_ACTION_CONSOLE_GET;
  m_MapStrToActionType["consolegetstate"] = MO_ACTION_CONSOLE_GETSTATE;
  m_MapStrToActionType["consolesetstate"] = MO_ACTION_CONSOLE_SETSTATE;
  /** console presets map actions*/
  m_MapStrToActionType["consolepresetadd"] = MO_ACTION_CONSOLE_PRESET_ADD;
  m_MapStrToActionType["consolepresetdelete"] = MO_ACTION_CONSOLE_PRESET_DELETE;
  m_MapStrToActionType["consolepresetsave"] = MO_ACTION_CONSOLE_PRESET_SAVE;
  m_MapStrToActionType["consolepresetset"] = MO_ACTION_CONSOLE_PRESET_SET;

}

moReactionListenerManager::~moReactionListenerManager() {

}
