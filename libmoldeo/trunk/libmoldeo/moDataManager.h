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

#include <moActions.h>
#include <moValue.h>
#include <moParam.h>
#include <moEventList.h>
#include <moResourceManager.h>

class moVideoGraph;

/***

    Modos de funcionamiento de la sesion


*/
enum moDataSessionMode {
        MO_DATASESSION_INACTIVE = 0, ///grabación y reproducción inactivos, modo predeterminado...
        MO_DATASESSION_PLAYBACK_ACTIVE = 1, ///reproducción activa
        MO_DATASESSION_RECORD_ACTIVE = 2,///grabación activa
        MO_DATASESSION_PLAYANDRECORD_ACTIVE = 3 ///reproducción y grabación simultánea
};

/***

    Modos de grabación de sesión


*/
enum moDataSessionRecordMode {
    MO_DATASESSION_RECORD_TOMEMORY = 0, ///Grabación a memoria de las claves (al finalizar se puede elegir grabar o no a disco... modo predeterminado)
    MO_DATASESSION_RECORD_BUFFERINGTOFILE = 1, ///grabación con memoria intermedia a disco
    MO_DATASESSION_RECORD_DIRECTTOFILE = 2, ///grabación directa de claves a archivo
    MO_DATASESSION_RECORD_STREAMING = 4
};

/***

    Modos de reproducción de sesión


*/
enum moDataSessionPlaybackMode {
    MO_DATASESSION_PLAY_LIVETOCONSOLE = 0, ///Reproducción en tiempo real a consola
    MO_DATASESSION_PLAY_LIVETOVIDEO = 1, ///Reproducción en vivo a video
    MO_DATASESSION_PLAY_LIVETOSTREAM = 2, ///Reproducción en vivo por streaming
    MO_DATASESSION_PLAY_RENDERTOVIDEO = 4 ///Reproducción renderizada a video por cuadros
};



/**

Configuración de sesión. Se crea antes de empezar una sesión con los datos específicos como son:
    el directorio de datos
    el nombre del archivo de consola
    el nombre del archivo para grabar la sesión
    la cantidad máxima de claves
    la cantidad máxima de tiempo a grabar en milisegundos

*/
class LIBMOLDEO_API moDataSessionConfig {

	public:

		moDataSessionConfig();

		moDataSessionConfig(
                            moText p_apppath,
                            moText p_datapath,
                            moText p_consoleconfig,
                            moText p_SessionFileName=moText("sesion.sml"),
                            moText p_VideoFileName=moText("sesion.mpg"),
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

    moText m_AppPath;/// Directorio de ejecución de la aplicación
		moText m_DataPath;/// Directorio de datos de la sesión
		moText m_ConsoleConfigName;///archivo de definición de la consola (*.mol)
    moText m_AppDataPath;
        /*
        MO_DATASESSION_MODE_SAVETOMEMORY
        */
		moText  m_SessionFileName; ///nombre del archivo a grabar
		moText  m_VideoFileName; ///nombre del arcivo de video a grabar
		long    m_MaxKeys; ///número de eventos clave a grabar en memoria
		long    m_MaxTimecode;///tiempo máximo de grabación de claves

		bool    m_Port;///para streaming
		bool    m_Address;///para streaming

};

class LIBMOLDEO_API moDataSessionKey {

    public:

        moDataSessionKey();
        moDataSessionKey(   long p_Timecode,
                            moMoldeoActionType p_ActionType,
                            moValue& p_Value,
                            long p_MoldeoObjectId=MO_PARAM_UNDEFINED,
                            long m_ParamId=MO_PARAM_UNDEFINED,
                            long m_ValueId=MO_PARAM_UNDEFINED );
        virtual ~moDataSessionKey();

        moValue& GetValue();
        moMoldeoActionType GetActionType();

    private:

        long                  m_TimeCode; /// Valor del tick
        moMoldeoActionType    m_ActionType; /// accion

        long                  m_ObjectId; ///Opcional para identificación del índice único de objeto
        long                  m_ParamId; ///Opcional para identificación del índice único de parámetro
        long                  m_ValueId; ///Opcional para identificación del índice único de valor

        moValue               m_Value; /// Valor del dato agregado o modificado

};

class LIBMOLDEO_API moDataSessionEventKey {

    public:

        moDataSessionEventKey();
        moDataSessionEventKey(  long p_Timecode,
                                moMessage& event );

        virtual ~moDataSessionEventKey();

        long    GetTimecode();
        moMessage&    GetMessage();


    private:

        long                  m_TimeCode; /// Valor del tick
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
                    moDataSessionPlaybackMode p_playbackmode );


        bool SaveToFile( moText p_filename );
        bool LoadFromFile( moText p_filename );

        bool AddKey( moDataSessionKey* p_key );
        bool AddEventKey( moDataSessionEventKey* p_eventkey );

        bool Playback(); ///fast playback
        bool Record(); ///fast record
        bool RecordLive( moResourceManager* pRM ); /// fast live record



    private:

        moText                  m_Name;
        moDataSessionConfig*    m_pDataSessionConfig;

        moDataSessionKeys        m_Keys;
        moDataSessionEventKeys   m_EventKeys;

        moDataSessionMode       m_SessionMode;
        moDataSessionRecordMode m_SessionRecordMode;
        moDataSessionPlaybackMode m_SessionPlaybackMode;

        moVideoGraph*           m_pVideoGraph;


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

    void StartRecordingSession(  );
    void StartPlayinbackSession(  );

    moDataSession*  GetSession();

    bool Export( const moText& p_export_path , moText p_from_config_console = moText("") );
    bool IteratedExport( const moText& p_from_config_file_ );
    bool ImportFile( const moText& p_import_file_full_path  );
    bool InData( const moText& p_file_full_path  );
    moText MakeRelativeToData( const moText& p_file_full_path  );

	protected:

		moDataSessionConfig*	m_pDataSessionConfig;
		moDataSession*          m_pDataSession;


};

#endif

