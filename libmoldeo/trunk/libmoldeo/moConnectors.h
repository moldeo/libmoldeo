/*******************************************************************************

								moConnectors.h

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
  Andrs Colubri

*******************************************************************************/

#ifndef __MO_CONNECTORS_H__
#define __MO_CONNECTORS_H__

#include "moTypes.h"
#include "moText.h"
#include "moAbstract.h"
#include "moEventList.h"
#include "moConfig.h"
#include "moArray.h"

/// Conector para vincular objetos con datos
/**
 *  Objeto base de outlet e inlets para manejar la transmisin de datos entre moMoldeoObject 's
 *
 * @see moConnection
 * @see moOutlet
 * @see moInlet
 */
class LIBMOLDEO_API moConnector:public moAbstract
{
	public:
		/**
		 * Constructor
		 */
		moConnector ();

		/**
		 * Desctructor
		 */
		virtual ~ moConnector ();

		/**
		 * Inicializador
		 */
		virtual MOboolean Init ();

		/**
		 * Finalizador
		 */
		virtual MOboolean Finish ();

		/**
		 * Inicializador
		 * @param p_ConnectorLabelName nombre nico del objeto al que pertenece este conector
		 * @param ConnectorId id nico del objeto al que pertenece este conector
		 * @param p_datatype tipo de dato manejado por este conector
		 */
		virtual MOboolean Init (moText p_ConnectorLabelName, MOint ConnectorId,
			moDataType p_datatype);

		/**
		 * Inicializador
		 * @param p_ConnectorLabelName nombre nico del objeto al que pertenece este conector
		 * @param ConnectorId id nico del objeto al que pertenece este conector
		 * @param t_datatype tipo de dato manejado por este conector en formato texto
		 */
		virtual MOboolean Init (moText p_ConnectorLabelName, MOint ConnectorId,
			moText t_datatype);

		/**
		 * Inicializador
		 * @param p_ConnectorLabelName nombre nico del objeto al que pertenece este conector
		 * @param ConnectorId id nico del objeto al que pertenece este conector
		 * @param p_data puntero al dato al que estar vinculado este conector
		 */
		virtual MOboolean Init (moText p_ConnectorLabelName, MOint ConnectorId,
			moData * p_data);

		/**
		 * Inicializador
		 * @param p_ConnectorLabelName nombre nico del objeto al que pertenece este conector
		 * @param ConnectorId id nico del objeto al que pertenece este conector
		 * @param p_param parmetro al que estar vinculado este conector
		 */
		virtual MOboolean Init (moText p_ConnectorLabelName, MOint ConnectorId,
			moParam * p_param);

		/**
		 * Inicializador
		 * esta funcin de inicializacin es utilizada en todos los moMoldeoObjects
		 * sincretiza un tipo de parmetro moParamType, orientado a un archivo de configuracin ( simblico a interpretar [frmula] ) a
		 * un tipo de dato moDataType ya resuelto ( directo interpretado [dato] )
		 * @param p_ConnectorLabelName nombre nico del objeto al que pertenece este conector
		 * @param ConnectorId id nico del objeto al que pertenece este conector
		 * @param p_paramtype tipo de parmetro al que estar vinculado este conector, en este caso cada tipo de parametro tendra un moData asociado especfico (ver codigo)
		 */
		virtual MOboolean Init (moText p_ConnectorLabelName, MOint ConnectorId,
			moParamType p_paramtype);

		/**
		 * observador de la clase, devuelve el id del conector
		 * @return el id del conector
		 */
		MOint GetConnectorId ();

		/**
		 * observador de la clase, devuelve el nombre nico del objeto al que pertenece el conector
		 * @return el nombre nico del objeto al que pertenece el conector
		 */
		moText GetMoldeoLabelName ();

		/**
		 * fija el nombre nico del objeto al que pertenece el conector
		 * @param p_MoldeoLabelName el nombre nico del objeto al que pertenece el conector
		 */
		void SetMoldeoLabelName (moText p_MoldeoLabelName);

		/**
		 * observador de la clase, devuelve el id del objeto al que pertenece este conector
		 * @return el id del objeto al que pertenece este conector
		 */
		MOint GetMoldeoId ();

		/**
		 * fija el id nico del objeto al que pertenece este conector
		 * @param p_moldeoid el id del objeto al que pertenece este conector
		 */
		void SetMoldeoId (MOint p_moldeoid);

		/**
		 * observador de la clase, devuelve el nombre nico de este conector
		 * @return el nombre nico de este conector
		 */
		moText GetConnectorLabelName ();

		/**
		 * fija el tipo de dato vinculado a este conector
		 * @param p_datatype el tipo de dato vinculado a este conector
		 */
		void SetType (moDataType p_datatype);

		/**
		 * observador de la clase, devuelve el tipo de dato del conector
		 * @return el tipo de dato del conector
		 */
		moDataType GetType ();

		/**
		 * fija el parmetro al que estar vinculado este conector
		 * @param p_data el parmetro al que estar vinculado este conector
		 */
		void SetData (moData * p_data);

		/**
		 * observador de la clase, devuelve el puntero al parmetro al que est vinculado este conector
		 * @return el puntero al parmetro al que est vinculado este conector
		 */
		moData * GetData ();

		/**
		 * crea un objeto nuevo de datos
		 */
		void NewData ();

		/**
		 *   construye el mensaje para ser almacenado, esto es necesario para aquellos conectores que
		 *   usen streaming de datos...sin estructura fija....
		 */
		void AddData (const moData & data);

		/**
		 *   agrega este mensaje al buffer de mensajes del outlet
		 *   esta funcionalidad es util para streaming de datos estructurados por paquetes...(OSC y otros)
		 */
		void AddMessage (const moDataMessage & datamessage);

		/**
		 * observador de la clase, devuelve la referencia al buffer de mensajes
		 * @return la referencia la buffer de mensajes
		 */
		moDataMessages & GetMessages ();

		/**
		 * observador sobre la actualizacin del conector
		 * @return devuelve true si fue actualizado recientemente, false si no
		 */
		MOboolean Updated ();

		/**
		 * marca este dato como actualizado
		 */
		void Update (bool force = true);
	protected:
		//Type and Data, it may be a pointer to the plugins data...:-D
		/**
		 * actual data, for realtime........(last one)
		 */
		moData * m_pData;

		/**
		 * historical data, for streaming....
		 */
								 //historical data for streaming
		moDataMessages m_DataMessages;
		moParam * m_pParam;
		MOint m_MoldeoId;
		moText m_MoldeoLabelName;
		MOint m_ConnectorId;
		moText m_ConnectorLabelName;
		MOboolean m_bUpdated;
};

typedef moConnector *moConnectorPtr;
moDeclareExportedDynamicArray (moConnector *, moConnectors)
/// Coneccin, vnculo entre dos objetos
/**
 *  Vnculo especfico desde un conector moConnector a uno o ms conectores
 *
 */
class LIBMOLDEO_API moConnection:public moAbstract
{
	public:
		/**
		 * Constructor
		 */
		moConnection ();

		/**
		 * Constructor
		 * @param p_DestinationMoldeoLabelName el nombre nico del objeto al que est coneccin se vincular
		 * @param p_DestinationConnectorLabelName el nombre nico del conector del objeto al que est coneccin se vincular
		 * @param p_DestinationMoldeoId el id nico del objeto al que est coneccin se vincular
		 * @param p_DestinationConnectorId el id nico del conector del objeto al que est coneccin se vincular
		 */
		moConnection (moText p_DestinationMoldeoLabelName,
			moText p_DestinationConnectorLabelName,
			MOint p_DestinationMoldeoId =
			-1, MOint p_DestinationConnectorId = -1);

		/**
		 * Desctructor
		 */
		virtual ~ moConnection ();

		/**
		 * Devuelve el id nico del objeto al que est coneccin est destinada
		 */
		MOint GetDestinationMoldeoId ();

		/**
		 * Devuelve el nombre nico del objeto al que est coneccin est destinada
		 */
		moText GetDestinationMoldeoLabelName ();

		/**
		 * Devuelve el id nico del conector al que est coneccin est destinada
		 */
		MOint GetDestinationConnectorId ();

		/**
		 * Devuelve el nombre nico del conector al que est coneccin est destinada
		 */
		moText GetDestinationConnectorLabelName ();

		/**
		 * Fija el id nico del objeto al que est coneccin est destinada
		 */
		void SetDestinationMoldeoId (MOint p_DestinationMoldeoId);

		/**
		 * Fija el nombre nico del objeto al que est coneccin est destinada
		 */
		void SetDestinationMoldeoLabelName (moText p_DestinationMoldeoLabelName);

		/**
		 * Fija el id nico del conector al que est coneccin est destinada
		 */
		void SetDestinationConnectorId (MOint p_DestinationConnectorId);

		/**
		 * Fija el nombre nico del conector al que est coneccin est destinada
		 */
		void SetDestinationConnectorLabelName (moText
			p_DestinationConnectorLabelName);

		/*
		   virtual void Send();//ask to connector to receive data...
		   virtual void OnSend();
		 */
	protected:
		//SOURCE: not really necessary
		moText m_SourceMoldeoLabelName;
		MOint m_SourceMoldeoId;
		moText m_SourceConnectorLabelName;
		MOint m_SourceConnectorId;

		//DESTINATION
		moText m_DestinationMoldeoLabelName;
		MOint m_DestinationMoldeoId;
		moText m_DestinationConnectorLabelName;
		MOint m_DestinationConnectorId;
};

typedef moConnection *moConnectionPtr;
moDeclareExportedDynamicArray (moConnectionPtr, moConnections)
/// Conector Inlet, conector que recibe datos
/**
 *  Recibe datos desde otro conector. El conector que enva es un Outlet (moOutlet)
 *
 * @see moConnector
 * @see moConnection
 * @see moOutlet
 */
class LIBMOLDEO_API moInlet:public moConnector
{
	public:
		/**
		 * Constructor
		 */
		moInlet ();

		/**
		 * Destructor
		 */
		virtual ~ moInlet ();
};

moDeclareExportedDynamicArray (moInlet *, moInlets)
/// Conector Outlet, conector que enva datos
/**
 *  Enva datos a uno o ms conectores. El conector que recibe es un Intlet (moInlet)
 *
 *  Comunica los datos internos para cada coneccin de forma independiente
 *  genera el evento/mensaje poblando la lista de eventos (moEventList) con el nuevo dato y el MoldeoID correspondiente a cada coneccin
 *
 * @see moConnector
 * @see moConnection
 * @see moInlet
 */
class LIBMOLDEO_API moOutlet:public moConnector
{
	public:
		/**
		 * Constructor
		 */
		moOutlet ();

		/**
		 * Destrcutor
		 */
		virtual ~ moOutlet ();

		/**
		 * Finalizador
		 */
		virtual MOboolean Finish ();

		/**
		 * conecta el outlet a un conector inlet agregando una coneccin a la lista de conecciones del outlet
		 */
		virtual bool Connect (moInlet * p_connector);

		/**
		 * desconecta el outlet de un conector inlet eliminando la coneccin de la lista de conecciones del outlet
		 */
		virtual void Disconnect (moInlet * p_connector);

		/// Devuelve las conecciones de un outlet
		/**
		 *   Es una lista de conecciones que puede ser enumerada
		 */
		moConnections * GetConnections ();
	private:
		/**
		 * Conecciones
		 */
		moConnections m_Connections;
};

moDeclareExportedDynamicArray (moOutlet *, moOutlets)
#endif							 /*  */
