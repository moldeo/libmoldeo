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

#define SDLTHREADS 1

#ifdef MO_WIN32
  //we use plain WINAPI code: CreateMutex, ReleaseMutex, WaitForSingleObject, ...
#endif

#ifdef MO_LINUX
	#ifdef SDLTHREADS
		#include "SDL/SDL_thread.h"
	#endif

  #include "pthread.h"
#endif

#ifdef MO_MACOSX
  //#include "SDL/SDL_thread.h"
#endif


moLock::moLock() {

	m_lock = NULL;

#ifndef MO_WIN32
    #ifdef SDLTHREADS
			m_lock = (void*) SDL_CreateMutex();
		#else
			pthread_mutexattr_t attr;

			pthread_mutex_init( (pthread_mutex_t*)&m_lock, &attr);

			pthread_mutexattr_init(&attr);
			pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
			pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
			if ( pthread_mutex_init((pthread_mutex_t*)&m_lock, &attr) != 0 ) {
				cout << "couldn create mutex and init with PTHREAD_MUTEX_RECURSIVE" << endl;
				exit(1);
			}
		#endif
#else
    #ifdef UNICODE
        m_lock = (void*)CreateMutex( NULL, FALSE, L"lockMutex" );
    #else
        m_lock = (void*)CreateMutex( NULL, FALSE, "lockMutex" );
    #endif
#endif

}

moLock::~moLock() {

#ifndef WIN32
		#ifdef SDLTHREADS
			SDL_DestroyMutex((SDL_mutex*)m_lock);
		#else
			if (m_lock!=NULL) {
				pthread_mutex_destroy( (pthread_mutex_t*)&m_lock );
			}
		#endif
#else
	CloseHandle(m_lock);
#endif

	m_lock = NULL;
}

bool
moLock::Lock() {

	if (m_lock==NULL) {
		return false;
	}

#ifndef WIN32
		#ifdef SDLTHREADS
			return(SDL_mutexP( (SDL_mutex*)m_lock )!=-1);
		#else
			if (pthread_mutex_lock( (pthread_mutex_t*)&m_lock ) < 0 ) {
				return false;
			}
		#endif
#else
    WaitForSingleObject( m_lock, INFINITE);
#endif

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
  return true;
}

bool
moLock::Unlock() {

	if (m_lock==NULL) {
		return false;
	}

#ifndef WIN32
	#ifdef SDLTHREADS
		return(SDL_mutexV( (SDL_mutex*)m_lock )!=-1);
	#else
		if (pthread_mutex_unlock( (pthread_mutex_t*)&m_lock )<0) {
			return false;
		}
	#endif
#else
	return ReleaseMutex( m_lock );
#endif

	return true;
}

