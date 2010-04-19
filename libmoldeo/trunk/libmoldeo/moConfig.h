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
  Andrés Colubri

*******************************************************************************/

#ifndef __MO_CONFIG_H__
#define __MO_CONFIG_H__

#include "moTypes.h"
#include "moText.h"
#include "moParam.h"
#include "moArrayH.h"
#include "moPreConfig.h"

#define MO_PARAM_NOT_SEL  -1
#define MO_PARAM_NOT_FOUND  -1
#define MO_CONFIGFILE_NOT_FOUND  -1
#define MO_CONFIG_OK 0
#define MO_SELECTED -1

/// definición de todos los parámetros a encontrar o a crear dentro del moConfig
/**
 *  Cumple la función de describir detalladamente que nombres de parámetros y de que tipo
 *  deben ser encontrados en el moConfig correspondiente.
 *
 *  Esta clase es rellenada por cada plugin y objeto moldeo que necesite tener controlado el formato de su archivo de configuración
 *  @see    moParamDefinition
 *  @see    moMoldeoObject
 *  @see    moConfig
 */

class LIBMOLDEO_API moConfigDefinition
{
	public:
        /// constructor
		moConfigDefinition();

		/// destructor
		virtual ~moConfigDefinition();

        /// Agrega la definición de un parámetro
        /**
        *
        * @param p_name nombre del parámetro
        * @param p_type tipo del parámetro
        * @param p_index    índice del parámetro dentro del archivo de configuración (-1 si no está definido aún)
        */
		void Add( moText p_name, moParamType p_type, MOint p_index = -1 );

        /// Agrega la definición de un parámetro con un valor predeterminado a tomar
        /**
        *
        * @param p_name nombre del parámetro
        * @param p_type tipo del parámetro
        * @param p_index    índice del parámetro dentro del archivo de configuración (-1 si no está definido aún)
        * @param p_defaultvalue  valor predeterminado
        */
		//void Add( moText p_name, moParamType p_type, MOint p_index, const moValue& p_defaultvalue );
		void Add( moText p_name, moParamType p_type, MOint p_index, moValue p_defaultvalue );

		/// Devuelve el puntero al arreglo de definciones de parámetros
        /**
        *   Este objeto permite la enumeración de las definiciones de cada parámetro
        * @return el puntero al arreglo de definiciones de parámetros moParamDefinitions
        */
		moParamDefinitions*	GetParamDefinitions() {
			return &m_ParamDefinitions;
		}

		/// Devuelve el puntero al arreglo de índices de los parámetros
        /**
        *   Este objeto permite la enumeración de los índices correspondiente a cada parámetro
        * @return el puntero al arreglo de índices de los parámetros
        */
		moParamIndexes& ParamIndexes() { return m_ParamIndexes; }

		/// Fija el nombre y la clase del objeto  a configurar
        /**
        * El nombre del objeto no especifica la clase.
        * El nombre de la clase puede ser del objeto del que deriva este.
        * @param p_objectname el nombre del objeto
        * @param p_objectclass la clase del objeto
        */
        void Set( moText p_objectname, moText p_objectclass ) {
            m_ObjectName = p_objectname;
            m_ObjectClass = p_objectclass;
        }

		/// Devuelve el nombre del objeto asociado a este config
        /**
        * El nombre del objeto no especifica la clase.
        * @return el nombre del objeto
        */
        moText GetObjectName() {
            return m_ObjectName;
        }

		/// Devuelve el nombre de la clase del objeto asociado a este config
        /**
        * El nombre de la clase puede ser del objeto del que deriva este.
        * @return el nombre de la clase
        */
        moText GetObjectClass() {
            return m_ObjectClass;
        }

	private:
		moParamDefinitions	m_ParamDefinitions;
		moParamIndexes		m_ParamIndexes;

		moText              m_ObjectName;
		moText              m_ObjectClass;

};

#define moDefineParamIndex(X,Y) if ( m_Config.GetConfigDefinition()!=NULL && X<m_Config.GetConfigDefinition()->ParamIndexes().Count() )\
                                    m_Config.GetConfigDefinition()->ParamIndexes().Set( X, moParamIndex(m_Config.GetParamIndex(Y)))


/// almacena la configuración de los parámetros de un objeto en un archivo XML
/**
 *  Es una clase de manejo de archivo de configuración basado en XML
 *
 *  ex:	Config[ PARAM_INDEX ][ VALUE_INDEX ][ SUBVALUE_INDEX ].Float()
 *
 *  Config[ ICON_COLOR ][-1][MO_RED].Float()
 */

class LIBMOLDEO_API moConfig
{
	public:

        /// Constructor
		moConfig();

        /// Destructor
		virtual	~moConfig();

		/// Fija el nombre y la clase del objeto  a configurar
        /**
        * El nombre del objeto no especifica la clase.
        * El nombre de la clase puede ser del objeto del que deriva este.
        * @param p_objectname el nombre del objeto
        * @param p_objectclass la clase del objeto
        */
        void Set( moText p_objectname, moText p_objectclass ) {
            m_ConfigDefinition.Set( p_objectname, p_objectclass);
        }

		/// Devuelve el nombre del objeto asociado a este config
        /**
        * El nombre del objeto no especifica la clase.
        * @return el nombre del objeto
        */
        moText GetObjectName() {
            return m_ConfigDefinition.GetObjectName();
        }

		/// Devuelve el nombre de la clase del objeto asociado a este config
        /**
        * El nombre de la clase puede ser del objeto del que deriva este.
        * @return el nombre de la clase
        */
        moText GetObjectClass() {
            return m_ConfigDefinition.GetObjectClass();
        }

        /// Devuelve el nombre del archivo de configuración
        /** @return el nombre del archivo de configuración */
		moText					GetName() { return m_FileName; }

        /// Devuelve el puntero al arreglo de parámetros
        /** @return una referencia al arreglo de parámetros*/
		moParams&				GetParams();


        /// Indica si ha sido cargado con éxito el archivo de configuración
        /** @return verdadero si ha sido cargado el archivo, falso si no*/
        MOboolean				IsConfigLoaded();

        /// Lee la configuracion de un archivo.
        /**  @return MO_CONFIG_OK, MO_CONFIGFILE_NOT_FOUND, -1 si es otro error*/
        int						LoadConfig( moText p_filename );

        /// Devuelve true si pudo crear el archivo de configuración junto con su archivo correspondiente
        /**
        * @param
        * @return true si fue exitoso, false si no lo fue
        */
		MOboolean               CreateDefault( moText p_fullconfigfilename );

        /// Elimina todos los parámetros anteriormente cargados
        /**
        * Limpia la configuración.
        */
        void					UnloadConfig();

        /// Guarda la configuracion en un archivo físico
        /**  @param p_filename   nombre completo del archivo incluyendo el camino
        * @return 0 si fue exitoso, otro valor en otro caso*/
        int						SaveConfig( moText p_filename = moText(""));


        /// Borra la configuracion de parámetros con sus respectivos valores
        void					DeleteConfig();

        /// Devuelve el contenido completo de la configuración como texto plano
        /**  @return el texto completo de la configuración*/
        moText					GetFullText();

        /// Devuelve la cantidad de valores que contiene el parámetro indexado
        /**  @param p_paramindex índice del parámetro
        * @return cantidad de valores*/
		MOuint						GetValuesCount( int p_paramindex );

        /// Devuelve el valor indicado por el nombre del parámetro y el índice del valor
        /**  @param nameparam nombre del parámetro
        * @param indexvalue   índice del valor dentro del parámetro
        * @return referencia al valor moValue*/
		moValue&				GetValue( moText nameparam, int indexvalue = -1 );//el valor seleccionado del parametro

        /// Devuelve el valor indicado por el índice del parámetro y el índice del valor
        /**  @param indexparam índice del parámetro
        * @param indexvalue   índice del valor dentro del parámetro
        * @return referencia al valor moValue*/
		moValue&				GetValue( int indexparam, int indexvalue = -1 );//el valor seleccionado del parametro

        /// Operador de acceso directo a un parámetro por índice
        /**  @param p_paramindex índice del parámetro
        * @return referencia al parámetro moParam*/
		moParam&				operator [] ( MOint p_paramindex ) {
			return GetParam( p_paramindex );
		}

        /// Operador de acceso directo a un parámetro por nombre
        /**  @param p_paramname nombre del parámetro
        * @return referencia al parámetro moParam*/
        moParam&				operator [] ( moText p_paramname ) {
			return GetParam( p_paramname );
		}

        /// Operador de acceso directo a un parámetro por estructura de índice (moParamIndex)
        /**  @param p_paramindex estructura de índice del parámetro
        * @return referencia al parámetro moParam*/
		moParam&				operator [] ( moParamIndex p_paramindex ) {
			return GetParam(p_paramindex.index );
		}

        /// Operador de acceso directo a un parámetro por referencia de parámetro (moParamReference)
        /**  @param p_paramreference estructura de referenciado de parámetro moParamReference
        * @return referencia al parámetro moParam*/
        moParam&				operator [] ( moParamReference p_paramreference ) {
			return GetParam( m_ConfigDefinition.ParamIndexes().Get(p_paramreference.reference) );
		}

        /// Devuelve el parámetro actualmente seleccionado
        /**  @return referencia al parámetro moParam*/
		moParam&				GetCurrentParam();

        /// Devuelve el parámetro por índice
        /**  @param p_paramindex estructura de índice del parámetro
        * @return referencia al parámetro moParam*/
		moParam&				GetParam( MOint	p_paramindex = -1 );

        /// Devuelve el parámetro por nombre
        /**  @param p_paramname nombre del parámetro
        * @return referencia al parámetro moParam*/
		moParam&				GetParam( moText	p_paramname );

        /// Devuelve el parámetro por estructura de índice (moParamIndex)
        /**  @param p_paramindex estructura de índice del parámetro
        * @return referencia al parámetro moParam*/
		moParam&				GetParam( moParamIndex p_paramindex ) {
			return GetParam(p_paramindex.index );
		}

        /// Devuelve el parámetro por referencia de parámetro (moParamReference)
        /**  @param p_paramreference estructura de referenciado de parámetro moParamReference
        * @return referencia al parámetro moParam*/
		moParam&				GetParam( moParamReference p_paramreference ) {
			return GetParam( m_ConfigDefinition.ParamIndexes().Get(p_paramreference.reference) );
		}


        /// Devuelve la cantidad de parámetros de la configuracíón
        /**  @return cantidad de parámetros*/
		int						GetParamsCount();

        /// Devuelve el índice correspondiente al valor seleccionado del parámetro por índice de parámetro
        /**  @param p_paramindex índice del parámetro
        * @return índice del valor*/
		int						GetCurrentValueIndex( MOint p_paramindex );

        /// Devuelve el índice correspondiente al parámetro por nombre
        /**  @param p_paramname nombre del parámetro
        * @return índice del parámetro*/
	    int						GetParamIndex( moText p_paramname);

        /// Devuelve el índice del parámetro actualmente seleccionado
        /**  @return índice del parámetro*/
		int						GetCurrentParamIndex();

        /// Selecciona el parámetro por el índice
        /**  @param índice del parámetro*/
        void					SetCurrentParamIndex( int);

        /// Selecciona el primer parámetro
        void					FirstParam();

        /// Selecciona el próximo parámetro
		void					NextParam();

        /// Selecciona el parámetro anterior
		void					PrevParam();

        /// Posiciona el puntero de selección del valor del parámetro a la posición indicada
        /**  @param p_paramindex índice de parámetro a posicionar el valor
        * @param p_valueindex índice de valor a posicionar*/
		void					SetCurrentValueIndex( int p_paramindex, int p_valueindex );

        /// Devuelve el valor actual del parámetro actual
        /**  @return referencia al valor moValue*/
		moValue&				GetCurrentValue();

        /// Selecciona el primer valor del parámetro actual
		void					FirstValue();

        /// Selecciona el próximo valor del parámetro actual
        void					NextValue();

        /// Selecciona el valor anterior del parámetro actual
        void					PreviousValue();

        /// Devuelve el puntero al objeto de definición de la configuración
        /**  @return puntero al objeto de definición de la configuración moConfigDefinition*/
		moConfigDefinition*		GetConfigDefinition();


        /// Devuelve la cantidad de preconfiguraciones definidas
        /**  @return cantidad de preconfiguraciones*/
		MOint					GetPreConfCount();

        /// Devuelve el índice de la preconfiguración seleccionada
        /**  @return índice de la preconfiguración*/
		MOint					GetCurrentPreConf();

        /// Posiciona la preconfiguración actual en el índice indicado
        /**  @param índice de la preconfiguración*/
		void					SetCurrentPreConf( MOint p_actual );

        /// Selecciona el primer valor de la preconfiguración
		void					PreConfFirst();

		/// Selecciona el próximo valor de la preconfiguración
		void					PreConfNext();

		/// Selecciona el valor anterior de la preconfiguración
		void					PreConfPrev();

	private:

		MOboolean				m_ConfigLoaded;
		moParams				m_Params;//los parametros del config
		moPreConfigs			m_PreConfigs;
		moConfigDefinition		m_ConfigDefinition;

		MOint					m_MajorVersion;
		MOint					m_MinorVersion;
		moText					m_FileName;

		int						m_CurrentParam;// el indice que indica cual es el parametro actual.
		MOint					m_PreconfParamNum;
		MOint					m_PreconfActual;
};


#endif
