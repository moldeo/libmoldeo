/*******************************************************************************

                                moConsole.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
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

//#define MO_DEBUG

#ifndef __MOCONSOLE_H
#define __MOCONSOLE_H


//TYPES GENERALES
#include <moTypes.h>
#include <moText.h>

//RESOURCES MANAGER
#include <moResourceManager.h>

//EFFECTS MANAGER
#include <moEffectManager.h>

//CONFIGURATIONS - STATES
#include <moConfig.h>
#include <moPresets.h>
#include <moConsoleState.h>

//IO MANAGER - PLUGINS
#include <moIODevicePlugin.h>
#include <moIODeviceManager.h>

#include <moTimeManager.h>

#define MO_MAX_EFFECT   40
#define MO_MAX_PRESETS  9
#define MO_NO_EFFECT    -1

#define MO_CFG_EFFECT   0
#define MO_CFG_EFFECT_CONFIG 1
#define MO_CFG_EFFECT_LABEL	2
#define MO_CFG_EFFECT_PRE	3
#define MO_CFG_EFFECT_ON	4
#define MO_CFG_EFFECT_KEY	5

#define MO_CFG_ACCION_STR 0
#define MO_CFG_ACCION_COD 1
#define MO_CFG_ACCION_CODDISP 2


#define MAX_SELECCIONADOS 255

#define MO_DEF_SCREEN_WIDTH	800
#define MO_DEF_SCREEN_HEIGHT 600
#define MO_DEF_RENDER_WIDTH 800
#define MO_DEF_RENDER_HEIGHT 600

#define MO_SCREEN_WIDTH 800
#define MO_SCREEN_HEIGHT 600
#define MO_SCREEN_DEPTH	32



class LIBMOLDEO_API moPresetParamDefinition {

    public:

        moPresetParamDefinition();
		moPresetParamDefinition(const moPresetParamDefinition &src);
		virtual ~moPresetParamDefinition();

		moPresetParamDefinition &operator = (const moPresetParamDefinition &src);

        MOint           m_ObjectId;
        MOint           m_ParamIndex;
        MOint           m_ValueIndex;
        moEffectState   m_State;


};

moDeclareExportedDynamicArray( moPresetParamDefinition, moPresetParams );


enum moConsoleParamIndex {
    CONSOLE_INLET,
    CONSOLE_OUTLET,
    CONSOLE_SCRIPT,

    CONSOLE_DEVICES,
    CONSOLE_PREEFFECT,
    CONSOLE_EFFECT,
    CONSOLE_POSTEFFECT,
    CONSOLE_MASTEREFFECT,
    CONSOLE_RESOURCES,
    CONSOLE_ON,
    CONSOLE_FULLDEBUG,
    CONSOLE_OUTPUTMODE,
    CONSOLE_OUTPUTRESOLUTION,
    CONSOLE_RENDERMODE,
    CONSOLE_RENDERRESOLUTION,
    CONSOLE_CLIP1,
    CONSOLE_CLIP2,
    CONSOLE_CLIP3,
    CONSOLE_PREVIEWS,
    CONSOLE_SCREENSHOTS,
    CONSOLE_VIDEOSHOTS
};

/// Objeto Consola
/**
 * <b>Consola Moldeo</b><br>
 * Este objeto implementa las funciones de inicializaci�n necesarias para la comunicaci�n
 * entre objetos de moldeo, dispositivos de entrada/salida, y recursos<br>
 * al ser a su vez un objeto moldeo, puede ser instanciada como tal<br>
 * @see moMoldeoObject
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */
class LIBMOLDEO_API moConsole : public moMoldeoObject {

    public:

        /**
         * constructor gen�rico de la clase.
         */
        moConsole();


        /**
         * constructor gen�rico de la clase.
         */
        virtual ~moConsole();


        /// Inicializador predeterminado
        /**
         * Derivado de moAbstract, inicializa con par�metros predeterminados.
         */
        virtual MOboolean Init();


        /// Inicializador param�trico.
        /**
         * Estos par�metros son especificados por la aplicaci�n de l�nea de comando.
         * @param p_datapath directorio de acceso a los datos
         * @param p_consoleconfig nombre completo incluyendo directorio del archivo de configuraci�n .mol
         * @param p_pIODeviceManager (opcional) puntero al administrador de dispositivos de entrada/salida
         * @param p_pResourceManager (opcional) puntero al administrador de recursos
         * @param p_render_to_texture_mode (opcional MO_RENDER_TO_TEXTURE_FBSCREEN ) modo de render a textura
         * @param p_screen_width (opcional MO_DEF_SCREEN_WIDTH) ancho de la pantalla de salida
         * @param p_screen_height (opcional MO_DEF_SCREEN_HEIGHT) alto de la pantalla de salida
         * @param p_render_width (opcional MO_DEF_RENDER_WIDTH) ancho de la textura de salida
         * @param p_render_height (opcional MO_DEF_RENDER_HEIGHT) alto de la textura de salida
         */

        virtual MOboolean Init(
                        moText p_apppath,
                        moText p_datapath,
						moText p_consoleconfig,
						moIODeviceManager* p_pIODeviceManager = NULL,
						moResourceManager *p_pResourceManager = NULL,
            moRenderManagerMode p_render_to_texture_mode = RENDERMANAGER_MODE_NORMAL,
					   MOint p_screen_width = MO_DEF_SCREEN_WIDTH, MOint p_screen_height = MO_DEF_SCREEN_HEIGHT,
					   MOint p_render_width = MO_DEF_RENDER_WIDTH, MOint p_render_height = MO_DEF_RENDER_HEIGHT,
					   MO_HANDLE p_OpWindowHandle  = 0,
					   MO_DISPLAY p_Display  = NULL );


        /// Procesa los eventos de los dispositivos de entrada/salida
        /**
         * Este funci�n procesa los eventos regidos por el administrador de dispositivos
         * @see moIODeviceManager
         * @see moIODevice
         */
        virtual int  Interaction();


        /// Actualiza el estado de los recursos
        /**
         * Cada objeto moResource implementa su propia funci�n de actualizaci�n de recursos.
         */
         virtual void Update();


        /// Dibuja
        /**
         * Cada objeto derivado de moEffect es dibujado seg�n la jerarqu�a que los define.
         */
        virtual void Draw();
        virtual void DrawMasterEffects( int interface_width=0, int interface_height=0 );

        /// Finalizador
        /**
         * finaliza liberando todos los recursos
         */
        virtual MOboolean Finish();


        /// inicializa el administrador de recursos
        /**
         * Crea un administrador nuevo o inicializa el predefinido.
         * @param pResourceManager
         * @param p_datapath
         * @param p_consoleconfig
         * @param p_render_to_texture_mode
         * @param p_screen_width
         * @param p_screen_height
         * @param p_render_width
         * @param p_render_height
         * @see moResourceManager
         * @see moResource
         * @see moConfig
         */

         virtual void InitResources(moResourceManager *pResourceManager,
                        moText p_apppath,
						moText p_datapath,
						moConfig& p_consoleconfig,
		               MOint p_render_to_texture_mode = MO_RENDER_TO_TEXTURE_FBSCREEN,
					   MOint p_screen_width = MO_DEF_SCREEN_WIDTH, MOint p_screen_height = MO_DEF_SCREEN_HEIGHT,
					   MOint p_render_width = MO_DEF_RENDER_WIDTH, MOint p_render_height = MO_DEF_RENDER_HEIGHT,
                       MO_HANDLE p_OpWindowHandle = 0,
                       MO_DISPLAY p_Display  = NULL);


        virtual int Save( const moText& p_save_filename = moText("") );

        /// asignaci�n del administrador de dispositivos
        /**
         * Crea el administrador de dispositivos o asigna el predeterminado.
         * @param p_IODeviceManager
         * @see moIODeviceManager
         */

        void SetIODeviceManager( moIODeviceManager*	p_IODeviceManager );

        /// asignaci�n de la definici�n del archivo de configuraci�n de la consola
        /**
         * Fija las definiciones de los par�metros de este MOB. Si no se especifica par�metro
         * lo har� dentro del archivo de configuraci�n propio.
         * @param p_configdefinition (opcional) puntero al objeto que ser� poblado con las definiciones de los par�metros de este MOB
         * @see moConfigDefinition
         * @see moMoldeoObject
         * @see moConfig
         */

        moConfigDefinition* GetDefinition( moConfigDefinition *p_configdefinition = NULL );

        /// existe ya esta etiqueta dentro de los MOBs de esta consola
        /**
        *
        * Esta funci�n devuelve verdadero o falso si existe o no la etiqueta en el
        * arreglo din�mico de m_MoldeoObjects
        *
        * @param labelname la etiqueta del objeto
        * @return verdadero si existe, falso si no
        */
        bool LabelNameExists( const moText& labelname );

        moMoldeoObjects& GetMoldeoObjects();
        moEffectManager& GetEffectManager();

        int RelativeToGeneralIndex( int relativeindex, moMoldeoObjectType p_type );


        int ProcessSessionKey( const moDataSessionKey & p_session_key );
        int ProcessSessionEventKey( const moDataSessionEventKey & p_session_event_key );

        /// enciende el reloj de la consola
        /**
        * El reloj interno de la consola arranca con este comando. Puede ser parado y pausado a su vez con
        * las funciones de ConsoleStop y ConsolePause
        */
        void ConsolePlay();

        /// inicia la reproducci�n de una sesi�n pregrabada
        /**
        * Reproduce la �ltima sesi�n grabada
        */
        void ConsolePlaySession();

        /// inicia la grabaci�n de la sesi�n actual
        /**
        * Registra la sesi�n actual en memoria luego al disco (a definir)
        */
        void ConsoleRecordSession();

        /// inicia el renderizado de una sesi�n pregrabada
        /**
        * Renderiza una sesi�n
        */
        void ConsoleRenderSession( const moText& p_frame_quality=moText("JPGGOOD") );

        /// salva la sesi�n a un archivo
        /**
        * Permite salvar la sesi�n actual grabada en un archivo XML
        */
        void ConsoleSaveSessionAs();

        /// pausa el reloj de la consola
        /**
        * El reloj interno de la consola es pausado con este comando. Puede ser parado y arrancado a su vez con
        * las funciones de ConsoleStop y ConsolePlay
        */
        void ConsolePause();

        /// para el reloj de la consola
        /**
        * El reloj interno de la consola es detenido con este comando. Puede ser pausado y arrancado a su vez con
        * las funciones de ConsolePause y ConsolePlay
        */
        void ConsoleStop();

        /// devuelve el estado del reloj de la consola
        /**
        * EL reloj de la consola puede estar en diferentes estados, definidos por moTimerState. Estos son:
        * MO_TIMERSTATE_STOPPED : detenido
        * MO_TIMERSTATE_PAUSED : en pausa
        * MO_TIMERSTATE_STARTED : arrancado
        * @return moTimerState el estado del reloj
        */
        moTimerState GetConsoleState();

        /// devuelve el modo de la consola
        /**
        * El modo de la consola define si se trata de una sesi�n en vivo, grabada, de playback o en proceso de renderizaci�n.
        * MO_CONSOLE_MODE_LIVE : predeterminado
        * MO_CONSOLE_MODE_RECORD_SESSION: graba la sesi�n actual
        * MO_CONSOLE_MODE_PLAY_SESSION: reproduce una sesi�n pregrabada
        * MO_CONSOLE_MODE_RENDER_SESSION: renderiza la sesi�n pregrabada
        *
        * @return moConsoleMode
        */
        moConsoleMode GetConsoleMode();

        void ConsoleModeUpdate();

        int SetEffectState( int m_MoldeoObjectId, const moEffectState& p_effect_state );
        int SetParam( int m_MoldeoObjectId, int m_ParamId, const moParamDefinition &p_param_definition );
        int SetValue( int m_MoldeoObjectId, int m_ParamId, int m_ValueId, const moValue &p_value );
        int RefreshValue( int m_MoldeoObjectId, int m_ParamId, int m_ValueId, bool p_Refresh=true );
        int EffectPlay( int m_MoldeoObjectId );
        int EffectPause( int m_MoldeoObjectId );
        int EffectStop( int m_MoldeoObjectId );
        int ObjectEnable( int m_MoldeoObjectId );
        int ObjectDisable( int m_MoldeoObjectId );

        /// devuelve el preset actualmente seleccionado
        /**
        *
        * Los presets son aquellas configuraciones del estado completo de los efectos de una consola.
        * Los presets se guardan en objetos independientes y cada uno contiene un estado completo, de efectos
        * activados, apagados, con sus respectivas configuraciones seleccionadas.
        * Estos presets pueden crearse en tiempo de ejecuci�n, funcionando como memoria de un estado general
        * correspondiente a la composici�n actual.
        *
        * @return int el indice del preset seleccionado
        */
        int GetPreset();

        /// fija el preset seleccionado
        /**
        *
        * Los presets son aquellas configuraciones del estado completo de los efectos de una consola.
        * Los presets se guardan en objetos independientes y cada uno contiene un estado completo, de efectos
        * activados, apagados, con sus respectivas configuraciones seleccionadas.
        * Estos presets pueden crearse en tiempo de ejecuci�n, funcionando como memoria de un estado general
        * correspondiente a la composici�n actual.
        *
        * @param presetid el indice del preset seleccionado
        */
        void SetPreset( int presetid );

        /// devuelve la configuraci�n actualmente seleccionada de un objeto de la consola
        /**
        *
        * Cada moMoldeoObject tiene su espacio de configuraciones pre-fijadas. Estas est�n definidas al final
        * del config de cada objeto, ver moConfig, moPreConfig.
        *
        * @param objectid el id �nico del objeto
        * @return int el indice del preconf seleccionado del objeto
        */
        int GetPreconf( int objectid );

        /// fija la configuraci�n actualmente seleccionada de un objeto de la consola
        /**
        *
        * Cada moMoldeoObject tiene su espacio de configuraciones pre-fijadas. Estas est�n definidas al final
        * del config de cada objeto, ver moConfig, moPreConfig.
        *
        * @param objectid el id �nico del objeto
        * @param preconfid el indice �nico del preconf a seleccionar
        */
        void SetPreconf( int objectid, int preconfid );

        void SetTicks( int ticksid );

        /**
        * Obtener un Moldeo Object por su indice
        */
        virtual moMoldeoObject* GetObjectByIdx( int p_object_id );

        /**
        * Obtener el indice de un Moldeo Object por su etiqueta (label name)
        */
        int GetObjectId( const moText& p_objectlabelname );



        int GetDirectoryFileCount( const moText& p_path );


        void ProcessConsoleMessage( moMessage* p_pMessage );


        /// Procesa los mensajes de la Moldeo API 1.0
        /**
        * Procesar los mensajes de la API
        */
        int ProcessMoldeoAPIMessage( moDataMessage* p_pDataMessage );
        int SendMoldeoAPIMessage( moDataMessage* p_pDataMessage );

        ///============================
        ///SCRIPTED in LUA
        ///============================
        void ScriptExeDraw();
        int ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber);
        void RegisterFunctions();

        const moText& ToJSON();

        int TestScreen( const moDisplay& p_display_info );

        int NewMob( const moMobDefinition &p_MobDef );
        int AddChildMob( const moMobDefinition &p_MobDef, const moMobDefinition &p_MobDefFather );
        int MoveMob( const moMobDefinition& p_MobDef, int position, MOboolean is_relative = MO_FALSE  );
        int DuplicateMob( const moMobDefinition& p_MobDef );
        int DeleteMob( const moMobDefinition& p_MobDef );

        virtual void Error( const moText &p_message );

    protected:

        /** \defgroup luascript_console Funciones espec�ficas de scripting de moConsole
        \ingroup luascript
        */
        /** @{ */

        //@{
        /** Same documentation for both members. Details */
        int luaPlay( moLuaVirtualMachine& vm );
        int luaPause( moLuaVirtualMachine& vm );
        int luaStop( moLuaVirtualMachine& vm );
        int luaState( moLuaVirtualMachine& vm );

        int luaGetPreset(moLuaVirtualMachine& vm);
        int luaSetPreset(moLuaVirtualMachine& vm);

        int luaGetTicks(moLuaVirtualMachine& vm);
        int luaSetTicks(moLuaVirtualMachine& vm);

        /**
        * Obtener y fijar par�metros de un cierto moMoldeoObject
        * Todos necesitan un parametro, id o texto para devolver un valor
        */
        int luaGetObjectId( moLuaVirtualMachine& vm );
        int luaGetObjectPreconf(moLuaVirtualMachine& vm);
        int luaSetObjectPreconf(moLuaVirtualMachine& vm);
        int luaGetObjectParamIndex(moLuaVirtualMachine& vm);
        int luaGetObjectParamValues(moLuaVirtualMachine& vm);
        int luaSetObjectCurrentValue(moLuaVirtualMachine& vm);
        int luaGetObjectCurrentValue(moLuaVirtualMachine& vm);
        ///especificos de datos
        int luaGetObjectDataIndex(moLuaVirtualMachine& vm);
        int luaGetObjectData(moLuaVirtualMachine& vm);
        int luaSetObjectData(moLuaVirtualMachine& vm);
        ///habilita deshabilita
        int luaObjectDisable(moLuaVirtualMachine& vm);
        int luaObjectEnable(moLuaVirtualMachine& vm);
        ///cambia el estado de un efecto (sus valores)
        ///tinta, velocidad, alpha, etc...
        int luaSetEffectState(moLuaVirtualMachine& vm);
        int luaGetEffectState(moLuaVirtualMachine& vm);

        /**
        *  Funciones espec�ficas del moIODeviceManager
        *  y la lista de eventos.
        */
        int luaGetDeviceCode(moLuaVirtualMachine& vm);
        int luaGetDeviceCodeId(moLuaVirtualMachine& vm);
        int luaAddEvent(moLuaVirtualMachine& vm);

        /**
        *   Funciones espec�ficas de acceso a los recursos:
        *   moTextureManager: ABM de texturas
        *   moTexture -> moTextureAnimated  -> moMovie
        *                                   -> moTextureMultiple
        *
        *   moVideoManager: ABM de recursos de c�mara
        *   moVideoBuffer
        *
        */
        int luaGetDirectoryFileCount(moLuaVirtualMachine& vm);
        int luaScreenshot(moLuaVirtualMachine& vm);

        ///
        int luaGetTextureId(moLuaVirtualMachine& vm);

        int luaAddTexture(moLuaVirtualMachine& vm);
        int luaGetTexture(moLuaVirtualMachine& vm); ///devuelve un objeto textura?
        int luaDeleteTexture(moLuaVirtualMachine& vm);

        int luaGet(moLuaVirtualMachine& vm);

        int luaEffectPlay(moLuaVirtualMachine& vm);
        int luaEffectStop(moLuaVirtualMachine& vm);
        int luaEffectPause(moLuaVirtualMachine& vm);
        int luaEffectTimerState(moLuaVirtualMachine& vm);



        //@}

        /** @} */

        virtual MOulong GetTicks();
        virtual void GLSwapBuffers();
        virtual void GUIYield();

        moConsoleState			m_ConsoleState;

        MOboolean				m_bExternalResources;

        MOboolean				m_bIODeviceManagerDefault;
        moIODeviceManager*		m_pIODeviceManager;
        moMoldeoObjects			m_MoldeoObjects;
        moMoldeoObjects			m_MoldeoSceneObjects;
        moEffectManager			m_EffectManager;
        moReactionListenerManager m_ReactionListenerManager;


        moText                  m_ConsoleScript;

        int idebug,iligia,iborrado;

        MOdouble fps_current;
        MOdouble fps_mean;
        MOuint fps_count;
        MOuint ticks, ticksprevious, tickselapsed;
        moText fps_text;
        moText  m_LastScreenshot;


        moText      m_ScreenshotPath;
        moTimer     m_ScreenshotTimer;
        long        m_ScreenshotInterval;
        moText      m_ScreenshotFormat;
        moText      m_ScreenshotResolution;

        long m_ScriptTimecodeOffset;

        void UpdateMoldeoIds();
        void LoadConnections();

        void LoadResources();
        void UnloadResources();
        void LoadIODevices();
        void UnloadIODevices();
        void AddMoldeoAPIDevices();
        void LoadMasterEffects();
        void UnloadMasterEffects();
        void LoadPreEffects();
        void UnloadPreEffects();
        void LoadEffects();
        void UnloadEffects();
        void LoadPostEffects();
        void UnloadPostEffects();

        void InitializeAllEffects();
        void FinalizeAllEffects();

        void StartMasterEffects();
        void StopMasterEffects();

        void InitPrevTexture();

        void SaveStatus(int);
        void LoadStatus(int);

        MOint ConvertKeyNameToIdx(moText& name);

        void LoadObjects( moMoldeoObjectType fx_type = MO_OBJECT_UNDEFINED );

        moPresetParams m_PresetParams;
};
#endif
