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

class LIBMOLDEO_API moConfigDefinition : public moAbstract
{
	public:

    /// constructor
		moConfigDefinition();

		/// destructor
		virtual ~moConfigDefinition();

    /// copy constructor
    moConfigDefinition( const moConfigDefinition& p_src );

    /// assignment copy operator
    moConfigDefinition& operator =( const moConfigDefinition& p_src );

    /// Agrega la definición de un parámetro con un valor predeterminado a tomar
    /**
    *
    * @param p_name nombre del parámetro
    * @param p_type tipo del parámetro
    * @param p_index    índice del parámetro dentro del archivo de configuración (-1 si no está definido aún)
    * @param p_defaultvalue  valor predeterminado
    */
    void Add( const moText& p_name, moParamType p_type, int p_index=-1, const moValue& p_defaultvalue = moValue("INVALID",MO_VALUE_UNDEFINED), const moText& p_OptionsStr = moText("") );

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
    const moText& GetObjectName() {
        return m_ObjectName;
    }

    /// Devuelve el nombre de la clase del objeto asociado a este config
    /**
    * El nombre de la clase puede ser del objeto del que deriva este.
    * @return el nombre de la clase
    */
    const moText& GetObjectClass() {
        return m_ObjectClass;
    }

    /// Fija el indice del array con el indice del parametro...
    /**
    *
    *   Asocia en el array m_ParamIndexes, un entero (para el acceso rapido) al indice de un parametro...
    *   esta funcion es solo para facilitar el acceso a un parametro, sin necesidad de generar una busqueda a cada vez
    *   permitiendo tener configs de muchos parametros con acceso de orden 1. (no N)
    *
    *   @param defined_array_index el entero con el que se asocia el parametro
    *   @param paramindex es el indice que corresponde a la posicion del parámetro dentro del xml del archivo de configuracion
    *   @return verdadero si pudo asociarlo correctamente...
    */
    bool SetParamIndex( int defined_array_index, moParamIndex paramindex );
    bool SetIndex( moText p_name, MOint p_index);

    /// Verifica si el parametro no existe ya
    /**
    *
    *   Permite verificar si un parametro se encuentra definido
    *
    *   @param p_name el nombre del parametro
    *   @return verdadero si ya existe
    */
    bool Exists( moText p_name );

    const moParamDefinition& GetParamDefinition( const moText& p_param_name );

    ///Devuelve en formato texto la estructura del objeto convertida a JSON
    const moText& ToJSON();

	private:

		moParamDefinitions	m_ParamDefinitions;

		moParamIndexes		m_ParamIndexes;

		moText              m_ObjectName;
		moText              m_ObjectClass;

    moText              m_FullJSON;
};

/**
*
*   moDefineParamIndex >
*   Asocia el #DEFINE a su parametro correspondiente en el indice.
*/

#define moDefineParamIndex(X,Y) if ( m_Config.GetConfigDefinition()!=NULL) {\
                                    int pidx = (MOint)m_Config.GetParamIndex(Y);\
                                    if (pidx>-1) {\
                                      if (!m_Config.GetConfigDefinition()->SetParamIndex( (int)X, moParamIndex(pidx))) {\
                                        MODebug2->Warning( "<"+m_Config.GetObjectClass()+">["+m_Config.GetObjectName()+"]::Init()_moDefineParamIndex > Config parameter \"" + moText(Y) + moText("\"  warning. Bad indexation could cause errors."));\
                                      }\
                                    } else MODebug2->Error( m_Config.GetObjectClass()+": "+m_Config.GetObjectName()+ " parameter \"" + moText(Y) + moText("\" not found."));\
                                }
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
        const moText& GetObjectName() {
            return m_ConfigDefinition.GetObjectName();
        }

		/// Devuelve el nombre de la clase del objeto asociado a este config
        /**
        * El nombre de la clase puede ser del objeto del que deriva este.
        * @return el nombre de la clase
        */
        const moText& GetObjectClass() {
            return m_ConfigDefinition.GetObjectClass();
        }

        /// Devuelve el nombre del archivo de configuración
        /** @return el nombre del archivo de configuración */
        const moText& GetName() { return m_FileName; }

        /// Devuelve el puntero al arreglo de parámetros
        /** @return una referencia al arreglo de parámetros*/
        moParams&				GetParams();


        /// Indica si ha sido cargado con éxito el archivo de configuración
        /** @return verdadero si ha sido cargado el archivo, falso si no*/
        MOboolean				IsConfigLoaded();

        /// Lee la configuracion de un archivo.
        /**  @return MO_CONFIG_OK, MO_CONFIGFILE_NOT_FOUND, -1 si es otro error*/
        int						LoadConfig( moText p_filename );

        /// Corrige el archivo de configuración
        /**
        * Corrige el archivo de configuración agregando aquellos parámetros faltantes
        * TODO : 1) Que arregle problemas de formato de tipos de dato o
        * 2) versionado (core creador, director creador, plugin creador, os creador)
        * 3) convierta de ANSI a UTF
        * @return MO_CONFIG_OK, MO_CONFIGFILE_NOT_FOUND, -1 si es otro error
        */
        void						FixConfig();

        /// Indexa los parámetros
        /**
        *   Asocia indices de parametros a las definiciones.
        *
        *
        */
        void        Indexation();

        /// Devuelve true si pudo crear el archivo de configuración junto con su archivo correspondiente
        /**
        * @param
        * @return true si fue exitoso, false si no lo fue
        */
        MOboolean               CreateDefault( const moText &p_fullconfigfilename );

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


        /// Acceso rápido a un valor entero
        /**  @param indexparam índice del parámetro
        * @return el entero*/
        MOint                   Int( moParamReference p_paramreference );
        MOint                   Int( moText p_param_name );

        /// Acceso rápido a un valor double
        /**  @param indexparam índice del parámetro
        * @return el entero*/
        MOdouble                   Double( moParamReference p_paramreference );
        MOdouble                   Double( moText p_param_name );

        /// Acceso rápido a un valor de texto
        /**  @param indexparam índice del parámetro
        * @return el entero*/
        moText                   Text( moParamReference p_paramreference );
        moText                   Text( moText p_param_name );
        moText                   Text( int p_param_index );

        /// Acceso rápido a evaluar la función
        /**  @param indexparam índice del parámetro
        * @return el entero*/
        MOdouble                   Eval( moParamReference p_paramreference, double x, ... );
        MOdouble                   Eval( moParamReference p_paramreference);
        MOdouble                   Eval( moText p_param_name, double x, ... );
        MOdouble                   Eval( moText p_param_name );

        moVector4d                 EvalColor( moParamReference p_paramreference );
        moVector4d                 EvalColor( moParamReference p_paramreference, double x, ... );

        /// Acceso rápido a los datos de una textura
        /**  @param indexparam índice del parámetro
        * @return el id de la textura*/
        GLint       GetGLId( moParamReference p_paramreference, MOfloat p_cycle, MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );
        GLint       GetGLId( moParamReference p_paramreference, moTempo *p_tempo, MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );
        GLint       GetGLId( moParamReference p_paramreference, MOuint p_i, MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );
        GLint       GetGLId( moParamReference p_paramreference, MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );

        ///referencias a clases
        moMathFunction&               Fun(  moParamReference p_paramreference );
        const moFont&                 Font( moParamReference p_paramreference );
        const moTextureBuffer&        TextureBuffer(  moParamReference p_paramreference );
        const moSceneNode&            Model(  moParamReference p_paramreference );
        const moVector2d&             Vector2d( moParamReference p_paramreference );
        const moVector2i&             Vector2i( moParamReference p_paramreference );
        const moVector3d&             Vector3d( moParamReference p_paramreference );
        const moVector3i&             Vector3i( moParamReference p_paramreference );
        const moVector4d&             Vector4d( moParamReference p_paramreference );
        const moVector4i&             Vector4i( moParamReference p_paramreference );
        const moDataMessage&          Message(  moParamReference p_paramreference );
        const moDataMessages&         Messages( moParamReference p_paramreference );
        moSound&                Sound(  moParamReference p_paramreference );
        const moTexture&              Texture(  moParamReference p_paramreference );


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
        int						GetCurrentParamIndex() const;

        /// Selecciona el parámetro por el índice
        /**  @param índice del parámetro*/
        bool					SetCurrentParamIndex( int);

        bool					SetCurrentParam( const moText& p_param_name ) {
          return SetCurrentParamIndex( GetParamIndex( p_param_name ) );
        }

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
        bool					FirstValue();

        /// Selecciona el próximo valor del parámetro actual
        bool					NextValue();

        /// Selecciona el valor anterior del parámetro actual
        bool					PreviousValue();

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





        /// Agrega un valor
        void                    AddValue( int paramindex,  moValue& p_value );

        /// Inserta un valor
        void                    InsertValue( int paramindex,  int valueindex, moValue& p_value );

        /// Fija un valor
        void                    SetValue( int paramindex,  int valueindex, moValue& p_value );

        /// Borra un valor
        void                    DeleteValue( int paramindex,  int valueindex );


        /// Agrega una pre-configuración
        const moPreConfig& GetPreconfig( int valueindex );

        /// Agrega una pre-configuración
        void                    AddPreconfig( moPreconfigIndexes& p_preconfindexes );

        /// Agrega una pre-configuración y las precendentes
        void AddPreconfig( int preconfig_index);

        /// Inserta una pre-configuración
        void                    InsertPreconfig( int valueindex, moPreconfigIndexes& p_preconfindexes );

        /// Setea una pre-configuración
        void                    SetPreconfig( int valueindex, moPreconfigIndexes& p_preconfindexes );

        /// Borra una pre-configuración
        void                    DeletePreconfig( int valueindex );

        /// Devuelve la estructura del config en formato JSON
        const moText&  ToJSON();

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

    ///solo para poder devolver una referencia
    static moFont*         m_pFont;
		static moMathFunction* m_pFun;
		static moTextureBuffer* m_pTextureBuffer;
		static moSceneNode*            m_pModel;
		static moVector2d*             m_pVector2d;
		static moVector2i*             m_pVector2i;
		static moVector3d*             m_pVector3d;
		static moVector3i*             m_pVector3i;
		static moVector4d*             m_pVector4d;
		static moVector4i*             m_pVector4i;
		static moDataMessage*          m_pMessage;
		static moDataMessages*         m_pMessages;
		static moSound*                m_pSound;
		static moTexture*              m_pTexture;


    void CreateParam( moParamDefinition& p_ParamDef );

    moText        m_FullJSON;
};

#endif

