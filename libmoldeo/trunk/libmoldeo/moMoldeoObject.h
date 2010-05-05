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
  Andrs Colubri

*******************************************************************************/

#ifndef __MO_MOLDEOOBJECT_H__
#define __MO_MOLDEOOBJECT_H__

//#include "moTypes.h"
//#include "moText.h"
#include <moAbstract.h>
#include <moScript.h>
#include <moEventList.h>
#include "moConfig.h"
//#include "moArray.h"
#include <moConnectors.h>

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
#define MO_MOLDEOOBJECTS_OFFSET_ID  100
#define MO_MOLDEOOBJECT_UNDEFINED_ID -1
class moResourceManager;

/// Indice referente a la descripcin del objeto en un archivo de configuracin
/**
 * indice de un objeto dentro del archivo de configuracin del proyecto .MOL (no confundir con el archivo de configuracin de cada efecto)
 * contiene dos valores:
 * GetParamIndex() el indice del parmetro
 * GetValueIndex() el indice del valor correspondiente
 * de esta manera m_Config.GetParam(GetParamIndex()).GetValue(GetValueIndex()) devuelve el moValue correspondiente a la configuracin del efecto
 */
class LIBMOLDEO_API moMobIndex:public moAbstract
{
	public:
		/**
		 * constructor genrico de la clase.
		 */
		moMobIndex ()
		{
			m_paramindex = -1;
			m_valueindex = -1;
		}
		/**
		 * constructor con los dos parmetros necesarios
		 * @param p_paramindex
		 * @param p_valueindex
		 */
		moMobIndex (MOint p_paramindex, MOint p_valueindex)
		{
			m_paramindex = p_paramindex;
			m_valueindex = p_valueindex;
		} void SetParamIndex (MOint p_paramindex)
		{
			m_paramindex = p_paramindex;
		} void SetValueIndex (MOint p_valueindex)
		{
			m_valueindex = p_valueindex;
		} virtual ~ moMobIndex ()
		{
		}
		/**
		 * observador, devuelve el ndice del parmetro
		 */
		MOint GetParamIndex ()
		{
			return m_paramindex;
		}

		/**
		 * observador, devuelve el ndice del valor
		 */
		MOint GetValueIndex ()
		{
			return m_valueindex;
		}
	private:MOint m_paramindex;
	MOint m_valueindex;
};

/// Clase Base Descriptiva de un Objeto Moldeo
/**
 * clase que define las caracteristicas basicas de un objeto Moldeo
 * los datos bsicos dentro de esta definicin son suficientes para poder crear un objeto moldeo
 * como son: el nombre del objeto, el nombre del archivo de la configuracin del objeto, el tipo de objeto
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
		moMobDefinition ()
		{
			m_Name = moText ("");
			m_ConfigName = moText ("");
			m_MoldeoLabelName = moText ("");
			m_MoldeoId = -1;
			m_Type = MO_OBJECT_UNDEFINED;
		}

		/// Constructor
		/**
		 *   Los datos de base son
		 * @param p_name nombre del objeto
		 * @param p_configname nombre del archivo de configuracin
		 * @param p_type tipo de objeto
		 * @param p_labelname etiqueta del objeto
		 * @param p_moldeoid identificador nico para este objeto
		 * @param p_MobIndex ndice referente a un archivo de configuracin
		 */
		moMobDefinition (moText p_name, moText p_configname,
			moMoldeoObjectType p_type, moText p_labelname =
			moText (""), MOint p_moldeoid =
			-1, moMobIndex p_MobIndex = moMobIndex (-1, -1))
		{
			m_Name = p_name;
			m_ConfigName = p_configname;
			m_Type = p_type;
			m_MoldeoLabelName = p_labelname;
			m_MoldeoId = p_moldeoid;
			m_MobIndex = p_MobIndex;
		}

		/// Destructor
		virtual ~ moMobDefinition ()
		{
		}

		/// Operador de asignacin
		moMobDefinition & operator = (const moMobDefinition & mb)
		{
			m_Name = mb.m_Name;
			m_ConfigName = mb.m_ConfigName;
			m_Type = mb.m_Type;
			m_MoldeoLabelName = mb.m_MoldeoLabelName;
			m_MoldeoId = mb.m_MoldeoId;
			m_MobIndex = mb.m_MobIndex;
			return (*this);
		}

		/// Objeto vlido
		bool IsValid ()
		{
			return (m_Type != MO_OBJECT_UNDEFINED);
		}

		/// Nombre del objeto
		moText GetName ()
		{
			return m_Name;
		}

		/// Fijar el nombre del objeto
		void SetName (moText p_name)
		{
			m_Name = p_name;
		}
		/// Nombre del archivo de configuracin
		moText GetConfigName ()
		{
			return m_ConfigName;
		}

		/// Fijar el nombre del archivo de configuracin
		void SetConfigName (moText p_configname)
		{
			m_ConfigName = p_configname;
		}
		/// Nombre del archivo de configuracin
		moMoldeoObjectType GetType ()
		{
			return m_Type;
		}

		/// Transforma una cadena de caracteres en su correspondiente moMoldeoObjectType
		static moMoldeoObjectType GetStrType (moText p_Str = moText ("default"))
		{
			if (p_Str == moText ("effect"))
			{
				return MO_OBJECT_EFFECT;
			}
			else if (p_Str == moText ("mastereffect"))
			{
				return MO_OBJECT_MASTEREFFECT;
			}
			else if (p_Str == moText ("posteffect"))
			{
				return MO_OBJECT_POSTEFFECT;
			}
			else if (p_Str == moText ("preeffect"))
			{
				return MO_OBJECT_PREEFFECT;
			}
			else if (p_Str == moText ("iodevice"))
			{
				return MO_OBJECT_IODEVICE;
			}
			else if (p_Str == moText ("resource"))
			{
				return MO_OBJECT_RESOURCE;
			}
			else if (p_Str == moText ("console"))
			{
				return MO_OBJECT_CONSOLE;
			}
			return MO_OBJECT_UNDEFINED;
		}

		/// Transforma un moMoldeoObjectType en el nombre de su correspondiente clase base
		moText GetTypeStr (moMoldeoObjectType p_Type = MO_OBJECT_UNDEFINED)
		{
			if (!(p_Type == MO_OBJECT_UNDEFINED))
			{
				return moText ("MOB undefined");
			}
			switch (m_Type)
			{
				case MO_OBJECT_EFFECT:
					return moText ("moEffect");
					break;
				case MO_OBJECT_MASTEREFFECT:
					return moText ("moMasterEffect");
					break;
				case MO_OBJECT_POSTEFFECT:
					return moText ("moPostEffect");
					break;
				case MO_OBJECT_PREEFFECT:
					return moText ("moPreEffect");
					break;
				case MO_OBJECT_IODEVICE:
					return moText ("moIODevice");
					break;
				case MO_OBJECT_RESOURCE:
					return moText ("moResource");
					break;
				case MO_OBJECT_CONSOLE:
					return moText ("moConsole");
					break;
				case MO_OBJECT_UNDEFINED:
					return moText ("MOB undefined");
					break;
				default:
					return moText ("MOB undefined");
					break;
			}
		}

		/// Fija el tipo de moMoldeoObject o moMoldeoObjectType
		void SetType (moMoldeoObjectType p_type)
		{
			m_Type = p_type;
		}
		/// Devuelve la dupla de ndices para el archivo de configuracin
		moMobIndex & GetMobIndex ()
		{
			return m_MobIndex;
		}

		/// Fija la etiqueta de este objeto
		void SetLabelName (moText p_labelname)
		{
			m_MoldeoLabelName = p_labelname;
		}
		/// Devuelve la etiqueta de este objeto
		moText GetLabelName ()
		{
			return m_MoldeoLabelName;
		}

		/// Fija el identificador de este objeto
		/**
		 *   Este identificador debe ser nico
		 */
		void SetMoldeoId (MOint p_moldeoid)
		{
			m_MoldeoId = p_moldeoid;
		}
		/// Devuelve el identificador de este objeto
		/**
		 *   Este identificador debe ser nico
		 */
		MOint GetMoldeoId ()
		{
			return m_MoldeoId;
		}
	protected:MOint m_MoldeoId;	 /// Identificador de objeto Moldeo
	moText m_MoldeoLabelName;	 /// Etiqueta o Identificador de texto de este objeto
	moMoldeoObjectType m_Type;	 /// Tipo de Objeto
	moText m_Name;				 /// Nombre del objeto (relativo a la clase)
	moText m_ConfigName;		 /// Nombre del archivo de configuracin
	moMobIndex m_MobIndex;		 /// ndice referente al archivo de configuracin que describe a este objeto
};

/// Clase Base para Objetos Moldeo ( moEffect, moIODevice, moResource, moConsole )
/**
 * clase base para definir Objetos Moldeo.
 * Esta tiene como miembros principales:
 *
 * una definicin de objeto moMobDefinition
 * un archivo de configuracin moConfig
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
class LIBMOLDEO_API moMoldeoObject:public moAbstract, public moScript
{
	public:
		/**
		 * constructor genrico de la clase.
		 */
		moMoldeoObject ();

		/**
		 * constructor de la clase con especificacin de tipo.
		 * @param p_type tipo de Objeto Moldeo.
		 * @see moMoldeoObjectType
		 */
		moMoldeoObject (moMoldeoObjectType p_type);
		virtual ~ moMoldeoObject ();

		/**
		 * Inicializador genrico derivado de moAbstract.
		 */
		virtual MOboolean Init ();

		/**
		 * Inicializador de la clase con especificacin del Administrador de Recursos.
		 */
		virtual MOboolean Init (moResourceManager * p_pResources);

		/**
		 * mtodo de actualizacin de datos del objeto.
		 * @param p_EventList puntero a la lista de eventos.
		 */
								 //checks p_EventList for events/messages
		virtual void Update (moEventList * p_EventList);

		/**
		 * mtodo de especificacin del Administrador de Recursos (moResourceManager).
		 * @param p_pResourceManager puntero al Administrador de Recursos.
		 */
								 //set the pointer to the resource manager for this object
		void SetResourceManager (moResourceManager * p_pResourceManager);

		/**
		 * funcin que devuelve el puntero al Administrador de Recursos del objeto.
		 */
								 //return pointer to the resource manager of this object
		moResourceManager *GetResourceManager ();

		/**
		 * funcin que devuelve el Identificador del objeto.
		 */
		MOint GetId ();

		/**
		 * funcin que especifica el Identificador del objeto.
		 */
		void SetId (MOint p_id);

		/**
		 * funcin que devuelve el tipo del objeto.
		 */
		moMoldeoObjectType GetType ();

		/**
		 * funcin que especifica el tipo del objeto.
		 */
		void SetType (moMoldeoObjectType p_type);

		/**
		 * funcin que especifica el nombre del objeto.
		 */
		void SetName (moText p_name)
		{
			m_MobDefinition.SetName (p_name);
		}
		/**
		 * funcin que especifica el nombre identificatorio del objeto.
		 */
		void SetLabelName (moText p_labelname)
		{
			m_MobDefinition.SetLabelName (p_labelname);
		}
		/**
		 * funcin que especifica el nombre del objeto.
		 */
		moText GetName ()
		{
			return m_MobDefinition.GetName ();
		}

		/**
		 * funcin que devuelve el nombre identificatorio del objeto.
		 */
		moText GetLabelName ()
		{
			return m_MobDefinition.GetLabelName ();
		}

		/**
		 * funcin que devuelve el puntero a la clase de configuracin del objeto.
		 */
		moConfig * GetConfig ()
		{
			return &m_Config;
		}

		/**
		 * funcin que especifica el nombre del archivo de configuracin del objeto.
		 */
		void SetConfigName (moText p_configname);

		/**
		 * funcin que devuelve el nombre del archivo de configuracin del objeto.
		 */
		moText GetConfigName ()
		{
			return m_MobDefinition.GetConfigName ();
		}

		/**
		 * funcin que devuelve la descripcin del objeto.
		 */
		moText GetDescription ()
		{
			return m_Description;
		}
		moMobDefinition & GetMobDefinition ()
		{
			return m_MobDefinition;
		}

		/**
		 * funcin que devuelve la definicin del archivo de configuracin del objeto.
		 * @param p_configdefinition (opcional) puntero a la clase de definicin de la configuracin dnde se registran las definiciones. Si este parmetro no est definido la funcin toma el puntero a la definicin dentro del objeto de configuracin (moConfig).
		 */
								 //retreive the parameters definition of the Moldeo Object
		virtual moConfigDefinition * GetDefinition (moConfigDefinition * p_configdefinition = NULL);

		/**
		 * funcin que devuelve el puntero a los Conectores de Envo de este objeto.
		 */
		moOutlets *GetOutlets ();

		/**
		 * funcin que devuelve el puntero a los Conectores de Recepcin de este objeto.
		 */
		moInlets * GetInlets ();

		/**
		 * funcin que devuelve el indice del conector Inlet del correspondiente nombre
		 * @param p_connector_name nombre del conector (generalmente el mismo del parametro  u otro arbitrario definido por el usuario)
		 */
		MOint GetInletIndex (moText p_connector_name);

		/**
		 * funcin que devuelve el indice del conector Outlet del correspondiente nombre
		 * @param p_connector_name nombre del conector (generalmente el mismo del parametro u otro arbitrario definido por el usuario)
		 */
		MOint GetOutletIndex (moText p_connector_name);

		/**
		 * funcin que registra las funciones para lua en particular para esta clase y las derivadas
		 */
		virtual void RegisterFunctions ();

		/**
		 * funcin que define las funciones accesibles por scripting de esta clase particular.
		 */
		virtual int ScriptCalling (moLuaVirtualMachine & vm, int iFunctionNumber);

		/**
		 * funcin que se encarga de manipular el retorno de las funciones llamadas por el script
		 */
		virtual void HandleReturns (moLuaVirtualMachine & vm, const char *strFunc);
	protected:
		/// \if spanish Carga las definiciones de parmetros del archivo de configuracin \endif \if english Loads parameter's config definitions \endif
		virtual void LoadDefinition ();

		/// \if spanish Definicin del objeto \endif \if english Object definition \endif
		moMobDefinition m_MobDefinition;

		/// Descripcin
		moText m_Description;

		/// Configuracin de parmetros del objeto
		moConfig m_Config;

		/// Puntero al administrador de recursos
		moResourceManager * m_pResourceManager;

		/// Conectores de salida, Arreglo de moOutlet's
		moOutlets m_Outlets;

		/// Conectores de entrada, Arreglo de moInlet's
		moInlets m_Inlets;
	private:
		/** \defgroup luascript Funciones accesibles por scripting de LUA */
		/** @{ */
		/// Funcin de impresin de cadena de carcteres cmoda para la depuracin
		int luaPushDebugString (moLuaVirtualMachine & vm);

		/// \if spanish Fija la preconfiguracin de este objeto \endif \if english Set the preconfiguration value of this object \endif
		int luaSetPreconf (moLuaVirtualMachine & vm);

		/// \if spanish Devuelve la preconfiguracin de este objeto \endif \if english Get the preconfiguration value of this object \endif
		int luaGetPreconf (moLuaVirtualMachine & vm);

		/// \if spanish Devuelve el ndice del parmetro \endif \if english Get the parameter index \endif
		int luaGetParamIndex (moLuaVirtualMachine & vm);

		/// \if spanish Devuelve el ndice del valor \endif \if english Get the value index \endif
		int luaGetCurrentValue (moLuaVirtualMachine & vm);
		int luaSetCurrentValue (moLuaVirtualMachine & vm);
		int luaGetInletIndex (moLuaVirtualMachine & vm);
		int luaGetInletData (moLuaVirtualMachine & vm);
		int luaSetInletData (moLuaVirtualMachine & vm);

		///functions to access Inlets Data
		///TUIO
		///Tracker, etc...
		int luaGetTuioSystem (moLuaVirtualMachine & vm);
		int luaGetTuioCursorCount (moLuaVirtualMachine & vm);
		int luaGetTuioCursor (moLuaVirtualMachine & vm);
		int luaGetTuioObjectCount (moLuaVirtualMachine & vm);
		int luaGetTuioObject (moLuaVirtualMachine & vm);
		int luaGetTrackerSystemData (moLuaVirtualMachine & vm);
		int luaGetTrackerFeaturesCount (moLuaVirtualMachine & vm);
		int luaGetTrackerFeature (moLuaVirtualMachine & vm);
		int luaGetTrackerVariance (moLuaVirtualMachine & vm);
		int luaGetTrackerBarycenter (moLuaVirtualMachine & vm);
		int luaGetTrackerAcceleration (moLuaVirtualMachine & vm);
		int luaGetTrackerVelocity (moLuaVirtualMachine & vm);
		int luaGetTrackerZone (moLuaVirtualMachine & vm);

		/** @} */
};

moDeclareExportedDynamicArray (moMoldeoObject *, moMoldeoObjects);

#include "moResourceManager.h"
#endif							 /*  */
