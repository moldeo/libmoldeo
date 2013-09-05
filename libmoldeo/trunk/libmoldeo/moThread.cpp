/*******************************************************************************

                              moThread.cpp

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

#include "moThread.h"

moThread::moThread() {
	m_handleThread = NULL;
}

moThread::~moThread() {
	if(ThreadExists())
		KillThread();
}

bool moThread::CreateThread() {
    //thread* pthread;
    //pthread = new thread( boost::bind( moThread::InitialThreadFunction, (void*)this ) );
    //m_handleThread = (void*) pthread;

    //m_handleThread = (void *)SDL_CreateThread( InitialThreadFunction,(void*)this);
#ifndef MO_WIN32
    pthread_create( (pthread_t*)&m_handleThread , NULL, InitialThreadFunction, this);
#else
    DWORD threadId;
    m_handleThread = (void*) ::CreateThread( 0, 0, InitialThreadFunction, this, 0, &threadId );
#endif

	return(m_handleThread!=NULL);
}

bool moThread::KillThread() {
	if(ThreadExists()) {
	    /*
		SDL_KillThread( (SDL_Thread *) m_handleThread );//SDL_WaitThread ???
		*/
#ifndef MO_WIN32
   pthread_cancel((pthread_t)m_handleThread);
#else
    CloseHandle( m_handleThread );
#endif
		//thread* pthread = (thread*)m_handleThread;
		//delete pthread;
		m_handleThread=NULL;
	}
	return(m_handleThread==NULL);
}
bool moThread::ThreadExists() {
	return(m_handleThread!=NULL);
}

bool moThread::SendThreadMessage( int message ) {
	return true;
}

#ifndef WIN32
void* moThread::InitialThreadFunction( void *data ) {
#else
DWORD WINAPI moThread::InitialThreadFunction( LPVOID data ) {
#endif
	moThread* m_pThreadClass;
	m_pThreadClass =(moThread*)data;
	int b = m_pThreadClass->ThreadUserFunction();
	#ifndef WIN32
    return (void*)0;
  #else
    return b;
  #endif
}

bool moThread::ProcessMessage() {
	return true;
}


