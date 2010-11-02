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

#include <moArray.h>
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

	m_pResourceManager = NULL;
	m_Inlets.Init( 0 , NULL );
	m_Outlets.Init( 0 , NULL );
}

moMoldeoObject::~moMoldeoObject() {

}


MOboolean
moMoldeoObject::Init() {

    int idx;

    LoadDefinition();

	if (m_pResourceManager!=NULL)
		m_vm = m_pResourceManager->GetScriptMan()->GetVM();

	///Inicializa las funciones matemáticas del config
	///así como los inlets y outlets por cada parámetro
	///así como las texturas
	for( MOint p=0;p<m_Config.GetParamsCount();p++) {

		moParam	&param( m_Config[p] );

		MODebug2->Log( moText("moMoldeoObject:: Init param type ") + param.GetParamDefinition().GetTypeStr() + moText(" name: ") + param.GetParamDefinition().GetName() );


        ///CREAMOS UN INLET POR CADA PARAMETRO
        moInlet* Inlet = new moInlet();
        if (Inlet) {
            Inlet->Init( param.GetParamDefinition().GetName(), m_Inlets.Count(), param.GetPtr() );
            param.SetExternData( Inlet->GetData() );
            m_Inlets.Add(Inlet);
        }

		for( MOuint v=0;v<param.GetValuesCount();v++) {

			moValue& value( param.GetValue(v) );

			//MODebug2->Message( moText("+Init value #") + IntToStr(v) );

            ///RESUELVE LAS FUNCIONES!!!!
            ///esto debe hacerse antes de aplicar filtros y otros...

            for( MOuint ivb=0; ivb<value.GetSubValueCount(); ivb++) {
                moValueBase& VB( value.GetSubValue( ivb ) );
                if (VB.GetType() == MO_VALUE_FUNCTION ) {
                    idx = m_pResourceManager->GetMathMan()->AddFunction( VB.Text(), (MOboolean)true, GetConfig() );
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
				Outlet->Init( OutletName, i, m_Config.GetParam(OutletName).GetPtr());
			} else {
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

	return true;
}

MOboolean
moMoldeoObject::Init( moResourceManager* p_pResources ) {
	m_pResourceManager = p_pResources;
	if (m_pResourceManager!=NULL)
		m_vm = m_pResourceManager->GetScriptMan()->GetVM();

	moMoldeoObject::Init();

	return (m_pResourceManager == NULL);
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

	p_configdefinition->Add( moText("inlet"), MO_PARAM_INLET );
	p_configdefinition->Add( moText("outlet"), MO_PARAM_OUTLET );
	return p_configdefinition;
}

void
moMoldeoObject::LoadDefinition() {

	GetDefinition();

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

	//Procesamos los eventos recibidos de los MoldeoObject Outlets
	while(actual!=NULL) {
		tmp = actual->next;
		//procesamos aquellos Outlet q estan dirigidos a este objeto

		if (actual->deviceid == GetId() && actual->reservedvalue3 == MO_MESSAGE) {

			//pSample = (moVideoSample*)actual->pointer;
			pmessage = (moMessage*)actual;

			//process message:
			MOint inletid = pmessage->m_InletIdDest;
			moData pdata = pmessage->m_Data;

			//buscar el inlet...
			if (inletid>=0 && inletid<(int)m_Inlets.Count() ) {
				moInlet* pinlet = m_Inlets[inletid];
				if (pinlet->GetData()==NULL) pinlet->NewData();
				pinlet->GetData()->Copy(pdata);
				pinlet->Update();//notifica al inlet que ya esta actualizado...
			}

		} else if (actual->reservedvalue3 == MO_MESSAGE) {
		    //Broadcasting: borra su propio mensaje....

			pmessage = (moMessage*)actual;

			//se fija si es un mensaje generado por este objeto
			if (pmessage->m_MoldeoIdSrc == GetId() ) {
				p_EventList->Delete(pmessage);
			}

		}
		//pasamos al siguiente
		actual = tmp;
	}


	//generamos los mensajes emergentes de los Outlets
	for( MOuint i=0; i<m_Outlets.Count() ; i++) {
		moOutlet* poutlet = m_Outlets[i];

        if (poutlet)
		if (poutlet->Updated()) {
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

}


///beware ! call only once or die!!!
void moMoldeoObject::RegisterFunctions()
{
    RegisterBaseFunction("PushDebugString"); //0

    ///functions to access moConfig data
	RegisterFunction("SetPreconf");//1
	RegisterFunction("GetPreconf");//2

	RegisterFunction("GetParamIndex");//3
	RegisterFunction("GetCurrentValue");//4
	RegisterFunction("SetCurrentValue");//5

	RegisterFunction("GetInletIndex");//6
	RegisterFunction("GetInletData");//7
	RegisterFunction("SetInletData");//8

    ///for TUIO tracking
    RegisterFunction("GetTuioSystem");//9
    RegisterFunction("GetTuioCursorCount");//10
    RegisterFunction("GetTuioCursor");//11
    RegisterFunction("GetTuioObjectCount");//12
    RegisterFunction("GetTuioObject");//13

    ///for features tracking

    ///all values nvalids, nfeatures, barycenter, acceleration, velocity
	RegisterFunction("GetTrackerSystemData"); //14
	RegisterFunction("GetTrackerFeaturesCount"); //15
	RegisterFunction("GetTrackerFeature"); //16
	RegisterFunction("GetTrackerVariance"); //17
	RegisterFunction("GetTrackerBarycenter"); //18
	RegisterFunction("GetTrackerAcceleration"); //19
	RegisterFunction("GetTrackerVelocity"); //20
	RegisterFunction("GetTrackerZone"); //21

}

int moMoldeoObject::ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber) {

 switch ( iFunctionNumber - m_iMethodBase )
    {
        case 0:
            ResetScriptCalling();
            return luaPushDebugString(vm);//0

        ///functions to access moConfig data
        case 1:
            ResetScriptCalling();
            return luaSetPreconf(vm);//1
        case 2:
            ResetScriptCalling();
            return luaGetPreconf(vm);//2


        case 3:
            ResetScriptCalling();
            return luaGetParamIndex(vm);//3
        case 4:
            ResetScriptCalling();
            return luaGetCurrentValue(vm);//4
        case 5:
            ResetScriptCalling();
            return luaSetCurrentValue(vm);//5



        case 6:
            ResetScriptCalling();
            return luaGetInletIndex(vm);//6
        case 7:
            ResetScriptCalling();
            return luaGetInletData(vm);//7
        case 8:
            ResetScriptCalling();
            return luaSetInletData(vm);//8


        ///functions to access Inlets Data
        ///TUIO
        ///Tracker, etc...
        case 9:
            ResetScriptCalling();
            return luaGetTuioSystem(vm);//9
        case 10:
            ResetScriptCalling();
            return  luaGetTuioCursorCount(vm);//10
        case 11:
            ResetScriptCalling();
            return  luaGetTuioCursor(vm);//11
        case 12:
            ResetScriptCalling();
            return  luaGetTuioObjectCount(vm);//12
        case 13:
            ResetScriptCalling();
            return  luaGetTuioObject(vm);//13
        case 14:
            ResetScriptCalling();
            return luaGetTrackerSystemData(vm);//14
        case 15:
            ResetScriptCalling();
            return luaGetTrackerFeaturesCount(vm);//15
        case 16:
            ResetScriptCalling();
            return luaGetTrackerFeature(vm);//16
        case 17:
            ResetScriptCalling();
            return luaGetTrackerVariance(vm);//17
        case 18:
            ResetScriptCalling();
            return luaGetTrackerBarycenter(vm);//18
        case 19:
            ResetScriptCalling();
            return luaGetTrackerAcceleration(vm);//19
        case 20:
            ResetScriptCalling();
            return luaGetTrackerVelocity(vm);//20
        case 21:
            ResetScriptCalling();
            return luaGetTrackerZone(vm);//21

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
                        MO_DATA_NUMBER_FLOAT:
                        lua_pushnumber( state, (lua_Number) pData->Double() );
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
        return 1;
    }

    lua_pushnumber( state, (lua_Number) nitems);
    return 1;
}



