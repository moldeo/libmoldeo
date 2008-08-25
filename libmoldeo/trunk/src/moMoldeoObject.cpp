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

#include "moMoldeoObject.h"
#include "moTextureFilterIndex.h"

#include "moArray.cpp"
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
	m_bLoading = false;
	m_pResourceManager = NULL;
}

moMoldeoObject::~moMoldeoObject() {

}

MOboolean
moMoldeoObject::Init() {

    int idx;

	if (m_pResourceManager!=NULL)
		m_vm = m_pResourceManager->GetScriptMan()->GetVM();

	//Inicializa las funciones matemáticas del config
	//así como las texturas
	for( MOint p=0;p<m_Config.GetParamsCount();p++) {

		moParam	&param( m_Config[p] );

		MODebug2->Log( moText("moMoldeoObject:: Init param type ") + param.GetParamDefinition().GetTypeStr() + moText(" name: ") + param.GetParamDefinition().GetName() );

		for( MOuint v=0;v<param.GetValuesCount();v++) {

			moValue& value( param.GetValue(v) );

			//MODebug2->Message( moText("+Init value #") + IntToStr(v) );

            //RESUELVE LAS FUNCIONES!!!!
            //esto debe hacerse antes de aplicar filtros y otros...

            for( MOuint ivb=0; ivb<value.GetSubValueCount(); ivb++) {
                moValueBase& VB( value.GetSubValue( ivb ) );
                if (VB.GetType() == MO_VALUE_FUNCTION ) {
                    idx = m_pResourceManager->GetMathMan()->AddFunction( VB.Text(), (MOboolean)false );
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

                switch(param.GetParamDefinition().GetType() ) {

                    case MO_PARAM_TEXTURE:
                    case MO_PARAM_VIDEO:
                    case MO_PARAM_FILTER:
                        MODebug2->Log( moText("") );
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
                        break;

                    case MO_PARAM_FONT:
                        moFont* pFont;
                        pFont = m_pResourceManager->GetFontMan()->AddFont( valuebase.Text(), MO_FONT_TRANSLUCENT );
                        if (pFont) {
                            valuebase.SetFont( pFont );
                        }
                        break;

                    case MO_PARAM_3DMODEL:
                    case MO_PARAM_OBJECT:
                        if (value.GetSubValueCount()>0) {
                            moValueBase& valuebase(value.GetSubValue(0));
                            mo3dModel* pModel = m_pResourceManager->GetModelMan()->Get3dModel( valuebase.Text() );
                            /*if (pModel)
                                valuebase.SetModel( pModel );*/
                        }
                        break;

                }//fin siwtch

			}//fin if
		}
	}

	//Levanta los Inlets y Outlets definidos en el config
	moParam& pinlets = m_Config[moText("inlet")];

	for( MOuint i=0; i<pinlets.GetValuesCount(); i++ ) {
		moInlet* Inlet = new moInlet();
		if (Inlet) {
			Inlet->SetMoldeoLabelName( GetLabelName() );
			//Buscamos el parametro asociado al inlet
			//para asociar un parametro a un inlet debe simplemente tener el mismo nombre...
			moText InletName = pinlets[i][MO_INLET_NAME].Text();
			if ( m_Config.GetParamIndex(InletName)>-1 ) {
				Inlet->Init( InletName, i, pinlets[i][MO_INLET_TYPE].Text());
				//Generamos el dato que sobreescribirá... (en caso de ser Updateado)
				m_Config.GetParam(InletName).SetExternData( Inlet->GetData() );
			} else {
				Inlet->Init( InletName, i, pinlets[i][MO_INLET_TYPE].Text() );
			}
			m_Inlets.Add( Inlet );
		}
	}


	moParam& poutlets = m_Config[moText("outlet")];

	for( MOuint i=0; i<poutlets.GetValuesCount(); i++ ) {
		moOutlet* Outlet = new moOutlet();
		if (Outlet) {
			Outlet->SetMoldeoLabelName( GetLabelName() );
			//Buscamos el parametro asociado al outlet
			//para asociar un parametro a un outlet debe simplemente tener el mismo nombre...
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

void
moMoldeoObject::Load() {
	if (m_bLoading) {
		//something
	}
	return;
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

MOboolean
moMoldeoObject::LoadConnectors() {

	//check for inlet and outlet on config
	moValues inletvalues = m_Config[ moText("inlet") ].GetValues();
	moValues outletvalues = m_Config[ moText("outlet") ].GetValues();

	//generates inlets and outlets
	int ConnectorId = 0;

	MOuint i,j;

	for( i = 0; i < inletvalues.Count(); i++ ) {
		moText inletname = inletvalues[i].GetSubValue(MO_INLET_NAME).Text();
		moDataType inlettype = (moDataType)inletvalues[i].GetSubValue(MO_INLET_TYPE).Int();
		moInlet* pInlet = new moInlet();
		pInlet->Init( inletname, ConnectorId++, inlettype );
		m_Inlets.Add( pInlet );
	}

	//for outlets:
	//get the ids of each one connected to outlets
	for( i = 0; i < outletvalues.Count(); i++ ) {
		moText outletname = outletvalues[i].GetSubValue(MO_OUTLET_NAME).Text();

		moText datatypetxt = outletvalues[i].GetSubValue(MO_OUTLET_TYPE).Text();

        moData dummy;

		moDataType outlettype = (moDataType) dummy.TextToType( datatypetxt );

		moOutlet* pOutlet = new moOutlet();
		pOutlet->Init( outletname, ConnectorId++, outlettype );

		for( j = 2; j < outletvalues[i].GetSubValueCount(); j+=2 ) {

			//retrieve Moldeo Object Labelname
			moText MoldeoObjectName = outletvalues[i].GetSubValue(j).Text();
			//retrieve Connector Labelname
			moText ConnectorLabelName = outletvalues[i].GetSubValue(j+1).Text();

			moConnection* pconnection = new moConnection( MoldeoObjectName, ConnectorLabelName );
			pOutlet->GetConnections()->Add( pconnection );

		}
		m_Outlets.Add( pOutlet );
	}

	return true;
}

void
moMoldeoObject::UnloadConnectors() {

	MOuint i;

	for( i = 0; i < m_Inlets.Count(); i++ ) {
		m_Inlets[i]->Finish();
		delete m_Inlets[i];
	}
	m_Inlets.Empty();

	for( i = 0; i < m_Outlets.Count(); i++ ) {
		m_Outlets[i]->Finish();
		delete m_Outlets[i];
	}
	m_Outlets.Empty();
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
