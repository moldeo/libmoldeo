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
  Andr�s Colubri

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



/**

Configuraci�n de sesi�n. Se crea antes de empezar una sesi�n con los datos espec�ficos como son:
    el directorio de datos
    el nombre del archivo de consola
    el nombre del archivo para grabar la sesi�n
    la cantidad m�xima de claves
    la cantidad m�xima de tiempo a grabar en milisegundos

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

        moText m_AppPath;/// Directorio de ejecuci�n de la aplicaci�n
		moText m_DataPath;/// Directorio de datos de la sesi�n
		moText m_ConsoleConfigName;///archivo de definici�n de la consola (*.mol)

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

        long                  m_ObjectId; ///Opcional para identificaci�n del �ndice �nico de objeto
        long                  m_ParamId; ///Opcional para identificaci�n del �ndice �nico de par�metro
        long                  m_ValueId; ///Opcional para identificaci�n del �ndice �nico de valor

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

        void StartRecordingSession(  );
        void StartPlayinbackSession(  );

        moDataSession*  GetSession();

	protected:

		moDataSessionConfig*	m_pDataSessionConfig;
		moDataSession*          m_pDataSession;


};

#endif
