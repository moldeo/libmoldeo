/*******************************************************************************

                              moValue.h

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

/*
NOTES:

	m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(  );

	m_Config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(  );

	moXmlConfig
		+moXmlParam
			+moXmlValue
				+moXmlSubValue
				+moXmlSubValue
				+moXmlSubValue
			+moXmlValue
				+moXmlSubValue
				+moXmlSubValue
		+moXmlParam
			+moXmlValue
				+moXmlSubValue
		+moXmlParam
			+moXmlValue
				+moXmlSubValue
				+moXmlSubValue
			+moXmlValue
				+moXmlSubValue

*/

#ifndef __MO_VALUE_H__
#define __MO_VALUE_H__

#include "moTypes.h"
#include "moText.h"
#include "moArrayH.h"
#include "moMathFunction.h"


#define moRound(x) ((x) < (LONG_MIN-0.5) || (x) > (LONG_MAX+0.5)) ?\
0.0f : ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

class moFont;
class mo3DModel;
class moTexture;
class moTextureFilter;
class moTextFilterParam;

/**
*   moDataType
*   Tipos de datos, especifica como está almacenada la información en memoria, y de qué tipo es.
*
*
*/

enum moDataType {
	MO_DATA_UNDEFINED,
	MO_DATA_NUMBER,
	MO_DATA_NUMBER_CHAR,
	MO_DATA_NUMBER_INT,
	MO_DATA_NUMBER_LONG,
	MO_DATA_NUMBER_DOUBLE,
	MO_DATA_NUMBER_FLOAT,
	MO_DATA_NUMBER_MIDI,
	MO_DATA_FUNCTION,
	MO_DATA_POINTER,//may be a pointer to struct or to class
	MO_DATA_VECTOR,//array of values
	MO_DATA_IMAGESAMPLE,//pointer to an imagesample pointer
	MO_DATA_IMAGESAMPLE_FILTERED,//pointer to a TextureFilter
	MO_DATA_SOUNDSAMPLE,//pointer to a soundsample pointer
	MO_DATA_VIDEOSAMPLE,//pointer to a videosample pointer
	MO_DATA_FONTPOINTER,
	MO_DATA_3DMODELPOINTER,
	MO_DATA_TEXT,//text,
	MO_DATA_MESSAGE,
	MO_DATA_MESSAGES
};

/**
 * moNumber
 * número universal
 *
 */

union moNumber {
	MOchar		m_Char;
	MOint		m_Int;
	MOlonglong	m_Long;
	MOdouble	m_Double;
	MOfloat		m_Float;
	MOpointer	m_Pointer;
};


/**
 * moData
 * dato universal
 *
 */

class moDatas;

typedef moDatas moDataMessage;

class moDataMessages;

class LIBMOLDEO_API moData {

	public:
		moData();
		moData( MOchar data );
		moData( MOint data);
		moData( MOlonglong data);
		moData( MOdouble data);
		moData( MOfloat data);
		moData( MOpointer data , MOulong size, moDataType type );
		moData( moText data );
		moData( moDataType datatype );
		virtual ~moData();

		moData& operator = ( const moData& data);
		void Copy( const moData& data );

		void		SetFloat( MOfloat pfloat );
		void		SetDouble( MOdouble pdouble );
		void		SetInt( MOint pint );
		void		SetLong( MOlonglong plong );
		void		SetChar( MOchar pchar );
		void		SetPointer( MOpointer data, MOulong size );
		void		SetText( moText ptext );
		void		SetNumber( moNumber p_number );
		void		SetType( moDataType	p_DataType  );
		void		SetSize( MOulong p_DataSize );

		/// Fija el dato a una textura
		/**
		*   Estas funciones asignan la referencia al objeto definido por el valor, a través del puntero a su objeto
		*   @{
		*/
		void		SetFun( moMathFunction*	p_Function );
		void        SetTexture( moTexture*	p_Texture );
		void        SetTextureFilter( moTextureFilter*	p_TextureFilter );
		void        SetTextureFilterAlpha( moData* p_alpha );
		void        SetTextureFilterParam( moTextFilterParam *p_filterparam );
		void        SetFont( moFont*	p_Font );
		void        SetModel( mo3DModel*    p_Model );
		void        SetMessage( moDataMessage*  p_datamessage );
		void        SetMessages( moDataMessages*  p_datamessages );
		/**
		*   @}
		*/
		//void		SetSound( moSound*	p_Sound );

		moText		Text();
		MOint		Int();
		MOlonglong	Long();
		MOfloat		Float();
		MOdouble	Double();
		MOchar		Char();
		moNumber	Number();
		MOpointer	Pointer();
		moDataType	Type();
		MOulong		Size();
		moMathFunction*	Fun();
		moFont*     Font();
		mo3DModel*  Model();
		moDataMessage*  Message();
		moDataMessages*  Messages();
		/*
		moTexture*	Texture();
		moSound*	Sound();
*/

		moData*		GetData();

		//conversion
		moText		ToText();
		moText      TypeToText();
		moDataType  TextToType( moText texttype );

		GLint       GetGLId( MOfloat p_cycle, MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );
		GLint       GetGLId( moTempo *p_tempo, MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );
		GLint       GetGLId( MOuint p_i, MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );
		GLint       GetGLId(MOfloat p_fade=1.0, moTextFilterParam *p_filterparam = NULL );

	protected:
		moDataType		m_DataType;
		moNumber		m_Number;
		moText			m_Text;
		MOulong			m_DataSize;

		bool            m_bFilteredAlpha;
		bool            m_bFilteredParams;
		MOfloat         m_AlphaFilter;
        moTextFilterParam*  m_pFilterParam;
        moData*         m_pAlphaFilter;
/*
		MOint			m_IndexValueInterpolating;//Indice del valor desde el cual se interpola;
		MOboolean		m_bInterpolated;//Marcado cuando el modo de interpolación está activado
		moData*			m_pInterpolatedData;
*/
};



moDeclareExportedDynamicArray( moData, moDatas )

moDeclareExportedDynamicArray( moDataMessage, moDataMessages )


/**
 *   moValueType
 *   Tipos de valores para interpretar los datos de texto de los archivos de configuración
 *
 *
 */

enum moValueType {
	MO_VALUE_NUM,//cualquier número
	MO_VALUE_NUM_CHAR,//un byte
	MO_VALUE_NUM_INT,//entero 32b
	MO_VALUE_NUM_LONG,//entero largo 64b
	MO_VALUE_NUM_FLOAT,//coma flotante 32b
	MO_VALUE_NUM_DOUBLE,//coma flotante precision doble
	MO_VALUE_MATRIX,//any type of VECTOR
	MO_VALUE_TXT,//any type of text, single or multiline
	MO_VALUE_LNK,//link to a file, text is interpreted as relative, absolute link to a file
	MO_VALUE_FUNCTION,//function parameter value, with lots of attributes....
	MO_VALUE_XML//text, xml formatted...
};


/**
 * moValueIndex
 * índice para localizar un valor
 *
 */

struct moValueIndex {
	MOint	m_ParamIndex;
	MOint	m_ValueIndex;
};


moDeclareExportedDynamicArray( moValueIndex, moValueIndexes )


/**
 * moValueDefinition
 * clase que define un valor, con un código de texto único, rangos y tipo
 *  Este objeto es lo que diferencia a un Dato (moData) de un Valor (moValueBase)
 */

class LIBMOLDEO_API moValueDefinition
{
	public:
		moValueDefinition();
		moValueDefinition(const moValueDefinition &src);
		virtual ~moValueDefinition();

		moValueDefinition &operator = (const moValueDefinition &src);

		void SetType( moValueType p2_type );
		void SetIndex( MOint	p_index );
		moValueType GetType();
		moText		GetTypeStr();
		MOint GetIndex();

		moText      GetCodeName();
		void        SetCodeName( moText p_codename );
		void        SetRange( MOfloat min, MOfloat max );
		void        SetRange( moText min, moText max );

		void        GetRange( MOfloat* min, MOfloat* max);

		moText      GetAttribute();
		void        SetAttribute( moText p_attribute );

	private:
		moValueType		m_Type;
		MOint			m_Index;
		moText          m_CodeName;
		MOfloat         m_Min,m_Max;

		moText          m_Attribute; //for future use

};

    /**
     * moValueBase
     * clase base para la representación de un valor, derivado de moData
     * es un dato asociado a una definición de dato
     *
     */

class LIBMOLDEO_API moValueBase : public moData
{
	public:
        /// Contructor
        /**
        */
		moValueBase();
		moValueBase(const moValueBase &src);
		virtual ~moValueBase();

		moValueBase &operator = (const moValueBase &src);

        /// Fija la definición del valor [ nombre de codigo, rango, tipo, índice ]
        /**
        *   La definición de un valor da mucha más información que el simple dato o tipo
        *   también permite acotar el dato que representa el valor
        *   @param p_valuedefinition    la definición
        */
        void SetValueDefinition( moValueDefinition& p_valuedefinition) {
            m_ValueDefinition = p_valuedefinition;
        }

        /// Devuelve una referencia a la definición del valor [ nombre de codigo, rango, tipo, índice ]
        /**
        *   La definición de un valor da mucha más información que el simple dato o tipo
        *   también permite acotar el dato que representa el valor
        *   @return la definición del valor
        */
        moValueDefinition& GetValueDefinition() {
            return m_ValueDefinition;
        }

        /// Fija el tipo de valor, esta función se implementa sólo como atajo a ciertos datos de la definición
        /**
        *   @param p_type el tipo de valor: @see moValueType
        */
		void		SetType( moValueType p_type );

        /// Devuelve el tipo de valor ,esta función se implementa sólo como atajo a ciertos datos de la definición
        /**
        *   @return la definición del valor
        */
		moValueType GetType();

        /// Fija el índice al que corresponde este valor dentro de la configuración
        /**
        *   @param p_index el índice
        */
		void		SetIndex( MOint	p_index );

        /// Devuelve el tipo en formato texto
        /**
        *
        *   @return  el texto que corresponde al tipo del valor [ MO_VALUE_TXT devuelve "TXT", MO_VALUE_FUNCTION devuelve "FUNCTION" ... ]
        */
		moText		GetTypeStr();

        /// Devuelve el índice de este valor
        /**
        *
        *   @return  el índice dentro de los subvalores
        */
		MOint		GetIndex();

        /// Devuelve un nombre de código en texto fijado por el usuario para este valor
        /**
        *
        *   @return  el nombre de código de este valor
        */
        moText      GetCodeName();

        /// Fija el nombre del código para este valor
        /**
        *
        *   @param p_codename   un texto
        */
        void        SetCodeName( moText p_codename );

	private:

        /// Este miembro es lo que diferencia un Valor (moValueBase) de un Dato ( moData ).
		moValueDefinition	m_ValueDefinition;

};


moDeclareExportedDynamicArray( moValueBase, moValueBases )


    /**
     * moValue
     * clase de manejo de una entrada de datos
     * moValue puede contener varios datos encapsulados a los que se acceden a través de las funciones:
     * GetSubValue, GetSubValueCount, y el operador []
     *
     */

class LIBMOLDEO_API moValue
{
	public:
		moValue();
		moValue( const moValue &src);
		moValue( const moText &strvalue, const moText &type );
		void AddSubValue(const  moText &strvalue, const moText &type );
		void AddSubValue(const  moValueBase &valuebase);
		void RemoveSubValue( MOint p_indexsubvalue );
		void RemoveSubValues( bool leavefirstone = true );
		virtual ~moValue();

		moValue &operator = (const moValue &src);
		moValueBase &operator [] ( MOint p_indexsubvalue ) {
			if (p_indexsubvalue!=-1) {
				return m_List.Get( p_indexsubvalue );
			} else {
				return m_List.Get( 0 );
			}
		}
		moValueBase &GetSubValue( MOint p_indexsubvalue = 0 ) {
			return m_List.Get( p_indexsubvalue );
		}
		moValueBase &GetLastSubValue() {
			return m_List.Get( GetSubValueCount() - 1 );
		}
		MOuint	GetSubValueCount() {
			return m_List.Count();
		}
		moValue& Ref() {
		    return *this;
        }

	private:
		moValueBases	m_List;

};


/*
template class LIBMOLDEO_API moDynamicArray<moValue>;
typedef  moDynamicArray<moValue> moValues;
*/

moDeclareExportedDynamicArray( moValue, moValues )

#endif /* __MO_VALUE_H__ */

