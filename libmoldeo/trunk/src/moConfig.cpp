/*******************************************************************************

                              moConfig.cpp

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

#include "moConfig.h"

#include "moArray.cpp"
moDefineDynamicArray(moParamIndexes)

#include <tinyxml.h>


//================================================================
//	moConfigDefinition
//================================================================

moConfigDefinition::moConfigDefinition() {

}

moConfigDefinition::~moConfigDefinition() {
}

void
moConfigDefinition::Add( moText p_name, moParamType p_type , MOint p_index) {

	moParamDefinition pdef( p_name, p_type );

	pdef.SetIndex( p_index );

	m_ParamDefinitions.Add( pdef );

	m_ParamIndexes.Add( p_index );

}

void
moConfigDefinition::Add( moText p_name, moParamType p_type , MOint p_index, moValue p_defaultvalue ) {

	moParamDefinition pdef( p_name, p_type );

	pdef.SetIndex( p_index );
	pdef.SetDefault( p_defaultvalue );

	m_ParamDefinitions.Add( pdef );

	m_ParamIndexes.Add( p_index );

}
//================================================================
//	moConfig ( XML Config )
//================================================================


moConfig::moConfig() {
	m_CurrentParam = -1;
	m_ConfigLoaded = false;
}

moConfig::~moConfig() {

}

MOboolean moConfig::IsConfigLoaded() {

	return m_ConfigLoaded;

}

int moConfig::LoadConfig( moText p_filename ) {

	TiXmlDocument			m_XMLDocument;

	UnloadConfig();

	if (m_XMLDocument.LoadFile( (char *) p_filename )) {

		//parse the xml for params...

		TiXmlHandle docHandle( &m_XMLDocument );

		TiXmlElement* MOCONFIG = docHandle.FirstChild( "MOCONFIG" ).Element();
//		FirstChild( "DEFINITION" ).Child( "Child", 1 ).Element();
		if ( MOCONFIG )
		{
			MOCONFIG->QueryIntAttribute( "majorversion", &m_MajorVersion);
			MOCONFIG->QueryIntAttribute( "minorversion", &m_MinorVersion);

			TiXmlNode* NODEDEFINITION = MOCONFIG->FirstChild( "DEFINITION" );
			TiXmlElement* DEFINITION = NULL;
			if (NODEDEFINITION) {
				DEFINITION = NODEDEFINITION->ToElement();
			}
			if (DEFINITION) {
				moText objectname( (char*)DEFINITION->Attribute( "name" ) );
				moText objectclass( (char*) DEFINITION->Attribute( "class" ));
				Set( objectname, objectclass );
			} else return MO_PARAM_NOT_FOUND;

			TiXmlNode* NODEUPDATE = MOCONFIG->FirstChild( "UPDATE" );
			TiXmlElement* UPDATE = NULL;
			if (NODEUPDATE) {
				UPDATE = NODEUPDATE->ToElement();
			}

			TiXmlNode* NODECONFIGPARAMS = MOCONFIG->FirstChild( "CONFIGPARAMS" );
			TiXmlElement* CONFIGPARAMS = NULL;
			if (NODECONFIGPARAMS) {
				CONFIGPARAMS = NODECONFIGPARAMS->ToElement();
			}
			if (CONFIGPARAMS) {
				TiXmlNode*  NODEPARAM = CONFIGPARAMS->FirstChild("PARAM");
				TiXmlElement*  PARAM = NULL;
				if (NODEPARAM) {
					PARAM = NODEPARAM->ToElement();
				}
				while(PARAM) {

					moText paramname((char*) PARAM->Attribute( "name"));
					moText paramtype((char*) PARAM->Attribute( "type"));

					moParamDefinition definition( paramname, paramtype );
					moParam	xparam( definition );

					TiXmlElement*  VALUE = NULL;
					TiXmlNode* NODE = PARAM->FirstChild("VAL");
					if (NODE) {
						VALUE = NODE->ToElement();
					}
					while(VALUE) {

						TiXmlElement*  VALUEDATA = NULL;
						TiXmlNode* NODEDATA = VALUE->FirstChild("D");
						if (NODEDATA) VALUEDATA = NODEDATA->ToElement();

						if (VALUEDATA) {
							moText valuetype( (char*) VALUEDATA->Attribute("type") );
							moText valuecodename( (char*) VALUEDATA->Attribute("code") );
							moText valueattribute( (char*) VALUEDATA->Attribute("attribute") );
							moText valuemin( (char*) VALUEDATA->Attribute("min") );
							moText valuemax( (char*) VALUEDATA->Attribute("max") );
							moText valuedata( (char*) VALUEDATA->GetText() );

							moValue xvalue( valuedata, valuetype );

                            moValueBase& xvbase( xvalue.GetLastSubValue() );

                            xvbase.GetValueDefinition().SetCodeName( valuecodename );
                            xvbase.GetValueDefinition().SetAttribute( valueattribute );
                            xvbase.GetValueDefinition().SetRange( valuemin, valuemax );

							VALUEDATA = VALUEDATA->NextSiblingElement("D");

							while( VALUEDATA ) {
								//.... ;
								moText subvaluetype( (char*) VALUEDATA->Attribute("type") );
                                moText subvaluecodename( (char*) VALUEDATA->Attribute("code") );
                                moText subvalueattribute( (char*) VALUEDATA->Attribute("attribute") );
                                moText subvaluemin( (char*) VALUEDATA->Attribute("min") );
                                moText subvaluemax( (char*) VALUEDATA->Attribute("max") );
								moText subvaluedata( (char*) VALUEDATA->GetText() );

								xvalue.AddSubValue( subvaluedata, subvaluetype );

								moValueBase& xvbase( xvalue.GetLastSubValue() );

								xvbase.GetValueDefinition().SetCodeName( subvaluecodename );
								xvbase.GetValueDefinition().SetAttribute( subvalueattribute );
								xvbase.GetValueDefinition().SetRange( subvaluemin, subvaluemax );

								VALUEDATA = VALUEDATA->NextSiblingElement("D");
							}

							xparam.AddValue( xvalue );
						}
						VALUE = VALUE->NextSiblingElement("VAL");
					}

					xparam.FirstValue();
					m_Params.Add( xparam );

					PARAM = PARAM->NextSiblingElement("PARAM");
				}

			}

			TiXmlNode* PRECONFIGSNODE = MOCONFIG->FirstChild( "PRECONFIGS" );
			TiXmlElement* PRECONFIGS = NULL;
			if (PRECONFIGSNODE) {
				PRECONFIGS = PRECONFIGSNODE->ToElement();
			}
			if (PRECONFIGS) {

				moPreConfig PreConfig;
				TiXmlNode* PRECONFIGNODE = PRECONFIGS->FirstChild("PRE");
				TiXmlElement* PRECONFIG = NULL;
				if (PRECONFIGNODE) {
					PRECONFIG = PRECONFIGNODE->ToElement();
				}
				while(PRECONFIG) {

					TiXmlElement*  PREVALUE = NULL;
					TiXmlNode* NODE = PRECONFIG->FirstChild("P");

					if (NODE) {
						PREVALUE = NODE->ToElement();
					}
					while(PREVALUE) {

						moValueIndex VIndex;

						moText paramname((char*) PREVALUE->Attribute( "name"));
						moValue Value( moText((char*) PREVALUE->GetText()) , moText("INT") );

						VIndex.m_ParamIndex = GetParamIndex(paramname);
						VIndex.m_ValueIndex = Value.GetSubValue().Int();

						PreConfig.m_ValueIndexes.Add( VIndex );
						PREVALUE = PREVALUE->NextSiblingElement("P");

					}

					m_PreConfigs.Add( PreConfig );
					PRECONFIG = PRECONFIG->NextSiblingElement("PRE");
				}

			}

			FirstParam();
			PreConfFirst();
			m_FileName = p_filename;
			m_ConfigLoaded = true;
			return MO_CONFIG_OK;


		}

	} else return MO_CONFIGFILE_NOT_FOUND;
	m_ConfigLoaded = false;
	return -1;
}

int moConfig::SaveConfig( moText p_filename ) {

    if (p_filename.Length()==0)
        p_filename = this->m_FileName;


    //chequear si existe el archivo?
    TiXmlDocument			m_XMLDocument( p_filename );

    TiXmlDeclaration * DECL = new TiXmlDeclaration( "1.0", "", "" );
    m_XMLDocument.LinkEndChild( DECL );

    TiXmlElement * MOCONFIG = new TiXmlElement( "MOCONFIG" );

    if ( MOCONFIG )
    {
        MOCONFIG->SetAttribute( "majorversion", m_MajorVersion);
        MOCONFIG->SetAttribute( "minorversion", m_MinorVersion);

        TiXmlElement* DEFINITION = new TiXmlElement( "DEFINITION" );
        if (DEFINITION) {
            DEFINITION->SetAttribute( "name", GetObjectName() );
            DEFINITION->SetAttribute( "class", GetObjectClass() );
        }
        MOCONFIG->LinkEndChild(DEFINITION);

        TiXmlElement* UPDATE = new TiXmlElement( "UPDATE" );
        MOCONFIG->LinkEndChild(UPDATE);

        TiXmlElement* CONFIGPARAMS = new TiXmlElement( "CONFIGPARAMS" );
        if (CONFIGPARAMS) {

            for( int p = 0; p< m_Params.Count(); p++ ) {

                //proximo parámetro
                moParam& xparam( m_Params.Get(p) );
                moParamDefinition definition = xparam.GetParamDefinition();

                TiXmlElement*  PARAM = new TiXmlElement( "PARAM" );
                if (PARAM) {

                    PARAM->SetAttribute( "name" , definition.GetName() );
                    PARAM->SetAttribute( "type" , definition.GetTypeStr() );

                    for( int  v = 0; v< xparam.GetValuesCount(); v++ ) {

                        //proximo valor
                        moValue &xvalue( xparam.GetValue(v) );

                        TiXmlElement* VALUE = new TiXmlElement("VAL");
                        if (VALUE) {
                            for( int s = 0; s< xvalue.GetSubValueCount(); s++ ) {

                                //proximo subvalor
                                moValueBase& xvaluedata( xvalue.GetSubValue(s) );

                                TiXmlElement* VALUEDATA = new TiXmlElement("D");
                                if (VALUEDATA) {

                                    VALUEDATA->SetAttribute( "type" , xvaluedata.GetTypeStr() );

                                    moText  valuecodename = xvaluedata.GetValueDefinition().GetCodeName();
                                    moText  valueattribute = xvaluedata.GetValueDefinition().GetAttribute();
                                    MOfloat valuemin, valuemax;
                                    xvaluedata.GetValueDefinition().GetRange( &valuemin, &valuemax );

                                    if ( valuecodename !=moText("") ) VALUEDATA->SetAttribute( "code" , valuecodename );
                                    if ( valueattribute !=moText("") ) VALUEDATA->SetAttribute( "attribute" , valueattribute );
                                    if ( valuemin != valuemax ) {
                                            VALUEDATA->SetAttribute( "min" , FloatToStr( valuemin ) );
                                            VALUEDATA->SetAttribute( "max" , FloatToStr( valuemax ) );
                                    }
                                    TiXmlText * datatext = new TiXmlText( xvaluedata.ToText() );

                                    VALUEDATA->LinkEndChild( datatext );
                                }
                                VALUE->LinkEndChild( VALUEDATA );
                            }
                        }
                        PARAM->LinkEndChild(VALUE);
                    }
                }
                CONFIGPARAMS->LinkEndChild( PARAM );
            }
        }
        MOCONFIG->LinkEndChild( CONFIGPARAMS );


        TiXmlElement* PRECONFIGS = new TiXmlElement( "PRECONFIGS" );
        if (PRECONFIGS) {

            for( int pc=0; pc< m_PreConfigs.Count(); pc++) {

                moPreConfig& xPreConfig( m_PreConfigs.Get(pc) );

                TiXmlElement* PRECONFIG = new TiXmlElement("PRE");
                if (PRECONFIG) {

                    for( int pv=0; pv< xPreConfig.m_ValueIndexes.Count(); pv++) {

                        moValueIndex xValueIndex = xPreConfig[pv];

                        TiXmlElement*  PREVALUE = new TiXmlElement("P");

                        if (PREVALUE) {


                            PREVALUE->SetAttribute( "name", GetParam( xValueIndex.m_ParamIndex ).GetParamDefinition().GetName() );

                            TiXmlText * datatext = new TiXmlText( IntToStr(xValueIndex.m_ValueIndex) );

                            if (datatext)
                                PREVALUE->LinkEndChild( datatext );

                        }
                        PRECONFIG->LinkEndChild( PREVALUE );
                    }
                    PRECONFIGS->LinkEndChild( PRECONFIG );
                }
            }
            MOCONFIG->LinkEndChild( PRECONFIGS );
        }
        m_XMLDocument.LinkEndChild( MOCONFIG );
    }

    if (m_XMLDocument.SaveFile( p_filename )) {
        return MO_CONFIG_OK;
    } else return -1;

}

MOboolean
moConfig::CreateDefault( moText p_fullconfigfilename ) {

    if (this->IsConfigLoaded())
        return false;

    //crea los parámetros en función de las definiciones, con valores por default...
    moParamDefinitions* pParamDefinitions = NULL;

    pParamDefinitions = this->GetConfigDefinition()->GetParamDefinitions();

    if (pParamDefinitions) {

        this->UnloadConfig();


        for(int i=0; i<pParamDefinitions->Count() ; i++) {

            moParamDefinition pParamDefinition = pParamDefinitions->Get(i);

            moParam	xparam( pParamDefinition );
            if (pParamDefinition.GetDefaultValue().GetSubValueCount()>0) {
                xparam.AddValue( pParamDefinition.GetDefaultValue() );
            } else {
                xparam.SetDefaultValue();
            }
            m_Params.Add( xparam );
        }

        if (SaveConfig( p_fullconfigfilename )==MO_CONFIG_OK)
            return true;

    }

    return false;

}

void
moConfig::UnloadConfig() {

	m_Params.Empty();

    m_ConfigLoaded = false;

}

void moConfig::DeleteConfig() {

    UnloadConfig();

}

moText
moConfig::GetFullText() {
/*
	moText linebr,tab;

	tab = "\t";
	linebr = "\n";

	if (m_ConfigLoaded) {

		MOuint p,v,s;
		moText	fulltext,indent;

		fulltext = "<?xml version=\"1.0\" standalone=no>";
		fulltext+=linebr;
		fulltext+= "<!-- Console version 0.7 -->";
		fulltext+=linebr;
		fulltext+= "<MOCONFIG majorversion=\"1\" minorversion=\"1\">";
		fulltext+=linebr;
		fulltext+= "<DEFINITION name=\"objectname\" class=\"classobject\">descripcion</DEFINITION>";
		fulltext+=linebr;
		fulltext+="<UPDATE day=\"--/--/2007\" hour=\"00:00:00\">--/--/---- --:--:--</UPDATE>";
		fulltext+=linebr;
		fulltext+= "<CONFIGPARAMS>";
		fulltext+=linebr;
		for( p=0; p<(MOuint)GetParamsCount(); p++ ) {
			fulltext+= tab + "<PARAM name=\""+GetParam(p).GetParamDefinition().GetName()+"\" type=\""+GetParam(p).GetParamDefinition().GetTypeStr()+"\">";
			fulltext+=linebr;
			for( v=0; v< GetParam(p).GetValuesCount(); v++) {
				fulltext+= tab + tab +"<VAL>";
				for( s=0; s< GetParam(p).GetValue(v).GetSubValueCount(); s++) {
					fulltext+= "<D type=\""+GetParam(p).GetValue(v).GetSubValue(s).GetTypeStr()+"\">";
					fulltext+= GetParam(p).GetValue(v).GetSubValue(s).Text();
					fulltext+= "</D>";
				}
				fulltext+= "</VAL>";
				fulltext+=linebr;
			}
			fulltext+= tab + "</PARAM>";
			fulltext+=linebr;
		}
		fulltext+= "</CONFIGPARAMS>";
		fulltext+=linebr;
		fulltext+= "<PRECONFIGS>";
		fulltext+=linebr;
		for( p=0; p<m_PreConfigs.Count(); p++ ) {
			fulltext+= tab + "<PRE>";
			for( v=0; v<m_PreConfigs[p].m_ValueIndexes.Count(); v++ ) {
				fulltext+= "<P name=\""+GetParam(m_PreConfigs[p].m_ValueIndexes[v].m_ParamIndex).GetParamDefinition().GetName()+"\">";
				fulltext+= IntToStr(m_PreConfigs[p].m_ValueIndexes[v].m_ParamIndex);
				fulltext+= "</P>";
			}
			fulltext+= "</PRE>";
			fulltext+=linebr;

		}
		fulltext+= "</PRECONFIGS>";
		fulltext+=linebr;
		return fulltext;
	}
*/
	return moText("unloaded");
}

moParams&
moConfig::GetParams() {

	return m_Params;

}

moParam&
moConfig::GetParam( MOint	p_paramindex ) {
	if ( p_paramindex == -1 )
		return m_Params.Get( m_CurrentParam );
	else
		return m_Params.Get( p_paramindex );
}

moParam&
moConfig::GetParam( moText	p_paramname ) {

	for( MOuint i=0; i< m_Params.Count(); i++ ) {
		if ( m_Params[i].GetParamDefinition().GetName() == p_paramname ) {
			return m_Params[i];
		}
	}
	return m_Params[-1];
}

int
moConfig::GetParamsCount() {
	return m_Params.Count();
}

int
moConfig::GetParamIndex( moText p_paramname ) {

	for( MOuint i=0; i< m_Params.Count(); i++ ) {
		if ( m_Params[i].GetParamDefinition().GetName() == p_paramname ) {
			return i;
		}
	}
	return (-1);
}

MOuint
moConfig::GetValuesCount( int p_paramindex ) {
	return m_Params.Get( p_paramindex ).GetValuesCount();
}

void
moConfig::SetCurrentValueIndex( int p_paramindex, int p_valueindex ) {
	m_Params[p_paramindex].SetIndexValue( p_valueindex );
}

int
moConfig::GetCurrentValueIndex( MOint p_paramindex ) {
	return m_Params.Get( p_paramindex ).GetIndexValue();
}

moValue&
moConfig::GetCurrentValue() {
	return m_Params.Get( m_CurrentParam ).GetValue();
}

moParam&
moConfig::GetCurrentParam() {
	return m_Params.Get( m_CurrentParam );
}


int
moConfig::GetCurrentParamIndex() {
	return m_CurrentParam;
}

void
moConfig::SetCurrentParamIndex( int p_currentparam ) {
	m_CurrentParam = p_currentparam;
}

void
moConfig::FirstParam() {
	if ( m_Params.Count()>0 ) {
		m_CurrentParam = 0;
	} else m_CurrentParam = -1;
}

void
moConfig::NextParam() {
	if ( m_Params.Count()>0 ) {
		if ( m_CurrentParam < ( (MOint)m_Params.Count()-1 ) ) {
			m_CurrentParam++;
		}
	} else m_CurrentParam = -1;
}

void
moConfig::PrevParam() {
	if ( m_Params.Count() > 0 ) {
		if ( m_CurrentParam > 0 ) {
			m_CurrentParam--;
		}
	} else m_CurrentParam = -1;
}

void
moConfig::FirstValue() {
	if ( m_CurrentParam>=0 ) {
		m_Params[m_CurrentParam].FirstValue();
	}
}

void
moConfig::NextValue() {
	if ( m_CurrentParam>=0 ) {
		m_Params[m_CurrentParam].NextValue();
	}
}

void
moConfig::PreviousValue() {
	if ( m_CurrentParam>=0 ) {
		m_Params[m_CurrentParam].PrevValue();
	}
}


moConfigDefinition*
moConfig::GetConfigDefinition() {

	return &m_ConfigDefinition;

}

//PRECONFIGS

MOint
moConfig::GetPreConfCount() {
	return m_PreConfigs.Count();
}

MOint
moConfig::GetCurrentPreConf() {
	return m_PreconfActual;
}

void
moConfig::SetCurrentPreConf( MOint p_actual ) {
    if(0<=p_actual && p_actual<(MOint)m_PreConfigs.Count())
    {
		for( MOuint i=0; i<m_PreConfigs[p_actual].m_ValueIndexes.Count(); i++) {
			moValueIndex Val = m_PreConfigs[p_actual][i];
            SetCurrentValueIndex( Val.m_ParamIndex, Val.m_ValueIndex);
		}
        m_PreconfActual = p_actual;
    }
}

void
moConfig::PreConfFirst() {
	if(m_PreConfigs.Count()>0)
		SetCurrentPreConf( 0 );
}

void
moConfig::PreConfNext() {
    if( m_PreconfActual!=-1 && (m_PreconfActual<((MOint)m_PreConfigs.Count()-1)) )
        SetCurrentPreConf( ++m_PreconfActual );
}

void
moConfig::PreConfPrev() {
    if(m_PreconfActual!=-1 && m_PreconfActual>0)
        SetCurrentPreConf( --m_PreconfActual );
}
