/*******************************************************************************

                              moNetManager.cpp

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


******************************************************************************/

#include "moNetManager.h"

#include "moArray.h"
moDefineDynamicArray(moNetObjects)
moDefineDynamicArray( moNetConnections )

#include <boost/bind.hpp>

#ifndef MO_MACOSX
#include <boost/asio.hpp>
#else
#include <boost/asio.hpp>
#endif

#include <boost/shared_ptr.hpp>

#ifdef MO_WIN32
  using namespace boost;
  using namespace boost::asio;
  using namespace boost::asio::ip;
#endif

#ifdef MO_MACOSX
  using namespace boost;
  using namespace asio;
  using namespace asio::ip;
#endif

#ifdef MO_LINUX
  using namespace boost;
  using namespace asio;
  using namespace asio::ip;
#endif


class moBoostHandleFunctions {

    public:

        moBoostHandleFunctions( void* pObject,  moNetObjectType p_type ) {
            m_pObject = pObject;
            m_type = p_type;
        }
        virtual ~moBoostHandleFunctions() {

        }

    public:

#ifdef MO_WIN32
        void HandleRead( const system::error_code& e,
                        std::size_t bytes_transferred ) {

                moNetConnection* pConnection;

                if (!e) {
                    switch(m_type) {
                         case MO_NET_CONNECTION:
                            pConnection = (moNetConnection*) m_pObject;
                            pConnection->HandleRead( (long)bytes_transferred );
                            break;
                        case MO_NET_SESSION:
                            //m_pObject->HandleAccept();
                            break;
                        case MO_NET_SERVER:
                            //moNetServer* pServer = (moNetServer*) m_pObject;

                            //pServer->HandleRead();
                            break;
                    }
                }
        }

        /// Handle completion of a write operation.
        void HandleWrite(const boost::system::error_code& e) {

                moNetConnection* pConnection;

                if (!e) {
                    switch(m_type) {
                         case MO_NET_CONNECTION:
                            pConnection = (moNetConnection*) m_pObject;
                            pConnection->HandleWrite();
                            break;
                        case MO_NET_SESSION:
                            //m_pObject->HandleAccept();
                            break;
                        case MO_NET_SERVER:
                            //moNetServer* pServer = (moNetServer*) m_pObject;

                            //pServer->HandleRead();
                            break;
                    }
                }
        }


        void HandleAccept( const boost::system::error_code& e ) {
                if (!e) {
                    switch(m_type) {
                         case MO_NET_CONNECTION:
                            //m_pObject->HandleAccept();
                            break;
                        case MO_NET_SESSION:
                            //m_pObject->HandleAccept();
                            break;
                        case MO_NET_SERVER:
                            moNetServer* pServer = (moNetServer*) m_pObject;

                            pServer->HandleAccept();
                            break;
                    }
                }
        }
#endif
    private:

        moNetObjectType m_type;
        void*           m_pObject;

};


//===========================================
//
//				moNetManager
//
//===========================================

moNetObject::moNetObject() {

}

moNetObject::~moNetObject() {

}

//===========================================
//
//				moNetConnection
//
//===========================================

moNetConnection::moNetConnection( moBoostIOService pIOS ) {

   m_Socket = NULL;
   m_Resolver = NULL;

   asio::io_service *p_io_service = (asio::io_service *) pIOS;

   tcp::socket* p_socket = new tcp::socket( (*p_io_service)  );

   if (p_socket)
        m_Socket = (void*)p_socket;

    tcp::resolver *p_resolver = new tcp::resolver( (*p_io_service) );

    if (p_resolver)
        m_Resolver = (void*)p_resolver;

/*
   tcp::resolver::query query( , "http");
   tcp::resolver::iterator endpoint_iterator = m_Resolver->resolve( query );
   tcp::resolver::iterator end;



   tcp::no_delay option( true );
   p_socket->set_option( option );
   p_socket->connect(*endpoint_iterator++, error);

*/
}

moNetConnection::~moNetConnection() {

}

bool moNetConnection::Start() {

    if (m_Socket) {
        //tcp::socket* p_socket = (tcp::socket*) m_Socket;

        /*p_socket->async_read_some( boost::asio::buffer(buffer_),
            boost::bind(&connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
            */
    }

    // Try each endpoint until we successfully establish a connection.
/*
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);
*/
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    /*
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << argv[2] << " HTTP/1.0\r\n";
    request_stream << "Host: " << argv[1] << "\r\n";
    */
    //request_stream << "Accept: */*\r\n";
    //request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    //boost::asio::write(socket, request);

    return false;
}



bool moNetConnection::Stop() {

    if (m_Socket) {

        tcp::socket* p_socket = (tcp::socket*) m_Socket;

        p_socket->close();

    }

    return true;
}

moBoostSocket moNetConnection::Socket() {

        return m_Socket;

}


void
moNetConnection::HandleRead( long bytesbuf ) {

    bytesbuf = 0 + bytesbuf;
/*
    boost::tribool result;

    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result)
    {
      request_handler_.handle_request(request_, reply_);

      boost::asio::async_write(socket_, reply_.to_buffers(),
                                boost::bind(&connection::handle_write, shared_from_this(),
                                            boost::asio::placeholders::error));
    }
    else if (!result)
    {
      reply_ = reply::stock_reply(reply::bad_request);

      boost::asio::async_write( socket_, reply_.to_buffers(),
                                boost::bind(&connection::handle_write, shared_from_this(),
                                            boost::asio::placeholders::error));
    }
    else
    {
      socket_.async_read_some(  boost::asio::buffer(buffer_),
                                boost::bind(&connection::handle_read, shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }
*/
}

void
moNetConnection::HandleWrite() {
    // Initiate graceful connection closure.
    /*
    boost::system::error_code ignored_ec;

    if (m_Socket) {

        tcp::socket* p_socket = (tcp::socket*) m_Socket;

        p_socket->shutdown( boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

    }
    */

}

//===========================================
//
//				moNetConnectionManager
//
//===========================================


moNetConnectionManager::moNetConnectionManager() {


}

moNetConnectionManager::~moNetConnectionManager() {

}


void moNetConnectionManager::Start( moNetConnection* p_new_connection ) {
    m_Connections.Add( p_new_connection );
    p_new_connection->Start();
}

void moNetConnectionManager::Stop( moNetConnection* p_connection ) {
    for( int i=0; i<(int)m_Connections.Count(); i++) {
        if (m_Connections[i]==p_connection) {
            m_Connections.Remove(i);
            p_connection->Stop();
            i--;
        }

    }
}

void moNetConnectionManager::StopAll() {
    for( int i=0; i<(int)m_Connections.Count(); i++) {
        if (m_Connections[i]) {
            m_Connections[i]->Stop();
        }

    }
    m_Connections.Empty();
}

//===========================================
//
//				moNetServer
//
//===========================================

void moNetServer::HandleAccept( )
{

    m_ConnectionManager.Start( m_pNewConnection );

    m_pNewConnection = new moNetConnection( m_pIOService );

    tcp::acceptor *p_acceptor = (tcp::acceptor *) m_pAcceptor;
    if (p_acceptor) {

        //tcp::socket* p_socket = (tcp::socket*) m_pNewConnection->Socket();

        //moBoostHandleFunctions* pFunctions = (moBoostHandleFunctions*) m_pHandleFunctions;

        /*
        p_acceptor->async_accept( *p_socket,
            boost::bind( &moBoostHandleFunctions::HandleAccept, pFunctions,
                        placeholders::error));
                        */
    }

}

/*

void HandleAccept( const boost::system::error_code& error )
{
    if (!error) {
        ///moNetServer::HandleAccept();
    }
}


*/
moNetServer::moNetServer( moAddress p_address, moPort  p_port, moText p_directory ) {

    m_pIOService = NULL;

    if ( p_directory==moText("") ) p_directory = "";

    io_service* p_io_service = new io_service();
    m_pIOService = (void*) p_io_service;

    if (m_pIOService) {

        m_pNewConnection = new moNetConnection( m_pIOService );


        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        tcp::resolver resolver( *p_io_service );

        tcp::resolver::query query( (char*)p_address, (char*)p_port);

        tcp::endpoint endpoint = *resolver.resolve(query);

        tcp::acceptor *p_acceptor = new tcp::acceptor( *p_io_service );
        m_pAcceptor = (void*) p_acceptor;

        if (m_pAcceptor) {
            p_acceptor->open( endpoint.protocol() );
            p_acceptor->set_option( tcp::acceptor::reuse_address(true) );
            p_acceptor->bind( endpoint );
            p_acceptor->listen();

            ///tcp::socket* p_socket = (tcp::socket*) m_pNewConnection->Socket();

            /*
            p_acceptor->async_accept(   *p_socket,
                                        boost::bind( &moNetServer::HandleAccept, this,
                                                    boost::asio::placeholders::error));

            */
            moBoostHandleFunctions* pFunctions = new moBoostHandleFunctions( (void*) this, MO_NET_SERVER );
            m_pHandleFunctions = (void*) pFunctions;
            /*
            p_acceptor->async_accept(   *p_socket,
                                        boost::bind( &moBoostHandleFunctions::HandleAccept, pFunctions,
                                         placeholders::error ));
                                         */


        }
    }
}


moNetServer::~moNetServer() {

}


void
moNetServer::Run() {
    io_service* p_io_service = NULL;
    if (m_pIOService) {
        p_io_service = (io_service*) m_pIOService;


        p_io_service->run();
    }
}

void
moNetServer::Stop() {
    io_service* p_io_service = NULL;
    if (m_pIOService) {
        p_io_service = (io_service*) m_pIOService;

        p_io_service->post( boost::bind( &moNetServer::HandleStop, this) );
    }

}




void
moNetServer::HandleStop() {

  // The server is stopped by cancelling all outstanding asynchronous
  // operations. Once all operations have finished the io_service::run() call
  // will exit.
  tcp::acceptor* p_acceptor = NULL;
  p_acceptor = (tcp::acceptor*) m_pAcceptor;
  if (p_acceptor) {
    p_acceptor->close();
  }
  m_ConnectionManager.StopAll();

}


//===========================================
//
//				moNetManager
//
//===========================================

moNetManager::moNetManager() {
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_NET );

	SetName("netmanager");
	SetLabelName("netmanager");
}

moNetManager::~moNetManager() {

}

MOboolean moNetManager::Init() {
	m_NetObjects.Init(0,NULL);



	return true;
}


MOboolean moNetManager::Finish() {
	m_NetObjects.Finish();
	return true;
}


