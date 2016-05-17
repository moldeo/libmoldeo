/*******************************************************************************

                              moParam.cpp

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

#include "moParam.h"
#include <tinyxml.h>
#include "moDebugManager.h"
#include "moArray.h"

moDefineDynamicArray(moParamDefinitions)
moDefineDynamicArray(moParams)
moDefineDynamicArray( moParamIndexes )

//================================================================
//	moParamDefinition
//================================================================

moParamDefinition::moParamDefinition() {
    m_Name = moText("");
    m_Index = -1;
    m_Type = MO_PARAM_UNDEFINED;
    m_Property = "";
    m_OptionsStr = "";
}

moParamDefinition::moParamDefinition( const moParamDefinition &src) {
	*this = src;
}

moParamDefinition::moParamDefinition( const moText& p_name, moParamType p_type ) {
		m_Name = p_name;
		m_Type = p_type;
		m_Index = -1;
    m_Property = "";
    m_OptionsStr = "";
}

moParamType
moParamDefinition::ParamTypeFromStr( const moText& p_type ) {

    moParamType p_Type;

    if ( p_type == moText("ALPHA") ) {
			p_Type = MO_PARAM_ALPHA;
		} else
		if ( p_type == moText("COLOR") ) {
			p_Type = MO_PARAM_COLOR;
		} else
		if ( p_type == moText("BLENDING") ) {
			p_Type = MO_PARAM_BLENDING;
		} else
		if ( p_type == moText("POLYGONMODE") ) {
			p_Type = MO_PARAM_POLYGONMODE;
		} else
		if ( p_type == moText("SYNC") ) {
			p_Type = MO_PARAM_SYNC;
		} else
		if ( p_type == moText("PHASE") ) {
			p_Type = MO_PARAM_PHASE;
		} else
		if ( p_type == moText("TEXT") ) {
			p_Type = MO_PARAM_TEXT;
		} else
		if ( p_type == moText("TEXTURE") ) {
			p_Type = MO_PARAM_TEXTURE;
		} else
		if ( p_type == moText("TEXTUREFOLDER") ) {
			p_Type = MO_PARAM_TEXTUREFOLDER;
		} else
		if ( p_type == moText("FONT") ) {
			p_Type = MO_PARAM_FONT;
		} else
		if ( p_type == moText("3DMODEL") ) {
			p_Type = MO_PARAM_3DMODEL;
		} else
		if ( p_type == moText("MOLDEOOBJECT") ) {
			p_Type = MO_PARAM_MOLDEO_OBJECT;
		} else
		if ( p_type == moText("OBJECT") ) {
			p_Type = MO_PARAM_OBJECT;
		} else
		if ( p_type == moText("VIDEO") ) {
			p_Type = MO_PARAM_VIDEO;
		} else
        if ( p_type == moText("FILTER") ) {
			p_Type = MO_PARAM_FILTER;
		} else
		if ( p_type == moText("SOUND") ) {
			p_Type = MO_PARAM_SOUND;
		} else
		if ( p_type == moText("NUM") ) {
			p_Type = MO_PARAM_NUMERIC;
		} else
		if ( p_type == moText("FUNCTION") ) {
			p_Type = MO_PARAM_FUNCTION;
		} else
        if ( p_type == moText("TRANSLATEX") ) {
			p_Type = MO_PARAM_TRANSLATEX;
		} else
        if ( p_type == moText("TRANSLATEY") ) {
			p_Type = MO_PARAM_TRANSLATEY;
		} else
        if ( p_type == moText("TRANSLATEZ") ) {
			p_Type = MO_PARAM_TRANSLATEZ;
		} else
        if ( p_type == moText("SCALEX") ) {
			p_Type = MO_PARAM_SCALEX;
		} else
        if ( p_type == moText("SCALEY") ) {
			p_Type = MO_PARAM_SCALEY;
		} else
        if ( p_type == moText("SCALEZ") ) {
			p_Type = MO_PARAM_SCALEZ;
		} else
        if ( p_type == moText("ROTATEX") ) {
			p_Type = MO_PARAM_ROTATEX;
		} else
        if ( p_type == moText("ROTATEY") ) {
			p_Type = MO_PARAM_ROTATEY;
		} else
        if ( p_type == moText("ROTATEZ") ) {
			p_Type = MO_PARAM_ROTATEZ;
		} else
		if ( p_type == moText("SCRIPT") ) {
			p_Type = MO_PARAM_SCRIPT;
		} else
		if ( p_type == moText("COMPOSE") ) {
			p_Type = MO_PARAM_COMPOSE;
		} else
		if ( p_type == moText("VECTOR") ) {
			p_Type = MO_PARAM_VECTOR;
		} else
		if ( p_type == moText("INLET") ) {
			p_Type = MO_PARAM_INLET;
		} else
		if ( p_type == moText("OUTLET") ) {
			p_Type = MO_PARAM_OUTLET;
		} else
		if ( p_type == moText("UNDEFINED") ) {
			p_Type = MO_PARAM_UNDEFINED;
		}

  return p_Type;

}

bool
moParamDefinition::IsValid() const {
    return (m_Type!=MO_PARAM_UNDEFINED);
}
/*
MO_PARAM_ALPHA,			//value type: NUM or FUNCTION
	MO_PARAM_COLOR,			//value type: NUM[4] or FUNCTION[4] or
	MO_PARAM_SYNC,			//value type: NUM or FUNCTION
	MO_PARAM_TEXT,			//value type: TXT or LNK
	MO_PARAM_TEXTURE,		//value type: TXT or LNK
	MO_PARAM_FONT,		//value type: TXT or LNK
	MO_PARAM_VIDEO,		//value type: TXT or LNK
	MO_PARAM_SOUND,		//value type: TXT or LNK
	MO_PARAM_FUNCTION,		//value type: NUM or FUNCTION
	MO_PARAM_SCRIPT,		//value type: TXT or LNK
	MO_PARAM_TRA,			//value type: NUM[3] or FUNCTION[3]
	MO_PARAM_ROT,			//value type: NUM[3] or FUNCTION[3]
	MO_PARAM_SCA			//value type: NUM[3] or FUNCTION[3]
	MO_PARAM_INLET,			//value type: TXT or LNK
	MO_PARAM_OUTLET			//value type: TXT or LNK
*/
moParamDefinition::moParamDefinition( const moText& p_name,
                                      const moText& p_type,
                                      const moText& p_property,
                                      const moText& p_group,
                                      const moText& p_interpolation,
                                      const moText& p_duration,
                                      const moText& p_optionsstr ) {

		m_Name = p_name;

		m_Property = p_property;
		m_Group = p_group;

		bool valid_interpolation = false;
		if ( p_type == moText("ALPHA") ) {
			m_Type = MO_PARAM_ALPHA;
			valid_interpolation = true;
		} else
		if ( p_type == moText("COLOR") ) {
			m_Type = MO_PARAM_COLOR;
			valid_interpolation = false;// TODO: do it!
		} else
		if ( p_type == moText("BLENDING") ) {
			m_Type = MO_PARAM_BLENDING;
		} else
		if ( p_type == moText("POLYGONMODE") ) {
			m_Type = MO_PARAM_POLYGONMODE;
		} else
		if ( p_type == moText("SYNC") ) {
			m_Type = MO_PARAM_SYNC;
			valid_interpolation = true;
		} else
		if ( p_type == moText("PHASE") ) {
			m_Type = MO_PARAM_PHASE;
			valid_interpolation = true;
		} else
		if ( p_type == moText("TEXT") ) {
			m_Type = MO_PARAM_TEXT;
		} else
		if ( p_type == moText("TEXTURE") ) {
			m_Type = MO_PARAM_TEXTURE;
		} else
		if ( p_type == moText("TEXTUREFOLDER") ) {
			m_Type = MO_PARAM_TEXTUREFOLDER;
		} else
		if ( p_type == moText("FONT") ) {
			m_Type = MO_PARAM_FONT;
		} else
		if ( p_type == moText("3DMODEL") ) {
			m_Type = MO_PARAM_3DMODEL;
		} else
		if ( p_type == moText("MOLDEOOBJECT") ) {
			m_Type = MO_PARAM_MOLDEO_OBJECT;
		} else
		if ( p_type == moText("OBJECT") ) {
			m_Type = MO_PARAM_OBJECT;
		} else
		if ( p_type == moText("VIDEO") ) {
			m_Type = MO_PARAM_VIDEO;
		} else
        if ( p_type == moText("FILTER") ) {
			m_Type = MO_PARAM_FILTER;
		} else
		if ( p_type == moText("SOUND") ) {
			m_Type = MO_PARAM_SOUND;
		} else
		if ( p_type == moText("NUM") ) {
			m_Type = MO_PARAM_NUMERIC;
			valid_interpolation = true;
		} else
		if ( p_type == moText("FUNCTION") ) {
			m_Type = MO_PARAM_FUNCTION;
			valid_interpolation = true;
		} else
        if ( p_type == moText("TRANSLATEX") ) {
			m_Type = MO_PARAM_TRANSLATEX;
			valid_interpolation = true;
		} else
        if ( p_type == moText("TRANSLATEY") ) {
			m_Type = MO_PARAM_TRANSLATEY;
			valid_interpolation = true;
		} else
        if ( p_type == moText("TRANSLATEZ") ) {
			m_Type = MO_PARAM_TRANSLATEZ;
			valid_interpolation = true;
		} else
        if ( p_type == moText("SCALEX") ) {
			m_Type = MO_PARAM_SCALEX;
			valid_interpolation = true;
		} else
        if ( p_type == moText("SCALEY") ) {
			m_Type = MO_PARAM_SCALEY;
			valid_interpolation = true;
		} else
        if ( p_type == moText("SCALEZ") ) {
			m_Type = MO_PARAM_SCALEZ;
			valid_interpolation = true;
		} else
        if ( p_type == moText("ROTATEX") ) {
			m_Type = MO_PARAM_ROTATEX;
			valid_interpolation = true;
		} else
        if ( p_type == moText("ROTATEY") ) {
			m_Type = MO_PARAM_ROTATEY;
			valid_interpolation = true;
		} else
        if ( p_type == moText("ROTATEZ") ) {
			m_Type = MO_PARAM_ROTATEZ;
			valid_interpolation = true;
		} else
		if ( p_type == moText("SCRIPT") ) {
			m_Type = MO_PARAM_SCRIPT;
		} else
		if ( p_type == moText("COMPOSE") ) {
			m_Type = MO_PARAM_COMPOSE;
		} else
		if ( p_type == moText("VECTOR") ) {
			m_Type = MO_PARAM_VECTOR;
		} else
		if ( p_type == moText("INLET") ) {
			m_Type = MO_PARAM_INLET;
		} else
		if ( p_type == moText("OUTLET") ) {
			m_Type = MO_PARAM_OUTLET;
		} else
		if ( p_type == moText("UNDEFINED") ) {
			m_Type = MO_PARAM_UNDEFINED;
		}

		m_Index = -1;

    moParamInterpolation paraminter;
    if (valid_interpolation) {
            //cout << " moParamDefinition(..): function: " << p_interpolation << " duration: " << p_duration << endl;
            if ( p_interpolation == "linear" ) {
                //cout << " moParamDefinition(..): interpolation recognized: [" << p_interpolation << "]";
                paraminter.SetInterpolation( MO_INTERPOLATION_LINEAR, p_duration );
                //cout << " saved as:" << paraminter.GetFunctionToText() << endl;
                m_Interpolation = paraminter;
            } else if (p_interpolation==moText("easeinoutquad") ) {
                paraminter.SetInterpolation( MO_INTERPOLATION_EASEINOUTQUAD, p_duration );
                m_Interpolation = paraminter;
            } else if (p_interpolation==moText("easeinoutsin") ) {
                paraminter.SetInterpolation( MO_INTERPOLATION_EASEINOUTSIN, p_duration );
                m_Interpolation = paraminter;
            } else {
              //cout << " moParamDefinition(..): interpolation unrecognized: [" << p_interpolation << "]" << endl;
              paraminter.SetInterpolation( MO_INTERPOLATION_NONE, "0" );
              m_Interpolation = paraminter;
              //cout << "NO INTERPOLATION PARAMETERS: [" << p_interpolation << "] [" << p_duration << "]" << endl;
            }
    }// else cout << "INVALID type of parameter: " << p_type << endl;

    if ( p_optionsstr.Length() > 0  ) {
        m_OptionsStr = p_optionsstr;
        m_Options = p_optionsstr.Explode(moText(","));
    }

}

moParamDefinition::moParamDefinition( const moText& p_name,
                                      const moText& p_type,
                                      const moText& p_property,
                                      const moText& p_group,
                                      const moText& p_interpolation,
                                      const moText& p_duration,
                                      const moTextArray& p_Options ) {
  (*this) = moParamDefinition(p_name,p_type,p_property,p_group,p_interpolation,p_duration);
  m_Options = p_Options;
}


moParamDefinition::~moParamDefinition() {
}

moParamDefinition &moParamDefinition::operator = (const moParamDefinition &src)
{
		m_Name = src.m_Name;
		m_Type = src.m_Type;
		m_Index = src.m_Index;
		m_DefaultValue = src.m_DefaultValue;
		m_Interpolation = src.m_Interpolation;
		m_Property = src.m_Property;
		m_Group = src.m_Group;
		m_OptionsStr = src.m_OptionsStr;
		m_Options = src.m_Options;
		return *this;
}


moText
moParamDefinition::GetTypeStr() const {
    switch(m_Type) {
        case MO_PARAM_UNDEFINED:
            return moText("UNDEFINED");
            break;
        case MO_PARAM_ALPHA:
            return moText("ALPHA");
            break;
        case MO_PARAM_COLOR:
            return moText("COLOR");
            break;
        case MO_PARAM_BLENDING:
            return moText("BLENDING");
            break;
        case MO_PARAM_POLYGONMODE:
            return moText("POLYGONMODE");
            break;
        case MO_PARAM_SYNC:
            return moText("SYNC");
            break;
        case MO_PARAM_PHASE:
            return moText("PHASE");
            break;
        case MO_PARAM_TEXT:
            return moText("TEXT");
            break;
        case MO_PARAM_TEXTURE:
            return moText("TEXTURE");
            break;
        case MO_PARAM_TEXTUREFOLDER:
            return moText("TEXTUREFOLDER");
            break;
        case MO_PARAM_FONT:
            return moText("FONT");
            break;
        case MO_PARAM_3DMODEL:
            return moText("3DMODEL");
            break;
        case MO_PARAM_MOLDEO_OBJECT:
            return moText("MOLDEOOBJECT");
            break;
        case MO_PARAM_OBJECT:
            return moText("OBJECT");
            break;
        case MO_PARAM_FILTER:
            return moText("FILTER");
            break;
        case MO_PARAM_NUMERIC:
            return moText("NUM");
            break;
        case MO_PARAM_FUNCTION:
            return moText("FUNCTION");
            break;
        case MO_PARAM_TRANSLATEX:
            return moText("TRANSLATEX");
            break;
        case MO_PARAM_TRANSLATEY:
            return moText("TRANSLATEY");
            break;
        case MO_PARAM_TRANSLATEZ:
            return moText("TRANSLATEZ");
            break;
        case MO_PARAM_SCALEX:
            return moText("SCALEX");
            break;
        case MO_PARAM_SCALEY:
            return moText("SCALEY");
            break;
        case MO_PARAM_SCALEZ:
            return moText("SCALEZ");
            break;
        case MO_PARAM_ROTATEX:
            return moText("ROTATEX");
            break;
        case MO_PARAM_ROTATEY:
            return moText("ROTATEY");
            break;
        case MO_PARAM_ROTATEZ:
            return moText("ROTATEZ");
            break;
        case MO_PARAM_SCRIPT:
            return moText("SCRIPT");
            break;
        case MO_PARAM_SOUND:
            return moText("SOUND");
            break;
        case MO_PARAM_VIDEO:
            return moText("VIDEO");
            break;
        case MO_PARAM_COMPOSE:
            return moText("COMPOSE");
            break;
        case MO_PARAM_VECTOR:
            return moText("VECTOR");
            break;
        case MO_PARAM_INLET:
            return moText("INLET");
            break;
        case MO_PARAM_OUTLET:
            return moText("OUTLET");
            break;
        case MO_PARAM_FILE:
            return moText("FILE");
            break;
        default:
            break;
    }
    return moText("UNDEFINED");
}


void
moParamDefinition::SetType( moParamType p_ParamType ) {

  m_Type = p_ParamType;

}

void
moParamDefinition::SetDefault( const moValue& p_defaultvalue ) {

  moValue Default = p_defaultvalue;
  m_DefaultValue = p_defaultvalue;

  if (  Default.GetSubValue().GetType()==MO_VALUE_UNDEFINED  ) {
    switch ( GetType()  ) {

      case MO_PARAM_TEXT:
      case MO_PARAM_TEXTURE:
      case MO_PARAM_TEXTUREFOLDER:
      case MO_PARAM_FILTER:
      case MO_PARAM_VIDEO:
      case MO_PARAM_SOUND:
      case MO_PARAM_FILE:
      case MO_PARAM_SCRIPT:
      case MO_PARAM_OBJECT:
      case MO_PARAM_3DMODEL:
        m_DefaultValue = moValue( "", "TXT" );
        break;

      case MO_PARAM_MOLDEO_OBJECT:
        /** TODO: removing subvalues if any*/
        m_DefaultValue.RemoveSubValues();
        break;

      case MO_PARAM_INLET:
        m_DefaultValue = moValue( "VARIABLE1", "TXT", "NUMERIC", "TXT" );
        break;

      case MO_PARAM_FONT:
        m_DefaultValue = moValue( "", "TXT", "", "TXT", "", "TXT" );
        break;

      case MO_PARAM_PHASE:
      case MO_PARAM_ROTATEX:
      case MO_PARAM_ROTATEY:
      case MO_PARAM_ROTATEZ:
      case MO_PARAM_TRANSLATEX:
      case MO_PARAM_TRANSLATEY:
      case MO_PARAM_TRANSLATEZ:
      case MO_PARAM_FUNCTION:
        m_DefaultValue = moValue( "0.0", "FUNCTION" ).Ref();
        break;

      case MO_PARAM_ALPHA:
      case MO_PARAM_SYNC:
      case MO_PARAM_SCALEX:
      case MO_PARAM_SCALEY:
      case MO_PARAM_SCALEZ:
        m_DefaultValue = moValue( "1.0", "FUNCTION" ).Ref();
        break;

      case MO_PARAM_POLYGONMODE:
      case MO_PARAM_BLENDING:
        m_DefaultValue = moValue( "0", "INT" );
        break;
      case MO_PARAM_COLOR:
        m_DefaultValue = moValue( "1.0", "FUNCTION","1.0", "FUNCTION","1.0", "FUNCTION","1.0", "FUNCTION"  );
        break;
      case MO_PARAM_COMPOSE:
        m_DefaultValue = moValue( "composed by", "TXT","1.0", "FUNCTION","<nada></nada>", "XML","12345", "INT"  );
        break;
      case MO_PARAM_VECTOR:
        m_DefaultValue = moValue( "1.0", "FUNCTION","2.0", "FUNCTION","3.0", "FUNCTION","4.0", "FUNCTION" );
        break;
      case MO_PARAM_NUMERIC:
        m_DefaultValue = moValue( "0", "NUM" );
        break;
      case MO_PARAM_UNDEFINED:
        m_DefaultValue = moValue( "INVALID", MO_VALUE_UNDEFINED );
        break;
      default:
        break;
    };
  }

}


void
moParamDefinition::SetOptions( const moTextArray& p_options ) {
  m_Options = p_options;
  m_OptionsStr = "";
  moText comma = "";
  for(int i=0; i<(int)m_Options.Count(); i++ ) {
    m_OptionsStr+= comma + m_Options[i];
    comma = ",";
  }
}

void
moParamDefinition::SetOptions( const moText& p_OptionsStr ) {
  m_OptionsStr = p_OptionsStr;
  m_Options = m_OptionsStr.Explode(moText(","));
}



const moText&
moParamDefinition::ToJSON() {
  moText fieldseparation  = ",";

  m_FullJSON = "{";
  m_FullJSON+= "'name': '"+ GetName()+"'";
  m_FullJSON+= fieldseparation + "'type': '"+ GetTypeStr()+"'";
  m_FullJSON+= fieldseparation + "'index': '"+ IntToStr(GetIndex())+"'";
  m_FullJSON+= fieldseparation + "'property': '"+ GetProperty()+"'";
  m_FullJSON+= fieldseparation + "'group': '"+ GetGroup()+"'";
  m_FullJSON+= fieldseparation + "'interpolation': "+ GetInterpolation().ToJSON();
  m_FullJSON+= fieldseparation + "'options': [";
  fieldseparation = "";
  for(int i=0; i<(int)m_Options.Count(); i++ ) {
    moText ostr( m_Options[ i ] );
    ostr.Replace( "'","&apos;" );
    //std::string option_str = (char*)m_Options[ i ];
    //option_str.replace( option_str.begin(), option_str.end(), "\'", "&apos;"  );
    //option_str.replace( option_str.begin(), option_str.end(), "\"", "&quot;"  );
    //moText ostr( option_str.c_str());
    m_FullJSON+= fieldseparation + "'"+ostr+"'";
    fieldseparation = ",";
  }
  m_FullJSON+= "]";

  m_FullJSON+= "}";

  return m_FullJSON;
}

const moText&
moParamDefinition::ToXML() {
  moText fieldseparation  = " ";

  m_FullXML = "<moParamDefinition ";
  m_FullXML+= "name='"+ GetName()+"'";
  m_FullXML+= fieldseparation + "type='"+ GetTypeStr()+"'";
  m_FullXML+= fieldseparation + "index='"+ IntToStr(GetIndex())+"'";
  m_FullXML+= fieldseparation + "property='"+ GetProperty()+"'";
  m_FullXML+= fieldseparation + "options='"+ GetOptionsStr()+"'";
  m_FullXML+= fieldseparation + "group='"+ GetGroup()+"'";
  m_FullXML+= fieldseparation + "interpolation='"+ GetInterpolation().GetFunctionToText()+"'";
  m_FullXML+= fieldseparation + "duration='"+ IntToStr(GetInterpolation().GetDuration())+"' >";
  m_FullXML+= "</moParamDefinition>";

  return m_FullXML;
}

int
moParamDefinition::Set( const moText& p_XmlText ) {
 TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  //TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );
  TiXmlElement* definitionNode = m_XMLDoc.FirstChildElement("moParamDefinition");

  //if (rootKey) {

    //TiXmlElement* sceneStateNode = rootKey->FirstChildElement("moSceneState");
    if (definitionNode) {
      m_Name = moText( definitionNode->Attribute("name") );
      m_Type = ParamTypeFromStr( moText(definitionNode->Attribute("type")) );
      m_Index = atoi(moText( definitionNode->Attribute("index") ));
      m_Property = moText( definitionNode->Attribute("property") );
      m_OptionsStr = moText( definitionNode->Attribute("options") );
      m_Group = moText( definitionNode->Attribute("group") );
      m_Interpolation.SetInterpolation(  moText( definitionNode->Attribute("interpolation")),  moText( definitionNode->Attribute("duration")) );
      return 0;
    } else moDebugManager::Log( "No XML moEffectState in: " + p_XmlText );

  //} else moDebugManager::Error();
  return -1;
}

//================================================================
//	moParamInterpolation
//================================================================


moParamInterpolation::moParamInterpolation() :
    m_bIsOn(false), m_Duration(0), m_Function(MO_INTERPOLATION_NONE)
{
}

moParamInterpolation::moParamInterpolation(const moParamInterpolation &src) {
    *this = src;
}

moParamInterpolation::~moParamInterpolation() {

}

moParamInterpolation &
moParamInterpolation::operator=(const moParamInterpolation &src) {
    m_bIsOn = src.m_bIsOn;
    m_DataIn = src.m_DataIn;
    m_DataOut = src.m_DataOut;
    m_DataInterpolated = src.m_DataInterpolated;

    m_Timer = (moTimer&)src.m_Timer;
    m_Duration = src.m_Duration;
    m_Function = src.m_Function;

    return(*this);
}

void
moParamInterpolation::StartInterpolation( const moData& p_data_in, const moData& p_data_out ) {
    m_DataIn = p_data_in;
    m_DataOut = p_data_out;
    m_DataIn.Eval();
    m_DataOut.Eval();
    m_DataInterpolated = p_data_in;
    m_Timer.Start();
}

void
moParamInterpolation::StartInterpolation( const moValue& p_value_in, const moValue& p_value_out ) {
    m_ValueIn = p_value_in;
    m_ValueOut = p_value_out;
    //m_ValueIn.Eval();
    ///m_ValueOut.Eval();
    m_ValueInterpolated = m_ValueIn;
    m_Timer.Start();
}

void
moParamInterpolation::SetDuration(  MOlong p_fun_duration ) {
    m_Duration = p_fun_duration;
}

MOlong
moParamInterpolation::GetDuration() {
    return m_Duration;
}

void
moParamInterpolation::StopInterpolation() {
    m_Timer.Stop();
}

void
moParamInterpolation::SetInterpolationFunction( const moText& p_interpol_fun ) {
  SetInterpolation( p_interpol_fun, IntToStr(GetDuration()) );
}


void
moParamInterpolation::SetInterpolation( const moText& p_interpol_fun,
                                const moText& p_fun_duration,
                                const moText& p_fun_expression) {
            if ( p_interpol_fun == "linear" ) {
                SetInterpolation( MO_INTERPOLATION_LINEAR, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("ease") ) {
                SetInterpolation( MO_INTERPOLATION_EASE, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("easein") ) {
                SetInterpolation( MO_INTERPOLATION_EASE, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("easeout") ) {
                SetInterpolation( MO_INTERPOLATION_EASE, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("easeinout") ) {
                SetInterpolation( MO_INTERPOLATION_EASEINOUT, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("stepstart") ) {
                SetInterpolation( MO_INTERPOLATION_STEPSTART, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("stepend") ) {
                SetInterpolation( MO_INTERPOLATION_STEPEND, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("expression") ) {
                SetInterpolation( MO_INTERPOLATION_EXPRESSION, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("easeinoutquad") ) {
                SetInterpolation( MO_INTERPOLATION_EASEINOUTQUAD, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("easeinoutsin") ) {
                SetInterpolation( MO_INTERPOLATION_EASEINOUTSIN, p_fun_duration, p_fun_expression );

            } else if (p_interpol_fun==moText("easeinoutcubic") ) {
              SetInterpolation( MO_INTERPOLATION_EASEINOUTCUBIC, p_fun_duration, p_fun_expression );

            } else {
              SetInterpolation( MO_INTERPOLATION_NONE, "0", p_fun_expression );
            }

}


moText
moParamInterpolation::GetFunctionToText() {
  switch(m_Function) {
      case MO_INTERPOLATION_NONE:
          return moText("none");
          break;
      case MO_INTERPOLATION_LINEAR:
          return moText("linear");
          break;
      case MO_INTERPOLATION_EASE:
          return moText("ease");
          break;
      case MO_INTERPOLATION_EASEIN:
          return moText("easein");
          break;
      case MO_INTERPOLATION_EASEOUT:
          return moText("easeout");
          break;
      case MO_INTERPOLATION_EASEINOUT:
          return moText("easeinout");
          break;
      case MO_INTERPOLATION_CUBICBEZIER:
          return moText("cubicbezer");
          break;
      case MO_INTERPOLATION_EASEINOUTCUBIC:
          return moText("easeinoutcubic");
          break;
      case MO_INTERPOLATION_STEPSTART:
          return moText("stepstart");
          break;
      case MO_INTERPOLATION_STEPEND:
          return moText("stepend");
          break;
      case MO_INTERPOLATION_EASEINOUTQUAD:
          return moText("easeinoutquad");
          break;
      case MO_INTERPOLATION_EXPRESSION:
          return moText("expression");
          break;
      case MO_INTERPOLATION_EASEINOUTSIN:
          return moText("easeinoutsin");
          break;
      default:
          return moText("");
          break;
  }
  return moText("");

}

void
moParamInterpolation::SetInterpolation( moParamInterpolationFunction p_interpol_fun,
                                const moText& p_fun_duration,
                                const moText& p_fun_expression) {

  if (p_fun_expression!=moText("")) {
    cout << "p_fun_expression: " << p_fun_expression << endl;
  }

  if (p_fun_duration!="") {
   moParserFunction mp_dur;
    mp_dur.Init( p_fun_duration );
    m_Duration = mp_dur.Eval();
  }
   /* moParserFunction mp_exp;*/
    //moParserFunction* mp_exp = new moParserFunction();
    /*mp_exp.Init( p_fun_expression );*/
    //setting
    m_Function = p_interpol_fun;
//    cout << "SetInterpolation: settting duration: " << p_fun_duration << " is finally m_Duration:" << m_Duration << endl;
//    cout << " p_fun_expression:" << p_fun_expression << endl;
    Activate();
//    cout << "SetInterpolation: Activated: " << IsOn() << endl;
}

bool
moParamInterpolation::IsOn() const {
    return m_bIsOn;
}

void
moParamInterpolation::Activate() {
    m_bIsOn = true;
}


const moTimer&
moParamInterpolation::GetTimer() const {
    return m_Timer;
}



/*

Easing functions in c++

http://www.gizma.com/easing/#l

*/

double moParamInterpolation::bezier_x( double t, double Ax, double Bx, double Cx) {
   return t * (Cx + t * (Bx + t * Ax));
}

double moParamInterpolation::bezier_y( double t, double Ay, double By, double Cy) {
  return t * (Cy + t * (By + t * Ay));
}

double moParamInterpolation::bezier_x_der( double t, double Ax, double Bx, double Cx) {
  return Cx + t * (2*Bx + 3*Ax * t);
}

double moParamInterpolation::find_x_for( double t, double Ax, double Bx, double Cx ) {

  double x=t, i=0, z;

  while (i < 5) { // making 5 iterations max
    z = bezier_x(x, Ax, Bx, Cx) - t;

    if ( fabs(z) < 0.001) break; // if already got close enough

    x = x - z/bezier_x_der(x, Ax, Bx, Cx);
    i++;
  }
//xx
  return x;
}

double moParamInterpolation::cubic_bezier( double t, double p1, double p2, double p3, double p4 ) {

  double Cx = 3 * p1;
  double Bx = 3 * (p3 - p1) - Cx;
  double Ax = 1 - Cx - Bx;

  double Cy = 3 * p2;
  double By = 3 * (p4 - p2) - Cy;
  double Ay = 1 - Cy - By;

  return bezier_y( find_x_for( t, Ax, Bx, Cx ), Ay, By, Cy );
}

moData*
moParamInterpolation::InterpolateData( moData& pParamData  ) {

    if (m_Function==MO_INTERPOLATION_NONE) return &pParamData;

    //FIRST treat the UNDEFINED... starting
    if (   m_DataIn.Type() == MO_DATA_UNDEFINED
        || m_DataOut.Type() == MO_DATA_UNDEFINED ) {
        ///cout << "moParamInterpolation::InterpolateData > datain or out UNDEFINED : resetting " << endl;
        //no in or out are set...
        StopInterpolation();
        //set them...
        StartInterpolation( pParamData, pParamData );
    } else {

    //RUNNING; NEW DATA COMING!!

        //it target is different just set with it the new Out
        if ( m_DataOut.ToText() != pParamData.ToText() ) {
            StopInterpolation();
            StartInterpolation( m_DataInterpolated, pParamData );
            ///cout << "moParamInterpolation::InterpolateData > StartInterpolation: " << m_DataIn.ToText() << "  --> TO --> m_DataOut: " << m_DataOut.ToText() << endl;
        }

        m_DataIn.Eval();//so it records last val
        m_DataOut.Eval();//so it records his last val
        //cout << "m_DataIn: " << m_DataIn.LastEval() << "  --> TO --> m_DataOut: " << m_DataOut.LastEval() << endl;

        double RDouble;
        //double differential = 1.0f* m_Timer.Duration() / (1.0f*m_Duration);

        //process here with some function
        double c = (double) ( m_DataOut.LastEval() - m_DataIn.LastEval() );
        double t = (double) m_Timer.Duration();
        double d = (double) m_Duration;
        double b = (double) m_DataIn.LastEval();



        if (d>=0.0f && t<=d && t>=0.0f) {
          switch(m_Function) {

              //cases based on http://www.gizma.com/easing/

              case MO_INTERPOLATION_LINEAR:
                  RDouble = c * t/d + b;
                  break;

              case MO_INTERPOLATION_EASEINOUTQUAD:
                  RDouble = c* cubic_bezier( t/d, 0.42, 0.0, 0.58, 1.0 ) + b;
                  break;

              case MO_INTERPOLATION_EASEINOUTSIN:
                  RDouble = -c/2 * ( moMathd::Cos( moMathd::PI*t/d) - 1) + b;
                  break;
              case MO_INTERPOLATION_EXPRESSION:
                  break;
              default:
                cout << "moParamInterpolation::InterpolateData > nterpolation function doesn't exists for " << m_Function << endl;
                break;
          }
        } else {
          RDouble = m_DataOut.LastEval();
        }
        m_DataInterpolated.SetDouble(RDouble);

    }

    return &m_DataInterpolated;

}

moValue*
moParamInterpolation::InterpolateValue( moValue& pParamValue  ) {

  if (m_Function==MO_INTERPOLATION_NONE) return &pParamValue;

  return &m_ValueInterpolated;

}



//================================================================
//	moParam
//================================================================


moParam::moParam() {
	m_CurrentIndexValue = 0;
	m_bExternDataUpdated = false;
	m_pExternData = NULL;
}

moParam::moParam(const moParam &src) {
	*this = src;
}

moParam::moParam( moParamDefinition &p_paramdefinition) {
    m_CurrentIndexValue = 0;
	m_bExternDataUpdated = false;
	m_pExternData = NULL;
	m_ParamDefinition = p_paramdefinition;

}

moParam::~moParam() {

}

moParam &moParam::operator = (const moParam &src) {
		m_bExternDataUpdated = src.m_bExternDataUpdated;
		m_pExternData = src.m_pExternData;
		m_ParamDefinition = src.m_ParamDefinition;
		m_Values = src.m_Values;
		m_CurrentIndexValue = src.m_CurrentIndexValue;
		return *this;
}

moValues&
moParam::GetValues() {
	return m_Values;
}

MOuint
moParam::GetValuesCount() const {
	return m_Values.Count();
}


void
moParam::AddValue( moValue &value ) {
	m_Values.Add( value );
}


void
moParam::DeleteValue( MOint i ) {
	m_Values.Remove(i);
}

void moParam::SetDefaultValue() {

    //atencion: siempre setear el tipo despues de pasar el valor del texto...
    if (this->GetValuesCount()==0) {
        moValue xvalue;
        moValueBase valuebase;

        switch( (int)m_ParamDefinition.GetType() ) {
            case MO_PARAM_COLOR:
                valuebase.SetText( "1.0" );
                valuebase.SetType( MO_VALUE_FUNCTION );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_BLENDING:
            case MO_PARAM_POLYGONMODE:
                valuebase.SetText( "0" );
                valuebase.SetInt(0);
                valuebase.SetType( MO_VALUE_NUM );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_ALPHA:
            case MO_PARAM_SYNC:
            case MO_PARAM_SCALEX:
            case MO_PARAM_SCALEY:
            case MO_PARAM_SCALEZ:
                valuebase.SetText( "1.0" );
                valuebase.SetType( MO_VALUE_FUNCTION );
                xvalue.AddSubValue( valuebase );
                break;

            case MO_PARAM_FUNCTION:
            case MO_PARAM_PHASE:
            case MO_PARAM_TRANSLATEX:
            case MO_PARAM_TRANSLATEY:
            case MO_PARAM_TRANSLATEZ:
            case MO_PARAM_ROTATEX:
            case MO_PARAM_ROTATEY:
            case MO_PARAM_ROTATEZ:
                valuebase.SetText( "0.0" );
                valuebase.SetType( MO_VALUE_FUNCTION );
                xvalue.AddSubValue( valuebase );
                break;

            case MO_PARAM_TEXTURE:
                valuebase.SetText( "default" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_3DMODEL:
            case MO_PARAM_OBJECT:
            case MO_PARAM_VIDEO:
            case MO_PARAM_TEXTUREFOLDER:
            case MO_PARAM_SOUND:
            case MO_PARAM_SCRIPT:
            case MO_PARAM_TEXT:
                if ( GetParamDefinition().GetName()=="effect"
                    ||
                    GetParamDefinition().GetName()=="preeffect"
                    ||
                    GetParamDefinition().GetName()=="posteffect"
                    ||
                    GetParamDefinition().GetName()=="mastereffect"
                    ||
                    GetParamDefinition().GetName()=="devices"
                    ||
                    GetParamDefinition().GetName()=="resources" ) {
                      return;
                    }
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_MOLDEO_OBJECT:
              return;
              break;
            case MO_PARAM_FONT:
                valuebase.SetText( "fonts/Tuffy.ttf" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_FILTER:
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_INLET:
            case MO_PARAM_OUTLET:
                //valuebase.SetText( "" );
                //valuebase.SetType( MO_VALUE_TXT );
                //xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_NUMERIC:
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_NUM );
                valuebase.SetInt(0);
                break;
            case MO_PARAM_COMPOSE:
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_VECTOR:
                valuebase.SetText( "0.0" );
                valuebase.SetType( MO_VALUE_NUM_FLOAT );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;

        }

        AddValue(xvalue);
    }
}

moValue&
moParam::GetValue( MOint i ) {
//TODO: apply interpolation if needed
	if ( i == -1 )
		i = m_CurrentIndexValue;

	if (0<=i && i<(MOint)m_Values.Count() )
		return m_Values[i];
	else
		return m_Values[m_CurrentIndexValue];
}

void
moParam::SetIndexValue( int indexvalue ) {

	if (0<=indexvalue && indexvalue<(MOint)m_Values.Count() ) {
		m_CurrentIndexValue = indexvalue;
		m_bExternDataUpdated = false;
	}

}


int
moParam::GetIndexValue() const {

	return m_CurrentIndexValue;

}

void
moParam::NextValue() {
	if ( m_Values.Count() > 0 ) {
	    m_bExternDataUpdated = false;
		if ( m_CurrentIndexValue < ((MOint)m_Values.Count()-1) ) {
			m_CurrentIndexValue++;
		}
	} else m_CurrentIndexValue = -1;
}

void
moParam::PrevValue() {
	if ( m_Values.Count() > 0 ) {
	    m_bExternDataUpdated = false;
		if ( m_CurrentIndexValue > 0 ) {
			m_CurrentIndexValue--;
		}
	} else m_CurrentIndexValue = -1;
}

void
moParam::FirstValue() {
	if (m_Values.Count() > 0) {
	    m_bExternDataUpdated = false;
		m_CurrentIndexValue = 0;
	} else m_CurrentIndexValue = -1;
}

void
moParam::SetParamDefinition( moParamDefinition &p_paramdefinition ) {
	m_ParamDefinition = p_paramdefinition;
}


moParamDefinition&
moParam::GetParamDefinition() {

	return m_ParamDefinition;

}

moParam*
moParam::GetPtr() {
	return (this);
}

void
moParam::Update() {
	if (m_pExternData)
		m_bExternDataUpdated = true;
}

moData*
moParam::GetData() {

    moData* pReturnData  = NULL;

    ///this Data is connected and updated by an Outlet Connection
	if (m_pExternData && m_bExternDataUpdated) {
		pReturnData = m_pExternData;
    ///dato original del config
    if (GetParamDefinition().GetName()=="control_roll_angle" ) {
      ///cout << "control_roll_angle: updated externally val: " + FloatToStr(pReturnData->Eval()) << endl;
    }
	} else {
	  //only work for single data values: FUNCTION > float evaluation
    pReturnData = GetValue().GetSubValue().GetData();
	}

  ///Interpolation code (defined in config using attributes: interpolation="linear" duration="1000"
  if ( m_ParamDefinition.GetInterpolation().IsOn() && pReturnData ) {

      /// Eval Data
      pReturnData->Eval();
      ///
      pReturnData = m_ParamDefinition.GetInterpolation().InterpolateData( *pReturnData );
  }

	return pReturnData;
}

void
moParam::SetExternData( moData* p_pExternData) {
	m_pExternData = p_pExternData;
}

bool
moParam::FixType( moParamType p_NewType ) {
  unsigned int v = 0, s = 0;
  bool result = true;
  if ( p_NewType == MO_PARAM_UNDEFINED ) {

      ///JUST CHECK IF moValues and moParamType are coherent
      result = true;

  }
  else
  ///check every values if they can be translated...
  if (p_NewType==MO_PARAM_FUNCTION) {

    switch( m_ParamDefinition.GetType() ) {

      case MO_PARAM_NUMERIC:///OK TO CHANGE AND FIX
        m_ParamDefinition.SetType(p_NewType);

        for( v = 0; v<this->m_Values.Count() ; v++ ) {

          moValue& pValue( this->m_Values.GetRef( v ) );
          for( s = 0; s< pValue.GetSubValueCount(); s++ ) {
            moValueBase& vBase( pValue.GetSubValue(s) );
            result = result && vBase.FixType( MO_VALUE_FUNCTION );
          }

        }
        break;

      default:
          break;

    }
  }


  return result;
}

bool
moParam::FixOptions( moTextArray& p_NewOptions ) {

  m_ParamDefinition.SetOptions( p_NewOptions );
  return m_ParamDefinition.GetOptions().Count()==p_NewOptions.Count();
}


const moText&
moParam::ToJSON() {

      moText fieldSeparation = ",";
      m_fullJSON = "{";
      m_fullJSON+= "'paramdefinition': " + GetParamDefinition().ToJSON();
      m_fullJSON+= fieldSeparation + "'paramindexvalue': " + IntToStr(this->GetIndexValue());
      m_fullJSON+= fieldSeparation + "'paramvalues': [";

      fieldSeparation = "";
      for( int vi = 0; vi < (int)m_Values.Count(); vi++ ) {
        m_fullJSON+= fieldSeparation + m_Values[vi].ToJSON();
        fieldSeparation = ",";
      }
     // m_fullJSON+= fieldSeparation + "'paramvalues': " + ToJSON();
      m_fullJSON+= "]";
      m_fullJSON+= "}";
      return m_fullJSON;

}



