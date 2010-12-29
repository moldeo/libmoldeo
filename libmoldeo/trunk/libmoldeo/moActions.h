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

enum moMoldeoActionType {

  MO_ACTION_VALUE_ADD,/* EDITOR */
  MO_ACTION_VALUE_DELETE,/* EDITOR */
  MO_ACTION_VALUE_SAVE,/* EDITOR */

  MO_ACTION_VALUE_SET,/* PLAYER */

  MO_ACTION_PRECONFIG_ADD,/* EDITOR */
  MO_ACTION_PRECONFIG_DELETE,/* EDITOR */
  MO_ACTION_PRECONFIG_SAVE,/* EDITOR */

  MO_ACTION_PRECONFIG_SET,/* PLAYER */

  MO_ACTION_PRESET_ADD,/* EDITOR */
  MO_ACTION_PRESET_DELETE,/* EDITOR */
  MO_ACTION_PRESET_SAVE,/* EDITOR */

  MO_ACTION_PRESET_SET,  /* PLAYER */

  MO_ACTION_EVENT_ADD

};

enum moMoldeoPlayerActionType {

    MO_ACTION_PLAY_VALUE_SET,

    MO_ACTION_PLAY_PRECONFIG_SET,

    MO_ACTION_PLAY_PRESET_SAVE,
    MO_ACTION_PLAY_PRESET_SET,

    MO_ACTION_PLAY_EVENT_SET


};


enum moMoldeo {




};

#endif
