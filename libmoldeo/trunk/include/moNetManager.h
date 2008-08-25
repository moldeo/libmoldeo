/*******************************************************************************

                                moNetManager.h

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
#include "moTypes.h"
#include "moText.h"
#include "moConfig.h"
#include "moLock.h"
#include "moResourceManager.h"

/*
#include "tnl.h"
#include "tnlNetObject.h"
#include "tnlNetConnection.h"
*/

#ifndef __MO_NETMANAGER_H__
#define __MO_NETMANAGER_H__

/*
class moNetObject : public TNL::NetObject {

	public:
		moNetObject();
		virtual ~moNetObject();

};

class moNetConnection : public TNL::NetConnection {

	public:
		moNetConnection();
		virtual ~moNetConnection();

};
*/

class moNetObject : public moAbstract {

	public:
		moNetObject();
		virtual ~moNetObject();

};

class moNetConnection : public moAbstract {

	public:
		moNetConnection();
		virtual ~moNetConnection();

};

/*
template class LIBMOLDEO_API moDynamicArray<moNetObject*>;
typedef moDynamicArray<moNetObject*> moNetObjects;
*/
moDeclareExportedDynamicArray( moNetObject*, moNetObjects )

class LIBMOLDEO_API moNetManager : public moResource
{
	public:

		moNetManager();
		virtual ~moNetManager();

		virtual MOboolean Init();
		virtual MOboolean Finish();

	protected:
		moNetObjects	m_NetObjects;

};

#endif
