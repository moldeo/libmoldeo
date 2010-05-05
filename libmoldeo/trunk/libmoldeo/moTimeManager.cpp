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
  Andrés Colubri

*******************************************************************************/

#include "moTimeManager.h"

#include <moArray.h>
moDefineDynamicArray( moTimers )



void moTimerAbsolute::Start() {
    on = true;
    pause_on = false;
    start_tick = moGetTicksAbsolute();
}

long moTimerAbsolute::Duration() {
    if (on) {
        start_last = moGetTicksAbsolute();
        last_duration = duration;
        if (!pause_on) duration = start_last - start_tick;
        else start_tick = start_last - last_duration;
    } else duration = 0;
    return duration;
}

void moTimerAbsolute::SetDuration( MOulong p_timecode ) {
    start_tick = moGetTicksAbsolute()-p_timecode;
    start_last = moGetTicksAbsolute();
    duration = p_timecode;
    last_duration = duration;
}




moTimerAbsolute* moTimeManager::MoldeoTimer = new moTimerAbsolute();

void  moStartTimer() {
    moTimeManager::MoldeoTimer->Start();
}

void  moPauseTimer() {
    moTimeManager::MoldeoTimer->Pause();
}

void  moContinueTimer() {
    moTimeManager::MoldeoTimer->Continue();
}

void  moStopTimer() {
    moTimeManager::MoldeoTimer->Stop();
}

void moSetDuration( MOulong p_timecode ) {
    long test = 0;
    test = p_timecode;
    moTimeManager::MoldeoTimer->SetDuration( test );
}

MOulong moGetDuration() {
    return moTimeManager::MoldeoTimer->Duration();
}

MOulong moGetTicks() {
    return moTimeManager::MoldeoTimer->Duration();
}

void moTimer::Start() {
    on = true;
    pause_on = false;
    start_tick = moGetTicks();
}

long moTimer::Duration() {
    if (on) {
        start_last = moGetTicks();
        last_duration = duration;
        if (!pause_on) duration = start_last - start_tick;
        else start_tick = start_last - last_duration;
    } else duration = 0;
    return duration;
}

void moTimer::SetDuration( MOulong p_timecode ) {
    start_tick = moGetTicks()-p_timecode;
    start_last = moGetTicks();
    duration = p_timecode;
    last_duration = duration;
}

void moTimer::Fix() {

    start_tick = moGetTicks();

}

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

    for( int i=0; i<m_Timers.Count(); i++ ) {
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
