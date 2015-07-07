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
#include <tinyxml.h>

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
    m_Rendered_Frames = 0;
    m_pResourceManager = NULL;
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
            moDataSessionPlaybackMode p_playbackmode,
            moResourceManager*  p_ResMan ) {
    m_Name = p_Name;
    m_pDataSessionConfig = pSessionConfig;
    m_pResourceManager = p_ResMan;
    m_SessionMode = p_sessionmode;
    m_SessionPlaybackMode = p_playbackmode;
    m_SessionRecordMode = p_recordmode;
    //m_Keys.Init( pSessionConfig->GetMaxKeys(), NULL );
    m_Keys.Init(0,NULL);
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
moDataSession::SaveToFile( const moText& p_filename ) {

  MODebug2->Message("moDataSession::SaveToFile: ");

  bool result = false;
  if (m_pDataSessionConfig==NULL) {
    MODebug2->Error("moDataSession::SaveToFile > no Data Session Config defined.");
    return false;
  }
  moText FileDestination = m_pDataSessionConfig->GetSessionFileName();

  if (p_filename != moText("") && p_filename!=FileDestination) {
      //m_pDataSessionConfig->GetConfigDefinition()->Set
      //MODebug2->Error("moDataSession::SaveToFile > p_filename undefined.");
      //result = false;
      //m_pDataSessionConfig->CreateDefault( p_filename );
      FileDestination = p_filename;
  }

  moParam& paramKeys( m_pDataSessionConfig->GetParam( "keys" ) );
  moValues& valuesKey( paramKeys.GetValues() );
  valuesKey.Empty();
  /**
  while(paramKeys.GetValuesCount()>0) {
      paramKeys.DeleteValue(0);
  }
  */
  for( int keyi=0; keyi<(int)this->m_Keys.Count(); keyi++) {

    moDataSessionKey* pKey = m_Keys[keyi];
    if (pKey) {
      moText pKeyXML = pKey->ToXML();
      moValue newValue( "","XML" );
      MODebug2->Message("moDataSession::SaveToFile > keyi: " + IntToStr(keyi) + " xml:" + pKeyXML );

      moValueBase& vb( newValue.GetSubValue(0) );
      vb.SetText( pKeyXML );
      vb.SetType( MO_VALUE_XML );
      paramKeys.AddValue( newValue );
    }
  }

  m_pDataSessionConfig->SaveConfig( FileDestination );


	return result;
}

bool
moDataSession::LoadFromFile( const moText& p_filename ) {
  bool result = false;

  moText FileDestination = p_filename;

  if (m_pDataSessionConfig==NULL) {
    MODebug2->Error("moDataSession::LoadFromFile > no Data Session Config defined.");
    return false;
  }


  if (p_filename == "") {
    FileDestination = m_pDataSessionConfig->GetSessionFileName();
  }

  if (m_pDataSessionConfig->LoadConfig( FileDestination )==MO_CONFIG_OK) {

    for( int keyi=0; keyi<(int)this->m_Keys.Count(); keyi++) {
      moDataSessionKey* dsk = m_Keys[keyi];
      if (dsk) delete dsk;
    }

    m_Keys.Empty();

    moParam& paramKeys( m_pDataSessionConfig->GetParam( "keys" ) );
    moValues& valuesKey( paramKeys.GetValues() );
    for( int i=0; i<(int)valuesKey.Count(); i++ ) {
      moValueBase& vbase( valuesKey[i].GetSubValue(0) );
      if (vbase.GetType()==MO_VALUE_XML) {
        moDataSessionKey* newKey = new moDataSessionKey();
        if (newKey) {
          newKey->Set( vbase.Text() );
          m_Keys.Add( newKey );
        }
      }
    }

  } else MODebug2->Error("moDataSession::LoadFromFile > could not load the session config file at " + FileDestination);

	return result;
}

bool
moDataSession::LoadSession() {
  if (!m_pDataSessionConfig) {
      MODebug2->Error("moDataSession::LoadSession() > no session config object.");
      return false;
  }
  return m_pDataSessionConfig->LoadConfig( m_pDataSessionConfig->GetSessionFileName() )!=MO_CONFIGFILE_NOT_FOUND;
}

bool
moDataSession::AddKey( const moDataSessionKey& p_key ) {
  moDataSessionKey* newKey = new moDataSessionKey();
  if (newKey) {
    (*newKey) = p_key;
    m_Keys.Add( newKey );
    MODebug2->Message( "moDataSession::AddKey > " + newKey->ToJSON() );
    return true;
  }
	return false;
}

bool
moDataSession::AddEventKey( const moDataSessionEventKey& p_eventkey ) {
  moDataSessionEventKey* eventKey = new moDataSessionEventKey();
  if (eventKey) {
    (*eventKey) = p_eventkey;
    m_EventKeys.Add( eventKey );
    return true;
  }
  return false;
}

bool
moDataSession::Playback(moConsoleState& p_console_state) {
  m_SessionPlaybackMode = MO_DATASESSION_PLAY_LIVETOCONSOLE;
  m_iActualKey = 0;
  p_console_state.m_Mode = MO_CONSOLE_MODE_PLAY_SESSION;
	return true;
}

bool
moDataSession::StopRecord(moConsoleState& p_console_state) {
  m_EndTimeCode = moGetTicks();
  MODebug2->Message("moDataSession::StopRecord > m_StartTimeCode: "+IntToStr(m_StartTimeCode)+" m_EndTimeCode:" + IntToStr(m_EndTimeCode));
  p_console_state.m_Mode = MO_CONSOLE_MODE_LIVE;
  SaveToFile();
  return true;
}

bool
moDataSession::Record(moConsoleState& p_console_state) {
  m_SessionRecordMode = MO_DATASESSION_RECORD_TOMEMORY;

  if (p_console_state.m_Mode==MO_CONSOLE_MODE_RECORD_SESSION) {
    return StopRecord( p_console_state ) && Render( p_console_state );
  } else if (p_console_state.m_Mode==MO_CONSOLE_MODE_RENDER_SESSION) {
    return StopRender( p_console_state );
  }

  p_console_state.m_Mode = MO_CONSOLE_MODE_RECORD_SESSION;
  moStopTimer();
  moStartTimer();
  m_StartTimeCode = moGetTicks();
  MODebug2->Message("moDataSession::Record > m_StartTimeCode:" + IntToStr(m_StartTimeCode));

	return true;
}


int
moDataSession::GetRenderedFrames() const {
  return m_Rendered_Frames;
}

bool
moDataSession::StopRender(moConsoleState& p_console_state) {
  //m_EndTimeCode = moGetTicks();
  int zero = moGetTicksAbsoluteStep(-moGetTicksAbsoluteStep(0));
  p_console_state.m_Mode = MO_CONSOLE_MODE_LIVE;
  MODebug2->Message("moDataSession::StopRender > m_EndTimeCode:" + IntToStr(m_EndTimeCode)+" zero:" + IntToStr(zero));
  return true;
}

bool
moDataSession::Render(moConsoleState& p_console_state) {
  m_SessionRenderMode = MO_DATASESSION_RENDER_TOMEMORY;

  if (p_console_state.m_Mode==MO_CONSOLE_MODE_RENDER_SESSION) {
    return StopRender( p_console_state );
  } else if (p_console_state.m_Mode!=MO_CONSOLE_MODE_RENDER_SESSION) {
    m_Rendered_Frames = 0;
  }

  p_console_state.m_Mode = MO_CONSOLE_MODE_RENDER_SESSION;
  //RESET
  int zero = moGetTicksAbsoluteStep(-moGetTicksAbsoluteStep(0));
  if (zero!=0) MODebug2->Error("moDataSession::Render> not zero! zero: " + IntToStr(zero));
  long tickis = moGetTicksAbsoluteStep( 41 );
  moStopTimer();
  moStartTimer();
  long tickis_moldeo = moGetTicks();
  MODebug2->Message("moConsole::ConsoleModeUpdate > START RENDER session: absolute tickis: " + IntToStr(tickis)+" tickis(moldeo): " + IntToStr(tickis_moldeo) );
  m_iActualKey = 0;
  MODebug2->Message("moConsole::ConsoleModeUpdate > START RENDER session: m_StartTimeCode: " + IntToStr(m_StartTimeCode) );

  return true;
}

bool
moDataSession::StepRender( moConsoleState& p_console_state ) {

    moText frame_result;
    moText temp_render = moText("temp_render");

    p_console_state.Activated();
    //int mod1000 = m_Rendered_Frames / (int)1000;
    //temp_render+="_"+IntToStr(mod1000);
    moText frame_filename = "frame_" + IntToStr( m_Rendered_Frames, 7 );
    if (m_pResourceManager && m_pResourceManager->GetRenderMan())
      RenderMan()->Screenshot( DataMan()->GetDataPath() + temp_render, frame_result, "JPGGOOD", frame_filename );

    MODebug2->Message("moDataSession::StepRender > 24/1 frame number:" + IntToStr(m_Rendered_Frames)+" frame_result:" + frame_result);
    m_Rendered_Frames+=1;
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

bool
moDataSession::Loaded() {
  if (m_pDataSessionConfig) {
    return m_pDataSessionConfig->IsConfigLoaded();
  }
  return false;
}

bool
moDataSession::SessionEnded() {
  if (m_iActualKey>=(int)m_Keys.Count() || m_Keys.Count()==0 )
    return true;

  return false;
}

int
moDataSession::GetKeyCount() {
  return m_Keys.Count();
}

bool moDataSession::SessionStart() {
  m_iActualKey = 0;
  return true;
}

const moDataSessionKey&
moDataSession::GetActualKey() {
  return m_ActualKey;
}


bool
moDataSession::SetKey( int p_actual_key) {
if ( 0<=p_actual_key && p_actual_key<(int)m_Keys.Count()) {
    m_iActualKey = p_actual_key;
    m_ActualKey = (*m_Keys[m_iActualKey]);
    return true;
  }

  return false;
}


const moDataSessionKey&
moDataSession::NextKey( moConsoleState& m_ConsoleState ) {

  //check if timecode correspond to m_ConsoleState clock ??
  m_ActualKey = moDataSessionKey();
  int time_code_ms = m_ConsoleState.tempo.Duration();

  if (m_iActualKey<0 || m_iActualKey>=(int)m_Keys.Count()) {
    return m_ActualKey;
  }

  if (m_iActualKey<(int)m_Keys.Count()) {

    moDataSessionKey TestKey = (*m_Keys[m_iActualKey]);

    if ( TestKey.IsInTime(time_code_ms, m_ConsoleState.step_interval ) ) {

      MODebug2->Message("moDataSession::NextKey > Founded timecode Key in! time_code_ms: " + IntToStr(time_code_ms) );

      m_ActualKey = TestKey;
      m_iActualKey++;/**go to the next key*/
    }
  }

  return m_ActualKey;
}


//===========================================
//
//   moDataSessionKey
//
//===========================================

moDataSessionKey::moDataSessionKey() {
  m_TimeCode = 0;
  m_ActionType = MO_ACTION_UNDEFINED;


  m_ObjectId = MO_UNDEFINED;
  m_ParamId = MO_UNDEFINED;
  m_ValueId = MO_UNDEFINED;
  m_PreconfId = MO_UNDEFINED;
}

moDataSessionKey::moDataSessionKey(		long p_Timecode,
										moMoldeoActionType p_ActionType,
										long p_MoldeoObjectId,
										long p_PreconfId ) {
  m_TimeCode = p_Timecode;
  m_ActionType = p_ActionType;

  m_ObjectId = p_MoldeoObjectId;
  m_PreconfId = p_PreconfId;
}

moDataSessionKey::moDataSessionKey(		long p_Timecode,
										moMoldeoActionType p_ActionType,
										long p_MoldeoObjectId ) {
  m_TimeCode = p_Timecode;
  m_ActionType = p_ActionType;
  m_ObjectId = p_MoldeoObjectId;
}


moDataSessionKey::moDataSessionKey(   long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            long p_MoldeoObjectId,
                            long p_ParamId,
                            long p_ValueId,
                            const moValue& p_Value ) {
  m_TimeCode = p_Timecode;
  m_ActionType = p_ActionType;

  m_Value = p_Value;

  m_ObjectId = p_MoldeoObjectId;
  m_ParamId = p_ParamId;
  m_ValueId = p_ValueId;

}

moDataSessionKey::moDataSessionKey(   long p_Timecode,
                    moMoldeoActionType p_ActionType,
                    long p_MoldeoObjectId,
                    const moEffectState& p_effect_state ) {

  m_TimeCode = p_Timecode;
  m_ActionType = p_ActionType;

  m_ObjectId = p_MoldeoObjectId;

  m_EffectState = p_effect_state;
}

moDataSessionKey::moDataSessionKey(   long p_Timecode,
                                      moMoldeoActionType p_ActionType,
                                      long p_MoldeoObjectId,
                                      long p_ParamId,
                                      const moParamDefinition& p_param_definition ) {
  m_TimeCode = p_Timecode;
  m_ActionType = p_ActionType;

  m_ObjectId = p_MoldeoObjectId;
  m_ParamId = p_ParamId;

  m_ParamDefinition = p_param_definition;
}


moDataSessionKey::~moDataSessionKey() {

}

moDataSessionKey::moDataSessionKey(const moDataSessionKey &src) {
 (*this) = src;
}

moDataSessionKey &
moDataSessionKey::operator = (const moDataSessionKey &src) {
		m_TimeCode = src.m_TimeCode;
		m_ActionType = src.m_ActionType;
		m_Value = src.m_Value;
		m_ObjectId = src.m_ObjectId;
		m_ParamId = src.m_ParamId;
		m_ValueId = src.m_ValueId;
		m_PreconfId = src.m_PreconfId;
		m_ParamDefinition = src.m_ParamDefinition;
		m_EffectState = src.m_EffectState;
		return *this;
}

bool
moDataSessionKey::IsInTime( long time_position, long time_interval ) {
    if (   m_TimeCode<=time_position
        && time_position< (m_TimeCode+time_interval) ) {
      return true;
    }
    return false;
}



moValue& moDataSessionKey::GetValue() {
	return m_Value;
}

moMoldeoActionType moDataSessionKey::GetActionType() const {
	return m_ActionType;
}

const moText&
moDataSessionKey::ToJSON() {
  /** --- */
  m_FullJSON = "";
  m_FullJSON+= "{";
  m_FullJSON+= moText("'TimeCode': '") + IntToStr((long)m_TimeCode)+"',";
  m_FullJSON+= moText("'ActionType': '") + IntToStr((long)m_ActionType)+"',";
  m_FullJSON+= moText("'ObjectId': '") + IntToStr(m_ObjectId)+"',";
  m_FullJSON+= moText("'ParamId': '") + IntToStr(m_ParamId)+"',";
  m_FullJSON+= moText("'ValueId': '") + IntToStr(m_ValueId)+"',";
  m_FullJSON+= moText("'PreconfId': '") + IntToStr(m_PreconfId)+"',";
  m_FullJSON+= moText("'ParamDefinition': ") + m_ParamDefinition.ToJSON() +",";
  m_FullJSON+= moText("'EffectState': ") + m_EffectState.ToJSON() +",";
  m_FullJSON+= moText("'Value': ") + m_Value.ToJSON() +",";
  m_FullJSON+= "}";
  return m_FullJSON;
}

const moText&
moDataSessionKey::ToXML() {
  /** --- */
  m_FullXML = "";

  m_FullXML+= "<moDataSessionKey timecode='"+IntToStr((long)m_TimeCode)+"' ";
  m_FullXML+= moText(" actiontype='") + IntToStr((long)GetActionType())+"' ";
  m_FullXML+= moText(" objectid='") + IntToStr(m_ObjectId)+"' ";
  m_FullXML+= moText(" paramid='") + IntToStr(m_ParamId)+"' ";
  m_FullXML+= moText(" valueid='") + IntToStr(m_ValueId)+"' ";
  m_FullXML+= moText(" preconfid='") + IntToStr(m_PreconfId)+"' >";
  if (GetActionType()==MO_ACTION_PARAM_SET) m_FullXML+= m_ParamDefinition.ToXML();
  if (GetActionType()==MO_ACTION_EFFECT_SETSTATE) m_FullXML+= m_EffectState.ToXML();
  if (GetActionType()==MO_ACTION_VALUE_SET) m_FullXML+= m_Value.ToXML();
  m_FullXML+= "</moDataSessionKey>";
  return m_FullXML;
}

int
moDataSessionKey::Set(const moText& p_XmlText ) {

  TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  //TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );
  TiXmlElement* sessionkeyNode = m_XMLDoc.FirstChildElement("moDataSessionKey");

  //if (rootKey) {

    //TiXmlElement* sceneStateNode = rootKey->FirstChildElement("moSceneState");
    if (sessionkeyNode) {
      m_TimeCode = atoi(moText( sessionkeyNode->Attribute("timecode") ));
      m_ActionType = moReactionListenerManager::StrToActionType( moText( sessionkeyNode->Attribute("actiontype") ) );
      m_ObjectId = atoi( moText( sessionkeyNode->Attribute("objectid") ) );
      m_ParamId = atoi( moText( sessionkeyNode->Attribute("paramid") ) );
      m_ValueId = atoi( moText( sessionkeyNode->Attribute("valueid") ) );
      m_PreconfId = atoi( moText( sessionkeyNode->Attribute("preconfid") ) );
      if (GetActionType()==MO_ACTION_PARAM_SET) {
        TiXmlElement* paramdefNode = sessionkeyNode->FirstChildElement("moParamDefinition");
        if (paramdefNode) m_ParamDefinition.Set( moText( sessionkeyNode->GetText() ) );
      } else if (GetActionType()==MO_ACTION_EFFECT_SETSTATE) {
        TiXmlElement* efffectstateNode = sessionkeyNode->FirstChildElement("moEffectState");
        if (efffectstateNode) m_EffectState.Set( moText( sessionkeyNode->GetText() ) );
      } else if (GetActionType()==MO_ACTION_VALUE_SET) {
        TiXmlElement* valueNode = sessionkeyNode->FirstChildElement("moValue");
        if (valueNode) m_Value.Set( moText( sessionkeyNode->GetText() ) );
      }
      return 0;
    } else moDebugManager::Log( "No XML moDataSessionKey in: " + p_XmlText );

  //} else moDebugManager::Error();
  return -1;
}

/**********************************************
*
*          moDataSessionEventKey
*
***********************************************/


moDataSessionEventKey::moDataSessionEventKey() {
  m_Timecode = 0;
}

moDataSessionEventKey::moDataSessionEventKey(  long p_Timecode,
	moMessage event ) {
  m_Timecode = p_Timecode;
  moMessage msg(event);
  moDebugManager::Message( " Message Id:" + IntToStr( msg.deviceid ) );
}

moDataSessionEventKey::~moDataSessionEventKey() {
}

long    moDataSessionEventKey::GetTimecode() {
	return m_Timecode;
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
	m_PluginsPath = MODULESDIR;

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
  moFile mosFile( m_SessionFileName );

  if ( molFile.GetPath()==moText("") ) {
    m_ConsoleConfigName = m_DataPath + moSlash + m_ConsoleConfigName;
    moDebugManager::Message(  moText(" moDataSessionConfig() > m_ConsoleConfigName fixed to: ") + m_ConsoleConfigName );
  }

  if ( mosFile.GetPath()==moText("") ) {
    m_SessionFileName = m_DataPath + moSlash + m_SessionFileName;
    moDebugManager::Message(  moText(" moDataSessionConfig() > m_SessionFileName fixed to: ") + m_SessionFileName );
  }

  GetConfigDefinition()->Set( "session", "moDataSession" );
  GetConfigDefinition()->Add( "project", MO_PARAM_TEXT, MO_DATA_SESSION_CONFIG_PROJECT, moValue( m_ConsoleConfigName, "TXT") );
  GetConfigDefinition()->Add( "resolution", MO_PARAM_TEXT, MO_DATA_SESSION_CONFIG_RESOLUTION, moValue( "1024x768", "TXT") );
  GetConfigDefinition()->Add( "render_folder", MO_PARAM_TEXT, MO_DATA_SESSION_CONFIG_RENDER_FOLDER, moValue( "", "TXT") );
  GetConfigDefinition()->Add( "length", MO_PARAM_NUMERIC, MO_DATA_SESSION_CONFIG_LENGTH, moValue( "0", "NUM").Ref() );
  GetConfigDefinition()->Add( "keys", MO_PARAM_TEXT, MO_DATA_SESSION_CONFIG_KEYS, moValue( "<moDataSessionKey></moDataSessionKey>", "XML") );
  GetConfigDefinition()->Add( "eventkeys", MO_PARAM_TEXT, MO_DATA_SESSION_CONFIG_EVENT_KEYS, moValue( "<moDataSessionEventKey></moDataSessionEventKey>", "XML") );
  if (CreateDefault( m_SessionFileName )) {
    moDebugManager::Message("moDataSessionConfig > Created "+m_SessionFileName);
  }

  if (LoadConfig(m_SessionFileName)==MO_CONFIG_OK) {
    moDebugManager::Message("moDataSessionConfig > Loaded "+m_SessionFileName);
  } else moDebugManager::Error("moDataSessionConfig > Not Loaded "+m_SessionFileName);

	///check if DATADIR exists
	/// a) in linux: just take the datadir
	/// b) in windows: it depends
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

moText
moDataSessionConfig::GetPluginsPath() {
  return m_PluginsPath;
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
                                 MO_DATASESSION_PLAY_LIVETOCONSOLE,
                                 m_pResourceManager );
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
                                 MO_DATASESSION_PLAY_LIVETOCONSOLE,
                                 m_pResourceManager );
	}

	///reload plugin definitions
	ReloadPluginDefinitions();

	return true;
}

int
moDataManager::ReloadPluginDefinitions( moText plugindir, moMoldeoObjectType mobjecttype ) {

  moDirectory DirEffects;
  bool bDebug = true;
  moText PluginName;
  moText pluginfullpath = m_pDataSessionConfig->GetPluginsPath();

  if (plugindir.Length()==0 && mobjecttype==MO_OBJECT_UNDEFINED) {

    ReloadPluginDefinitions( pluginfullpath + moSlash + "preeffects", MO_OBJECT_PREEFFECT );
    ReloadPluginDefinitions( pluginfullpath + moSlash + "effects", MO_OBJECT_EFFECT );
    ReloadPluginDefinitions( pluginfullpath + moSlash + "posteffects", MO_OBJECT_POSTEFFECT );
    ReloadPluginDefinitions( pluginfullpath + moSlash + "mastereffects", MO_OBJECT_MASTEREFFECT );
    ReloadPluginDefinitions( pluginfullpath + moSlash + "resources", MO_OBJECT_RESOURCE );
    ReloadPluginDefinitions( pluginfullpath + moSlash + "iodevices", MO_OBJECT_IODEVICE );
    return 0;
  }

  if (plugindir.Length()==0 && mobjecttype!=MO_OBJECT_UNDEFINED) {
    switch(mobjecttype) {
      case MO_OBJECT_PREEFFECT:
        ReloadPluginDefinitions( pluginfullpath + moSlash + "preeffects", MO_OBJECT_PREEFFECT );
        break;
      case MO_OBJECT_EFFECT:
        ReloadPluginDefinitions( pluginfullpath + moSlash + "effects", MO_OBJECT_EFFECT );
        break;
      case MO_OBJECT_POSTEFFECT:
        ReloadPluginDefinitions( pluginfullpath + moSlash + "posteffects", MO_OBJECT_POSTEFFECT );
        break;
      case MO_OBJECT_MASTEREFFECT:
        ReloadPluginDefinitions( pluginfullpath + moSlash + "mastereffects", MO_OBJECT_MASTEREFFECT );
        break;
      case MO_OBJECT_IODEVICE:
        ReloadPluginDefinitions( pluginfullpath + moSlash + "resources", MO_OBJECT_RESOURCE );
        break;
      case MO_OBJECT_RESOURCE:
        ReloadPluginDefinitions( pluginfullpath + moSlash + "iodevices", MO_OBJECT_IODEVICE );
        break;
      default:
        break;
    }
    return 0;
  }


  /// Open DIRECTORY [plugindir] and search for module libraries (plugins)
  ///
  #ifdef MO_WIN32
  DirEffects.Open( plugindir, moText("/*.dll") );
  #else
  DirEffects.Open( plugindir, moText("/*.so") );
  #endif // MO_LINUX

  if (DirEffects.Exists()) {

      moFile* pFile = NULL;
      moText FileNameEnd;

      pFile = DirEffects.FindFirst();

      if (pFile!=NULL)
          MODebug2->Message( moText("File founded") );

      while(pFile!=NULL) {

          FileNameEnd = pFile->GetFileName();
          PluginName = pFile->GetFileName();

          FileNameEnd.Right(2);
          bDebug = ( FileNameEnd==moText("_d") );
          if (bDebug) PluginName.Left( PluginName.Length() - 3 );

          ///LINUX VERSION
          if (!bDebug) {
              if (pFile->GetExtension()==moText(".so")) {
                  #ifndef MO_WIN32
                      ///extract the "libmoldeo_" part
                      PluginName.Right( PluginName.Length() - 10 );
                      m_PluginDefinitions.Add( moPluginDefinition( PluginName, pFile->GetCompletePath(), mobjecttype ) );
                      MODebug2->Message( pFile->GetFileName() );
                  #endif
              } else if (pFile->GetExtension()==moText(".dll")) {
                  #ifdef MO_WIN32
                      m_PluginDefinitions.Add( moPluginDefinition( PluginName, pFile->GetCompletePath(), mobjecttype ) );
                      MODebug2->Message( pFile->GetFileName() );

                  #endif
              }
          }

          pFile = DirEffects.FindNext();
      }
  } else MODebug2->Error( moText("Directory doesn´t exists:")+(moText)plugindir );


  return 0;
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

moText
moDataManager::GetPluginsPath() {
	//m_DataSessionIndex
	if (m_pDataSessionConfig)
		return m_pDataSessionConfig->GetPluginsPath();
	return moText("");
}

MOboolean moDataManager::Finish() {
	return true;
}

void moDataManager::StartRecordingSession(  ) {

    //m_pDataSession->Record(m_ConsoleState);

}

void moDataManager::StartPlayinbackSession(  ) {
   // m_pDataSession->Playback(m_ConsoleState);
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

        moText object_type_class = MobDef.GetTypeToClass( (moMoldeoObjectType) m  );

        for( MOuint i=0; i< from_console.GetParam( object_type_class ).GetValuesCount(); i++) {

            moText config_file = from_console.GetParam( object_type_class ).GetValue(i).GetSubValue(1).Text();
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
