/*******************************************************************************

                                moAbstract.h

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

#include "moAbstract.h"
using namespace std;

moTextHeap *moAbstract::MODebug = new moTextHeap();


moDebug::moDebug() {
    moStdOut.open ("cout.txt");
    moErr.open ("moldeoerrors.txt");
    moLog.open ("moldeolog.txt");

    backup = cout.rdbuf();     // back up cout's streambuf
    #ifdef MO_WIN32XX
    psbuf = moStdOut.rdbuf();   // get file's streambuf
    cout.rdbuf( psbuf );         // assign streambuf to cout
    FILE* fp = freopen("stdout.txt", "w", stdout);
    #endif
}

moDebug::~moDebug() {

    cout.rdbuf( backup );
    setbuf(stdout,NULL);

    moStdOut.close();
    moErr.close();
    moLog.close();
}

void
moDebug::SetStdout( const moText& filename ) {

    moText fname = filename;

    if (fname.Trim().Length()>0) {
        moStdOut.open ( fname );

        psbuf = moStdOut.rdbuf();   // get file's streambuf
        cout.rdbuf( psbuf );         // assign streambuf to cout

        FILE* fp = freopen( fname, "w", stdout);
    } else {
        cout.rdbuf( backup );         // assign streambuf to cout
    }

}

void
moDebug::Error( moText p_text ) {
  Lock();
  moText prefx_error = moText("Error! ");
  //system("Color 04");
#ifndef MO_WIN32
  cout << "\x1b[31;1m" << prefx_error << p_text << "\x1b[0m" << endl;
  moErr << prefx_error << p_text << endl;
#else
  cout << prefx_error << p_text << endl;
  moErr << prefx_error << p_text << endl;
#endif
  moLog << prefx_error << p_text << endl;
  //system("Color 02");
  m_Debug.Push( prefx_error + p_text );
  Unlock();
}

void
moDebug::Warning( moText p_text ) {
    Lock();
    moText prefx_error = moText("Warning! ");
#ifndef MO_WIN32
    cout << "\x1b[33;1m" << prefx_error << p_text << "\x1b[0m" << endl;
    moErr << prefx_error << p_text << endl;
    moLog << prefx_error << p_text << endl;
#else
    cout << prefx_error << p_text << endl;
    moErr << prefx_error << p_text << endl;
    moLog << prefx_error << p_text << endl;
#endif
    m_Debug.Push( prefx_error + p_text );
    Unlock();
}

void
moDebug::Message( moText p_text ) {
  Lock();
  cout << p_text << endl;
  moLog << p_text << endl;
  m_Debug.Push( p_text );
  Unlock();
}

/*
void
moDebug::Message( moData p_data ) {
  Lock();
  moText p_text = p_data.ToText();
  cout << p_text << endl;
  moLog << p_text << endl;
  m_Debug.Push( p_text );
  Unlock();
}

void
moDebug::Message( moDatas p_message ) {
  Lock();
  for(int i=0; i<p_message.Count(); i++) {
    moData& p_data(p_message[i]);
    Message(p_data);
  }
  Unlock();
}

void
moDebug::Message( moDataMessage p_message ) {
  Lock();
  Message( (moDatas) p_data );
  Unlock();
}

void
moDebug::Message( moDataMessages p_messages ) {
  Lock();
  for(int i=0; i<p_messages.Count(); i++) {
    moDataMessage& p_datas(p_messages[i]);
    Message(p_datas);
  }
  Unlock();
}
*/
void
moDebug::Log( moText p_text ) {
  Lock();
  moLog << p_text << endl;
  Unlock();
}

moDebug *moAbstract::MODebug2 = new moDebug();


moAbstract::moAbstract() {
	//MODebug = NULL;
	m_bInitialized = false;
}

moAbstract::~moAbstract() {
}

MOboolean
moAbstract::Init() {
	m_bInitialized = true;
	return true;
}

MOboolean
moAbstract::Finish() {
  m_bInitialized = false;
	return true;
}

MOboolean
moAbstract::Initialized() {
	return m_bInitialized;
}
