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
#include "moTempo.h"
#include "moText.h"
#include "moArray.h"
#include "moMathMatrix.h"
#include "moMathFunction.h"
#include "moMathVector.h"
#include "moMathVector3.h"
#include "moMathVector4.h"
#include "moTextFilterParam.h"

#include <iostream>
#include <map>
#include <string>
using namespace std;

#define moRound(x) ((x) < (LONG_MIN-0.5) || (x) > (LONG_MAX+0.5)) ?\
0.0f : ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

class moFont;
class moSceneNode;
class moTexture;
class moTextureFilter;
class moTextureBuffer;
class moVideoBuffer;
class moTextFilterParam;
class moSound;

/**
*   moDataType
*   Tipos de datos, especifica como est� almacenada la informaci�n en memoria, y de qu� tipo es.
*
*
*/

enum moDataType {
	MO_DATA_UNDEFINED=-1,
	MO_DATA_NUMBER,
	MO_DATA_NUMBER_CHAR,
	MO_DATA_NUMBER_INT,
	MO_DATA_NUMBER_LONG,
	MO_DATA_NUMBER_DOUBLE,
	MO_DATA_NUMBER_FLOAT,
	MO_DATA_NUMBER_MIDI,
	MO_DATA_FUNCTION,
	MO_DATA_POINTER,//may be a pointer to struct or to class
	MO_DATA_VECTOR2I,//array of values
	MO_DATA_VECTOR3I,//array of values
	MO_DATA_VECTOR4I,//array of values
	MO_DATA_VECTOR2F,//array of values
	MO_DATA_VECTOR3F,//array of values
	MO_DATA_VECTOR4F,//array of values
	MO_DATA_IMAGESAMPLE,//pointer to an imagesample pointer
	MO_DATA_IMAGESAMPLE_FILTERED,//pointer to a TextureFilter
  MO_DATA_IMAGESAMPLE_TEXTUREBUFFER,//pointer to a texturebuffer pointer
	MO_DATA_SOUNDSAMPLE,//pointer to a soundsample pointer
	MO_DATA_VIDEOSAMPLE,//pointer to a videosample pointer: video buffer
	MO_DATA_FONTPOINTER,
	MO_DATA_3DMODELPOINTER,
	MO_DATA_TEXT,//text,
	MO_DATA_MESSAGE,
	MO_DATA_MESSAGES
};

/**
 * moNumber
 * n�mero universal
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

		bool        IsValid() const;

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
		void		SetFun( const moText& p_functionExpression );

		/// Fija el dato a una textura
		/**
		*   Estas funciones asignan la referencia al objeto definido por el valor, a trav�s del puntero a su objeto
		*   @{
		*/
		void        SetFun( moMathFunction*	p_Function );
		void        SetTexture( moTexture*	p_Texture );
		void        SetTextureBuffer( moTextureBuffer*	p_TextureBuffer );
		void        SetVideoBuffer( moVideoBuffer*	p_VideoBuffer );
		void        SetTextureFilter( moTextureFilter*	p_TextureFilter );
		void        SetTextureFilterAlpha( moData* p_alpha );
		void        SetTextureFilterParam( const moTextFilterParam &p_filterparam );
		void        SetFont( moFont*	p_Font );
		void        SetModel( moSceneNode*    p_Model );
		void        SetSound( moSound*	p_Sound );
		void        SetVector( moVector2d *p_vector2d );
		void        SetVector( moVector3d *p_vector3d );
		void        SetVector( moVector4d *p_vector4d );
		void        SetVector( moVector2i *p_vector2i );
		void        SetVector( moVector3i *p_vector3i );
		void        SetVector( moVector4i *p_vector4i );

		void        SetMessage( moDataMessage*  p_datamessage );
		void        SetMessages( moDataMessages*  p_datamessages );
		/**
		*   @}
		*/


		moText                  Text();
		MOint                   Int() const;
		MOlonglong              Long() const;
		MOfloat                 Float() const;
		MOdouble                Double() const;
		MOchar                  Char() const;
		moNumber                Number();
		MOpointer               Pointer();
		moDataType              Type() const;
		MOulong                 Size() const;
		MOdouble                Eval();
		MOdouble                Eval( double x );
		MOdouble                LastEval() const;

		///referencias a clases
		moMathFunction*         Fun();
		moFont*                 Font();
		moTextureBuffer*        TextureBuffer();
		moSceneNode*            Model();
		moVector2d*             Vector2d();
		moVector2i*             Vector2i();
		moVector3d*             Vector3d();
		moVector3i*             Vector3i();
		moVector4d*             Vector4d();
		moVector4i*             Vector4i();
		moDataMessage*          Message();
		moDataMessages*         Messages();
		moSound*                Sound();
		moTexture*              Texture();
		moTexture*              TextureDestination();

		moData*                 GetData();

		//conversion
		moText                  ToText() const;
		static moText           TypeToText( moDataType p_data_type );
		moText                  TypeToText();
		moDataType              TextToType( moText texttype );


		GLint       GetGLId( MOfloat p_cycle, MOfloat p_fade, const moTextFilterParam &p_filterparam);
		GLint       GetGLId( moTempo *p_tempo, MOfloat p_fade, const moTextFilterParam &p_filterparam );
		GLint       GetGLId( MOuint p_i, MOfloat p_fade, const moTextFilterParam &p_filterparam );
		GLint       GetGLId( MOfloat p_fade, const moTextFilterParam &p_filterparam );
		GLint       GetGLId( moMoldeoObject* p_mob, MOfloat p_fade, const moTextFilterParam &p_filterparam );
    GLint       GetGLId( moMoldeoObject* p_mob, moTempo *p_tempo, MOfloat p_fade, const moTextFilterParam &p_filterparam );
    GLint       GetGLId( MOfloat p_cycle);
    GLint       GetGLId( moTempo *p_tempo );
    GLint       GetGLId( MOuint p_i);
    GLint       GetGLId();
    GLint       GetGLId( moMoldeoObject* p_mob );
    GLint       GetGLId( moMoldeoObject* p_mob, moTempo *p_tempo );


	protected:
		moDataType		m_DataType;
		moNumber		m_Number;
		moText			m_Text;
		MOulong			m_DataSize;


		bool            m_bFilteredAlpha;
		bool            m_bFilteredParams;
		MOfloat         m_AlphaFilter;
		moTextFilterParam  m_pFilterParam;
		moData*         m_pAlphaFilter;

		MOdouble        m_LastEval;

};



moDeclareExportedDynamicArray( moData, moDatas );
moDeclareExportedDynamicArray( moDataMessage, moDataMessages );
typedef std::map< std::string, moData > moAttributes;


/**
 *   moValueType
 *   Tipos de valores para interpretar los datos de texto de los archivos de configuraci�n
 *
 *
 */

enum moValueType {
	MO_VALUE_NUM,//cualquier n�mero
	MO_VALUE_NUM_CHAR,//un byte
	MO_VALUE_NUM_INT,//entero 32b
	MO_VALUE_NUM_LONG,//entero largo 64b
	MO_VALUE_NUM_FLOAT,//coma flotante 32b
	MO_VALUE_NUM_DOUBLE,//coma flotante precision doble
	MO_VALUE_MATRIX,//any type of VECTOR
	MO_VALUE_TXT,//any type of text, single or multiline
	MO_VALUE_LNK,//link to a file, text is interpreted as relative, absolute link to a file
	MO_VALUE_FUNCTION,//function parameter value, with lots of attributes....
	MO_VALUE_XML,//text, xml formatted...
	MO_VALUE_UNDEFINED
};


/**
 * moValueIndex
 * �ndice para localizar un valor
 *
 */

struct moValueIndex {
	MOint	m_ParamIndex;
	MOint	m_ValueIndex;
};


moDeclareExportedDynamicArray( moValueIndex, moValueIndexes );


/**
 * moValueDefinition
 * clase que define un valor, con un c�digo de texto �nico, rangos y tipo
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
		moValueType GetType()  const;
		static moValueType ValueTypeFromStr( const moText& p_value_type_str );
		moText		GetTypeStr()  const;
		MOint GetIndex()  const;

		moText      GetCodeName() const;
		void        SetCodeName( moText p_codename );
		void        SetRange( MOfloat min, MOfloat max );
		void        SetRange( moText min, moText max );

		void        GetRange( MOfloat* min, MOfloat* max);

		moText      GetAttribute()  const;
		void        SetAttribute( moText p_attribute );

		bool        IsValid()  const;
		int         Set( const moText& p_XmlText );
    const moText& ToJSON();
    const moText& ToXML();

	private:
		moValueType		m_Type;
		MOint			m_Index;
		moText          m_CodeName;
		MOfloat         m_Min,m_Max;

		moText          m_Attribute; //for future use
		moText          m_FullJSON;
		moText          m_FullXML;

};

    /**
     * moValueBase
     * clase base para la representaci�n de un valor, derivado de moData
     * es un dato asociado a una definici�n de dato
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

        /// Fija la definici�n del valor [ nombre de codigo, rango, tipo, �ndice ]
        /**
        *   La definici�n de un valor da mucha m�s informaci�n que el simple dato o tipo
        *   tambi�n permite acotar el dato que representa el valor
        *   @param p_valuedefinition    la definici�n
        */
        void SetValueDefinition( moValueDefinition& p_valuedefinition) {
            m_ValueDefinition = p_valuedefinition;
        }

        /// Devuelve una referencia a la definici�n del valor [ nombre de codigo, rango, tipo, �ndice ]
        /**
        *   La definici�n de un valor da mucha m�s informaci�n que el simple dato o tipo
        *   tambi�n permite acotar el dato que representa el valor
        *   @return la definici�n del valor
        */
        const moValueDefinition& GetValueDefinition() const {
            return m_ValueDefinition;
        }

        /// Fija el tipo de valor, esta funci�n se implementa s�lo como atajo a ciertos datos de la definici�n
        /**
        *   @param p_type el tipo de valor: @see moValueType
        */
		void		SetType( moValueType p_type );

        /// Devuelve el tipo de valor ,esta funci�n se implementa s�lo como atajo a ciertos datos de la definici�n
        /**
        *   @return la definici�n del valor
        */
		moValueType GetType()  const;

        /// Fija el �ndice al que corresponde este valor dentro de la configuraci�n
        /**
        *   @param p_index el �ndice
        */
		void		SetIndex( MOint	p_index );

        /// Devuelve el tipo en formato texto
        /**
        *
        *   @return  el texto que corresponde al tipo del valor [ MO_VALUE_TXT devuelve "TXT", MO_VALUE_FUNCTION devuelve "FUNCTION" ... ]
        */
		moText		GetTypeStr()  const;

        /// Devuelve el �ndice de este valor
        /**
        *
        *   @return  el �ndice dentro de los subvalores
        */
		MOint		GetIndex()  const;

        /// Devuelve un nombre de c�digo en texto fijado por el usuario para este valor
        /**
        *
        *   @return  el nombre de c�digo de este valor
        */
        moText      GetCodeName()  const;

        /// Fija el nombre del c�digo para este valor
        /**
        *
        *   @param p_codename   un texto
        */
        void        SetCodeName( moText p_codename );


		void        SetRange( MOfloat min, MOfloat max );
		void        SetRange( moText min, moText max );

		void        GetRange( MOfloat* min, MOfloat* max);

		moText      GetAttribute()  const;
		void        SetAttribute( moText p_attribute );

    const moText& ToJSON();
    const moText& ToXML();
    int Set( const moText& p_XmlText );

    bool FixType( moValueType p_ValueType );
	private:

        /// Este miembro es lo que diferencia un Valor (moValueBase) de un Dato ( moData ).
		moValueDefinition	m_ValueDefinition;
    moText m_FullJSON;
    moText m_FullXML;
};


moDeclareExportedDynamicArray( moValueBase, moValueBases );


/// Valor de un Par�metro
/**
 * moValue es la clase para manejar el valor de un par�metro
 * el valor de un par�metro puede ser uno mas datos
 *
 * moValue puede contener varios datos encapsulados a los que se acceden a trav�s de las funciones:
 * GetSubValue, GetSubValueCount, y el operador []
 *
 */

class LIBMOLDEO_API moValue
{
	public:
		moValue();
		moValue( const moValue &src);
		moValue( const moText &strvalue, moValueType p_valuetype );
		moValue( const moText &strvalue );
		moValue( MOchar p_char );
		moValue( MOint p_int );
		moValue( MOlong p_long );
		moValue( MOfloat p_float );
		moValue( MOdouble p_double );

		moValue( const moText &strvalue, const moText &type );
		moValue( const moText &strvalue, const moText &type, const moText &strvalue2, const moText &type2 );
		moValue( const moText &strvalue, const moText &type, const moText &strvalue2, const moText &type2, const moText &strvalue3, const moText &type3 );
		moValue( const moText &strvalue, const moText &type, const moText &strvalue2, const moText &type2, const moText &strvalue3, const moText &type3, const moText &strvalue4, const moText &type4 );

    void AddSubValue(const  moText &strvalue,  moValueType p_valuetype );

		void AddSubValue(const  moText &strvalue, const moText &type );

		void AddSubValue(const  moValueBase &valuebase);

		void RemoveSubValue( MOint p_indexsubvalue );

		void RemoveSubValues( bool leavefirstone = true );

		virtual ~moValue();

		moValue &operator = (const moValue &src);
		moValueBase &operator [] ( MOint p_indexsubvalue ) {
			if (p_indexsubvalue!=-1) {
				return m_List[p_indexsubvalue];
			} else {
				return m_List[0];
			}
		}
		moValueBase &GetSubValue( MOint p_indexsubvalue = 0 ) {
			return m_List[p_indexsubvalue];
		}
		moValueBase &GetLastSubValue() {
			return m_List[GetSubValueCount() - 1];
		}
		MOuint	GetSubValueCount() {
			return m_List.Count();
		}
		moValue& Ref() {
		    return *this;
        }

    int Set( const moText& p_XmlText );
    const moText& ToJSON();
    const moText& ToXML();

	private:
		moValueBases	m_List;
    moText m_FullJSON;
    moText m_FullXML;
};


moDeclareExportedDynamicArray( moValue, moValues );

#endif

