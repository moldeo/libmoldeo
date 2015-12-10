/*******************************************************************************

                                moMoldeoObject.h

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

#ifndef __MO_MOLDEOOBJECT_H__
#define __MO_MOLDEOOBJECT_H__

#include "moTypes.h"
#include "moText.h"
#include "moAbstract.h"
#include "moScript.h"
#include "moEventList.h"
#include "moConfig.h"
#include "moArray.h"
#include "moConnectors.h"

#define MO_INLET_NAME 0
#define MO_INLET_TYPE 1

#define MO_OUTLET_NAME 0
#define MO_OUTLET_TYPE 1
#define MO_OUTLET_INLETS_OFFSET 2

#define MO_IODEVICE_KEYBOARD 0
#define MO_IODEVICE_MOUSE 1
#define MO_IODEVICE_MIDI 2
#define MO_IODEVICE_MIXER 3
#define MO_IODEVICE_JOYSTICK 4
#define MO_IODEVICE_NET_TCP_IN 5
#define MO_IODEVICE_NET_UDP_IN 6
#define MO_IODEVICE_NET_TCP_OUT 7
#define MO_IODEVICE_NET_UDP_OUT 8
#define MO_IODEVICE_LIVE 9
#define MO_IODEVICE_TRACKER 10

#define MO_IODEVICE_TABLET 11
#define MO_IODEVICE_TOUCH 12
#define MO_IODEVICE_CONSOLE 20
#define MO_IODEVICE_ANY -1

///Manteniendo compatibilidad con los id de dispositivos de versiones de 0.6.x
///los moldeo ID's empiezan en 100...
#define MO_MOLDEOOBJECTS_OFFSET_ID	100
///scene objects are recursive sub-scene-fx's
#define MO_MOLDEOSCENEOBJECTS_OFFSET_ID	10000

#define MO_MOLDEOOBJECT_UNDEFINED_ID -1

#define RenderMan()   m_pResourceManager->GetRenderMan()
#define TextureMan()  m_pResourceManager->GetTextureMan()
#define DataMan() m_pResourceManager->GetDataMan()
#define VideoMan()  m_pResourceManager->GetVideoMan()
#define SoundMan()  m_pResourceManager->GetSoundMan()
#define GLMan()  m_pResourceManager->GetGLMan()


class moResourceManager;

/// Indice referente a la descripción del objeto en un archivo de configuración
/**
 * indice de un objeto dentro del archivo de configuración del proyecto .MOL (no confundir con el archivo de configuración de cada efecto)
 * contiene dos valores:
 * GetParamIndex() el indice del parámetro
 * GetValueIndex() el indice del valor correspondiente
 * de esta manera m_Config.GetParam(GetParamIndex()).GetValue(GetValueIndex()) devuelve el moValue correspondiente a la configuración del efecto
  */
class LIBMOLDEO_API moMobIndex : public moAbstract {

	public:

        /**
         * constructor genérico de la clase.
         */
		moMobIndex() { m_paramindex = -1; m_valueindex = -1; }

    moMobIndex( const moMobIndex &src) {
      (*this) = src;
    }

        /**
         * constructor con los dos parámetros necesarios
         * @param p_paramindex
         * @param p_valueindex
         */
		moMobIndex( MOint p_paramindex, MOint p_valueindex ) {
		  m_paramindex = p_paramindex; m_valueindex = p_valueindex;
    }

    moMobIndex& operator = ( const moMobIndex &src) {
       m_paramindex = src.m_paramindex;
       m_valueindex = src.m_valueindex;
       return(*this);
    }

    void SetParamIndex( MOint p_paramindex) { m_paramindex = p_paramindex;}

    void SetValueIndex( MOint p_valueindex ) { m_valueindex = p_valueindex; }

		virtual ~moMobIndex() {}

        /**
         * observador, devuelve el índice del parámetro
         */
		MOint GetParamIndex() const { return m_paramindex; }

        /**
         * observador, devuelve el índice del valor
         */
		MOint GetValueIndex() const { return m_valueindex; }

	private:
		MOint	m_paramindex;
		MOint	m_valueindex;

};

/// Clase Base Descriptiva de un Objeto Moldeo
/**
 * clase que define las caracteristicas basicas de un objeto Moldeo
 * los datos básicos dentro de esta definición son suficientes para poder crear un objeto moldeo
 * como son: el nombre del objeto, el nombre del archivo de la configuración del objeto, el tipo de objeto
 * @see moEffect
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */
class LIBMOLDEO_API moMobDefinition
{
	public:

    /// Constructor
		moMobDefinition();
    moMobDefinition(const moMobDefinition& mb);

    /// Constructor
    /**
    *   Los datos de base son
    * @param p_name nombre del objeto
    * @param p_configname nombre del archivo de configuración
    * @param p_type tipo de objeto
    * @param p_labelname etiqueta del objeto
    * @param p_moldeoid identificador único para este objeto
    * @param p_MobIndex índice referente a un archivo de configuración
    */
		moMobDefinition( const moText& p_name,
                      const moText& p_configname,
                      moMoldeoObjectType p_type,
                      const moText& p_labelname,
                      MOint p_moldeoid = -1,
                      const moMobIndex& p_MobIndex = moMobIndex(-1,-1) ) {
        m_Name = p_name;
        m_ConfigName = p_configname;
        m_Type = p_type;
        m_MoldeoLabelName = p_labelname;
        m_MoldeoId = p_moldeoid;
        m_MobIndex = p_MobIndex;
        m_MoldeoFatherId = -1;
        m_MoldeoFatherLabelName = "";
        m_Activate = true;
        m_KeyName = "";
    }

    /// Destructor
    virtual ~moMobDefinition();

    /// Operador de asignación
    moMobDefinition& operator = ( const moMobDefinition& mb);

    /// Objeto válido
    inline bool    IsValid() const {
        bool result;
        result = ( m_Type != MO_OBJECT_UNDEFINED
                && m_MoldeoId!=-1
                /*
                && m_MobIndex.GetParamIndex()!=-1
                && m_MobIndex.GetValueIndex()!=-1
                  */ );
        return result;
    }

    /// Nombre del objeto
    const moText& GetName() const;

    /// Fijar el nombre del objeto
    void SetName( const moText& p_name );

    /// Nombre del archivo de configuración
    const moText& GetConfigName() const;

    /// Fijar el nombre del archivo de configuración
    void SetConfigName( const moText& p_configname );

    /// Nombre del archivo de configuración
    moMoldeoObjectType GetType() const;

    /// Transforma una cadena de caracteres en su correspondiente moMoldeoObjectType
    moMoldeoObjectType GetStrToType( const moText& p_Str = moText("default") ) const;

    /// Transforma un moMoldeoObjectType en el nombre de su correspondiente clase base
    moText GetTypeStr() const;

    /// Transforma un moMoldeoObjectType en el nombre de su correspondiente clase base
    static moText GetTypeToClass( moMoldeoObjectType p_Type = MO_OBJECT_UNDEFINED );

    /// Transforma un moMoldeoObjectType en el nombre de su correspondiente
    static moText GetTypeToName( moMoldeoObjectType p_Type = MO_OBJECT_UNDEFINED );

    /// Fija el tipo de moMoldeoObject o moMoldeoObjectType
    void SetType( moMoldeoObjectType p_type );

    /// Devuelve la dupla de índices para el archivo de configuración
    const moMobIndex& GetMobIndex() const;

    /// Fija la etiqueta de este objeto
    void SetLabelName( const moText& p_labelname );
    /// Devuelve la etiqueta de este objeto
    const moText& GetLabelName() const;

    /// Fija el identificador de este objeto
    /**
    *   Este identificador debe ser único
    */
    void SetMoldeoId( MOint p_moldeoid );

    /// Devuelve el identificador de este objeto
    /**
    *   Este identificador debe ser único
    */
    MOint GetMoldeoId() const;

    /// Devuelve el identificador del padre de este objeto
    /**
    *   Este identificador debe ser único
    */
    MOint GetMoldeoFatherId() const {
      return m_MoldeoFatherId;
    }

    /// Fija el identificador del padre de este objeto
    /**
    *   Este identificador debe ser único
    */
    void SetMoldeoFatherId( MOint p_moldeoid ) {
        m_MoldeoFatherId  = p_moldeoid;
    }

    /// Fija la etiqueta del padre de este objeto
    void SetFatherLabelName( const moText& p_labelname );
    /// Devuelve la etiqueta del padre de este objeto
    const moText& GetFatherLabelName() const {
      return m_MoldeoFatherLabelName;
    }

    /// Fija la etiqueta del padre de este objeto
    void SetKeyName( const moText& p_keyname );
    /// Devuelve la etiqueta del padre de este objeto
    const moText& GetKeyName() const {
      return m_KeyName;
    }


    /// Devuelve al descripción del objeto
    /**
    *   La descripción describe la funcionalidad de este objeto
    */
    const moText& GetDescription() const {
      return m_Description;
    }

    /// Fija la descripción de este objeto
    /**
    *  La descripción describe la funcionalidad de este objeto
    */
    void SetDescription( const moText& p_Description );

    void SetConsoleParamIndex( MOint p_paramindex );

    void SetConsoleValueIndex(MOint p_valueindex);


    /// Devuelve el modo de activación al inicio del proyecto
    /**
    *   Si este valor es verdadero, este objeto se activará al iniciar el proyecto al que pertenece.
    */
    bool GetActivate() const {
      return m_Activate;
    }

    /// Fija el modo de activación al inicio del proyecto
    /**
    *  Fija si el objeto está activo al iniciar el proyecto.
    */
    void SetActivate( bool p_activate );


    const moText& ToJSON();

    protected:

      MOint                 m_MoldeoFatherId;
      moText					      m_MoldeoFatherLabelName;

      MOint					        m_MoldeoId; /// Identificador de objeto Moldeo
      moText					      m_MoldeoLabelName; /// Etiqueta o Identificador de texto de este objeto

      moMoldeoObjectType		m_Type; /// Tipo de Objeto
      moText					      m_Name; /// Nombre del objeto (relativo a la clase)
      moText					      m_ConfigName; /// Nombre del archivo de configuración
      moText                m_Description;/// Descripción del objeto

      moMobIndex            m_MobIndex; /// Índice referente al archivo de configuración que describe a este objeto

      moText                m_KeyName;/// nombre de la tecla que activa el objeto
      bool                  m_Activate;/// activo al iniciar el proyecto
      moText                m_FullJSON;
};

/*doesnt work with GCC 4.7.1*/
LIBMOLDEO_API moMoldeoObjectType moGetStrType( const moText& p_Str );


class LIBMOLDEO_API moMobState : public moAbstract {

    public:

        moMobState();
        virtual ~moMobState();
        moMobState( const moMobState& p_MobState );
        moMobState& operator = ( const moMobState& src);

        void Activate();
        void Deactivate();

        bool Activated() const;

        void Select();
        void Unselect();

        bool Selected()  const;
        const moText& ToJSON();

    protected:
        MOswitch m_Activated;
        MOswitch m_Selected;
        moText m_FullJSON;

};


/// Clase Base para Objetos Moldeo ( moEffect, moIODevice, moResource, moConsole )
/**
 * clase base para definir Objetos Moldeo.
 * Esta tiene como miembros principales:
 *
 * una definición de objeto moMobDefinition
 * un archivo de configuración moConfig
 * un conjunto de inlets moInlet's
 * un conjunto de outlets moOutlet's
 * una referencia a los recursos del sistema moResourceManager
 *
 * Los Objetos Moldeo son de 6 tipos posibles.
 * @see moEffect
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moConsole
 * @see moConfig
 * @see moMobDefinition
 * @see moMoldeoObjectType
 */
class LIBMOLDEO_API moMoldeoObject : public moAbstract, public moScript
{
	public:
		/**
		 * constructor genérico de la clase.
		 */
		moMoldeoObject();

		/**
		 * constructor de la clase con especificación de tipo.
		 * @param p_type tipo de Objeto Moldeo.
		 * @see moMoldeoObjectType
		 */
		moMoldeoObject( moMoldeoObjectType p_type);
		virtual ~moMoldeoObject();

		/**
		* Inicializador genérico derivado de moAbstract.
		*/
		virtual MOboolean Init();

		/**
		* Inicializador de la clase con especificación del Administrador de Recursos.
		*/
		virtual MOboolean Init( moResourceManager* p_pResources );

		/**
		* Finalizador genérico derivado de moAbstract.
		*/
		virtual MOboolean Finish();


		/**
		* Carga los parámetros con información del resourcemanager y resuelve la creación de los conectores
		* correspondientes.
		* Atención  :CreateCOnnectors siempre debe llamarse despues del Init() y asegurándose de
		* que el m_pResourceManager no sea nulo y esté inicializado.
		*/
		virtual MOboolean CreateConnectors();

		/**
		* Actualiza todos los conectores
		* Esta función vuelve a crear los conectores y sus conecciones, y evaluar todos los parámetros.
		*/
		virtual MOboolean UpdateConnectors();


    /**
		* Recarga un valor de un parámetro
		* Esta función reasigna o recarga funciones, filtros, imagenes (se carga de disco nuevamente), sonidos, y todo tipo de recursos asociados a los valores descriptos en los parámetros
		*/
		virtual MOboolean RefreshValue( moParam& param, int value_index );

		/**
		* Resuelve un valor de un parámetro
		* Esta función asigna funciones, filtros, sonidos, y todo tipo de recursos asociados a los valores descriptos en los parámetros
		*/
		virtual MOboolean ResolveValue( moParam& param, int value_index, bool p_refresh=false );


		/**
		 * método de actualización de datos del objeto.
		 * @param p_EventList puntero a la lista de eventos.
		 */
		virtual void Update( moEventList* p_EventList );//checks p_EventList for events/messages

		/**
		 * método de especificación del Administrador de Recursos (moResourceManager).
		 * @param p_pResourceManager puntero al Administrador de Recursos.
		 */
		void	SetResourceManager( moResourceManager* p_pResourceManager );//set the pointer to the resource manager for this object

		/**
		 * función que devuelve el puntero al Administrador de Recursos del objeto.
		 */
		moResourceManager* GetResourceManager();//return pointer to the resource manager of this object

		/**
		 * función que devuelve el Identificador del objeto.
		 */
		MOint	GetId() const;

		/**
		 * función que especifica el Identificador del objeto.
		 */
		void	SetId( MOint	p_id );

		/**
		 * función que devuelve el tipo del objeto.
		 */
		moMoldeoObjectType	GetType() const;

		/**
		 * función que especifica el tipo del objeto.
		 */
		void	SetType( moMoldeoObjectType p_type );

		/**
		 * función que especifica el nombre del objeto.
		 */
		void	SetName( const moText& p_name ) { m_MobDefinition.SetName(p_name); }

		/**
		 * función que especifica el nombre identificatorio del objeto.
		 */
		void	SetLabelName( const moText& p_labelname ) { m_MobDefinition.SetLabelName(p_labelname); }

		/**
		 * función que especifica la tecla que activa el objeto.
		 */
		void	SetKeyName( const moText& p_keyname ) { m_MobDefinition.SetKeyName(p_keyname); }

		const moText&	GetKeyName() const { return m_MobDefinition.GetKeyName(); }

		/**
		 * función que especifica el nombre del objeto.
		 */
		const moText&	GetName() const { return m_MobDefinition.GetName(); }

		/**
		 * función que devuelve el nombre identificatorio del objeto.
		 */
		const moText&	GetLabelName() const { return m_MobDefinition.GetLabelName(); }

		/**
		 * función que devuelve el puntero a la clase de configuración del objeto.
		 */
		moConfig*	GetConfig() { return &m_Config; }

    /**
		 * función que guarda la información de conectores y conexiones.
		 */
		void  SyncConnections();

    /**
		 * función que guarda el objeto a disco.
		 */
		virtual int Save( const moText& p_save_filename = moText("") );

		/**
		 * función que especifica el nombre del archivo de configuración del objeto.
		 */
		void	SetConfigName( const moText& p_configname );

		/**
		 * función que devuelve el nombre del archivo de configuración del objeto.
		 */
		const moText&	GetConfigName() const { return m_MobDefinition.GetConfigName(); }

       /// Devuelve al descripción del objeto
        /**
        *   La descripción describe la funcionalidad de este objeto
        */
        const moText& GetDescription() const {
            return m_MobDefinition.GetDescription();
        }


        /// Fija la descripción de este objeto
        /**
        *  La descripción describe la funcionalidad de este objeto
        */
        void SetDescription( const moText& p_Description ) {
            m_MobDefinition.SetDescription(p_Description);

        }

		const moMobDefinition&     GetMobDefinition() const {
            return m_MobDefinition;
        }

        void     SetMobDefinition( const moMobDefinition& p_MobDef ) {
            m_MobDefinition = p_MobDef;
        }


		/**
		 * función que devuelve la definición del archivo de configuración del objeto.
		 * @param p_configdefinition (opcional) puntero a la clase de definición de la configuración dónde se registran las definiciones. Si este parámetro no está definido la función toma el puntero a la definición dentro del objeto de configuración (moConfig).
		 */
		virtual moConfigDefinition * GetDefinition( moConfigDefinition *p_configdefinition = NULL );//retreive the parameters definition of the Moldeo Object

		/**
		 * función que devuelve el puntero a los Conectores de Envío de este objeto.
		 */
		moOutlets* GetOutlets();

		/**
		 * función que devuelve el puntero a los Conectores de Recepción de este objeto.
		 */
		moInlets* GetInlets();

		/**
		 * función que devuelve el indice del conector Inlet del correspondiente nombre
		 * @param p_connector_name nombre del conector (generalmente el mismo del parametro  u otro arbitrario definido por el usuario)
		 */
		MOint GetInletIndex( moText p_connector_name ) const;

		/**
		 * función que devuelve el indice del conector Outlet del correspondiente nombre
		 * @param p_connector_name nombre del conector (generalmente el mismo del parametro u otro arbitrario definido por el usuario)
		 */
		MOint GetOutletIndex( moText p_connector_name ) const;

		/**
		 * función que registra las funciones para lua en particular para esta clase y las derivadas
		 */
        virtual void RegisterFunctions();

		/**
		 * función que define las funciones accesibles por scripting de esta clase particular.
		 */
		virtual int ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber);

		/**
		 * función que se encarga de manipular el retorno de las funciones llamadas por el script
		 */
		virtual void HandleReturns(moLuaVirtualMachine& vm, const char *strFunc);


        /**
        * Devuelve el estado del objeto
        */
        virtual const moMobState& GetState() const;

        /**
        * Fija el estado del objeto
        */
        virtual bool SetState( const moMobState& p_MobState );


        virtual void Activate();

        virtual void Deactivate();

        virtual bool Activated() const;

        virtual void Select();

        virtual void Unselect();

        virtual bool Selected() const;

        virtual void SetConsoleParamIndex( MOint p_paramindex ) {
            m_MobDefinition.SetConsoleParamIndex(p_paramindex);

        }

        virtual void SetConsoleValueIndex(MOint p_valueindex) {
            m_MobDefinition.SetConsoleValueIndex(p_valueindex);
        }

        virtual const moText& ToJSON();

	protected:

        moText    m_FullJSON;

        /// \if spanish Carga las definiciones de parámetros del archivo de configuración \endif \if english Loads parameter's config definitions \endif
		virtual void LoadDefinition();

        /// Corre la funcion de script Run o Compila el nuevo script
        void ScriptExeInit();
		void ScriptExeRun();
		void ScriptExeUpdate();
		void ScriptExeFinish();

		void SetScript( const moText& p_script );


		moText  m_Script;


        /// \if spanish Definición del objeto \endif \if english Object definition \endif
        moMobDefinition         m_MobDefinition;

        /// Moldeo Object State
        moMobState              m_MobState;

        /// Configuración de parámetros del objeto
		moConfig				m_Config;

		/// Puntero al administrador de recursos
		moResourceManager*		m_pResourceManager;

		/// Conectores de salida, Arreglo de moOutlet's
		moOutlets				m_Outlets;

		/// Conectores de entrada, Arreglo de moInlet's
		moInlets				m_Inlets;

    moInlet*        InletScreenWidth;
    moInlet*        InletScreenHeight;
    moInlet*        InletTimeabs;

		MOboolean m_bConnectorsLoaded;

    int __iscript;

    private:


        /** \defgroup luascript Funciones accesibles por scripting de LUA */
        /** @{ */

        /// Función de impresión de cadena de carácteres cómoda para la depuración
        int luaPushDebugString(moLuaVirtualMachine& vm);

        /// Devuelve el objeto de moResourceManager para el manejo de recursos: texturas, videos, archivos, etc..
        int luaGetResourceManager(moLuaVirtualMachine& vm);

        /// \if spanish Fija la preconfiguración de este objeto \endif \if english Set the preconfiguration value of this object \endif
        int luaSetPreconf(moLuaVirtualMachine& vm);
        /// \if spanish Devuelve la preconfiguración de este objeto \endif \if english Get the preconfiguration value of this object \endif
        int luaGetPreconf(moLuaVirtualMachine& vm);

        /// \if spanish Devuelve el índice del parámetro \endif \if english Get the parameter index \endif
        int luaGetParamIndex(moLuaVirtualMachine& vm);
        /// \if spanish Devuelve el índice del valor \endif \if english Get the value index \endif
        int luaGetCurrentValue(moLuaVirtualMachine& vm);
        int luaSetCurrentValue(moLuaVirtualMachine& vm);

        int luaGetInletIndex(moLuaVirtualMachine& vm);
        int luaGetInletData(moLuaVirtualMachine& vm);
        int luaSetInletData(moLuaVirtualMachine& vm);

#ifdef USE_TUIO
        ///functions to access Inlets Data
        ///TUIO
        ///Tracker, etc...
        int luaGetTuioSystem(moLuaVirtualMachine& vm);
        int luaGetTuioCursorCount(moLuaVirtualMachine& vm);
        int luaGetTuioCursor(moLuaVirtualMachine& vm);
        int luaGetTuioObjectCount(moLuaVirtualMachine& vm);
        int luaGetTuioObject(moLuaVirtualMachine& vm);
#endif

        int luaGetTrackerSystemData(moLuaVirtualMachine& vm);
        int luaGetTrackerFeaturesCount(moLuaVirtualMachine& vm);
        int luaGetTrackerValidFeatures(moLuaVirtualMachine& vm);
        int luaGetTrackerFeature(moLuaVirtualMachine& vm);
        int luaGetTrackerVariance(moLuaVirtualMachine& vm);
        int luaGetTrackerBarycenter(moLuaVirtualMachine& vm);
        int luaGetTrackerAcceleration(moLuaVirtualMachine& vm);
        int luaGetTrackerVelocity(moLuaVirtualMachine& vm);
        int luaGetTrackerZone(moLuaVirtualMachine& vm);

        int luaGetTrackerHistory(moLuaVirtualMachine& vm); ///devuelve la cantidad de records registrados?!
        int luaStartTrackerHistory(moLuaVirtualMachine& vm); ///arranca la grabacion
        int luaPauseTrackerHistory(moLuaVirtualMachine& vm);
        int luaContinueTrackerHistory(moLuaVirtualMachine& vm);
        int luaStopTrackerHistory(moLuaVirtualMachine& vm);
        ///Devuelve...el baricentro
        int luaGetHistoryRecord(moLuaVirtualMachine& vm); ///va de parametro el id del tracker, el id de la instancia
        int luaGetHistoryBarycenter(moLuaVirtualMachine& vm);
        ///Devuelve las varianzas
        int luaGetHistoryVariance(moLuaVirtualMachine& vm);
        ///Devuelve los rectangulos
        int luaGetHistoryBounding(moLuaVirtualMachine& vm);
        ///Devuelve los promedios
        int luaGetHistoryAverage(moLuaVirtualMachine& vm);
        ///Devuelve el rectangulo max, min.
        int luaGetHistoryMinMax(moLuaVirtualMachine& vm);

        /** @} */


};

moDeclareExportedDynamicArray( moMoldeoObject*, moMoldeoObjects);

#include "moResourceManager.h"

#endif


