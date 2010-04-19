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
  Andrés Colubri

*******************************************************************************/

#ifndef __MO_MOLDEOOBJECT_H__
#define __MO_MOLDEOOBJECT_H__

#include "moTypes.h"
#include "moText.h"
#include "moAbstract.h"
#include "moScript.h"
#include "moEventList.h"
#include "moConfig.h"
#include "moArrayH.h"
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

//Manteniendo compatibilidad con los id de dispositivos de versiones de 0.6.x
//los moldeo ID's empiezan en 100...
#define MO_MOLDEOOBJECTS_OFFSET_ID	100
#define MO_MOLDEOOBJECT_UNDEFINED_ID -1

class moResourceManager;

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

        /**
         * constructor con los dos parámetros necesarios
         * @param p_paramindex
         * @param p_valueindex
         */
		moMobIndex( MOint p_paramindex, MOint p_valueindex ) { m_paramindex = p_paramindex; m_valueindex = p_valueindex; }

        void SetParamIndex( MOint p_paramindex) { m_paramindex = p_paramindex;}

        void SetValueIndex( MOint p_valueindex ) { m_valueindex = p_valueindex; }

		virtual ~moMobIndex() {}

        /**
         * observador, devuelve el índice del parámetro
         */
		MOint GetParamIndex() { return m_paramindex; }

        /**
         * observador, devuelve el índice del valor
         */
		MOint GetValueIndex() { return m_valueindex; }

	private:
		MOint	m_paramindex;
		MOint	m_valueindex;

};

/**
 * clase que define las caracteristicas basicas de un objeto Moldeo
 * los datos basicos dentro de esta definición son suficientes para poder crear un objeto moldeo
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
		moMobDefinition() {
		    m_Name = "";
		    m_ConfigName = "";
		    m_MoldeoLabelName = "";
		    m_MoldeoId = -1;
		    m_Type = MO_OBJECT_UNDEFINED;
        }
		moMobDefinition( moText p_name, moText p_configname, moMoldeoObjectType p_type, moText p_labelname = moText(""), MOint p_moldeoid = -1, moMobIndex p_MobIndex = moMobIndex(-1,-1) ) {
		    m_Name = p_name;
		    m_ConfigName = p_configname;
		    m_Type = p_type;
		    m_MoldeoLabelName = p_labelname;
		    m_MoldeoId = p_moldeoid;
		    m_MobIndex = p_MobIndex;
        }
        virtual ~moMobDefinition() {}

        moMobDefinition& operator = ( const moMobDefinition& mb) {
		    m_Name = mb.m_Name;
		    m_ConfigName = mb.m_ConfigName;
		    m_Type = mb.m_Type;
		    m_MoldeoLabelName = mb.m_MoldeoLabelName;
		    m_MoldeoId = mb.m_MoldeoId;
		    m_MobIndex = mb.m_MobIndex;
        }

        moText GetName() {
            return m_Name;
        }
        void SetName( moText p_name ) {
            m_Name = p_name;
        }
        moText GetConfigName() {
            return m_ConfigName;
        }
        void SetConfigName( moText p_configname ) {
            m_ConfigName = p_configname;
        }
        moMoldeoObjectType GetType() {
            return m_Type;
        }
        moText GetTypeStr() {
            switch(m_Type) {
                case MO_OBJECT_EFFECT:
                    return moText("moEffect");
                    break;
                case MO_OBJECT_MASTEREFFECT:
                    return moText("moMasterEffect");
                    break;
                case MO_OBJECT_POSTEFFECT:
                    return moText("moPostEffect");
                    break;
                case MO_OBJECT_PREEFFECT:
                    return moText("moPreEffect");
                    break;
                case MO_OBJECT_IODEVICE:
                    return moText("moIODevice");
                    break;
                case MO_OBJECT_RESOURCE:
                    return moText("moResource");
                    break;
                case MO_OBJECT_UNDEFINED:
                    return moText("MOB undefined");
                    break;
                default:
                    return moText("MOB undefined");
                    break;
            }
        }
        void SetType( moMoldeoObjectType p_type ) {
            m_Type = p_type;
        }
        moMobIndex& GetMobIndex() {
            return m_MobIndex;
        }


        ///este nombre es definido por el usuario
        void SetLabelName( moText p_labelname ) {
            m_MoldeoLabelName = p_labelname;
        }
        moText GetLabelName() {
            return m_MoldeoLabelName;
        }

        ///este id es definido internamente por Moldeo
        void SetMoldeoId( MOint p_moldeoid ) {
            m_MoldeoId  = p_moldeoid;
        }
        MOint GetMoldeoId() {
            return m_MoldeoId;
        }

    protected:

		MOint					m_MoldeoId;//integer unique identifier
		moText					m_MoldeoLabelName;//text unique identifier

		moMoldeoObjectType		m_Type;
		moText					m_Name;
		moText					m_ConfigName;

        moMobIndex              m_MobIndex;
};



/**
 * clase base para definir Objetos Moldeo.
 * Los Objetos Moldeo son de 6 tipos posibles.
 * @see moEffect
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
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

		virtual MOboolean LoadConnectors();//try to create inlets and outlets connectors from the config file

		virtual void	UnloadConnectors();


		virtual void	Load();

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
		MOint	GetId();

		/**
		 * función que especifica el Identificador del objeto.
		 */
		void	SetId( MOint	p_id );

		/**
		 * función que devuelve el tipo del objeto.
		 */
		moMoldeoObjectType	GetType();

		/**
		 * función que especifica el tipo del objeto.
		 */
		void	SetType( moMoldeoObjectType p_type );

		/**
		 * función que especifica el nombre del objeto.
		 */
		void	SetName( moText p_name ) { m_MobDefinition.SetName(p_name); }

		/**
		 * función que especifica el nombre identificatorio del objeto.
		 */
		void	SetLabelName( moText p_labelname ) { m_MobDefinition.SetLabelName(p_labelname); }

		/**
		 * función que especifica el nombre del objeto.
		 */
		moText	GetName() { return m_MobDefinition.GetName(); }

		/**
		 * función que devuelve el nombre identificatorio del objeto.
		 */
		moText	GetLabelName() { return m_MobDefinition.GetLabelName(); }

		/**
		 * función que devuelve el puntero a la clase de configuración del objeto.
		 */
		moConfig*	GetConfig() { return &m_Config; }

		/**
		 * función que especifica el nombre del archivo de configuración del objeto.
		 */
		void	SetConfigName( moText p_configname );

		/**
		 * función que devuelve el nombre del archivo de configuración del objeto.
		 */
		moText	GetConfigName() { return m_MobDefinition.GetConfigName(); }

		/**
		 * función que devuelve la descripción del objeto.
		 */
		moText	GetDescription() { return m_Description; }

		moMobDefinition&     GetMobDefinition() {
            return m_MobDefinition;
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
		 * función que define las funciones accesibles por scripting de esta clase particular.
		 */
		virtual int ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber) { return 0; }

		/**
		 * función que se encarga de manipular el retorno de las funciones llamadas por el script
		 */
		virtual void HandleReturns(moLuaVirtualMachine& vm, const char *strFunc) {}

	protected:

		MOboolean				m_bLoading;

		virtual void LoadDefinition();//loads the list of parameter's definitions of the MOOB MOldeo OBject

        //
        moMobDefinition         m_MobDefinition;

		//MoldeoObject data
		/*
		MOint					m_MoldeoId;//integer unique identifier
		moText					m_MoldeoLabelName;//text unique identifier

		moMoldeoObjectType		m_Type;
		moText					m_Name;
		moText					m_ConfigName;
		*/

		moText					m_Description;

		moConfig				m_Config;
		moResourceManager*		m_pResourceManager;

		//Events/messages connectors
		moOutlets				m_Outlets;
		moInlets				m_Inlets;

};

/*
template class LIBMOLDEO_API moDynamicArray<moMoldeoObject*>;
typedef moDynamicArray<moMoldeoObject*> moMoldeoObjects;
*/
moDeclareExportedDynamicArray( moMoldeoObject*, moMoldeoObjects)

#include "moResourceManager.h"

#endif
