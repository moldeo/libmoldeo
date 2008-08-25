/*******************************************************************************

                              moValue.cpp

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

#include "moValue.h"


#include "moTexture.h"
#include "moTextureFilter.h"

#include "moArray.cpp"

moDefineDynamicArray( moValueIndexes )
moDefineDynamicArray( moValueBases )
moDefineDynamicArray( moValues )
moDefineDynamicArray( moDatas )
moDefineDynamicArray( moDataMessages )

//================================================================
//	moData
//================================================================

moData::moData() {
	m_Number.m_Long = 0;
	m_DataSize = 0;
	m_DataType = MO_DATA_UNDEFINED;
	m_bFilteredAlpha = false;
	m_bFilteredParams = false;
	m_AlphaFilter = 1.0;
	m_pFilterParam = NULL;
	m_pAlphaFilter = NULL;
}

moData::moData( MOchar data ) {
	m_Number.m_Char = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_CHAR;
}

moData::moData( MOint data) {
	m_Number.m_Int = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_INT;
}

moData::moData( MOlonglong data) {
	m_Number.m_Long = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_LONG;
}

moData::moData( MOdouble data) {
	m_Number.m_Double = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_DOUBLE;
}

moData::moData( MOfloat data) {
	m_Number.m_Float = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_FLOAT;
}

/*
	MO_DATA_POINTER,//may be a pointer to struct or to class
	MO_DATA_VECTOR,//array of values
	MO_DATA_IMAGESAMPLE,//pointer to an imagesample pointer
	MO_DATA_SOUNDSAMPLE,//pointer to a soundsample pointer
	MO_DATA_VIDEOSAMPLE,//pointer to a videosample pointer
*/
moData::moData( MOpointer data , MOulong size, moDataType type ) {
	m_Number.m_Pointer = data;
	m_DataSize = size;
	m_DataType = type;
}

moData::moData( moText data ) {
	m_Text = data;
	m_DataSize = 0;
	m_Number.m_Long = 0;
	m_DataType = MO_DATA_TEXT;
}

moData::moData( moDataType datatype ) {
	m_DataType = datatype;
}

moData::~moData() {

}

moData& moData::operator =( const moData& data)
{
	m_Number = data.m_Number;
	m_Text = data.m_Text;
	m_DataType = data.m_DataType;
	m_DataSize = data.m_DataSize;
	m_AlphaFilter = data.m_AlphaFilter;
	m_pFilterParam = data.m_pFilterParam;
	m_bFilteredAlpha = data.m_bFilteredAlpha;
	m_bFilteredParams = data.m_bFilteredParams;
    return *this;
}

void
moData::Copy( const moData& data ) {
    (*this) = data;
}

void
moData::SetText( moText ptext ) {
	m_Text = ptext;
	m_DataType = MO_DATA_TEXT;
}

void
moData::SetFloat( MOfloat pfloat ) {
	m_Number.m_Float = pfloat;
	m_DataType = MO_DATA_NUMBER_FLOAT;
}

void
moData::SetDouble( MOdouble pdouble ) {
	m_Number.m_Double = pdouble;
	m_DataType = MO_DATA_NUMBER_DOUBLE;
}

void
moData::SetInt( MOint pint ) {
	m_Number.m_Int = pint;
	m_DataType = MO_DATA_NUMBER_INT;
}

void
moData::SetLong( MOlonglong plong ) {
	m_Number.m_Long = plong;
	m_DataType = MO_DATA_NUMBER_LONG;
}

void
moData::SetChar( MOchar pchar ) {
	m_Number.m_Char = pchar;
	m_DataType = MO_DATA_NUMBER_CHAR;
}

void
moData::SetNumber( moNumber p_number ) {
	m_Number = p_number;
	m_DataType = MO_DATA_NUMBER;
}

void
moData::SetType( moDataType	p_DataType  ) {
	m_DataType = p_DataType;
}

void
moData::SetPointer( MOpointer data, MOulong size ) {
	m_Number.m_Pointer = (MOpointer) data;
	m_DataSize = size;
	if (m_DataType==MO_DATA_UNDEFINED)
		m_DataType = MO_DATA_POINTER;
}

void
moData::SetSize( MOulong p_DataSize  ) {
	m_DataSize = p_DataSize;
}

void
moData::SetFun( moMathFunction*	p_Function ) {
	m_DataType = MO_DATA_FUNCTION;
	m_Number.m_Pointer = (MOpointer) p_Function;
}


void
moData::SetTexture( moTexture*	p_Texture ) {
	m_DataType = MO_DATA_IMAGESAMPLE;
	m_Number.m_Pointer = (MOpointer) p_Texture;
}

void
moData::SetTextureFilter( moTextureFilter*	p_TextureFilter ) {
    m_DataType = MO_DATA_IMAGESAMPLE_FILTERED;
	m_Number.m_Pointer = (MOpointer) p_TextureFilter;

}

void
moData::SetFont( moFont*	p_Font ) {
    m_DataType = MO_DATA_FONTPOINTER;
	m_Number.m_Pointer = (MOpointer) p_Font;
}

void
moData::SetModel( mo3DModel*    p_Model ) {
    m_DataType = MO_DATA_3DMODELPOINTER;
	m_Number.m_Pointer = (MOpointer) p_Model;
}

void
moData::SetTextureFilterAlpha( moData* p_alpha ) {

    if (p_alpha!=NULL)
    switch( p_alpha->m_DataType ) {
        case MO_DATA_FUNCTION:
            m_bFilteredAlpha = true;
            m_AlphaFilter = p_alpha->Fun()->Eval(0);
            m_pAlphaFilter = p_alpha;
            break;
        case MO_DATA_NUMBER:
        case MO_DATA_NUMBER_CHAR:
        case MO_DATA_NUMBER_DOUBLE:
        case MO_DATA_NUMBER_FLOAT:
        case MO_DATA_NUMBER_INT:
        case MO_DATA_NUMBER_LONG:
            m_bFilteredAlpha = true;
            m_AlphaFilter = p_alpha->Float();
            m_pAlphaFilter = NULL;
            break;
        default:
            m_bFilteredAlpha = true;
            m_AlphaFilter = 1.0;
            m_pAlphaFilter = NULL;
            break;
    }
}

void
moData::SetTextureFilterParam( moTextFilterParam *p_filterparam ) {
    m_bFilteredParams = true;
    m_pFilterParam = p_filterparam;

}

void
moData::SetMessage( moDataMessage *p_datamessage ) {
    m_DataType = MO_DATA_MESSAGE;
	m_Number.m_Pointer = (MOpointer) p_datamessage;
}


void
moData::SetMessages( moDataMessages *p_datamessages ) {
    m_DataType = MO_DATA_MESSAGES;
	m_Number.m_Pointer = (MOpointer) p_datamessages;
}

/*
void
moData::SetSound( moSound*	p_Sound ) {
	m_DataType = MO_DATA_SOUNDSAMPLE;
	m_Number.m_Pointer = (MOpointer) p_Sound;
}
*/
moDataMessage*
moData::Message() {
	return (moDataMessage*) m_Number.m_Pointer;
}

moDataMessages*
moData::Messages() {
	return (moDataMessages*) m_Number.m_Pointer;
}

moMathFunction*
moData::Fun() {
	return (moMathFunction*) m_Number.m_Pointer;
}

moFont*
moData::Font() {
    return (moFont*) m_Number.m_Pointer;
}

mo3DModel*
moData::Model() {
    return (mo3DModel*) m_Number.m_Pointer;
}
/*
moTexture*
moData::Texture() {
	return (moTexture*) m_Number.m_Pointer;
}

moSound*
moData::Sound() {
	return (moSound*) m_Number.m_Pointer;
}
*/

moText
moData::Text() {
	return m_Text;
}

moText
moData::TypeToText() {

    switch(m_DataType) {
        case MO_DATA_NUMBER:
            return moText("MO_DATA_NUMBER");
            break;
        case MO_DATA_NUMBER_CHAR:
            return moText("MO_DATA_NUMBER_CHAR");
            break;
        case MO_DATA_NUMBER_INT:
            return moText("MO_DATA_NUMBER_INT");
            break;
        case MO_DATA_NUMBER_LONG:
            return moText("MO_DATA_NUMBER_LONG");
            break;
        case MO_DATA_NUMBER_DOUBLE:
            return moText("MO_DATA_NUMBER_DOUBLE");
            break;
        case MO_DATA_NUMBER_FLOAT:
            return moText("MO_DATA_NUMBER_FLOAT");
            break;
        case MO_DATA_NUMBER_MIDI:
            return moText("MO_DATA_NUMBER_MIDI");
            break;
        case MO_DATA_FUNCTION:
            return moText("MO_DATA_FUNCTION");
            break;
        case MO_DATA_IMAGESAMPLE:
            return moText("MO_DATA_IMAGESAMPLE");
            break;
        case MO_DATA_IMAGESAMPLE_FILTERED:
            return moText("MO_DATA_IMAGESAMPLE_FILTERED");
            break;
        case MO_DATA_VIDEOSAMPLE:
            return moText("MO_DATA_VIDEOSAMPLE");
            break;
        case MO_DATA_POINTER:
            return moText("MO_DATA_POINTER");
            break;
        case MO_DATA_TEXT:
            return moText("MO_DATA_TEXT");
            break;
        case MO_DATA_UNDEFINED:
            return moText("MO_DATA_UNDEFINED");
            break;
        case MO_DATA_VECTOR:
            return moText("MO_DATA_VECTOR");
            break;
        case MO_DATA_MESSAGE:
            return moText("MO_DATA_MESSAGE");
            break;
        case MO_DATA_MESSAGES:
            return moText("MO_DATA_MESSAGES");
            break;

    }

    return moText("MO_DATA_UNDEFINED");
}

moDataType
moData::TextToType( moText texttype ) {

    if ( texttype ==  moText("MO_DATA_NUMBER") ) {
        return MO_DATA_NUMBER;
    } else
    if ( texttype ==  moText("MO_DATA_NUMBER_CHAR") ) {
        return MO_DATA_NUMBER_CHAR;
    } else
    if ( texttype ==  moText("MO_DATA_NUMBER_INT") ) {
        return MO_DATA_NUMBER_INT;
    } else
    if ( texttype ==  moText("MO_DATA_NUMBER_LONG") ) {
        return MO_DATA_NUMBER_LONG;
    } else
    if ( texttype ==  moText("MO_DATA_NUMBER_DOUBLE") ) {
        return MO_DATA_NUMBER_DOUBLE;
    } else
    if ( texttype ==  moText("MO_DATA_NUMBER_FLOAT") ) {
        return MO_DATA_NUMBER_FLOAT;
    } else
    if ( texttype ==  moText("MO_DATA_NUMBER_MIDI") ) {
        return MO_DATA_NUMBER_MIDI;
    } else
    if ( texttype ==  moText("MO_DATA_FUNCTION") ) {
        return MO_DATA_FUNCTION;
    } else
    if ( texttype ==  moText("MO_DATA_IMAGESAMPLE") ) {
        return MO_DATA_IMAGESAMPLE;
    } else
    if ( texttype ==  moText("MO_DATA_IMAGESAMPLE_FILTERED") ) {
        return MO_DATA_IMAGESAMPLE_FILTERED;
    } else
    if ( texttype ==  moText("MO_DATA_VIDEOSAMPLE") ) {
        return MO_DATA_VIDEOSAMPLE;
    } else
    if ( texttype ==  moText("MO_DATA_POINTER") ) {
        return MO_DATA_POINTER;
    } else
    if ( texttype ==  moText("MO_DATA_TEXT") ) {
        return MO_DATA_TEXT;
    } else
    if ( texttype ==  moText("MO_DATA_UNDEFINED") ) {
        return MO_DATA_UNDEFINED;
    } else
    if ( texttype ==  moText("MO_DATA_VECTOR") ) {
        return MO_DATA_VECTOR;
    } else
    if ( texttype ==  moText("MO_DATA_MESSAGE") ) {
        return MO_DATA_MESSAGE;
    } else
    if ( texttype ==  moText("MO_DATA_MESSAGES") ) {
        return MO_DATA_MESSAGES;
    }

    return MO_DATA_UNDEFINED;
}


moText
moData::ToText() {

    switch(m_DataType) {
        case MO_DATA_NUMBER:
            return IntToStr( Int() );
            break;
        case MO_DATA_NUMBER_CHAR:
            return IntToStr( Char() );
            break;
        case MO_DATA_NUMBER_INT:
            return IntToStr( Int() );
            break;
        case MO_DATA_NUMBER_LONG:
            return IntToStr( Long() );
            break;
        case MO_DATA_NUMBER_DOUBLE:
            return FloatToStr( Double() );
            break;
        case MO_DATA_NUMBER_FLOAT:
            return FloatToStr( Float() );
            break;
    }

    return Text();
}

MOint
moData::Int() {
	double rndD;
	float rndF;
	switch(m_DataType) {
		case MO_DATA_NUMBER_INT:
			return m_Number.m_Int;
			break;
		case MO_DATA_NUMBER_CHAR:
			return (MOint)m_Number.m_Char;
			break;
		case MO_DATA_NUMBER_LONG:
			return (MOint)m_Number.m_Long;
			break;
		case MO_DATA_NUMBER_FLOAT:
            rndF = moRound(m_Number.m_Float);
			return (MOint) rndF;
			break;
		case MO_DATA_NUMBER_DOUBLE:
            rndD = moRound(m_Number.m_Double);
			return (MOint) rndD;
			break;
		default:
			return m_Number.m_Int;
			break;
	}
}

MOlonglong
moData::Long() {
	double rndD;
	float rndF;
	switch(m_DataType) {
		case MO_DATA_NUMBER_LONG:
			return m_Number.m_Long;
			break;
		case MO_DATA_NUMBER_INT:
			return (MOlonglong) m_Number.m_Int;
			break;
		case MO_DATA_NUMBER_CHAR:
			return (MOlonglong)m_Number.m_Char;
			break;
		case MO_DATA_NUMBER_FLOAT:
            rndF = moRound( m_Number.m_Float );
			return (MOlonglong) rndF;
			break;
		case MO_DATA_NUMBER_DOUBLE:
		    rndD = moRound( m_Number.m_Double );
			return (MOlonglong) rndD;
			break;
		default:
			return m_Number.m_Long;
			break;
	}
}

MOfloat
moData::Float() {
	switch(m_DataType) {
		case MO_DATA_NUMBER_FLOAT:
			return m_Number.m_Float;
			break;
		case MO_DATA_NUMBER_DOUBLE:
			return (MOfloat) m_Number.m_Double;
			break;
		case MO_DATA_NUMBER_INT:
			return (MOfloat)m_Number.m_Int;
			break;
		case MO_DATA_NUMBER_LONG:
			return (MOfloat)m_Number.m_Long;
			break;
		default:
			return m_Number.m_Float;
			break;
	}
}

MOdouble
moData::Double() {
	switch(m_DataType) {
		case MO_DATA_NUMBER_DOUBLE:
			return m_Number.m_Double;
			break;
		case MO_DATA_NUMBER_FLOAT:
			return (MOdouble)m_Number.m_Float;
			break;
		case MO_DATA_NUMBER_INT:
			return (MOdouble)m_Number.m_Int;
			break;
		case MO_DATA_NUMBER_LONG:
			return (MOdouble)m_Number.m_Long;
			break;
		default:
			return m_Number.m_Double;
			break;
	}
}

MOchar
moData::Char() {
	float rndF;
	double rndD;
	switch(m_DataType) {
		case MO_DATA_NUMBER_CHAR:
			return m_Number.m_Char;
			break;
		case MO_DATA_NUMBER_LONG:
			return (MOchar)m_Number.m_Long;
			break;
		case MO_DATA_NUMBER_INT:
			return (MOchar) m_Number.m_Int;
			break;
		case MO_DATA_NUMBER_FLOAT:
		    rndF = moRound( m_Number.m_Float );
			return (MOchar) rndF;
			break;
		case MO_DATA_NUMBER_DOUBLE:
		    rndD = moRound( m_Number.m_Double );
			return (MOchar) rndD;
			break;
		default:
			return m_Number.m_Char;
			break;
	}
}

moNumber
moData::Number() {
	return m_Number;
}

MOpointer
moData::Pointer() {
    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                return (MOpointer) PTI->GetTexture(0);
            }
        }
    } else return m_Number.m_Pointer;
}

MOulong
moData::Size() {
	return m_DataSize;
}


moDataType
moData::Type() {
	return m_DataType;
}

moData*
moData::GetData() {
	return (this);
}

GLint
moData::GetGLId( MOfloat p_cycle, MOfloat p_fade, moTextFilterParam *p_filterparam ) {

    moTexture*	Texture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {
            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(p_cycle);
                }
            }
            if (m_pFilterParam)
                p_filterparam = m_pFilterParam;

            pTF->Apply( p_cycle, p_fade, p_filterparam);
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                Texture = PTI->GetTexture(0);
            }
        }
    } else Texture = (moTexture*) m_Number.m_Pointer;

    if (Texture!=NULL) {
        if ((p_cycle >= 0.0) && ((Texture->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(Texture->GetType() == MO_TYPE_MOVIE) ||
								(Texture->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)Texture;
			return ptex_anim ->GetGLId((MOfloat)p_cycle);
		}
		else return Texture->GetGLId();

    } else return 0;
}

GLint
moData::GetGLId( moTempo *p_tempo, MOfloat p_fade, moTextFilterParam *p_filterparam ) {

    moTexture*	Texture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {

            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(p_tempo->ang);
                }
            }
            if (m_pFilterParam)
                p_filterparam = m_pFilterParam;

            pTF->Apply( p_tempo, p_fade, p_filterparam);
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                Texture = PTI->GetTexture(0);
            }
        }
    } else Texture = (moTexture*) m_Number.m_Pointer;

    if (Texture!=NULL) {
        if ( (p_tempo != NULL) && ((Texture->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(Texture->GetType() == MO_TYPE_MOVIE) ||
								(Texture->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)Texture;
			return ptex_anim ->GetGLId((moTempo *) p_tempo);
		}
		else return Texture->GetGLId();

    } else return 0;
}

GLint
moData::GetGLId( MOuint p_i , MOfloat p_fade, moTextFilterParam *p_filterparam ) {

    moTexture*	Texture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {

            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(p_i);
                }
            }
            if (m_pFilterParam)
                p_filterparam = m_pFilterParam;


            pTF->Apply( p_i, p_fade, p_filterparam);
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                Texture = PTI->GetTexture(0);
            }
        }
    } else Texture = (moTexture*) m_Number.m_Pointer;

    if (Texture!=NULL) {
        if (( p_i >= 0 ) && ((Texture->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(Texture->GetType() == MO_TYPE_MOVIE) ||
								(Texture->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)Texture;
			return ptex_anim ->GetGLId( (MOuint) p_i);
		}
		else return Texture->GetGLId();

    } else return 0;
}

GLint
moData::GetGLId(MOfloat p_fade, moTextFilterParam *p_filterparam ) {

    moTexture*	Texture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {

            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(0);
                }
            }
            if (m_pFilterParam)
                p_filterparam = m_pFilterParam;


            pTF->Apply( (MOuint)0, p_fade, p_filterparam );

            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                Texture = PTI->GetTexture(0);
            }
        }
    } else Texture = (moTexture*) m_Number.m_Pointer;

    if (Texture!=NULL)
        return Texture->GetGLId();
    else return 0;
}

//================================================================
//	moValueDefinition
//================================================================


moValueDefinition::moValueDefinition() {
    m_Min = -1.0;
    m_Max = -1.0;
    m_CodeName = "";
    m_Attribute = "";
    m_Index = -1;
}

moValueDefinition::moValueDefinition(const moValueDefinition &src) {
	*this = src;
}

moValueDefinition::~moValueDefinition() {
}

moValueDefinition &moValueDefinition::operator = (const moValueDefinition &src)
{
		m_Type = src.m_Type;
		m_Index = src.m_Index;
		return *this;
}

void
moValueDefinition::SetType( moValueType p_type ) {
	m_Type = p_type;
}

void
moValueDefinition::SetIndex( MOint	p_index ) {
	m_Index = p_index;
}

moValueType
moValueDefinition::GetType() {
	return m_Type;
}

moText
moValueDefinition::GetTypeStr() {
	switch((int)m_Type) {
		case MO_VALUE_FUNCTION:
			return moText("FUNCTION");
			break;
		case MO_VALUE_LNK:
			return moText("LNK");
			break;
		case MO_VALUE_NUM:
		case MO_VALUE_NUM_CHAR:
		case MO_VALUE_NUM_DOUBLE:
		case MO_VALUE_NUM_FLOAT:
		case MO_VALUE_NUM_INT:
		case MO_VALUE_NUM_LONG:
			return moText("NUM");
			break;
		case MO_VALUE_TXT:
			return moText("TXT");
			break;
		case MO_VALUE_MATRIX:
			return moText("MATRIX");
			break;
	}
	return moText("UNDEFINED");
}

MOint
moValueDefinition::GetIndex() {
	return m_Index;
}

moText
moValueDefinition::GetCodeName() {
    return m_CodeName;
}

void
moValueDefinition::SetCodeName( moText p_codename ) {
    m_CodeName = p_codename;
}

void
moValueDefinition::SetRange( MOfloat min, MOfloat max ) {
    m_Min = min;
    m_Max = max;
}

void
moValueDefinition::SetRange( moText min, moText max ) {

    sscanf( min, "%f", &m_Min);
    sscanf( max, "%f", &m_Max);
}

void
moValueDefinition::GetRange( MOfloat* min, MOfloat* max) {
    if (min && max) {
        (*min) = m_Min;
        (*max) = m_Max;
    }
}

moText
moValueDefinition::GetAttribute() {
    return m_Attribute;
}

void
moValueDefinition::SetAttribute( moText p_attribute ) {
    m_Attribute = p_attribute;
}

//================================================================
//	moValueBase
//================================================================

moValueBase::moValueBase() {
}

moValueBase::moValueBase(const moValueBase &src) : moData(src)
{
	*this = src;
}

moValueBase::~moValueBase() {
}

moValueBase &moValueBase::operator = (const moValueBase &src) {
		m_ValueDefinition = src.m_ValueDefinition;
		(moData&)(*this)=(const moData&) src;
		return *this;
}




void
moValueBase::SetType( moValueType p_type ) {
	m_ValueDefinition.SetType(p_type);
}

void
moValueBase::SetIndex( MOint	p_index ) {
	m_ValueDefinition.SetIndex(p_index);
}

moValueType
moValueBase::GetType() {
	return m_ValueDefinition.GetType();
}

moText
moValueBase::GetTypeStr() {
	return m_ValueDefinition.GetTypeStr();
}

MOint
moValueBase::GetIndex() {
	return m_ValueDefinition.GetIndex();
}

moText
moValueBase::GetCodeName() {
    return m_ValueDefinition.GetCodeName();
}

void
moValueBase::SetCodeName( moText p_codename ) {
    m_ValueDefinition.SetCodeName( p_codename );
}

//================================================================
//	moValue
//================================================================

moValue::moValue() {

}

moValue::moValue(const moValue &src) {
	*this = src;
}
/*
	MO_VALUE_NUM,//any type of number
	MO_VALUE_MATRIX,//any type of VECTOR
	MO_VALUE_TXT,//any type of text, single or multiline
	MO_VALUE_LNK,//link to a file, text is interpreted as relative, absolute link to a file
	MO_VALUE_FUNCTION,//function parameter value, with lots of attributes....
	MO_VALUE_XML//
*/
moValue::moValue( const moText &strvalue , const moText &type ) {
	AddSubValue( strvalue, type );
}

void moValue::RemoveSubValue( MOint p_indexsubvalue ) {
    m_List.Remove( p_indexsubvalue );
}


void moValue::RemoveSubValues( bool leavefirstone ) {
    if (leavefirstone) {
        while( m_List.Count()>1 ) {
            m_List.Remove(1);
        }
    } else {
        m_List.Empty();
    }
}

void
moValue::AddSubValue( const moValueBase &valuebase ) {
    m_List.Add( valuebase );
}


void
moValue::AddSubValue( const moText &strvalue, const moText &type ) {

	moValueBase valuebase;

	if ( (moText)type == moText("TXT") ) {

		valuebase.SetText( strvalue );
		valuebase.SetType( MO_VALUE_TXT );

	} else if ((moText)type == moText("LNK")) {

		valuebase.SetText( strvalue );
		valuebase.SetType( MO_VALUE_LNK );

	} else if ((moText)type== moText("XML") ) {

		valuebase.SetText( strvalue );
		valuebase.SetType( MO_VALUE_XML );

	} else if ( (moText)type== moText("NUM")) {

		MOint tmp2;
		sscanf( moText(strvalue), "%i", &tmp2);
		valuebase.SetInt( tmp2 );
		valuebase.SetType( MO_VALUE_NUM );

	} else if ( (moText)type== moText("INT")) {

		MOint tmp2;
		sscanf( moText(strvalue), "%i", &tmp2);
		valuebase.SetInt( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_INT );

	} else if ( (moText)type== moText("CHAR")) {

		MOchar tmp2;
		sscanf( moText(strvalue), "%c", &tmp2);
		valuebase.SetChar( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_CHAR );

	} else if ( (moText)type== moText("LONG")) {

		MOlonglong tmp2;
		sscanf( moText(strvalue), "%li", &tmp2);
		valuebase.SetLong( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_LONG );

	} else if ( (moText)type== moText("FLOAT")) {

		MOfloat tmp2;
		sscanf( moText(strvalue), "%f", &tmp2);
		valuebase.SetFloat( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_LONG );

	} else if ( (moText)type== moText("DOUBLE")) {

		MOdouble tmp2;
		sscanf( moText(strvalue), "%lf", &tmp2);
		valuebase.SetDouble( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_DOUBLE );

	} else if ((moText)type== moText("FUNCTION")) {
		//float tmp2;
		//sscanf( strvalue, "%f", &tmp2);
		//valuebase.SetFloat( tmp2 );
		valuebase.SetText( strvalue );
		valuebase.SetType( MO_VALUE_FUNCTION );

	} else if ((moText)type== moText("MATRIX") ) {

		//SetText( strvalue );
		//float tmp2;
		//sscanf( strvalue, "%f", &tmp2);
		//valuebase.SetFloat( tmp2 );
		valuebase.SetText( strvalue );
		valuebase.SetType( MO_VALUE_MATRIX );

	}

	m_List.Add( valuebase );

}

moValue::~moValue() {

}

moValue &moValue::operator = (const moValue &src) {
		m_List = src.m_List;
		return *this;
}

