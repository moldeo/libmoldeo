/*******************************************************************************

							   moConfig.h

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

#ifndef __MO_CONFIG_H__
#define __MO_CONFIG_H__

#include "moTypes.h"
#include "moText.h"
#include "moParam.h"
#include "moArray.h"
#include "moPreConfig.h"

#define MO_PARAM_NOT_SEL  -1
#define MO_PARAM_NOT_FOUND  -1
#define MO_CONFIGFILE_NOT_FOUND  -1
#define MO_CONFIG_OK 0
#define MO_SELECTED -1

/// definicin de todos los parmetros a encontrar o a crear dentro del moConfig
/**
 *  Cumple la funcin de describir detalladamente que nombres de parmetros y de que tipo
 *  deben ser encontrados en el moConfig correspondiente.
 *
 *  Esta clase es rellenada por cada plugin y objeto moldeo que necesite tener controlado el formato de su archivo de configuracin
 *  @see    moParamDefinition
 *  @see    moMoldeoObject
 *  @see    moConfig
 */
class LIBMOLDEO_API moConfigDefinition
{
	public:
		/// constructor
		moConfigDefinition ();

		/// destructor
		virtual ~ moConfigDefinition ();

		/// Agrega la definicin de un parmetro
		/**
		 *
		 * @param p_name nombre del parmetro
		 * @param p_type tipo del parmetro
		 * @param p_index    ndice del parmetro dentro del archivo de configuracin (-1 si no est definido an)
		 */
		void Add (moText p_name, moParamType p_type, MOint p_index = -1);

		/// Agrega la definicin de un parmetro con un valor predeterminado a tomar
		/**
		 *
		 * @param p_name nombre del parmetro
		 * @param p_type tipo del parmetro
		 * @param p_index    ndice del parmetro dentro del archivo de configuracin (-1 si no est definido an)
		 * @param p_defaultvalue  valor predeterminado
		 */
		//void Add( moText p_name, moParamType p_type, MOint p_index, const moValue& p_defaultvalue );
		void Add (moText p_name, moParamType p_type, MOint p_index,
			moValue p_defaultvalue);

		/// Devuelve el puntero al arreglo de definciones de parmetros
		/**
		 *   Este objeto permite la enumeracin de las definiciones de cada parmetro
		 * @return el puntero al arreglo de definiciones de parmetros moParamDefinitions
		 */
		moParamDefinitions * GetParamDefinitions ()
		{
			return &m_ParamDefinitions;
		}

		/// Devuelve el puntero al arreglo de ndices de los parmetros
		/**
		 *   Este objeto permite la enumeracin de los ndices correspondiente a cada parmetro
		 * @return el puntero al arreglo de ndices de los parmetros
		 */
		moParamIndexes & ParamIndexes ()
		{
			return m_ParamIndexes;
		}

		/// Fija el nombre y la clase del objeto  a configurar
		/**
		 * El nombre del objeto no especifica la clase.
		 * El nombre de la clase puede ser del objeto del que deriva este.
		 * @param p_objectname el nombre del objeto
		 * @param p_objectclass la clase del objeto
		 */
		void Set (moText p_objectname, moText p_objectclass)
		{
			m_ObjectName = p_objectname;
			m_ObjectClass = p_objectclass;
		}
		/// Devuelve el nombre del objeto asociado a este config
		/**
		 * El nombre del objeto no especifica la clase.
		 * @return el nombre del objeto
		 */
		moText GetObjectName ()
		{
			return m_ObjectName;
		}

		/// Devuelve el nombre de la clase del objeto asociado a este config
		/**
		 * El nombre de la clase puede ser del objeto del que deriva este.
		 * @return el nombre de la clase
		 */
		moText GetObjectClass ()
		{
			return m_ObjectClass;
		}
	private:
		moParamDefinitions m_ParamDefinitions;
		moParamIndexes m_ParamIndexes;
		moText m_ObjectName;
		moText m_ObjectClass;

};

#define moDefineParamIndex(X,Y) \
	if ( m_Config.GetConfigDefinition()!=NULL && \
	(MOuint)X<m_Config.GetConfigDefinition()->ParamIndexes().Count() )\
	m_Config.GetConfigDefinition ()->ParamIndexes ().Set ((int) X,\
	moParamIndex ((MOint) m_Config.GetParamIndex (Y)))
/// almacena la configuracin de los parmetros de un objeto en un archivo XML
/**
 *  Es una clase de manejo de archivo de configuracin basado en XML
 *
 *  ex:	Config[ PARAM_INDEX ][ VALUE_INDEX ][ SUBVALUE_INDEX ].Float()
 *
 *  Config[ ICON_COLOR ][-1][MO_RED].Float()
 */
class LIBMOLDEO_API moConfig
{
	public:
		/// Constructor
		moConfig ();

		/// Destructor
			virtual ~ moConfig ();

		/// Fija el nombre y la clase del objeto  a configurar
		/**
		 * El nombre del objeto no especifica la clase.
		 * El nombre de la clase puede ser del objeto del que deriva este.
		 * @param p_objectname el nombre del objeto
		 * @param p_objectclass la clase del objeto
		 */
			void Set (moText p_objectname, moText p_objectclass)
		{
			m_ConfigDefinition.Set (p_objectname, p_objectclass);
		}
		/// Devuelve el nombre del objeto asociado a este config
		/**
		 * El nombre del objeto no especifica la clase.
		 * @return el nombre del objeto
		 */
		moText GetObjectName ()
		{
			return m_ConfigDefinition.GetObjectName ();
		}

		/// Devuelve el nombre de la clase del objeto asociado a este config
		/**
		 * El nombre de la clase puede ser del objeto del que deriva este.
		 * @return el nombre de la clase
		 */
		moText GetObjectClass ()
		{
			return m_ConfigDefinition.GetObjectClass ();
		}

		/// Devuelve el nombre del archivo de configuracin
		/** @return el nombre del archivo de configuracin */
		moText GetName ()
		{
			return m_FileName;
		}

		/// Devuelve el puntero al arreglo de parmetros
		/** @return una referencia al arreglo de parmetros*/
		moParams & GetParams ();

		/// Indica si ha sido cargado con xito el archivo de configuracin
		/** @return verdadero si ha sido cargado el archivo, falso si no*/
			MOboolean IsConfigLoaded ();

		/// Lee la configuracion de un archivo.
		/**  @return MO_CONFIG_OK, MO_CONFIGFILE_NOT_FOUND, -1 si es otro error*/
			int LoadConfig (moText p_filename);

		/// Devuelve true si pudo crear el archivo de configuracin junto con su archivo correspondiente
		/**
		 * @param
		 * @return true si fue exitoso, false si no lo fue
		 */
			MOboolean CreateDefault (moText p_fullconfigfilename);

		/// Elimina todos los parmetros anteriormente cargados
		/**
		 * Limpia la configuracin.
		 */
			void UnloadConfig ();

		/// Guarda la configuracion en un archivo fsico
		/**  @param p_filename   nombre completo del archivo incluyendo el camino
		 * @return 0 si fue exitoso, otro valor en otro caso*/
			int SaveConfig (moText p_filename = moText (""));

		/// Borra la configuracion de parmetros con sus respectivos valores
			void DeleteConfig ();

		/// Devuelve el contenido completo de la configuracin como texto plano
		/**  @return el texto completo de la configuracin*/
			moText GetFullText ();

		/// Devuelve la cantidad de valores que contiene el parmetro indexado
		/**  @param p_paramindex ndice del parmetro
		 * @return cantidad de valores*/
			MOuint GetValuesCount (int p_paramindex);

		/// Devuelve el valor indicado por el nombre del parmetro y el ndice del valor
		/**  @param nameparam nombre del parmetro
		 * @param indexvalue   ndice del valor dentro del parmetro
		 * @return referencia al valor moValue*/
								 //el valor seleccionado del parametro
			moValue & GetValue (moText nameparam, int indexvalue = -1);

		/// Devuelve el valor indicado por el ndice del parmetro y el ndice del valor
		/**  @param indexparam ndice del parmetro
		 * @param indexvalue   ndice del valor dentro del parmetro
		 * @return referencia al valor moValue*/
								 //el valor seleccionado del parametro
			moValue & GetValue (int indexparam, int indexvalue = -1);

		/// Acceso rpido a un valor entero
		/**  @param indexparam ndice del parmetro
		 * @return el entero*/
			MOint Int (moParamReference p_paramreference);

		/// Acceso rpido a un valor double
		/**  @param indexparam ndice del parmetro
		 * @return el entero*/
			MOdouble Double (moParamReference p_paramreference);

		/// Acceso rpido a un valor de texto
		/**  @param indexparam ndice del parmetro
		 * @return el entero*/
			moText Text (moParamReference p_paramreference);

		/// Acceso rpido a evaluar la funcin
		/**  @param indexparam ndice del parmetro
		 * @return el entero*/
			MOdouble Eval (moParamReference p_paramreference, double x, ...);
			MOdouble Eval (moParamReference p_paramreference);

		/// Operador de acceso directo a un parmetro por ndice
		/**  @param p_paramindex ndice del parmetro
		 * @return referencia al parmetro moParam*/
			moParam & operator [](MOint p_paramindex)
		{
			return GetParam (p_paramindex);
		}

		/// Operador de acceso directo a un parmetro por nombre
		/**  @param p_paramname nombre del parmetro
		 * @return referencia al parmetro moParam*/
		moParam & operator [](moText p_paramname)
		{
			return GetParam (p_paramname);
		}

		/// Operador de acceso directo a un parmetro por estructura de ndice (moParamIndex)
		/**  @param p_paramindex estructura de ndice del parmetro
		 * @return referencia al parmetro moParam*/
		moParam & operator [](moParamIndex p_paramindex)
		{
			return GetParam (p_paramindex.index);
		}

		/// Operador de acceso directo a un parmetro por referencia de parmetro (moParamReference)
		/**  @param p_paramreference estructura de referenciado de parmetro moParamReference
		 * @return referencia al parmetro moParam*/
		moParam & operator [](moParamReference p_paramreference)
		{
			return GetParam (m_ConfigDefinition.
				ParamIndexes ().Get (p_paramreference.reference));
		}

		/// Devuelve el parmetro actualmente seleccionado
		/**  @return referencia al parmetro moParam*/
		moParam & GetCurrentParam ();

		/// Devuelve el parmetro por ndice
		/**  @param p_paramindex estructura de ndice del parmetro
		 * @return referencia al parmetro moParam*/
			moParam & GetParam (MOint p_paramindex = -1);

		/// Devuelve el parmetro por nombre
		/**  @param p_paramname nombre del parmetro
		 * @return referencia al parmetro moParam*/
			moParam & GetParam (moText p_paramname);

		/// Devuelve el parmetro por estructura de ndice (moParamIndex)
		/**  @param p_paramindex estructura de ndice del parmetro
		 * @return referencia al parmetro moParam*/
			moParam & GetParam (moParamIndex p_paramindex)
		{
			return GetParam (p_paramindex.index);
		}

		/// Devuelve el parmetro por referencia de parmetro (moParamReference)
		/**  @param p_paramreference estructura de referenciado de parmetro moParamReference
		 * @return referencia al parmetro moParam*/
		moParam & GetParam (moParamReference p_paramreference)
		{
			return GetParam (m_ConfigDefinition.
				ParamIndexes ().Get (p_paramreference.reference));
		}

		/// Devuelve la cantidad de parmetros de la configuracn
		/**  @return cantidad de parmetros*/
		int GetParamsCount ();

		/// Devuelve el ndice correspondiente al valor seleccionado del parmetro por ndice de parmetro
		/**  @param p_paramindex ndice del parmetro
		 * @return ndice del valor*/
			int GetCurrentValueIndex (MOint p_paramindex);

		/// Devuelve el ndice correspondiente al parmetro por nombre
		/**  @param p_paramname nombre del parmetro
		 * @return ndice del parmetro*/
			int GetParamIndex (moText p_paramname);

		/// Devuelve el ndice del parmetro actualmente seleccionado
		/**  @return ndice del parmetro*/
			int GetCurrentParamIndex ();

		/// Selecciona el parmetro por el ndice
		/**  @param ndice del parmetro*/
			void SetCurrentParamIndex (int);

		/// Selecciona el primer parmetro
			void FirstParam ();

		/// Selecciona el prximo parmetro
			void NextParam ();

		/// Selecciona el parmetro anterior
			void PrevParam ();

		/// Posiciona el puntero de seleccin del valor del parmetro a la posicin indicada
		/**  @param p_paramindex ndice de parmetro a posicionar el valor
		 * @param p_valueindex ndice de valor a posicionar*/
			void SetCurrentValueIndex (int p_paramindex, int p_valueindex);

		/// Devuelve el valor actual del parmetro actual
		/**  @return referencia al valor moValue*/
			moValue & GetCurrentValue ();

		/// Selecciona el primer valor del parmetro actual
			void FirstValue ();

		/// Selecciona el prximo valor del parmetro actual
			void NextValue ();

		/// Selecciona el valor anterior del parmetro actual
			void PreviousValue ();

		/// Devuelve el puntero al objeto de definicin de la configuracin
		/**  @return puntero al objeto de definicin de la configuracin moConfigDefinition*/
			moConfigDefinition *GetConfigDefinition ();

		/// Devuelve la cantidad de preconfiguraciones definidas
		/**  @return cantidad de preconfiguraciones*/
			MOint GetPreConfCount ();

		/// Devuelve el ndice de la preconfiguracin seleccionada
		/**  @return ndice de la preconfiguracin*/
			MOint GetCurrentPreConf ();

		/// Posiciona la preconfiguracin actual en el ndice indicado
		/**  @param ndice de la preconfiguracin*/
			void SetCurrentPreConf (MOint p_actual);

		/// Selecciona el primer valor de la preconfiguracin
			void PreConfFirst ();

		/// Selecciona el prximo valor de la preconfiguracin
			void PreConfNext ();

		/// Selecciona el valor anterior de la preconfiguracin
			void PreConfPrev ();

		/// Agrega un valor
			void AddValue (int paramindex, moValue & p_value);

		/// Inserta un valor
			void InsertValue (int paramindex, int valueindex, moValue & p_value);

		/// Fija un valor
			void SetValue (int paramindex, int valueindex, moValue & p_value);

		/// Borra un valor
			void DeleteValue (int paramindex, int valueindex);

		/// Agrega una pre-configuracin
			void AddPreconfig (moPreconfigIndexes & p_preconfindexes);

		/// Agrega una pre-configuracin
			void InsertPreconfig (int valueindex,
			moPreconfigIndexes & p_preconfindexes);

		/// Agrega una pre-configuracin
			void SetPreconfig (int valueindex,
			moPreconfigIndexes & p_preconfindexes);

		/// Borra una pre-configuracin
			void DeletePreconfig (int valueindex);
		private:

		MOboolean m_ConfigLoaded;
			moParams m_Params;	 //los parametros del config
			moPreConfigs m_PreConfigs;
			moConfigDefinition m_ConfigDefinition;

			MOint m_MajorVersion;
			MOint m_MinorVersion;
			moText m_FileName;

			int m_CurrentParam;	 // el indice que indica cual es el parametro actual.
			MOint m_PreconfParamNum;
			MOint m_PreconfActual;
};
#endif
