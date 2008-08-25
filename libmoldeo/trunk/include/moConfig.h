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
  Andr�s Colubri

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

/// definici�n de todos los par�metros a encontrar o a crear dentro del moConfig
/**
 *  Cumple la funci�n de describir detalladamente que nombres de par�metros y de que tipo
 *  deben ser encontrados en el moConfig correspondiente.
 *
 *  Esta clase es rellenada por cada plugin y objeto moldeo que necesite tener controlado el formato de su archivo de configuraci�n
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

        /// Agrega la definici�n de un par�metro
        /**
        *
        * @param p_name nombre del par�metro
        * @param p_type tipo del par�metro
        * @param p_index    �ndice del par�metro dentro del archivo de configuraci�n (-1 si no est� definido a�n)
        */
		void Add( moText p_name, moParamType p_type, MOint p_index = -1 );

        /// Agrega la definici�n de un par�metro con un valor predeterminado a tomar
        /**
        *
        * @param p_name nombre del par�metro
        * @param p_type tipo del par�metro
        * @param p_index    �ndice del par�metro dentro del archivo de configuraci�n (-1 si no est� definido a�n)
        * @param p_defaultvalue  valor predeterminado
        */
		//void Add( moText p_name, moParamType p_type, MOint p_index, const moValue& p_defaultvalue );
		void Add( moText p_name, moParamType p_type, MOint p_index, moValue p_defaultvalue );

		/// Devuelve el puntero al arreglo de definciones de par�metros
        /**
        *   Este objeto permite la enumeraci�n de las definiciones de cada par�metro
        * @return el puntero al arreglo de definiciones de par�metros moParamDefinitions
        */
		moParamDefinitions*	GetParamDefinitions() {
			return &m_ParamDefinitions;
		}

		/// Devuelve el puntero al arreglo de �ndices de los par�metros
        /**
        *   Este objeto permite la enumeraci�n de los �ndices correspondiente a cada par�metro
        * @return el puntero al arreglo de �ndices de los par�metros
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


/// almacena la configuraci�n de los par�metros de un objeto en un archivo XML
/**
 *  Es una clase de manejo de archivo de configuraci�n basado en XML
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

        /// Devuelve el nombre del archivo de configuraci�n
        /** @return el nombre del archivo de configuraci�n */
		moText					GetName() { return m_FileName; }

        /// Devuelve el puntero al arreglo de par�metros
        /** @return una referencia al arreglo de par�metros*/
		moParams&				GetParams();


        /// Indica si ha sido cargado con �xito el archivo de configuraci�n
        /** @return verdadero si ha sido cargado el archivo, falso si no*/
        MOboolean				IsConfigLoaded();

        /// Lee la configuracion de un archivo.
        /**  @return MO_CONFIG_OK, MO_CONFIGFILE_NOT_FOUND, -1 si es otro error*/
        int						LoadConfig( moText p_filename );

        /// Devuelve true si pudo crear el archivo de configuraci�n junto con su archivo correspondiente
        /**
        * @param
        * @return true si fue exitoso, false si no lo fue
        */
		MOboolean               CreateDefault( moText p_fullconfigfilename );

        /// Elimina todos los par�metros anteriormente cargados
        /**
        * Limpia la configuraci�n.
        */
        void					UnloadConfig();

        /// Guarda la configuracion en un archivo f�sico
        /**  @param p_filename   nombre completo del archivo incluyendo el camino
        * @return 0 si fue exitoso, otro valor en otro caso*/
        int						SaveConfig( moText p_filename = moText(""));


        /// Borra la configuracion de par�metros con sus respectivos valores
        void					DeleteConfig();

        /// Devuelve el contenido completo de la configuraci�n como texto plano
        /**  @return el texto completo de la configuraci�n*/
        moText					GetFullText();

        /// Devuelve la cantidad de valores que contiene el par�metro indexado
        /**  @param p_paramindex �ndice del par�metro
        * @return cantidad de valores*/
		MOuint						GetValuesCount( int p_paramindex );

        /// Devuelve el valor indicado por el nombre del par�metro y el �ndice del valor
        /**  @param nameparam nombre del par�metro
        * @param indexvalue   �ndice del valor dentro del par�metro
        * @return referencia al valor moValue*/
		moValue&				GetValue( moText nameparam, int indexvalue = -1 );//el valor seleccionado del parametro

        /// Devuelve el valor indicado por el �ndice del par�metro y el �ndice del valor
        /**  @param indexparam �ndice del par�metro
        * @param indexvalue   �ndice del valor dentro del par�metro
        * @return referencia al valor moValue*/
		moValue&				GetValue( int indexparam, int indexvalue = -1 );//el valor seleccionado del parametro

        /// Operador de acceso directo a un par�metro por �ndice
        /**  @param p_paramindex �ndice del par�metro
        * @return referencia al par�metro moParam*/
		moParam&				operator [] ( MOint p_paramindex ) {
			return GetParam( p_paramindex );
		}

        /// Operador de acceso directo a un par�metro por nombre
        /**  @param p_paramname nombre del par�metro
        * @return referencia al par�metro moParam*/
        moParam&				operator [] ( moText p_paramname ) {
			return GetParam( p_paramname );
		}

        /// Operador de acceso directo a un par�metro por estructura de �ndice (moParamIndex)
        /**  @param p_paramindex estructura de �ndice del par�metro
        * @return referencia al par�metro moParam*/
		moParam&				operator [] ( moParamIndex p_paramindex ) {
			return GetParam(p_paramindex.index );
		}

        /// Operador de acceso directo a un par�metro por referencia de par�metro (moParamReference)
        /**  @param p_paramreference estructura de referenciado de par�metro moParamReference
        * @return referencia al par�metro moParam*/
        moParam&				operator [] ( moParamReference p_paramreference ) {
			return GetParam( m_ConfigDefinition.ParamIndexes().Get(p_paramreference.reference) );
		}

        /// Devuelve el par�metro actualmente seleccionado
        /**  @return referencia al par�metro moParam*/
		moParam&				GetCurrentParam();

        /// Devuelve el par�metro por �ndice
        /**  @param p_paramindex estructura de �ndice del par�metro
        * @return referencia al par�metro moParam*/
		moParam&				GetParam( MOint	p_paramindex = -1 );

        /// Devuelve el par�metro por nombre
        /**  @param p_paramname nombre del par�metro
        * @return referencia al par�metro moParam*/
		moParam&				GetParam( moText	p_paramname );

        /// Devuelve el par�metro por estructura de �ndice (moParamIndex)
        /**  @param p_paramindex estructura de �ndice del par�metro
        * @return referencia al par�metro moParam*/
		moParam&				GetParam( moParamIndex p_paramindex ) {
			return GetParam(p_paramindex.index );
		}

        /// Devuelve el par�metro por referencia de par�metro (moParamReference)
        /**  @param p_paramreference estructura de referenciado de par�metro moParamReference
        * @return referencia al par�metro moParam*/
		moParam&				GetParam( moParamReference p_paramreference ) {
			return GetParam( m_ConfigDefinition.ParamIndexes().Get(p_paramreference.reference) );
		}


        /// Devuelve la cantidad de par�metros de la configurac��n
        /**  @return cantidad de par�metros*/
		int						GetParamsCount();

        /// Devuelve el �ndice correspondiente al valor seleccionado del par�metro por �ndice de par�metro
        /**  @param p_paramindex �ndice del par�metro
        * @return �ndice del valor*/
		int						GetCurrentValueIndex( MOint p_paramindex );

        /// Devuelve el �ndice correspondiente al par�metro por nombre
        /**  @param p_paramname nombre del par�metro
        * @return �ndice del par�metro*/
	    int						GetParamIndex( moText p_paramname);

        /// Devuelve el �ndice del par�metro actualmente seleccionado
        /**  @return �ndice del par�metro*/
		int						GetCurrentParamIndex();

        /// Selecciona el par�metro por el �ndice
        /**  @param �ndice del par�metro*/
        void					SetCurrentParamIndex( int);

        /// Selecciona el primer par�metro
        void					FirstParam();

        /// Selecciona el pr�ximo par�metro
		void					NextParam();

        /// Selecciona el par�metro anterior
		void					PrevParam();

        /// Posiciona el puntero de selecci�n del valor del par�metro a la posici�n indicada
        /**  @param p_paramindex �ndice de par�metro a posicionar el valor
        * @param p_valueindex �ndice de valor a posicionar*/
		void					SetCurrentValueIndex( int p_paramindex, int p_valueindex );

        /// Devuelve el valor actual del par�metro actual
        /**  @return referencia al valor moValue*/
		moValue&				GetCurrentValue();

        /// Selecciona el primer valor del par�metro actual
		void					FirstValue();

        /// Selecciona el pr�ximo valor del par�metro actual
        void					NextValue();

        /// Selecciona el valor anterior del par�metro actual
        void					PreviousValue();

        /// Devuelve el puntero al objeto de definici�n de la configuraci�n
        /**  @return puntero al objeto de definici�n de la configuraci�n moConfigDefinition*/
		moConfigDefinition*		GetConfigDefinition();


        /// Devuelve la cantidad de preconfiguraciones definidas
        /**  @return cantidad de preconfiguraciones*/
		MOint					GetPreConfCount();

        /// Devuelve el �ndice de la preconfiguraci�n seleccionada
        /**  @return �ndice de la preconfiguraci�n*/
		MOint					GetCurrentPreConf();

        /// Posiciona la preconfiguraci�n actual en el �ndice indicado
        /**  @param �ndice de la preconfiguraci�n*/
		void					SetCurrentPreConf( MOint p_actual );

        /// Selecciona el primer valor de la preconfiguraci�n
		void					PreConfFirst();

		/// Selecciona el pr�ximo valor de la preconfiguraci�n
		void					PreConfNext();

		/// Selecciona el valor anterior de la preconfiguraci�n
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
