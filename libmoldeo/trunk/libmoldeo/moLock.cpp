/*******************************************************************************

                              moLock.cpp

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

#include "moLock.h"

#ifdef MO_WIN32
  #include "boost/thread/mutex.hpp"
  #include "boost/thread/locks.hpp"
  #include "SDL_thread.h"
  using namespace boost;
#endif

#ifdef MO_LINUX
  #include "SDL/SDL_thread.h"
#endif

#ifdef MO_MACOSX
  #include "SDL/SDL_thread.h"
#endif


moLock::moLock() {

//#ifdef MO_LINUX
	m_lock = (void*) SDL_CreateMutex();
//#endif
/*
#ifdef MO_WIN32
	timed_mutex *pMutex;
	pMutex = new timed_mutex();
	m_mutex = (void*) pMutex;

	if (pMutex) {
        timed_mutex::scoped_timed_lock *pLock;
        pLock = new timed_mutex::scoped_timed_lock( *pMutex );
        m_lock = (void*) pLock;
        if (pLock) pLock->unlock();
	}
#endif
*/

}

moLock::~moLock() {
	//#ifdef MO_LINUX
    SDL_DestroyMutex((SDL_mutex*)m_lock);
 // #endif

	//#ifdef MO_WIN32

    ///todo arreglar!!! pierde memoria!
    //ATENCION!!!! CORREGIR aqui puede estar el problema de memoria q teniamos!!!!

    //mutex *pMutex = (mutex*) m_lock;
    //delete pMutex;
 // #endif
}

bool
moLock::Lock() {
/*
  #ifdef MO_WIN32
    timed_mutex *pMutex = (timed_mutex*) m_mutex;
    timed_mutex::scoped_timed_lock *pLock = (timed_mutex::scoped_timed_lock *) m_lock;

    try {
        //pLock->timed_lock();
        pLock->lock();
    }
    catch(...) {
        return false;
    }
  #endif
*/
	//#ifdef MO_LINUX
    return(SDL_mutexP( (SDL_mutex*)m_lock )!=-1);
  //#endif
	//return true;
}

bool
moLock::Unlock() {
    /*
  #ifdef MO_WIN32
    timed_mutex *pMutex = (timed_mutex*) m_mutex;
    timed_mutex::scoped_timed_lock *pLock = (timed_mutex::scoped_timed_lock *) m_lock;

    try {
        pLock->unlock();
    }
    catch(...) {
        return false;
    }
    return( true );
#endif
*/
    //#ifdef MO_LINUX
    return(SDL_mutexV( (SDL_mutex*)m_lock )!=-1);
    //#endif
}

