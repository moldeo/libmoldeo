/*******************************************************************************

                                moDataManager.h

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

#ifndef __MO_DATAMANAGER_H__
#define __MO_DATAMANAGER_H__

#include "moBasePlugin.h"
#include <moConsoleState.h>
#include <moActions.h>
#include <moValue.h>
#include <moParam.h>
#include <moEventList.h>
#include <moFileManager.h>
#include <moResourceManager.h>

class moVideoGraph;

/***

    Modos de funcionamiento de la sesion


*/
enum moDataSessionMode {
        MO_DATASESSION_INACTIVE = 0, ///grabaci�n y reproducci�n inactivos, modo predeterminado...
        MO_DATASESSION_PLAYBACK_ACTIVE = 1, ///reproducci�n activa
        MO_DATASESSION_RECORD_ACTIVE = 2,///grabaci�n activa
        MO_DATASESSION_PLAYANDRECORD_ACTIVE = 3 ///reproducci�n y grabaci�n simult�nea
};

/***

    Modos de grabaci�n de sesi�n


*/
enum moDataSessionRecordMode {
    MO_DATASESSION_RECORD_TOMEMORY = 0, ///Grabaci�n a memoria de las claves (al finalizar se puede elegir grabar o no a disco... modo predeterminado)
    MO_DATASESSION_RECORD_BUFFERINGTOFILE = 1, ///grabaci�n con memoria intermedia a disco
    MO_DATASESSION_RECORD_DIRECTTOFILE = 2, ///grabaci�n directa de claves a archivo
    MO_DATASESSION_RECORD_STREAMING = 4
};

/***

    Modos de reproducci�n de sesi�n


*/
enum moDataSessionPlaybackMode {
    MO_DATASESSION_PLAY_LIVETOCONSOLE = 0, ///Reproducci�n en tiempo real a consola
    MO_DATASESSION_PLAY_LIVETOVIDEO = 1, ///Reproducci�n en vivo a video
    MO_DATASESSION_PLAY_LIVETOSTREAM = 2, ///Reproducci�n en vivo por streaming
    MO_DATASESSION_PLAY_RENDERTOVIDEO = 4 ///Reproducci�n renderizada a video por cuadros
};


enum moDataSessionRenderdMode {
    MO_DATASESSION_RENDER_TOMEMORY = 0, ///Renderizado en memoria
    MO_DATASESSION_RENDER_BUFFERINGTOFILE = 1, ///renderizado con memoria intermedia a disco
    MO_DATASESSION_RENDER_DIRECTTOFILE = 2, ///
    MO_DATASESSION_RENDER_STREAMING = 4
};

enum moDataSessionConfigParameters {
  MO_DATA_SESSION_CONFIG_PROJECT=0,
  MO_DATA_SESSION_CONFIG_RESOLUTION,
  MO_DATA_SESSION_CONFIG_RENDER_FOLDER,
  MO_DATA_SESSION_CONFIG_LENGTH,
  MO_DATA_SESSION_CONFIG_KEYS,
  MO_DATA_SESSION_CONFIG_EVENT_KEYS,
};


/**

Configuraci�n de sesi�n. Se crea antes de empezar una sesi�n con los datos espec�ficos como son:
    el directorio de datos
    el nombre del archivo de consola
    el nombre del archivo para grabar la sesi�n
    la cantidad m�xima de claves
    la cantidad m�xima de tiempo a grabar en milisegundos

*/
class LIBMOLDEO_API moDataSessionConfig : public moConfig {
  friend class moDataSession;
	public:

		moDataSessionConfig();

		moDataSessionConfig(
                            moText p_apppath,
                            moText p_datapath,
                            moText p_consoleconfig,
                            moText p_SessionFileName=moText("sesion.mos"),
                            moText p_VideoFileName=moText("sesion.mp4"),
                            long p_MaxKeys = 10000,
                            long p_MaxTimecode=3600000/*1 hora*/,
                            long p_Port = 0 /**/,
                            long p_Address=0 /**/ );

		virtual ~moDataSessionConfig();

		moText GetDataPath();
		moText GetAppPath();
		moText GetAppDataPath();
		moText GetConsoleConfigName();
		moText GetVideoFileName();
		moText GetSessionFileName();
		moText GetPluginsPath();

		long   GetMaxKeys() {
                return m_MaxKeys;
        }

		long   GetMaxTimecode() {
                return m_MaxTimecode;
        }

		long   GetPort() {
		    return m_Port;
		}

		long   GetAddress() {
                return m_Address;
        }

	private:

    moText m_AppPath;/// Directorio de ejecuci�n de la aplicaci�n
		moText m_DataPath;/// Directorio de datos de la sesi�n
		moText m_ConsoleConfigName;///archivo de definici�n de la consola (*.mol)
    moText m_AppDataPath;
    moText m_PluginsPath;
        /*
        MO_DATASESSION_MODE_SAVETOMEMORY
        */
		moText  m_SessionFileName; ///nombre del archivo a grabar
		moText  m_VideoFileName; ///nombre del arcivo de video a grabar
		long    m_MaxKeys; ///n�mero de eventos clave a grabar en memoria
		long    m_MaxTimecode;///tiempo m�ximo de grabaci�n de claves

		bool    m_Port;///para streaming
		bool    m_Address;///para streaming

};

class LIBMOLDEO_API moDataSessionKey {
    friend class moConsole;
    public:

        moDataSessionKey();
        moDataSessionKey(		long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            long p_MoldeoObjectId,
                            long p_PreconfId );

        moDataSessionKey(   long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            long p_MoldeoObjectId=MO_UNDEFINED );

        moDataSessionKey(   long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            long p_MoldeoObjectId,
                            long p_ParamId,
                            long p_ValueId );

        moDataSessionKey(   long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            long p_MoldeoObjectId,
                            long p_ParamId,
                            long p_ValueId,
                            const moValue& p_Value );

        moDataSessionKey(   long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            long p_MoldeoObjectId,
                            const moEffectState& p_effect_state );

        moDataSessionKey(   long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            long p_MoldeoObjectId,
                            long p_ParamId,
                            const moParamDefinition& p_param_definition );

        virtual ~moDataSessionKey();
        moDataSessionKey(const moDataSessionKey &src);
        moDataSessionKey &operator = (const moDataSessionKey &src);

        moValue& GetValue();
        moMoldeoActionType GetActionType() const;
        bool IsInTime( long time_position, long time_interval );

        const moText& ToJSON();
        const moText& ToXML();

        int Set(const moText& p_XmlText );

    private:

        long                  m_TimeCode; /// Valor del tick
        moMoldeoActionType    m_ActionType; /// accion

        long                  m_ObjectId; ///Opcional para identificaci�n del �ndice �nico de objeto
        long                  m_ParamId; ///Opcional para identificaci�n del �ndice �nico de par�metro
        long                  m_ValueId; ///Opcional para identificaci�n del �ndice �nico de valor
        long                  m_PreconfId;
        moValue               m_Value; /// Valor del dato agregado o modificado
        moParamDefinition     m_ParamDefinition; /// Valor del dato agregado o modificado
        moEffectState         m_EffectState; /// Valor del dato agregado o modificado

        moText                m_FullJSON;
        moText                m_FullXML;
};

class LIBMOLDEO_API moDataSessionEventKey {

    public:

        moDataSessionEventKey();
        moDataSessionEventKey(  long p_Timecode,
                                moMessage event );

        virtual ~moDataSessionEventKey();

        long    GetTimecode();
        moMessage&    GetMessage();


    private:

        long                  m_Timecode; /// Valor del tick
        moMessage             m_Message;

};

moDeclareExportedDynamicArray( moDataSessionKey*, moDataSessionKeys);
moDeclareExportedDynamicArray( moDataSessionEventKey*, moDataSessionEventKeys );

class LIBMOLDEO_API moDataSession : public moAbstract {

  public:
        moDataSession();
        virtual ~moDataSession();

        void Set(   moText p_Name,
                    moDataSessionConfig* pSessionConfig,
                    moDataSessionMode p_sessionmode,
                    moDataSessionRecordMode p_recordmode,
                    moDataSessionPlaybackMode p_playbackmode,
                    moResourceManager*  p_ResMan );


        bool SaveToFile( const moText& p_filename=moText("") );
        bool LoadFromFile( const moText& p_filename );

        bool AddKey( const moDataSessionKey& p_key );
        bool AddEventKey( const moDataSessionEventKey& p_eventkey );

        bool Playback( moConsoleState& p_console_state ); ///fast start playback
        bool StopPlayback(moConsoleState& p_console_state); ///stop playback

        bool Record(moConsoleState& p_console_state); ///start record
        bool StopRecord(moConsoleState& p_console_state); ///stop record

        bool Render(moConsoleState& p_console_state); ///render to images
        bool StopRender(moConsoleState& p_console_state); ///stop render

        bool RecordLive( moResourceManager* pRM ); /// fast live record

        bool Loaded();
        bool LoadSession();
        bool SetKey( int p_actual_key);
        const moDataSessionKey& GetActualKey();
        const moDataSessionKey& NextKey( moConsoleState& m_ConsoleState );

        bool SessionStart();
        bool SessionEnded();
        int GetKeyCount();
        int GetRenderedFrames() const;
        const moText& GetRenderedFolder() {
          return m_Rendered_Folder;
        }
        void SetRenderedFolder( const moText& p_rendered_folder );
        bool StepRender( moConsoleState& p_console_state );

        const moText& ToJSON();

    private:

        long                    m_StartTimeCode;
        long                    m_EndTimeCode;
        moText                  m_Name;
        moDataSessionConfig*    m_pDataSessionConfig;
        moResourceManager*      m_pResourceManager;

        int                      m_iActualKey;
        moDataSessionKeys        m_Keys;
        moDataSessionEventKeys   m_EventKeys;

        moDataSessionMode         m_SessionMode;
        moDataSessionRecordMode   m_SessionRecordMode;
        moDataSessionPlaybackMode m_SessionPlaybackMode;
        moDataSessionRenderdMode  m_SessionRenderMode;

        moVideoGraph*           m_pVideoGraph;
        moDataSessionKey        m_ActualKey;

        int                     m_Rendered_Frames;
        moText                  m_Rendered_Folder;
        moText                  m_FullJSON;

};


moDeclareExportedDynamicArray( moDataSession*, moDataSessions );


class LIBMOLDEO_API moDataManager : public moResource
{
	public:

		moDataManager();
		virtual ~moDataManager();

		virtual MOboolean Init();
		virtual MOboolean Init( moText p_apppath, moText p_datapath, moText p_consoleconfig );
		virtual MOboolean Finish();

		moText GetDataPath();
		moText GetConsoleConfigName();
		moText GetAppPath();
		moText GetAppDataPath();
		moText GetPluginsPath();

    /**
    * Devuelve un moFile relativo a la carpeta del proyecto .mol cargado.
    * @see GetDataPath
    * @see GetDataDir para obtener una carpeta relativa al proyecto
    */
		moFile GetDataFile( const moText& p_file_name );

    /**
    * Devuelve un moFile relativo a la carpeta de la aplicacion
    * @see GetAppDataPath
    * @see GetAppDataDir para obtener una carpeta relativa a la aplicacion
    */
		moFile GetAppDataFile( const moText& p_file_name );

    /**
    * Devuelve un moDirectory relativo a la carpeta del proyecto .mol cargado.
    * @see GetDataPath
    * @see GetDataFile para obtener un archivo relativo al proyecto
    */
		moDirectory GetDataDir( const moText& p_dir_name );

    /**
    * Devuelve un moDirectory relativo a la carpeta de la aplicacion
    * @see GetAppDataPath
    * @see GetAppDataFile para obtener un archivo relativo a la aplicacion
    */
		moDirectory GetAppDataDir( const moText& p_dir_name );

		const moPluginDefinitions& GetPluginDefinitions() {
      return m_PluginDefinitions;
		}

    void StartRecordingSession(  );
    void StartPlayinbackSession(  );

    moDataSession*  GetSession();

    bool Export( const moText& p_export_path , moText p_from_config_console = moText("") );
    bool IteratedExport( const moText& p_from_config_file_ );
    bool ImportFile( const moText& p_import_file_full_path  );
    bool InData( const moText& p_file_full_path  );
    moText MakeRelativeToData( const moText& p_file_full_path  );

    int ReloadPluginDefinitions( moText plugindir = "", moMoldeoObjectType mobjecttype=MO_OBJECT_UNDEFINED );

    static const moText& GetDataDir();
    static const moText& GetModulesDir();

    static moText m_DataDir; /** data dir*/
    static moText m_ModulesDir;/** plugin dir */

	protected:

		moDataSessionConfig*	m_pDataSessionConfig;
		moDataSession*          m_pDataSession;

    moPluginDefinitions m_PluginDefinitions;

};

#endif
