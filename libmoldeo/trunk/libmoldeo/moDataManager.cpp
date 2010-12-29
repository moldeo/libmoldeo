/*******************************************************************************

                              moDataManager.cpp

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

#include "moDataManager.h"
#include <moArray.h>
#include <moVideoGraph.h>
#include <moGsGraph.h>
#include <moResourceManager.h>
#include <moRenderManager.h>

#include "moArray.cpp"
moDefineDynamicArray(moDataSessionKeys)
moDefineDynamicArray(moDataSessionEventKeys)

//===========================================
//
//				moDataSession
//
//===========================================

moDataSession::moDataSession() {
    m_pVideoGraph = NULL;
    m_pDataSessionConfig = NULL;
}

moDataSession::~moDataSession() {
    if (m_pVideoGraph) {
        delete m_pVideoGraph;
        m_pVideoGraph = NULL;
    }
}

void
moDataSession::Set(   moText p_Name,
            moDataSessionConfig* pSessionConfig,
            moDataSessionMode p_sessionmode,
            moDataSessionRecordMode p_recordmode,
            moDataSessionPlaybackMode p_playbackmode ) {
    m_Name = p_Name;
    m_pDataSessionConfig = pSessionConfig;
    m_SessionMode = p_sessionmode;
    m_SessionPlaybackMode = p_playbackmode;
    m_SessionRecordMode = p_recordmode;
    m_Keys.Init( pSessionConfig->GetMaxKeys(), NULL );
    if (m_pVideoGraph) {
        delete m_pVideoGraph;
        m_pVideoGraph = (moVideoGraph*) new moGsGraph();
        m_pVideoGraph->InitGraph();
    } else {
        m_pVideoGraph = (moVideoGraph*) new moGsGraph();
        m_pVideoGraph->InitGraph();
    }
}


bool
moDataSession::SaveToFile( moText p_filename ) {


}

bool
moDataSession::LoadFromFile( moText p_filename ) {

}

bool
moDataSession::AddKey( moDataSessionKey* p_key ) {

    m_Keys.Add( p_key );

}

bool
moDataSession::AddEventKey( moDataSessionEventKey* p_eventkey ) {
    m_EventKeys.Add( p_eventkey );
}

bool
moDataSession::Playback() {
    m_SessionPlaybackMode = MO_DATASESSION_PLAY_LIVETOCONSOLE;
}

bool
moDataSession::Record() {

    m_SessionRecordMode = MO_DATASESSION_RECORD_DIRECTTOFILE;

}

bool
moDataSession::RecordLive( moResourceManager* pRM ) {

    m_SessionPlaybackMode = MO_DATASESSION_PLAY_LIVETOVIDEO;
    if (m_pVideoGraph && m_pDataSessionConfig) {
        m_pVideoGraph->BuildRecordGraph( m_pDataSessionConfig->GetVideoFileName(), pRM->GetRenderMan()->GetFramesPool() );
    }
}

//===========================================
//
//				moDataSessionConfig
//
//===========================================

moDataSessionConfig::moDataSessionConfig() {
	m_DataPath = moText("");
	m_ConsoleConfigName = moText("");
}

moDataSessionConfig::moDataSessionConfig(   moText p_apppath,
                                            moText p_datapath,
                                            moText p_consoleconfig,
                                            moText p_SessionFileName,
                                            moText p_VideoFileName,
                                            long p_MaxKeys,
                                            long p_MaxTimecode,
                                            long p_Port,
                                            long p_Address ) {
    m_AppPath = p_apppath;
	m_DataPath = p_datapath;
	m_ConsoleConfigName = p_consoleconfig;
	m_SessionFileName = p_SessionFileName;
	m_VideoFileName = p_VideoFileName;
	m_MaxKeys = p_MaxKeys;
	m_MaxTimecode = p_MaxTimecode;
}

moDataSessionConfig::~moDataSessionConfig() {
}

moText
moDataSessionConfig::GetAppPath() {
	return m_AppPath;
}

moText
moDataSessionConfig::GetDataPath() {
	return m_DataPath;
}

moText
moDataSessionConfig::GetConsoleConfigName() {
	return m_ConsoleConfigName;
}

moText
moDataSessionConfig::GetVideoFileName() {
	return m_VideoFileName;
}

moText
moDataSessionConfig::GetSessionFileName() {
	return m_SessionFileName;
}


//===========================================
//
//				moDataManager
//
//===========================================


moDataManager::moDataManager() {
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_DATA );

	SetName("Data Manager");

	m_pDataSession = NULL;
    m_pDataSessionConfig = NULL;
}

moDataManager::~moDataManager() {

}

MOboolean moDataManager::Init() {
	if (!m_pDataSessionConfig) m_pDataSessionConfig = new moDataSessionConfig( moText(""), moText(DATADIR), moText(DATADIR "/console.mol") );
    if (!m_pDataSession) {
        m_pDataSession = new moDataSession();
        if (m_pDataSession)
            m_pDataSession->Set( moText("session 1"),
                                 m_pDataSessionConfig,
                                 MO_DATASESSION_INACTIVE,
                                 MO_DATASESSION_RECORD_BUFFERINGTOFILE,
                                 MO_DATASESSION_PLAY_LIVETOCONSOLE );
    }
	return true;
}

MOboolean moDataManager::Init( moText p_apppath, moText p_datapath, moText p_consoleconfig ) {

	if (!m_pDataSessionConfig) m_pDataSessionConfig = new moDataSessionConfig( p_apppath, p_datapath, p_consoleconfig );
	if (!m_pDataSession) {
        m_pDataSession = new moDataSession();
            m_pDataSession->Set( moText("session 1"),
                                 m_pDataSessionConfig,
                                 MO_DATASESSION_INACTIVE,
                                 MO_DATASESSION_RECORD_BUFFERINGTOFILE,
                                 MO_DATASESSION_PLAY_LIVETOCONSOLE );
	}
	return true;
}

moText
moDataManager::GetAppPath() {
	//m_DataSessionIndex
	if (m_pDataSessionConfig)
		return m_pDataSessionConfig->GetAppPath();
	return moText("");
}

moText
moDataManager::GetDataPath() {
	//m_DataSessionIndex
	if (m_pDataSessionConfig)
		return m_pDataSessionConfig->GetDataPath();
	return moText("");
}


moText
moDataManager::GetConsoleConfigName() {
	//m_DataSessionIndex
	if (m_pDataSessionConfig)
		return m_pDataSessionConfig->GetConsoleConfigName();
	return moText("");
}


MOboolean moDataManager::Finish() {
	return true;
}

void moDataManager::StartRecordingSession(  ) {

    m_pDataSession->Record();

}

void moDataManager::StartPlayinbackSession(  ) {
    m_pDataSession->Playback();
}

moDataSession*  moDataManager::GetSession() {
        return m_pDataSession;
}

