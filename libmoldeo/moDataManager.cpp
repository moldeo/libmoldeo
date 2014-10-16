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


*******************************************************************************/

#include "moDataManager.h"
#include <moArray.h>
#include <moVideoGraph.h>
#include <moGsGraph.h>
#include <moResourceManager.h>
#include <moRenderManager.h>

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
/*
    if (m_pVideoGraph) {
        delete m_pVideoGraph;
        m_pVideoGraph = (moVideoGraph*) new moGsGraph();
        m_pVideoGraph->InitGraph();
    } else {
        m_pVideoGraph = (moVideoGraph*) new moGsGraph();
        m_pVideoGraph->InitGraph();
    }
*/
}


bool
moDataSession::SaveToFile( moText p_filename ) {

  bool result = false;
  //
  if (p_filename == "") {
      MODebug2->Error("moDataSession::SaveToFile > p_filename undefined.");
      result = false;
  } else {
  }

	return result;
}

bool
moDataSession::LoadFromFile( moText p_filename ) {
  bool result = false;
  //
  if (p_filename == "") {
      MODebug2->Error("moDataSession::LoadFromFile > p_filename undefined.");
      result = false;
  } else {
  }

	return result;
}


bool
moDataSession::AddKey( moDataSessionKey* p_key ) {

    m_Keys.Add( p_key );
	return true;
}

bool
moDataSession::AddEventKey( moDataSessionEventKey* p_eventkey ) {
    m_EventKeys.Add( p_eventkey );
	return true;
}

bool
moDataSession::Playback() {
    m_SessionPlaybackMode = MO_DATASESSION_PLAY_LIVETOCONSOLE;
	return true;
}

bool
moDataSession::Record() {

    m_SessionRecordMode = MO_DATASESSION_RECORD_DIRECTTOFILE;
	return true;

}

bool
moDataSession::RecordLive( moResourceManager* pRM ) {

    m_SessionPlaybackMode = MO_DATASESSION_PLAY_LIVETOVIDEO;
    if (m_pVideoGraph && m_pDataSessionConfig) {
        m_pVideoGraph->BuildRecordGraph( m_pDataSessionConfig->GetVideoFileName(), pRM->GetRenderMan()->GetFramesPool() );
    }
	return true;
}


//===========================================
//
//   moDataSessionKey
//
//===========================================

moDataSessionKey::moDataSessionKey() {
}

moDataSessionKey::moDataSessionKey(		long p_Timecode,
										moMoldeoActionType p_ActionType,
										moValue& p_Value,
										long p_MoldeoObjectId,
										long p_ParamId,
										long p_ValueId ) {
  p_Timecode = 0;
  p_ActionType = MO_ACTION_UNDEFINED;
  moValue& mvalue(p_Value);
  mvalue.GetSubValueCount();
  p_MoldeoObjectId = 0;
  p_ParamId = 0;
  p_ValueId = 0;

}

moDataSessionKey::~moDataSessionKey() {

}

moValue& moDataSessionKey::GetValue() {
	return m_Value;
}

moMoldeoActionType moDataSessionKey::GetActionType() {
	return m_ActionType;
}



moDataSessionEventKey::moDataSessionEventKey() {

}

moDataSessionEventKey::moDataSessionEventKey(  long p_Timecode,
	moMessage& event ) {
  p_Timecode = 0;
  moMessage msg(event);
  moDebugManager::Message( " Message Id:" + IntToStr( msg.deviceid ) );
}

moDataSessionEventKey::~moDataSessionEventKey() {
}

long    moDataSessionEventKey::GetTimecode() {
	return 0;
}

moMessage&    moDataSessionEventKey::GetMessage() {
	return m_Message;
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

  if ( m_AppPath==moText("") ) {

    moFile fileMol( p_apppath );
    moText workPath = moFileManager::GetWorkPath();
    m_AppPath = workPath;
    moDebugManager::Message(  moText(" moDataSessionConfig() > m_AppPath empty, setting to Work Path: ")
                            + m_AppPath
                            + " p_Port: " + IntToStr( p_Port )
                            + " p_Address: " + IntToStr( p_Address ) );
  }

  moDebugManager::Message(  moText(" moDataSessionConfig() > m_AppPath: ") + m_AppPath );
  moDebugManager::Message(  moText(" moDataSessionConfig() > m_ConsoleConfigName: ") + m_ConsoleConfigName );

  moFile molFile(  m_ConsoleConfigName );

  if ( molFile.GetPath()==moText("") ) {
    m_ConsoleConfigName = m_DataPath + moSlash + m_ConsoleConfigName;
    moDebugManager::Message(  moText(" moDataSessionConfig() > m_ConsoleConfigName fixed to: ") + m_ConsoleConfigName );
  }

	///check if DATADIR exists
	/// a) in linux: just take the datadir
	/// b) in windows: depends
	m_AppDataPath = DATADIR;

	if ( m_AppDataPath == moText("../../data") ) {

    moDirectory mDir( m_AppDataPath );
    moDebugManager::Message(  moText(" moDataSessionConfig() > m_AppDataPath: ") + m_AppDataPath
                            + moText(" Exists: ") + IntToStr( mDir.Exists() ) );

    if (!mDir.Exists()) {
      /// check if
      /// moFile mFile(

      #ifdef MO_WIN32

      moDebugManager::Message(  moText(" moDataSessionConfig() > exeFile Path: ") + moFileManager::GetExePath() );
      moFile exeFile( moFileManager::GetExePath() );
      m_AppDataPath = exeFile.GetPath() + moSlash + m_AppDataPath;

      moDebugManager::Message( m_AppDataPath + moText(" doesn't exists > adding absolute path: ") + m_AppDataPath );
      #else
      moDebugManager::Error( moText(" moDataSessionConfig() > App Data Path doesn't exists: ")  + m_AppDataPath );
      moDebugManager::Error( moText(" please check libmoldeo DATADIR settings (configure.ac) > DATADIR is: ")  + DATADIR );
      #endif
    }

	}


  if ( m_DataPath == moText("") ) {
      m_DataPath = m_AppPath;
      moDebugManager::Message(  moText(" moDataSessionConfig() > m_DataPath set to: ") + m_DataPath );
  } else moDebugManager::Message(  moText(" moDataSessionConfig() > m_DataPath: ") + m_DataPath );



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
moDataSessionConfig::GetAppDataPath() {
  return m_AppDataPath;
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

	SetName("datamanager");
	SetLabelName("datamanager");

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

  if (m_pDataSession) {
    delete m_pDataSession;
    m_pDataSession = NULL;
  }
  if (m_pDataSessionConfig) {
      delete m_pDataSessionConfig;
      m_pDataSessionConfig = NULL;
  }

	if (!m_pDataSessionConfig) {
      m_pDataSessionConfig = new moDataSessionConfig( p_apppath, p_datapath, p_consoleconfig );
	}
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
moDataManager::GetAppDataPath() {
	//m_DataSessionIndex
	if (m_pDataSessionConfig)
		return m_pDataSessionConfig->GetAppDataPath();
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


bool
moDataManager::Export( const moText& p_export_path_, moText p_from_config_console ) {


    /** OPENING CONSOLE PROJECT */

    moConfig from_console;

    MODebug2->Message( "moDataManager::Export > p_export_path_: " + p_export_path_ );

    if ( m_pDataSessionConfig && p_from_config_console == "") {

        p_from_config_console = m_pDataSessionConfig->GetDataPath() + moSlash + m_pDataSessionConfig->GetConsoleConfigName();

    }

    if ( p_from_config_console != "" ) {

        if ( from_console.LoadConfig( p_from_config_console ) != MO_CONFIG_OK ) {

            MODebug2->Error( moText("moDataManager::Export > Couldn't load config from ") + p_from_config_console );
            return false;

        } else {
            MODebug2->Push( moText("moDataManager::Export > Exporting moldeo console project: ") + p_from_config_console  );
        }

    } else {

        MODebug2->Error( moText("moDataManager::Export > Couldn't load config from no console config file, please define one or load a project.") );
        return false;

    }


    /** EXPORTING */


    /** Iterate every config file */

    /** specific to console... */


    bool result = true;
    moMobDefinition MobDef;

    /** IODevices */
    /** Resources */
        /** videomanager > videobuffer... */
    /** PreEffects */
    /** Effects */
    /** PostEffects */
    /** MasterEffects */

    for( int m=0; m<MO_OBJECT_TYPES; m++ ) {

        moText object_type_str = MobDef.GetTypeStr( (moMoldeoObjectType) m  );

        for( MOuint i=0; i< from_console.GetParam( object_type_str ).GetValuesCount(); i++) {

            moText config_file = from_console.GetParam( object_type_str ).GetValue(i).GetSubValue(1).Text();
            result&= IteratedExport( config_file );
        }

    }

    result&= IteratedExport( p_from_config_console );

    return result;

}

bool moDataManager::IteratedExport( const moText& p_from_config_file ) {


    moConfig config;

    if ( p_from_config_file != "" ) {

        if ( config.LoadConfig( p_from_config_file ) != MO_CONFIG_OK ) {

            MODebug2->Error( moText("moDataManager::IteratedExport > error loading config file ") + p_from_config_file );
            return false;

        }


/**

    Browse the parameters of type:

        SCRIPT > copy and create folders and files .
        TEXTURE > check shader's configs .
        TEXTURE_FOLDER > copy full folders .


*/




    } else return true;


    return false;

}

moText moDataManager::MakeRelativeToData( const moText& p_file_full_path  ) {

  moText filerelative_Data = "";
  moFile filetoImport( p_file_full_path );

  if (InData(p_file_full_path)) {

    moText fileabs = filetoImport.GetAbsolutePath();
    fileabs.ReplaceChar( "\\","/");

    moText datapath = GetDataPath();
    datapath.ReplaceChar( "\\","/");

    filerelative_Data = fileabs.Right( fileabs.Length() - datapath.Length() );
    MODebug2->Message( "moDataManager::MakeRelativeToData > fileabs: " + fileabs + " datapath:" + datapath
                    + " filerelative_Data: " + filerelative_Data );
  }

  return filerelative_Data;
}

bool moDataManager::InData( const moText& p_file_full_path  ) {

  //check if p_file_full_path is in the folder...
  moFile filetoImport( p_file_full_path );
  moText fileabs = filetoImport.GetAbsolutePath();
  fileabs.ReplaceChar( "\\","/");

  moText datapath = GetDataPath();
  datapath.ReplaceChar( "\\","/");

  MODebug2->Message( "moDataManager::InData > fileabs: " + fileabs + " datapath:" + datapath );

  if (fileabs.Length()>datapath.Length()) {

    moText fileabs_Data = fileabs.Left( datapath.Length()-1 );
    MODebug2->Message( "moDataManager::InData > fileabs_Data:" + fileabs_Data );
    if (fileabs_Data==datapath) {
      return true;
    }
  }
  MODebug2->Message( "moDataManager::InData > not InData! Copy file please!");
  return false;

}

bool moDataManager::ImportFile( const moText& p_import_file_full_path  ) {

  bool result = false;
  /// Check extension ( if it is a CFG > try to import internal files associated to it )

  /// Copy file to project data folder: DIRECT COPY TO DATA FOLDER root...

  moFile importFile( p_import_file_full_path );
  moText  file_destination_path;
  if (importFile.Exists()) {
    file_destination_path = GetDataPath() + importFile.GetFullName();
    MODebug2->Message("moDataManager::ImportFile > p_import_file_full_path: " + p_import_file_full_path
                      + " to " + file_destination_path );
    result = moFileManager::CopyFile( p_import_file_full_path, file_destination_path );

  }

  return result;

}
