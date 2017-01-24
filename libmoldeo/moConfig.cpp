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

#include "moTypes.h"
#include <tinyxml.h>

#include "moConfig.h"

#include "moFileManager.h"
#include "moFontManager.h"
#include "moTextureManager.h"
#include "mo3dModelManager.h"
#include "moSoundManager.h"
#include "moDebugManager.h"

moFont*                 moConfig::m_pFont = new moFont();
moMathFunction*         moConfig::m_pFun = new moParserFunction();
moTextureBuffer*        moConfig::m_pTextureBuffer = new moTextureBuffer();
moSceneNode*            moConfig::m_pModel = new moSceneNode();
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

/// copy constructor
moConfigDefinition::moConfigDefinition( const moConfigDefinition& p_src ) {
    (*this) = p_src;
}

/// assignment copy operator
moConfigDefinition&
moConfigDefinition::operator =( const moConfigDefinition& p_src ) {

 (moAbstract)(*this) = (moAbstract) p_src;
  m_ParamDefinitions = p_src.m_ParamDefinitions;
  m_ParamIndexes = p_src.m_ParamIndexes;
  m_ObjectName = p_src.m_ObjectName;
  m_ObjectClass = p_src.m_ObjectClass;
  m_FullJSON = p_src.m_FullJSON;

  return (*this);
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

const moParamDefinition&
moConfigDefinition::GetParamDefinition( const moText& p_param_name ) {

  MOuint i = 0;

  for( i = 0; i < m_ParamDefinitions.Count(); i++ ) {
    moParamDefinition& pdef( m_ParamDefinitions[i] );
    if ( pdef.GetName() == p_param_name )
      return m_ParamDefinitions[i];
  }

  return m_ParamDefinitions.Get(-1);
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
moConfigDefinition::Add( const moText& p_name, moParamType p_type, int p_index, const moValue& p_defaultvalue, const moText& p_OptionsStr ) {

  if ( Exists(p_name) ) {
    MODebug2->Error( p_name + " already defined in " + m_ObjectName );
    return;
  }

  moParamDefinition pdef( p_name, p_type );

  pdef.SetIndex( p_index );

  if (p_type!=MO_PARAM_MOLDEO_OBJECT)
      pdef.SetDefault( p_defaultvalue );

  pdef.SetOptions(p_OptionsStr);

  //IF TYPE IS COLOR > sub 0: RED, 1: GREEN, 2: BLUE, 3: ALPHA
  if (p_type==MO_PARAM_COLOR) {
      moValueDefinition vd;
      vd = pdef.GetDefaultValue().GetSubValue((int)MO_RED).GetValueDefinition();
      vd.SetCodeName( "RED" );
      pdef.GetDefaultValue().GetSubValue((int)MO_RED).SetValueDefinition(vd);

      vd = pdef.GetDefaultValue().GetSubValue((int)MO_GREEN).GetValueDefinition();
      vd.SetCodeName( "GREEN" );
      pdef.GetDefaultValue().GetSubValue((int)MO_GREEN).SetValueDefinition(vd);

      vd = pdef.GetDefaultValue().GetSubValue((int)MO_BLUE).GetValueDefinition();
      vd.SetCodeName( "BLUE" );
      pdef.GetDefaultValue().GetSubValue((int)MO_BLUE).SetValueDefinition(vd);

      vd = pdef.GetDefaultValue().GetSubValue((int)MO_ALPHA).GetValueDefinition();
      vd.SetCodeName( "ALPHA" );
      pdef.GetDefaultValue().GetSubValue((int)MO_ALPHA).SetValueDefinition(vd);

      /*
      p_defaultvalue.GetSubValue(MO_GREEN).GetValueDefinition().SetCodeName( "GREEN" );
      p_defaultvalue.GetSubValue(MO_BLUE).GetValueDefinition().SetCodeName( "BLUE" );
      p_defaultvalue.GetSubValue(MO_ALPHA).GetValueDefinition().SetCodeName( "ALPHA" );
      */
  }

  if (p_type==MO_PARAM_FONT) {
      //p_defaultvalue.GetSubValue(0).GetValueDefinition().SetCodeName( "RED" );
  }

  m_ParamDefinitions.Add( pdef );

  m_ParamIndexes.Add( p_index );
}

/*
void
moConfigDefinition::Add( moText p_name, moParamType p_type , MOint p_index) {

    if ( Exists(p_name) ) {
        MODebug2->Error( p_name + " already defined in " + m_ObjectName );
        return;
    }

	moParamDefinition pdef( p_name, p_type );

	pdef.SetIndex( p_index );

	m_ParamDefinitions.Add( pdef );

   ATENCION!!!! si p_index no coincide con el indice real de la entrada d el array de m_ParamIndexes....
      entonces no funcionará.... hay que evitar este problema....
      sobre todo  y tambien si no se llama en el orden correcto.

      soluciones:

        Espera que las sucesivas llamadas se hagan correctamente.
        Si el siguiente indice no coincide...




	m_ParamIndexes.Add( p_index );

}
*/

bool
moConfigDefinition::SetParamIndex( int defined_array_index, moParamIndex paramindex  ) {

  if (defined_array_index==-1) {
    MODebug2->Error("moConfigDefinition::SetParamIndex in [" + m_ObjectName + "] > defined array entry index is -1 (undefined).");
    return false;
  }

  if ( ! ( 0 <= defined_array_index && defined_array_index < (int)m_ParamIndexes.Count()) ) {
        /*MODebug2->Message( moText("WARNING!! moConfigDefinition::SetParamIndex[" + m_ObjectName + "] > defined array entry index not in range : ")
                        + IntToStr(defined_array_index)
                        + moText(" Max Indexes : ")
                        + IntToStr(m_ParamIndexes.Count()) );*/
      MODebug2->Warning( moText("Extra parameters must be defined at the end of config file. \"") + m_ObjectName + moText("\""));
        MODebug2->Message( this->ToJSON() );
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

const moText&
moConfigDefinition::ToJSON() {

  moText fieldSeparation = ",";

  m_FullJSON = "{";
  m_FullJSON+= "'objectname': '" + this->m_ObjectName + "'";
  m_FullJSON+= fieldSeparation + "'objectclass': '" + this->m_ObjectClass + "'";
  m_FullJSON+= "}";

  return m_FullJSON;
}

//================================================================
//	moConfig ( XML Config )
//================================================================


moConfig::moConfig() {
	m_CurrentParam = -1;
	m_ConfigLoaded = false;
	m_PreconfActual = -1;
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
                moDebugManager::Message( "moConfig::Indexation > <mo"+GetObjectClass()+"::"+GetObjectName()+"> Parameter \"" + moText(pdef.GetName()) + moText("\"  warning. Bad indexation could cause errors."));
              }
            } else moDebugManager::Error( "moConfig::Indexation > <mo"+GetObjectClass()+"::"+GetObjectName()+"> > Parameter \"" + moText(pdef.GetName()) + moText("\" not found."));
        }
    }
}

void
moConfig::FixConfig() {

  ///AGREGAMOS PARAMETROS FALTANTES
  /// Y OPCIONES !!!
  for( MOuint j=0; j< m_ConfigDefinition.GetParamDefinitions()->Count(); j++ ) {
    moParamDefinitions* pd = m_ConfigDefinition.GetParamDefinitions();
    if (pd) {

        moParamDefinition pDef = pd->Get(j);

        moText paramDefName = pDef.GetName();
        moParamType paramDefType = pDef.GetType();

        int real_idx = GetParamIndex( paramDefName );

        if (real_idx==-1) {

          CreateParam( pDef );

        } else {

          /// check type and format
          moParam& pParamLoaded( GetParam(real_idx) );

          if ( pParamLoaded.GetParamDefinition().GetType() != paramDefType ) {
            /// fix type
            pParamLoaded.FixType( paramDefType );

          }
          /// TODO: perform a JSON test of the full Options object instead of only checking the list size
          if (pDef.GetOptions().Count()!=pParamLoaded.GetParamDefinition().GetOptions().Count()) {
            pParamLoaded.FixOptions( pDef.GetOptions() );
          }
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

  ///ACTUALIZAMOS LOS VALUE DEFINITION ( CODENAME, MIN, MAX, etc...)
  //for( MOuint i=0; i< m_Params.Count(); i++ ) {

  //moParam& param(m_Params[i]);

  for( MOuint j=0; j< m_ConfigDefinition.GetParamDefinitions()->Count(); j++ ) {

      moParamDefinitions* pd = m_ConfigDefinition.GetParamDefinitions();
      moParamDefinition pDef = pd->Get(j);
      int pdefindex = pDef.GetIndex();

      if (pDef.GetName()=="color") {
        //break here
        pDef.GetTypeStr();
      }

      moParam& param( m_Params[pdefindex] );
      //moValue defaultValue = param.GetParamDefinition().GetDefaultValue();
      moValue defaultValue = pDef.GetDefaultValue();

      for( MOuint k=0; k< param.GetValuesCount(); k++ ) {
        for( MOuint l=0; l< param.GetValue(k).GetSubValueCount(); l++ ) {
          moValueBase &vb( param.GetValue(k).GetSubValue(l) );
          moValueBase &dfvb( defaultValue.GetSubValue(l) );
          //compare vb.
          moValueDefinition vd = vb.GetValueDefinition();
          moValueDefinition dfvd = dfvb.GetValueDefinition();
          if(vd.GetCodeName()!=dfvd.GetCodeName())
              vd.SetCodeName( dfvd.GetCodeName() );
          vb.SetValueDefinition( vd );

        }
      }
  }

}

int moConfig::LoadConfig( moText p_filename ) {

	TiXmlDocument			m_XMLDocument;
    TiXmlEncoding           xencoding = TIXML_ENCODING_LEGACY;

	UnloadConfig();

	//p_filename = p_filename.Replace("\\","//");
	//p_filename = p_filename.Replace("\\\\","//");
	p_filename.Replace("\\","/");

      if (!moFileManager::FileExists(p_filename)) {
        moDebugManager::Error( "moConfig::LoadConfig > Error " + p_filename + " doesn't exists." );
      }

	cout << "XML DOM about to load..." << endl;

	if (m_XMLDocument.LoadFile( (char *) p_filename, xencoding )) {

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
				if (UPDATE) {
          moText version = UPDATE->Attribute("");
				}
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
          char* resStr = NULL;

					moText paramname;
					resStr = ((char*) PARAM->Attribute( "name"));
					if (resStr) paramname = resStr;

					moText paramtype;
					resStr = ((char*) PARAM->Attribute( "type"));
					if (resStr) paramtype = resStr;

					moText paramproperty;
					resStr = ((char*) PARAM->Attribute( "property"));
					if (resStr) paramproperty = resStr;

					moText paramgroup;
					resStr = ((char*) PARAM->Attribute( "group"));
					if (resStr) paramgroup = resStr;

					moText paraminterpolation;
					resStr = ((char*) PARAM->Attribute( "interpolation"));
					if (resStr) paraminterpolation = resStr;

					moText paramduration;
					resStr = ((char*) PARAM->Attribute( "duration"));
          if (resStr) paramduration = resStr;

          moText paramoptions;
					resStr = ((char*) PARAM->Attribute( "options"));
					if (resStr) paramoptions = resStr;

          /*cout << endl << "moConfig::LoadConfig > name: " <<  paramname << " paraminterpolation: " << paraminterpolation  << endl;*/
					moParamDefinition definition( paramname, paramtype, paramproperty, paramgroup, paraminterpolation, paramduration, paramoptions );
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

              if (valuetype=="XML") {
                  TiXmlPrinter printer;
                  TiXmlNode* NODEDATAXML = VALUEDATA->FirstChild();
                  if (NODEDATAXML) {
                    NODEDATAXML->Accept( &printer );
                    valuedata = moText( printer.CStr() );
                  }
              }

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

                if (subvaluetype=="XML") {
                    TiXmlPrinter printer;
                    TiXmlNode* NODEDATAXML = VALUEDATA->FirstChild();
                    if (NODEDATAXML) {
                      NODEDATAXML->Accept( &printer );
                      subvaluedata = moText( printer.CStr() );
                    }
                }

								xvalue.AddSubValue( subvaluedata, subvaluetype );

								moValueBase& xxvbase( xvalue.GetLastSubValue() );

								xxvbase.SetCodeName( subvaluecodename );
								xxvbase.SetAttribute( subvalueattribute );
								xxvbase.SetRange( subvaluemin, subvaluemax );

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
					//PreConfig.m_ValueIndexes.Empty();
					PreConfig.m_PreconfIndexes.Empty();
					PreConfig.m_Name = moText((char*) PRECONFIG->Attribute( "name"));

					if (NODE) {
						PREVALUE = NODE->ToElement();
					}
					while(PREVALUE) {

						moPreconfigParamIndex VIndex;

						moText paramname((char*) PREVALUE->Attribute( "name"));
						moValue Value( moText((char*) PREVALUE->GetText()) , moText("INT") );

            VIndex.m_ParamName = paramname;
						VIndex.m_ParamIndex = GetParamIndex(paramname);
						VIndex.m_ValueIndex = Value.GetSubValue().Int();

						PreConfig.m_PreconfIndexes.Add( VIndex );
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

	} else {
    if (m_XMLDocument.Error()) {
      moDebugManager::Error( moText("XML ERROR:") + p_filename+moText(" XML error:") + moText(m_XMLDocument.ErrorDesc()) );
    }
    return MO_CONFIGFILE_NOT_FOUND;
  }
	m_ConfigLoaded = false;
	return -1;
}

int moConfig::SaveConfig( moText p_filename ) {
    moText TNodeValue;

    if (p_filename.Length()==0) {
        p_filename = this->m_FileName;
    }

    cout << "moConfig::SaveConfig > filename is " << p_filename << endl;


    //chequear si existe el archivo?
    TiXmlDocument			m_XMLDocument( p_filename );

    TiXmlDeclaration * DECL = new TiXmlDeclaration( "1.0", "ISO-8859-1", "" );
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

                    if (definition.GetProperty().Length()) PARAM->SetAttribute( "property", definition.GetProperty() );
                    if (definition.GetGroup().Length()) PARAM->SetAttribute( "group", definition.GetGroup() );
                    if (definition.GetTypeStr().Length()) PARAM->SetAttribute( "type" , definition.GetTypeStr() );
                    if (definition.GetOptionsStr().Length()) PARAM->SetAttribute( "options" , definition.GetOptionsStr() );

                    if (definition.GetInterpolation().GetFunctionToText()!=moText("none") ) {

                      PARAM->SetAttribute( "interpolation" , definition.GetInterpolation().GetFunctionToText() );
                      PARAM->SetAttribute( "duration" , IntToStr( definition.GetInterpolation().GetDuration() ) );

                    }

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
                                    if (xvaluedata.GetTypeStr()=="XML") {
                                      TiXmlDocument   XMLDoc;
                                      TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8
                                      XMLDoc.Parse((const char*) xvaluedata.Text(), 0, xencoding );
                                      TiXmlElement* rootKey = XMLDoc.FirstChildElement( "D" );
                                      TiXmlNode* XMLC;

                                      if (rootKey==NULL) {
                                        XMLC = XMLDoc.FirstChild();
                                      } else XMLC = rootKey->FirstChild();

                                      ///iterate childrens
                                      while(XMLC) {
                                        TNodeValue = XMLC->Value();
                                        VALUEDATA->InsertEndChild( *XMLC );
                                        XMLC = XMLC->NextSibling();
                                      }
                                    } else {
                                      TiXmlText * datatext = new TiXmlText( xvaluedata.ToText() );
                                      VALUEDATA->LinkEndChild( datatext );
                                    }


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

                    for( int pv=0; pv< (int)xPreConfig.m_PreconfIndexes.Count(); pv++) {

                        moPreconfigParamIndex xPreIndex = xPreConfig[pv];

                        TiXmlElement*  PREVALUE = new TiXmlElement("P");

                        if (PREVALUE) {


                            PREVALUE->SetAttribute( "name", GetParam( xPreIndex.m_ParamIndex ).GetParamDefinition().GetName() );

                            TiXmlText * datatext = new TiXmlText( IntToStr(xPreIndex.m_ValueIndex) );

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
        m_FileName = p_filename;
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
  if ( p_ParamDef.GetDefaultValue().GetSubValueCount()>0
      &&
      p_ParamDef.GetDefaultValue().GetSubValue(0).GetTypeStr()!="UNDEFINED" ) {
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
    if (param.GetData()) return param.GetData()->Int();
    return param.GetValue().GetSubValue().Int();
}

MOint
moConfig::Int( moText p_param_name ) {
    moParam& param(GetParam(p_param_name));
    if (param.GetData()) return param.GetData()->Int();
    return param.GetValue().GetSubValue().Int();
}

MOdouble
moConfig::Double( moParamReference p_paramreference ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    if (param.GetData()) return param.GetData()->Double();
    return param.GetValue().GetSubValue().Double();
}

MOdouble
moConfig::Double( moText p_param_name ) {
    moParam& param(GetParam(p_param_name));
    if (param.GetData()) return param.GetData()->Double();
    return param.GetValue().GetSubValue().Double();
}

moText
moConfig::Text( moParamReference p_paramreference ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    if (param.GetData()) return param.GetData()->Text();
    return param.GetValue().GetSubValue().Text();
}

moText
moConfig::Text( moText p_param_name ) {
    moParam& param(GetParam(p_param_name));
    if (param.GetData()) return param.GetData()->Text();
    return param.GetValue().GetSubValue().Text();
}

moText
moConfig::Text( int p_param_index ) {
    moParam& param(GetParam(p_param_index));
    if (param.GetData()) return param.GetData()->Text();
    return param.GetValue().GetSubValue().Text();
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
moConfig::GetGLId( moParamReference p_paramreference, MOfloat p_cycle, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {

    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_cycle, p_fade, p_filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, moTempo *p_tempo, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_tempo, p_fade, p_filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, MOuint p_i, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_i, p_fade, p_filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, MOfloat p_fade, const moTextFilterParam &p_filterparam ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
      return pdata->GetGLId( p_fade, p_filterparam );
    }
    return 0;
}


GLint
moConfig::GetGLId( moParamReference p_paramreference, MOfloat p_cycle ) {
    
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
        moTextFilterParam filterparam;
        return pdata->GetGLId( p_cycle, 1.0, filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, moTempo *p_tempo) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
        moTextFilterParam filterparam;
        return pdata->GetGLId( p_tempo, 1.0, filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference, MOuint p_i ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
        moTextFilterParam filterparam;
        return pdata->GetGLId( p_i, 1.0, filterparam );
    }
    return 0;
}

GLint
moConfig::GetGLId( moParamReference p_paramreference ) {
    moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
    moData* pdata = param.GetData();
    if (pdata) {
        moTextFilterParam filterparam;
        return pdata->GetGLId( 1.0, filterparam );
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

const moSceneNode&
moConfig::Model(  moParamReference p_paramreference ) {

  moParam& param( GetParam( m_ConfigDefinition.ParamIndexes().GetRef(p_paramreference.reference) ));
  moData* pdata = param.GetData();
  if (pdata) {
      moSceneNode* pModel = pdata->Model();
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
      moTexture* pTexture = pdata->Texture();
      if (pTexture) {
        return *pTexture;
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

bool
moConfig::SetCurrentParamIndex( int p_currentparam ) {
  if (  0<=p_currentparam
        && p_currentparam<(int)m_Params.Count()) {
    m_CurrentParam = p_currentparam;
    return true;
  }
  return false;
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

bool
moConfig::FirstValue() {
	if ( m_CurrentParam>=0 ) {
	  moParam& pParam( m_Params[m_CurrentParam] );
    if ( pParam.GetValuesCount() == 0 ) {
      return false;
    }
		pParam.FirstValue();
		return true;
	}
	return false;
}

bool
moConfig::NextValue() {
	if ( m_CurrentParam>=0 ) {
		moParam& pParam( m_Params[m_CurrentParam] );
    if ( pParam.GetIndexValue() == (int)(pParam.GetValuesCount()-1) ) {
      return false;
    }
		pParam.NextValue();
		return true;
	}
	return false;
}

bool
moConfig::PreviousValue() {
	if ( m_CurrentParam>=0 ) {
		moParam& pParam( m_Params[m_CurrentParam] );
    if ( pParam.GetIndexValue() == 0 ) {
      return false;
    }
		pParam.PrevValue();
		return true;
	}
	return false;
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
		for( MOuint i=0; i<m_PreConfigs[p_actual].m_PreconfIndexes.Count(); i++) {
			moPreconfigParamIndex Val = m_PreConfigs[p_actual][i];
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


const moPreConfig&
moConfig::GetPreconfig( int valueindex ) {
  if (0<=valueindex && valueindex<GetPreConfCount() ) {
    return m_PreConfigs.Get( valueindex );
  }
  return m_PreConfigs.Get(-1);
}

void
moConfig::AddPreconfig( moPreconfigIndexes& p_preconfindexes ) {
    m_PreConfigs.Add( moPreConfig( p_preconfindexes ) );
}

void
moConfig::AddPreconfig( int preconfig_index) {

  int new_preconfigs = preconfig_index - GetPreConfCount() + 1;

  //MODebug2->Message("moConsole::ProcessMoldeoAPIMessage > Adding Preconfigs: " + IntToStr(valcount) );
  //mo->Message("moConfig::AddPreconfig > Adding Preconfigs: " + IntToStr(new_preconfigs) );

  if (new_preconfigs>0) {
    int base_index = GetPreConfCount();
    int min_index = 0;
    for( int sum=0; sum < new_preconfigs ; sum++ ) {

      moPreConfig newPreCfg;

      /// ADD PARAMETERS THAT HAS BEEN
      /// CUSTOMIZED (so they have values count > 1)
      for( int pi=0; pi < GetParamsCount(); pi++) {
          moParam& rParam2( GetParam(pi) );
          int nvalues = GetParam(pi).GetValuesCount();
          min_index = nvalues - 1;

          if ( nvalues > 1 || ( preconfig_index==0 && rParam2.GetParamDefinition().GetName()!="inlet" && rParam2.GetParamDefinition().GetName()!="outlet" ) ) {

            moPreconfigParamIndex preIndexA;

            preIndexA.m_ParamName = rParam2.GetParamDefinition().GetName();
            preIndexA.m_ParamIndex = rParam2.GetParamDefinition().GetIndex();
            preIndexA.m_ValueIndex = momin( min_index, base_index+sum );

            newPreCfg.m_PreconfIndexes.Add( preIndexA );

          }
      }

      AddPreconfig( newPreCfg.m_PreconfIndexes );
    }
  }

}


void
moConfig::InsertPreconfig( int valueindex, moPreconfigIndexes& p_preconfindexes ) {
    m_PreConfigs.Insert( valueindex,  moPreConfig( p_preconfindexes ) );
}

void
moConfig::SetPreconfig( int valueindex, moPreconfigIndexes& p_preconfindexes ) {
    m_PreConfigs.Set( valueindex, moPreConfig( p_preconfindexes ) );
}

void
moConfig::DeletePreconfig( int valueindex ) {
    m_PreConfigs.Remove( valueindex );
}

const moText&
moConfig::ToJSON() {

  moText fieldSeparation = ",";

  m_FullJSON = "{";

  m_FullJSON+= "'configdefinition': " + this->GetConfigDefinition()->ToJSON();
  m_FullJSON+= fieldSeparation + "'currentpreconfig':" + IntToStr( this->GetCurrentPreConf() );
  m_FullJSON+= fieldSeparation + "'parameters': {";

  fieldSeparation  = "";

  for( int p=0; p<(int)m_Params.Count();p++) {
    moParam Param = m_Params[p];
    if ( Param.GetParamDefinition().GetType()==MO_PARAM_INLET
    || Param.GetParamDefinition().GetType()==MO_PARAM_OUTLET ) {
        ///outlets must not be in a param...
    } else {
      m_FullJSON+= fieldSeparation + "'" + Param.GetParamDefinition().GetName() + "': " + "{";
      m_FullJSON+= "}";
      fieldSeparation = ",";
    }

  }
  m_FullJSON+= "}";

  fieldSeparation = ",";
  m_FullJSON+= fieldSeparation + "'preconfigs': [";
  fieldSeparation  = "";
  for( int pre=0; pre<(int)m_PreConfigs.Count();pre++) {
    moPreConfig PreConfig = m_PreConfigs[pre];
    //m_FullJSON+= fieldSeparation + PreConfig.ToJSON();
    m_FullJSON+= fieldSeparation + "{'n': '"+PreConfig.m_Name+"'}";
    fieldSeparation = ",";
  }
  m_FullJSON+= "]";

  m_FullJSON+= "}";
  return m_FullJSON;

}


