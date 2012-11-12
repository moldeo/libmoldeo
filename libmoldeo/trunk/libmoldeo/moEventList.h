/*******************************************************************************

                                moEventList.h

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

#include "moValue.h"
#include "moLock.h"

#ifndef __MOEVENTO_H
#define __MOEVENTO_H

/// Estructura base de un evento
/**
* Estructura base de un evento
*/
struct LIBMOLDEO_API moEventStruct
{
	MOint valid;
	MOint deviceid;
	MOint devicecode;
	MOint reservedvalue0;
	MOint reservedvalue1;
	MOint reservedvalue2;
	MOint reservedvalue3;
};

/// Clase Evento
/**
* Clase Evento
*/
class LIBMOLDEO_API moEvent {
public:

	moEvent *previous;
	moEvent *next;

	MOint deviceid; //esto vuela
	MOint devicecode;//esto puede q quede
	MOint reservedvalue0;
	MOint reservedvalue1;
	MOint reservedvalue2;
	MOint reservedvalue3;
	MOpointer pointer;

    /// Constructor
	moEvent();
	/// Constructor
	moEvent(MOint did, MOint cod, MOint val0 /**/= 0, MOint val1 /**/= 0, MOint val2 /**/= 0, MOint val3 /**/= 0, MOpointer ptr/**/=0);
	/// Constructor
	moEvent(MOint did, MOint cod, MOint val0, MOpointer ptr );
	/// Destructor
	virtual ~moEvent();

};

#define MO_MESSAGE 0xFABC05
/// Clase Mensaje
/**
* Clase Mensaje
* Esta clase deriva de un Evento adquiriendo además tanto un emisor como un receptor
* identificados ambos en el contexto de el administrador general de objetos moConsole y los identificadores
* individuales únicos de los objetos creados
*/
class LIBMOLDEO_API moMessage : public moEvent {

	public:
        moMessage();
        moMessage(const moMessage&);
		moMessage( MOint m_MoldeoIdDest, MOint m_MoldeoIdSrc, const moData& data );
		moMessage( MOint m_MoldeoIdDest, MOint m_InletIdDest, MOint m_MoldeoIdSrc, const moData& data );
		moMessage( MOint p_MoldeoIdDest, MOint m_InletIdDest, moMoldeoObjectType p_TypeDest, MOint p_MoldeoIdSrc, moMoldeoObjectType p_TypeSrc, const moData& p_data );
		moMessage( MOint p_MoldeoIdDest, MOint m_InletIdDest, moMoldeoObjectType p_TypeDest, moText	p_NameDest, MOint p_MoldeoIdSrc, moMoldeoObjectType p_TypeSrc, moText p_NameSrc, const moData& p_data );
		virtual ~moMessage();

		moData					m_Data;

		MOint					m_MoldeoIdDest;
		MOint					m_InletIdDest;
		moMoldeoObjectType		m_TypeDest;
		moText					m_NameDest;

		MOint					m_MoldeoIdSrc;
		MOint					m_OutletIdSrc;
		moMoldeoObjectType		m_TypeSrc;
		moText					m_NameSrc;

};

/// Lista de eventos
/**
* Lista de eventos
* Esta lista es creada por el moIODeviceManager, el administrador de dispositivos de entrada/salida de Moldeo
* se crea un único moIODeviceManager por cada instancia de moConsole creado
* Esta lista de eventos funciona de forma sincrónica con el ciclo de dibujado y está protegida de todas maneras para ser utilizada de forma asinc´ronica
* a través de semáforos
*/
class LIBMOLDEO_API moEventList : public moAbstract {

public:

	moLock	m_lock;

	moEvent *First;
	moEvent *Last;

	moEventList();
	virtual ~moEventList();


	MOboolean Init();
	void Add( moMessage* p_Message );
	void Add( moEvent* p_Event );
	void Add(MOint did, MOint cod, MOint val0 /**/= 0, MOint val1 /**/= 0, MOint val2 /**/= 0, MOint val3 /**/= 0, MOpointer ptr/**/=0);
	void Add(MOint did, MOint cod, MOint val0, MOpointer ptr );
	MOboolean Delete(moEvent *ev);
	MOboolean Delete(moMessage *ev);
	MOboolean Finish();

};

// moEventPacket class **************************************************
/// Paquete de eventos
/**
* Paquete de eventos
*   creado para poder empaquetar una cantidad de eventos determinada, y ser enviados o procesados simultaneamente,
*   se implementa para la conección via TCP/UDP entre dos estaciones y agiliazar/acelerar la comunicación entre estas.
*
*/
class LIBMOLDEO_API moEventPacket : public moAbstract
{
public:
	moEventPacket(float p_sendInterval, int p_maxEventNum);
    ~moEventPacket();

	void ClearPacket();
	bool AddEvent(moEvent *e);
	bool ReadyToSend();

	int GetNumEvents() { return num_events; }
	int GetPacketSize() { return num_events * sizeof(moEventStruct); }
	moEventStruct* GetPacket() { return buffer_events; }
	moEventStruct GetEvent(int i) { return buffer_events[i]; }
private:
	float sendInterval;
	int maxEventNum;

    float time0;

    moEventStruct* buffer_events;
	void* pointer;
	moEventStruct empty_event;
	int num_events;
	bool packet_full;
};

moDeclareExportedDynamicArray( moEventPacket*, moEventPacketArray );

#endif
