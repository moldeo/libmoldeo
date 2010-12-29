/*******************************************************************************

                                moLock.h

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

  Class:
  moLock

  Description:
  Mutex Lock-Unlockin class implementation

*******************************************************************************/

#include "moTypes.h"

#ifndef __MO_LOCK_H
#define __MO_LOCK_H

/// implementación de bloqueo de ejecución, para el manejo asincrónico, primera versión basda en SDL_mutex
/**
 * Revisar.
 */

typedef  void* moBoostMutex;
typedef  void* moBoostLock;

class LIBMOLDEO_API moLock {

	public:
		moLock();
		~moLock();

		bool Lock();

		bool Unlock();

	private:
		moBoostMutex m_mutex;
		moBoostLock m_lock;
};

#endif
