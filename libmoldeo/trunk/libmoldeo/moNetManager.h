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

#ifndef __MO_NETMANAGER_H__
#define __MO_NETMANAGER_H__

#include "moTypes.h"
#include "moText.h"
#include "moConfig.h"
#include "moLock.h"
#include "moResourceManager.h"


/// Boost.Asio specific classes type definitions

/**
*/

typedef void* moBoostIOService;
typedef void* moBoostAddress;

typedef void* moBoostResolver;
typedef void* moBoostQuery;
typedef void* moBoostSocket;
typedef void* moBoostAcceptor;

typedef void* moBoostSystemErrorCode;

typedef void* moBoostHandleFunctionsPtr;

typedef moText moAddress;
typedef moText moPort;


enum moNetObjectType {

    MO_NET_CONNECTION,
    MO_NET_SESSION,
    MO_NET_SERVER

};

class LIBMOLDEO_API moNetObject : public moAbstract {

	public:
		moNetObject();
		virtual ~moNetObject();

};



/// clase para crear y mantener una conección basada en el protocolo tcp
/**

*/
class LIBMOLDEO_API moNetConnection : public moAbstract {

	public:
		moNetConnection( moBoostIOService pIOS );
		virtual ~moNetConnection();

		bool Start();
        bool Stop();

        moBoostSocket Socket();



        void HandleRead( long bytesbuf );

        /// Handle completion of a write operation.
        void HandleWrite();

    private:
        moBoostSocket m_Socket;

        moBoostResolver m_Resolver;

};

moDeclareExportedDynamicArray( moNetConnection*, moNetConnections );

class LIBMOLDEO_API moNetConnectionManager : public moAbstract {

  public:
    moNetConnectionManager();
    virtual ~moNetConnectionManager();

    void Start( moNetConnection* p_new_connection );
    void Stop( moNetConnection* p_connection );
    void StopAll();

    private:

        moNetConnections    m_Connections;

        moBoostHandleFunctionsPtr   m_pHandleFunctions;

};

class LIBMOLDEO_API moNetServer : public moAbstract {

    public:
        moNetServer( moAddress p_address, moPort  p_port, moText p_directory );
        virtual ~moNetServer();

        void Run();
        void Stop();


        void HandleAccept();

    private:



        void HandleStop();

        moBoostAcceptor m_pAcceptor;
        moBoostIOService m_pIOService;

        moNetConnectionManager m_ConnectionManager;

        moNetConnection*         m_pNewConnection;

        moBoostHandleFunctionsPtr   m_pHandleFunctions;


};



moDeclareExportedDynamicArray( moNetObject*, moNetObjects );

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
