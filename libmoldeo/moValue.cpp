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

*******************************************************************************/

#include "moValue.h"


#include "moTexture.h"
#include "moTextureFilter.h"

#include "moArray.h"
#include "moText.h"
#include <tinyxml.h>
#include "moDebugManager.h"

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
	m_Number.m_Pointer = NULL;
	m_DataSize = 0;
	m_DataType = MO_DATA_UNDEFINED;
	m_bFilteredAlpha = false;
	m_bFilteredParams = false;
	m_AlphaFilter = 1.0;
	//m_pFilterParam = NULL;
	m_pAlphaFilter = NULL;
}

moData::moData( MOchar data ) {
    (*this) = moData();
	m_Number.m_Char = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_CHAR;
}

moData::moData( MOint data) {
    (*this) = moData();
	m_Number.m_Int = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_INT;
}

moData::moData( MOlonglong data) {
    (*this) = moData();
	m_Number.m_Long = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_LONG;
}

moData::moData( MOdouble data) {
    (*this) = moData();
	m_Number.m_Double = data;
	m_DataSize = 0;
	m_DataType = MO_DATA_NUMBER_DOUBLE;
}

moData::moData( MOfloat data) {
    (*this) = moData();
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
    (*this) = moData();
	m_Number.m_Pointer = data;
	m_DataSize = size;
	m_DataType = type;
}

moData::moData( moText data ) {
    (*this) = moData();
	m_Text = data;
	m_DataSize = 0;
	m_Number.m_Long = 0;
	m_DataType = MO_DATA_TEXT;
}

moData::moData( moDataType datatype ) {
    (*this) = moData();
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
	m_pAlphaFilter = data.m_pAlphaFilter;
	m_pFilterParam = data.m_pFilterParam;
	m_bFilteredAlpha = data.m_bFilteredAlpha;
	m_bFilteredParams = data.m_bFilteredParams;
	m_LastEval = data.m_LastEval;
    return *this;
}

void
moData::Copy( const moData& data ) {
    (*this) = data;
}


bool
moData::IsValid() const {
    return (m_DataType!=MO_DATA_UNDEFINED);
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
	m_Text = FloatToStr(m_Number.m_Float);
	m_LastEval = (MOdouble)pfloat;
}

void
moData::SetDouble( MOdouble pdouble ) {
	m_Number.m_Double = pdouble;
	m_DataType = MO_DATA_NUMBER_DOUBLE;
	m_Text = FloatToStr(m_Number.m_Double);
	m_LastEval = pdouble;
}

void
moData::SetInt( MOint pint ) {
	m_Number.m_Int = pint;
	m_DataType = MO_DATA_NUMBER_INT;
	m_Text = IntToStr(m_Number.m_Int);
	m_LastEval = (MOdouble)pint;
}

void
moData::SetLong( MOlonglong plong ) {
	m_Number.m_Long = plong;
	m_DataType = MO_DATA_NUMBER_LONG;
	m_Text = IntToStr(m_Number.m_Long);
	m_LastEval = (MOdouble)plong;//loose precision...
}

void
moData::SetChar( MOchar pchar ) {
	m_Number.m_Char = pchar;
	m_DataType = MO_DATA_NUMBER_CHAR;
	m_Text = IntToStr(m_Number.m_Char);
}

void
moData::SetNumber( moNumber p_number ) {
	m_Number = p_number;
	m_DataType = MO_DATA_NUMBER;
	m_Text = "";
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
moData::SetFun( const moText& p_functionExpression ) {
  m_DataType = MO_DATA_FUNCTION;
  m_Text = p_functionExpression;
  m_Number.m_Pointer = (MOpointer) NULL;
}

void
moData::SetFun( moMathFunction*	p_Function ) {
	m_DataType = MO_DATA_FUNCTION;
	m_Number.m_Pointer = (MOpointer) p_Function;
	if (p_Function)
        m_Text = p_Function->GetExpression();
}


void
moData::SetTexture( moTexture*	p_Texture ) {
	m_DataType = MO_DATA_IMAGESAMPLE;
	m_Number.m_Pointer = (MOpointer) p_Texture;
}

void
moData::SetTextureBuffer( moTextureBuffer*	p_TextureBuffer ) {
    m_DataType = MO_DATA_IMAGESAMPLE_TEXTUREBUFFER;
	m_Number.m_Pointer = (MOpointer) p_TextureBuffer;
}

void
moData::SetVideoBuffer( moVideoBuffer*	p_VideoBuffer ) {
    m_DataType = MO_DATA_VIDEOSAMPLE;
	m_Number.m_Pointer = (MOpointer) p_VideoBuffer;
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
moData::SetModel( moSceneNode*    p_Model ) {
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
            m_pAlphaFilter = NULL;
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
moData::SetTextureFilterParam( const moTextFilterParam &p_filterparam ) {
    m_bFilteredParams = true;
    m_pFilterParam = p_filterparam;

}

void
moData::SetVector( moVector2d *p_vector2d ) {
    m_DataType = MO_DATA_VECTOR2F;
	m_Number.m_Pointer = (MOpointer) p_vector2d;
}

void
moData::SetVector( moVector3d *p_vector3d ) {
    m_DataType = MO_DATA_VECTOR3F;
	m_Number.m_Pointer = (MOpointer) p_vector3d;
}

void
moData::SetVector( moVector4d *p_vector4d ) {
    m_DataType = MO_DATA_VECTOR4F;
	m_Number.m_Pointer = (MOpointer) p_vector4d;
}

void
moData::SetVector( moVector2i *p_vector2i ) {
    m_DataType = MO_DATA_VECTOR2I;
	m_Number.m_Pointer = (MOpointer) p_vector2i;
}

void
moData::SetVector( moVector3i *p_vector3i ) {
    m_DataType = MO_DATA_VECTOR3I;
	m_Number.m_Pointer = (MOpointer) p_vector3i;
}

void
moData::SetVector( moVector4i *p_vector4i ) {
    m_DataType = MO_DATA_VECTOR4I;
	m_Number.m_Pointer = (MOpointer) p_vector4i;
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


void
moData::SetSound( moSound*	p_Sound ) {
	m_DataType = MO_DATA_SOUNDSAMPLE;
	m_Number.m_Pointer = (MOpointer) p_Sound;
}


moVector2d *
moData::Vector2d() {
    return (moVector2d*) m_Number.m_Pointer;
}

moVector2i *
moData::Vector2i() {
    return (moVector2i*) m_Number.m_Pointer;
}

moVector3d *
moData::Vector3d() {
    return (moVector3d*) m_Number.m_Pointer;
}

moVector3i *
moData::Vector3i() {
    return (moVector3i*) m_Number.m_Pointer;
}

moVector4d *
moData::Vector4d() {
    return (moVector4d*) m_Number.m_Pointer;
}

moVector4i *
moData::Vector4i() {
    return (moVector4i*) m_Number.m_Pointer;
}



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
  if (m_DataType==MO_DATA_FUNCTION) {
    return (moMathFunction*) m_Number.m_Pointer;
  } else return NULL;
}

MOdouble
moData::Eval() {
    if (m_DataType==MO_DATA_FUNCTION) {
      moMathFunction* pFun = Fun();
      m_LastEval = 0.0;
      if (pFun)
          m_LastEval = pFun->Eval();
    }

    return LastEval();
}

MOdouble
moData::Eval( double x ) {
    if (m_DataType==MO_DATA_FUNCTION) {
      moMathFunction* pFun = Fun();
      m_LastEval = 0.0;
      if (pFun)
          m_LastEval = pFun->Eval(x);
    }

    return LastEval();

}

MOdouble
moData::LastEval() const {
    switch((int)m_DataType) {
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
        case MO_DATA_FUNCTION:
            return m_LastEval;
            break;
		default:
			return m_LastEval;
			break;
	}
}


moFont*
moData::Font() {
    moFont* pFont = static_cast<moFont*>(m_Number.m_Pointer);
    return pFont;
}

moTextureBuffer*     moData::TextureBuffer() {
    moTextureBuffer* pTexBuf = static_cast<moTextureBuffer*>(m_Number.m_Pointer);
    return pTexBuf;

}


moSceneNode*
moData::Model() {
    moSceneNode* pModel = static_cast<moSceneNode*>(m_Number.m_Pointer);
    return pModel;
}

moSound*
moData::Sound() {
    moSound* pSound = static_cast<moSound*>(m_Number.m_Pointer);
    return pSound;
}

///TODO: esta funcion llama la textura de fuente ( al aplicar un shader, es la primera de las  de fuente)
///no confundir con la textura de destino...!!!
moTexture*
moData::Texture() {
  moTexture* pTexture = NULL;
  if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
      moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
      if (pTF) {
          moTextureIndex* PTI = pTF->GetSrcTex();
          if (PTI) {
              pTexture = PTI->GetTexture(0);
          }
      }
  } else if (m_DataType==MO_DATA_IMAGESAMPLE || m_DataType==MO_DATA_IMAGESAMPLE_TEXTUREBUFFER  ) {
    pTexture = static_cast<moTexture*>(m_Number.m_Pointer);
  }
  return pTexture;
}

///if MO_DATA_IMAGESAMPLE source = destination
moTexture*
moData::TextureDestination() {

  moTexture* pTexture = NULL;

  if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
      moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
      if (pTF) {
          moTextureIndex* PTI = pTF->GetDestTex();
          if (PTI) {
              pTexture = PTI->GetTexture(0);
          }
      }
  } else if (m_DataType==MO_DATA_IMAGESAMPLE || m_DataType==MO_DATA_IMAGESAMPLE_TEXTUREBUFFER  ) {
    pTexture = static_cast<moTexture*>(m_Number.m_Pointer);
  }
  return pTexture;
}


moText
moData::Text() {
	return m_Text;
}

moText
moData::TypeToText() {
  return moData::TypeToText( m_DataType );
}

moText
moData::TypeToText( moDataType p_data_type ) {

    switch((int)p_data_type) {
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
        case MO_DATA_SOUNDSAMPLE:
            return moText("MO_DATA_SOUNDSAMPLE");
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
        case MO_DATA_VECTOR2I:
            return moText("MO_DATA_VECTOR2I");
            break;
        case MO_DATA_VECTOR3I:
            return moText("MO_DATA_VECTOR3I");
            break;
        case MO_DATA_VECTOR4I:
            return moText("MO_DATA_VECTOR4I");
            break;
        case MO_DATA_VECTOR2F:
            return moText("MO_DATA_VECTOR2F");
            break;
        case MO_DATA_VECTOR3F:
            return moText("MO_DATA_VECTOR3F");
            break;
        case MO_DATA_VECTOR4F:
            return moText("MO_DATA_VECTOR4F");
            break;
        case MO_DATA_MESSAGE:
            return moText("MO_DATA_MESSAGE");
            break;
        case MO_DATA_MESSAGES:
            return moText("MO_DATA_MESSAGES");
            break;
        default:
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
    if ( texttype ==  moText("MO_DATA_SOUNDSAMPLE") ) {
        return MO_DATA_SOUNDSAMPLE;
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
    if ( texttype ==  moText("MO_DATA_VECTOR2I") ) {
        return MO_DATA_VECTOR2I;
    } else
    if ( texttype ==  moText("MO_DATA_VECTOR2F") ) {
        return MO_DATA_VECTOR2F;
    } else
    if ( texttype ==  moText("MO_DATA_VECTOR3I") ) {
        return MO_DATA_VECTOR3I;
    } else
    if ( texttype ==  moText("MO_DATA_VECTOR3F") ) {
        return MO_DATA_VECTOR3F;
    } else
    if ( texttype ==  moText("MO_DATA_VECTOR4I") ) {
        return MO_DATA_VECTOR4I;
    } else
    if ( texttype ==  moText("MO_DATA_VECTOR4F") ) {
        return MO_DATA_VECTOR4F;
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
moData::ToText() const {

    moText finalMsg = "";
    moDataMessage *dM  =NULL;
    moDataMessages *dMs = NULL;

    switch((int)m_DataType) {
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
        /*
        case MO_DATA_FUNCTION:
            return moText(Eval());
            break;
        case MO_DATA_TEXT:
            return Text();
            break;
            */
        case MO_DATA_MESSAGE:
          dM = (moDataMessage*)m_Number.m_Pointer;
          if (dM) {
            for(int c=0;c<(int)dM->Count();c++) {
              finalMsg+= ";" + dM->Get(c).ToText();
            }
          }
          return finalMsg;
          break;
        case MO_DATA_MESSAGES:
          dMs = (moDataMessages*)m_Number.m_Pointer;
          if (dMs) {
            for(int c=0;c<(int)dMs->Count();c++) {
              //moDataMessage Mes = dMs->Get(c);
              for(int cc=0;cc<(int)dMs->Get(c).Count();cc++) {
                finalMsg+= ";" + dMs->Get(c).Get(cc).ToText();
              }
              finalMsg+= "\n";

            }
          }
          return finalMsg;
          break;
        default:
            break;
    }

    return m_Text;
    //return moText("");
    /*return moText("moData::ToText() for "+TypeToText()+" not implemented");*/
}

MOint
moData::Int() const {
	double rndD;
	float rndF;
	switch((int)m_DataType) {
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
        case MO_DATA_FUNCTION:
            return (MOint)m_LastEval;
            break;
		default:
			return m_Number.m_Int;
			break;
	}
}

MOlonglong
moData::Long() const {
	double rndD;
	float rndF;
	switch((int)m_DataType) {
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
        case MO_DATA_FUNCTION:
            return (MOlonglong)m_LastEval;
            break;
		default:
			return m_Number.m_Long;
			break;
	}
}

MOfloat
moData::Float() const {
	switch((int)m_DataType) {
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
        case MO_DATA_FUNCTION:
            return (MOfloat)m_LastEval;
            break;
		default:
			return m_Number.m_Float;
			break;
	}
}

MOdouble
moData::Double() const {
	switch((int)m_DataType) {
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
        case MO_DATA_FUNCTION:
            return m_LastEval;
            break;
		default:
			return m_Number.m_Double;
			break;
	}
}

MOchar
moData::Char() const {
	float rndF;
	double rndD;
	switch((int)m_DataType) {
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
        case MO_DATA_FUNCTION:
            return (MOchar)m_LastEval;
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
    }

    return m_Number.m_Pointer;
}

MOulong
moData::Size() const {
	return m_DataSize;
}


moDataType
moData::Type() const {
	return m_DataType;
}

moData*
moData::GetData() {
	return (this);
}

GLint
moData::GetGLId( MOfloat p_cycle ) {
    moTextFilterParam p_filterparam;
    return GetGLId( p_cycle, 1.0, p_filterparam);
}

GLint
moData::GetGLId( MOfloat p_cycle, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {

    moTexture*	pTexture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {
            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(p_cycle);
                }
            }
            //if (m_pFilterParam)
            //    p_filterparam = m_pFilterParam;

            pTF->Apply( p_cycle, p_fade, p_filterparam);
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                pTexture = PTI->GetTexture(0);
            }
        }
    } else pTexture = this->Texture();

    if (pTexture) {
        if ((p_cycle >= 0.0) && ((pTexture->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(pTexture->GetType() == MO_TYPE_MOVIE) ||
								(pTexture->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)pTexture;
			return ptex_anim ->GetGLId((MOfloat)p_cycle);
		}
		else return pTexture->GetGLId();

    } else return 0;
}

GLint
moData::GetGLId( moTempo *p_tempo ) {
    moTextFilterParam p_filterparam;
    return GetGLId( p_tempo, 1.0, p_filterparam);
}

GLint
moData::GetGLId( moTempo *p_tempo, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {

    moTexture*	pTexture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {

            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(p_tempo->ang);
                }
            }
            //if (m_pFilterParam)
            //    p_filterparam = m_pFilterParam;

            pTF->Apply( p_tempo, p_fade, p_filterparam);
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                pTexture = PTI->GetTexture(0);
            }
        }
    } else pTexture = this->Texture();

    if (pTexture) {
        if ( (p_tempo != NULL) && ((pTexture->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(pTexture->GetType() == MO_TYPE_MOVIE) ||
								(pTexture->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)pTexture;
			return ptex_anim ->GetGLId((moTempo *) p_tempo);
		}
		else return pTexture->GetGLId();

    } else return 0;
}


GLint
moData::GetGLId( MOuint p_i ) {
    moTextFilterParam p_filterparam;
    return GetGLId( p_i, 1.0, p_filterparam);
}

GLint
moData::GetGLId( MOuint p_i , MOfloat p_fade, const moTextFilterParam &p_filterparam ) {

    moTexture*	pTexture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {

            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(p_i);
                }
            }
            //if (m_pFilterParam)
            //    p_filterparam = m_pFilterParam;


            pTF->Apply( p_i, p_fade, p_filterparam);
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                pTexture = PTI->GetTexture(0);
            }
        }
    } else pTexture = this->Texture();

    if (pTexture) {
        if ( ((pTexture->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(pTexture->GetType() == MO_TYPE_MOVIE) ||
								(pTexture->GetType() == MO_TYPE_VIDEOBUFFER)))
		{
			moTextureAnimated* ptex_anim = (moTextureAnimated*)pTexture;
			return ptex_anim ->GetGLId( (MOuint) p_i);
		}
		else return pTexture->GetGLId();

    } else return 0;
}


GLint
moData::GetGLId() {
    moTextFilterParam p_filterparam;
    return GetGLId(1.0, p_filterparam);
}

GLint
moData::GetGLId( MOfloat p_fade, const moTextFilterParam &p_filterparam ) {

    moTexture*	pTexture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {

            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    p_fade = m_pAlphaFilter->Fun()->Eval(0);
                }
            }
            //if (m_pFilterParam)
            //    p_filterparam = m_pFilterParam;


            pTF->Apply( (MOuint)0, p_fade, p_filterparam );

            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                pTexture = PTI->GetTexture(0);
            }
        }
    } else pTexture = this->Texture();

    if (pTexture)
        return pTexture->GetGLId();
    else return 0;
}

GLint
moData::GetGLId( moMoldeoObject* p_mob ) {
    return GetGLId( p_mob, NULL);
}

GLint
moData::GetGLId( moMoldeoObject* p_mob, moTempo *p_tempo ) {
    moTextFilterParam p_filterparam;
    return GetGLId( p_mob, p_tempo, 1.0, p_filterparam);
}


GLint
moData::GetGLId( moMoldeoObject* p_mob, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {

    GetGLId( p_mob, NULL, p_fade, p_filterparam );
}

GLint
moData::GetGLId( moMoldeoObject* p_mob, moTempo *p_tempo, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {
    moTexture*	pTexture = NULL;

    if (m_DataType==MO_DATA_IMAGESAMPLE_FILTERED) {
        moTextureFilter* pTF = (moTextureFilter*) m_Number.m_Pointer;
        if (pTF) {

            if (m_bFilteredAlpha) {
                p_fade = m_AlphaFilter;
                if (m_pAlphaFilter) {
                    moMathFunction* mFun = m_pAlphaFilter->Fun();
                    mFun->Init( mFun->GetExpression(), p_mob );
                    p_fade = m_pAlphaFilter->Fun()->Eval();
                }
            }
            //if (m_pFilterParam)
            //    p_filterparam = m_pFilterParam;

            pTF->Apply( p_mob, p_tempo, p_fade, p_filterparam);
            moTextureIndex* PTI = pTF->GetDestTex();
            if (PTI) {
                pTexture = PTI->GetTexture(0);
            }
        }
    } else pTexture = this->Texture();

    if (pTexture) {
        if ( (p_tempo != NULL) && (
                (pTexture->GetType() == MO_TYPE_TEXTURE_MULTIPLE) ||
								(pTexture->GetType() == MO_TYPE_MOVIE) ||
								(pTexture->GetType() == MO_TYPE_VIDEOBUFFER)
              )
            )
        {
          moTextureAnimated* ptex_anim = (moTextureAnimated*)pTexture;
          return ptex_anim->GetGLId((moTempo *) p_tempo);
        }
        else return pTexture->GetGLId();

    } else return 0;
}

//================================================================
//	moValueDefinition
//================================================================


moValueDefinition::moValueDefinition() {
    m_Min = -1.0;
    m_Max = -1.0;
    m_CodeName = moText("");
    m_Attribute = moText("");
    m_Index = -1;
    m_Type =  MO_VALUE_UNDEFINED;
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
		m_CodeName = src.m_CodeName;
		m_Min = src.m_Min;
		m_Max = src.m_Max;
		m_Attribute = src.m_Attribute;
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
moValueDefinition::GetType() const {
	return m_Type;
}

moText
moValueDefinition::GetTypeStr() const {
	switch((int)m_Type) {
		case MO_VALUE_FUNCTION:
			return moText("FUNCTION");
			break;
		case MO_VALUE_LNK:
			return moText("LNK");
			break;
		case MO_VALUE_NUM:
      return moText("NUM");
      break;
		case MO_VALUE_NUM_CHAR:
      return moText("CHAR");
      break;
		case MO_VALUE_NUM_DOUBLE:
      return moText("DOUBLE");
      break;
		case MO_VALUE_NUM_FLOAT:
      return moText("FLOAT");
      break;
		case MO_VALUE_NUM_INT:
			return moText("INT");
			break;
		case MO_VALUE_NUM_LONG:
			return moText("LONG");
			break;
		case MO_VALUE_TXT:
			return moText("TXT");
			break;
		case MO_VALUE_XML:
			return moText("XML");
			break;
		case MO_VALUE_MATRIX:
			return moText("MATRIX");
			break;
	}
	return moText("UNDEFINED");
}

moValueType
moValueDefinition::ValueTypeFromStr( const moText& p_value_type_str ) {
    if (p_value_type_str=="FUNCTION") {
      return MO_VALUE_FUNCTION;
    } else if (p_value_type_str=="NUM") {
      return MO_VALUE_NUM;
    } else if (p_value_type_str=="CHAR") {
      return MO_VALUE_NUM_CHAR;
    } else if (p_value_type_str=="INT") {
      return MO_VALUE_NUM_INT;
    } else if (p_value_type_str=="LONG") {
      return MO_VALUE_NUM_LONG;
    } else if (p_value_type_str=="FLOAT") {
      return MO_VALUE_NUM_FLOAT;
    } else if (p_value_type_str=="DOUBLE") {
      return MO_VALUE_NUM_DOUBLE;
    } else if (p_value_type_str=="TXT") {
      return MO_VALUE_TXT;
    } else if (p_value_type_str=="XML") {
      return MO_VALUE_XML;
    } else if (p_value_type_str=="LNK") {
      return MO_VALUE_LNK;
    } else if (p_value_type_str=="MATRIX") {
      return MO_VALUE_MATRIX;
    }

    return MO_VALUE_UNDEFINED;
}

MOint
moValueDefinition::GetIndex() const {
	return m_Index;
}

moText
moValueDefinition::GetCodeName() const {
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
moValueDefinition::GetAttribute() const {
    return m_Attribute;
}

void
moValueDefinition::SetAttribute( moText p_attribute ) {
    m_Attribute = p_attribute;
}

bool
moValueDefinition::IsValid() const {
    return (m_Type!=MO_VALUE_UNDEFINED);
}

const moText&
moValueDefinition::ToJSON() {
  moText fieldSeparation = ",";

  m_FullJSON = "{";
    m_FullJSON+=  "'t': '" + GetTypeStr() + "'";
    if (m_CodeName!="") m_FullJSON+= fieldSeparation+"'cod': '" + m_CodeName + "'";
    if (m_Min!=m_Max) {
        m_FullJSON+= fieldSeparation + "'min': " + FloatToStr(m_Min);
        m_FullJSON+= fieldSeparation + "'max': " + FloatToStr(m_Max);
    }
  m_FullJSON+= "}";
  return m_FullJSON;
}

const moText&
moValueDefinition::ToXML() {
  moText fieldSeparation = " ";

  m_FullXML = "<moValueDefinition ";
  m_FullXML+= "codename='" + m_CodeName + "'";
  m_FullXML+= fieldSeparation + "type='" + GetTypeStr() + "'";
  m_FullXML+= fieldSeparation + "min='" + FloatToStr(m_Min)+ "'";
  m_FullXML+= fieldSeparation + "max='" + FloatToStr(m_Max)+ "'";
  m_FullXML+= fieldSeparation + ">";
  m_FullXML+= "</moValueDefinition>";
  return m_FullXML;
}


int
moValueDefinition::Set( const moText& p_XmlText ) {
 TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  //TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );
  TiXmlElement* definitionNode = m_XMLDoc.FirstChildElement("moValueDefinition");

  //if (rootKey) {

    //TiXmlElement* sceneStateNode = rootKey->FirstChildElement("moSceneState");
    if (definitionNode) {
      m_CodeName = moText( definitionNode->Attribute("codename") );
      m_Type = ValueTypeFromStr( moText(definitionNode->Attribute("type")) );
      m_Min = atof(moText( definitionNode->Attribute("min") ));
      m_Max = atof(moText( definitionNode->Attribute("max") ));
      return 0;
    } else moDebugManager::Log( "No XML moValueDefinition in: " + p_XmlText );

  //} else moDebugManager::Error();
  return -1;
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
moValueBase::SetRange( MOfloat min, MOfloat max ) {
    m_ValueDefinition.SetRange(min,max);
}

void
moValueBase::SetRange( moText min, moText max ) {
    m_ValueDefinition.SetRange(min,max);
}

void
moValueBase::GetRange( MOfloat* min, MOfloat* max) {
    m_ValueDefinition.GetRange(min,max);
}

moText
moValueBase::GetAttribute() const {
    return m_ValueDefinition.GetAttribute();
}

void
moValueBase::SetAttribute( moText p_attribute ) {
    m_ValueDefinition.SetAttribute(p_attribute);
}

void
moValueBase::SetType( moValueType p_type ) {
	m_ValueDefinition.SetType(p_type);
}

bool moValueBase::FixType( moValueType p_ValueType ) {

  moText datavalue;
  bool result = true;

  if (p_ValueType==MO_VALUE_FUNCTION) {
    switch(m_ValueDefinition.GetType()) {

      case MO_VALUE_NUM:
      case MO_VALUE_NUM_CHAR:
      case MO_VALUE_NUM_DOUBLE:
      case MO_VALUE_NUM_FLOAT:
      case MO_VALUE_NUM_INT:
      case MO_VALUE_NUM_LONG:
        datavalue = moData::ToText();
        SetFun( datavalue );
        this->m_ValueDefinition.SetType( p_ValueType );
        result = true;
        break;

      case MO_VALUE_FUNCTION:
        result = true;
        break;
      default:
        result = false;
        break;
    }
  }
  return result;
}

void
moValueBase::SetIndex( MOint	p_index ) {
	m_ValueDefinition.SetIndex(p_index);
}

moValueType
moValueBase::GetType() const {
	return m_ValueDefinition.GetType();
}

moText
moValueBase::GetTypeStr() const {
	return m_ValueDefinition.GetTypeStr();
}

MOint
moValueBase::GetIndex() const {
	return m_ValueDefinition.GetIndex();
}

moText
moValueBase::GetCodeName() const {
    return m_ValueDefinition.GetCodeName();
}

void
moValueBase::SetCodeName( moText p_codename ) {
    m_ValueDefinition.SetCodeName( p_codename );
}

const moText&
moValueBase::ToJSON() {
  moText fieldSeparation =",";
  m_FullJSON = "{";
  m_FullJSON+= "'d':" + m_ValueDefinition.ToJSON();
  m_FullJSON+= fieldSeparation + "'v':" + "'" + ToText() + "'";
  m_FullJSON+= "}";
  return m_FullJSON;
}

const moText&
moValueBase::ToXML() {
  moText fieldSeparation = " ";
  m_FullXML = "<moValueBase ";
  m_FullXML+= fieldSeparation+"type='"+GetTypeStr()+"'";
  m_FullXML+= fieldSeparation+"value='"+ToText()+"'";
  /** TODO: add range
  if (m_ValueDefinition.)
  */
  /*m_FullXML+= m_ValueDefinition.ToXML();*/
  m_FullXML+= fieldSeparation+">";
  m_FullXML+= ToText();
  m_FullXML+= "</moValueBase>";
  return m_FullXML;
}

int
moValueBase::Set( const moText& p_XmlText ) {

TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  //TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );
  TiXmlElement* valueNode = m_XMLDoc.FirstChildElement("moValueBase");

  if (valueNode) {
    //moData data( moText( valueNode->Attribute("value") ) );
    //SetType( );

    moText value = moText( valueNode->Attribute("value") );
    switch(GetType()) {
      case MO_VALUE_FUNCTION:
          SetFun( value );
          break;
      case MO_VALUE_NUM:
      case MO_VALUE_NUM_INT:
          SetInt( atoi(value) );
          break;
      case MO_VALUE_NUM_CHAR:
          SetChar( atoi(value) );
          break;
      case MO_VALUE_NUM_LONG:
          SetLong( atoi(value) );
          break;
      case MO_VALUE_NUM_FLOAT:
          SetFloat( atof(value) );
          break;
      case MO_VALUE_NUM_DOUBLE:
          SetDouble( atof(value) );
          break;
      case MO_VALUE_TXT:
          SetText( value );
          break;
      case MO_VALUE_XML:
          SetText( value );
          break;
      case MO_VALUE_MATRIX:
      case MO_VALUE_LNK:
          SetText( value );
          break;
      default:
          SetText(value);
          break;
    };

    SetType( moValueDefinition::ValueTypeFromStr( moText( valueNode->Attribute("type") ) )  );

    return 0;
  } else moDebugManager::Log( "No XML moValueBase in: " + p_XmlText );

  return -1;

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

moValue::moValue( const moText &strvalue, moValueType p_type ) {
  AddSubValue( strvalue, p_type );
}

moValue::moValue( MOchar p_char ) {
  moValueBase valuebase;
  valuebase.SetChar( p_char );
  AddSubValue( valuebase );
}

moValue::moValue( MOint p_int ) {
  moValueBase valuebase;
  valuebase.SetInt( p_int );
  AddSubValue( valuebase );
}

moValue::moValue( MOlong p_long ) {
  moValueBase valuebase;
  valuebase.SetLong( p_long );
  AddSubValue( valuebase );
}

moValue::moValue( MOfloat p_float ) {
  moValueBase valuebase;
  valuebase.SetFloat( p_float );
  AddSubValue( valuebase );
}

moValue::moValue( MOdouble p_double ) {
  moValueBase valuebase;
  valuebase.SetDouble( p_double );
  AddSubValue( valuebase );
}

moValue::moValue( const moText &strvalue , const moText &type ) {
	AddSubValue( strvalue, type );
}


moValue::moValue( const moText &strvalue, const moText &type, const moText &strvalue2, const moText &type2 ) {
	AddSubValue( strvalue, type );
	AddSubValue( strvalue2, type2 );
}

moValue::moValue( const moText &strvalue, const moText &type, const moText &strvalue2, const moText &type2, const moText &strvalue3, const moText &type3 ) {
	AddSubValue( strvalue, type );
	AddSubValue( strvalue2, type2 );
	AddSubValue( strvalue3, type3 );
}
moValue::moValue( const moText &strvalue, const moText &type, const moText &strvalue2, const moText &type2, const moText &strvalue3, const moText &type3, const moText &strvalue4, const moText &type4 ) {
	AddSubValue( strvalue, type );
	AddSubValue( strvalue2, type2 );
	AddSubValue( strvalue3, type3 );
	AddSubValue( strvalue4, type4 );
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
moValue::AddSubValue(const  moText &strvalue,  moValueType p_valuetype ) {

  MOint tmpInt;
  MOchar tmpChar;
  MOlong tmpLong;
  MOdouble tmpDouble;
  MOfloat tmpFloat;

  moValueBase valuebase;

  valuebase.SetText( strvalue );

  switch(p_valuetype) {

    case MO_VALUE_NUM:
    case MO_VALUE_NUM_INT:
      sscanf( moText(strvalue), "%i", &tmpInt);
      valuebase.SetInt( tmpInt );
      break;

    case MO_VALUE_NUM_LONG:
      sscanf( moText(strvalue), "%li", &tmpLong);
      valuebase.SetLong( tmpLong );
      break;

    case MO_VALUE_NUM_CHAR:
      sscanf( moText(strvalue), "%c", &tmpChar);
      valuebase.SetChar( tmpChar );
      break;

    case MO_VALUE_NUM_FLOAT:
      sscanf( moText(strvalue), "%f", &tmpFloat);
      valuebase.SetFloat( tmpFloat );
      break;

    case MO_VALUE_NUM_DOUBLE:
      sscanf( moText(strvalue), "%lf", &tmpDouble);
      valuebase.SetDouble( tmpDouble );
      break;

    case MO_VALUE_FUNCTION:
      valuebase.SetFun( strvalue );
      valuebase.SetType( MO_VALUE_FUNCTION );
      break;

    default:
      break;

  }

  valuebase.SetType( p_valuetype );

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

		MOint tmp2 = 0;
		sscanf( moText(strvalue), "%i", &tmp2);
		valuebase.SetInt( tmp2 );
		valuebase.SetType( MO_VALUE_NUM );

	} else if ( (moText)type== moText("INT")) {

		MOint tmp2 = 0;
		sscanf( moText(strvalue), "%i", &tmp2);
		valuebase.SetInt( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_INT );

	} else if ( (moText)type== moText("CHAR")) {

		MOchar tmp2 = 0;
		sscanf( moText(strvalue), "%c", &tmp2);
		valuebase.SetChar( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_CHAR );

	} else if ( (moText)type== moText("LONG")) {

		MOlong tmp2 = 0;
		sscanf( moText(strvalue), "%li", &tmp2);
		valuebase.SetLong( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_LONG );

	} else if ( (moText)type== moText("FLOAT")) {

		MOfloat tmp2 = 0.0f;
		sscanf( moText(strvalue), "%f", &tmp2);
		valuebase.SetFloat( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_FLOAT );

	} else if ( (moText)type== moText("DOUBLE")) {

		MOdouble tmp2 = 0.0;
		sscanf( moText(strvalue), "%lf", &tmp2);
		valuebase.SetDouble( tmp2 );
		valuebase.SetType( MO_VALUE_NUM_DOUBLE );

	} else if ((moText)type== moText("FUNCTION")) {
		//float tmp2;
		//sscanf( strvalue, "%f", &tmp2);
		//valuebase.SetFloat( tmp2 );
		valuebase.SetFun( strvalue );
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

const moText&
moValue::ToJSON() {
  moText fieldSeparation  = "";
  m_FullJSON = "[";
  for( int vbase=0; vbase < (int)m_List.Count(); vbase++) {
    m_FullJSON+= fieldSeparation + m_List[vbase].ToJSON();
    fieldSeparation = ",";
  }
  m_FullJSON+= "]";
  return m_FullJSON;
}

const moText&
moValue::ToXML() {
  moText fieldSeparation  = "";
  m_FullXML = "<moValue>";
  for( int vbase=0; vbase < (int)m_List.Count(); vbase++) {
    m_FullXML+= fieldSeparation + m_List[vbase].ToXML();
    fieldSeparation = "";
  }
  m_FullXML+= "</moValue>";
  return m_FullXML;
}


int
moValue::Set( const moText& p_XmlText ) {

  TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  //TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );
  TiXmlElement* valueNode = m_XMLDoc.FirstChildElement("moValue");

  //if (rootKey) {

    //TiXmlElement* sceneStateNode = rootKey->FirstChildElement("moSceneState");
    if (valueNode) {

      TiXmlElement* valuebaseNode = valueNode->FirstChildElement("moValueBase");
      int vbidx = 0;
      while(valuebaseNode) {
        moValueBase vb;
        moText vbXML;
        TiXmlPrinter printer;
        TiXmlNode* NODEDATAXML = valuebaseNode;
        if (NODEDATAXML) {
          NODEDATAXML->Accept( &printer );
          vbXML = moText( printer.CStr() );
        }
        vb.Set( vbXML );

        if (vbidx<(int)GetSubValueCount()) GetSubValue(vbidx) = vb;
        else AddSubValue( vb );

        valuebaseNode = valuebaseNode->NextSiblingElement("moValueBase");
        vbidx+= 1;
      }
      return 0;
    } else moDebugManager::Log( "No XML moValue in: " + p_XmlText );

  //} else moDebugManager::Error();
  return -1;
}
