/*******************************************************************************

                              moTimeManager.cpp

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

#include "moTimeManager.h"


//===========================================
//
//				moTimeManager
//
//===========================================

MOuint
moTimeManager::GetTicks() {

	return moGetTicks();

}

moTimeManager::moTimeManager() {
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_TIME );
	SetName("Time Manager");
}

moTimeManager::~moTimeManager() {

}

MOboolean moTimeManager::Init() {
  m_Timers.Init( 0, NULL );
	return true;
}

MOboolean moTimeManager::Finish() {
	return true;
}

moTimer*    moTimeManager::NewTimer() {
    moTimer* newtimer = new moTimer();
    m_Timers.Add( newtimer );
    return newtimer;
}

void moTimeManager::AddTimer( moTimer* pTimer ) { ///agrega un temporizador para ser manipulado
    m_Timers.Add( pTimer );

}

void    moTimeManager::FixTimers() { /// modifica los temporizadores para adecuarse al cambio ocurrido en el temporizador absoluto...

    for( MOuint i=0; i<m_Timers.Count(); i++ ) {
        ///aaaa
        moTimer* pTimer = m_Timers[i];

        if (pTimer) {

            pTimer->Fix();

        }

    }

}

void    moTimeManager::SetDuration( MOulong p_timecode ) {

    moSetDuration( p_timecode );
    FixTimers();

}


void moTimeManager::ClearByObjectId( long p_objectid ) {

    int max = m_Timers.Count();

    for( int i=0; i<max; i++ ) {
        ///aaaa
        moTimer* pTimer = m_Timers[i];

        if ( pTimer && pTimer->GetObjectId()==p_objectid ) {

            m_Timers.Remove(i);
            max--;

        }

    }


}


void moTimeManager::ClearByTimerId( long p_timerid ) {

    int max = m_Timers.Count();

    for( int i=0; i<max; i++ ) {
        ///aaaa
        moTimer* pTimer = m_Timers[i];

        if ( pTimer && pTimer->GetTimerId()==p_timerid ) {

            m_Timers.Remove(i);
            max--;

        }

    }

}

void moTimeManager::FixByObjectId( long p_objectid ) {

    int max = m_Timers.Count();

    for( int i=0; i<max; i++ ) {
        ///aaaa
        moTimer* pTimer = m_Timers[i];

        if ( pTimer && pTimer->GetObjectId()==p_objectid ) {

            pTimer->Fix();

        }

    }

}

