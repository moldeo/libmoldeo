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

*******************************************************************************/

#include "moConfig.h"

#include <tinyxml.h>
#include "moFileManager.h"
#include "moFontManager.h"
#include "moTextureManager.h"
#include "mo3dModelManager.h"
#include "moSoundManager.h"
#include "moDebugManager.h"

moFont*                 moConfig::m_pFont = new moFont();
moMathFunction*         moConfig::m_pFun = new moParserFunction();
moTextureBuffer*        moConfig::m_pTextureBuffer = new moTextureBuffer();
mo3DModelSceneNode*     moConfig::m_pModel = new mo3DModelSceneNode();
moVector2d*             moConfig::m_pVector2d = new moVector2d();
moVector2i*             moConfig::m_pVector2i = new moVector2i();
moVector3d*             moConfig::m_pVector3d = new moVector3d();
moVector3i*            moConfig::m_pVector3i = new moVector3i();
moVector4d*             moConfig::m_pVector4d = new moVector4d();
moVector4i*            moConfig::m_pVector4i = new moVector4i();
moDataMessage*          moConfig::m_pMessage = new moDataMessage();
moDataMessages*         moConfig::m_pMessages = new moDataMessages();
moSound*                moConfig::m_pSound = new moSound();
moTexture*              moConfig::m_pTexture = new moTexture();

//================================================================
//	moConfigDefinition
//================================================================

moConfigDefinition::moConfigDefinition() {

}

moConfigDefinition::~moConfigDefinition() {
}

bool
moConfigDefinition::Exists( moText p_name ) {
  for( MOuint i = 0; i < m_ParamDefinitions.Count(); i++ ) {
    moParamDefinition pdef = m_ParamDefinitions[i];
    if ( pdef.GetName() == p_name )
      return true;
  }
  return false;
}

bool
moConfigDefinition::SetIndex( moText p_name, MOint p_index) {

    bool founded;
    moParamDefinition pdef;

    founded = false;

    for( MOuint i = 0; i < m_ParamDefinitions.Count(); i++ ) {
        pdef = m_ParamDefinitions[i];
        if ( pdef.GetName() == p_name ) {
            founded = true;
            break;
        }
    }

    if (founded) {
        pdef.SetIndex( p_index );
        m_ParamIndexes.Add( p_index );
    }

    return founded;
}

void
moConfigDefinition::Add( moText p_name, moParamType p_type , MOint p_index) {

    if ( Exists(p_name) ) {
        MODebug2->Error( p_name + " already defined in " + m_ObjectName );
        return;
    }

	moParamDefinition pdef( p_name, p_type );

	pdef.SetIndex( p_index );

	m_ParamDefinitions.Add( pdef );

  /** ATENCION!!!! si p_index no coincide con el indice real de la entrada d el array de m_ParamIndexes....
      entonces no funcionará.... hay que evitar este problema....
      sobre todo  y tambien si no se llama en el orden correcto.

      soluciones:

        Espera que las sucesivas llamadas se hagan correctamente.
        Si el siguiente indice no coincide...


      */

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

void
moConfigDefinition::Add( moText p_name, moParamType p_type , MOint p_index, moValue p_defaultvalue, const moTextArray& p_Options ) {

	moParamDefinition pdef( p_name, p_type );

	pdef.SetIndex( p_index );
	pdef.SetDefault( p_defaultvalue );
	pdef.SetOptions(p_Options);

	m_ParamDefinitions.Add( pdef );

	m_ParamIndexes.Add( p_index );

}

bool
moConfigDefinition::SetParamIndex( int defined_array_index, moParamIndex paramindex  ) {

  if (defined_array_index==-1) {
    MODebug2->Error("moConfigDefinition::SetParamIndex in [" + m_ObjectName + "] > defined array entry index is -1 (undefined).");
    return false;
  }

  if ( ! ( 0 <= defined_array_index && defined_array_index < (int)m_ParamIndexes.Count()) ) {
        MODebug2->Error( moText("moConfigDefinition::SetParamIndex[" + m_ObjectName + "] > defined array entry index not in range : ")
                        + IntToStr(defined_array_index)
                        + moText(" Max Indexes : ")
                        + IntToStr(m_ParamIndexes.Count()) );
        return false;
  }

  if (paramindex.index==MO_PARAM_NOT_FOUND) {
    MODebug2->Error( moText("moConfigDefinition::SetParamIndex[" + m_ObjectName + "] > moParamIndex undefined : ")
                        + IntToStr(paramindex.index) );
    return false;
  }

  m_ParamIndexes.Set( defined_array_index , paramindex );
  return true;

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

void
moConfig::Indexation() {

    if ( GetConfigDefinition()!=NULL) {

        for( MOuint i = 0; i < GetConfigDefinition()->GetParamDefinitions()->Count(); i++ ) {

            moParamDefinition pdef = GetConfigDefinition()->GetParamDefinitions()->Get(i);

            int pidx = (MOint)GetParamIndex(  pdef.GetName() );
            if (pidx>-1) {
              if (!GetConfigDefinition()->SetParamIndex( (int)pdef.GetIndex(), moParamIndex(pidx))) {
                moDebugManager::Error( moText(pdef.GetName()) + moText("  warning. Bad indexation could cause errors."));
              }
            } else moDebugManager::Error( "<"+GetObjectName()+">::Init() > "+GetObjectClass()+ " \"" + moText(pdef.GetName()) + moText("\" not found."));
        }
    }
}

void
moConfig::FixConfig() {

  ///AGREGAMOS PARAMETROS FALTANTES
  for( MOuint j=0; j< m_ConfigDefinition.GetParamDefinitions()->Count(); j++ ) {
    moParamDefinitions* pd = m_ConfigDefinition.GetParamDefinitions();
    if (pd) {
        moParamDefinition pDef = pd->Get(j);
        moText paramname = pDef.GetName();
        int real_idx = GetParamIndex( paramname );
        if (real_idx==-1) {
          CreateParam( pDef );
        }
    }
  }

  ///RE-INDEXAMOS....
  for( MOuint i=0; i< m_Params.Count(); i++ ) {
    moParam& param(m_Params[i]);
    for( MOuint j=0; j< m_ConfigDefinition.GetParamDefinitions()->Count(); j++ ) {
        moParamDefinitions* pd = m_ConfigDefinition.GetParamDefinitions();
        moParamDefinition pDef = pd->Get(j);
        if (pd) {
            if ( param.GetParamDefinition().GetName() == pDef.GetName() ) {
                pDef.SetIndex(i);
                pd->Set( j, pDef );
                break;
            }
        }
    }
    param.GetParamDefinition().SetIndex(i);
  }

}

int moConfig::LoadConfig( moText p_filename ) {

	TiXmlDocument			m_XMLDocument;

	UnloadConfig();

  if (!moFileManager::FileExists(p_filename)) {
    moDebugManager::Error( "moConfig::LoadConfig > Error " + p_filename + " doesn't exists." );
  }

	cout << "XML DOM about to load..." << endl;

	if (m_XMLDocument.LoadFile( (char *) p_filename )) {

		//parse the xml for params...
		cout << "XML DOM loaded" << endl;

		TiXmlHandle docHandle( &m_XMLDocument );

		TiXmlElement* MOCONFIG = docHandle.FirstChild( "MOCONFIG" ).Element();
//		FirstChild( "DEFINITION" ).Child( "Child", 1 ).Element();
		if ( MOCONFIG )
		{
			MOCONFIG->QueryIntAttribute( "majorversion", &((int&)m_MajorVersion));
			MOCONFIG->QueryIntAttribute( "minorversion", &((int&)m_MinorVersion));

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

                            xvbase.SetCodeName( valuecodename );
                            xvbase.SetAttribute( valueattribute );
                            xvbase.SetRange( valuemin, valuemax );
                            /** TODO: */
                            // SetSelect ???

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

								xvbase.SetCodeName( subvaluecodename );
								xvbase.SetAttribute( subvalueattribute );
								xvbase.SetRange( subvaluemin, subvaluemax );

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
					PreConfig.m_ValueIndexes.Empty();

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

			///corregimos, asignamos indices, agregamos faltantes...
			FixConfig();

			return MO_CONFIG_OK;


		}

	} else return MO_CONFIGFILE_NOT_FOUND;
	m_ConfigLoaded = false;
	return -1;
}

int moConfig::SaveConfig( moText p_filename ) {

    if (p_filename.Length()==0) {
        p_filename = this->m_FileName;
    }

    cout << "moConfig::SaveConfig > filename is " << p_filename << endl;


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

            for( int p = 0; p< (int)m_Params.Count(); p++ ) {

                //proximo parámetro
                moParam& xparam( m_Params[p] );
                moParamDefinition definition = xparam.GetParamDefinition();

                TiXmlElement*  PARAM = new TiXmlElement( "PARAM" );
                if (PARAM) {

                    PARAM->SetAttribute( "name" , definition.GetName() );
                    PARAM->SetAttribute( "type" , definition.GetTypeStr() );

                    for( int  v = 0; v< (int)xparam.GetValuesCount(); v++ ) {

                        //proximo valor
                        moValue &xvalue( xparam.GetValue(v) );

                        TiXmlElement* VALUE = new TiXmlElement("VAL");
                        if (VALUE) {
                            for( int s = 0; s< (int)xvalue.GetSubValueCount(); s++ ) {

                                //proximo subvalor
                                moValueBase& xvaluedata( xvalue.GetSubValue(s) );

                                TiXmlElement* VALUEDATA = new TiXmlElement("D");
                                if (VALUEDATA) {

                                    VALUEDATA->SetAttribute( "type" , xvaluedata.GetTypeStr() );

                                    moText  valuecodename = xvaluedata.GetValueDefinition().GetCodeName();
                                    moText  valueattribute = xvaluedata.GetValueDefinition().GetAttribute();
                                    MOfloat valuemin, valuemax;
                                    xvaluedata.GetRange( &valuemin, &valuemax );

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

            for( int pc=0; pc< (int)m_PreConfigs.Count(); pc++) {

                moPreConfig& xPreConfig( m_PreConfigs[pc] );

                TiXmlElement* PRECONFIG = new TiXmlElement("PRE");
                if (PRECONFIG) {

                    for( int pv=0; pv< (int)xPreConfig.m_ValueIndexes.Count(); pv++) {

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

    cout << "moConfig::SaveConfig > saving file to " << p_filename << endl;
    if (m_XMLDocument.SaveFile( p_filename )) {
        cout << "moConfig::SaveConfig > success " << endl;
        return MO_CONFIG_OK;
    } else {
        cout << "moConfig::SaveConfig > ERROR " << endl;
        return -1;
    }

}

void
moConfig::CreateParam( moParamDefinition& p_ParamDef ) {

  moParam	xparam( p_ParamDef );

  ///asigna valor predeterminado definido por el plugin, sino
  ///aplica el estandar
  if (p_ParamDef.GetDefaultValue().GetSubValueCount()>0) {
      xparam.AddValue( p_ParamDef.GetDefaultValue() );
  } else {
      xparam.SetDefaultValue();
  }
  m_Params.Add( xparam );

  ///asigna los indices...
  moParam& pParam( m_Params[m_Params.Count()-1] );
  pParam.GetParamDefinition().SetIndex(m_Params.Count()-1);
  p_ParamDef.SetIndex(m_Params.Count()-1);
}

MOboolean
moConfig::CreateDefault( const moText &p_fullconfigfilename ) {

    moFile cFile( p_fullconfigfilename );

    moText pt;
    pt = p_fullconfigfilename;

    /// Do not overwrite... by default
    if (cFile.Exists()) {
        cout << "moConfig::CreateDefault > file already exists " << pt << endl;
        return false;
    }

    if (this->IsConfigLoaded()) {
        cout << "moConfig::CreateDefault > config has information already (is loaded) " << endl;
        return false;
    }

    //crea los parámetros en función de las definiciones, con valores por default...
    moParamDefinitions* pParamDefinitions = NULL;

    pParamDefinitions = this->GetConfigDefinition()->GetParamDefinitions();

    if (pParamDefinitions) {

        this->UnloadConfig();

        cout << "moConfig::CreateDefault > Loading default ParamDefinition in moConfig : number of params : " << pParamDefinitions->Count() << endl;
        for(int i=0; i<(int)pParamDefinitions->Count() ; i++) {

            moParamDefinition pParamDefinition = pParamDefinitions->Get(i);

            CreateParam( pParamDefinition );
        }

        cout << "moConfig::CreateDefault > Saving Config to disk..." << endl;
        if (SaveConfig( pt )==MO_CONFIG_OK) {
            cout << "moConfig::CreateDefault > Saved Config Succesfully " << endl;
            return true;
        }
        cout << "moConfig::CreateDefault > Error Saving Config to disk " << endl;

    }

    cout << "moConfig::CreateDefault > Errors occured " << endl;
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
		return m_Params[m_CurrentParam];
	else
		return m_Params[p_paramindex];
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

moValue&
moConfig::GetValue( moText nameparam, int indexvalue ) {

    moParam& param( GetParam( nameparam ));

    return param.GetValue( indexvalue );
}

moValue&
moConfig::GetValue( int indexparam, int indexvalue ) {

    moParam& param( GetParam( indexparam ));

    return param.GetValue( indexvalue );

}


MOint
moConfig::Int( moParamReference p_paramreference ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    return param.GetValue().GetSubValue().Int();
}

MOint
moConfig::Int( moText p_param_name ) {
    return GetValue(p_param_name).GetSubValue().Int();
}

MOdouble
moConfig::Double( moParamReference p_paramreference ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    return param.GetValue().GetSubValue().Double();
}

MOdouble
moConfig::Double( moText p_param_name ) {
    return GetValue(p_param_name).GetSubValue().Double();
}

moText
moConfig::Text( moParamReference p_paramreference ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    return param.GetValue().GetSubValue().Text();
}

moText
moConfig::Text( moText p_param_name ) {
    return GetValue( p_param_name ).GetSubValue().Text();
}

moVector4d
moConfig::EvalColor( moParamReference p_paramreference ) {

    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moVector4d  full_color(0.0,0.0,0.0,1.0);
    double r,g,b,a;
    if (param.GetValue().GetSubValueCount()==4) {
      r = param.GetValue().GetSubValue(MO_RED).Eval();
      g = param.GetValue().GetSubValue(MO_GREEN).Eval();
      b = param.GetValue().GetSubValue(MO_BLUE).Eval();
      a = param.GetValue().GetSubValue(MO_ALPHA).Eval();
      full_color = moVector4d( r, g, b, a);
    }
    return full_color;

}

moVector4d
moConfig::EvalColor( moParamReference p_paramreference, double x, ... ) {

    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moVector4d  full_color(0.0,0.0,0.0,1.0);
    double r,g,b,a;
    if (param.GetValue().GetSubValueCount()==4) {
      r = param.GetValue().GetSubValue(MO_RED).Eval( x );
      g = param.GetValue().GetSubValue(MO_GREEN).Eval( x );
      b = param.GetValue().GetSubValue(MO_BLUE).Eval( x );
      a = param.GetValue().GetSubValue(MO_ALPHA).Eval( x );
      full_color = moVector4d( r, g, b, a);
    }
    return full_color;

}

MOdouble
moConfig::Eval( moParamReference p_paramreference, double x, ... ) {

    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->Eval( x );
    }
    return 0.0f;

}

MOdouble
moConfig::Eval( moText p_param_name , double x, ... ) {

    moParam& param( GetParam(p_param_name) );
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->Eval( x );
    }
    return 0.0f;

}

MOdouble
moConfig::Eval( moParamReference p_paramreference ) {

    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->Eval();
    }
    return 0.0f;
}

MOdouble
moConfig::Eval( moText p_param_name ) {

    moParam& param( GetParam(p_param_name) );
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->Eval();
    }
    return 0.0f;

}


GLint
moConfig::GetGLId( moParamReference p_paramreference, MOfloat p_cycle, MOfloat p_fade, moTextFilterParam *p_filterparam ) {

    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_cycle, p_fade, p_filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, moTempo *p_tempo, MOfloat p_fade, moTextFilterParam *p_filterparam ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_tempo, p_fade, p_filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, MOuint p_i, MOfloat p_fade, moTextFilterParam *p_filterparam ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_i, p_fade, p_filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, MOfloat p_fade, moTextFilterParam *p_filterparam ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_fade, p_filterparam );
    }
    return 0;
}


moMathFunction&
moConfig::Fun(  moParamReference p_paramreference ) {
  if ( p_paramreference.reference == -1 ) {
      ///error...
      return *m_pFun;
  }
  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moMathFunction* pFun = pdata->Fun();
      if (pFun) {
        return *pFun;
      }
  }
  return (*m_pFun);
}

const moFont&
moConfig::Font( moParamReference p_paramreference ) {
  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moFont* pFont = pdata->Font();
      if (pFont) {
        return *pFont;
      }
  }
  return (*m_pFont);
}

const moTextureBuffer&
moConfig::TextureBuffer(  moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moTextureBuffer* pTextureBuffer = pdata->TextureBuffer();
      if (pTextureBuffer) {
        return *pTextureBuffer;
      }
  }
  return (*m_pTextureBuffer);

}

const mo3DModelSceneNode&
moConfig::Model(  moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      mo3DModelSceneNode* pModel = pdata->Model();
      if (pModel) {
        return *pModel;
      }
  }
  return (*m_pModel);

}

const moVector2d&
moConfig::Vector2d( moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moVector2d* pVector = pdata->Vector2d();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pVector2d);

}

const moVector2i&
moConfig::Vector2i( moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moVector2i* pVector = pdata->Vector2i();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pVector2i);

}

const moVector3d&
moConfig::Vector3d( moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moVector3d* pVector = pdata->Vector3d();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pVector3d);

}

const moVector3i&
moConfig::Vector3i( moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moVector3i* pVector = pdata->Vector3i();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pVector3i);

}

const moVector4d&
moConfig::Vector4d( moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moVector4d* pVector = pdata->Vector4d();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pVector4d);

}


const moVector4i&
moConfig::Vector4i( moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moVector4i* pVector = pdata->Vector4i();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pVector4i);

}

const moDataMessage&
moConfig::Message(  moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moDataMessage* pVector = pdata->Message();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pMessage);

}

const moDataMessages&
moConfig::Messages( moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moDataMessages* pVector = pdata->Messages();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pMessages);

}

moSound&
moConfig::Sound(  moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moSound* pVector = pdata->Sound();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pSound);

}

const moTexture&
moConfig::Texture(  moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moTexture* pVector = pdata->Texture();
      if (pVector) {
        return *pVector;
      }
  }
  return (*m_pTexture);

}

void
moConfig::SetCurrentValueIndex( int
p_paramindex, int p_valueindex ) {
	m_Params[p_paramindex].SetIndexValue( p_valueindex );
}

int
moConfig::GetCurrentValueIndex( MOint p_paramindex ) {
	return m_Params.Get( p_paramindex ).GetIndexValue();
}

moValue&
moConfig::GetCurrentValue() {
	return m_Params[m_CurrentParam].GetValue();
}

moParam&
moConfig::GetCurrentParam() {
	return m_Params[m_CurrentParam];
}


int
moConfig::GetCurrentParamIndex() const {
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


/// Agrega un valor
void
moConfig::AddValue( int paramindex,  moValue& p_value ) {
    GetParam( paramindex ).AddValue( p_value );
}

/// Inserta un valor
void
moConfig::InsertValue( int paramindex,  int valueindex, moValue& p_value ) {
    GetParam( paramindex ).GetValues().Insert( valueindex, p_value );
}

/// Fija un valor
void
moConfig::SetValue( int paramindex,  int valueindex, moValue& p_value ) {
    GetParam( paramindex ).GetValue( valueindex ) = p_value;
}

/// Borra un valor
void
moConfig::DeleteValue( int paramindex,  int valueindex ) {

    GetParam( paramindex ).DeleteValue( valueindex );

}



/// Agrega una pre-configuración
void
moConfig::AddPreconfig( moPreconfigIndexes& p_preconfindexes ) {
    m_PreConfigs.Add( moPreConfig( p_preconfindexes ) );
}

/// Agrega una pre-configuración
void
moConfig::InsertPreconfig( int valueindex, moPreconfigIndexes& p_preconfindexes ) {
    m_PreConfigs.Insert( valueindex,  moPreConfig( p_preconfindexes ) );
}

/// Agrega una pre-configuración
void
moConfig::SetPreconfig( int valueindex, moPreconfigIndexes& p_preconfindexes ) {
    m_PreConfigs.Set( valueindex, moPreConfig( p_preconfindexes ) );
}

/// Borra una pre-configuración
void
moConfig::DeletePreconfig( int valueindex ) {
    m_PreConfigs.Remove( valueindex );
}
