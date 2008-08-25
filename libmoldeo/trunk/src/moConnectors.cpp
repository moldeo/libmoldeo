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
  Andrés Colubri

*******************************************************************************/

#include "moConnectors.h"

#include "moArray.cpp"
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
moConnector::Init( moText p_ConnectorLabelName, MOint ConnectorId, moText t_datatype ) {
	m_ConnectorLabelName = p_ConnectorLabelName;
	m_ConnectorId = ConnectorId;
/*	moParamType p;

enum moParamType {
	MO_PARAM_ALPHA,			//value type: NUM or FUNCTION
	MO_PARAM_COLOR,			//value type: NUM[4] or FUNCTION[4] or
	MO_PARAM_SYNC,			//value type: NUM or FUNCTION
	MO_PARAM_TEXT,			//value type: TXT or LNK
	MO_PARAM_TEXTURE,		//value type: TXT or LNK
	MO_PARAM_VIDEO,			//value type: TXT or LNK
	MO_PARAM_SOUND,			//value type: TXT or LNK
	MO_PARAM_FUNCTION,		//value type: NUM or FUNCTION
	MO_PARAM_SCRIPT,		//value type: TXT or LNK
	MO_PARAM_INLET,			//value type: TXT or LNK
	MO_PARAM_OUTLET			//value type: TXT or LNK
};

	moDataType d;

enum moDataType {
	MO_DATA_UNDEFINED,
	MO_DATA_NUMBER,
	MO_DATA_NUMBER_CHAR,
	MO_DATA_NUMBER_INT,
	MO_DATA_NUMBER_LONG,
	MO_DATA_NUMBER_DOUBLE,
	MO_DATA_NUMBER_FLOAT,
	MO_DATA_POINTER,//may be a pointer to struct or to class
	MO_DATA_VECTOR,//array of values
	MO_DATA_IMAGESAMPLE,//pointer to an imagesample pointer
	MO_DATA_SOUNDSAMPLE,//pointer to a soundsample pointer
	MO_DATA_VIDEOSAMPLE,//pointer to a videosample pointer
	MO_DATA_TEXT//text
};

	moValueType v;
just to interpret Config info (text based definition)...
enum moValueType {
	MO_VALUE_NUM,//cualquier número
	MO_VALUE_NUM_CHAR,//un byte
	MO_VALUE_NUM_INT,//entero 32b
	MO_VALUE_NUM_LONG,//entero largo 64b
	MO_VALUE_NUM_FLOAT,//coma flotante 32b
	MO_VALUE_NUM_DOUBLE,//coma flotante precision doble
	MO_VALUE_MATRIX,//any type of VECTOR
	MO_VALUE_TXT,//any type of text, single or multiline
	MO_VALUE_LNK,//link to a file, text is interpreted as relative, absolute link to a file
	MO_VALUE_FUNCTION,//function parameter value, with lots of attributes....
	MO_VALUE_XML//text, xml formatted...
};
*/
    moData dummy;
    moDataType ttype;

    ttype = dummy.TextToType( t_datatype );

	if (t_datatype==moText("NUMBER")) {
		m_pData = new moData( MO_DATA_NUMBER );
	} else if(t_datatype==moText("POINTER")) {
		m_pData = new moData( MO_DATA_POINTER );
	} else if(t_datatype==moText("DATA")) {
		m_pData = new moData( MO_DATA_POINTER );
	} else if(t_datatype==moText("VECTOR")) {
		m_pData = new moData( MO_DATA_VECTOR );
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
	m_pData = NULL;
	m_pParam = p_param;
	return true;
}

MOint
moConnector::GetMoldeoId() {
	return m_MoldeoId;
}

void
moConnector::SetMoldeoId( MOint p_moldeoid ) {

	m_MoldeoId = p_moldeoid;

}

moText
moConnector::GetMoldeoLabelName() {
	return m_MoldeoLabelName;
}

void
moConnector::SetMoldeoLabelName( moText p_MoldeoLabelName ) {
	m_MoldeoLabelName = p_MoldeoLabelName;
}

MOint
moConnector::GetConnectorId() {
	return m_ConnectorId;
}

moText
moConnector::GetConnectorLabelName() {
	return m_ConnectorLabelName;
}



void
moConnector::SetType( moDataType p_datatype ) {
	if (m_pData) m_pData->SetType( p_datatype );
	else m_pData = new moData( p_datatype );
}

moDataType
moConnector::GetType() {
	if (m_pData) return m_pData->Type();
	else return MO_DATA_UNDEFINED;
}

void
moConnector::SetData( moData*	p_data ) {
	m_pData = p_data;
}

moData*
moConnector::GetData() {
	if (m_pData)
		return m_pData;
	if (m_pParam)
		m_pParam->GetData();
	return NULL;
}

void
moConnector::NewData() {
	if (m_pData)
		delete m_pData;
	else
		m_pData = new moData();
}


void
moConnector::AddData( const moData& data ) {

    moDataMessage message;

    if ( m_DataMessages.Count() == 0 ) {
        message.Empty();
        message.Add( data );
    } else {
        moDataMessage& LastMessage( m_DataMessages.Get( m_DataMessages.Count() - 1 ) );
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
moConnector::Updated() {
	return m_bUpdated;
}

void
moConnector::Update( bool force ) {
	m_bUpdated = force;
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

