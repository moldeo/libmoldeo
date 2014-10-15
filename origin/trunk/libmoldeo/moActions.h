/*******************************************************************************

                                moActions.h

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

#ifndef __MOACTIONS_H
#define __MOACTIONS_H

#include "moTypes.h"
#include "moValue.h"


#include <iostream>
#include <map>
#include <string>
using namespace std;

#define MO_ACTION_LISTENER_HOST_UNDEFINED 127.0.0.1
#define MO_ACTION_LISTENER_PORT_UNDEFINED 7777

#define MO_ACTION_MOLDEOAPI_EVENT_SEND 333
#define MO_ACTION_MOLDEOAPI_EVENT_RECEIVE 444

enum moMoldeoActionType {

  MO_ACTION_UNDEFINED = -1,

  /** MO_ACTION related to VALUES */
  MO_ACTION_VALUE_ADD = 100, /** add a value to existing param ( objectname|objectid, paramname|paramid, defaultvalue )*/
  MO_ACTION_VALUE_DELETE, /** delete a value from existing param ( objectname|objectid, paramname|paramid )*/
  MO_ACTION_VALUE_SAVE, /** save a value ( objectname|objectid, paramname|paramid, valueid, value )*/
  MO_ACTION_VALUE_SET,/** set the index of a value ( objectname|objectid, paramname|paramid, valueid, indexvalue ) */
  MO_ACTION_VALUE_GET,/** get the index of a value  ( objectname|objectid, paramname|paramid, valueid ) */
  MO_ACTION_VALUE_GETCOMPUTED,/** get the fully computed value (in case of a function>use this one to know actual value!) ( objectname|objectid, paramname|paramid, valueid ) */

  /** MO_ACTION related to PARAMETERS */
  MO_ACTION_PARAM_GET, /** ( objectname|objectid, paramname|paramid ) > Retreive full info of a parameters ( name, type and all values)*/
  MO_ACTION_PARAM_SET, /** ( objectname|objectid, paramname|paramid, paraminfo ) > if passed name/type doest affect, but can be affected by 'interpolation' and 'duration', 'position' !! */
  MO_ACTION_PARAM_SAVE, /** ( objectname|objectid, paramname|paramid) > Save actual parameter to file !? */

  MO_ACTION_PRECONFIG_ADD,/** add a preconfig set to object ( uses actual param's value position ) ( objectname|objectid, paramname|paramid, preconfigname ) */
  MO_ACTION_PRECONFIG_DELETE,/** delete a preconfig set ( objectname|objectid, paramname|paramid, preconfigname ) */
  MO_ACTION_PRECONFIG_SAVE,/** save a preconfig set ( objectname|objectid, paramname|paramid, preconfigname ) */
  MO_ACTION_PRECONFIG_SET,/** set a preconfig set ( objectname|objectid, paramname|paramid, preconfigame ) */

  MO_ACTION_OBJECT_ADD, /** EDITOR */
  /** RETREIVE FULL OBJECT INFO > state + config... */
  MO_ACTION_OBJECT_GET, /** EDITOR > if passed a string (object label string) or a number.... retreive FULL INFO*/
  MO_ACTION_OBJECT_GETSTATE,
  MO_ACTION_OBJECT_SAVE, /** EDITOR */
  MO_ACTION_OBJECT_SET, /** EDITOR */
  MO_ACTION_OBJECT_DELETE, /** EDITOR */
  MO_ACTION_OBJECT_MOVE, /** EDITOR */
  MO_ACTION_OBJECT_ENABLE, /** PLAYER */
  MO_ACTION_OBJECT_DISABLE, /** PLAYER*/
  MO_ACTION_OBJECT_TOGGLE, /** PLAYER */

  MO_ACTION_EFFECT_SET, /** EDITOR ( objectname|objectid, { OBJECT|EFFECT META INFO } )  */
  MO_ACTION_EFFECT_SETSTATE, /** PLAYER */
  MO_ACTION_EFFECT_GETSTATE, /** PLAYER */
  MO_ACTION_EFFECT_PLAY, /** PLAYER */
  MO_ACTION_EFFECT_STOP, /** PLAYER */
  MO_ACTION_EFFECT_PAUSE, /** PLAYER */

  MO_ACTION_EFFECT_ENABLE, /** PLAYER */
  MO_ACTION_EFFECT_DISABLE, /** PLAYER */
  MO_ACTION_EFFECT_TOGGLE, /** PLAYER */


  MO_ACTION_EVENT_ADD, /** PLAYER */

  MO_ACTION_CONSOLE_PLAY, /** PLAYER */
  MO_ACTION_CONSOLE_PAUSE, /** PLAYER */
  MO_ACTION_CONSOLE_STOP, /** PLAYER */

  MO_ACTION_CONSOLE_SAVE, /** SAVE*/
  MO_ACTION_CONSOLE_SAVEAS, /** SAVEAS*/
  MO_ACTION_CONSOLE_SCREENSHOT, /** SCREENSHOT */
  MO_ACTION_CONSOLE_PRESENTATION, /** PRESENTATION: fullscreen/output/etc... */
  MO_ACTION_CONSOLE_FULLSCREEN,
  MO_ACTION_CONSOLE_FULLSCREEN_SECONDARY,
  MO_ACTION_CONSOLE_WINDOWED,
  MO_ACTION_CONSOLE_RESET,
  MO_ACTION_CONSOLE_GET, /** FULL INFO: not configs */
  MO_ACTION_CONSOLE_GETSTATE, /** PLAYER */
  MO_ACTION_CONSOLE_SETSTATE, /** PLAYER */
  MO_ACTION_CONSOLE_PRESET_ADD,/** EDITOR */
  MO_ACTION_CONSOLE_PRESET_DELETE,/** EDITOR */
  MO_ACTION_CONSOLE_PRESET_SAVE,/** EDITOR */
  MO_ACTION_CONSOLE_PRESET_SET,  /** PLAYER */

  /** Subscribe specific connection to receive REACTION MESSAGES */
  MO_ACTION_LISTEN_TO_REACTIONS, /** ( hostname|ipname, portname|portnumber, actiontypename|actiontypeid, objectname|objectid, paramname|paramid ) */
  MO_ACTION_STOP_LISTENING_TO_REACTIONS /** ( hostname|ipname, portname|portnumber, objectname|objectid, paramname|paramid ) */

};


enum moMoldeoReactionType {

  MO_REACTION_UNDEFINED = -1,
  /** specified value of some object was updated */
  MO_REACTION_VALUE_UPDATED = 500, /** ( objectname|objectid, paramname|paramid, valuename|valueid, { FULL VALUE INFO } ) */
  MO_REACTION_VALUE_ERROR,

  /** specified parameter of some object was updated (a param has changed a property/add or deletion of inner values ) */
  MO_REACTION_PARAM_UPDATED, /** ( objectname|objectid, paramname|paramid, { FULL PARAM INFO }  ) */
  MO_REACTION_PARAM_ERROR,

  /** specified object was updated, (params were added ?! label of object was changed ) */
  MO_REACTION_OBJECT_UPDATED, /** ( objectname|objectid, { FULL OBJECT INFO } ) */
  MO_REACTION_OBJECT_ERROR,

  /** console has to be fully reloaded ( objects are added, moved or removed ) */
  MO_REACTION_CONSOLE_UPDATED, /** (  ) */
  MO_REACTION_CONSOLE_ERROR
};





/**
  IMPERATIVE ORDERS
  imperative orders can be sent as messages from OSC like [ /moldeo loadproject C:\test.mol ]
  or just [ /moldeo setcurrentvalue icono1 alpha 2 ]
  or [ /moldeo valuesave icono1 alpha 2 ]
*/
class moAction {

  public:
    moAction();
    moAction( moMoldeoActionType p_ActionType );
    virtual ~moAction();

    moMoldeoActionType m_ActionType;
    moDataMessage m_Arguments;
};

class moReaction {

  public:
    moReaction();
    moReaction( moMoldeoReactionType p_ReactionType );
    virtual ~moReaction();

    moMoldeoReactionType m_ReactionType;

};



/**
  This class is a connection to a reaction listener
*/
class moReactionListener {

  public:
    moReactionListener();
    moReactionListener( const moText &p_host, int p_port );
    virtual ~moReactionListener();

    moText m_hostname;
    int m_port;

};

moDeclareExportedDynamicArray( moReactionListener*, moReactionListeners );


typedef std::map< std::string, moAction > TMapStrToAction;
typedef std::map< std::string, int> TMapStrToActionType;

class moReactionListenerManager : public moAbstract {

  public:
    moReactionListenerManager();
    virtual ~moReactionListenerManager();

    /**List of connections */
    moReactionListeners m_ReactionListeners;

    TMapStrToAction      m_MapStrToAction;
    TMapStrToActionType  m_MapStrToActionType;

};

#endif

