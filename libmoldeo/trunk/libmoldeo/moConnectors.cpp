/*******************************************************************************

                              moConnectors.cpp

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

#include "moConnectors.h"

#include "moArray.h"
moDefineDynamicArray(moConnectors)
moDefineDynamicArray(moConnections)
moDefineDynamicArray(moInlets)
moDefineDynamicArray(moOutlets)

//===========================================
//
//				moConnectors
//
//===========================================

moConnector::moConnector() {
	m_ConnectorLabelName = moText("");
	m_ConnectorId = -1;
	m_bUpdated = false;
	m_pData = NULL;
	m_pParam = NULL;
}
moConnector::~moConnector() {
}

MOboolean moConnector::Init() {
	return true;
}

MOboolean moConnector::Finish() {
	return true;
}


MOboolean
moConnector::Init( moText p_ConnectorLabelName, MOint ConnectorId, moDataType p_datatype ) {
	m_ConnectorLabelName = p_ConnectorLabelName;
	m_ConnectorId = ConnectorId;
	m_pData = new moData( p_datatype );
	m_pParam = NULL;
	return true;
}

MOboolean
moConnector::Init( moText p_ConnectorLabelName, MOint ConnectorId, moParamType p_paramtype ) {
	m_ConnectorLabelName = p_ConnectorLabelName;
	m_ConnectorId = ConnectorId;

    ///En este caso se crea un tipo de dato compatible con el dato que se quiere enviar
    ///tomando de base el tipo de parámetro ya conocemos el dato que recibiremos
    ///esto varia ligeramente del dato original del parametro del config, ya que debería estar resuelto ya
    ///o sea, en caso de una funcion , se recibirá el resultado de esta
    ///en el caso de una textura, el puntero a ella
    ///en el caso de un texto será un texto
    ///de cualquier tipo de coordenada será un real
    ///de un color será un vector de 4 valores (float por precision)
    switch( p_paramtype ) {
        case MO_PARAM_ALPHA:
        case MO_PARAM_PHASE:
        case MO_PARAM_SYNC:
        case MO_PARAM_TRANSLATEX:
        case MO_PARAM_TRANSLATEY:
        case MO_PARAM_TRANSLATEZ:
        case MO_PARAM_SCALEX:
        case MO_PARAM_SCALEY:
        case MO_PARAM_SCALEZ:
        case MO_PARAM_ROTATEX:
        case MO_PARAM_ROTATEY:
        case MO_PARAM_ROTATEZ:
        case MO_PARAM_FUNCTION:
            m_pData = new moData( MO_DATA_NUMBER_FLOAT );
            break;

        case MO_PARAM_NUMERIC:
        case MO_PARAM_BLENDING:
        case MO_PARAM_POLYGONMODE:
            m_pData = new moData( MO_DATA_NUMBER_LONG );
            break;

        case MO_PARAM_COLOR:
            m_pData = new moData( MO_DATA_VECTOR4F );
            break;

        case MO_PARAM_FONT:
            m_pData = new moData( MO_DATA_FONTPOINTER );
            break;

        case MO_PARAM_TEXT:
        case MO_PARAM_SCRIPT:
            m_pData = new moData( MO_DATA_TEXT );
            break;

        case MO_PARAM_OBJECT:
        case MO_PARAM_3DMODEL:
            m_pData = new moData( MO_DATA_3DMODELPOINTER );
            break;

        case MO_PARAM_TEXTURE:
        case MO_PARAM_TEXTUREFOLDER:
        case MO_PARAM_VIDEO:
            m_pData = new moData( MO_DATA_VIDEOSAMPLE );
            break;

        case MO_PARAM_COMPOSE:
            m_pData = new moData( MO_DATA_MESSAGE );
            break;

        case MO_PARAM_SOUND:
            m_pData = new moData( MO_DATA_SOUNDSAMPLE );
            break;

        case MO_PARAM_UNDEFINED:
            m_pData = new moData( MO_DATA_POINTER );
            break;

        default:
            m_pData = new moData( MO_DATA_NUMBER );
            break;
    }

	m_pParam = NULL;
	return true;
}

MOboolean
moConnector::Init( moText p_ConnectorLabelName, MOint ConnectorId, moText t_datatype ) {
	m_ConnectorLabelName = p_ConnectorLabelName;
	m_ConnectorId = ConnectorId;

    moData dummy;
    moDataType ttype;

    ttype = dummy.TextToType( t_datatype );

	if (t_datatype==moText("NUMBER")) {
		m_pData = new moData( MO_DATA_NUMBER );
	} else if(t_datatype==moText("POINTER")) {
		m_pData = new moData( MO_DATA_POINTER );
	} else if(t_datatype==moText("DATA")) {
		m_pData = new moData( MO_DATA_POINTER );
	} else if(t_datatype==moText("VECTOR2F")) {
		m_pData = new moData( MO_DATA_VECTOR2F );
	} else if(t_datatype==moText("VECTOR3F")) {
		m_pData = new moData( MO_DATA_VECTOR3F );
	} else if(t_datatype==moText("VECTOR4F")) {
		m_pData = new moData( MO_DATA_VECTOR4F );
	} else if(t_datatype==moText("VECTOR2I")) {
		m_pData = new moData( MO_DATA_VECTOR2I );
	} else if(t_datatype==moText("VECTOR3I")) {
		m_pData = new moData( MO_DATA_VECTOR3I );
	} else if(t_datatype==moText("VECTOR4I")) {
		m_pData = new moData( MO_DATA_VECTOR4I );
	} else if(t_datatype==moText("IMAGE")) {
		m_pData = new moData( MO_DATA_IMAGESAMPLE );
	} else if(t_datatype==moText("SOUND")) {
		m_pData = new moData( MO_DATA_SOUNDSAMPLE );
	} else if(t_datatype==moText("VIDEO")) {
		m_pData = new moData( MO_DATA_VIDEOSAMPLE );
	} else if(t_datatype==moText("TEXT")) {
		m_pData = new moData( MO_DATA_TEXT );
	} else {
		m_pData = new moData( ttype );
	}
	m_pParam = NULL;
	return true;
}


MOboolean
moConnector::Init( moText p_ConnectorLabelName, MOint ConnectorId, moData* p_data ) {
	m_ConnectorLabelName = p_ConnectorLabelName;
	m_ConnectorId = ConnectorId;
	m_pData = p_data;
	m_pParam = NULL;
	return true;
}

MOboolean
moConnector::Init( moText p_ConnectorLabelName, MOint ConnectorId, moParam* p_param ) {
	m_ConnectorLabelName = p_ConnectorLabelName;
	m_ConnectorId = ConnectorId;
	//m_pData = NULL;
	///Fija un dato interno para manejar este valor....
	moConnector::Init( p_ConnectorLabelName, ConnectorId, p_param->GetParamDefinition().GetType());

	///y asociamos a su vez el parametro del config
	m_pParam = p_param;

	///fix numeric!!! and value!!!
	if (m_pData)
	if ( p_param->GetParamDefinition().GetType() == MO_PARAM_NUMERIC ) {
	    moValueBase& firstvalue( p_param->GetValue().GetSubValue() );
	    switch(firstvalue.GetType()) {
	        case MO_DATA_NUMBER_DOUBLE:
                m_pData->SetDouble( firstvalue.Double() );
                break;
	        case MO_DATA_NUMBER_FLOAT:
                m_pData->SetFloat( firstvalue.Float() );
                break;
	        case MO_DATA_NUMBER_CHAR:
                m_pData->SetChar( firstvalue.Char() );
                break;
	        case MO_DATA_NUMBER_INT:
                m_pData->SetInt( firstvalue.Int() );
                break;
	        case MO_DATA_NUMBER_LONG:
	        case MO_DATA_NUMBER:
                m_pData->SetLong( firstvalue.Long() );
                break;
        }

    }

	return true;
}

MOint
moConnector::GetMoldeoId() const {
	return m_MoldeoId;
}

void
moConnector::SetMoldeoId( MOint p_moldeoid ) {

	m_MoldeoId = p_moldeoid;

}

moText
moConnector::GetMoldeoLabelName() const {
	return m_MoldeoLabelName;
}

void
moConnector::SetMoldeoLabelName( moText p_MoldeoLabelName ) {
	m_MoldeoLabelName = p_MoldeoLabelName;
}

MOint
moConnector::GetConnectorId() const {
	return m_ConnectorId;
}

const moText&
moConnector::GetConnectorLabelName() const {
	return m_ConnectorLabelName;
}



void
moConnector::SetType( moDataType p_datatype ) {
	if (m_pData) m_pData->SetType( p_datatype );
	else m_pData = new moData( p_datatype );
}

moDataType
moConnector::GetType() const {
	if (m_pData) return m_pData->Type();
	else return MO_DATA_UNDEFINED;
}

void
moConnector::SetData( moData*	p_data ) {
	m_pData = p_data;
}

moData*
moConnector::GetData() {

	if (m_pParam) {
	    /** Si es un inlet asociado a un parametro devuelve el m_pData interno que fue asociado por el SetExternalData*/
		return m_pParam->GetData();
	}

    /** Unicamente para inlets o outlets independientes...*/
	if (m_pData) {
		return m_pData;
	}

  return NULL;
}

moData*
moConnector::GetInternalData() {

    /** Unicamente para inlets o outlets independientes...*/
	if (m_pData) {
		return m_pData;
	}

  return NULL;
}


void
moConnector::NewData() {
	if (m_pData) {
		delete m_pData;
		m_pData = NULL;
	}
    m_pData = new moData();

}


void
moConnector::AddData( const moData& data ) {

    moDataMessage message;

    if ( m_DataMessages.Count() == 0 ) {
        message.Empty();
        message.Add( data );
        AddMessage(message);
    } else {
        moDataMessage& LastMessage( m_DataMessages[ m_DataMessages.Count() - 1] );
        LastMessage.Add( data );
    }

}

void
moConnector::AddMessage( const moDataMessage& datamessage ) {

    m_DataMessages.Add( datamessage );

}

moDataMessages&
moConnector::GetMessages() {
    return m_DataMessages;
}

MOboolean
moConnector::Updated()  const {
	return m_bUpdated;
}

void
moConnector::Update( bool force ) {
	m_bUpdated = force;

	///also update the associated parameter
	if (m_pParam)
        m_pParam->Update();
}
//===========================================
//
//				moConnection
//
//===========================================

moConnection::moConnection() {
	m_DestinationConnectorId = -1;
	m_DestinationConnectorLabelName = moText("");
	m_DestinationMoldeoId = -1;
	m_DestinationMoldeoLabelName = moText("");
}

moConnection::moConnection( moText p_DestinationMoldeoLabelName,  moText p_DestinationConnectorLabelName, MOint p_DestinationMoldeoId, MOint p_DestinationConnectorId ) {
	m_DestinationMoldeoId = p_DestinationMoldeoId;
	m_DestinationMoldeoLabelName = p_DestinationMoldeoLabelName;
	m_DestinationConnectorId = p_DestinationConnectorId;
	m_DestinationConnectorLabelName = p_DestinationConnectorLabelName;
}

moConnection::~moConnection() {
}

MOint
moConnection::GetDestinationMoldeoId() {
	return m_DestinationMoldeoId;
}
moText
moConnection::GetDestinationMoldeoLabelName() {
	return m_DestinationMoldeoLabelName;
}
MOint
moConnection::GetDestinationConnectorId() {
	return m_DestinationConnectorId;
}
moText
moConnection::GetDestinationConnectorLabelName() {
	return m_DestinationConnectorLabelName;
}


void
moConnection::SetDestinationMoldeoId( MOint p_DestinationMoldeoId ) {
	m_DestinationMoldeoId = p_DestinationMoldeoId;
}
void
moConnection::SetDestinationMoldeoLabelName( moText p_DestinationMoldeoLabelName ) {
	m_DestinationMoldeoLabelName = p_DestinationMoldeoLabelName;
}
void
moConnection::SetDestinationConnectorId( MOint p_DestinationConnectorId ) {
	m_DestinationConnectorId = p_DestinationConnectorId;
}
void
moConnection::SetDestinationConnectorLabelName( moText p_DestinationConnectorLabelName ) {
	m_DestinationConnectorLabelName = p_DestinationConnectorLabelName;
}


//===========================================
//
//				moInlet
//
//===========================================

moInlet::moInlet() {
}
moInlet::~moInlet() {
}

MOboolean
moInlet::Init( moText p_ConnectorLabelName, MOint ConnectorId, moParam* p_param ) {

    MOboolean res = moConnector::Init( p_ConnectorLabelName, ConnectorId, p_param );

    /** Se fija al parámetro el m_pData como Dato Externo del Parámetro,

     Al llamar al moParam::GetData se evalua asi:

    a) DATO DINAMICO: el dato interno del inlet (m_pData) segun si fue Actualizado (m_bUpdated)
    b) DATO ESTATICO: el valor del valor de la configuracion

    */
    if (m_pParam) {
        m_pParam->SetExternData( m_pData );
    }
    return res;
}


//===========================================
//
//				moOutlet
//
//===========================================


moOutlet::moOutlet() {

}
moOutlet::~moOutlet() {
	m_Connections.Empty();
}

bool moOutlet::Connect( moInlet* p_connector ) {
	for( MOuint i = 0; i < m_Connections.Count(); i++ ) {
		if ( m_Connections[i]->GetDestinationMoldeoId() == p_connector->GetMoldeoId()
			&& m_Connections[i]->GetDestinationConnectorId() == p_connector->GetConnectorId())
		return false;
	}
	m_Connections.Add( new moConnection( p_connector->GetMoldeoLabelName(), p_connector->GetConnectorLabelName(), p_connector->GetMoldeoId(), p_connector->GetConnectorId() ) );
	return true;
}

void moOutlet::Disconnect( moInlet* p_connector ) {
	for( MOuint i = 0; i < m_Connections.Count(); i++ ) {
		if ( m_Connections[i]->GetDestinationMoldeoId() == p_connector->GetMoldeoId()
			&& m_Connections[i]->GetDestinationConnectorId() == p_connector->GetConnectorId())
			m_Connections.Remove(i);
		break;
	}
}

MOboolean
moOutlet::Finish() {
	for(MOuint i=0; i<m_Connections.Count(); i++) {
		if (m_Connections[i])
			delete m_Connections[i];
	}
	m_Connections.Empty();
	return true;
}

moConnections*	moOutlet::GetConnections() {
	return &m_Connections;
}
