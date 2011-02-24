/*******************************************************************************

                              moMoldeoObject.cpp

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

#include <moMoldeoObject.h>
#include <moTextureFilterIndex.h>
#include <moScriptManager.h>
#include <moMathManager.h>
#include <moFontManager.h>
#include <mo3dModelManager.h>
#include <moFilterManager.h>

#include "moArray.h"
moDefineDynamicArray(moMoldeoObjects)

//===========================================
//
//				moMoldeoObject
//
//===========================================

moMoldeoObject::moMoldeoObject() {
	SetId(-1);
	SetType(MO_OBJECT_UNDEFINED);
	SetLabelName("");
	SetName("");
	SetConfigName("");

	m_Description = moText("");
	m_Script = moText("");

	m_pResourceManager = NULL;
	m_Inlets.Init( 0 , NULL );
	m_Outlets.Init( 0 , NULL );
	m_bConnectorsLoaded = false;
	__iscript = -1;
}

moMoldeoObject::~moMoldeoObject() {
  moMoldeoObject::Finish();
}


MOboolean
moMoldeoObject::Init() {

    moText confignamecompleto="";

    GetDefinition();

    if ( GetType()==MO_OBJECT_CONSOLE ) {
      confignamecompleto = GetConfigName();
    } else {
      if (m_pResourceManager) {
        if (m_pResourceManager->GetDataMan()) {
          confignamecompleto = m_pResourceManager->GetDataMan()->GetDataPath();
          confignamecompleto +=  moSlash + GetConfigName();
          confignamecompleto +=  moText(".cfg");
        } else {
            MODebug2->Error("moMoldeoObject::Init > DataManager undefined > config:"+GetConfigName()+" label:"+GetLabelName() );
            return false;
        }
      } else {
          MODebug2->Error("moMoldeoObject::Init > ResourceManager undefined > config:"+GetConfigName()+" label:"+GetLabelName() );
          return false;
      }
    }

    MODebug2->Message("*****Initializing " + GetName() + " *****");

    if(m_Config.LoadConfig(confignamecompleto) != MO_CONFIG_OK ) {
      MODebug2->Error("moMoldeoObject::Init > Config file invalid or not found > object:" + GetName() + " config:" + confignamecompleto + " label: " + GetLabelName());
      return false;//bad
    }

    __iscript = m_Config.GetParamIndex("script");
    if(__iscript==MO_PARAM_NOT_FOUND)
      MODebug2->Error(moText("moMoldeoObject::Init > config:"+GetConfigName()+" label:"+GetLabelName()+" script parameter missing"));


    InitScript();
    RegisterFunctions();

    return true;
}

void moMoldeoObject::ScriptExeInit() {
    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Init")) {
            SelectScriptFunction("Init");
            RunSelectedFunction();
        }
    }
}

void moMoldeoObject::ScriptExeFinish() {
    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Finish")) {
            SelectScriptFunction("Finish");
            RunSelectedFunction();
        }
    }
}

void moMoldeoObject::ScriptExeUpdate() {
    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Update")) {
            SelectScriptFunction("Update");
            RunSelectedFunction();
        }
    }
}


void moMoldeoObject::ScriptExeRun() {


  moText cs;
  cs = m_Config.Text(__iscript);

  //MODebug2->Push( moText("cs:") + (moText)cs );
  //MODebug2->Push( moText("m_ConsoleScript:") +  (moText)m_ConsoleScript );
  ///Reinicializamos el script en caso de haber cambiado
	if ((moText)m_Script!=cs && IsInitialized()) {

        m_Script = cs;
        moText fullscript = m_pResourceManager->GetDataMan()->GetDataPath()+ moSlash + (moText)m_Script;

        if (moFileManager::FileExists(fullscript)) {

          MODebug2->Message( GetLabelName() +  moText(" script loading : ") + (moText)fullscript );

          if ( CompileFile(fullscript) ) {

              MODebug2->Message( GetLabelName() + moText(" script loaded : ") + (moText)fullscript );

              ///Reinicializamos el script

              SelectScriptFunction( "Init" );
              /**TODO: revisar uso de offset, para multipantallas
              moText toffset=moText("");

              toffset = m_Config[moR(CONSOLE_SCRIPT)][MO_SELECTED][1].Text();
              if (toffset!=moText("")) {
                  m_ScriptTimecodeOffset = atoi( toffset );
              } else {
                  m_ScriptTimecodeOffset = 0;
              }
              AddFunctionParam( (int)m_ScriptTimecodeOffset );
              */
              RunSelectedFunction();

          } else MODebug2->Error( moText("Couldn't compile lua script ") + (moText)fullscript + " config:"+GetConfigName()+" label:"+GetLabelName() );
        } else MODebug2->Message("Script file not present. " + (moText)fullscript + " config:"+GetConfigName()+" label:"+GetLabelName() );
	}


  ///Si tenemos un script inicializado... corremos la funcion Run()
    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Run")) {
            SelectScriptFunction("Run");
            RunSelectedFunction();
        }
    }
}


MOboolean
moMoldeoObject::CreateConnectors() {

  int idx = -1;

  if (m_pResourceManager == NULL) {
    MODebug2->Error("moMoldeoObject::CreateConnectors > ResourceManager is NULL!!! Can't continue. Sorry. Object: " + GetName() + " label:" + GetLabelName() );
    return false;
  }

  if (m_bConnectorsLoaded) {
    MODebug2->Error("moMoldeoObject::CreateConnectors : Calling twice. Can't continue. Sorry. Object: " + GetName() + "label:" + GetLabelName() );
    return false;
  }

	///Inicializa las funciones matemáticas del config
	///así como los inlets y outlets por cada parámetro
	///así como las texturas
	for( MOint p=0;p<m_Config.GetParamsCount();p++) {

		moParam	&param( m_Config[p] );

		MODebug2->Log( moText("moMoldeoObject:: Init param type ") + param.GetParamDefinition().GetTypeStr() + moText(" name: ") + param.GetParamDefinition().GetName() );


        ///CREAMOS UN INLET POR CADA PARAMETRO
        int inletidx = GetInletIndex(param.GetParamDefinition().GetName());
        if (inletidx==-1) {
          moInlet* Inlet = new moInlet();
          if (Inlet) {
            Inlet->Init( param.GetParamDefinition().GetName(), m_Inlets.Count(), param.GetPtr() );
            param.SetExternData( Inlet->GetData() );
            m_Inlets.Add(Inlet);
          }
        }

		for( MOuint v=0;v<param.GetValuesCount();v++) {

			moValue& value( param.GetValue(v) );

			//MODebug2->Message( moText("+Init value #") + IntToStr(v) );

            ///RESUELVE LAS FUNCIONES!!!!
            ///esto debe hacerse antes de aplicar filtros y otros...

            for( MOuint ivb=0; ivb<value.GetSubValueCount(); ivb++) {
                moValueBase& VB( value.GetSubValue( ivb ) );
                if (VB.GetType() == MO_VALUE_FUNCTION ) {
                    idx = -1;
                    if (m_pResourceManager->GetMathMan())
                      idx = m_pResourceManager->GetMathMan()->AddFunction( VB.Text(), (MOboolean)true, this );
                    if (idx>-1) {
                        VB.SetFun( m_pResourceManager->GetMathMan()->GetFunction(idx) );
                        //MODebug2->Message( moText("function defined: ") + VB.Text() );
                    } else {
                        MODebug2->Error(moText("function couldn't be defined: ") + VB.Text() );
                    }
                }
            }


            //

			if (value.GetSubValueCount()>0) {

                moValueBase& valuebase(value.GetSubValue(0));

                switch((int)param.GetParamDefinition().GetType() ) {

                    case MO_PARAM_TEXTUREFOLDER:
                        ///es una carpeta pero puede tener otros parametros
                        ///
                        if ( ! (valuebase.Text().Trim() == moText("")) ) {

                            ///si tenemos un segundo parametro deberia ser el formato del buffer (JPG)

                            idx = m_pResourceManager->GetTextureMan()->GetTextureBuffer( valuebase.Text(), true );
                            if (idx>-1) {

                                moTextureBuffer*  pTextureBuffer = m_pResourceManager->GetTextureMan()->GetTextureBuffer(idx);
                                valuebase.SetTextureBuffer( pTextureBuffer );

                            }

                        }


                        break;

                    case MO_PARAM_VIDEO:
                        ///ojo aquí el video es tratado por el VideoManager si quiere ser tratado realamente
                        /// como video y no como texturaanimada....

                        break;

                    case MO_PARAM_TEXTURE:
                    case MO_PARAM_FILTER:
                        if ( ! (valuebase.Text().Trim() == moText("")) ) {
                            idx = m_pResourceManager->GetTextureMan()->GetTextureMOId( valuebase.Text(), true);
                            if (idx>-1) {
                                moTexture*  pTexture = m_pResourceManager->GetTextureMan()->GetTexture(idx);
                                valuebase.SetTexture( pTexture );

                                if (pTexture->GetType()!=MO_TYPE_TEXTURE_MULTIPLE && value.GetSubValueCount()>1) {
                                    idx = m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->LoadFilter( &value );
                                    moTextureFilter*  pTextureFilter = m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->Get(idx-1);
                                    valuebase.SetTextureFilter( pTextureFilter );
                                }

                                if (value.GetSubValueCount()==4) {
                                    valuebase.SetTextureFilterAlpha( value.GetSubValue(3).GetData() );
                                }

                                if (value.GetSubValueCount()>=5) {
                                    //valuebase.SetTextureFilterParam( value.GetSubValue(4).GetData() );
                                }
                            }
                        } else {
                            MODebug2->Error( moText(" VALUE BASE EMPTY: ")+ valuebase.Text() + moText(" Param name:") +param.GetParamDefinition().GetName() );
                        }
                        break;

                    case MO_PARAM_FONT:

                        moFont* pFont;
                        moFontType fonttype;
                        moFontSize  fontsize;

                        if ( value.GetSubValueCount()==3 ) {
                            if ( valuebase.Text().Trim() == moText("Default") ) {
                              pFont = m_pResourceManager->GetFontMan()->GetFont(0);
                            } else if ( ! (valuebase.Text().Trim() == moText("")) ) {

                                if ( value.GetSubValue(1).GetType()==MO_VALUE_TXT) {
                                    moText fonttypeT = value.GetSubValue(1).Text();
                                    fonttype = m_pResourceManager->GetFontMan()->GetFontType(fonttypeT);
                                } else {
                                    fonttype = (moFontType)value.GetSubValue(1).Int();
                                }

                                if ( value.GetSubValue(2).GetType()==MO_VALUE_NUM ) {
                                    fontsize = value.GetSubValue(2).Int();
                                } else if ( value.GetSubValue(2).GetType()==MO_VALUE_FUNCTION ) {
                                    fontsize = 12;
                                }

                                pFont = m_pResourceManager->GetFontMan()->AddFont( valuebase.Text(), fonttype, fontsize);
                                if (pFont==NULL) {
                                  MODebug2->Error( moText(" FONT NOT FOUND: Using Default")+ valuebase.Text() + moText(" Param name:") +param.GetParamDefinition().GetName() );
                                  pFont = m_pResourceManager->GetFontMan()->GetFont(0);
                                }
                            } else {
                                MODebug2->Error( moText(" VALUE BASE EMPTY: Using Default")+ valuebase.Text() + moText(" Param name:") +param.GetParamDefinition().GetName() );
                                pFont = m_pResourceManager->GetFontMan()->GetFont(0);
                            }

                            if (pFont) {
                                valuebase.SetFont( pFont );
                            }
                        } else {
                            MODebug2->Error( moText(" MISSING VALUES: ")+ valuebase.Text() + moText(" Param name:") +param.GetParamDefinition().GetName() );
                        }
                        break;

                    case MO_PARAM_3DMODEL:
                    case MO_PARAM_OBJECT:
                        if (value.GetSubValueCount()>0) {
                            //PROBAR!!!!!

                            moValueBase& valuebase(value.GetSubValue(0));
                            mo3dModel* pModel = m_pResourceManager->GetModelMan()->Get3dModel( valuebase.Text() );
                            if (pModel) {
                                mo3DModelSceneNode *newSceneNode;
                                newSceneNode = new mo3DModelSceneNode();
                                if (newSceneNode) {
                                    newSceneNode->Init(pModel);
                                }
                                valuebase.SetModel( newSceneNode );
                            }
                        }
                        break;

                    case MO_PARAM_SOUND:
                        if (value.GetSubValueCount()>0) {
                          moValueBase& valuebase(value.GetSubValue(0));
                          moSound* pSound = m_pResourceManager->GetSoundMan()->GetSound( valuebase.Text() );
                          if (pSound) {
                              valuebase.SetSound( pSound );
                          }
                        }
                        break;

                }//fin siwtch

			}//fin if
		}
	}

	///Levanta los Inlets adicionales a los parámetros....
	moParam& pinlets = m_Config[moText("inlet")];

	for( MOuint i=0; i<pinlets.GetValuesCount(); i++ ) {
		moInlet* Inlet = new moInlet();
		if (Inlet) {
			Inlet->SetMoldeoLabelName( GetLabelName() );
			moText InletName = pinlets[i][MO_INLET_NAME].Text();
			///lo creamos si y solo si no existe como parametro....
			if ( m_Config.GetParamIndex(InletName)==-1 ) {
				Inlet->Init( InletName, m_Inlets.Count(), pinlets[i][MO_INLET_TYPE].Text() );
				m_Inlets.Add( Inlet );
			}
		}
	}

    /** VERIFICAR ESTO!!!!*/
    /**
    Solo se crean los outlets declarados en el xml.
    */

    ///Levanta Outlets definidos en el config
	moParam& poutlets = m_Config[moText("outlet")];

	for( MOuint i=0; i<poutlets.GetValuesCount(); i++ ) {
		moOutlet* Outlet = new moOutlet();
		if (Outlet) {
			Outlet->SetMoldeoLabelName( GetLabelName() );
			///Buscamos el parametro asociado al outlet
			///para asociar un parametro a un outlet debe simplemente tener el mismo nombre...
			moText OutletName = poutlets[i][MO_OUTLET_NAME].Text();

			if ( m_Config.GetParamIndex(OutletName)>-1 ) {
			  ///CREAMOS UN OUTLET nuevo para este parametro....
			  MODebug2->Message( this->GetLabelName() + moText(" Init > creating Outlet automatic to param.") + OutletName  );
				Outlet->Init( OutletName, i, m_Config.GetParam(OutletName).GetPtr());
			} else {
			  ///CREAMOS UN OUTLET desde el .cfg, teniendo en cuenta los tipos...
			  MODebug2->Message( this->GetLabelName() + moText(" Init > creating outlet not as param.") + OutletName  );
				Outlet->Init( OutletName, i, poutlets[i][MO_OUTLET_TYPE].Text() );
			}
			m_Outlets.Add( Outlet );
			for( MOuint j=MO_OUTLET_INLETS_OFFSET; j<poutlets[i].GetSubValueCount(); j+=2 ) {
				moText objectname = poutlets[i][j].Text();
				moText inletname = poutlets[i][j+1].Text();
				moConnection* Connection = new moConnection( objectname, inletname );
				if (Connection)
					Outlet->GetConnections()->Add(Connection);
			}
		}
	}

  m_bConnectorsLoaded = true;

  ///Una vez establecidos los conectores, podemos inicializar el script a su vez....
	moMoldeoObject::ScriptExeInit();

	return m_bConnectorsLoaded;
}

MOboolean
moMoldeoObject::Init( moResourceManager* p_pResources ) {
	m_pResourceManager = p_pResources;

	return moMoldeoObject::Init();
}


MOboolean
moMoldeoObject::Finish() {

  ScriptExeFinish();

  for(int i=0; i<m_Inlets.Count(); i++ ) {
    moInlet *pInlet = dynamic_cast<moInlet*>(m_Inlets[i]);
    if (pInlet) {
        delete pInlet;
    }
  }
  m_Inlets.Empty();

  for(int i=0; i<m_Outlets.Count(); i++ ) {
    moOutlet *pOutlet = dynamic_cast<moOutlet*>(m_Outlets[i]);
    if (pOutlet) {
        delete pOutlet;
    }
  }
  m_Outlets.Empty();

  return true;

}



moMoldeoObjectType
moMoldeoObject::GetType() {
	return m_MobDefinition.GetType();
}

void
moMoldeoObject::SetType( moMoldeoObjectType p_type ) {
	m_MobDefinition.SetType(p_type);
}

MOint
moMoldeoObject::GetId() {
	return m_MobDefinition.GetMoldeoId();
}

void
moMoldeoObject::SetId( MOint	p_id ) {
	m_MobDefinition.SetMoldeoId( p_id );
}


void
moMoldeoObject::SetResourceManager( moResourceManager* p_pResourceManager ) {

	m_pResourceManager = p_pResourceManager;

}

moResourceManager*
moMoldeoObject::GetResourceManager() {

	return m_pResourceManager;

}

void
moMoldeoObject::SetConfigName( moText p_configname ) {
	m_MobDefinition.SetConfigName( p_configname );
}


moConfigDefinition *
moMoldeoObject::GetDefinition( moConfigDefinition *p_configdefinition ) {

	if ( p_configdefinition==NULL ) {
		p_configdefinition = m_Config.GetConfigDefinition();
	}

    p_configdefinition->GetParamDefinitions()->Empty();
    p_configdefinition->ParamIndexes().Empty();

  p_configdefinition->Set( GetName(), m_MobDefinition.GetTypeStr() );
	p_configdefinition->Add( moText("inlet"), MO_PARAM_INLET );
	p_configdefinition->Add( moText("outlet"), MO_PARAM_OUTLET );
	p_configdefinition->Add( moText("script"), MO_PARAM_SCRIPT );
	return p_configdefinition;
}

void
moMoldeoObject::LoadDefinition() {

	GetDefinition();

	//m_Config.Check();

	moParamDefinitions *pdefinitions = m_Config.GetConfigDefinition()->GetParamDefinitions();

	for( MOuint i=0; i < pdefinitions->Count(); i++ ) {
		pdefinitions->Get(i).SetIndex( m_Config.GetParamIndex( pdefinitions->Get(i).GetName() ));
	}

}

moOutlets*
moMoldeoObject::GetOutlets() {
	return &m_Outlets;
}


moInlets*
moMoldeoObject::GetInlets() {
	return &m_Inlets;
}

MOint
moMoldeoObject::GetInletIndex( moText p_connector_name ) {

	for( MOuint i=0; i< m_Inlets.Count(); i++ ) {
	    moInlet* pInlet = m_Inlets[i];
      if ( pInlet )
            if ( pInlet->GetConnectorLabelName() == p_connector_name )
                return i;
	}
	return (-1);

}

MOint
moMoldeoObject::GetOutletIndex( moText p_connector_name ) {

	for( MOuint i=0; i< m_Outlets.Count(); i++ ) {
	    moOutlet* pOutlet = m_Outlets[i];
		if ( pOutlet )
            if ( pOutlet->GetConnectorLabelName() == p_connector_name )
                return i;
	}
	return (-1);

}


void
moMoldeoObject::Update( moEventList* p_EventList ) {

	moEvent *actual,*tmp;
	moMessage *pmessage;

	actual = p_EventList->First;

	///Procesamos los eventos recibidos de los MoldeoObject Outlets
	while(actual!=NULL) {
		tmp = actual->next;
		///procesamos aquellos Outlet q estan dirigidos a este objeto

		if (actual->deviceid == GetId() && actual->reservedvalue3 == MO_MESSAGE) {

			///pSample = (moVideoSample*)actual->pointer;
			pmessage = (moMessage*)actual;

			///process message:
			MOint inletid = pmessage->m_InletIdDest;
			moData pdata = pmessage->m_Data;

			///buscar el inlet...
			if (inletid>=0 && inletid<(int)m_Inlets.Count() ) {
				moInlet* pinlet = m_Inlets[inletid];
				if (pinlet->GetData()==NULL) pinlet->NewData();
				pinlet->GetData()->Copy(pdata);
				pinlet->Update();///notifica al inlet que ya esta actualizado...
			}

		} else if (actual->reservedvalue3 == MO_MESSAGE) {
		    ///Broadcasting: borra su propio mensaje....

			pmessage = (moMessage*)actual;

			///se fija si es un mensaje generado por este objeto
			if (pmessage->m_MoldeoIdSrc == GetId() ) {
				p_EventList->Delete(pmessage);
			}

		}
		///pasamos al siguiente
		actual = tmp;
	}


	///generamos los mensajes emergentes de los Outlets
	for( MOuint i=0; i<m_Outlets.Count() ; i++) {
		moOutlet* poutlet = m_Outlets[i];

        if (poutlet)
		if (poutlet->Updated()) {
		  ///solo notificamos a los inlets si los outlets estan Updated() importante revisar esto...
		  ///puede  deba ser algo condicional

		  //MODebug2->Message( poutlet->GetConnectorLabelName() + moText(" outlet updated. MOB : ") + this->GetLabelName() );

			moData pdata = (*(poutlet->GetData()));
			moConnections* pconnections = poutlet->GetConnections();
			for(MOuint j=0; j<pconnections->Count(); j++) {
				moConnection* pconnection = pconnections->Get(j);
				pmessage = new moMessage( pconnection->GetDestinationMoldeoId(), pconnection->GetDestinationConnectorId(), GetId(), pdata);
				p_EventList->Add( (moEvent*) pmessage );
				//if (pmessage) delete pmessage;
			}
		}
	}

  moMoldeoObject::ScriptExeUpdate();

}


///beware ! call only once or die!!!
void moMoldeoObject::RegisterFunctions()
{
    RegisterBaseFunction("PushDebugString"); //0
    RegisterFunction("GetResourceManager"); //1

    ///functions to access moConfig data
	RegisterFunction("SetPreconf");//2
	RegisterFunction("GetPreconf");//3

	RegisterFunction("GetParamIndex");//4
	RegisterFunction("GetCurrentValue");//5
	RegisterFunction("SetCurrentValue");//6

	RegisterFunction("GetInletIndex");//7
	RegisterFunction("GetInletData");//8
	RegisterFunction("SetInletData");//9

    ///for TUIO tracking
    RegisterFunction("GetTuioSystem");//10
    RegisterFunction("GetTuioCursorCount");//11
    RegisterFunction("GetTuioCursor");//12
    RegisterFunction("GetTuioObjectCount");//13
    RegisterFunction("GetTuioObject");//14

    ///for features tracking

    ///all values nvalids, nfeatures, barycenter, acceleration, velocity
	RegisterFunction("GetTrackerSystemData"); //15
	RegisterFunction("GetTrackerFeaturesCount"); //16
	RegisterFunction("GetTrackerValidFeatures"); //17
	RegisterFunction("GetTrackerFeature"); //18
	RegisterFunction("GetTrackerVariance"); //19
	RegisterFunction("GetTrackerBarycenter"); //20
	RegisterFunction("GetTrackerAcceleration"); //21
	RegisterFunction("GetTrackerVelocity"); //22
	RegisterFunction("GetTrackerZone"); //23

	RegisterFunction("GetTrackerHistory"); //24
	RegisterFunction("StartTrackerHistory"); //25
	RegisterFunction("PauseTrackerHistory"); //26
	RegisterFunction("ContinueTrackerHistory"); //27
	RegisterFunction("StopTrackerHistory"); //28
	RegisterFunction("GetHistoryRecord"); //29
	RegisterFunction("GetHistoryBarycenter"); //30
	RegisterFunction("GetHistoryVariance"); //31
	RegisterFunction("GetHistoryBounding"); //32
	RegisterFunction("GetHistoryAverage"); //33
	RegisterFunction("GetHistoryMinMax"); //34

}

int moMoldeoObject::ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber) {

 switch ( iFunctionNumber - m_iMethodBase )
    {
        case 0:
            ResetScriptCalling();
            return luaPushDebugString(vm);//0

        case 1:
            ResetScriptCalling();
            return luaGetResourceManager(vm);//1

        ///functions to access moConfig data
        case 2:
            ResetScriptCalling();
            return luaSetPreconf(vm);//2
        case 3:
            ResetScriptCalling();
            return luaGetPreconf(vm);//3


        case 4:
            ResetScriptCalling();
            return luaGetParamIndex(vm);//4
        case 5:
            ResetScriptCalling();
            return luaGetCurrentValue(vm);//5
        case 6:
            ResetScriptCalling();
            return luaSetCurrentValue(vm);//6



        case 7:
            ResetScriptCalling();
            return luaGetInletIndex(vm);//7
        case 8:
            ResetScriptCalling();
            return luaGetInletData(vm);//8
        case 9:
            ResetScriptCalling();
            return luaSetInletData(vm);//9


        ///functions to access Inlets Data
        ///TUIO
        ///Tracker, etc...
        case 10:
            ResetScriptCalling();
            return luaGetTuioSystem(vm);//10
        case 11:
            ResetScriptCalling();
            return  luaGetTuioCursorCount(vm);//11
        case 12:
            ResetScriptCalling();
            return  luaGetTuioCursor(vm);//12
        case 13:
            ResetScriptCalling();
            return  luaGetTuioObjectCount(vm);//13
        case 14:
            ResetScriptCalling();
            return  luaGetTuioObject(vm);//14


        case 15:
            ResetScriptCalling();
            return luaGetTrackerSystemData(vm);//15
        case 16:
            ResetScriptCalling();
            return luaGetTrackerFeaturesCount(vm);//16
        case 17:
            ResetScriptCalling();
            return luaGetTrackerValidFeatures(vm);//17
        case 18:
            ResetScriptCalling();
            return luaGetTrackerFeature(vm);//18
        case 19:
            ResetScriptCalling();
            return luaGetTrackerVariance(vm);//19
        case 20:
            ResetScriptCalling();
            return luaGetTrackerBarycenter(vm);//20
        case 21:
            ResetScriptCalling();
            return luaGetTrackerAcceleration(vm);//21
        case 22:
            ResetScriptCalling();
            return luaGetTrackerVelocity(vm);//22
        case 23:
            ResetScriptCalling();
            return luaGetTrackerZone(vm);//23

        case 24:
            ResetScriptCalling();
            return luaGetTrackerHistory(vm);//24
        case 25:
            ResetScriptCalling();
            return luaStartTrackerHistory(vm);//25
        case 26:
            ResetScriptCalling();
            return luaPauseTrackerHistory(vm);//26
        case 27:
            ResetScriptCalling();
            return luaContinueTrackerHistory(vm);//27
        case 28:
            ResetScriptCalling();
            return luaStopTrackerHistory(vm);//28

        case 29:
            ResetScriptCalling();
            return luaGetHistoryRecord(vm);//29
        case 30:
            ResetScriptCalling();
            return luaGetHistoryBarycenter(vm);//30
        case 31:
            ResetScriptCalling();
            return luaGetHistoryVariance(vm);//31
        case 32:
            ResetScriptCalling();
            return luaGetHistoryBounding(vm);//32
        case 33:
            ResetScriptCalling();
            return luaGetHistoryAverage(vm);//33
        case 34:
            ResetScriptCalling();
            return luaGetHistoryMinMax(vm);//34

	}
    return 0;
}
//#include "lua.h"
void moMoldeoObject::HandleReturns(moLuaVirtualMachine& vm, const char *strFunc)
{
    lua_State *state = (lua_State *) vm;

    if (    strcmp( "Run", strFunc) == 0 ||
            strcmp( "Draw", strFunc) == 0 ||
            strcmp( "Init", strFunc) == 0 ||
            strcmp( "Finish", strFunc) == 0 ||
            strcmp( "RunSystem", strFunc) == 0 ||
            strcmp( "Update", strFunc) == 0 ||
            strcmp( "RunParticle", strFunc) == 0
        ) {
        MOint script_result = (MOint) lua_tonumber (state, 1);

        if (script_result != 0) {

            char *text = (char *) lua_tostring (state, -1);

            MODebug2->Error(
                moText(GetLabelName())
                + moText(" function:")
                + moText(strFunc)
                + moText(" >> lua script returned error code: ")
                + (moText)IntToStr(script_result)
                + moText(" >> ")
                + moText(text)
            );

        }
    }
}


int moMoldeoObject::luaPushDebugString(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;
	if (lua_isboolean(state,1)) {
		bool vb = lua_toboolean(state,1);
		vb ? MODebug2->Message(moText("true")) : MODebug2->Push(moText("false"));
	} else {
		char *text = (char *) lua_tostring (state, 1);
		MODebug2->Message(moText(text));
	}

    return 0;
}

int moMoldeoObject::luaGetResourceManager(moLuaVirtualMachine& vm) {

  lua_State *state = (lua_State *) vm;
  if (GetResourceManager()) {
    if (GetResourceManager()->GetScriptMan()) {
      GetResourceManager()->GetScriptMan()->PushLuaResourceManager();
    }
  }
  return 1;
}

///functions to access moConfig data
int moMoldeoObject::luaSetPreconf(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    MOint preconfid = (MOint) lua_tonumber (state, 1);

    if (this->GetConfig()) {
        this->GetConfig()->SetCurrentPreConf( preconfid );
    } else MODebug2->Push("no debug");

    return 0;
}

int moMoldeoObject::luaGetPreconf(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    if (this->GetConfig()) {
        lua_pushnumber(state, (lua_Number) this->GetConfig()->GetCurrentPreConf() );
    }

    return 1;
}

int moMoldeoObject::luaGetParamIndex(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    char *text = (char *) lua_tostring (state, 1);

    if (GetConfig()) {
        lua_pushnumber(state, (lua_Number) GetConfig()->GetParamIndex(text) );
    }

    return 1;

}

int moMoldeoObject::luaSetCurrentValue(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint paramid = (MOint) lua_tonumber (state, 1);
    MOint valueid = (MOint) lua_tonumber (state, 2);

    if (GetConfig()) {
        GetConfig()->SetCurrentValueIndex( paramid, valueid );
    } else {
        MODebug2->Error( moText(" in MoldeoObject script: SetCurrentValue : config not founded : id:")+(moText)GetLabelName() );
    }

    return 0;
}


int moMoldeoObject::luaGetCurrentValue(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint paramid = (MOint) lua_tonumber (state, 1);

    if (GetConfig()) {
        int valueid = GetConfig()->GetParam( paramid ).GetIndexValue();
        lua_pushnumber(state, (lua_Number) valueid );
        return 1;
    } else {
        MODebug2->Error( moText(" in MoldeoObject script: GetCurrentValue : config not founded : id:")+(moText)GetLabelName() );
    }

    return 0;
}






int moMoldeoObject::luaGetInletIndex(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    char *inletlabelname = (char *) lua_tostring (state, 1);

    int inletid = -1;

    for( int i=0; i<m_Inlets.Count(); i++ ) {
        moInlet* pInlet = m_Inlets[i];
        if (pInlet) {
            moText lname = pInlet->GetConnectorLabelName();
            moText lmname( inletlabelname );
            if ( lname == lmname ) {
               lua_pushnumber(state, (lua_Number) i );
               return 1;
            }
        }
    }

    lua_pushnumber(state, (lua_Number) -1 );
    return 1;
}

int moMoldeoObject::luaGetInletData(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    moVector2d* pv2d;
    moVector3d* pv3d;
    moVector4d* pv4d;
    moVector2i* pv2i;
    moVector3i* pv3i;
    moVector4i* pv4i;


    MOint inletindex = (MOint) lua_tonumber (state, 1);

    if ( inletindex>=0 && inletindex<m_Inlets.Count()) {
        moInlet* pInlet = m_Inlets[inletindex];
        if ( pInlet ) {
            moData* pData = pInlet->GetData();
            moDataType pType = pData->Type();
            lua_pushnumber( state, (lua_Number) (int)pType );
            switch(pType) {
                case 	MO_DATA_NUMBER:
                        MO_DATA_NUMBER_CHAR:
                        MO_DATA_NUMBER_INT:
                        MO_DATA_NUMBER_LONG:
                        lua_pushnumber( state, (lua_Number) pData->Long() );
                        return 1;
                case    MO_DATA_NUMBER_DOUBLE:
                        lua_pushnumber( state, (lua_Number) pData->Double() );
                        return 1;
                case    MO_DATA_NUMBER_FLOAT:
                        lua_pushnumber( state, (lua_Number) pData->Float() );
                        return 1;
                case    MO_DATA_TEXT:
                        lua_pushstring( state, (char*) pData->Text() );
                        return 1;
                case MO_DATA_VECTOR2F:
                        pv2d = (moVector2d*)pData->Pointer();
                        lua_pushnumber( state, (lua_Number) pv2d->X() );
                        lua_pushnumber( state, (lua_Number) pv2d->Y() );
                        return 2;
                case MO_DATA_VECTOR3F:
                        pv3d = (moVector3d*)pData->Pointer();
                        lua_pushnumber( state, (lua_Number) pv3d->X() );
                        lua_pushnumber( state, (lua_Number) pv3d->Y() );
                        lua_pushnumber( state, (lua_Number) pv3d->Z() );
                        return 3;
                case MO_DATA_VECTOR4F:
                        pv4d = (moVector4d*)pData->Pointer();
                        lua_pushnumber( state, (lua_Number) pv4d->X() );
                        lua_pushnumber( state, (lua_Number) pv4d->Y() );
                        lua_pushnumber( state, (lua_Number) pv4d->Z() );
                        lua_pushnumber( state, (lua_Number) pv4d->W() );
                        return 4;

                case MO_DATA_VECTOR2I:
                        pv2i = (moVector2i*)pData->Pointer();
                        lua_pushnumber( state, (lua_Number) pv2i->X() );
                        lua_pushnumber( state, (lua_Number) pv2i->Y() );
                        return 2;
                case MO_DATA_VECTOR3I:
                        pv3i = (moVector3i*)pData->Pointer();
                        lua_pushnumber( state, (lua_Number) pv3i->X() );
                        lua_pushnumber( state, (lua_Number) pv3i->Y() );
                        lua_pushnumber( state, (lua_Number) pv3i->Z() );
                        return 3;
                case MO_DATA_VECTOR4I:
                        pv4i = (moVector4i*)pData->Pointer();
                        lua_pushnumber( state, (lua_Number) pv4i->X() );
                        lua_pushnumber( state, (lua_Number) pv4i->Y() );
                        lua_pushnumber( state, (lua_Number) pv4i->Z() );
                        lua_pushnumber( state, (lua_Number) pv4i->W() );
                        return 4;
                        /*
                case MO_DATA_FUNCTION:
                        //lua_pushnumber( state, (lua_Number) pData->Fun()->Eval() );
                        return 1;*/
                default:
                    moText ttype = pData->TypeToText();
                    lua_pushstring( state, ttype );
                    return 1;


            }
        }
        moText tres("inlet data not updated");
        lua_pushstring( state, tres );
        return 1;

    }
    moText tres("invalid");
    lua_pushstring( state, tres );
    return 1;
}


int moMoldeoObject::luaSetInletData(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    MOint inletindex = (MOint) lua_tonumber (state, 1);

    if ( inletindex>=0 && inletindex<m_Inlets.Count()) {

        moInlet* pInlet = m_Inlets[inletindex];

        if ( pInlet ) {
            moData* pData = pInlet->GetData();
            moDataType pType = pData->Type();
            if (pData) {
                switch(pData->Type()) {
                    case MO_DATA_NUMBER:
                    case MO_DATA_NUMBER_CHAR:
                    case MO_DATA_NUMBER_INT:
                    case MO_DATA_NUMBER_LONG:
                    case MO_DATA_NUMBER_MIDI:
                        pData->SetLong( (MOlong) lua_tonumber ( state, 2 ) );
                        pInlet->Update();

                        return 0;

                    case MO_DATA_3DMODELPOINTER:
                    case MO_DATA_FONTPOINTER:
                    case MO_DATA_IMAGESAMPLE:
                    case MO_DATA_IMAGESAMPLE_FILTERED:
                    case MO_DATA_IMAGESAMPLE_TEXTUREBUFFER:
                        //pData->SetLong( (MOlong) lua_tonumber ( state, 3 ) );
                        pInlet->Update();
                        return 0;

                    case MO_DATA_VECTOR2I:
                        (*pData->Vector2i()) = moVector2i(  (MOlong) lua_tonumber ( state, 2 ),
                                                            (MOlong) lua_tonumber ( state, 3 ) );
                        pInlet->Update();

                        return 0;

                    case MO_DATA_VECTOR3I:
                        (*pData->Vector3i()) = moVector3i(  (MOlong) lua_tonumber ( state, 2 ),
                                                            (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ) );
                                                            pInlet->Update();
                        return 0;

                    case MO_DATA_VECTOR4I:
                        (*pData->Vector4i()) = moVector4i(  (MOlong) lua_tonumber ( state, 2 ),
                                                            (MOlong) lua_tonumber ( state, 3 ),
                                                            (MOlong) lua_tonumber ( state, 4 ),
                                                            (MOlong) lua_tonumber ( state, 5 ) );
                                                            pInlet->Update();
                        return 0;

                    case MO_DATA_VECTOR2F:
                        (*pData->Vector2d()) = moVector2d(  (MOdouble) lua_tonumber ( state, 2 ),
                                                            (MOdouble) lua_tonumber ( state, 3 ));
                                                            pInlet->Update();
                        return 0;

                    case MO_DATA_VECTOR3F:
                        (*pData->Vector3d()) = moVector3d(  (MOdouble) lua_tonumber ( state, 2 ),
                                                            (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ));
                        pInlet->Update();
                        return 0;

                    case MO_DATA_VECTOR4F:
                        (*pData->Vector4d()) = moVector4d(  (MOdouble) lua_tonumber ( state, 2 ),
                                                            (MOdouble) lua_tonumber ( state, 3 ),
                                                            (MOdouble) lua_tonumber ( state, 4 ),
                                                            (MOdouble) lua_tonumber ( state, 5 ) );
                        pInlet->Update();
                        return 0;

                    case MO_DATA_MESSAGE:
                    case MO_DATA_MESSAGES:
                        pInlet->Update();
                        return 0;

                    case MO_DATA_NUMBER_DOUBLE:
                    case MO_DATA_NUMBER_FLOAT:
                        pData->SetDouble( (MOdouble) lua_tonumber ( state, 2 ) );
                        pInlet->Update();
                        return 0;

                    case MO_DATA_TEXT:
                        //lua_pushstring(state, pData->Text() );
                        pData->SetText( lua_tostring ( state, 2 ) );
                        pInlet->Update();
                        return 0;

                    default:
                        moText tres("data type invalid");
                        tres = tres + pData->TypeToText();
                        lua_pushstring( state, tres );
                        pInlet->Update();
                        return 1;

                }
            } else {
                moText tres("data si null");
                lua_pushstring( state, tres );
                return 1;
            }
        }
    }

    moText tres("invalid");
    lua_pushstring( state, tres );
    return 1;
}




/**
*
*
*
*           TRACKERS LUA CALLING METHODS
*
*               TUIO Methods
*
*/



using namespace TUIO;
///functions to access Inlets Data
///TUIO
///Tracker, etc...
int moMoldeoObject::luaGetTuioSystem(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    int tuioindex = GetInletIndex("TUIOSYSTEM");

    lua_pushnumber( state, (lua_Number) tuioindex );

    return 1;
}

int moMoldeoObject::luaGetTuioCursorCount(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTUIOSystemData* pTuio = NULL;

    int tuioindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[tuioindex];

    if (pInlet && pInlet->Updated()) {
        pTuio = (moTUIOSystemData*)pInlet->GetData()->Pointer();
        if (pTuio) {
            std::list<TuioCursor*> pCursors = pTuio->getTuioCursors();
            int nc = pCursors.size();
            lua_pushnumber( state, (lua_Number) nc );
            return 1;
        }

    }
    lua_pushnumber( state, (lua_Number) -1 );
    return 1;
}


int moMoldeoObject::luaGetTuioCursor(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTUIOSystemData* pTuio = NULL;

    int tuioindex = (int)lua_tonumber( state, 1);
    int cursorindex = (int)lua_tonumber( state, 2);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[tuioindex];

    if (pInlet && pInlet->Updated()) {
        pTuio = (moTUIOSystemData*)pInlet->GetData()->Pointer();
        if (pTuio) {

            std::list<TuioCursor*> cursors = pTuio->getTuioCursors();

            int cn = 0;
            TuioCursor* pCursor = NULL;

            for (std::list<TuioCursor*>::iterator tuioCursor = cursors.begin(); tuioCursor!=cursors.end(); tuioCursor++,cn++) {
                if (cn == cursorindex) {
                    pCursor = (*tuioCursor);
                }
            }

            if (pCursor) {
                lua_pushnumber( state, (lua_Number) pCursor->getCursorID() );
                lua_pushnumber( state, (lua_Number) pCursor->getSessionID() );
                lua_pushnumber( state, (lua_Number) pCursor->getStartTime().getTotalMilliseconds() );
                lua_pushnumber( state, (lua_Number) pCursor->getPosition().getX() );
                lua_pushnumber( state, (lua_Number) pCursor->getPosition().getY() );
                lua_pushnumber( state, (lua_Number) pCursor->getXSpeed() );
                lua_pushnumber( state, (lua_Number) pCursor->getYSpeed() );
                return 7;
            }
        }

    }
    lua_pushnumber( state, (lua_Number) -1 );
    return 1;
}

int moMoldeoObject::luaGetTuioObjectCount(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTUIOSystemData* pTuio = NULL;

    int tuioindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[tuioindex];

    if (pInlet && pInlet->Updated()) {
        pTuio = (moTUIOSystemData*)pInlet->GetData()->Pointer();
        if (pTuio) {
            std::list<TuioObject*> pObjects = pTuio->getTuioObjects();
            int nc = pObjects.size();
            lua_pushnumber( state, (lua_Number) nc );
            return 1;
        }

    }
    ///Inlet not available
    lua_pushnumber( state, (lua_Number) -1 );
    return 1;
}

int moMoldeoObject::luaGetTuioObject(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTUIOSystemData* pTuio = NULL;

    int tuioindex = (int)lua_tonumber( state, 1);
    int objectindex = (int)lua_tonumber( state, 2);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[tuioindex];

    if (pInlet && pInlet->Updated()) {
        pTuio = (moTUIOSystemData*)pInlet->GetData()->Pointer();
        if (pTuio) {

            std::list<TuioObject*> objects = pTuio->getTuioObjects();

            int cn = 0;
            TuioObject* pObject = NULL;

            for (std::list<TuioObject*>::iterator tuioObject = objects.begin(); tuioObject!=objects.end(); tuioObject++,cn++) {
                if (cn == objectindex) {
                    pObject = (*tuioObject);
                }
            }

            if (pObject) {
                lua_pushnumber( state, (lua_Number) pObject->getSymbolID() );
                lua_pushnumber( state, (lua_Number) pObject->getSessionID() );
                lua_pushnumber( state, (lua_Number) pObject->getStartTime().getTotalMilliseconds() );
                lua_pushnumber( state, (lua_Number) pObject->getPosition().getX() );
                lua_pushnumber( state, (lua_Number) pObject->getPosition().getY() );
                lua_pushnumber( state, (lua_Number) pObject->getXSpeed() );
                lua_pushnumber( state, (lua_Number) pObject->getYSpeed() );
                lua_pushnumber( state, (lua_Number) pObject->getAngle() );
                return 8;
            }
        }

    }
    lua_pushnumber( state, (lua_Number) -1 );
    return 1;

}


/**
*
*
*           FEATURE TRACKING METHODS
*
*
*/




int moMoldeoObject::luaGetTrackerSystemData(moLuaVirtualMachine& vm) {

    ///verificar la existencia de un inlet con esas caracterisitcas
    ///si existe, ademas que tenga un dato!!!
    lua_State *state = (lua_State *) vm;

    int trackerindex = GetInletIndex("TRACKERKLT");

    lua_pushnumber( state, (lua_Number) trackerindex );

    return 1;
}

int moMoldeoObject::luaGetTrackerFeaturesCount(moLuaVirtualMachine& vm) {

    ///este debería devolver -1 si no existe un dato de trackeo disponible...
    lua_State *state = (lua_State *) vm;
    int ft_count = -1;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            ft_count = pTracker->GetFeaturesCount();
        }
    }

    lua_pushnumber( state, (lua_Number) ft_count );

    return 1;
}

int moMoldeoObject::luaGetTrackerValidFeatures(moLuaVirtualMachine& vm) {

    ///este debería devolver -1 si no existe un dato de trackeo disponible...
    lua_State *state = (lua_State *) vm;
    int ft_validcount = 0;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            ft_validcount = pTracker->GetValidFeatures();
        }
    }

    lua_pushnumber( state, (lua_Number) ft_validcount );

    return 1;
}

int moMoldeoObject::luaGetTrackerFeature(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int featureindex = (MOint) lua_tonumber (state, 2);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    float x, y;
    int v;
    x = y = 0.0;
    v = 0;

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            moTrackerFeature* TF = pTracker->GetFeature(featureindex);
            if (TF) {
                x = TF->x;
                y = TF->y;
                v = TF->val;
            }
        }
    }

	lua_pushnumber(state, (lua_Number)x);
	lua_pushnumber(state, (lua_Number)y);
	lua_pushnumber(state, (lua_Number)v);

    return 3;

}

int moMoldeoObject::luaGetTrackerVariance(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

   float x, y, L;
    x = y = L = 0.0;

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            L = pTracker->GetVariance().Length();
            x = pTracker->GetVariance().X();
            y = pTracker->GetVariance().Y();
        }
    }

	lua_pushnumber(state, (lua_Number)L);
	lua_pushnumber(state, (lua_Number)x);
	lua_pushnumber(state, (lua_Number)y);

	return 3;
}

int moMoldeoObject::luaGetTrackerBarycenter(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

   float x, y;
    x = y = 0.0;

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            x = pTracker->GetBarycenter().X();
            y = pTracker->GetBarycenter().Y();
        }
    }

	lua_pushnumber(state, (lua_Number)x);
	lua_pushnumber(state, (lua_Number)y);

	return 2;

}


int moMoldeoObject::luaGetTrackerAcceleration(moLuaVirtualMachine& vm) {


    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

   float x, y, L;
    x = y = L = 0.0;

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            L = pTracker->GetBarycenterAcceleration().Length();
            x = pTracker->GetBarycenterAcceleration().X();
            y = pTracker->GetBarycenterAcceleration().Y();
        }
    }

	lua_pushnumber(state, (lua_Number)L);
	lua_pushnumber(state, (lua_Number)x);
	lua_pushnumber(state, (lua_Number)y);

	return 3;
}

int moMoldeoObject::luaGetTrackerVelocity(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

   float x, y, L;
    x = y = L = 0.0;

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            L = pTracker->GetBarycenterMotion().Length();
            x = pTracker->GetBarycenterMotion().X();
            y = pTracker->GetBarycenterMotion().Y();
        }
    }

	lua_pushnumber(state, (lua_Number)L);
	lua_pushnumber(state, (lua_Number)x);
	lua_pushnumber(state, (lua_Number)y);
	return 3;
}


int moMoldeoObject::luaGetTrackerZone(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int izone = (int) lua_tonumber (state, 2);

    int nitems;
    nitems = 0;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            nitems = pTracker->GetPositionMatrix( pTracker->ZoneToPosition(izone) );
        }
    }

    lua_pushnumber( state, (lua_Number) nitems);
    return 1;
}

int moMoldeoObject::luaGetTrackerHistory(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    int nitems;
    nitems = 0;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            nitems = pTracker->GetHistory().CountRecords();
        }
    }

    lua_pushnumber( state, (lua_Number) nitems);
    return 1;

}

int moMoldeoObject::luaStartTrackerHistory(moLuaVirtualMachine& vm) {
     lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int maxtime_ms = (int)lua_tonumber( state, 2);
    int granularity_ms = (int)lua_tonumber( state, 3);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            pTracker->GetHistory().StartRecording(maxtime_ms,granularity_ms);
        }
    }
    return 0;
}

int moMoldeoObject::luaPauseTrackerHistory(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            pTracker->GetHistory().PauseRecording();
        }
    }
    return 0;
}


int moMoldeoObject::luaContinueTrackerHistory(moLuaVirtualMachine& vm) {

    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            pTracker->GetHistory().ContinueRecording();
        }
    }

    return 0;
}
int moMoldeoObject::luaStopTrackerHistory(moLuaVirtualMachine& vm) {
    lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker) {
            pTracker->GetHistory().StopRecording();
        }
    }

    return 0;

}
int moMoldeoObject::luaGetHistoryRecord(moLuaVirtualMachine& vm) {
     lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int recordindex = (int)lua_tonumber( state, 2);

    moTrackerInstanceRecord IRecord;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker && recordindex>=0 && recordindex<pTracker->GetHistory().CountRecords()) {
            IRecord = pTracker->GetHistory().Get( recordindex );
        } else {
          MODebug2->Error("luaGetHistoryRecord::out of bound recordindex:"+IntToStr(recordindex)+" histories:"+IntToStr( pTracker->GetHistory().CountRecords()) );
        }
    } else {
      MODebug2->Error("luaGetHistoryRecord:: no tracker index or inlet not updated");
    }

    lua_pushnumber( state, (lua_Number) IRecord.m_ValidFeatures);
    lua_pushnumber( state, (lua_Number) IRecord.m_DeltaValidFeatures);
    lua_pushnumber( state, (lua_Number) IRecord.m_nFeatures);
    lua_pushnumber( state, (lua_Number) IRecord.m_SurfaceCovered);
    lua_pushnumber( state, (lua_Number) IRecord.m_Tick);
    return 5;
}

int moMoldeoObject::luaGetHistoryBarycenter(moLuaVirtualMachine& vm) {
     lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int recordindex = (int)lua_tonumber( state, 2);

    moTrackerInstanceRecord IRecord;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker && recordindex>=0 && recordindex<pTracker->GetHistory().CountRecords()) {
            IRecord = pTracker->GetHistory().Get( recordindex );
        }
    }

    lua_pushnumber( state, (lua_Number) IRecord.m_Barycenter.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_Barycenter.Y());
    lua_pushnumber( state, (lua_Number) IRecord.m_BarycenterMotion.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_BarycenterMotion.Y());
    lua_pushnumber( state, (lua_Number) IRecord.m_BarycenterAcceleration.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_BarycenterAcceleration.Y());
    return 6;
}

///Devuelve las varianzas
int moMoldeoObject::luaGetHistoryVariance(moLuaVirtualMachine& vm) {
     lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int recordindex = (int)lua_tonumber( state, 2);

    moTrackerInstanceRecord IRecord;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker && recordindex>=0 && recordindex<pTracker->GetHistory().CountRecords()) {
            IRecord = pTracker->GetHistory().Get( recordindex );
        }
    }

    lua_pushnumber( state, (lua_Number) IRecord.m_Variance.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_Variance.Y());
    lua_pushnumber( state, (lua_Number) IRecord.m_SpeedVariance.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_SpeedVariance.Y());
    lua_pushnumber( state, (lua_Number) IRecord.m_AccelerationVariance.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_AccelerationVariance.Y());
    return 6;

}

///Devuelve los rectangulos
int moMoldeoObject::luaGetHistoryBounding(moLuaVirtualMachine& vm) {
     lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int recordindex = (int)lua_tonumber( state, 2);

    moTrackerInstanceRecord IRecord;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker && recordindex>=0 && recordindex<pTracker->GetHistory().CountRecords()) {
            IRecord = pTracker->GetHistory().Get( recordindex );
        }
    }

    lua_pushnumber( state, (lua_Number) IRecord.m_BoundingRectangle.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_BoundingRectangle.Y());
    lua_pushnumber( state, (lua_Number) IRecord.m_BoundingRectangle.Z());
    lua_pushnumber( state, (lua_Number) IRecord.m_BoundingRectangle.W());
    lua_pushnumber( state, (lua_Number) IRecord.m_BoundingRectangleAngle );
    lua_pushnumber( state, (lua_Number) IRecord.m_BlobCandidates );
    return 6;

}

///Devuelve los promedios
int moMoldeoObject::luaGetHistoryAverage(moLuaVirtualMachine& vm) {
     lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int recordindex = (int)lua_tonumber( state, 2);

    moTrackerInstanceRecord IRecord;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker && recordindex>=0 && recordindex<pTracker->GetHistory().CountRecords()) {
            IRecord = pTracker->GetHistory().Get( recordindex );
        }
    }

    lua_pushnumber( state, (lua_Number) IRecord.m_AbsoluteSpeedAverage);
    lua_pushnumber( state, (lua_Number) IRecord.m_AbsoluteAccelerationAverage);
    lua_pushnumber( state, (lua_Number) IRecord.m_AbsoluteTorqueAverage);
    return 3;

}

///Devuelve el rectangulo max, min.
int moMoldeoObject::luaGetHistoryMinMax(moLuaVirtualMachine& vm) {

     lua_State *state = (lua_State *) vm;

    moTrackerSystemData* pTracker = NULL;

    int trackerindex = (int)lua_tonumber( state, 1);
    int recordindex = (int)lua_tonumber( state, 2);

    moTrackerInstanceRecord IRecord;

    moInlet* pInlet = NULL;

    pInlet = m_Inlets[trackerindex];

    if (pInlet && pInlet->Updated()) {
        pTracker = (moTrackerSystemData*)pInlet->GetData()->Pointer();
        if (pTracker && recordindex>=0 && recordindex<pTracker->GetHistory().CountRecords()) {
            IRecord = pTracker->GetHistory().Get( recordindex );
        }
    }

    lua_pushnumber( state, (lua_Number) IRecord.m_Min.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_Min.Y());
    lua_pushnumber( state, (lua_Number) IRecord.m_Max.X());
    lua_pushnumber( state, (lua_Number) IRecord.m_Max.Y());
    return 4;

}

